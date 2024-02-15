/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_3.c
 *
 * @author Microchip FPGA Embedded Systems Solutions
 *
 * @brief Application code running on U54_3
 */
#include <stdio.h>
#include "mpfs_hal/mss_hal.h"

volatile uint32_t count_sw_ints_h3 = 0U;

/* Main function for the hart3(U54_3 processor).
 * Application code running on hart3 is placed here
 *
 * The hart3 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart
 */
void u54_3(void)
{

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

    printf("Hello World from u54 core 3 - hart3.\n\r\n");

    /* Raise software interrupt to wake hart 4 */
    raise_soft_interrupt(4U);
    for (;;)
    {

    }
    /* never return */
}

/* hart3 Software interrupt handler */
void Software_h3_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h3++;
}
