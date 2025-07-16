/*******************************************************************************
 * (c) Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file coreaxi4protoconv_user_config.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreAXI4ProtoConv user configuration
 *
 */

#ifndef __CORE_AXI4PC_USER_CONFIG_H
#define __CORE_AXI4PC_USER_CONFIG_H

/*******************************************************************************
 * MM2S Configuration
 */
#define MM2S_DATA_FIFO_DEPTH_MAX      0xFFu
#define MM2S_CMD_STS_FIFO_DEPTH_MAX   0x3FFu

/*******************************************************************************
 * S2MM Configuration
 */
#define S2MM_DATA_FIFO_DEPTH_MAX      0xFFu
#define S2MM_CMD_STS_FIFO_DEPTH_MAX   0x3FFu

#endif // __CORE_AXI4PC_USER_CONFIG_H
