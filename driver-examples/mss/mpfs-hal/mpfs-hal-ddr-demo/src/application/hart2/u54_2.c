/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 */
/*******************************************************************************
 *
 * Code running on U54 second hart
 *
 */
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"

volatile uint32_t count_sw_ints_h2 = 0U;
extern uint64_t hart_jump_ddr;

/* Main function for the HART2(U54_2 processor).
 * Application code running on HART2 is placed here
 *
 * The HART2 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_2(void)
{
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();

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

    while (1U)
    {
        if((hart_jump_ddr == 12U) ||
           (hart_jump_ddr == 123U) ||
           (hart_jump_ddr == 1234U))
        {
            jump_to_application(hls, M_MODE, (uint64_t)0x80000000);
        }
    }
    /* never return */
}

/* HART2 Software interrupt handler */
void Software_h2_IRQHandler(void)
{
    count_sw_ints_h2++;
}
