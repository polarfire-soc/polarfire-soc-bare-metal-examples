/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solutions.
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
    The mss_sw_config.h has the default software configuration settings for the
    MPFS HAL and will be located at
    <Project-Name>/src/platform/platform_config_reference folder of the bare
    metal SoftConsole project. The platform_config_reference is provided as a
    default reference configuration.
    When you want to configure the MPFS HAL with required configuration for
    your project, the mss_sw_config.h must be edited and be placed in the
    following project directory:
    <Project-Name>/src/boards/<your-board>/platform_config/mpfs_hal_config/

    @section

*//*==========================================================================*/


#ifndef MSS_SW_CONFIG_H_
#define MSS_SW_CONFIG_H_

/*
 * Settings for the Coremark project
 */

/* choose which core you want to test, set in the compiler preprocessor
 * configuration */
// #define TEST_CORE_E51
// #define TEST_CORE_U54_1
// #define TEST_CORE_U54_2
// #define TEST_CORE_U54_3
// #define TEST_CORE_U54_4

/* Choose memory you want to use , set in the compiler preprocessor
 * configuration  */
// #define COREMARK_RUN_FROM_LIM
// #define COREMARK_RUN_FROM_SCRATCHPAD
// #define COREMARK_RUN_FROM_ITIM
// #define COREMARK_RUN_FROM_DTIM
// #define COREMARK_RUN_FROM_DDR

#ifdef TEST_CORE_U54_1
#define CM_INFO_STRING "Core under test:U54_1"
#endif

#ifdef TEST_CORE_U54_2
#define CM_INFO_STRING "Core under test:U54_2"
#endif

#ifdef TEST_CORE_U54_3
#define CM_INFO_STRING "Core under test:U54_3"
#endif

#ifdef TEST_CORE_U54_4
#define CM_INFO_STRING "Core under test:U54_4"
#endif

/*
 * string showing memory used in Coremark test
 */
#ifdef  COREMARK_RUN_FROM_SCRATCHPAD
#define CM_MEM_INFO_STRING "stack:SCRATCHPAD"
#endif
#ifdef  COREMARK_RUN_FROM_LIM
#define CM_MEM_INFO_STRING "stack:LIM"
#endif
#ifdef COREMARK_RUN_FROM_ITIM
#define CM_MEM_INFO_STRING "stack:ITIM"
#endif
#ifdef COREMARK_RUN_FROM_DTIM
#define CM_MEM_INFO_STRING "stack:DTIM"
#endif
#ifdef COREMARK_RUN_FROM_DDR
#define CM_MEM_INFO_STRING "stack:DDR"
#endif

/*
 * MPFS_HAL_FIRST_HART and MPFS_HAL_LAST_HART defines are used to specify which
 * harts to actually start. The value and the actual hart it represents are
 * listed below:
 * value  hart
 *    0  E51
 *    1  U54_1
 *    2  U54_2
 *    3  U54_3
 *    4  U54_4
 * Set MPFS_HAL_FIRST_HART to a value greater than 0 if you do not want your
 * application to start and execute code on the harts represented by smaller
 * value numbers.
 * Set MPFS_HAL_LAST_HART to a value smaller than 4 if you do not wish to use
 * all U54_x harts.
 * Harts that are not started will remain in an infinite WFI loop unless used
 * through some other method.
 * The value of MPFS_HAL_FIRST_HART must always be less than MPFS_HAL_LAST_HART.
 * The value of MPFS_HAL_LAST_HART must never be greater than 4.
 * A typical use-case where you set MPFS_HAL_FIRST_HART = 1 and
 * MPFS_HAL_LAST_HART = 1 is when
 * your application is running on U54_1 and a bootloader running on E51 loads
 * your application to the target memory and kicks-off U54_1 to run it.
 */
#ifndef COREMARK_RUN_FROM_DDR

#ifndef MPFS_HAL_FIRST_HART
#define MPFS_HAL_FIRST_HART  0
#endif

#ifndef MPFS_HAL_LAST_HART
#define MPFS_HAL_LAST_HART   4

#endif
#else


#ifndef MPFS_HAL_FIRST_HART
#define MPFS_HAL_FIRST_HART  1
#endif

#ifndef MPFS_HAL_LAST_HART
#define MPFS_HAL_LAST_HART   1
#endif


#endif


/*
 * IMAGE_LOADED_BY_BOOTLOADER
 * We set IMAGE_LOADED_BY_BOOTLOADER = 0 if the application image runs from
 * non-volatile memory after reset. (No previous stage bootloader is used.)
 * Set IMAGE_LOADED_BY_BOOTLOADER = 1 if the application image is loaded by a
 * previous stage bootloader.
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
#ifndef COREMARK_RUN_FROM_DDR
#define IMAGE_LOADED_BY_BOOTLOADER 0
#ifndef MPFS_HAL_HW_CONFIG
#define MPFS_HAL_HW_CONFIG
#endif
#else
#define IMAGE_LOADED_BY_BOOTLOADER 1
#endif


/*
 * If you are using common memory for sharing across harts,
 * uncomment #define MPFS_HAL_SHARED_MEM_ENABLED
 * make sure common memory is allocated in the linker script
 * See app_hart_common mem section in the example platform
 * linker scripts.
 */

//#define MPFS_HAL_SHARED_MEM_ENABLED


/* define the required tick rate in Milliseconds */
/* if this program is running on one hart only, only that particular hart value
 * will be used */
#define HART0_TICK_RATE_MS  5UL
#define HART1_TICK_RATE_MS  5UL
#define HART2_TICK_RATE_MS  5UL
#define HART3_TICK_RATE_MS  5UL
#define HART4_TICK_RATE_MS  5UL

/*
 * Define the size of the Hart Local Storage (HLS).
 * In the MPFS HAL, we are using HLS for debug data storage during the initial
 * boot phase.
 * This includes the flags which indicate the hart state regarding boot state.
 * The HLS will take memory from top of each stack allocated at boot time.
 *
 */
#define HLS_DEBUG_AREA_SIZE     64

/*
 * Bus Error Unit (BEU) configurations
 * BEU_ENABLE => Configures the events that the BEU can report. bit value
 *               1= enabled, 0 = disabled.
 * BEU_PLIC_INT => Configures which accrued events should generate an
 *                 interrupt to the PLIC.
 * BEU_LOCAL_INT => Configures which accrued events should generate a
 *                 local interrupt to the hart on which the event accrued.
 */
#define BEU_ENABLE                  0x0ULL
#define BEU_PLIC_INT                0x0ULL
#define BEU_LOCAL_INT               0x0ULL

/*
 * Clear memory on startup
 * 0 => do not clear DTIM and L2
 * 1 => Clears memory
 * Note: If you are the zero stage bootloader, set this to one.
 */
#ifndef MPFS_HAL_CLEAR_MEMORY
#define MPFS_HAL_CLEAR_MEMORY  1
#endif

/*
 * We sometimes want to know which board we are compiling
 * enable define for particular board you are using if you are using this switch
 * in your application code
 */
//#define DDR_BASE_BOARD

/*
 * Comment out the lines to disable the corresponding hardware support not required
 * in your application.
 * This is not necessary from an operational point of view as operation dictated
 * by MSS configurator settings, and items are enabled/disabled by this method.
 * The reason you may want to use below is to save code space.
 */
#define SGMII_SUPPORT
#define DDR_SUPPORT
#define MSSIO_SUPPORT

/*
 * Uncomment MICROCHIP_STDIO_THRU_MMUARTx to enable stdio port
 * Note: you must have mss_mmuart driver source code included in the project.
 */
//#define MICROCHIP_STDIO_THRU_MMUARTX    &g_mss_uart0_lo
//#define MICROCHIP_STDIO_BAUD_RATE       MSS_UART_115200_BAUD

/*
 * The hardware configuration settings imported from Libero project get generated
 * into <project_name>/src/boards/<your-board>/<fpga-design-config> folder.
 * If you need to overwrite them for testing purposes, you can do so here.
 * e.g. If you want change the default SEG registers configuration defined by
 * LIBERO_SETTING_SEG0_0, define it here and it will take precedence.
 * #define LIBERO_SETTING_SEG0_0 0x80007F80UL
 *
 */

/*
 * DDR software options
 */

/*
 * Debug DDR startup through a UART
 * Comment out in normal operation. Useful for debug purposes in bring-up of DDR
 * in a new board design.
 * See the weak function setup_ddr_debug_port(mss_uart_instance_t * uart)
 * If you need to edit this function, make a copy of the function without the
 * weak declaration in your application code.
 * */
//#define DEBUG_DDR_INIT
//#define DEBUG_DDR_RD_RW_FAIL
//#define DEBUG_DDR_RD_RW_PASS
//#define DEBUG_DDR_CFG_DDR_SGMII_PHY
//#define DEBUG_DDR_DDRCFG

/*
 * skip extra error checking on startup
 * Set to 0xF to skip extra checking at end of training
 * */
// #define LIBERO_FAST_START                           0x0FU

/*
 * To lower training time adjust LIBERO_SETTING_CFG_DFI_T_PHY_WRLAT value so
 * wr calib result is 00001111
 *
 * */
// #define LIBERO_SETTING_CFG_DFI_T_PHY_WRLAT          0x00000007UL

#endif /* USER_CONFIG_MSS_USER_CONFIG_H_ */


