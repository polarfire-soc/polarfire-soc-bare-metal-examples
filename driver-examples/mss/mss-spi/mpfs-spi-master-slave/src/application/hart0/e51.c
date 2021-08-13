/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"

volatile uint32_t count_sw_ints_h0 = 0U;

/* Main function for the hart0(e51 processor).
 * Application code running on hart1 is placed here
 */

void e51(void)
{
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any. */

    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /* Raise software interrupt to wake hart 1 */
    raise_soft_interrupt(1U);

#endif

    __enable_irq();

    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the U54_1 core can bring this
     * core out of WFI by raising a software interrupt. */
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

/* hart1 Software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
	count_sw_ints_h0++;
}
