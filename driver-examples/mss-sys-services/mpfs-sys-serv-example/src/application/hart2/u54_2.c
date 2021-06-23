/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * PolarFire SoC MSS System Service example
 *
 * Application code running on U54_2
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"
#include "inc/helper.h"

volatile uint32_t count_sw_ints_h2 = 0U;

extern mss_uart_instance_t *g_uart;

/* Main function for the hart2(U54_2 processor).
 * Application code running on hart2 is placed here
 *
 * The hart2 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart
 */
void u54_2(void)
{
    char info_string[100];
    volatile uint32_t icount = 0U;
    uint64_t hartid = read_csr(mhartid);
    uint32_t pattern_offset = 12U;
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
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

    __enable_irq();

    sprintf(info_string, "\r\nHart %u, HLS mem address 0x%lx, Shared mem 0x%lx\r\n",\
                                                          hls->my_hart_id, (uint64_t)hls, (uint64_t)hls->shared_mem);
    spinlock(&hart_share->mutex_uart0);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
    spinunlock(&hart_share->mutex_uart0);

    while (1U)
    {
        icount++;

        if (0x100000U == icount)
        {
            icount = 0U;
            sprintf(info_string,"Hart %d\r\n", hartid);
            spinlock(&hart_share->mutex_uart0);
            MSS_UART_polled_tx(&g_mss_uart0_lo, info_string, strlen(info_string));
            spinunlock(&hart_share->mutex_uart0);
        }
    }
    /* never return */
}

/* hart2 Software interrupt handler */
void Software_h2_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h2++;
}
