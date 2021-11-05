/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
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

/*****************************************************************************/
#define BUFFER_A_SIZE       2048u
#define FLASH_PAGE_LENGTH   2048u

/*****************************************************************************
 * Local read write buffers.
 */
static uint8_t g_rd_buf[10] __attribute__ ((aligned (4)));
static uint8_t g_flash_wr_buf[BUFFER_A_SIZE] __attribute__ ((aligned (4)));
static uint8_t g_flash_rd_buf[BUFFER_A_SIZE] __attribute__ ((aligned (4)));

/*****************************************************************************
 * Local function to verify the write data by reading the read buffers and
 * comparing with write data buffers.
 */
static uint8_t verify_write(uint8_t* write_buff, uint8_t* read_buff,
                            uint32_t size);

/*****************************************************************************
 * MSS QSPI and MSS MMUART instances.
 */
static mss_qspi_io_format g_io_format = MSS_QSPI_NORMAL;
mss_uart_instance_t *g_uart = &g_mss_uart1_lo;

volatile uint32_t xip_read[10] = {0};
uint64_t uart_lock;

/* This function will write data to the QSPI flash, read it back and cross
 * verify the read data with the data that was written. */
static uint8_t flash_mem_readwrite_test(void)
{
    uint8_t* buf_ptr  = 0u;
    uint32_t loop_cnt;

    Flash_erase();

    buf_ptr = &g_flash_wr_buf[0];

    for(loop_cnt = 0u; loop_cnt < (BUFFER_A_SIZE/ FLASH_PAGE_LENGTH); loop_cnt++)
    {
        Flash_program(buf_ptr, (loop_cnt*FLASH_PAGE_LENGTH), FLASH_PAGE_LENGTH);
        buf_ptr += FLASH_PAGE_LENGTH;
    }

    Flash_read(g_flash_rd_buf, 0, BUFFER_A_SIZE);

    return(verify_write(g_flash_wr_buf, g_flash_rd_buf, BUFFER_A_SIZE));
}

/******************************************************************************
 * Main function for the HART1(U54_1 processor).
 * Application code running on HART1 is placed here.
 *****************************************************************************/
void u54_1(void)
{
    uint8_t rx_buff[1];
    uint8_t rx_size = 0u;
    uint32_t loop_count;
    uint8_t error = 0u;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /*Put this hart into WFI.*/
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));
#endif

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART4, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_QSPIXIP, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);



    MSS_UART_init( g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    PLIC_init();
    __enable_irq();

    MSS_UART_init( g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY |
            MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(g_uart,
            "\r\n\r\n\r\n********PolarFire SoC QSPI Example*********\r\n");

    MSS_UART_polled_tx_string(g_uart, "\r\nAccessing on-board Winbond Flash \
memory in NORMAL, DUAL and QUAD formats\r\n");

    PLIC_SetPriority(QSPI_PLIC, 2);
    PLIC_EnableIRQ(QSPI_PLIC);
    MSS_QSPI_enable();

    /* Ideally, this function call is not required. User must know what is the
     * default mode of the flash memory that is being used and configure the
     * QSPI controller accordingly. You may call this function if you are not
     * sure what is the default mode the flash memory is operating in and force
     * it to normal mode.
     *
     * At the end of this function the QSPI controller and flash memory both
     * shall be in the normal mode.*/
    /*    Flash_force_normal_mode(); */

    g_io_format = Flash_probe_io_format();
    MSS_UART_polled_tx_string (g_uart, "\r\n\r\nFlash memory current IO format:\
0 = Normal, 6 = Dual, 7 = Quad\r\n\r\n");

    display_output((uint8_t*)&g_io_format, 1);

    if (MSS_QSPI_NORMAL != g_io_format)
    {
        Flash_force_normal_mode();
        MSS_UART_polled_tx_string (g_uart, "\r\nGot the Flash memory to \
normal mode now.\r\n");
    }

/******************************************************************************
 * QSPI Normal mode operation
 */
    /* Initialize the write and read buffers */
    for(loop_count = 0; loop_count < (BUFFER_A_SIZE); loop_count++)
    {
        g_flash_wr_buf[loop_count] = 0x15 + loop_count;
        g_flash_rd_buf[loop_count] = 0x00;
    }

    MSS_UART_polled_tx_string (g_uart, "\r\nREADID: ");
    Flash_readid(g_rd_buf);
    display_output(g_rd_buf, 3);

    MSS_UART_polled_tx_string (g_uart, "\r\n\r\n MSS QSPI Normal mode demo: \r\n");
    Flash_init(MSS_QSPI_NORMAL);
    MSS_UART_polled_tx_string (g_uart, "\r\nREADID: ");
    Flash_readid(g_rd_buf);
    display_output(g_rd_buf, 3);
    MSS_UART_polled_tx_string (g_uart,
            "\r\nNormal mode IO format read write test\r\n");
    MSS_UART_polled_tx_string (g_uart,
            "READ flash result 0=PASS, 1=FAIL\r\n");

    error = flash_mem_readwrite_test();
    display_output((uint8_t*)&error, 1);
    MSS_UART_polled_tx_string (g_uart, "\r\n\r\nDATA\r\n");
    display_output(g_flash_rd_buf, 10);

/*****************************************************************************
 * MSS QSPI Dual Full mode operation
 */
    /* Initialize the write and read buffers */
    for(loop_count = 0; loop_count < (BUFFER_A_SIZE); loop_count++)
    {
        g_flash_wr_buf[loop_count] = 0x25 + loop_count;
        g_flash_rd_buf[loop_count] = 0x00;
    }

    MSS_UART_polled_tx_string (g_uart,
            "\r\n\r\n\r\n MSS QSPI Dual Full Mode demo: \r\n");
    Flash_init(MSS_QSPI_DUAL_FULL);
    MSS_UART_polled_tx_string (g_uart, "\r\nREADID : ");
    Flash_readid(g_rd_buf);
    display_output(g_rd_buf, 3);
    MSS_UART_polled_tx_string (g_uart,
            "\r\nDual Full Mode read write test\r\n");
    MSS_UART_polled_tx_string (g_uart,
            "READ flash result 0=PASS, 1=FAIL\r\n");
    error = flash_mem_readwrite_test();
    display_output((uint8_t*)&error, 1);
    MSS_UART_polled_tx_string (g_uart, "\r\n\r\nDATA\r\n");
    display_output(g_flash_rd_buf, 10);

  /*****************************************************************************
  * MSS QSPI Quad full mode operation
  */
    /* Initialize the write and read buffers */
    for(loop_count = 0; loop_count < (BUFFER_A_SIZE); loop_count++)
    {
        g_flash_wr_buf[loop_count] = 0x35 + loop_count;
        g_flash_rd_buf[loop_count] = 0x00;
    }

    MSS_UART_polled_tx_string (g_uart,
            "\r\n\r\n\r\n MSS QSPI Quad Full mode demo \r\n");
    Flash_init(MSS_QSPI_QUAD_FULL);
    MSS_UART_polled_tx_string (g_uart, "\r\nREADID : ");
    Flash_readid(g_rd_buf);
    display_output(g_rd_buf, 3);
    MSS_UART_polled_tx_string (g_uart,
            "\r\nQUAD Full IO format read write test\r\n");
    MSS_UART_polled_tx_string (g_uart,
            "\r\nREAD flash result 0=PASS, 1=FAIL\r\n");
    error = flash_mem_readwrite_test();
    display_output((uint8_t*)&error, 1);
    MSS_UART_polled_tx_string (g_uart, "\r\n\r\nDATA\r\n");
    display_output(g_flash_rd_buf, 10);

    MSS_UART_polled_tx_string (g_uart,
            "\r\n\r\n\r\n MSS QSPI Test Complete \r\n");
    while(1)
    {
        rx_size = MSS_UART_get_rx(&g_mss_uart0_lo, rx_buff, sizeof(rx_buff));
        if (rx_size > 0u)
        {
            /* Echo the rx char */
            MSS_UART_polled_tx(&g_mss_uart0_lo, rx_buff, rx_size);
        }
    }
}

/***************************************************************************//**
 * Read the data from SPI flash and compare the same with write buffer.
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

