/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * USB MSC Class Host controller example application to demonstrate the
 * PolarFire SoC MSS USB operations in USB Host mode.
 *
 * Header for pendrive_host_app.h
 *
 */

#ifndef APPLICATION_HART1_PENDRIVE_HOST_APP_H_
#define APPLICATION_HART1_PENDRIVE_HOST_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "FatFs/src/ff.h"
#include "FatFs/src/diskio.h"

/* The buffer size determines the USB read/write transfers.
 * For maximum file/read write performance, the buffer size must be maximum
 * possible.Due to limited RAM in the debug mode,the buffer size is selected
 * as 4096.
 * In Release mode of this project you may want to increase this value to
 * extract faster disk read/write performance.
 */
#define BUFFER_SIZE                                     4096

void display_main_menu(void);
void goto_main_menu(void);
void read_file_menu(void);
void invalid_selection_menu(void);
void copy_file_menu(void);
void get_device_descriptor(void);

/* Start node to be scanned (also used as work area) */
FRESULT scan_root_dir (char* path);
FRESULT read_root_dir_file (uint8_t index);
FRESULT copy_root_dir_file (uint8_t index, uint8_t character);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_HART1_PENDRIVE_HOST_APP_H_ */
