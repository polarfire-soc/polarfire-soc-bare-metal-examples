/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
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

/* #define RENODE_DEBUG */

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
#define MPFS_HAL_FIRST_HART  1
#endif

#ifndef MPFS_HAL_LAST_HART
#define MPFS_HAL_LAST_HART   4
#endif

/*------------------------------------------------------------------------------
 * Markers used to indicate startup status of hart
 */
#define HLS_DATA_IN_WFI                     0x12345678U
#define HLS_DATA_PASSED_WFI                 0x87654321U

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

#define H2F_BASE_ADDRESS 0x20126000     /* or 0x28126000 */

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
//#define SGMII_SUPPORT
//#define DDR_SUPPORT
#define MSSIO_SUPPORT
//#define SIMULATION_TEST_FEEDBACK

/*
 * DDR software options
 */
#define DDR_FULL_32BIT_NC_CHECK_EN

#define PATTERN_INCREMENTAL     (0x01U << 0U)
#define PATTERN_WALKING_ONE     (0x01U << 1U)
#define PATTERN_WALKING_ZERO    (0x01U << 2U)
#define PATTERN_RANDOM          (0x01U << 3U)
#define PATTERN_0xCCCCCCCC      (0x01U << 4U)
#define PATTERN_0x55555555      (0x01U << 5U)
#define PATTERN_ZEROS           (0x01U << 6U)
#define MAX_NO_PATTERNS         7U
/* number of test writes to perform */
#define SW_CFG_NUM_READS_WRITES        0x20000U
/*
 * what test patterns to write/read on start-up
 * */
#define SW_CONFIG_PATTERN (PATTERN_INCREMENTAL|\
                                        PATTERN_WALKING_ONE|\
                                        PATTERN_WALKING_ZERO|\
                                        PATTERN_RANDOM|\
                                        PATTERN_0xCCCCCCCC|\
                                        PATTERN_0x55555555)
/* Training types status offsets */
#define BCLK_SCLK_BIT                   (0x1U<<0U)
#define ADDCMD_BIT                      (0x1U<<1U)
#define WRLVL_BIT                       (0x1U<<2U)
#define RDGATE_BIT                      (0x1U<<3U)
#define DQ_DQS_BIT                      (0x1U<<4U)
/*  The first five bits represent the currently supported training in the TIP */
/*  This value will not change unless more training possibilities are added to
 *  the TIP */
#define TRAINING_MASK                   (BCLK_SCLK_BIT|\
                                        ADDCMD_BIT|\
                                        WRLVL_BIT|\
                                        RDGATE_BIT|\
                                        DQ_DQS_BIT)
/*
 * Debug DDR startup through a UART
 * Comment out in normal operation. May be useful for debug purposes in bring-up
 * of a new board design.
 * See the weak function setup_ddr_debug_port(mss_uart_instance_t * uart)
 * If you need to edit this function, make a copy of of the function without the
 * weak declaration in your application code.
 * */
//#define DEBUG_DDR_INIT


/*
 * You can over write any on the settings coming from Libero here
 *
 * e.g. Define how you want SEG registers configured, if you want to change from
 * the default settings
 */

#define LIBERO_SETTING_SEG0_0     (-(0x0080000000LL >> 24u))
#define LIBERO_SETTING_SEG0_1     (-(0x1000000000LL >> 24u))
#define LIBERO_SETTING_SEG1_2     (-(0x00C0000000LL >> 24u))
#define LIBERO_SETTING_SEG1_3     (-(0x1400000000LL >> 24u))
#define LIBERO_SETTING_SEG1_4     (-(0x00D0000000LL >> 24u))
#define LIBERO_SETTING_SEG1_5     (-(0x1800000000LL >> 24u))

/*
 * todo: verify with Alister if these need to added to Libero
 */
#define LIBERO_SETTING_MAX_ADDRESS_CMD_OFFSET   7
#define LIBERO_SETTING_MIN_ADDRESS_CMD_OFFSET   2

/*
 * Will review address settings in Libero, tie in, sanity check with SEG
 * settings
 */
#define LIBERO_SETTING_DDR_32_NON_CACHE 0xC0000000ULL

#endif /* USER_CONFIG_MSS_USER_CONFIG_H_ */

