/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_2.c
 *
 * @author Microchip FPGA Embedded Systems Solutions
 *
 * @brief Application code running on U54_2
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"

volatile uint32_t count_sw_ints_h2 = 0U;

/* Main function for the hart2(U54_2 processor).
 * Application code running on hart2 is placed here
 *
 * The hart2 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart
 */
void u54_2(void)
{
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this core
     * out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* put this hart into WFI. */
    do
    {
        __asm("wfi");
    } while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Here onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();

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

/* hart2 Software interrupt handler */
void Software_h2_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h2++;
}
