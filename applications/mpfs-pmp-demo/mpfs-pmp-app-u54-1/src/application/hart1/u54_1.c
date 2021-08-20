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
 * Code running on U54 first hart
 *
 */
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"

static volatile uint32_t count_sw_ints_h1 = 0U;
mss_uart_instance_t *g_uart = &g_mss_uart1_lo ;
volatile long mutex_uart1;

#define S_4MB    0x00400000
#define S_2MB    0x00200000
#define S_1MB    0x00100000

const uint8_t g_message_u54_1[] =
        "\r\n\
U54_1 is running this program from LPDDR4.\r\n\
";

const uint8_t g_message_u54_1_menu[] =
        "\r\n\
Choose one of the following options:\r\n\
Type 0 - Show this menu\r\n\
Type 1 - Test PMP UART peripheral access violation \r\n\
Type 2 - Test PMP memory access violation \r\n\
";

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 */
void u54_1(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U, val=0;
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;
    uint32_t *uart3_base_hi = (uint32_t *)(0x28104000);
    uint32_t *ddr_base_1M = (uint32_t *)(LIBERO_SETTING_DDR_32_CACHE + S_1MB);
    uint32_t *ddr_base_4M = (uint32_t *)(LIBERO_SETTING_DDR_32_CACHE + S_4MB);

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
       can enable and use any interrupts as required */
    clear_soft_interrupt();


    /* Turn on UART1 clock and take out of reset */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    MSS_UART_init(g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    spinlock(&mutex_uart1);
    MSS_UART_polled_tx_string ( g_uart, g_message_u54_1);
    MSS_UART_polled_tx_string ( g_uart, g_message_u54_1_menu);
    spinunlock(&mutex_uart1);

    while (1U)
    {
        spinlock(&mutex_uart1);
        rx_size = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff, (uint32_t)sizeof(rx_buff));
        spinunlock(&mutex_uart1);

        if (rx_size > 0)
        {
            switch(rx_buff[0])
            {
                 case '0':
                     spinlock(&mutex_uart1);
                     MSS_UART_polled_tx_string ( g_uart, g_message_u54_1_menu);
                     spinunlock(&mutex_uart1);
                     break;

                case '1':
                    spinlock(&mutex_uart1);
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nOption 1 is selected to Test PMP UART peripheral access violation.\n\r" );
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nTrying to access UART3 which is accessible to only U54_3 and U54_4.\n\r" );
                    spinunlock(&mutex_uart1);

                    val = *uart3_base_hi;
                    break;

                case '2':
                    spinlock(&mutex_uart1);
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nOption 2 is selected to Test PMP memory access violation.\n\r" );
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\r\nTrying to access LPDDR4 location 0x80400000 which is not accessible to any U54 cores.\n\r" );
                    spinunlock(&mutex_uart1);

                    //0x80100000
                    *ddr_base_1M = 0x5a5a5a5;
                    val = *ddr_base_1M;
                    *ddr_base_1M = val;

                    //0x80400000
                    *ddr_base_4M = 0x5a5a5a5;
                     val = *ddr_base_4M;
                     *ddr_base_4M = val;
                     break;

                default:
                    /* echo input */
                    spinlock(&mutex_uart1);
                    MSS_UART_polled_tx_string(g_uart, rx_buff);
                    spinunlock(&mutex_uart1);
                    break;
                    }
           }
    }
    /* never return */
}

/* hart1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}
