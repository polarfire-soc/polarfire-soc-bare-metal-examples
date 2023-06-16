/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fdma_benchmarking_config.h"
#include "drivers/fpga_ip/CoreAXI4DMAController/core_axi4dmacontroller.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "mpfs_hal/mss_hal.h"

axi4dma_instance_t g_dmac;
mss_uart_instance_t *uart1 = &g_mss_uart1_lo;

static uint8_t g_rx_buff[1u] = {0};
static volatile uint8_t g_rx_size = 0u;

static volatile fdma_transfer_status_t fdma_transfer_status = FDMA_TRANSFER_INCOMPLETE;
static volatile dma_error_status_t error_state = NO_ERROR;
static volatile uint64_t fdma_end_mcycle = 0u;

static const char fdma_menu_greeting[] = "\r\n\r\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                         "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n> FDMA - "
                                         "Select benchmark to run:\r\n";

static const char invalid_selection_message[] = "\r\n\r\nInvalid option!\r\nPlease select one "
                                                "of the following:\r\n\r\n";

static const char fdma_options[] = "\r\n\t1: Cached DDR to Non-Cached DDR\r\n"
                                   "\t2: Cached DDR to Cached DDR\r\n"
                                   "\r\n"
                                   "\t3: Non-Cached DDR to Non-Cached DDR\r\n"
                                   "\t4: Non-Cached DDR to Cached DDR\r\n"
                                   "\r\n"
                                   "\t5: FPGA Fabric to Non-Cached DDR\r\n"
                                   "\t6: FPGA Fabric to Cached DDR\r\n"
                                   "\r\n"
                                   "\ta: Run all benchmarks\r\n"
                                   "\tTo register a selection please press \'ENTER\'.\r\n\r\n";
;

static const char divider[] =
    "======================================================================================\r\n";

static const char table_header[] =
    " Data             Source           Destination      Test             Transfer\r\n"
    " Size             Address          Address          Result           Rate\r\n"
    " (Bytes)                                                             (MegaBits/second)\r\n";

static const char greeting_message[] =
    "\r\n\r\n\r\n **** PolarFire SoC CoreAXI4DMAController Benchmarking Application ****\r\n";

static const char memory_descriptors[3][21] = {"Cached DDR", "Non-Cached DDR", "FPGA Fabric"};

void
configure_board(void)
{
    PLIC_init();
    __enable_irq();

    PLIC_SetPriority_Threshold(0);

    /* Reset the peripherals turn on the clocks */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC0, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC3, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    mss_enable_fabric();

    PLIC_SetPriority(FABRIC_F2H_2_PLIC, 2);
    PLIC_EnableIRQ(FABRIC_F2H_2_PLIC);

    PLIC_SetPriority(DMA_CH0_DONE_IRQn, 2u);
    PLIC_SetPriority(DMA_CH0_ERR_IRQn, 1u);

    /* Enable PDMA Interrupts. */
    PLIC_EnableIRQ(DMA_CH0_DONE_IRQn);
    PLIC_EnableIRQ(DMA_CH0_ERR_IRQn);

    MSS_UART_init(uart1,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
}

uint32_t
get_user_input(void)
{
    uint32_t user_input = 0u;
    while (1u)
    {
        g_rx_size = MSS_UART_get_rx(uart1, g_rx_buff, sizeof(g_rx_buff));
        if (g_rx_size > 0u)
        {
            MSS_UART_polled_tx_string(uart1, g_rx_buff);
            if (g_rx_buff[0u] == '\r')
            {
                return user_input;
            }
            else
            {
                user_input = g_rx_buff[0u];
            }
        }
    }
}

void
print_table_cell(uint8_t *col_1)
{
    uint8_t message[50u] = {0};
    sprintf(message, " %-16s", col_1);
    MSS_UART_polled_tx_string(uart1, message);
}
/* The CoreAXI4DMAController Interrupt 0 via F2H interrupt. */
uint8_t
PLIC_f2m_2_IRQHandler(void)
{
    fdma_end_mcycle = readmcycle();
    axi4dma_desc_id_t desc_id;
    uint32_t ext_ptr_addr;
    uint32_t transaction_status = 0u;
    uint32_t descriptor = 0u;

    uint32_t irq_status = AXI4DMA_transfer_status(&g_dmac, IRQ_NUM_0, &desc_id, &ext_ptr_addr);

    transaction_status = irq_status & 0xFu;
    descriptor = (irq_status >> 4u) & 0x3Fu;

    switch (transaction_status)
    {
        case AXI4DMA_OP_COMPLETE_INTR_MASK:
            switch (descriptor)
            {
                case STREAM_DESC_33:
                    fdma_transfer_status = STREAM_TRANSFER_COMPLETE;
                    break;

                case INTRN_DESC_0:
                    fdma_transfer_status = BLOCK_TRANSFER_COMPLETE;
                    break;

                default:
                    fdma_transfer_status = FDMA_TRANSFER_ERROR;
                    error_state = UNKNOWN_DESCRIPTOR_TRANSFER_COMPLETE;
                    break;
            }
            break;

        case AXI4DMA_WR_ERR_INTR_MASK:
            fdma_transfer_status = FDMA_TRANSFER_ERROR;
            error_state = WRITE_ERROR;
            break;

        case AXI4DMA_RD_ERR_INTR_MASK:
            fdma_transfer_status = FDMA_TRANSFER_ERROR;
            error_state = READ_ERROR;
            break;

        case AXI4DMA_INVALID_DESC_INTR_MASK:
            fdma_transfer_status = FDMA_TRANSFER_ERROR;
            error_state = INVALID_DESCRIPTOR;
            break;

        default:
            fdma_transfer_status = FDMA_TRANSFER_ERROR;
            error_state = UNKNOWN_ERROR;

#ifdef DEBUG_DMA
            uint8_t error_message[50] = {0};
            sprintf(error_message, "> Transfer Status: %i\r\n", transaction_status);
            MSS_UART_polled_tx_string(uart1, error_message);
#endif
            break;
    }

    AXI4DMA_clear_irq(&g_dmac,
                      IRQ_NUM_0,
                      AXI4DMA_OP_COMPLETE_INTR_MASK | AXI4DMA_WR_ERR_INTR_MASK |
                          AXI4DMA_RD_ERR_INTR_MASK | AXI4DMA_INVALID_DESC_INTR_MASK);

    return EXT_IRQ_KEEP_ENABLED;
}

void
error_reporter(void)
{
    switch (error_state)
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
            MSS_UART_polled_tx_string(uart1, "\r\nError!\r\nDMA Invalid Descriptor\r\n");
            break;

        case UNKNOWN_ERROR:
            MSS_UART_polled_tx_string(uart1, "\r\nError!\r\nDMA Unknown Error\r\n");
            break;

        default:
            break;
    }
}

static uint32_t
block_transfer_verify_data(uint32_t transfer_size,
                           uint8_t *source_address,
                           uint8_t *destination_address)
{
    volatile uint8_t *source = NULL;
    volatile uint8_t *destination = NULL;
    uint32_t index = 0u;

    for (index = 0; index < transfer_size; index++)
    {
        source = (source_address + index);
        destination = (destination_address + index);
        mb();
        if (*source != *destination)
        {
#ifdef DEBUG_DMA
            uint8_t debug_message[100] = {0};

            sprintf(debug_message,
                    "\r\nError at address: 0x%-9x!"
                    "\tExpected: %-6iRead: %-6i",
                    destination,
                    *source,
                    *destination);
            MSS_UART_polled_tx_string(uart1, debug_message);
#endif
            return TRANSFER_DATA_MISMATCH;
        }
    }
    return TRANSFER_DATA_MATCH;
}

static uint32_t
stream_transfer_verify_data(uint64_t transfer_size, uint64_t destination_address)
{
    uint64_t expected = 0u;
    uint64_t *dst = destination_address;

    for (expected = 0; expected < (transfer_size / sizeof(uint64_t)); expected++)
    {
        if ((expected + 1u) != *dst)
        {
#ifdef DEBUG_DMA
            uint8_t debug_message[100] = {0};
            sprintf(debug_message,
                    "\r\nError at address: 0x%-9x!\tExpected: %-5iRead: %-5i\r\n",
                    dst,
                    (expected + 1u),
                    *dst);

            MSS_UART_polled_tx_string(uart1, debug_message);
#endif
            return TRANSFER_DATA_MISMATCH;
        }
        dst++;
    }
    return TRANSFER_DATA_MATCH;
}

double
calculate_rate(uint64_t clock_cycles, uint32_t transfer_size)
{
    /* seconds*/
    double seconds = clock_cycles / (double)LIBERO_SETTING_MSS_COREPLEX_CPU_CLK;
    /* Bits per second*/
    double transfer_rate = transfer_size / seconds;
    /* MegaBits per second*/
    transfer_rate /= BYTES_TO_MEGABITS_SCALE_FACTOR;
    return transfer_rate;
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
    tf_state transfer_state = TRANSFER_SELECTION;

    uint32_t total_benchmarks = 0u;
    uint32_t fdma_benchmark_index = 0u;
    uint32_t current_benchmark_count = 0u;

    uint32_t current_transfer_size = 0u;

    uint8_t fdma_choice = 0u;

    uint64_t benchmark_start_mcycle = 0u;

    uint32_t fdma_transfer_data_integrity_check = TRANSFER_DATA_MISMATCH;

    char selection_message[25u] = {0};

    configure_board();

    MSS_UART_polled_tx_string(uart1, greeting_message);

    AXI4DMA_init(&g_dmac, DMA_CONTROL_BASE_ADDRESS);

    /* The stream descriptor is associated with IRQ0 in the IP */
    AXI4DMA_enable_irq(&g_dmac,
                       IRQ_NUM_0,
                       AXI4DMA_OP_COMPLETE_INTR_MASK | AXI4DMA_WR_ERR_INTR_MASK |
                           AXI4DMA_RD_ERR_INTR_MASK | AXI4DMA_INVALID_DESC_INTR_MASK);

    while (1u)
    {
        switch (transfer_state)
        {
            case TRANSFER_SELECTION:

                fdma_choice = 0u;
                current_benchmark_count = 0u;
                fdma_benchmark_index = 0u;

                MSS_UART_polled_tx_string(uart1, fdma_menu_greeting);
                MSS_UART_polled_tx_string(uart1, fdma_options);

                while (1u)
                {
                    fdma_choice = get_user_input();
                    if ((fdma_choice == 'a') || ((fdma_choice > '0') && (fdma_choice < '7')))
                    {
                        break;
                    }
                    MSS_UART_polled_tx_string(uart1, invalid_selection_message);
                    MSS_UART_polled_tx_string(uart1, fdma_options);
                }
                if ('a' == fdma_choice)
                {
                    MSS_UART_polled_tx_string(uart1, "\r\n\r\nRunning all benchmarks.\r\n\r\n");
                    total_benchmarks = FDMA_BENCHMARKING_LIST_SIZE;
                }
                else
                {
                    fdma_benchmark_index = fdma_choice - '1';
                    total_benchmarks = 1u;
                }
                current_transfer_size = fdma_benchmark_list[fdma_benchmark_index].min_tranfer_size;

                MSS_UART_polled_tx_string(uart1, divider);
                MSS_UART_polled_tx_string(uart1, table_header);
                MSS_UART_polled_tx_string(uart1, divider);

                transfer_state = TRANSFER_SETUP;
                break;

            case TRANSFER_SETUP:

                if (current_benchmark_count < total_benchmarks)
                {
                    if (ROUND_TO_DATA_WIDTH(current_transfer_size) <
                        ROUND_TO_DATA_WIDTH(
                            fdma_benchmark_list[fdma_benchmark_index].max_transfer_size))
                    {
                        /* Initialize variabels */
                        fdma_transfer_status = FDMA_TRANSFER_INCOMPLETE;
                        fdma_end_mcycle = 0u;

                        /* F-DMA Setup Code - F-DMA Memory to Memory Transfer*/
                        if (fdma_benchmark_list[fdma_benchmark_index].source_address == STREAM_GEN)
                        {
                            STREAM_GEN_RESET_REG = UN_RESET_GENERATOR;

                            clear_64_mem((uint64_t *)fdma_benchmark_list[fdma_benchmark_index]
                                             .destination_address,
                                         (uint64_t *)(fdma_benchmark_list[fdma_benchmark_index]
                                                          .destination_address +
                                                      current_transfer_size));

                            /* Set the stream transfer to destination memory address */
                            AXI4DMA_configure_stream(
                                &g_dmac,
                                (axi4dma_stream_desc_t *)NON_CACHED_DDR_BASE_ADDRESS,
                                TDEST_0,
                                STREAM_DESCRIPTOR_VALID | STREAM_DEST_DATA_READY |
                                    STREAM_DEST_OPERAND,
                                (ROUND_TO_DATA_WIDTH(current_transfer_size)),
                                fdma_benchmark_list[fdma_benchmark_index].destination_address);

                            STREAM_GEN_PATTERN_COUNT_REG =
                                (STREAM_PATTERN_COUNT(current_transfer_size));
                        }
                        /* F-DMA Setup Code - F-DMA Memory to FPGA fabric Transfer*/
                        else
                        {
                            memset((uint8_t *)fdma_benchmark_list[fdma_benchmark_index]
                                       .destination_address,
                                   0x00,
                                   ROUND_TO_DATA_WIDTH(current_transfer_size));

                            /* Set a repeating pattern in the source memory block */
                            for (uint32_t index = 0;
                                 index < ROUND_TO_DATA_WIDTH(current_transfer_size);
                                 index++)
                            {
                                *((uint8_t *)fdma_benchmark_list[fdma_benchmark_index]
                                      .source_address +
                                  index) = ((index + 0x1u) & 0xFFu);
                            }

                            AXI4DMA_configure(
                                &g_dmac,
                                INTRN_DESC_0,
                                OP_INC_ADDR,
                                OP_INC_ADDR,
                                (ROUND_TO_DATA_WIDTH(current_transfer_size)),
                                fdma_benchmark_list[fdma_benchmark_index].source_address,
                                fdma_benchmark_list[fdma_benchmark_index].destination_address);
                        }

                        /* Both DMA Setup Correctly: Start Timing*/
                        write_csr(mcycle, 0x0u);

                        benchmark_start_mcycle = readmcycle();

                        /* Starting the FDMA */
                        if (fdma_benchmark_list[fdma_benchmark_index].source_address == STREAM_GEN)
                        {
                            STREAM_GEN_START_REG = START_STREAM_GEN;
                        }
                        else
                        {
                            AXI4DMA_start_transfer(&g_dmac, INTRN_DESC_0);
                        }

                        transfer_state = TRANSFER_IN_PROGRESS;
                        break;
                    }
                    else
                    {
                        current_benchmark_count++;
                        fdma_benchmark_index++;

                        if (current_benchmark_count < total_benchmarks)
                        {
                            current_transfer_size = ROUND_TO_DATA_WIDTH(
                                fdma_benchmark_list[fdma_benchmark_index].min_tranfer_size);
                        }
                    }
                }
                else
                {
                    transfer_state = TRANSFER_SELECTION;
                }
                break;

            case TRANSFER_IN_PROGRESS:
                if (BLOCK_TRANSFER_COMPLETE == fdma_transfer_status ||
                    STREAM_TRANSFER_COMPLETE == fdma_transfer_status)
                {
                    transfer_state = TRANSFER_COMPLETE;
                }
                if (FDMA_TRANSFER_ERROR == fdma_transfer_status)
                {
                    error_reporter();
                    HAL_ASSERT(0);
                }

                break;

            case TRANSFER_COMPLETE:

                /* Checking that the transferred data is correct */
                if (STREAM_TRANSFER_COMPLETE == fdma_transfer_status)
                {
                    STREAM_GEN_START_REG = STOP_STREAM_GEN;
                    STREAM_GEN_RESET_REG = RESET_GENERATOR;

                    fdma_transfer_data_integrity_check = stream_transfer_verify_data(
                        (ROUND_TO_DATA_WIDTH(current_transfer_size)),
                        fdma_benchmark_list[fdma_benchmark_index].destination_address);
                }
                else if (BLOCK_TRANSFER_COMPLETE == fdma_transfer_status)
                {
                    fdma_transfer_data_integrity_check = block_transfer_verify_data(
                        ROUND_TO_DATA_WIDTH(current_transfer_size),
                        (uint8_t *)fdma_benchmark_list[fdma_benchmark_index].source_address,
                        (uint8_t *)fdma_benchmark_list[fdma_benchmark_index].destination_address);
                }

                if (TRANSFER_DATA_MISMATCH == fdma_transfer_data_integrity_check)
                {
                    MSS_UART_polled_tx_string(uart1,
                                              "\r\nError Data Mismatch!!\r\n> Hal Assert!\r\n");
                    HAL_ASSERT(0);
                    break;
                }
                else
                {
                    /* Calculating the transfer rate MegaBits per Second */
                    double fdma_transfer_rate =
                        calculate_rate((fdma_end_mcycle - benchmark_start_mcycle),
                                       (ROUND_TO_DATA_WIDTH(current_transfer_size)));

                    /* Printing the results */
                    char results_cell[21] = {0};

                    sprintf(results_cell, "%d", ROUND_TO_DATA_WIDTH(current_transfer_size));
                    print_table_cell(results_cell);

                    /* Source Address*/
                    switch (fdma_benchmark_list[fdma_benchmark_index].source_address)
                    {
                        case CACHED_DDR0:
                            print_table_cell(memory_descriptors[0]);
                            break;

                        case NON_CACHED_DDR0:
                            print_table_cell(memory_descriptors[1]);
                            break;

                        case STREAM_GEN:
                            print_table_cell(memory_descriptors[2]);
                            break;

                        default:
                            sprintf(
                                results_cell,
                                "0x%lx",
                                (uint64_t)fdma_benchmark_list[fdma_benchmark_index].source_address);
                            print_table_cell(results_cell);
                            break;
                    }

                    /* Destination Address*/
                    switch (fdma_benchmark_list[fdma_benchmark_index].destination_address)
                    {
                        case CACHED_DDR0:
                            print_table_cell(memory_descriptors[0]);
                            break;

                        case CACHED_DDR1:
                            print_table_cell(memory_descriptors[0]);
                            break;

                        case NON_CACHED_DDR0:
                            print_table_cell(memory_descriptors[1]);
                            break;

                        case NON_CACHED_DDR1:
                            print_table_cell(memory_descriptors[1]);
                            break;

                        default:
                            sprintf(results_cell,
                                    "0x%lx",
                                    (uint64_t)fdma_benchmark_list[fdma_benchmark_index]
                                        .destination_address);
                            print_table_cell(results_cell);
                            break;
                    }

                    if (TRANSFER_DATA_MISMATCH == fdma_transfer_data_integrity_check)
                    {
                        print_table_cell("Fail");
                    }
                    else
                    {
                        print_table_cell("Pass");
                    }

                    sprintf(results_cell, "%ld", (uint64_t)fdma_transfer_rate);
                    print_table_cell(results_cell);
                    MSS_UART_polled_tx_string(uart1, "\r\n");

                    current_transfer_size += fdma_benchmark_list[fdma_benchmark_index].step_size;
                    transfer_state = TRANSFER_SETUP;
                    break;
                }

            default:
                /* Should never get here, trigger an HAL ASSERT if we do!*/
                HAL_ASSERT(0);
                break;
        }
    }
}
