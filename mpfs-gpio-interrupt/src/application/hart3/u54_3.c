/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_3
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

volatile uint32_t count_sw_ints_h3 = 0U;

/* Main function for the hart3(U54_3 processor).
 * Application code running on hart3 is placed here
 */

void u54_3(void)
{
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
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Here onwards application
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

/* hart3 Software interrupt handler */

void Software_h3_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h3++;
}
