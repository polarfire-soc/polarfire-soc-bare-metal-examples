/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * This example project demonstrates the use of the CoreAXI4DMAController IP for
 * data stream transfers.
 */

#ifndef PDMA_BENCHMARKING_CONFIG_H_
#define PDMA_BENCHMARKING_CONFIG_H_

#include <stdint.h>

/* Setting the size range in the benchmarks to be run */
#define MIN_TRANSFER_SIZE_BYTES (1000u)
#define MAX_TRANSFER_SIZE_BYTES (TRANSFER_1_MB)

/* Macros defining benchmark transfer sizes */
#define TRANSFER_STEP_SIZE      (100u)
#define TRANSFER_1_MB           (1000000u)

/* Turn on force ordering on for PDMA transactions*/
#undef FORCE_ORDER

#define DEBUG_DMA

/* Memory Addresses */
#define L2_LIM0                     (0x8040000u)
#define L2_LIM1                     (0x8060000u)
#define SCRATCHPAD0                 (0xA080000u)
#define SCRATCHPAD1                 (0xA0BC000u)
#define CACHED_DDR0                 (0x84000000u)
#define CACHED_DDR1                 (0x88000000u)
#define NON_CACHED_DDR0             (0xC0000000u)
#define NON_CACHED_DDR1             (0xC0800000)

#define HALF_LIM                    (L2_LIM1 - L2_LIM0)
#define FULL_LIM                    (HALF_LIM * 2)

#define HALF_SCRATCHPAD             (SCRATCHPAD1 - SCRATCHPAD0)
#define FULL_SCRATCHPAD             (HALF_SCRATCHPAD * 2)

/* Other macros*/
#define PDMA_BENCHMARKING_LIST_SIZE (16u)
/* Enumerations */

typedef enum
{
    TRANSFER_SELECTION,
    TRANSFER_SETUP,
    TRANSFER_IN_PROGRESS,
    TRANSFER_COMPLETE
} tf_state;

typedef enum
{
    PDMA_TRANSFER_INCOMPLETE,
    PDMA_TRANSFER_COMPLETE,
    PDMA_TRANSFER_ERROR
} pdma_transfer_status_t;

typedef enum
{
    NO_ERROR,
    UNKNOWN_DESCRIPTOR_TRANSFER_COMPLETE,
    WRITE_ERROR,
    READ_ERROR,
    INVALID_DESCRIPTOR,
    UNKNOWN_ERROR
} dma_error_status_t;

typedef enum
{
    TRANSFER_DATA_MISMATCH,
    TRANSFER_DATA_MATCH
} data_integrity_status_t;

/* Benchmarking parameters structure */

typedef struct
{
    uint32_t source_address;
    uint32_t destination_address;
    uint32_t min_tranfer_size;
    uint32_t max_transfer_size;
    uint32_t step_size;
} dma_benchmarking_params_t;

/*
 * PDMA benchmarking list
 */

const dma_benchmarking_params_t pdma_benchmark_list[PDMA_BENCHMARKING_LIST_SIZE] = {
    {L2_LIM0, L2_LIM1, MIN_TRANSFER_SIZE_BYTES, HALF_LIM, TRANSFER_STEP_SIZE},
    {L2_LIM0, SCRATCHPAD0, MIN_TRANSFER_SIZE_BYTES, FULL_LIM, TRANSFER_STEP_SIZE},
    {L2_LIM0, CACHED_DDR0, MIN_TRANSFER_SIZE_BYTES, FULL_LIM, TRANSFER_STEP_SIZE},
    {L2_LIM0, NON_CACHED_DDR0, MIN_TRANSFER_SIZE_BYTES, FULL_LIM, TRANSFER_STEP_SIZE},
    {SCRATCHPAD0, L2_LIM0, MIN_TRANSFER_SIZE_BYTES, FULL_LIM, TRANSFER_STEP_SIZE},
    {SCRATCHPAD0, SCRATCHPAD1, MIN_TRANSFER_SIZE_BYTES, HALF_SCRATCHPAD, TRANSFER_STEP_SIZE},
    {SCRATCHPAD0, CACHED_DDR0, MIN_TRANSFER_SIZE_BYTES, FULL_SCRATCHPAD, TRANSFER_STEP_SIZE},
    {SCRATCHPAD0, NON_CACHED_DDR0, MIN_TRANSFER_SIZE_BYTES, FULL_SCRATCHPAD, TRANSFER_STEP_SIZE},
    {CACHED_DDR0, L2_LIM0, MIN_TRANSFER_SIZE_BYTES, FULL_LIM, TRANSFER_STEP_SIZE},
    {CACHED_DDR0, SCRATCHPAD0, MIN_TRANSFER_SIZE_BYTES, FULL_SCRATCHPAD, TRANSFER_STEP_SIZE},
    {CACHED_DDR0,
     CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {CACHED_DDR0,
     NON_CACHED_DDR0,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {NON_CACHED_DDR0, L2_LIM0, MIN_TRANSFER_SIZE_BYTES, FULL_LIM, TRANSFER_STEP_SIZE},
    {NON_CACHED_DDR0, SCRATCHPAD0, MIN_TRANSFER_SIZE_BYTES, FULL_SCRATCHPAD, TRANSFER_STEP_SIZE},
    {NON_CACHED_DDR0,
     CACHED_DDR0,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {NON_CACHED_DDR0,
     NON_CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE}};

#endif /* PDMA_BENCHMARKING_CONFIG_H_ */
