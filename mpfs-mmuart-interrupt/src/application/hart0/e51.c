/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
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
const uint8_t g_message3[] =
        " \r\n\r\n------------------------------------\
---------------------------------\r\n\r\n\
 Please observe UART1, as application is using UART1 as \
 User-Interface\r\n\r\n--------------------------------\
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

    /* Raise software interrupt to wake hart 1 */

    raise_soft_interrupt(1U);

    __enable_irq();

#endif

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

/* hart0 software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h0++;
}
