/**************************************************************************//**
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Register defines for PAC1934
 *
 */

#ifndef SRC_APPLICATION_HART0_PAC1934_REGS_H_
#define SRC_APPLICATION_HART0_PAC1934_REGS_H_

#define REFRESH_REG 0x00
#define CTRL_REG 0x1
#define ACC_COUNT_REG 0x2
#define VPOWER1_ACC_REG 0x3
#define VPOWER2_ACC_REG 0x4
#define VPOWER3_ACC_REG 0x5
#define VPOWER4_ACC_REG 0x6
#define VBUS1_REG 0x7
#define VBUS2_REG 0x8
#define VBUS3_REG 0x9
#define VBUS4_REG 0xA
#define VSENSE1_REG 0xB
#define VSENSE2_REG 0xC
#define VSENSE3_REG 0xD
#define VSENSE4_REG 0xE
#define VBUS1_AVG_REG 0xF
#define VBUS2_AVG_REG 0x10
#define VBUS3_AVG_REG 0x11
#define VBUS4_AVG_REG 0x12
#define VSENSE1_AVG_REG 0x13
#define VSENSE2_AVG_REG 0x14
#define VSENSE3_AVG_REG 0x15
#define VSENSE4_AVG_REG 0x16
#define VPOWER1_REG 0x17
#define VPOWER2_REG 0x18
#define VPOWER3_REG 0x19
#define VPOWER4_REG 0x1A
#define CHANNEL_DIS_REG 0x1C
#define NEG_PWR_REG 0x1D
#define REFRESH_G_REG 0x1E
#define REFRESH_V_REG 0x1F
#define SLOW_REG 0x20
#define CTRL_ACT_REG 0x21
#define CHANNEL_DIS_ACT_REG 0x22
#define NEG_PWR_ACT_REG 0x23
#define CTRL_LAT_REG 0x24
#define CHANNEL_DIS_LAT_REG 0x25
#define NEG_PWR_LAT_REG 0x26
#define PID_REG 0xFD
#define MID_REG 0xFE
#define REV_REG 0xFF

#endif /* SRC_APPLICATION_HART0_PAC1934_REGS_H_ */
