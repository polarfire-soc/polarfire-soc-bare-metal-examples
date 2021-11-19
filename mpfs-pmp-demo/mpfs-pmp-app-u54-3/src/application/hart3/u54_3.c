/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS PMP Embedded Software example
 *
 */
/*******************************************************************************
 *
 * Code running on U54 hart 3
 *
 */
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

volatile uint32_t count_sw_ints_h3 = 0U;
volatile long mutex_uart3;

#define S_4MB    0x00400000
#define S_2MB    0x00200000
#define S_1MB    0x00100000

mss_uart_instance_t *g_uart= &g_mss_uart3_hi;
const uint8_t g_message_u54_3[] =
        "\r\n\
U54_3 is running this program from LPDDR4.\r\n\
";

const uint8_t g_message_u54_3_menu[] =
        "\r\n\
Choose one of the following options:\r\n\
Type 0 - Show this menu\r\n\
Type 1 - Test PMP UART peripheral access violation \r\n\
Type 2 - Test PMP memory access violation \r\n\
";

/* Main function for the hart3(U54_3 processor).
 * Application code running on hart3 is placed here
 */

void u54_3(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U, val=0;
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;
    uint32_t *uart1_base_lo = (uint32_t *)(0x20100000);
    uint32_t *ddr_base_3M = (uint32_t *)(LIBERO_SETTING_DDR_32_CACHE + S_2MB + S_1MB);
    uint32_t *ddr_base_4M = (uint32_t *)(LIBERO_SETTING_DDR_32_CACHE + S_4MB);

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();


    /* Turn on UART3 clock and take out of reset */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    MSS_UART_init( g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    spinlock(&mutex_uart3);
    MSS_UART_polled_tx_string (g_uart, g_message_u54_3 );
    MSS_UART_polled_tx_string (g_uart, g_message_u54_3_menu );
    spinunlock(&mutex_uart3);


    while (1U)
    {
        rx_size = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff, (uint32_t)sizeof(rx_buff));

        if (rx_size > 0)
        {
            switch(rx_buff[0])
            {
                 case '0':
                     spinlock(&mutex_uart3);
                     MSS_UART_polled_tx_string (g_uart, g_message_u54_3_menu );
                     spinunlock(&mutex_uart3);
                     break;

                 case '1':
                    spinlock(&mutex_uart3);
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nOption 1 is selected to Test PMP UART peripheral access violation.\n\r" );
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nTrying to access UART1 which is accessible to only U54_1 and U54_2.\n\r" );
                    spinunlock(&mutex_uart3);

                    val = *uart1_base_lo;
                   break;

                 case '2':
                    spinlock(&mutex_uart3);
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nOption 2 is selected to Test PMP memory access violation.\n\r" );
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nTrying to access LPDDR4 location 0x80400000 which is not accessible to any U54 cores.\n\r" );
                    spinunlock(&mutex_uart3);

                    //0x80300000
                    *ddr_base_3M = 0x5a5a5a5;
                    val = *ddr_base_3M;
                    *ddr_base_3M = val;

                    //0x80400000
                    *ddr_base_4M = 0x5a5a5a5;
                     val = *ddr_base_4M;
                     *ddr_base_4M = val;
                     break;
                default:
                    /* echo input */
                    spinlock(&mutex_uart3);
                    MSS_UART_polled_tx_string(g_uart, rx_buff);
                    spinunlock(&mutex_uart3);
                    break;
                    }
           }
    }
    /* never return */
}

/* hart3 Software interrupt handler */
void Software_h3_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h3++;
}
