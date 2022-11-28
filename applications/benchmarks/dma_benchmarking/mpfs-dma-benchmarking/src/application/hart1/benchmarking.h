/*******************************************************************************
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BENCHMARKING_H_
#define BENCHMARKING_H_

#include "fpga_design_config/fpga_design_config.h"

/* Addresses */
#define L2_LIM0             (0x8040000u)
#define L2_LIM1             (0x8060000u)
#define SCRATCHPAD0         (0xA080000u)
#define SCRATCHPAD1         (0xA0BC000u)
#define CACHED_DDR0         (0x84000000u)
#define CACHED_DDR1         (0x88000000u)
#define NON_CACHED_DDR0     (0xC0000000u)
#define NON_CACHED_DDR1     (0xC0800000u)
#define FAB_MSS_LSRAM0      (0x61000000u)
#define FAB_MSS_LSRAM1      (0x61000800u)
#define FAB_PF_LSRAM0       (0x60030000u)
#define FAB_PF_LSRAM1       (0x60800000u)

/* Transfer Sizes */
#define STEP_SIZE           (1000u)
#define MIN_TRANSFER_SIZE   (1000u)

#define HALF_LIM            (L2_LIM1 - L2_LIM0)
#define FULL_LIM            (HALF_LIM * 2)

#define HALF_SCRATCHPAD     (SCRATCHPAD1 - SCRATCHPAD0)
#define FULL_SCRATCHPAD     (HALF_SCRATCHPAD * 2)

#define TRANSFER_1_MB       (997500u)

/* Define to turn force ordering on for each transfer */
#undef FORCE_ORDER

/* Define any of the below to run a benchmark from SOURCE_DESTINATION */

#define LIM_LIM
#undef LIM_SCRATCHPAD
#undef LIM_CACHED_DDR
#undef LIM_NON_CACHED_DDR

#undef SCRATCHPAD_LIM
#undef SCRATCHPAD_SCRATCHPAD
#undef SCRATCHPAD_CACHED_DDR
#undef SCRATCHPAD_NON_CACHED_DDR

#undef CACHED_DDR_LIM
#undef CACHED_DDR_SCRATCHPAD
#undef CACHED_DDR_CACHED_DDR
#undef CACHED_DDR_NON_CACHED_DDR

#undef NON_CACHED_DDR_LIM
#undef NON_CACHED_DDR_SCRATCHPAD
#undef NON_CACHED_DDR_CACHED_DDR
#undef NON_CACHED_DDR_NON_CACHED_DDR

typedef struct
{
    uint32_t *src_addr;
    uint32_t *dst_addr;
    uint32_t min_transfer_size;
    uint32_t max_transfer_size;
    uint32_t step_size;
} benchmark_params_t;

const benchmark_params_t benchmark_list[] = {

/* Benchmarking Transfers using L2-LIM as the source:
* =============================================================================
*/

#ifdef LIM_LIM
    {(uint32_t *)L2_LIM0, (uint32_t *)L2_LIM1, MIN_TRANSFER_SIZE, HALF_LIM, STEP_SIZE},
#endif

#ifdef LIM_SCRATCHPAD
    {(uint32_t *)L2_LIM0, (uint32_t *)SCRATCHPAD0, MIN_TRANSFER_SIZE, FULL_LIM, STEP_SIZE},
#endif

#ifdef LIM_CACHED_DDR
    {(uint32_t *)L2_LIM0, (uint32_t *)CACHED_DDR0, MIN_TRANSFER_SIZE, FULL_LIM, STEP_SIZE},
#endif

#ifdef LIM_NON_CACHED_DDR
    {(uint32_t *)L2_LIM0, (uint32_t *)NON_CACHED_DDR0, MIN_TRANSFER_SIZE, FULL_LIM, STEP_SIZE},
#endif

/* Benchmarking Transfers using Scratchapad memory as the source:
* =============================================================================
*/

#ifdef SCRATCHPAD_LIM
    {(uint32_t *)SCRATCHPAD0, (uint32_t *)L2_LIM0, MIN_TRANSFER_SIZE, FULL_LIM, STEP_SIZE},
#endif

#ifdef SCRATCHPAD_SCRATCHPAD
    {(uint32_t *)SCRATCHPAD0, (uint32_t *)SCRATCHPAD1, MIN_TRANSFER_SIZE, HALF_SCRATCHPAD, STEP_SIZE},
#endif

#ifdef SCRATCHPAD_CACHED_DDR
    {(uint32_t *)SCRATCHPAD0, (uint32_t *)CACHED_DDR0, MIN_TRANSFER_SIZE, FULL_SCRATCHPAD, STEP_SIZE},
#endif

#ifdef SCRATCHPAD_NON_CACHED_DDR
    {(uint32_t *)SCRATCHPAD0, (uint32_t *)NON_CACHED_DDR0, MIN_TRANSFER_SIZE, FULL_SCRATCHPAD, STEP_SIZE},
#endif

/* Benchmarking Transfers using Cached DDR as the source:
* =============================================================================
*/

#ifdef CACHED_DDR_LIM
    {(uint32_t *)CACHED_DDR0, (uint32_t *)L2_LIM0, MIN_TRANSFER_SIZE, FULL_LIM, STEP_SIZE},
#endif

#ifdef CACHED_DDR_SCRATCHPAD
    {(uint32_t *)CACHED_DDR0, (uint32_t *)SCRATCHPAD0, MIN_TRANSFER_SIZE, FULL_SCRATCHPAD, STEP_SIZE},
#endif

#ifdef CACHED_DDR_CACHED_DDR
    {(uint32_t *)CACHED_DDR0, (uint32_t *)CACHED_DDR1, MIN_TRANSFER_SIZE, TRANSFER_1_MB, STEP_SIZE},
#endif

#ifdef CACHED_DDR_NON_CACHED_DDR
    {(uint32_t *)CACHED_DDR0, (uint32_t *)NON_CACHED_DDR0, MIN_TRANSFER_SIZE, TRANSFER_1_MB, STEP_SIZE},
#endif


/*
* Benchmarking Transfers using Non-Cached DDR as the source:
* =============================================================================
*/

#ifdef NON_CACHED_DDR_LIM
    {(uint32_t *)NON_CACHED_DDR0, (uint32_t *)L2_LIM0, MIN_TRANSFER_SIZE, FULL_LIM, STEP_SIZE},
#endif

#ifdef NON_CACHED_DDR_SCRATCHPAD
    {(uint32_t *)NON_CACHED_DDR0, (uint32_t *)SCRATCHPAD0, MIN_TRANSFER_SIZE, FULL_SCRATCHPAD, STEP_SIZE},
#endif

#ifdef NON_CACHED_DDR_CACHED_DDR
    {(uint32_t *)NON_CACHED_DDR0, (uint32_t *)CACHED_DDR0, MIN_TRANSFER_SIZE, TRANSFER_1_MB, STEP_SIZE},
#endif

#ifdef NON_CACHED_DDR_NON_CACHED_DDR
    {(uint32_t *)NON_CACHED_DDR0, (uint32_t *)NON_CACHED_DDR1, MIN_TRANSFER_SIZE, TRANSFER_1_MB, STEP_SIZE}
#endif

};

#endif /* BENCHMARKING_H_ */
