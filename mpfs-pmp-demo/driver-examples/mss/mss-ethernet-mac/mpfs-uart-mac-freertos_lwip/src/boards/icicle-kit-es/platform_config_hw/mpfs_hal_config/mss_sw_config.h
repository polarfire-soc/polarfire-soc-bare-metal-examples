/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software
 *
 */

/*******************************************************************************
 *
 * Platform definitions
 * Version based on requirements of MPFS MSS
 *
 */
 /*========================================================================*//**
  @mainpage Sample file detailing how mss_sw_config.h should be constructed for
    the MPFS MSS

    @section intro_sec Introduction
    The mss_sw_config.h is to be located in the project
    ./src/platform/config/software/mpfs_hal directory.
    This file must be hand crafted when using the MPFS MSS.


    @section

*//*==========================================================================*/


#ifndef USER_CONFIG_MSS_USER_CONFIG_H_
#define USER_CONFIG_MSS_USER_CONFIG_H_

/*------------------------------------------------------------------------------
 * MPFS_HAL_FIRST_HART and MPFS_HAL_LAST_HART defines used to specify which
 * harts to actually start.
 * Set MPFS_HAL_FIRST_HART to a value other than 0 if you do not want your code
 * to start and execute code on the E51 hart.
 * Set MPFS_HAL_LAST_HART to a value smaller than 4 if you do not wish to use
 * all U54 harts.
 * Harts that are not started will remain in an infinite WFI loop unless used
 * through some other method
 */
#ifndef MPFS_HAL_FIRST_HART
#define MPFS_HAL_FIRST_HART  0
#endif

#ifndef MPFS_HAL_LAST_HART
#define MPFS_HAL_LAST_HART   4
#endif

/*
 * MPFS_APP_LAUNCHED_BY_BOOTLOADER
 * We set MPFS_APP_LAUNCHED_BY_BOOTLOADER = 0 if we are a boot-loader
 * Set MPFS_APP_LAUNCHED_BY_BOOTLOADER = 1 if loaded by a boot loader
 *
 * MPFS_HAL_HW_CONFIG is defined if we are a boot-loader. This is a
 * conditional compile switch is used to determine if MPFS HAL will perform the
 * hardware configurations or not.
 * Defined      => This program acts as a First stage bootloader and performs
 *                 hardware configurations.
 * Not defined  => This program assumes that the hardware configurations are
 *                 already performed (Typically by a previous boot stage)
 *
 * List of items initialised when MPFS_HAL_HW_CONFIG is enabled
 * - load virtual rom (see load_virtual_rom(void) in system_startup.c)
 * - l2 cache config
 * - Bus error unit config
 * - MPU config
 * - pmp config
 * - I/O, clock and clock mux's, DDR and SGMII
 * - will start other harts, see text describing MPFS_HAL_FIRST_HART,
 *   MPFS_HAL_LAST_HART above
 *
 */
#define MPFS_APP_LAUNCHED_BY_BOOTLOADER 0
#if (MPFS_APP_LAUNCHED_BY_BOOTLOADER == 0)
#define MPFS_HAL_HW_CONFIG
#endif

/*
 * If you are using common memory for sharing across harts, make sure it is
 * allocated in the linker script
 * See app_hart_common mem section in the platform linker scripts.
 */
#ifndef MPFS_HAL_SHARED_MEM_ENABLED
//#define MPFS_HAL_SHARED_MEM_ENABLED
#endif

/*------------------------------------------------------------------------------
 * Define the size of the HLS used
 * In our HAL, we are using Hart Local storage for debug data storage only
 * as well as flags for wfi instruction management.
 * The TLS will take memory from top of the stack if allocated
 *
 */
#define HLS_DEBUG_AREA_SIZE     64

/* define the required tick rate in Milliseconds */
/* if this program is running on one hart only, only that particular hart value
 * will be used */
#define HART0_TICK_RATE_MS  5UL
#define HART1_TICK_RATE_MS  5UL
#define HART2_TICK_RATE_MS  5UL
#define HART3_TICK_RATE_MS  5UL
#define HART4_TICK_RATE_MS  5UL

/*
 * define how you want the Bus Error Unit configured
 */
#define BEU_ENABLE                  0x0ULL
#define BEU_PLIC_INT                0x0ULL
#define BEU_LOCAL_INT               0x0ULL

/*
 * Clear memory on startup
 * 0 => do not clear DTIM and L2
 * 1 => Clears memory
 */
#ifndef MPFS_HAL_CLEAR_MEMORY
#define MPFS_HAL_CLEAR_MEMORY  0
#endif

/*
 * If not using item, comment out line below
 */
#define SGMII_SUPPORT
#define DDR_SUPPORT
#define MSSIO_SUPPORT
//#define SIMULATION_TEST_FEEDBACK
//#define E51_ENTER_SLEEP_STATE

/*
 * DDR software options
 */
/*
 * If not using item, comment out line below
 */
//#define SGMII_SUPPORT
//#define DDR_SUPPORT
//#define MSSIO_SUPPORT
//#define SIMULATION_TEST_FEEDBACK
//#define E51_ENTER_SLEEP_STATE

/*
 * The following three setting disable Data Mask and enable Read Write Modify
 * This is required if accessing LPDDR4 with non-cached writes and using
 * MSS Configurator 2021.1 or earlier.
 */
#define LIBERO_SETTING_CFG_DM_EN 0x00000000UL
#define LIBERO_SETTING_CFG_RMW_EN 0x00000001UL
#define LIBERO_SETTING_DDRPHY_MODE 0x00014A24UL

#endif /* USER_CONFIG_MSS_USER_CONFIG_H_ */

