/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
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

#ifndef SIFIVE_HIFIVE_UNLEASHED
#include "drivers/mss/mss_mmuart/mss_uart.h"
#else
#include "drivers/FU540_uart/FU540_uart.h"
#endif

volatile uint32_t count_sw_ints_h3 = 0U;


/* Main function for the HART3(U54_3 processor).
 * Application code running on HART3 is placed here
 *
 * The HART3 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_3(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;
#endif

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /*put this hart into WFI.*/
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
       can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    spinlock(&hart_share->mutex_uart0);
    MSS_UART_polled_tx_string(&g_mss_uart0_lo,
            "Hello World from u54 core 3 - hart3.\r\n");
    spinunlock(&hart_share->mutex_uart0);
#endif

    while (1U)
    {
        icount++;
        if (0x100000U == icount)
        {
            icount = 0U;
            sprintf(info_string,"This is a message from HartID: %d, \r\n", hartid);
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
            spinlock(&hart_share->mutex_uart0);
            MSS_UART_polled_tx(&g_mss_uart0_lo, info_string, strlen(info_string));
            spinunlock(&hart_share->mutex_uart0);
#endif
        }
    }
    /* never return */
}

/* HART3 Software interrupt handler */
void Software_h3_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h3++;
}
