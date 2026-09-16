/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file pac1934.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief pac1934 source file
 *
 */

#ifndef PAC1934_H_
#define PAC1934_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void pac1934_draw_vb(void);
void pac1934_draw_isense(void);
int32_t pac1934_sensor_probe(void);

#ifdef __cplusplus
}
#endif

#endif
