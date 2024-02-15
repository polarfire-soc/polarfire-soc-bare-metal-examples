/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file e51.cpp
 *
 * @author Microchip FPGA Embedded Systems Solutions
 *
 * @brief Application code running on E51
 *
 * PolarFire SoC MSS  C++ example project
 */

#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"
#include <iostream>

volatile uint32_t count_sw_ints_h0 = 0U;
using namespace std;

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here
 *
 * The hart0 is used in the application for bootup and wakeup the U54_1 when
 * the application is running from LIM/eNVM memory. In case of DDR the
 * bootloader application will perform the booting and start executing this
 * application from U54_1.
 */
void e51(void)
{
    cout<<"\r\n\n\n **** PolarFire SoC MSS C++ example ****\n\n\r"<<endl;
    cout<<"Hello World from e51 core - hart0.\n\r"<<endl;

    /* Raise software interrupt to wake hart 1 */
    raise_soft_interrupt(1U);
    for (;;)
    {

    }
    /* never return */
}

/* hart0 Software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h0++;
}
