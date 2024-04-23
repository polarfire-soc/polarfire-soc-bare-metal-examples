/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on U54_1
 *
 * PolarFire SoC CoreQSPI example project
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/off_chip/micron_mt25q/micron_mt25q.h"
#include "inc/helper.h"

/*-------------------------------------------------------------------------*//**
 * CoreQSPI base address
 */

#define     COREQSPI_BASE_ADDRESS               0x60020000u

#define FLASH_MEMORY_SIZE                       0x2000000 /* (32MBytes Micron N25Q256A)*/
#define FLASH_PAGE_LENGTH                       256u

static uint8_t rd_buf[10] __attribute__ ((aligned (4)));
static uint8_t g_flash_wr_buf[FLASH_PAGE_LENGTH] __attribute__ ((aligned (4)));
static uint8_t g_flash_rd_buf[FLASH_PAGE_LENGTH] __attribute__ ((aligned (4)));

uint8_t g_ui_buf[500];

static uint8_t verify_write(uint8_t* write_buff, uint8_t* read_buff,
                            uint32_t size);

mss_uart_instance_t *g_uart = &g_mss_uart1_lo;

/******************************************************************************
 *  Greeting messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
     "\r\n\r\n\t  ******* PolarFire SoC CoreQSPI Example *******\r\n\n\
     \r\n\r\n\t  ******* Accessing external QSPI micron flash memory in NORMAL,\
DUAL and QUAD formats *******\n\n\n\r";

const uint8_t g_menu_msg[] =
"\r\n\n\nSelect option: \r\n\
 i - Read Device JEDEC ID \r\n\
 e - Erase Flash \r\n\
 p - Program Page(s) ( see the code for page number(s) )\r\n\
 r - Read Page(s)    ( see the code for page number(s) ) \r\n\
 t - Perform Write-Read Test \r\n\
 s - Read Status Register \r\n\
 d - Select Dual SPI IO \r\n\
 q - Select Quad SPI IO \r\n\
 n - Select Normal Mode \r\n";

/* This function will write data to the QSPI flash, read it back and cross
 * verify the read data with the data that was written. */
static uint8_t flash_mem_writeread_test(uint32_t num_pages)
{
    volatile uint64_t timediff = 0u;
    volatile uint64_t start_time = 0u;
    uint8_t error = 0u;
    uint8_t status = 0xFFu;

    start_time = readmtime();
    sprintf((char*)&g_ui_buf[0], "\r\nPerforming test on first 0x%x pages, may take few minutes...", num_pages);
    MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

    if (Flash_erase())
       error = 0xFF;

    for(uint32_t page_num = 0; page_num < 1024; page_num++)
    {
        status = Flash_program(&g_flash_wr_buf[0], (page_num * FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
        if (0u != status)
        {
            error = 0xFE;
            break;
        }

        Flash_read(g_flash_rd_buf, (page_num*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);

        error = verify_write(g_flash_wr_buf, g_flash_rd_buf, FLASH_PAGE_LENGTH);
        if (error)
        {
            sprintf((char*)&g_ui_buf[0], "\r\n\r\nfailed at Page number - %d", page_num);
            MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
            break;
        }

        if (!(page_num & 0x7F))
            MSS_UART_polled_tx_string (g_uart, ".");
    }

    timediff = readmtime() - start_time;
    sprintf((char*)&g_ui_buf[0], "\r\n\r\nMachine cycles taken by the test - 0x%x", timediff);
    MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

    return(error);
}

/* Main function for the hart1(U54 processor).
 * Application code running on hart1 is placed here.
 */

void u54_1(void)
{
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

    /* Reset the peripherals turn on the clocks */

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART,
                                                           PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC0, (uint8_t) MPFS_HAL_FIRST_HART,
                                                        PERIPHERAL_ON);

    PLIC_init();
    mss_enable_fabric();

    /* Make sure that the CoreQSPI interrupt is routed to PLIC Fabric—MSS
     * interrupt(F2M) interface.
     *
     * The TXAVAILABLE, RXAVAILABLE, TXFIFOFULL, RXFIFOEMPTY signals. Allows an
     * interrupt be generated. */

    PLIC_SetPriority(FABRIC_F2H_6_PLIC,2);
    PLIC_EnableIRQ(FABRIC_F2H_6_PLIC);
    __enable_irq();

    /* Initialize the write and read buffers */
    for(loop_count = 0; loop_count < (FLASH_PAGE_LENGTH); loop_count++)
    {
        g_flash_wr_buf[loop_count] = (0x15 + loop_count);
        g_flash_rd_buf[loop_count] = 0x00;
    }

    MSS_UART_init( g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string( g_uart, g_greeting_msg );

    MSS_UART_polled_tx_string( g_uart, "\r\n\n\nUsing Normal IO format\r\n");
    Flash_init(QSPI_NORMAL, COREQSPI_BASE_ADDRESS);

    MSS_UART_polled_tx_string( g_uart, "\r\nDevice ID:\r\n" );
    Flash_readid(rd_buf);
    display_output(rd_buf, 3);

    MSS_UART_polled_tx_string( g_uart, g_menu_msg);

    while (1)
    {
        rx_size = MSS_UART_get_rx( g_uart, rx_buff, sizeof(rx_buff));

        if (rx_size > 0u)
        {
            /* Initialize read buffers */
            for(loop_count = 0; loop_count < (FLASH_PAGE_LENGTH); loop_count++)
            {
                g_flash_rd_buf[loop_count] = 0x00;
            }

            /* Echo the rx char */
            sprintf((char*)&g_ui_buf[0], "\r\nselected: %c    ", rx_buff[0]);
            MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

            rx_size = 0u;

            switch(rx_buff[0])
            {
            case 'e':
                 g_flash_rd_buf[0] = Flash_erase();
                 sprintf((char*)&g_ui_buf[0], "\r\n\r\nErase operation complete. status = %x\r\n", g_flash_rd_buf[0]);
                 MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                 break;

            case 'p':
                Flash_program(&g_flash_wr_buf[0], (0*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nWriting Page num = %d\r\n", 1023);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

                Flash_program(&g_flash_wr_buf[0], (1024*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nWriting Page num = %d\r\n", 1024);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);

                MSS_UART_polled_tx_string (g_uart, (uint8_t*)"\r\nProgramming completed\r\n");
                break;

            case 'r':
                Flash_read(g_flash_rd_buf, (0*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nPage num = %d\r\n", 0);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                display_output(g_flash_rd_buf, FLASH_PAGE_LENGTH);

                Flash_read(g_flash_rd_buf, (1024*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
                sprintf((char*)&g_ui_buf[0], "\r\n\r\nPage num = %d\r\n", 1024);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                display_output(g_flash_rd_buf, FLASH_PAGE_LENGTH);
                break;

            case 't':
                error = flash_mem_writeread_test(FLASH_MEMORY_SIZE / FLASH_PAGE_LENGTH);
                MSS_UART_polled_tx_string (g_uart,
                        "\r\nWrite-Read test result 0=PASS, 1=FAIL\r\n");
                display_output((uint8_t*)&error, 1);
                MSS_UART_polled_tx_string (g_uart,
                        "\r\nWR_DATA\r\n");
                display_output(g_flash_wr_buf, FLASH_PAGE_LENGTH);
                MSS_UART_polled_tx_string (g_uart,
                        "\r\nRD_DATA\r\n");
                display_output(g_flash_rd_buf, FLASH_PAGE_LENGTH);
                break;

            case 'i':
                Flash_readid(rd_buf);
                MSS_UART_polled_tx_string (g_uart, "\r\nDevice ID : \r\n");
                display_output(rd_buf, 3);
                break;

            case 's':
                g_flash_rd_buf[0] = 0u;
                Flash_read_statusreg(g_flash_rd_buf, 0);

                sprintf((char*)&g_ui_buf[0], "\r\n\r\nStatus Reg1 = %x \r\n",
                                              g_flash_rd_buf[0]);
                MSS_UART_polled_tx_string (g_uart, (uint8_t*)g_ui_buf);
                break;

            case 'q':
                MSS_UART_polled_tx_string (g_uart, "\r\nQuad IO configured\r\n");
                Flash_init(QSPI_QUAD_FULL, COREQSPI_BASE_ADDRESS);
                break;

            case 'd':
                MSS_UART_polled_tx_string (g_uart, "\r\nDual IO configured\r\n");
                Flash_init(QSPI_DUAL_FULL, COREQSPI_BASE_ADDRESS);
                break;

            case 'n':
                MSS_UART_polled_tx_string (g_uart, "\r\nNormal IO configured\r\n");
                Flash_init(QSPI_NORMAL, COREQSPI_BASE_ADDRESS);
                break;

            default:
                MSS_UART_polled_tx_string(g_uart, "\r\nInvalid choice\r\n");
                break;
            }

            MSS_UART_polled_tx_string(g_uart, g_menu_msg);
            rx_size = 0u;
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
