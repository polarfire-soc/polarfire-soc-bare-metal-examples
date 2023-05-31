/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * This example project demonstrates the use of the CoreAXI4DMAController IP for
 * data stream transfers.
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/fpga_ip/CoreAXI4DMAController/core_axi4dmacontroller.h"
#include "streaming.h"

#define UART_BUFFER_FIRST_CHAR (0u)

/* Global Variables */
axi4dma_instance_t CoreAXI4DMAController;
axi4dma_instance_t *const core_axi4_dma_controller = &CoreAXI4DMAController;
mss_uart_instance_t *const uart1 = &g_mss_uart1_lo;
transfer_status_t stream_status;
volatile uint8_t uart_recieve_buffer_length = 0u;
uint8_t uart_recieve_buffer[UART_RECIEVE_BUFFER_MAX_SIZE] = {0u};
static volatile dma_error_status_t dma_transfer_error_type = NO_ERROR;

const uint8_t greeting_message[] = "\r\n\r\n\r\n **** PolarFire SoC CoreAXI4DMAController "
                                   "Stream Example ****\r\n";

const uint8_t selection_message[] = "\r\nPress <SPACE> to perform an AXI4DMA Stream transfer!\r\n";

/* Functions*/
uint8_t fabric_f2h_2_plic_IRQHandler(void);
static void configure_board(void);
static void coreaxidma_stream_transfer(void);
static bool does_stream_transferred_data_match(uint64_t *);
static void report_transfer_error(void);

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

    configure_board();

    MSS_UART_polled_tx_string(uart1, greeting_message);
    MSS_UART_polled_tx_string(uart1, selection_message);

    while (1u)
    {
        uart_recieve_buffer_length =
            MSS_UART_get_rx(uart1, uart_recieve_buffer, sizeof(uart_recieve_buffer));
        if (uart_recieve_buffer_length > 0u)
        {
            switch (uart_recieve_buffer[UART_BUFFER_FIRST_CHAR])
            {
                case ' ':
                    coreaxidma_stream_transfer();
                    MSS_UART_polled_tx_string(uart1, selection_message);
                    break;

                default:
                    MSS_UART_polled_tx_string(uart1, "\r\n\r\nInvalid Option Selected!!\r\n");
                    MSS_UART_polled_tx_string(uart1, selection_message);
                    break;
            }
            uart_recieve_buffer_length = 0u;
        }
    }
}

void
coreaxidma_stream_transfer(void)
{
    uint8_t transfer_message[80] = {0};
    STREAM_GEN_PATTERN_COUNT_REG = TRANSFER_PATTERN_COUNT;

    memset(((uint8_t *)transfer_dest_address), MEMORY_INIT_PATTERN, TRANSFER_SIZE_ROUNDED);

    AXI4DMA_init(core_axi4_dma_controller, DMA_CONTROL_BASE_ADDRESS);
    stream_status = STREAM_INCOMPLETE;

    /* Set the stream transfer to destination memory address*/
    AXI4DMA_configure_stream(core_axi4_dma_controller,
                             tdest0_stream_descriptor,
                             TDEST_0,
                             STREAM_DESCRIPTOR_VALID | STREAM_DEST_DATA_READY | STREAM_DEST_OPERAND,
                             TRANSFER_SIZE_ROUNDED,
                             transfer_dest_address);

    /* The stream descriptor is associated with IRQ0 in the IP*/
    AXI4DMA_enable_irq(core_axi4_dma_controller,
                       IRQ_NUM_0,
                       AXI4DMA_OP_COMPLETE_INTR_MASK | AXI4DMA_WR_ERR_INTR_MASK |
                           AXI4DMA_RD_ERR_INTR_MASK | AXI4DMA_INVALID_DESC_INTR_MASK);

    sprintf(transfer_message,
            "\r\n\r\nCoreAXI4 DMA: Stream Transfer\r\n"
            "> Transferring %-i MegaByte(s)\r\n",
            (TRANSFER_SIZE_BYTES / MEGA_BYTE));
    MSS_UART_polled_tx_string(uart1, transfer_message);

    /*  Initiating stream by writing to the register on the generator module*/
    STREAM_GEN_START_REG = START_STREAM_GEN;

    /* Wait for stream to complete*/
    while (STREAM_INCOMPLETE == stream_status)
    {
        asm volatile("nop");
    }

    switch (stream_status)
    {
        case STREAM_TRANSFER_COMPLETE:
            STREAM_GEN_START_REG = STOP_STREAM_GEN;
            MSS_UART_polled_tx_string(uart1, "> Stream transfer complete!\r\n");

            /* Verify data written matches expected*/
            if (true == does_stream_transferred_data_match((uint64_t *)transfer_dest_address))
            {
                MSS_UART_polled_tx_string(uart1, "> Data matches!\r\n\r\n");
            }
            else
            {
                MSS_UART_polled_tx_string(uart1, "\r\nERROR - Data mismatch\r\n\r\n");
            }
            break;
        case TRANSFER_ERROR:
            report_transfer_error();
            break;

        default:
            MSS_UART_polled_tx_string(uart1, "\r\nStream Status Unknown State Error\r\n");
            break;
    }
}

uint8_t
PLIC_f2m_2_IRQHandler(void)
{
    axi4dma_desc_id_t dma_descriptor_id;
    uint32_t external_descriptor_address;
    uint32_t dma_transfer_status = 0u;
    uint32_t descriptor = 0u;

    uint32_t irq_status = AXI4DMA_transfer_status(core_axi4_dma_controller,
                                                  IRQ_NUM_0,
                                                  &dma_descriptor_id,
                                                  &external_descriptor_address);

    dma_transfer_status = irq_status & DMA_OPERATION_STATUS_BIT_MASK;
    descriptor = RETURN_DESCRIPTOR_INTERRUPT_NUMBER(irq_status);

    switch (dma_transfer_status)
    {
        case AXI4DMA_OP_COMPLETE_INTR_MASK:
            switch (descriptor)
            {
                case STREAM_DESC_33:
                    stream_status = STREAM_TRANSFER_COMPLETE;
                    break;

                case INTRN_DESC_0:
                    stream_status = BLOCK_TRANSFER_COMPLETE;
                    break;

                default:
                    stream_status = TRANSFER_ERROR;
                    dma_transfer_error_type = UNKNOWN_DESCRIPTOR_TRANSFER_COMPLETE;
                    break;
            }
            break;

        case AXI4DMA_WR_ERR_INTR_MASK:
            stream_status = TRANSFER_ERROR;
            dma_transfer_error_type = WRITE_ERROR;
            break;

        case AXI4DMA_RD_ERR_INTR_MASK:
            stream_status = TRANSFER_ERROR;
            dma_transfer_error_type = READ_ERROR;
            break;

        case AXI4DMA_INVALID_DESC_INTR_MASK:
            stream_status = TRANSFER_ERROR;
            dma_transfer_error_type = INVALID_DESCRIPTOR;
            break;

        default:
            stream_status = TRANSFER_ERROR;
            dma_transfer_error_type = UNKNOWN_ERROR;
            break;

#ifdef DEBUG_DMA
            uint8_t error_message[50] = {0};
            sprintf(error_message, "> Transfer Status: %i\r\n", dma_transfer_status);
            MSS_UART_polled_tx_string(uart1, error_message);
#endif
            break;
    }

    AXI4DMA_clear_irq(core_axi4_dma_controller,
                      IRQ_NUM_0,
                      AXI4DMA_OP_COMPLETE_INTR_MASK | AXI4DMA_WR_ERR_INTR_MASK |
                          AXI4DMA_RD_ERR_INTR_MASK | AXI4DMA_INVALID_DESC_INTR_MASK);

    return EXT_IRQ_KEEP_ENABLED;
}

void
report_transfer_error(void)
{
    switch (dma_transfer_error_type)
    {
        case UNKNOWN_DESCRIPTOR_TRANSFER_COMPLETE:
            MSS_UART_polled_tx_string(uart1,
                                      "\r\nError!\r\nTransfer Complete set from unknown"
                                      " descirptor\r\n");
            break;

        case WRITE_ERROR:
            MSS_UART_polled_tx_string(uart1, "\r\nError!\r\nDMA Write Error\r\n");
            break;

        case READ_ERROR:
            MSS_UART_polled_tx_string(uart1, "\r\nError!\r\nDMA Read Error\r\n");
            break;

        case INVALID_DESCRIPTOR:
            MSS_UART_polled_tx_string(uart1, "\r\nError!\r\nDMA Invalid Descirptor\r\n");
            break;

        case UNKNOWN_ERROR:
            MSS_UART_polled_tx_string(uart1, "\r\nError!\r\nDMA Unknown Error\r\n");
            break;

        default:
            MSS_UART_polled_tx_string(uart1, "\r\nError!\r\nDMA Unknown Error\r\n");
            break;
    }
    dma_transfer_error_type = NO_ERROR;
}

void
configure_board(void)
{
    PLIC_init();
    __enable_irq();

    /* Reset the peripherals turn on the clocks */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC0, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC3, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    mss_enable_fabric();

    PLIC_SetPriority(FABRIC_F2H_2_PLIC, 2);
    PLIC_EnableIRQ(FABRIC_F2H_2_PLIC);

    MSS_UART_init(uart1,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
}

bool
does_stream_transferred_data_match(uint64_t *dest_address)
{
    uint64_t expected;
    uint64_t *dst;

    for (expected = 0; expected < (TRANSFER_SIZE_ROUNDED / sizeof(uint64_t)); expected++)
    {
        dst = (dest_address + expected);
        if ((expected + 1u) != *dst)
        {
#ifdef DEBUG_DMA
            uint8_t debug_message[100] = {0};
            sprintf(debug_message,
                    "\r\nError at address: 0x%-9x!\tExpected: %-5iRead: %-5i",
                    (dest_address + expected),
                    (expected + 1u),
                    *(dest_address + expected));

            MSS_UART_polled_tx_string(uart1, debug_message);
#endif
            return false;
        }
    }
    return true;
}
