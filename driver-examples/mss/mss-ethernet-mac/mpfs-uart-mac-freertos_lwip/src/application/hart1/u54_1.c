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
#include <math.h>
#include "mpfs_hal/mss_hal.h"
#if ((MPFS_HAL_FIRST_HART == 1) && (MPFS_HAL_LAST_HART == 1)) || ((MPFS_HAL_FIRST_HART == 0) && (MPFS_HAL_LAST_HART == 1))
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/common.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_sw_cfg.h"

/* Set to 1 to use direct mode and set to 0 to use vectored mode.
 *
 * VECTOR MODE=Direct --> all traps into machine mode cause the pc to be set to the
 * vector base address (BASE) in the mtvec register.
 *
 * VECTOR MODE=Vectored --> all synchronous exceptions into machine mode cause the
 * pc to be set to the BASE, whereas interrupts cause the pc to be set to the
 * address BASE plus four times the interrupt cause number.
 *
 * Both the local interrupt and PLIC interrupt versions of this app use the
 * direct mode but you can enable the vectored mode for the local interrupt
 * version to try out the vector mode but only one GEM interrupt is currently
 * supported.
 */
#if ((MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0_LOCAL) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_DISCOVERY_GEM0_LOCAL))
#define mainVECTOR_MODE_DIRECT                1
#else
#define mainVECTOR_MODE_DIRECT                1
#endif

#if ( mainVECTOR_MODE_DIRECT == 1 )
extern void freertos_risc_v_trap_handler( void );
#else
extern void freertos_vector_table_h1( void );
#endif

volatile uint32_t count_sw_ints_h1 = 0U;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
extern volatile uint64_t wait_flag;
#endif

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
#if (IMAGE_LOADED_BY_BOOTLOADER == 1)
/*
 * Enable this as a debug aid to halt the app when running in DDR so you can
 * attach and debug. Manually change icount to something like 5 to cause loop
 * to exit after attaching the debug session.
 */
#if 0
    while(0 == icount)
    {
        icount++;
        icount--;
    }
#endif
#endif

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Wait for U51 to finish all start up code */
    while(wait_flag != 0x987654321UL)
    {
        volatile int dummy;
        dummy++;
    }
#endif

#if ((MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0_LOCAL) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_DISCOVERY_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_DISCOVERY_GEM0_LOCAL))

    /*
     * Enable mac local interrupts to hart 1, U54 1
     */
    SYSREG->FAB_INTEN_MISC = FAB_INTEN_MAC0_U54_1_EN_MASK;
    /*
     * Call free RTOS. Will not return from here
     */
#if ( mainVECTOR_MODE_DIRECT == 1 )
    {
        __asm__ volatile ( "csrw mtvec, %0" : : "r" ( freertos_risc_v_trap_handler ) );
    }
#else
    {
        __asm__ volatile ( "csrw mtvec, %0" : : "r" ( ( uintptr_t ) freertos_vector_table_h1 | 0x1 ) );
    }
#endif
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
