
/*******************************************************************************
 * Copyright 2014 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file crc32.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief crc32 header file
 *
 */                                                                              

#ifndef __CRC32_H
#define __CRC32_H 1

#include "hal/cpu_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Calculates 32 bits CRC value of given data */
uint32_t TSE_crc32(uint32_t value, const uint8_t *data, uint32_t data_length);

/*Calculates 32 bit CRC value of given data,using standard Ethernet CRC function*/
uint32_t TSE_ethernet_crc(const uint8_t *data, uint32_t data_length);

#ifdef __cplusplus
}
#endif

#endif /* __CRC32_H */
