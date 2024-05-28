/*******************************************************************************
 * Copyright 2019-2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file hw_mssio_mux.h
 * @author Microchip-FPGA Embedded Systems Solutions
 *
 *
 * Note 1: This file should not be edited. If you need to modify a parameter
 * without going through regenerating using the MSS Configurator Libero flow 
 * or editing the associated xml file
 * the following method is recommended: 

 * 1. edit the following file 
 * boards/your_board/platform_config/mpfs_hal_config/mss_sw_config.h

 * 2. define the value you want to override there.
 * (Note: There is a commented example in the platform directory)

 * Note 2: The definition in mss_sw_config.h takes precedence, as
 * mss_sw_config.h is included prior to the generated header files located in
 * boards/your_board/fpga_design_config
 *
 */

#ifndef HW_MSSIO_MUX_H_
#define HW_MSSIO_MUX_H_


#ifdef __cplusplus
extern  "C" {
#endif

#if !defined (LIBERO_SETTING_IOMUX0_CR)
/*Selects whether the peripheral is connected to the Fabric or IOMUX structure.
*/
#define LIBERO_SETTING_IOMUX0_CR    0x00002F07UL
    /* SPI0_FABRIC                       [0:1]   RW value= 0x1 */
    /* SPI1_FABRIC                       [1:1]   RW value= 0x1 */
    /* I2C0_FABRIC                       [2:1]   RW value= 0x1 */
    /* I2C1_FABRIC                       [3:1]   RW value= 0x0 */
    /* CAN0_FABRIC                       [4:1]   RW value= 0x0 */
    /* CAN1_FABRIC                       [5:1]   RW value= 0x0 */
    /* QSPI_FABRIC                       [6:1]   RW value= 0x0 */
    /* MMUART0_FABRIC                    [7:1]   RW value= 0x0 */
    /* MMUART1_FABRIC                    [8:1]   RW value= 0x1 */
    /* MMUART2_FABRIC                    [9:1]   RW value= 0x1 */
    /* MMUART3_FABRIC                    [10:1]  RW value= 0x1 */
    /* MMUART4_FABRIC                    [11:1]  RW value= 0x1 */
    /* MDIO0_FABRIC                      [12:1]  RW value= 0x0 */
    /* MDIO1_FABRIC                      [13:1]  RW value= 0x1 */
#endif
#if !defined (LIBERO_SETTING_IOMUX1_CR)
/*Configures the IO Mux structure for each IO pad. 0 implies SD/SDIO, 1 implies
EMMC, 2 implies QSPI, 3 implies SPI,4 implies USB,5 implies MMUART,6 implies
I2C,7 implies CAN,8 implies MDIO,9 implies Miscellaneous,0xA implies Reserved
(Equivalent to Tristate),0xB implies GPIO ,0xC implies Fabric-test,0xD implies
Logic 0,0xE implies Logic 1, 0xF implies Tristate */
#define LIBERO_SETTING_IOMUX1_CR    0x11111111UL
    /* PAD0                              [0:4]   RW value= 0x1 */
    /* PAD1                              [4:4]   RW value= 0x1 */
    /* PAD2                              [8:4]   RW value= 0x1 */
    /* PAD3                              [12:4]  RW value= 0x1 */
    /* PAD4                              [16:4]  RW value= 0x1 */
    /* PAD5                              [20:4]  RW value= 0x1 */
    /* PAD6                              [24:4]  RW value= 0x1 */
    /* PAD7                              [28:4]  RW value= 0x1 */
#endif
#if !defined (LIBERO_SETTING_IOMUX2_CR)
/*Configures the IO Mux structure for each IO pad. 0 implies SD/SDIO, 1 implies
EMMC, 2 implies QSPI, 3 implies SPI,4 implies USB,5 implies MMUART,6 implies
I2C,7 implies CAN,8 implies MDIO,9 implies Miscellaneous,0xA implies Reserved
(Equivalent to Tristate),0xB implies GPIO ,0xC implies Fabric-test,0xD implies
Logic 0,0xE implies Logic 1, 0xF implies Tristate */
#define LIBERO_SETTING_IOMUX2_CR    0x00BB1111UL
    /* PAD8                              [0:4]   RW value= 0x1 */
    /* PAD9                              [4:4]   RW value= 0x1 */
    /* PAD10                             [8:4]   RW value= 0x1 */
    /* PAD11                             [12:4]  RW value= 0x1 */
    /* PAD12                             [16:4]  RW value= 0xB */
    /* PAD13                             [20:4]  RW value= 0xB */
#endif
#if !defined (LIBERO_SETTING_IOMUX3_CR)
/*Configures the IO Mux structure for each IO pad. 0 implies SD/SDIO, 1 implies
EMMC, 2 implies QSPI, 3 implies SPI,4 implies USB,5 implies MMUART,6 implies
I2C,7 implies CAN,8 implies MDIO,9 implies Miscellaneous,0xA implies Reserved
(Equivalent to Tristate),0xB implies GPIO ,0xC implies Fabric-test,0xD implies
Logic 0,0xE implies Logic 1, 0xF implies Tristate */
#define LIBERO_SETTING_IOMUX3_CR    0x44444444UL
    /* PAD14                             [0:4]   RW value= 0x4 */
    /* PAD15                             [4:4]   RW value= 0x4 */
    /* PAD16                             [8:4]   RW value= 0x4 */
    /* PAD17                             [12:4]  RW value= 0x4 */
    /* PAD18                             [16:4]  RW value= 0x4 */
    /* PAD19                             [20:4]  RW value= 0x4 */
    /* PAD20                             [24:4]  RW value= 0x4 */
    /* PAD21                             [28:4]  RW value= 0x4 */
#endif
#if !defined (LIBERO_SETTING_IOMUX4_CR)
/*Configures the IO Mux structure for each IO pad. 0 implies SD/SDIO, 1 implies
EMMC, 2 implies QSPI, 3 implies SPI,4 implies USB,5 implies MMUART,6 implies
I2C,7 implies CAN,8 implies MDIO,9 implies Miscellaneous,0xA implies Reserved
(Equivalent to Tristate),0xB implies GPIO ,0xC implies Fabric-test,0xD implies
Logic 0,0xE implies Logic 1, 0xF implies Tristate */
#define LIBERO_SETTING_IOMUX4_CR    0x55664444UL
    /* PAD22                             [0:4]   RW value= 0x4 */
    /* PAD23                             [4:4]   RW value= 0x4 */
    /* PAD24                             [8:4]   RW value= 0x4 */
    /* PAD25                             [12:4]  RW value= 0x4 */
    /* PAD26                             [16:4]  RW value= 0x6 */
    /* PAD27                             [20:4]  RW value= 0x6 */
    /* PAD28                             [24:4]  RW value= 0x5 */
    /* PAD29                             [28:4]  RW value= 0x5 */
#endif
#if !defined (LIBERO_SETTING_IOMUX5_CR)
/*Configures the IO Mux structure for each IO pad. 0 implies SD/SDIO, 1 implies
EMMC, 2 implies QSPI, 3 implies SPI,4 implies USB,5 implies MMUART,6 implies
I2C,7 implies CAN,8 implies MDIO,9 implies Miscellaneous,0xA implies Reserved
(Equivalent to Tristate),0xB implies GPIO ,0xC implies Fabric-test,0xD implies
Logic 0,0xE implies Logic 1, 0xF implies Tristate */
#define LIBERO_SETTING_IOMUX5_CR    0xB88B2222UL
    /* PAD30                             [0:4]   RW value= 0x2 */
    /* PAD31                             [4:4]   RW value= 0x2 */
    /* PAD32                             [8:4]   RW value= 0x2 */
    /* PAD33                             [12:4]  RW value= 0x2 */
    /* PAD34                             [16:4]  RW value= 0xB */
    /* PAD35                             [20:4]  RW value= 0x8 */
    /* PAD36                             [24:4]  RW value= 0x8 */
    /* PAD37                             [28:4]  RW value= 0xB */
#endif
#if !defined (LIBERO_SETTING_IOMUX6_CR)
/*Sets whether the MMC/SD Voltage select lines are inverted on entry to the
IOMUX structure */
#define LIBERO_SETTING_IOMUX6_CR    0x00000000UL
    /* VLT_SEL                           [0:1]   RW value= 0x0 */
    /* VLT_EN                            [1:1]   RW value= 0x0 */
    /* VLT_CMD_DIR                       [2:1]   RW value= 0x0 */
    /* VLT_DIR_0                         [3:1]   RW value= 0x0 */
    /* VLT_DIR_1_3                       [4:1]   RW value= 0x0 */
    /* SD_LED                            [5:1]   RW value= 0x0 */
    /* SD_VOLT_0                         [6:1]   RW value= 0x0 */
    /* SD_VOLT_1                         [7:1]   RW value= 0x0 */
    /* SD_VOLT_2                         [8:1]   RW value= 0x0 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_CFG_CR)
/*Configures the MSSIO block using SCB write */
#define LIBERO_SETTING_MSSIO_BANK4_CFG_CR    0x00040A0DUL
    /* BANK_PCODE                        [0:6]   RW value= 0xD */
    /* RESERVED0                         [6:2]   RW value= 0x00 */
    /* BANK_NCODE                        [8:6]   RW value= 0xA */
    /* RESERVED1                         [14:2]  RW value= 0x0 */
    /* VS                                [16:4]  RW value= 0x4 */
    /* RESERVED2                         [20:12] RW value= 0x0 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_IO_CFG_0_1_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK4_IO_CFG_0_1_CR    0x09300930UL
    /* IO_CFG_0                          [0:16]  RW value= 0x0930 */
    /* IO_CFG_1                          [16:16] RW value= 0x0930 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_IO_CFG_2_3_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK4_IO_CFG_2_3_CR    0x09300930UL
    /* IO_CFG_2                          [0:16]  RW value= 0x0930 */
    /* IO_CFG_3                          [16:16] RW value= 0x0930 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_IO_CFG_4_5_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK4_IO_CFG_4_5_CR    0x09300930UL
    /* IO_CFG_4                          [0:16]  RW value= 0x0930 */
    /* IO_CFG_5                          [16:16] RW value= 0x0930 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_IO_CFG_6_7_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK4_IO_CFG_6_7_CR    0x09300928UL
    /* IO_CFG_6                          [0:16]  RW value= 0x0928 */
    /* IO_CFG_7                          [16:16] RW value= 0x0930 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_IO_CFG_8_9_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK4_IO_CFG_8_9_CR    0x09300930UL
    /* IO_CFG_8                          [0:16]  RW value= 0x0930 */
    /* IO_CFG_9                          [16:16] RW value= 0x0930 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_IO_CFG_10_11_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK4_IO_CFG_10_11_CR    0x09300930UL
    /* IO_CFG_10                         [0:16]  RW value= 0x0930 */
    /* IO_CFG_11                         [16:16] RW value= 0x0930 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK4_IO_CFG_12_13_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK4_IO_CFG_12_13_CR    0x09280930UL
    /* IO_CFG_12                         [0:16]  RW value= 0x0930 */
    /* IO_CFG_13                         [16:16] RW value= 0x0928 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_CFG_CR)
/*Configures the MSSIO block using SCB write */
#define LIBERO_SETTING_MSSIO_BANK2_CFG_CR    0x00080907UL
    /* BANK_PCODE                        [0:6]   RW value= 0x7 */
    /* RESERVED0                         [6:2]   RW value= 0x00 */
    /* BANK_NCODE                        [8:6]   RW value= 0x9 */
    /* RESERVED1                         [14:2]  RW value= 0x0 */
    /* VS                                [16:4]  RW value= 0x8 */
    /* RESERVED2                         [20:12] RW value= 0x0 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_0_1_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_0_1_CR    0x08290829UL
    /* IO_CFG_0                          [0:16]  RW value= 0x0829 */
    /* IO_CFG_1                          [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_2_3_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_2_3_CR    0x08290829UL
    /* IO_CFG_2                          [0:16]  RW value= 0x0829 */
    /* IO_CFG_3                          [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_4_5_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_4_5_CR    0x08290829UL
    /* IO_CFG_4                          [0:16]  RW value= 0x0829 */
    /* IO_CFG_5                          [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_6_7_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_6_7_CR    0x08290829UL
    /* IO_CFG_6                          [0:16]  RW value= 0x0829 */
    /* IO_CFG_7                          [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_8_9_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_8_9_CR    0x08290829UL
    /* IO_CFG_8                          [0:16]  RW value= 0x0829 */
    /* IO_CFG_9                          [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_10_11_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_10_11_CR    0x08290829UL
    /* IO_CFG_10                         [0:16]  RW value= 0x0829 */
    /* IO_CFG_11                         [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_12_13_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_12_13_CR    0x08290829UL
    /* IO_CFG_12                         [0:16]  RW value= 0x0829 */
    /* IO_CFG_13                         [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_14_15_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_14_15_CR    0x08290829UL
    /* IO_CFG_14                         [0:16]  RW value= 0x0829 */
    /* IO_CFG_15                         [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_16_17_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_16_17_CR    0x08290829UL
    /* IO_CFG_16                         [0:16]  RW value= 0x0829 */
    /* IO_CFG_17                         [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_18_19_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_18_19_CR    0x08290829UL
    /* IO_CFG_18                         [0:16]  RW value= 0x0829 */
    /* IO_CFG_19                         [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_20_21_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_20_21_CR    0x08290829UL
    /* IO_CFG_20                         [0:16]  RW value= 0x0829 */
    /* IO_CFG_21                         [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_BANK2_IO_CFG_22_23_CR)
/*IO electrical configuration for MSSIO pad */
#define LIBERO_SETTING_MSSIO_BANK2_IO_CFG_22_23_CR    0x08290829UL
    /* IO_CFG_22                         [0:16]  RW value= 0x0829 */
    /* IO_CFG_23                         [16:16] RW value= 0x0829 */
#endif
#if !defined (LIBERO_SETTING_MSSIO_VB2_CFG)
/*default dpc values for MSSIO bank 2 */
#define LIBERO_SETTING_MSSIO_VB2_CFG    0x00000828UL
    /* DPC_IO_CFG_IBUFMD_0               [0:1]   RW value= 0x0 */
    /* DPC_IO_CFG_IBUFMD_1               [1:1]   RW value= 0x0 */
    /* DPC_IO_CFG_IBUFMD_2               [2:1]   RW value= 0x0 */
    /* DPC_IO_CFG_DRV_0                  [3:1]   RW value= 0x1 */
    /* DPC_IO_CFG_DRV_1                  [4:1]   RW value= 0x0 */
    /* DPC_IO_CFG_DRV_2                  [5:1]   RW value= 0x1 */
    /* DPC_IO_CFG_DRV_3                  [6:1]   RW value= 0x0 */
    /* DPC_IO_CFG_CLAMP                  [7:1]   RW value= 0x0 */
    /* DPC_IO_CFG_ENHYST                 [8:1]   RW value= 0x0 */
    /* DPC_IO_CFG_LOCKDN_EN              [9:1]   RW value= 0x0 */
    /* DPC_IO_CFG_WPD                    [10:1]  RW value= 0x0 */
    /* DPC_IO_CFG_WPU                    [11:1]  RW value= 0x1 */
    /* DPC_IO_CFG_ATP_EN                 [12:1]  RW value= 0x0 */
    /* DPC_IO_CFG_LP_PERSIST_EN          [13:1]  RW value= 0x0 */
    /* DPC_IO_CFG_LP_BYPASS_EN           [14:1]  RW value= 0x0 */
    /* RESERVED                          [15:17] R */
#endif
#if !defined (LIBERO_SETTING_MSSIO_VB4_CFG)
/*default dpc values for MSSIO bank 4 */
#define LIBERO_SETTING_MSSIO_VB4_CFG    0x00000828UL
    /* DPC_IO_CFG_IBUFMD_0               [0:1]   RW value= 0x0 */
    /* DPC_IO_CFG_IBUFMD_1               [1:1]   RW value= 0x0 */
    /* DPC_IO_CFG_IBUFMD_2               [2:1]   RW value= 0x0 */
    /* DPC_IO_CFG_DRV_0                  [3:1]   RW value= 0x1 */
    /* DPC_IO_CFG_DRV_1                  [4:1]   RW value= 0x0 */
    /* DPC_IO_CFG_DRV_2                  [5:1]   RW value= 0x1 */
    /* DPC_IO_CFG_DRV_3                  [6:1]   RW value= 0x0 */
    /* DPC_IO_CFG_CLAMP                  [7:1]   RW value= 0x0 */
    /* DPC_IO_CFG_ENHYST                 [8:1]   RW value= 0x0 */
    /* DPC_IO_CFG_LOCKDN_EN              [9:1]   RW value= 0x0 */
    /* DPC_IO_CFG_WPD                    [10:1]  RW value= 0x0 */
    /* DPC_IO_CFG_WPU                    [11:1]  RW value= 0x1 */
    /* DPC_IO_CFG_ATP_EN                 [12:1]  RW value= 0x0 */
    /* DPC_IO_CFG_LP_PERSIST_EN          [13:1]  RW value= 0x0 */
    /* DPC_IO_CFG_LP_BYPASS_EN           [14:1]  RW value= 0x0 */
    /* RESERVED                          [15:17] R */
#endif
#if !defined (LIBERO_SETTING_MSSIO_CONFIGURATION_OPTIONS)
/*Indicates if eMMC is configured for use (bit 0 == 1), If SD is configued for
use (bit 1 == 1). Bit 2 indicates which one should be used by default on MSS
embedded software startup ( bit2 == 0, implies default is eMMC, bit2 == 1,
implies default is SD). The eMMC configuration is always defined in xml tag
(io_mux, the SD configuration is always defined in xml tag (io_mux_alt). All
other elements in the (o_mux) and (io_mux_alt) not releating to eMMC/SD
differences should be the same values. */
#define LIBERO_SETTING_MSSIO_CONFIGURATION_OPTIONS    0x00000001UL
    /* EMMC_CONFIGURED                   [0:1]   RW value= 0x1 */
    /* SD_CONFIGURED                     [1:1]   RW value= 0x0 */
    /* DEFAULT_ON_START                  [2:1]   RW value= 0x0 */
#endif
#if !defined (LIBERO_SETTING_GPIO_INTERRUPT_FAB_CR)
/*To limit the number of interrupts fed to the PLINT, the seventy GPIO
interrupts (GPIO0=14, GPIO1=24, GPIO2=32) are reduced down to 41 interrupts by
OR'ing some together. There is some flexibility regarding which interrupts are
OR'd or are direct. This selection is controlled by a 32-bit system
register(GPIO_INTERRUPT_FAB_CR). For example, if bit 0 of the register is set
to 1, gpio2_0 is chosen as a direct interrupt on the PLIC and gpio0_0 will be
OR'd with any other non-direct gpio0 interrupts. Please see the GPIO driver for
more details on using GPIO interrupts. */
#define LIBERO_SETTING_GPIO_INTERRUPT_FAB_CR    0x00000000UL
    /* GPIO0_0_OR_GPIO2_0                [0:1]   RW value= 0x0 */
    /* GPIO0_1_OR_GPIO2_1                [1:1]   RW value= 0x0 */
    /* GPIO0_2_OR_GPIO2_2                [2:1]   RW value= 0x0 */
    /* GPIO0_3_OR_GPIO2_3                [3:1]   RW value= 0x0 */
    /* GPIO0_4_OR_GPIO2_4                [4:1]   RW value= 0x0 */
    /* GPIO0_5_OR_GPIO2_5                [5:1]   RW value= 0x0 */
    /* GPIO0_6_OR_GPIO2_6                [6:1]   RW value= 0x0 */
    /* GPIO0_7_OR_GPIO2_7                [7:1]   RW value= 0x0 */
    /* GPIO0_8_OR_GPIO2_8                [8:1]   RW value= 0x0 */
    /* GPIO0_9_OR_GPIO2_9                [9:1]   RW value= 0x0 */
    /* GPIO0_10_OR_GPIO2_10              [10:1]  RW value= 0x0 */
    /* GPIO0_11_OR_GPIO2_11              [11:1]  RW value= 0x0 */
    /* GPIO0_12_OR_GPIO2_12              [12:1]  RW value= 0x0 */
    /* GPIO0_13_OR_GPIO2_13              [13:1]  RW value= 0x0 */
    /* GPIO1_0_OR_GPIO2_14               [14:1]  RW value= 0x0 */
    /* GPIO1_1_OR_GPIO2_15               [15:1]  RW value= 0x0 */
    /* GPIO1_2_OR_GPIO2_16               [16:1]  RW value= 0x0 */
    /* GPIO1_3_OR_GPIO2_17               [17:1]  RW value= 0x0 */
    /* GPIO1_4_OR_GPIO2_18               [18:1]  RW value= 0x0 */
    /* GPIO1_5_OR_GPIO2_19               [19:1]  RW value= 0x0 */
    /* GPIO1_6_OR_GPIO2_20               [20:1]  RW value= 0x0 */
    /* GPIO1_7_OR_GPIO2_21               [21:1]  RW value= 0x0 */
    /* GPIO1_8_OR_GPIO2_22               [22:1]  RW value= 0x0 */
    /* GPIO1_9_OR_GPIO2_23               [23:1]  RW value= 0x0 */
    /* GPIO1_10_OR_GPIO2_24              [24:1]  RW value= 0x0 */
    /* GPIO1_11_OR_GPIO2_25              [25:1]  RW value= 0x0 */
    /* GPIO1_12_OR_GPIO2_26              [26:1]  RW value= 0x0 */
    /* GPIO1_13_OR_GPIO2_27              [27:1]  RW value= 0x0 */
    /* GPIO1_14_OR_GPIO2_28              [28:1]  RW value= 0x0 */
    /* GPIO1_15_OR_GPIO2_29              [29:1]  RW value= 0x0 */
    /* GPIO1_16_OR_GPIO2_30              [30:1]  RW value= 0x0 */
    /* GPIO1_17_OR_GPIO2_31              [31:1]  RW value= 0x0 */
#endif

#ifdef __cplusplus
}
#endif


#endif /* #ifdef HW_MSSIO_MUX_H_ */

