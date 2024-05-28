/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * This example is run from DDR and is best loaded with the MPFS HAL DDR Demo.
 *
 * Both the Icicle Kit and the BeagleV-Fire have 2GB of LPDDR 4 memory with
 * a 32bit interface. The following DDR memory map applies:
 *
 *   ddr_cached_32bit (rwx) :     ORIGIN  = 0x0080000000, LENGTH = 768M
 *   ddr_non_cached_32bit (rwx) : ORIGIN  = 0x00C0000000, LENGTH = 256M
 *   ddr_wcb_32bit (rwx) :        ORIGIN  = 0x00D0000000, LENGTH = 256M
 *   ddr_cached_38bit (rwx) :     ORIGIN  = 0x1000000000, LENGTH = 1024M
 *   ddr_non_cached_38bit (rwx) : ORIGIN  = 0x1400000000, LENGTH = 0k
 *   ddr_wcb_38bit (rwx) :        ORIGIN  = 0x1800000000, LENGTH = 0k
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#if ((MPFS_HAL_FIRST_HART == 1) && (MPFS_HAL_LAST_HART == 1))
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/common.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_sw_cfg.h"

volatile uint32_t count_sw_ints_h1 = 0U;

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void
u54_1(void)
{
    volatile uint32_t icount = 0U;

#if ((MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0_LOCAL) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0))

    /*
     * Enable mac local interrupts to hart 1, U54 1
     */
    SYSREG->FAB_INTEN_MISC = FAB_INTEN_MAC0_U54_1_EN_MASK;
    /*
     * Call free RTOS. Will not return from here
     */
    free_rtos();
#endif
    /*
     * should never get here
     */
    while (1U)
    {
        icount++;
    }

    /* Never return */
}

/* hart1 software interrupt handler */
void
U54_1_software_IRQHandler(void)
{
    count_sw_ints_h1++;
}

#endif /* #if ((MPFS_HAL_FIRST_HART == 1) && (MPFS_HAL_LAST_HART ==1)) */
