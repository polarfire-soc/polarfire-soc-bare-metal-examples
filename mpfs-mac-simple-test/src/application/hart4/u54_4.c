/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_4
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

volatile uint32_t count_sw_ints_h4 = 0U;
static uint64_t uart4_lock;
static uint8_t g_rx_buff4[5] = {0};

void
u54_4_uart0_rx_handler(mss_uart_instance_t *this_uart)
{
    //    mss_take_mutex((uint64_t)&uart4_lock);
    MSS_UART_get_rx(&g_mss_uart4_lo, g_rx_buff4, sizeof(g_rx_buff4));
    MSS_UART_polled_tx_string(&g_mss_uart4_lo, "hart4 MMUART4 local IRQ.\r\n");
    //   mss_release_mutex((uint64_t)&uart4_lock);
}

/* Main function for the hart4(U54_4 processor).
 * Application code running on hart4 is placed here
 *
 * The hart4 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void
u54_4(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart in WFI */
    do
    {
        __asm("wfi");
    } while (0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();

    //  mss_init_mutex((uint64_t)&uart4_lock);

    MSS_UART_init(&g_mss_uart4_lo, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY);

    MSS_UART_polled_tx_string(&g_mss_uart4_lo, "Hello World from U54_4\r\n");

    MSS_UART_set_rx_handler(&g_mss_uart4_lo, u54_4_uart0_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);

    MSS_UART_enable_local_irq(&g_mss_uart4_lo);

    while (1U)
    {
        icount++;

        if (0x100000U == icount)
        {
            icount = 0U;
            sprintf(info_string, "hart %d\r\n", hartid);
            //       mss_take_mutex((uint64_t)&uart4_lock);
            MSS_UART_polled_tx(&g_mss_uart4_lo, info_string, strlen(info_string));
            //      mss_release_mutex((uint64_t)&uart4_lock);
        }
    }

    /* Never return */
}

/* hart4 software interrupt handler */
void
U54_4_software_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h4++;
}
