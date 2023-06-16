/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fpga_design_config/clocks/hw_mss_clks.h"
#include "pdma_benchmarking_config.h"
#include "drivers/fpga_ip/CoreAXI4DMAController/core_axi4dmacontroller.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_pdma/mss_pdma.h"
#include "mpfs_hal/mss_hal.h"

#define BYTES_TO_MEGABITS_SCALE_FACTOR (125000.0)
#define CHAR_TO_LONG_CONVERSION_BASE   (10u)

mss_uart_instance_t *uart1 = &g_mss_uart1_lo;

static uint8_t g_rx_buff[1u] = {0};
static volatile uint8_t g_rx_size = 0u;
static volatile pdma_transfer_status_t pdma_transfer_status = PDMA_TRANSFER_INCOMPLETE;

static uint32_t benchmark_error_count = 0u;
static uint32_t pdma_error_interrupt_count = 0u;

static volatile uint64_t pdma_end_mcycle = 0u;

static const char pdma_menu_greeting[] = "\r\n\r\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                                         "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n> PDMA - "
                                         "Select benchmark to run:\r\n";

static const char pdma_options[] = "\r\n\t1: L2-Lim to L2-Lim\r\n"
                                   "\t2: L2-Lim to Scratchpad memory\r\n"
                                   "\t3: L2-Lim to Cached DDR\r\n"
                                   "\t4: L2-Lim to Non Cached DDR\r\n"
                                   "\r\n"
                                   "\t5: Scratchpad memory to L2-Lim\r\n"
                                   "\t6: Scratchpad memory to Scratchpad memory\r\n"
                                   "\t7: Scratchpad memory to Cached DDR\r\n"
                                   "\t8: Scratchpad memory to Non Cached DDR\r\n"
                                   "\r\n"
                                   "\t9: Cached DDR to L2-Lim\r\n"
                                   "\t10: Cached DDR to Scratchpad memory\r\n"
                                   "\t11: Cached DDR to Cached DDR\r\n"
                                   "\t12: Cached DDR to Non Cached DDR\r\n"
                                   "\r\n"
                                   "\t13: Non Cached DDR to L2-Lim\r\n"
                                   "\t14: Non Cached DDR to Scratchpad memory\r\n"
                                   "\t15: Non Cached DDR to Cached DDR\r\n"
                                   "\t16: Non Cached DDR to Non Cached DDR\r\n"
                                   "\r\n"
                                   "\ta: Run all benchmarks\r\n\r\n"
                                   "\tTo register a selection please press \'ENTER\'.\r\n\r\n";

static const char invalid_selection_message[] = "\r\n\r\nInvalid option!\r\nPlease select one "
                                                "of the following:\r\n\r\n";

static const char divider[] =
    "======================================================================================\r\n";

static const char table_header[] =
    " Data             Source           Destination      Test             Transfer\r\n"
    " Size             Address          Address          Result           Rate\r\n"
    " (Bytes)                                                             (MegaBits/second)\r\n";

static const char greeting_message[] =
    "\r\n\r\n\r\n **** PolarFire SoC Platform DMA Benchmarking Application ****\r\n";

static const char memory_descriptors[4][21] = {"L2-Lim",
                                               "Scratchpad",
                                               "Cached DDR",
                                               "Non-Cached DDR"};

uint32_t
get_user_input(void)
{
    uint8_t pdma_menu_msg[40u] = {0};
    uint8_t user_input[2u] = {'  '};
    uint32_t buffer_size = 0u;
    uint32_t selected_benchmark = 0u;

    while (1u)
    {
        g_rx_size = MSS_UART_get_rx(uart1, g_rx_buff, sizeof(g_rx_buff));
        if (g_rx_size > 0u)
        {
            MSS_UART_polled_tx_string(uart1, g_rx_buff);
            if (g_rx_buff[0u] == '\r')
            {
                selected_benchmark =
                    (uint32_t)strtol(user_input, NULL, CHAR_TO_LONG_CONVERSION_BASE);
                return selected_benchmark;
            }
            else if ('a' == g_rx_buff[0u])
            {
                return (uint32_t)'a';
            }
            else
            {
                if (buffer_size < sizeof(user_input))
                {
                    user_input[buffer_size] = g_rx_buff[0u];
                    buffer_size++;
                }
                else
                {
                    buffer_size = 0u;
                    user_input[buffer_size] = g_rx_buff[0u];
                    buffer_size++;
                    user_input[buffer_size] = ' ';
                }
            }
            g_rx_size = 0u;
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

void
configure_pdma(mss_pdma_channel_config_t *pdma_config_ch,
               uint64_t source_address,
               uint64_t destination_address,
               uint32_t transfer_size)
{
    pdma_config_ch->num_bytes = transfer_size;
    pdma_config_ch->src_addr = source_address;
    pdma_config_ch->dest_addr = destination_address;

    pdma_config_ch->enable_done_int = 1u;
    pdma_config_ch->enable_err_int = 1u;
    pdma_config_ch->repeat = 0u;

#ifdef FORCE_ORDER
    pdma_config_ch->force_order = 1u;
#else
    pdma_config_ch->force_order = 0u;
#endif
}

static void
pdma_isr(uint8_t interrupt_type)
{
    /* Clear DONE interrupt flag */
    if (interrupt_type == PDMA_CH0_DONE_INT)
    {
        pdma_end_mcycle = readmcycle();
        MSS_PDMA_clear_transfer_complete_status(interrupt_type);
        pdma_transfer_status = PDMA_TRANSFER_COMPLETE;
    }
    /* Clear ERROR interrupt flag */
    else if (interrupt_type == PDMA_CH0_ERROR_INT)
    {
        MSS_PDMA_clear_transfer_error_status(interrupt_type);
        pdma_error_interrupt_count++;
    }
}

void
pdma_print_error_count(void)
{
    uint8_t errors_message[50] = {"\r\n\t\t- - - - -\t\t\r\n"};
    MSS_UART_polled_tx_string(uart1, errors_message);

    if (pdma_error_interrupt_count > 0)
    {
        sprintf(errors_message,
                "\r\n%-3d PDMA Error Interrupts Occurred\r\n",
                pdma_error_interrupt_count);
        MSS_UART_polled_tx_string(uart1, errors_message);
        pdma_error_interrupt_count = 0u;
    }
    else
    {
        sprintf(errors_message, "\r\nNo PDMA Error Interrupts Occurred\r\n");
        MSS_UART_polled_tx_string(uart1, errors_message);
    }

    if (benchmark_error_count > 0)
    {
        sprintf(errors_message, "%-3d Benchmarking Tests FAILED!\r\n", benchmark_error_count);
        MSS_UART_polled_tx_string(uart1, errors_message);
        benchmark_error_count = 0u;
    }
    else
    {
        sprintf(errors_message, "All Benchmarking Tests PASSED!\r\n\r\n");
        MSS_UART_polled_tx_string(uart1, errors_message);
    }

    sprintf(errors_message, "\r\n\t\t========\t\t\r\n\r\n");
    MSS_UART_polled_tx_string(uart1, errors_message);
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
        /* Memory Fence Instruction */
        mb();
        if (*source != *destination)
        {
#ifdef DEBUG_DMA
            uint8_t debug_message[100] = {0};

            sprintf(debug_message,
                    "\r\nError at address: 0x%-9x!"
                    "\tExpected: %-6iRead: %-6i\r\n",
                    destination,
                    *destination,
                    *source);
            MSS_UART_polled_tx_string(uart1, debug_message);
#endif
            return TRANSFER_DATA_MISMATCH;
        }
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
    uint32_t pdma_benchmark_index = 0u;
    uint32_t current_benchmark_count = 0u;

    uint32_t current_transfer_size = 0u;

    uint32_t pdma_choice = 0u;

    uint64_t benchmark_start_mcycle = 0u;

    uint32_t pdma_transfer_data_integrity_check = TRANSFER_DATA_MISMATCH;

    mss_pdma_channel_config_t pdma_config_ch;

    char selection_message[25u] = {0};

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t)MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    MSS_UART_init(uart1,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    PLIC_init();
    __enable_irq();
    PLIC_SetPriority_Threshold(0);

    PLIC_SetPriority(DMA_CH0_DONE_IRQn, 1u);
    PLIC_SetPriority(DMA_CH0_ERR_IRQn, 1u);

    /* Enable PDMA Interrupts. */
    PLIC_EnableIRQ(DMA_CH0_DONE_IRQn);
    PLIC_EnableIRQ(DMA_CH0_ERR_IRQn);

    /* If the application is being debugged from LIM.
     * The HAL will not clear LIM memory, as to avoid clearing the memory the
     * application itself is stored in.
     * In this case, the region of LIM used by the PDMA is cleared instead.
     */
    clear_64_mem((uint64_t *)L2_LIM0, (uint64_t *)L2_LIM1);

    MSS_UART_polled_tx_string(uart1, greeting_message);

    while (1u)
    {
        switch (transfer_state)
        {
            case TRANSFER_SELECTION:

                pdma_choice = 0u;
                current_benchmark_count = 0u;
                pdma_benchmark_index = 0u;

                MSS_UART_polled_tx_string(uart1, pdma_menu_greeting);
                MSS_UART_polled_tx_string(uart1, pdma_options);

                while (1u)
                {
                    pdma_choice = get_user_input();
                    if ((pdma_choice == 'a') ||
                        ((pdma_choice > 0) && (pdma_choice <= PDMA_BENCHMARKING_LIST_SIZE)))
                    {
                        break;
                    }
                    MSS_UART_polled_tx_string(uart1, invalid_selection_message);
                    MSS_UART_polled_tx_string(uart1, pdma_options);
                }

                if ('a' == pdma_choice)
                {
                    MSS_UART_polled_tx_string(uart1, "\r\n\r\nRunning all benchmarks.\r\n\r\n");
                    total_benchmarks = PDMA_BENCHMARKING_LIST_SIZE;
                }
                else
                {
                    sprintf(selection_message, "\r\nSelected PDMA Benchmark: %i", (pdma_choice));
                    pdma_benchmark_index = pdma_choice - 1u;

                    total_benchmarks = 1u;
                }

                current_transfer_size = pdma_benchmark_list[pdma_benchmark_index].min_tranfer_size;

                MSS_UART_polled_tx_string(uart1, divider);
                MSS_UART_polled_tx_string(uart1, table_header);
                MSS_UART_polled_tx_string(uart1, divider);

                transfer_state = TRANSFER_SETUP;
                break;

            case TRANSFER_SETUP:

                if (current_benchmark_count < total_benchmarks)
                {
                    if (current_transfer_size <

                        pdma_benchmark_list[pdma_benchmark_index].max_transfer_size)
                    {
                        /* Initialize variables */
                        pdma_transfer_status = PDMA_TRANSFER_INCOMPLETE;

                        pdma_end_mcycle = 0u;

                        /* Clean Destination Memory*/
                        clear_64_mem((uint64_t *)pdma_benchmark_list[pdma_benchmark_index]
                                         .destination_address,
                                     (uint64_t *)(pdma_benchmark_list[pdma_benchmark_index]
                                                      .destination_address +
                                                  current_transfer_size));

                        /* Set a repeating pattern in the source memory block */
                        for (uint32_t index = 0; index < current_transfer_size; index++)
                        {
                            *((uint8_t *)pdma_benchmark_list[pdma_benchmark_index].source_address +
                              index) = (index & 0xFFu);
                        }

                        configure_pdma(
                            &pdma_config_ch,
                            (uint64_t)pdma_benchmark_list[pdma_benchmark_index].source_address,
                            (uint64_t)pdma_benchmark_list[pdma_benchmark_index].destination_address,
                            current_transfer_size);

                        if (MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_0,
                                                    &pdma_config_ch,
                                                    pdma_isr) != MSS_PDMA_OK)
                        {
                            MSS_UART_polled_tx_string(uart1, "\r\nError: Setup Transfer!\r\n");
                            HAL_ASSERT(0);
                        }

                        write_csr(mcycle, 0x0u);

                        benchmark_start_mcycle = readmcycle();

                        /* Starting the PDMA */
                        if (MSS_PDMA_start_transfer(MSS_PDMA_CHANNEL_0) != MSS_PDMA_OK)
                        {
                            MSS_UART_polled_tx_string(uart1, "\r\nError: Start Transfer!\r\n");
                            current_transfer_size =
                                pdma_benchmark_list[pdma_benchmark_index].max_transfer_size;
                            HAL_ASSERT(0);
                            break;
                        }

                        transfer_state = TRANSFER_IN_PROGRESS;
                        break;
                    }
                    else
                    {
                        current_benchmark_count++;
                        pdma_benchmark_index++;

                        if (current_benchmark_count < total_benchmarks)
                        {
                            current_transfer_size =
                                pdma_benchmark_list[pdma_benchmark_index].min_tranfer_size;
                        }
                    }
                }
                else
                {
                    pdma_print_error_count();
                    transfer_state = TRANSFER_SELECTION;
                }
                break;

            case TRANSFER_IN_PROGRESS:
                if (PDMA_TRANSFER_COMPLETE == pdma_transfer_status)
                {
                    transfer_state = TRANSFER_COMPLETE;
                }
                if (PDMA_TRANSFER_ERROR == pdma_transfer_status)
                {
                    transfer_state = TRANSFER_SELECTION;
                }
                break;

            case TRANSFER_COMPLETE:

                /* Checking that the transferred data is correct */
                pdma_transfer_data_integrity_check = block_transfer_verify_data(
                    current_transfer_size,
                    (uint8_t *)pdma_benchmark_list[pdma_benchmark_index].source_address,
                    (uint8_t *)pdma_benchmark_list[pdma_benchmark_index].destination_address);

                if (TRANSFER_DATA_MISMATCH == pdma_transfer_data_integrity_check)
                {
                    MSS_UART_polled_tx_string(uart1,
                                              "\r\nError Data Mismatch!!\r\n> Hal Assert!\r\n");
                    HAL_ASSERT(0);
                    break;
                }
                else
                {
                    /* Calculating the time (mcycles) */
                    double pdma_transfer_rate;

                    pdma_transfer_rate = calculate_rate((pdma_end_mcycle - benchmark_start_mcycle),
                                                        current_transfer_size);

                    /* Printing the results */
                    char results_cell[21] = {0};

                    sprintf(results_cell, "%d", current_transfer_size);
                    print_table_cell(results_cell);

                    /* Source Address*/
                    switch (pdma_benchmark_list[pdma_benchmark_index].source_address)
                    {
                        case L2_LIM0:
                            print_table_cell(memory_descriptors[0]);
                            break;

                        case SCRATCHPAD0:
                            print_table_cell(memory_descriptors[1]);
                            break;

                        case CACHED_DDR0:
                            print_table_cell(memory_descriptors[2]);
                            break;

                        case NON_CACHED_DDR0:
                            print_table_cell(memory_descriptors[3]);
                            break;

                        default:
                            sprintf(
                                results_cell,
                                "0x%lx",
                                (uint64_t)pdma_benchmark_list[pdma_benchmark_index].source_address);
                            print_table_cell(results_cell);
                            break;
                    }

                    /* Destination Address*/
                    switch (pdma_benchmark_list[pdma_benchmark_index].destination_address)
                    {
                        case L2_LIM0:
                            print_table_cell(memory_descriptors[0]);
                            break;

                        case L2_LIM1:
                            print_table_cell(memory_descriptors[0]);
                            break;

                        case SCRATCHPAD0:
                            print_table_cell(memory_descriptors[1]);
                            break;

                        case SCRATCHPAD1:
                            print_table_cell(memory_descriptors[1]);
                            break;

                        case CACHED_DDR0:
                            print_table_cell(memory_descriptors[2]);
                            break;

                        case CACHED_DDR1:
                            print_table_cell(memory_descriptors[2]);
                            break;

                        case NON_CACHED_DDR0:
                            print_table_cell(memory_descriptors[3]);
                            break;

                        case NON_CACHED_DDR1:
                            print_table_cell(memory_descriptors[3]);
                            break;

                        default:
                            sprintf(results_cell,
                                    "0x%lx",
                                    (uint64_t)pdma_benchmark_list[pdma_benchmark_index]
                                        .destination_address);
                            print_table_cell(results_cell);
                            break;
                    }

                    if (TRANSFER_DATA_MISMATCH == pdma_transfer_data_integrity_check)
                    {
                        print_table_cell("Fail");
                    }
                    else
                    {
                        print_table_cell("Pass");
                    }

                    sprintf(results_cell, "%ld", (uint64_t)pdma_transfer_rate);
                    print_table_cell(results_cell);
                    MSS_UART_polled_tx_string(uart1, "\r\n");

                    current_transfer_size += pdma_benchmark_list[pdma_benchmark_index].step_size;
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
