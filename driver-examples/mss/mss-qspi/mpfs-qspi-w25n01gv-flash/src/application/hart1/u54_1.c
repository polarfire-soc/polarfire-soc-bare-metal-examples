/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * MSS QSPI driver example
 * Please refer README.md in the root folder of this example project
 */

#include "mpfs_hal/mss_hal.h"
#include "drivers/off_chip/winbond_w25n01gv/winbond_w25n01gv.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_qspi/mss_qspi.h"
#include "inc/helper.h"

#define FLASH_PAGE_LENGTH       2048u

#define FLASH_MEMORY_SIZE       0x8000000       /*128MBytes*/
#define FLASH_TEST_PAGES        1024u

static uint8_t rd_buf[10] __attribute__ ((aligned (4)));
static uint8_t g_flash_wr_buf[FLASH_PAGE_LENGTH] __attribute__ ((aligned (4)));
static uint8_t g_flash_rd_buf[FLASH_PAGE_LENGTH] __attribute__ ((aligned (4)));
static uint8_t verify_write(uint8_t* write_buff, uint8_t* read_buff,
                            uint32_t size);

w25_bb_lut_entry_t w25_bb_lut[20];
uint16_t bad_blocks[1024u] = {0};
uint32_t num_bad_blocks = 0u;

mss_uart_instance_t *g_uart = &g_mss_uart1_lo;
uint64_t uart_lock;
uint8_t g_ui_buf[500];
const uint8_t g_greeting_msg[] =
"\r\n\n\n **** PolarFire SoC MSS QSPI - Winbond W25N01GV flash example ****\n\n\r";

const uint8_t g_menu_msg[] =
"\r\n\n\nSelect option: \r\n\
 b - Scan for bad blocks \r\n\
     ( This scan will find the bad blocks marked at factory. \r\n\
       Use on devices which was never erased/programmed.) \r\n\n\
 r - Read page    ( see the code for page number(s) ) \r\n\
 p - program page ( see the code for page number(s) )\r\n\
 l - Read Bad block LUT \r\n\
 t - Perform Write-Read test \r\n\
 e - Erase Flash \r\n\
 s - Read status registers \r\n\
 i - Read id \r\n\
";

/* This function will write data to the QSPI flash, read it back and cross
 * verify the read data with the data that was written. */
static inline uint8_t flash_mem_writeread_test(size_t num_pages)
{
    volatile uint64_t timediff = 0u;
    volatile uint64_t starttime = 0u;
    uint8_t error = 0u;
    uint8_t status = 0xFFu;
    uint32_t page_num;

    starttime = readmtime();
    MSS_UART_polled_tx_string (g_uart, "\r\nPerforming test, may take few minutes...");

    if (Flash_erase())
    {
        error = 0xFFu;
        MSS_UART_polled_tx_string (g_uart, "\r\nErase failed");
        return(error);
    }

    for (page_num = 0; page_num < FLASH_TEST_PAGES; page_num++)
    {
        status = Flash_program(&g_flash_wr_buf[0], (page_num * FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
        if (0u != status)
        {
            error = 0xFE;
            MSS_UART_polled_tx_string (g_uart, "\r\nProgramming failed at Page number - ");
            display_output((uint8_t*)&page_num, 1);
            break;
        }

        Flash_read(g_flash_rd_buf, (page_num * FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
        error = verify_write(g_flash_wr_buf, g_flash_rd_buf, FLASH_PAGE_LENGTH);
        if (error)
        {
            MSS_UART_polled_tx_string (g_uart, "\r\nFailed at Page number - ");
            display_output((uint8_t*)&page_num, 1);
        }

        if (!(page_num & 0x7F))
            MSS_UART_polled_tx_string (g_uart, ".");
    }

    timediff = readmtime() - starttime;
    sprintf((char*)&g_ui_buf[0], "\r\n\r\nMachine cycles taken by the test - 0x%x", timediff);
    MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

    return(error);
}

/* Main function for the HART1(U54_1 processor).
 * Application code running on HART1 is placed here.
 */
void u54_1(void)
{
    int8_t info_string[100];
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;
    uint32_t loop_count;
    uint8_t error=0;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt In case of external,
     * bootloader not present
     */

    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /*Put this hart into WFI.*/

    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();
#endif

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_QSPIXIP, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    MSS_UART_init( g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    PLIC_init();
    __enable_irq();

   PLIC_SetPriority(QSPI_PLIC,2);
   PLIC_EnableIRQ(QSPI_PLIC);

    /* Initialize the write and read buffers */
    for (loop_count = 0; loop_count < FLASH_PAGE_LENGTH; loop_count++)
    {
        g_flash_wr_buf[loop_count] = 0x1 + loop_count;
        g_flash_rd_buf[loop_count] = 0x00;
    }

    Flash_init(MSS_QSPI_NORMAL);

    MSS_UART_polled_tx_string(g_uart, g_greeting_msg);

    MSS_UART_polled_tx_string(g_uart, g_menu_msg);

    while (1)
    {
        rx_size = MSS_UART_get_rx(g_uart, rx_buff, sizeof(rx_buff));

        if (rx_size > 0u)
        {
            /* Echo the rx char */

            sprintf((char*)&g_ui_buf[0], "\r\nselected: %c    ", rx_buff[0]);
            MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

            rx_size = 0u;

            switch (rx_buff[0])
            {
            case 'b':
                MSS_UART_polled_tx_string (g_uart, "\r\nscanning...\r\n");
                num_bad_blocks = Flash_scan_for_bad_blocks(&bad_blocks[0]);

                if (num_bad_blocks > 0)
                {
                    MSS_UART_polled_tx_string (g_uart, "\r\nBlock numbers which are marked as BAD\r\n");

                    for (uint32_t cnt=0; cnt < num_bad_blocks; cnt++)
                    {
                        sprintf((char*)&g_ui_buf[0], "\r\n%d    ", bad_blocks[cnt]);
                        MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                    }
                }

                sprintf((char*)&g_ui_buf[0], "\r\nNumber of bad blocks returned by BB scan function = %d \r\n", num_bad_blocks);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                break;

            case 'l':
                Flash_read_bb_lut(w25_bb_lut);
                MSS_UART_polled_tx_string (g_uart, "\r\nReading Bad block LUT. Non-Zero values indicate an LUT entry\r\n");

                for(uint8_t cnt=0; cnt < 20; cnt++) /*Maximum 20 entries*/
                {
                    sprintf((char*)&g_ui_buf[0], "\r\n\r\n lba = %8x    pba = %8x", w25_bb_lut[cnt].lba, w25_bb_lut[cnt].pba);
                    MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                }
                break;

            case 'e':
                g_flash_rd_buf[0] = Flash_erase();
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nErase operation complete. status = %x\r\n", g_flash_rd_buf[0]);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                break;

            case 's':
                g_flash_rd_buf[0] = 0u;
                g_flash_rd_buf[1] = 0u;
                g_flash_rd_buf[2] = 0u;
                Flash_read_status_regs(g_flash_rd_buf);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nStatus Reg1 = %x Status Reg2 = %x   StatusReg 3 = %x\r\n",
                                              g_flash_rd_buf[0], g_flash_rd_buf[1], g_flash_rd_buf[2]);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                break;

            case 'r':
                Flash_read(g_flash_rd_buf, (0*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nPage num = %d\r\n", 0);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                display_output(g_flash_rd_buf, FLASH_PAGE_LENGTH);

                Flash_read(g_flash_rd_buf, (1*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nPage num = %d\r\n", 1);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                display_output(g_flash_rd_buf, FLASH_PAGE_LENGTH);
                break;

            case 'p':
                Flash_program(&g_flash_wr_buf[0], (0*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nWriting Page num = %d\r\n", 0);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

                Flash_program(&g_flash_wr_buf[0], (1*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nWriting Page num = %d\r\n", 1);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                break;

            case 't':
                error = flash_mem_writeread_test((uint32_t)(FLASH_MEMORY_SIZE) / (uint32_t)(FLASH_PAGE_LENGTH));
                MSS_UART_polled_tx_string (g_uart,
                        "\r\nWrite-Read test result 0=PASS, 1=FAIL\r\n");
                display_output((uint8_t*)&error, 1);
                break;

            case 'i':
                Flash_readid(rd_buf);
                display_output(rd_buf, 3);
                break;

            default:
                MSS_UART_polled_tx_string(g_uart, "\r\nInvalid choice\r\n");
                break;
            }

            MSS_UART_polled_tx_string(g_uart, g_menu_msg);
        }
    }
}

/***************************************************************************//**
 * Read the date from SPI flash and compare the same with write buffer.
 */
static uint8_t verify_write(uint8_t* write_buff, uint8_t* read_buff,
        uint32_t size)
{
    uint8_t error = 0u;
    uint32_t index = 0u;

    while (size != 0u)
    {
        if (write_buff[index] != read_buff[index])
        {
            error = 1u;
            sprintf((char*)&g_ui_buf[0], "\r\nExpected:0x%x   Received:0x%x   Index:0x%x\r\n",
                                          write_buff[index],
                                          read_buff[index],
                                          index);
            MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
            break;
        }

        index++;
        size--;
    }

    return error;
}

/* HART1 Software interrupt handler */
void Software_h1_IRQHandler (void)
{

}
