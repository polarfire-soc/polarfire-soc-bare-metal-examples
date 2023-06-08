/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * This example project demonstrates the use of the CoreAXI4DMAController IP for
 * data stream transfers.
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/fpga_ip/CoreAXI4DMAController/core_axi4dmacontroller.h"

/* To configure the memory peripheral used as the stream destination define the
 * following macro:*/
#undef USE_CACHED_DDR

#define TURN_ON_ADDED_DEBUG_MESSAGES

/* Defining the number of words / bytes to transfer*/
#define TRANSFER_SIZE_BYTES                (1000000u)

/* Defining memory addresses*/
#define DMA_CONTROL_BASE_ADDRESS           (0x60010000u)

#define DDR_CACHED_BASE_ADDRESS            (0x84000000u)
#define DDR_CACHED_MID_ADDRESS             (0x88000000u)

#define DDR_NON_CACHED_BASE_ADDRESS        (0xC0000000u)
#define DDR_NON_CACHED_MID_ADDRESS         (0xC8000000u)

/* The offset address in memory where the first stream value is written to*/
#define STREAM_DEST_START_OFFSET           (0x1000u)

/* Pattern Generator Data Widths*/
#define AXI4_STREAM_DATA_WIDTH_BYTES       (sizeof(uint64_t))
#define MODULUS(X, Y)                      ((X) % (Y))
#define ROUND_TO_DATA_WIDTH(X)             ((X)-MODULUS(X, AXI4_STREAM_DATA_WIDTH_BYTES))

/* Transfer Sizes*/
#define TRANSFER_SIZE_ROUNDED              ROUND_TO_DATA_WIDTH(TRANSFER_SIZE_BYTES)

/* Misc. Defines*/
#define SOURCE_MEMORY_INIT_PATTERN         (0xFFu)
#define MEGA_BYTE                          (1000000u)
#define SPACE_KEY                          (' ')

/* CoreAXI4 DMA Controller Interrupt Bit Masks
 * For full details see CoreAXI4DMAController Handbook section 5.3
 */
#define DMA_OPERATION_STATUS_BIT_MASK      (0xFu)
#define DMA_DESCRIPTOR_NUM_BIT_MASK        (0x3F0u)
#define IR_STATUS_DESCRIPTOR_NUM_BIT_SHIFT (0x4u)
#define RETURN_DESCRIPTOR_INTERRUPT_NUMBER(X) \
    (((X)&DMA_DESCRIPTOR_NUM_BIT_MASK) >> IR_STATUS_DESCRIPTOR_NUM_BIT_SHIFT)

#ifdef USE_CACHED_DDR
uint32_t transfer_source_address = DDR_CACHED_BASE_ADDRESS + STREAM_DEST_START_OFFSET;
uint32_t transfer_dest_address = DDR_CACHED_MID_ADDRESS;
#else
uint32_t transfer_source_address = DDR_NON_CACHED_BASE_ADDRESS + STREAM_DEST_START_OFFSET;
uint32_t transfer_dest_address = DDR_NON_CACHED_MID_ADDRESS;
#endif

typedef enum
{
    TRANSFER_SELECTION,
    TRANSFER_SETUP,
    TRANSFER_IN_PROGRESS,
    TRANSFER_COMPLETE,
    TRANSFER_ERROR
} dma_status_t;

typedef enum
{
    TRANSFER_DATA_MISMATCH,
    TRANSFER_DATA_MATCH
} data_integrity_status_t;

static axi4dma_instance_t coreaxi4dma;
static axi4dma_instance_t *const core_axi4_dma_controller = &coreaxi4dma;
static mss_uart_instance_t *const uart1 = &g_mss_uart1_lo;
static volatile uint32_t uart_recieve_buffer_length = 0u;
static uint8_t uart_recieve_buffer[1u] = {0u};
static volatile axi4dma_desc_id_t dma_descriptor_id;
static volatile uint32_t external_descriptor_address;
static volatile uint32_t irq_status = 0u;

static const char greeting_message[] = "\r\n\r\n\r\n **** PolarFire SoC CoreAXI4DMAController"
                                       "Block Transfer Example ****\r\n";

static const char selection_message[] = "\r\nPress <SPACE> to perform a Block transfer\r\n";

uint32_t
verify_transfer(uint64_t *source_address, uint64_t *dest_address)
{
    uint64_t *src;
    uint64_t *dst;
    uint64_t index;
    for (index = 0; index < (TRANSFER_SIZE_ROUNDED / sizeof(uint64_t)); index++)
    {
        dst = (dest_address + index);
        src = (source_address + index);

        if (*dst != *src)
        {
#ifdef TURN_ON_ADDED_DEBUG_MESSAGES
            uint8_t debug_message[100] = {0};

            sprintf(debug_message,
                    "\r\nError at address: 0x%-9x!"
                    "\tExpected: %-6iRead: %-6i",
                    (dest_address + index),
                    *(source_address + index),
                    *(dest_address + index));
            MSS_UART_polled_tx_string(uart1, debug_message);
#endif
            return TRANSFER_DATA_MISMATCH;
        }
    }
    return TRANSFER_DATA_MATCH;
}

void
memory_init(uint64_t *sourcce_address, uint64_t *dest_address)
{
    uint64_t index;

    /* set destination memory block to 0 */
    clear_64_mem(dest_address, (uint64_t *)(dest_address + TRANSFER_SIZE_ROUNDED));

    /* set pattern in source memory */
    for (index = 0; index < TRANSFER_SIZE_ROUNDED; index++)
    {
        sourcce_address[index] = (index & SOURCE_MEMORY_INIT_PATTERN);
    }
}

uint8_t
PLIC_f2m_2_IRQHandler(void)
{
    irq_status = AXI4DMA_transfer_status(core_axi4_dma_controller,
                                         IRQ_NUM_0,
                                         &dma_descriptor_id,
                                         &external_descriptor_address);

    AXI4DMA_clear_irq(core_axi4_dma_controller,
                      IRQ_NUM_0,
                      AXI4DMA_OP_COMPLETE_INTR_MASK | AXI4DMA_WR_ERR_INTR_MASK |
                          AXI4DMA_RD_ERR_INTR_MASK | AXI4DMA_INVALID_DESC_INTR_MASK);

    return EXT_IRQ_KEEP_ENABLED;
}

void
u54_1(void)
{
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /*Put this hart into WFI.*/

    do
    {
        __asm("wfi");
    } while (0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();
#endif

    char transfer_message[75] = {0};
    dma_status_t fdma_state = TRANSFER_SELECTION;

    mss_enable_fabric();

    PLIC_init();
    PLIC_SetPriority(FABRIC_F2H_2_PLIC, 2);
    PLIC_EnableIRQ(FABRIC_F2H_2_PLIC);

    __enable_irq();

    /* Reset the peripherals turn on the clocks */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC0, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC3, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    MSS_UART_init(uart1,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    AXI4DMA_init(core_axi4_dma_controller, DMA_CONTROL_BASE_ADDRESS);

    AXI4DMA_enable_irq(core_axi4_dma_controller,
                       IRQ_NUM_0,
                       AXI4DMA_OP_COMPLETE_INTR_MASK | AXI4DMA_WR_ERR_INTR_MASK |
                           AXI4DMA_RD_ERR_INTR_MASK | AXI4DMA_INVALID_DESC_INTR_MASK);

    MSS_UART_polled_tx_string(uart1, greeting_message);

    while (TRANSFER_ERROR != fdma_state)
    {
        switch (fdma_state)
        {
            case TRANSFER_SELECTION:
                uart_recieve_buffer_length = 0u;

                MSS_UART_polled_tx_string(uart1, selection_message);
                while (1u)
                {
                    uart_recieve_buffer_length =
                        MSS_UART_get_rx(uart1, uart_recieve_buffer, sizeof(uart_recieve_buffer));
                    if (uart_recieve_buffer_length > 0u)
                    {
                        if (SPACE_KEY == uart_recieve_buffer[0u])
                        {
                            fdma_state = TRANSFER_SETUP;
                            break;
                        }
                        else
                        {
                            MSS_UART_polled_tx_string(uart1,
                                                      "\r\n\r\nInvalid Option Selected!!\r\n");
                            MSS_UART_polled_tx_string(uart1, selection_message);
                            uart_recieve_buffer_length = 0u;
                        }
                    }
                }
                break;

            case TRANSFER_SETUP:

                irq_status = 0x0u;
                memory_init((uint64_t *)transfer_source_address, (uint64_t *)transfer_dest_address);

                AXI4DMA_configure(core_axi4_dma_controller,
                                  INTRN_DESC_0,
                                  OP_INC_ADDR,
                                  OP_INC_ADDR,
                                  TRANSFER_SIZE_BYTES,
                                  transfer_source_address,
                                  transfer_dest_address);

                AXI4DMA_start_transfer(core_axi4_dma_controller, INTRN_DESC_0);

                sprintf(transfer_message,
                        "\r\n\r\nCoreAXI4DMAController: Block Transfer\r\n"
                        "> Transferring %-i MB\r\n",
                        (TRANSFER_SIZE_BYTES / MEGA_BYTE));
                MSS_UART_polled_tx_string(uart1, transfer_message);

                fdma_state = TRANSFER_IN_PROGRESS;
                break;

            case TRANSFER_IN_PROGRESS:

                if (AXI4DMA_OP_COMPLETE_INTR_MASK & (irq_status & DMA_OPERATION_STATUS_BIT_MASK))
                {
                    if (INTRN_DESC_0 == RETURN_DESCRIPTOR_INTERRUPT_NUMBER(irq_status))
                    {
                        fdma_state = TRANSFER_COMPLETE;
                    }
                    else
                    {
                        fdma_state = TRANSFER_ERROR;
                    }
                }
                else if ((irq_status & DMA_OPERATION_STATUS_BIT_MASK) &
                         (AXI4DMA_WR_ERR_INTR_MASK | AXI4DMA_RD_ERR_INTR_MASK |
                          AXI4DMA_INVALID_DESC_INTR_MASK))
                {
                    fdma_state = TRANSFER_ERROR;
                }
                MSS_UART_polled_tx_string(uart1, ".");
                break;

            case TRANSFER_COMPLETE:

                MSS_UART_polled_tx_string(uart1, "\r\n> Block transfer complete!\r\n");

                if (TRANSFER_DATA_MATCH == verify_transfer((uint64_t *)transfer_source_address,
                                                           (uint64_t *)transfer_dest_address))
                {
                    MSS_UART_polled_tx_string(uart1, "> Data Matches!\r\n\r\n");
                    fdma_state = TRANSFER_SELECTION;
                }
                else
                {
                    MSS_UART_polled_tx_string(uart1, "\r\nERROR - Data mismatch\r\n\r\n");
                }
                break;

            default:
                /* The program should never reach this state */
                MSS_UART_polled_tx_string(uart1,
                                          "\r\nCoreAXI4DMAController: Unknown Error!\r\n\r\n");
                fdma_state = TRANSFER_ERROR;
                break;
        }
    }

    switch ((irq_status & DMA_OPERATION_STATUS_BIT_MASK))
    {
        case AXI4DMA_WR_ERR_INTR_MASK:
            MSS_UART_polled_tx_string(uart1, "\r\nCoreAXI4DMAController: Write Error!\r\n\r\n");
            break;

        case AXI4DMA_RD_ERR_INTR_MASK:
            MSS_UART_polled_tx_string(uart1, "\r\nCoreAXI4DMAController: Read Error!\r\n\r\n");
            break;

        case AXI4DMA_INVALID_DESC_INTR_MASK:
            MSS_UART_polled_tx_string(
                uart1,
                "\r\nCoreAXI4DMAController: Invalid Descriptor Error!\r\n\r\n");
            break;

        default:
            MSS_UART_polled_tx_string(uart1, "\r\nCoreAXI4DMAController: Unknown Error!\r\n\r\n");
            break;
    }
}
