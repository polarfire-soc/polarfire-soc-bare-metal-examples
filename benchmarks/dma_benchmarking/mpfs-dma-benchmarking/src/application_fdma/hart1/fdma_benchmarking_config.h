/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef FDMA_BENCHMARKING_CONFIG_H_
#define FDMA_BENCHMARKING_CONFIG_H_

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

#define STREAM_DEST_START_OFFSET       (0x1000u)

#define CACHED_DDR_BASE_ADDRESS        (0x84000000u)
#define CACHED_DDR0                    ((CACHED_DDR_BASE_ADDRESS) + (STREAM_DEST_START_OFFSET))
#define CACHED_DDR1                    (0x88000000u)

#define NON_CACHED_DDR_BASE_ADDRESS    (0xC0000000u)
#define NON_CACHED_DDR0                ((NON_CACHED_DDR_BASE_ADDRESS) + (STREAM_DEST_START_OFFSET))
#define NON_CACHED_DDR1                (0xC0800000u)

#define STREAM_GEN                     (0xFFFFFFFFu)

/* Module that produces the data for an AXI4Stream Transfer*/
#define DMA_CONTROL_BASE_ADDRESS       (0x60010000u)
/* AXI4 Stream Data Generator*/
#define STREAM_GEN_BASE_ADDRESS        (0x40000F00u)

/* Other macros*/
#define BYTES_TO_MEGABITS_SCALE_FACTOR (125000.0)

#define FDMA_BENCHMARKING_LIST_SIZE    (6u)

#define STREAM_DEST_OPERAND            (0x0001u << 0u)
#define STREAM_DEST_DATA_READY         (0x0001u << 2u)
#define STREAM_DESCRIPTOR_VALID        (0x0001u << 3u)

#define STREAM_GEN_START_REG_OFFSET    (0x4u)
#define STREAM_GEN_RESET_REG_OFFSET    (0x8u)
#define START_STREAM_GEN               (0x1u)
#define STOP_STREAM_GEN                (0x0u)
#define RESET_GENERATOR                (0x0u)
#define UN_RESET_GENERATOR             (0x1u)

/* Pattern Generator Data Widths*/
#define AXI4_STREAM_DATA_WIDTH_BYTES   (8u)
#define MODULUS(X, Y)                  ((X) % (Y))
#define STREAM_PATTERN_COUNT(X) \
    (((X)-MODULUS(X, AXI4_STREAM_DATA_WIDTH_BYTES)) / AXI4_STREAM_DATA_WIDTH_BYTES)
#define ROUND_TO_DATA_WIDTH(X)       ((X)-MODULUS(X, AXI4_STREAM_DATA_WIDTH_BYTES))

#define STREAM_GEN_PATTERN_COUNT_REG *((uint32_t *)(STREAM_GEN_BASE_ADDRESS))
#define STREAM_GEN_START_REG \
    *((uint32_t *)((STREAM_GEN_BASE_ADDRESS) + (STREAM_GEN_START_REG_OFFSET)))
#define STREAM_GEN_RESET_REG \
    *((uint32_t *)((STREAM_GEN_BASE_ADDRESS) + (STREAM_GEN_RESET_REG_OFFSET)))

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
    FDMA_TRANSFER_INCOMPLETE,
    STREAM_TRANSFER_COMPLETE,
    BLOCK_TRANSFER_COMPLETE,
    FDMA_TRANSFER_ERROR
} fdma_transfer_status_t;

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

const dma_benchmarking_params_t fdma_benchmark_list[FDMA_BENCHMARKING_LIST_SIZE] = {
    {CACHED_DDR0,
     CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {CACHED_DDR0,
     NON_CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {NON_CACHED_DDR0,
     NON_CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {NON_CACHED_DDR0,
     CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {STREAM_GEN,
     NON_CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE},
    {STREAM_GEN,
     CACHED_DDR1,
     MIN_TRANSFER_SIZE_BYTES,
     MAX_TRANSFER_SIZE_BYTES,
     TRANSFER_STEP_SIZE}};

#endif /* FDMA_BENCHMARKING_CONFIG_H_ */
