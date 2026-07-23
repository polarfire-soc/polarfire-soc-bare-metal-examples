/*******************************************************************************
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/uart_mapping.h"

volatile uint32_t count_sw_ints_h1 = 0U;
extern struct mss_uart_instance* p_uartmap_u54_1;
const uint8_t g_info_string_h2[] =
        " \r\n\r\n ****U54_1 core is up in DDR-Relase configuration !! ****\r\n";
/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 */

void u54_1(void)
{
    volatile uint32_t icount = 0U;
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART_U54_1,
            (uint8_t) 1U,
            PERIPHERAL_ON);

    MSS_UART_init(p_uartmap_u54_1,
                MSS_UART_115200_BAUD,
                MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_polled_tx_string(p_uartmap_u54_1, g_info_string_h2);

    while (1U)
    {
        icount++;
        if (0x100000U == icount)
        {
            icount = 0U;
        }
    }
    /* never return */
}

/* hart1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    count_sw_ints_h1++;
}
