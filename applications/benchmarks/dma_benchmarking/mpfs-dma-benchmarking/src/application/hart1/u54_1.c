/*******************************************************************************
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * This project demonstrates the performance of the PDMA transferring data to
 * various memory locations.
 * Please refer to the README.md file in the root folder of this example
 * project.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_pdma/mss_pdma.h"
#include "inc/common.h"
#include "benchmarking.h"

/*  Macros */
#define RX_BUFF_SIZE    64U
#define BYTES_TO_MB   1000000.0

/*  Global Variables */
volatile uint8_t g_rx_size = 0U;
static uint8_t g_rx_buff[RX_BUFF_SIZE] = { 0 };
static uint32_t benchmark_total = sizeof(benchmark_list)/sizeof(benchmark_list[0]);
static uint32_t pdma_error_interrupt_count = 0u;
static uint32_t benchmark_error_count = 0u;
static uint64_t end_mcycle = 0u;
mss_uart_instance_t *uart1 = &g_mss_uart1_lo;

typedef enum
{
    INIT,
    INIT_COMPLETE
} status_t;

typedef enum
{
    TRANSFER_NOT_DONE,
    TRANSFER_DONE
} pdma_done_interrupt_t;

typedef enum
{
    DATA_MATCH,
    DATA_MISMATCH
} data_verify_t;

pdma_done_interrupt_t transfer_status;

/*  Function prototypes */
double calculate_rate (uint64_t tfer_time, uint64_t tfer_size);
void configure_pdma (mss_pdma_channel_config_t *pdma_config_ch,
                        const benchmark_params_t *benchmark, uint32_t transfer_size);
static data_verify_t verify(benchmark_params_t, uint32_t);
void memory_init(benchmark_params_t, uint32_t);
static void pdma_isr(uint8_t interrupt_type);
void pdma_performance(void);
void print_results (benchmark_params_t, data_verify_t, double, uint32_t );
void print_table_cell(uint8_t*);
void print_table_header(void);
void print_error_counts(void);
void print_row (uint8_t *col[], uint32_t table_width);

const uint8_t selection_msg[] =
                    "\r\nPress SPACE to run PDMA performance benchmakrs\r\n";

void
u54_1
(
        void
)
{

    uint64_t hartid = read_csr(mhartid);
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /* Put this hart in WFI. */
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */
    clear_soft_interrupt();

#endif

    __enable_irq();

    /* Bring MMUARTs 0 and 1 out of Reset */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) MPFS_HAL_FIRST_HART,
                                                                PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART,
                                                                PERIPHERAL_ON);

    PLIC_init();
    __enable_irq();
    PLIC_SetPriority_Threshold(0);

    PLIC_SetPriority(DMA_CH0_DONE_IRQn, 2u);
    PLIC_SetPriority(DMA_CH0_ERR_IRQn, 1u);

    /* Enable PDMA Interrupts. */
    PLIC_EnableIRQ(DMA_CH0_DONE_IRQn);
    PLIC_EnableIRQ(DMA_CH0_ERR_IRQn);

    MSS_UART_init(uart1, MSS_UART_115200_BAUD,
    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    const uint8_t g_greeting_msg[] =
                    "\r\n\r\n\r\n **** PolarFire SoC DMA Benchmarking ****\r\n";

    MSS_UART_polled_tx_string (uart1, g_greeting_msg);

    MSS_UART_polled_tx_string (uart1, selection_msg);

    while (1u)
    {
        g_rx_size = MSS_UART_get_rx(uart1, g_rx_buff,
                                            sizeof(g_rx_buff));
        if (g_rx_size > 0u)
        {
            switch (g_rx_buff[0u])
            {
            case ' ':
                pdma_performance();
                MSS_UART_polled_tx_string (uart1, selection_msg);
                break;

            default:
                MSS_UART_polled_tx_string (uart1,
                                    "\r\n\r\nInvalid Option Selected!!\r\n");
                MSS_UART_polled_tx_string (uart1, selection_msg);
                break;
            }
            g_rx_size = 0u;
        }
    }
}

void
pdma_performance
(
    void
)
{
    uint32_t benchmark_num = 0u;
    uint32_t current_transfer_size = 0u;
    uint64_t start_mcycle = 0u;
    status_t status = INIT;
    data_verify_t data_check = DATA_MATCH;
    mss_pdma_channel_config_t pdma_config_ch;

    uint8_t msg_pdma_perf[] = "\r\nRunning PDMA performance test..\r\n";
    MSS_UART_polled_tx_string(uart1, msg_pdma_perf);

    print_table_header();

    while (benchmark_num < benchmark_total)
    {
        current_transfer_size = benchmark_list[benchmark_num].min_transfer_size;

        while (current_transfer_size <=
                                benchmark_list[benchmark_num].max_transfer_size)
        {

            if (INIT == status)
            {

                configure_pdma(&pdma_config_ch, &benchmark_list[benchmark_num],
                                                        current_transfer_size);

                memory_init(benchmark_list[benchmark_num],
                                                        current_transfer_size);

                if (MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_0,
                                            &pdma_config_ch, pdma_isr) != MSS_PDMA_OK)
                {
                    MSS_UART_polled_tx_string(uart1,
                                                "\r\nError: Setup Transfer!\r\n");
                    break;
                }

                start_mcycle = readmcycle();

                if (MSS_PDMA_start_transfer(MSS_PDMA_CHANNEL_0) != MSS_PDMA_OK)
                {
                    MSS_UART_polled_tx_string(uart1,
                                                "\r\nError: Start Transfer!\r\n");
                    break;
                }
                status = INIT_COMPLETE;
            }

            if (TRANSFER_DONE == transfer_status)
            {
                double transfer_rate = calculate_rate(
                            (end_mcycle - start_mcycle), current_transfer_size);

                data_check = verify(
                        benchmark_list[benchmark_num], current_transfer_size);
                benchmark_error_count += data_check;

                print_results(benchmark_list[benchmark_num],
                            data_check, transfer_rate, current_transfer_size);
                transfer_status = TRANSFER_NOT_DONE;
                status = INIT;
                current_transfer_size +=
                                        benchmark_list[benchmark_num].step_size;
            }
        }
        benchmark_num++;
    }
    print_error_counts();
}

static void
pdma_isr
(
        uint8_t interrupt_type
)
{
    /* Clear DONE interrupt flag */
    if (interrupt_type == PDMA_CH0_DONE_INT)
    {
        end_mcycle = readmcycle();
        MSS_PDMA_clear_transfer_complete_status(interrupt_type);
        transfer_status = TRANSFER_DONE;
    }
    /* Clear ERROR interrupt flag */
    else if (interrupt_type == PDMA_CH0_ERROR_INT)
    {
        MSS_PDMA_clear_transfer_error_status(interrupt_type);
        pdma_error_interrupt_count++;
    }
}

void
configure_pdma
(
    mss_pdma_channel_config_t *pdma_config_ch,
    const benchmark_params_t *benchmark,
    uint32_t transfer_size
)
{
    pdma_config_ch->num_bytes = transfer_size;
    pdma_config_ch->src_addr = (uint64_t) benchmark->src_addr;
    pdma_config_ch->dest_addr = (uint64_t) benchmark->dst_addr;

    pdma_config_ch->enable_done_int = 1u;
    pdma_config_ch->enable_err_int = 1u;
    pdma_config_ch->repeat = 0u;

#ifdef FORCE_ORDER
    pdma_config_ch->force_order = 1u;
#else 
    pdma_config_ch->force_order = 0u;
#endif

}

void
memory_init
(
    benchmark_params_t test,
    uint32_t transfer_size
)
{
    uint32_t i;
    uint8_t *src = (uint8_t *)test.src_addr;
    uint8_t *dst = (uint8_t *)test.dst_addr;

    /* set destination memory block to 0 */
    memset(((uint8_t *)test.dst_addr), 0x00, transfer_size);

    /* set pattern in source memory */
    for (i = 0; i < transfer_size; i++)
    {
        src[i] = (i & 0xFFu);
    }
}

static data_verify_t
verify
(
    benchmark_params_t test,
    uint32_t transfer_size
)
{
    for (uint32_t i = 0; i < (transfer_size / sizeof(uint32_t));
                                                    i = i + sizeof(uint32_t))
    {
       if (*( test.src_addr + i) != *( test.dst_addr + i))
        {
            return DATA_MISMATCH;
        }
    }
    return DATA_MATCH;
}

double
calculate_rate
(
    uint64_t clock_cycles,
    uint64_t transfer_size
)
{
    double seconds =
            clock_cycles
            / (double)LIBERO_SETTING_MSS_COREPLEX_CPU_CLK;  // seconds
    double transfer_rate = transfer_size / seconds;         // B/s
    transfer_rate /= BYTES_TO_MB;                           // MB/s
    return transfer_rate;
}

void
print_results
(
    benchmark_params_t test,
    data_verify_t mismatch_flag,
    double transfer_rate,
    uint32_t transfer_size
)
{
    uint8_t values[5][21] = {0};
    uint8_t *table_cells[5];

    sprintf(values[0], "%d", transfer_size);
    sprintf(values[1], "0x%lx", test.src_addr);
    sprintf(values[2], "0x%lx", test.dst_addr);
    sprintf(values[3], "%s", "Pass");
    sprintf(values[4], "%ld", (uint64_t) transfer_rate);

    if(DATA_MISMATCH == mismatch_flag)
    {
        sprintf(values[3], "%s", "Fail");
    }

    for (uint32_t x = 0; x < 5; x++)
    {
        table_cells[x] = values[x];
    }

    print_row(table_cells, 5);
}

void
print_row
(
    uint8_t *col[],
    uint32_t table_width
)
{
    for (uint32_t column = 0; column < table_width; column++)
    {
        print_table_cell(col[column]);
    }
    MSS_UART_polled_tx_string(uart1, "\r\n");
}

void
print_table_cell
(
        uint8_t* col_1
)
{
    uint8_t message[50] = {0};
    sprintf(message, " %-13s", col_1);
    MSS_UART_polled_tx_string(uart1, message );
}

void
print_error_counts
(
    void
)
{
    uint8_t errors_message[50] = {"\r\n\t\t- - - - -\t\t\r\n"};
    MSS_UART_polled_tx_string(uart1, errors_message);

    if (pdma_error_interrupt_count > 0)
    {
        sprintf(errors_message, "\r\n%-3d PDMA Error Interrupts Occurred\r\n",
                                                    pdma_error_interrupt_count);
        MSS_UART_polled_tx_string(uart1, errors_message);
        pdma_error_interrupt_count = 0u;
    }
    else
    {
        sprintf(errors_message, "\r\nNo PDMA Error Interrupts Occurred\r\n");
        MSS_UART_polled_tx_string(uart1, errors_message);
    }

    if(benchmark_error_count > 0)
    {
        sprintf(errors_message, "%-3d Benchmarking Tests FAILED!\r\n",
                                                        benchmark_error_count);
        MSS_UART_polled_tx_string(uart1, errors_message);
        benchmark_error_count = 0u;
    }
    else
    {
        sprintf(errors_message, "All Benchmarking Tests PASSED!\r\n\r\n");
        MSS_UART_polled_tx_string(uart1, errors_message);
    }

    sprintf(errors_message,"\r\n\t\t========\t\t\r\n\r\n");
    MSS_UART_polled_tx_string(uart1, errors_message);
}


void
print_table_header
(
    void
)
{
    uint8_t *header_first[] = {"Data", "Source", "Destination", "Test",
                                                                    "Transfer"};
    uint8_t *header_second[] = {"Size", "Address", "Address", "Result", "Rate"};
    uint8_t *header_third[] = {"(Bytes)", "", "", "", "(MB/s)"};

    MSS_UART_polled_tx_string (uart1, "===================================="
                                        "==============================\r\n");
    print_row(header_first, 5);
    print_row(header_second, 5);
    print_row(header_third, 5);

    MSS_UART_polled_tx_string (uart1, "===================================="
                                        "==============================\r\n");
}

