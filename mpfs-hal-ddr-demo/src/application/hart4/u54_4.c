/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 */
/*******************************************************************************
 *
 * Code running on U54 hart 4
 *
 */
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"

#ifndef SIFIVE_HIFIVE_UNLEASHED
#else
#include "drivers/FU540_uart/FU540_uart.h"
#endif

volatile uint32_t count_sw_ints_h4 = 0U;
extern uint64_t hart_jump_ddr;
extern mss_uart_instance_t *g_uart;

/* Main function for the HART4(U54_4 processor).
 * Application code running on HART4 is placed here
 *
 * The HART4 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_4(void)
{
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();

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
     * can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();

    while (1U)
    {
        if(hart_jump_ddr == 1234U)
        {
            jump_to_application(hls, M_MODE, (uint64_t)0x80000000);
        }
    }
    /* never return */
}

/* HART4 Software interrupt handler */
void Software_h4_IRQHandler(void)
{
    count_sw_ints_h4++;
}
