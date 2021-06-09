/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"

volatile uint32_t count_sw_ints_h1 = 0U;

static uint64_t uart1_lock;
static uint8_t g_rx_buff1[5] = {0};

void u54_1_uart0_rx_handler (mss_uart_instance_t * this_uart)
{
    mss_take_mutex((uint64_t)&uart1_lock);
    MSS_UART_get_rx(&g_mss_uart1_lo, g_rx_buff1, sizeof(g_rx_buff1));
    MSS_UART_polled_tx_string(&g_mss_uart1_lo, "hart1 MMUART1 local IRQ.\r\n");
    mss_release_mutex((uint64_t)&uart1_lock);
}

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void u54_1(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart in WFI. */
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();

    mss_init_mutex((uint64_t)&uart1_lock);

    MSS_UART_init(&g_mss_uart1_lo,
                   MSS_UART_115200_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY);

    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                              "Hello World from U54_1\r\n");

    MSS_UART_set_rx_handler(&g_mss_uart1_lo,
                            u54_1_uart0_rx_handler,
                            MSS_UART_FIFO_SINGLE_BYTE);

    MSS_UART_enable_local_irq(&g_mss_uart1_lo);

    while(1U)
    {
        icount++;

        if(0x100000U == icount)
        {
            icount = 0U;
            sprintf(info_string,"hart %d\r\n", hartid);
            mss_take_mutex((uint64_t)&uart1_lock);
            MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,
                               strlen(info_string));
            mss_release_mutex((uint64_t)&uart1_lock);
        }
    }

  /* Never return */
}

/* hart1 software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}
