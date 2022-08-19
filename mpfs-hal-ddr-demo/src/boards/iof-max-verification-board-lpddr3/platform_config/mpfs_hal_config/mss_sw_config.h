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
#ifndef MPFS_HAL_FIRST_HART
#define MPFS_HAL_FIRST_HART  0
#endif

#ifndef MPFS_HAL_LAST_HART
#define MPFS_HAL_LAST_HART   4
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
#define IMAGE_LOADED_BY_BOOTLOADER 0
#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
#define MPFS_HAL_HW_CONFIG
#endif


/*
 * If you are using common memory for sharing across harts,
 * uncomment #define MPFS_HAL_SHARED_MEM_ENABLED
 * make sure common memory is allocated in the linker script
 * See app_hart_common mem section in the example platform
 * linker scripts.
 */

#define MPFS_HAL_SHARED_MEM_ENABLED


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
#define DEBUG_DDR_INIT
#define DEBUG_DDR_RD_RW_FAIL
//#define DEBUG_DDR_RD_RW_PASS
//#define DEBUG_DDR_CFG_DDR_SGMII_PHY
//#define DEBUG_DDR_DDRCFG

#define LIBERO_SETTING_DPC_BITS         0x0004C552UL
//#define LIBERO_SETTING_DPC_BITS         0x00049432UL  // Received from SVG 5/14/2020
//#define LIBERO_SETTING_DDRPHY_MODE      0x00014B24UL
#define LIBERO_SETTING_DDRPHY_MODE      0x00000123UL //org
//#define LIBERO_SETTING_DDRPHY_MODE      0x00004123UL
#define LIBERO_SETTING_DATA_LANES_USED  0x00000004UL //org
//#define LIBERO_SETTING_DATA_LANES_USED  0x00000002UL
#define LIBERO_SETTING_CFG_DQ_WIDTH     0x00000000UL //org
//#define LIBERO_SETTING_CFG_DQ_WIDTH     0x00000001UL

#define LIBERO_SETTING_TIP_CFG_PARAMS    0x07C7E002UL//0x07CFE02AUL org
//#define LIBERO_SETTING_TIP_CFG_PARAMS    0x07C7E011UL//0x07CFE02AUL
    /* ADDCMD_OFFSET                     [0:3]   RW value= 0x2 5*/
    /* BCKLSCLK_OFFSET                   [3:3]   RW value= 0x4 */
    /* WRCALIB_WRITE_COUNT               [6:7]   RW value= 0x0 */
    /* READ_GATE_MIN_READS               [13:8]  RW value= 0x1F */
    /* ADDRCMD_WAIT_COUNT                [22:8]  RW value= 0x1F */

/*
 * over write value from Libero
 */
#define LIBERO_SETTING_CFG_DFI_T_PHY_WRLAT          0x00000005UL //org
//#define LIBERO_SETTING_CFG_DFI_T_PHY_WRLAT          0x00000006UL //org

/*
 * Temporarily over write values from Libero
 */

#define LIBERO_SETTING_RPC_ODT_ADDCMD           1
#define LIBERO_SETTING_RPC_ODT_CLK              1
#define LIBERO_SETTING_RPC_ODT_DQ               2 //6
#define LIBERO_SETTING_RPC_ODT_DQS              1 //2,4,6 (org 1)





/*
 * 0 implies all IP traing's used. This should be the default
 * setting.
 */
#define LIBERO_SETTING_TRAINING_SKIP_SETTING        0x00000002UL //org

//#define LIBERO_SETTING_TRAINING_SKIP_SETTING        0x00000001UL
/*
 * 1 implies sw BCLK_SCK traing carried out before IP training. This should be
 * the default
 * setting.
 */
#define USE_SW_BCLK_SCK_TRAINING                    0x00000001UL
#define SW_TRAING_BCLK_SCLK_OFFSET                  0x00000007UL //org
//#define SW_TRAING_BCLK_SCLK_OFFSET                  0x00000006UL
/* uncomment to turn off fifo tuning. This will allow negative testing on known marginal board (PVT testing) */
//#define TUNE_RPC_166_VALUE 0
//#define DEBUG_DDR_CFG_DDR_SGMII_PHY                   // 0x00000001UL
#define NOT_A_FULL_RETRAIN
/*
 * 0x6DU => setting vref_ca to 40%
 * This (0x6DU) is the default setting.
 * */
#define DDR_MODE_REG_VREF_VALUE       0x6DU //org
//#define DDR_MODE_REG_VREF_VALUE       0x6EU

/*
 * Will review address settings in Libero, tie in, sanity check with SEG
 * settings
 */
#define LIBERO_SETTING_DDR_32_NON_CACHE 0xC0000000ULL


#define LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI1_0    0x3FFFFFFFUL
#define LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI1_1    0x00000000UL
#define LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_0    0x7FFFFFFFUL
#define LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_1    0x00000000UL

//New parameters with LiberoV12.6
//#define LIBERO_SETTING_MSS_PLL_CKMUX    0x000003FDUL
//#define LIBERO_SETTING_SGMII_REFCLKMUX    0x00000005UL
//#define LIBERO_SETTING_CFG_READ_TO_WRITE    0x00000005UL


#define LIBERO_SETTING_DDR_PLL_REF_FB    0x00001900UL
    /* RFDIV                             [8:6]   RW value= 0x19 */
#define LIBERO_SETTING_DDR_SSCG_REG_2    0x0000029AUL // 666 is 1333Mbps
//#define LIBERO_SETTING_DDR_SSCG_REG_2    0x00000215UL // 533 is 1066Mbps
    /* INTIN                             [0:12]  RW value= 0x29A */
#define LIBERO_SETTING_MSS_PLL_FRACN    0x00000001UL
/*#define LIBERO_SETTING_CFG_RFC1    0x0000003CUL
#define LIBERO_SETTING_CFG_RFC2    0x0000003CUL
#define LIBERO_SETTING_CFG_RFC4    0x0000003CUL */
#define LIBERO_SETTING_CFG_CL    0x00000012UL
#define LIBERO_SETTING_CFG_READ_TO_WRITE    0x00000005UL //previous 5

//#define LIBERO_SETTING_CFG_DS    0x00000001UL
//#define LIBERO_SETTING_CFG_MIN_READ_IDLE    0x00000007UL

//#define LIBERO_SETTING_PHY_INDPNDT_TRAINING    0x00000001UL
//#define LIBERO_SETTING_PHY_EYE_TRAIN_DELAY    0x0000002FUL


//#define LIBERO_SETTING_CFG_DATA_MASK    0x00000004UL
//#define LIBERO_SETTING_PHY_DFI_INIT_START    0x00000000UL
#define LIBERO_SETTING_PHY_RESET_CONTROL    0x00008001UL

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

#endif 

