/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef STREAMING_H_
#define STREAMING_H_

/* To configure the memory peripheral used as the stream destination define the
 * following macro:*/
#undef USE_CACHED_DDR

#undef DEBUG_DMA

/* Defining the number of words / bytes to transfer*/
#define TRANSFER_SIZE_BYTES          (1000000u)

/* Defining memory addresses*/
#define STREAM_GEN_BASE_ADDRESS      (0x40000F00u)
#define DMA_CONTROL_BASE_ADDRESS     (0x60010000u)

#define DDR_CACHED_BASE_ADDRESS      (0x84000000u)
#define DDR_CACHED_MID_ADDRESS       (0x88000000u)

#define DDR_NON_CACHED_BASE_ADDRESS  (0xC0000000u)
#define DDR_NON_CACHED_MID_ADDRESS   (0xC8000000u)

#define STREAM_GEN_START_REG_OFFSET  (0x4u)
#define START_STREAM_GEN             (0x1u)
#define STOP_STREAM_GEN              (0x0u)

/* The offset address in memory where the first stream value is written to*/
#define STREAM_DEST_START_OFFSET     (0x1000u)

/* Stream configuraiton bit defines*/
#define STREAM_DEST_OPERAND          (0x0001u << 0u)
#define STREAM_DEST_DATA_READY       (0x0001u << 2u)
#define STREAM_DESCRIPTOR_VALID      (0x0001u << 3u)

/* Pattern Generator Data Widths*/
#define AXI4_STREAM_DATA_WIDTH_BYTES (sizeof(uint64_t))
#define MODULUS(X, Y)                ((X) % (Y))
#define STREAM_PATTERN_COUNT(X) \
    (((X)-MODULUS(X, AXI4_STREAM_DATA_WIDTH_BYTES)) / AXI4_STREAM_DATA_WIDTH_BYTES)
#define ROUND_TO_DATA_WIDTH(X) ((X)-MODULUS(X, AXI4_STREAM_DATA_WIDTH_BYTES))

/* Pattern Generator Registers*/
#define STREAM_GEN_START_REG \
    *((uint32_t *)((STREAM_GEN_BASE_ADDRESS) + (STREAM_GEN_START_REG_OFFSET)))
#define STREAM_GEN_PATTERN_COUNT_REG       *((uint32_t *)(STREAM_GEN_BASE_ADDRESS))

/* Transfer Sizes*/
#define TRANSFER_SIZE_ROUNDED              ROUND_TO_DATA_WIDTH(TRANSFER_SIZE_BYTES)
#define TRANSFER_PATTERN_COUNT             STREAM_PATTERN_COUNT(TRANSFER_SIZE_BYTES)

/* Misc. Defines*/
#define MEMORY_INIT_PATTERN                (0x0u)
#define UART_RECIEVE_BUFFER_MAX_SIZE       (64u)
#define MEGA_BYTE                          (1000000u)

/* CoreAXI4 DMA Controller Interrupt Bit Masks*/
/* For full details see Handbook section 5.3*/
#define DMA_OPERATION_STATUS_BIT_MASK      (0xFu)
#define DMA_DESCRIPTOR_NUM_BIT_MASK        (0x3F0u)
#define IR_STATUS_DESCRIPTOR_NUM_BIT_SHIFT (0x4u)
#define RETURN_DESCRIPTOR_INTERRUPT_NUMBER(X) \
    (((X)&DMA_DESCRIPTOR_NUM_BIT_MASK) >> IR_STATUS_DESCRIPTOR_NUM_BIT_SHIFT)

typedef enum
{
    STREAM_INCOMPLETE,
    STREAM_TRANSFER_COMPLETE,
    BLOCK_TRANSFER_COMPLETE,
    TRANSFER_ERROR
} transfer_status_t;

typedef enum
{
    NO_ERROR,
    UNKNOWN_DESCRIPTOR_TRANSFER_COMPLETE,
    WRITE_ERROR,
    READ_ERROR,
    INVALID_DESCRIPTOR,
    UNKNOWN_ERROR
} dma_error_status_t;

#ifdef USE_CACHED_DDR
uint32_t stream_source_address = DDR_CACHED_BASE_ADDRESS + STREAM_DEST_START_OFFSET;
uint32_t transfer_dest_address = DDR_CACHED_MID_ADDRESS;
axi4dma_stream_desc_t *tdest0_stream_descriptor = (axi4dma_stream_desc_t *)DDR_CACHED_BASE_ADDRESS;
#else
uint32_t stream_source_address = DDR_NON_CACHED_BASE_ADDRESS + STREAM_DEST_START_OFFSET;
uint32_t transfer_dest_address = DDR_NON_CACHED_MID_ADDRESS;
axi4dma_stream_desc_t *tdest0_stream_descriptor =
    (axi4dma_stream_desc_t *)DDR_NON_CACHED_BASE_ADDRESS;
#endif

#endif /* STREAMING_H_ */
