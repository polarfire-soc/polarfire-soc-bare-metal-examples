/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software
 *
 */

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_ 1

#include <stddef.h>
#include <stdint.h>
#include "state_machine_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
  Function prototypes
 */
uint8_t MSS_DDR_init_simulation(void);
uint8_t MSS_DDR_training(uint8_t ddr_type);
uint32_t state_machine_h0(void);
uint32_t state_machine_h1(void);

#ifdef __cplusplus
}
#endif

#endif /* _STATE_MACHINE_H_ */


