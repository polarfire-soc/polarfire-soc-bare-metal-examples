/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 * Example project demonstrating the use of polled and interrupt driven
 * transmission and reception over MMUART. Please refer README.txt in the root
 * folder of this example project
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"

/******************************************************************************
 * Instruction message. This message will be transmitted over the UART when
 * the program starts.
 *****************************************************************************/
const uint8_t g_message1[] =
        "\r\n\r\n\r\n **** PolarFire SoC MSS MMUART example ****\r\n\r\n\r\n";

const uint8_t g_message2[] =
        "This program is run from E51\r\n\
Type 0  Show hart 0 debug message\r\n\
Type 1  Raise sw int hart 1\r\n\
Type 2  Raise sw int hart 2\r\n\
Type 3  Raise sw int hart 3\r\n\
Type 4  Raise sw int hart 4\r\n\
Type 5  Show this menu\r\n\
";

#define RX_BUFF_SIZE    64
uint8_t g_rx_buff[RX_BUFF_SIZE] = {0};
volatile uint32_t count_sw_ints_h0 = 0U;
volatile uint8_t g_rx_size = 0;
uint64_t uart_lock;
static volatile uint32_t irq_cnt = 0;

/* Created for convenience. Uses Polled method of UART TX */
static void
uart_tx_with_mutex
(
    mss_uart_instance_t * this_uart,
    uint64_t mutex_addr,
    const uint8_t * pbuff,
    uint32_t tx_size
)
{
    mss_take_mutex(mutex_addr);
    MSS_UART_polled_tx(this_uart, pbuff, tx_size);
    mss_release_mutex(mutex_addr);
}

/* This is the handler function for the UART RX interrupt.
 * In this example project UART0 local interrupt is enabled on hart0.
 */
void uart0_rx_handler (mss_uart_instance_t * this_uart)
{
    uint32_t hart_id = read_csr(mhartid);
    int8_t info_string[50];

    irq_cnt++;
    sprintf(info_string,"UART0 Interrupt count = 0x%x \r\n\r\n", irq_cnt);

    /* This will execute when interrupt from hart 0 is raised */
    mss_take_mutex((uint64_t)&uart_lock);
    g_rx_size = MSS_UART_get_rx(this_uart, g_rx_buff, sizeof(g_rx_buff));
    mss_release_mutex((uint64_t)&uart_lock);

    uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
                        info_string, strlen(info_string));
}

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here.
 * MMUART0 local interrupt is enabled on hart0.
 * In the respective U54 harts, local interrupts of the corresponding MMUART
 * are enabled. e.g. in U54_1.c local interrupt of MMUART1 is enabled. */
void e51 (void)
{
    int8_t info_string[100];
    uint64_t mcycle_start = 0U;
    uint64_t mcycle_end = 0U;
    uint64_t delta_mcycle = 0U;
    uint64_t hartid = read_csr(mhartid);

    /* Bring all the MMUARTs out of Reset */
    SYSREG->SOFT_RESET_CR &= ~( (1u << 5u) | (1u << 6u) |
                                (1u << 7u) | (1u << 8u) | (1u << 9u));

    /* All clocks ON */
    SYSREG->SUBBLK_CLOCK_CR = 0xffffffff;

    __enable_irq();

    mss_init_mutex((uint64_t)&uart_lock);

    MSS_UART_init( &g_mss_uart0_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* Receive interrupt is enabled now. Please see uart0_rx_handler() for
     * more details */
    MSS_UART_set_rx_handler(&g_mss_uart0_lo,
                            uart0_rx_handler,
                            MSS_UART_FIFO_SINGLE_BYTE);

    MSS_UART_enable_local_irq(&g_mss_uart1_lo);

    /* Demonstrating polled MMUART transmission */
    uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
                       g_message1, sizeof(g_message1));

    mss_take_mutex((uint64_t)&uart_lock);

    /* Demonstrating interrupt method of transmission */
    MSS_UART_irq_tx(&g_mss_uart0_lo, g_message2, sizeof(g_message2));

    /* Makes sure that the previous interrupt based transmission is completed
     * Alternatively, you could register TX complete handler using
     * MSS_UART_set_tx_handler() */
    while (0u == MSS_UART_tx_complete(&g_mss_uart0_lo))
    {
        ;
    }

    mss_release_mutex((uint64_t)&uart_lock);
    mcycle_start = readmcycle();

    while (1u)
    {
        if (g_rx_size > 0u)
        {
            switch (g_rx_buff[0u])
            {

            case '0':
                mcycle_end      = readmcycle();
                delta_mcycle    = mcycle_end - mcycle_start;
                sprintf(info_string,"hart %ld, %ld delta_mcycle \r\n", hartid,
                        delta_mcycle);
                uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
                                   info_string, strlen(info_string));
                break;
            case '1':
                /* Software interrupt to hart1. The first software interrupt
                 * will bring the hart1 out of WFI. */
                raise_soft_interrupt(1u);
                break;
            case '2':
                /* Software interrupt to hart2. The first software interrupt
                 * will bring the hart2 out of WFI. */
                raise_soft_interrupt(2u);
                break;
            case '3':
                /* Software interrupt to hart3. The first software interrupt
                 * will bring the hart3 out of WFI. */
                raise_soft_interrupt(3u);
                break;
            case '4':
                /* Software interrupt to hart4. The first software interrupt
                 * will bring the hart4 out of WFI. */
                raise_soft_interrupt(4u);
                break;
            case '5':
                /* Demonstrating interrupt driven TX */
                MSS_UART_irq_tx(&g_mss_uart0_lo, g_message2,sizeof(g_message2));
                break;
            default:
                uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
                        g_rx_buff, g_rx_size);
                break;
            }

            g_rx_size = 0u;
        }
    }
}

/* hart0 Software interrupt handler */
void Software_h0_IRQHandler (void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h0++;
}
