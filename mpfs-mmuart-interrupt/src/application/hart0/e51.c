/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on e51
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
volatile uint32_t count_sw_ints_h0 = 0U;

/* Main function for the hart0(e51 processor).
 * Application code running on hart0 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt.
 */

#define RX_BUFF_SIZE    16U
uint8_t g_rx_buff0[RX_BUFF_SIZE] = {0};
uint8_t rx_size0 = 0U;

const uint8_t g_message3[] =
        " \r\n\r\n-------------------------------------------------------------\
--------\r\n\r\nPress 1 to demonstrate how to use local interrupts\r\n\r\n\
Press 2 to demonstrate how to use external interrupts\r\n\r\n------------------\
---------------------------------------------------\r\n";

const uint8_t g_message7[] =
        " \r\n\r\n------------------------------------\
---------------------------------\r\n\r\n\
Please observe UART1 for a demonstration of local interrupt\
\r\n\r\n--------------------------------\
-------------------------------------\r\n";

const uint8_t g_message8[] =
        " \r\n\r\n------------------------------------\
---------------------------------\r\n\r\n\
Please observe UART2 for a demonstration of external interrupt\
\r\n\r\n--------------------------------\
-------------------------------------\r\n";

void e51(void)
{
    volatile uint32_t icount = 0U;
    uint64_t hartid = read_csr(mhartid);

    (void) mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) 1, PERIPHERAL_ON);

    MSS_UART_init(&g_mss_uart0_lo,
    MSS_UART_115200_BAUD,
    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* Message on uart0 */
    MSS_UART_polled_tx(&g_mss_uart0_lo, g_message3,
            sizeof(g_message3));

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /* Clear pending software interrupt in case there was any. */

    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    while (1U)
    {
        rx_size0 = MSS_UART_get_rx(&g_mss_uart0_lo, g_rx_buff0, sizeof(g_rx_buff0));
        if(rx_size0 > 0)
        {
            switch (g_rx_buff0[0u])
            {
            case '1':
                /* Raise software interrupt to wake hart 1 */
                raise_soft_interrupt(1U);
                MSS_UART_polled_tx(&g_mss_uart0_lo, g_message7,
                            sizeof(g_message7));
                __enable_irq();
                break;
            case '2':
                /* Raise software interrupt to wake hart 2 */
                raise_soft_interrupt(2U);
                MSS_UART_polled_tx(&g_mss_uart0_lo, g_message8,
                            sizeof(g_message8));
                __enable_irq();
                break;
            default:
                MSS_UART_polled_tx(&g_mss_uart0_lo, g_message3,
                        sizeof(g_message3));
                break;

            }
        }
    }

#endif

    /* never return */
}

/* hart0 software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h0++;
}
