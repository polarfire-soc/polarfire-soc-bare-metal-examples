/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file pac1934.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief The PAC1934 bare metal software driver implementation.
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

extern const uint8_t msg_pac_show_menu_again_prompt[];
extern const uint8_t msg_pac_page_break[];
extern const uint8_t msg_pac_page_break_mid[];
extern const uint8_t msg_pac_page_break_edge[];
extern const uint8_t msg_pac_reading_current[];
extern const uint8_t msg_pac_column_title_current_vals[];
extern const uint8_t msg_pac_column_title_voltage_vals[];
extern const uint8_t msg_pac_column_title_power_vals[];

extern const uint8_t msg_pac_voltage_prints[];
extern const uint8_t msg_pac_current_prints[];

#ifdef __cplusplus
}
#endif

#endif

