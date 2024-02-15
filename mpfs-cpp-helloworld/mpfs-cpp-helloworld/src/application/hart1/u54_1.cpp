/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.cpp
 *
 * @author Microchip FPGA Embedded Systems Solutions
 *
 * @brief Application code running on U54_1.
 */

#include "mpfs_hal/mss_hal.h"
#include <iostream>

volatile uint32_t count_sw_ints_h1 = 0U;
using namespace std;

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart
 */
void u54_1(void)
{

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
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
#endif

#if (IMAGE_LOADED_BY_BOOTLOADER == 1)
    cout<<"\r\n\n\n **** PolarFire SoC MSS C++ example ****\n\n\r"<<endl;
#endif

    cout<<"Hello World from u54 core 1 - hart1.\n\r"<<endl;

    /* Raise software interrupt to wake hart 2 */
    raise_soft_interrupt(2U);

    for (;;)
    {

    }
    /* never return */
}

/* hart1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}
