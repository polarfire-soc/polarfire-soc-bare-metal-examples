/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_qspi_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreQSPI register bit offsets and masks definitions
 *
 */

#ifndef CORE_QSPI_REGS_H_
#define CORE_QSPI_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Control register bit offsets
 */

#define CONTROL_REG_OFFSET                         0x00u

#define CTRL_ENABLE_OFFSET                         0x0u
#define CTRL_ENABLE_MASK                           0x1u
#define CTRL_ENABLE_SHIFT                          0

#define CTRL_XIP_OFFSET                            0x00u
#define CTRL_XIP_MASK                              0x04u
#define CTRL_XIP_SHIFT                             2

#define CTRL_XIPADDR_OFFSET                        0x0u
#define CTRL_XIPADDR_MASK                          0x08u
#define CTRL_XIPADDR_SHIFT                         3

#define CTRL_CLKIDLE_OFFSET                        0x0u
#define CTRL_CLKIDLE_MASK                          0x400u
#define CTRL_CLKIDLE_SHIFT                         10

#define CTRL_SAMPLE_OFFSET                         0x0u
#define CTRL_SAMPLE_MASK                           0x1800u
#define CTRL_SAMPLE_SHIFT                          11

#define CTRL_QMODE0_OFFSET                         0x0u
#define CTRL_QMODE0_MASK                           0x2000u
#define CTRL_QMODE0_SHIFT                          13

#define CTRL_QMODE12_OFFSET                        0x0u
#define CTRL_QMODE12_MASK                          0xC000u
#define CTRL_QMODE12_SHIFT                         14

#define CTRL_FLAGSX4_OFFSET                        0x0u
#define CTRL_FLAGSX4_MASK                          0x10000u
#define CTRL_FLAGSX4_SHIFT                         16

#define CTRL_CLKRATE_OFFSET                        0x00u
#define CTRL_CLKRATE_MASK                          0xF000000u
#define CTRL_CLKRATE_SHIFT                         24

#define CTRL_SAMPLE_SCK_OFFSET                    (0x0u << CTRL_SAMPLE_SHIFT)
#define CTRL_SAMPLE_HCLKF_OFFSET                  (0x1u << CTRL_SAMPLE_SHIFT)
#define CTRL_SAMPLE_HCLKR_OFFSET                  (0x2u << CTRL_SAMPLE_SHIFT)

/*******************************************************************************
 * Frames register bit offsets
 */
#define FRAMES_REG_OFFSET                          0x04u

#define FRAMES_TOTALBYTES_OFFSET                   0x04u
#define FRAMES_TOTALBYTES_MASK                     0xFFFFu
#define FRAMES_TOTALBYTES_SHIFT                    0

#define FRAMES_COMMANDBYTES_OFFSET                 0x04u
#define FRAMES_COMMANDBYTES_MASK                   0x1FF0000u
#define FRAMES_COMMANDBYTES_SHIFT                  16

#define FRAMES_QSPI_OFFSET                         0x04u
#define FRAMES_QSPI_MASK                           0x2000000u
#define FRAMES_QSPI_SHIFT                          25

#define FRAMES_IDLE_OFFSET                         0x04u
#define FRAMES_IDLE_MASK                           0x3C000000u
#define FRAMES_IDLE_SHIFT                          26

#define FRAMES_FLAGBYTE_OFFSET                     0x04u
#define FRAMES_FLAGBYTE_MASK                       0x40000000u
#define FRAMES_FLAGBYTE_SHIFT                      30

#define FRAMES_FLAGWORD_OFFSET                     0x04u
#define FRAMES_FLAGWORD_MASK                       0x80000000u
#define FRAMES_FLAGWORD_SHIFT                      31

/*******************************************************************************
 * Interrupt enable register bit offsets
 */
#define INT_ENABLE_REG_OFFSET                      0x0Cu

#define INT_ENABLE_TXDONE_OFFSET                   0x0Cu
#define INT_ENABLE_TXDONE_MASK                     0x01u
#define INT_ENABLE_TXDONE_SHIFT                    0

#define INT_ENABLE_RXDONE_OFFSET                   0x0Cu
#define INT_ENABLE_RXDONE_MASK                     0x02u
#define INT_ENABLE_RXDONE_SHIFT                    1

#define INT_ENABLE_RXAVAILABLE_OFFSET              0x0Cu
#define INT_ENABLE_RXAVAILABLE_MASK                0x04u
#define INT_ENABLE_RXAVAILABLE_SHIFT               2

#define INT_ENABLE_TXAVAILABLE_OFFSET              0x0Cu
#define INT_ENABLE_TXAVAILABLE_MASK                0x08u
#define INT_ENABLE_TXAVAILABLE_SHIFT               3

#define INT_ENABLE_RXFIFOEMPTY_OFFSET              0x0Cu
#define INT_ENABLE_RXFIFOEMPTY_MASK                0x10u
#define INT_ENABLE_RXFIFOEMPTY_SHIFT               4

#define INT_ENABLE_TXFIFOFULL_OFFSET               0x0Cu
#define INT_ENABLE_TXFIFOFULL_MASK                 0x20u
#define INT_ENABLE_TXFIFOFULL_SHIFT                5

/******************************************************************************
 * Status register bit offsets
 */
#define STATUS_REG_OFFSET                       0x10u

#define STATUS_TXDONE_OFFSET                    0x10u
#define STATUS_TXDONE_MASK                      0x01u
#define STATUS_TXDONE_SHIFT                     0

#define STATUS_RXDONE_OFFSET                    0x10u
#define STATUS_RXDONE_MASK                      0x02u
#define STATUS_RXDONE_SHIFT                     1

#define STATUS_RXAVAILABLE_OFFSET               0x10u
#define STATUS_RXAVAILABLE_MASK                 0x04u
#define STATUS_RXAVAILABLE_SHIFT                2

#define STATUS_TXAVAILABLE_OFFSET               0x10u
#define STATUS_TXAVAILABLE_MASK                 0x08u
#define STATUS_TXAVAILABLE_SHIFT                3

#define STATUS_RXFIFOEMPTY_OFFSET               0x10u
#define STATUS_RXFIFOEMPTY_MASK                 0x10u
#define STATUS_RXFIFOEMPTY_SHIFT                4

#define STATUS_TXFIFOFULL_OFFSET                0x10u
#define STATUS_TXFIFOFULL_MASK                  0x20u
#define STATUS_TXFIFOFULL_SHIFT                 5

#define STATUS_READY_OFFSET                     0x10u
#define STATUS_READY_MASK                       0x80u
#define STATUS_READY_SHIFT                      7

#define STATUS_FLAGSX4_OFFSET                   0x10u
#define STATUS_FLAGSX4_MASK                     0x100u
#define STATUS_FLAGSX4_SHIFT                    8

/******************************************************************************
 * Direct access register bit offsets
 */
#define DIRECT_ACCESS_REG_OFFSET                 0x14u

#define DIRECT_ACCESS_EN_SSEL_OFFSET             0x14u
#define DIRECT_ACCESS_EN_SSEL_MASK               0x01u
#define DIRECT_ACCESS_EN_SSEL_SHIFT              0

#define DIRECT_ACCESS_OP_SSEL_OFFSET             0x14u
#define DIRECT_ACCESS_OP_SSEL_MASK               0x02u
#define DIRECT_ACCESS_OP_SSEL_SHIFT              1

#define DIRECT_ACCESS_EN_SCLK_OFFSET             0x14u
#define DIRECT_ACCESS_EN_SCLK_MASK               0x04u
#define DIRECT_ACCESS_EN_SCLK_SHIFT              2

#define DIRECT_ACCESS_OP_SCLK_OFFSET             0x14u
#define DIRECT_ACCESS_OP_SCLK_MASK               0x08u
#define DIRECT_ACCESS_OP_SCLK_SHIFT              3

#define DIRECT_ACCESS_EN_SDO_OFFSET              0x14u
#define DIRECT_ACCESS_EN_SDO_MASK                0xF0u
#define DIRECT_ACCESS_EN_SDO_SHIFT               4

#define DIRECT_ACCESS_OP_SDO_OFFSET              0x14u
#define DIRECT_ACCESS_OP_SDO_MASK                0xF00u
#define DIRECT_ACCESS_OP_SDO_SHIFT               8

#define DIRECT_ACCESS_OP_SDOE_OFFSET             0x14u
#define DIRECT_ACCESS_OP_SDOE_MASK               0xF000u
#define DIRECT_ACCESS_OP_SDOE_SHIFT              12

#define DIRECT_ACCESS_IP_SDI_OFFSET              0x14u
#define DIRECT_ACCESS_IP_SDI_MASK                0xF0000u
#define DIRECT_ACCESS_IP_SDI_SHIFT               16

#define DIRECT_ACCESS_IP_SCLK_OFFSET             0x14u
#define DIRECT_ACCESS_IP_SCLK_MASK               0x200000u
#define DIRECT_ACCESS_IP_SCLK_SHIFT              21

#define DIRECT_ACCESS_IP_SSEL_OFFSET             0x14u
#define DIRECT_ACCESS_IP_SSEL_MASK               0x400000u
#define DIRECT_ACCESS_IP_SSEL_SHIFT              22

#define DIRECT_ACCESS_IDLE_OFFSET                0x14u
#define DIRECT_ACCESS_IDLE_MASK                  0x800000u
#define DIRECT_ACCESS_IDLE_SHIFT                 23

/******************************************************************************
 * Upper address register bit offsets
 */
#define UPPER_ADDRESS_REG_OFFSET                   0x18u

#define UPPER_ADDRESS_ADDRUP_OFFSET                0x18u
#define UPPER_ADDRESS_ADDRUP_MASK                  0xFFu
#define UPPER_ADDRESS_ADDRUP_SHIFT                 0

/******************************************************************************
 * Receive data register bit offsets
 */
#define RECEIVE_DATA_REG_OFFSET                    0x40u

#define RECEIVE_DATA_RXDATA_OFFSET                 0x40u
#define RECEIVE_DATA_RXDATA_MASK                   0xFFu
#define RECEIVE_DATA_RXDATA_SHIFT                  0

/******************************************************************************
 * Transmit data register bit offsets
 */
#define TRANSMIT_DATA_REG_OFFSET                   0x44u

#define TRANSMIT_DATA_TXDATA_OFFSET                0x44u
#define TRANSMIT_DATA_TXDATA_MASK                  0xFFu
#define TRANSMIT_DATA_TXDATA_SHIFT                 0

/******************************************************************************
 * X4ReceiveData register bit offsets
 */
#define X4RECEIVE_DATA_REG_OFFSET                 0x48u

#define X4RECEIVE_DATA_RXDATA4_OFFSET             0x48u
#define X4RECEIVE_DATA_RXDATA4_MASK               0xFFFFFFFFu
#define X4RECEIVE_DATA_RXDATA4_SHIFT              0

/******************************************************************************
 * X4 Transmit data register bit offsets
 */
#define X4TRANSMIT_DATA_REG_OFFSET                0x4Cu

#define X4TRANSMIT_DATA_TXDATA4_OFFSET            0x4Cu
#define X4TRANSMIT_DATA_TXDATA4_MASK              0xFFFFFFFFu
#define X4TRANSMIT_DATA_TXDATA4_SHIFT             0

/******************************************************************************
 * FRAMESUP register bit offsets
 */
#define FRAMESUP_REG_OFFSET                      0x50

#define FRAMESUP_BYTESLOWER_OFFSET               0x50
#define FRAMESUP_BYTESLOWER_MASK                 0xFFFFu
#define FRAMESUP_BYTESLOWER_SHIFT                0

#define FRAMESUP_BYTESUPPER_OFFSET               0x50
#define FRAMESUP_BYTESUPPER_MASK                 0xFFFF0000u
#define FRAMESUP_BYTESUPPER_SHIFT                16


#ifdef __cplusplus
}
#endif

#endif /* CORE_QSPI_REGS_H_ */
