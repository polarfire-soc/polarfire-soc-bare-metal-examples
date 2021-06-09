/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 */
/*******************************************************************************
 *
 * Code running on E51
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"

#ifndef SIFIVE_HIFIVE_UNLEASHED
#include "drivers/mss_mmuart/mss_uart.h"
#else
#include "drivers/FU540_uart/FU540_uart.h"
#endif

static volatile uint32_t count_sw_ints_h0 = 0U;
volatile uint32_t g_10ms_count;
#define MAX_BUFFER_SIZE 128
uint8_t data_block[256];

mss_uart_instance_t *g_uart= &g_mss_uart1_lo ;

/*******************************************************************************
 * Instruction message. This message will be transmitted over the UART to
 * HyperTerminal when the program starts.
 ******************************************************************************/
const uint8_t g_message[] =
        "\r\n\r\n\r\n\
MPFS HAL DDR example version 0.3.2\r\n\
This program is run from ddr in the E51\r\n\
Type 0  Show this menu\r\n\
";

uint64_t uart_lock;

/* Main function for the HART0(E51 processor).
 * Application code running on HART0 is placed here.
 */
void e51(void)
{
    int8_t info_string[100];
    uint64_t mcycle_start = 0U;
    uint64_t mcycle_end = 0U;
    uint64_t delta_mcycle = 0U;
    uint64_t hartid = read_csr(mhartid);
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;
    uint8_t debug_hart0 = 0U;

    /* Turn on UART0 clock */
    SYSREG->SUBBLK_CLOCK_CR |= SUBBLK_CLOCK_CR_MMUART1_MASK;
    /* Remove soft reset */
    SYSREG->SOFT_RESET_CR   &= ~SOFT_RESET_CR_MMUART1_MASK;

    /* This mutex is used to serialize accesses to UART0 when all harts want to
       TX/RX on UART0. This mutex is shared across all harts. */
    mss_init_mutex((uint64_t)&uart_lock);

    MSS_UART_init( &g_mss_uart1_lo,
    MSS_UART_115200_BAUD,
    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string (&g_mss_uart1_lo, g_message);

    SysTick_Config();

    /* Start the other harts with appropriate UART input from user */
    while (1)
    {
      mcycle_start = readmcycle();

      if (1u == debug_hart0)
      {
          debug_hart0 = 0U;
          sprintf(info_string,"Hart %ld, %ld delta_mcycle %ld mtime\r\n",
          hartid, delta_mcycle, readmtime());
          mss_take_mutex((uint64_t)&uart_lock);
          MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,strlen(info_string));
          mss_release_mutex((uint64_t)&uart_lock);
      }

        mss_take_mutex((uint64_t)&uart_lock);
        rx_size = MSS_UART_get_rx(&g_mss_uart1_lo, rx_buff, sizeof(rx_buff));
        mss_release_mutex((uint64_t)&uart_lock);

        if (rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case '0':
                    mss_take_mutex((uint64_t)&uart_lock);
                    MSS_UART_polled_tx_string (&g_mss_uart1_lo, g_message );
                    mss_release_mutex((uint64_t)&uart_lock);
                    break;

                default:
                    /* echo input */
                    mss_take_mutex((uint64_t)&uart_lock);
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo, rx_buff);
                    mss_release_mutex((uint64_t)&uart_lock);
                    break;
            }
        }

        mcycle_end    = readmcycle();
        delta_mcycle = mcycle_end - mcycle_start;
    }
}

/* HART0 Software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h0++;
}

#if defined(TARGET_RENODE)
#define TIMER_INCREMENT 35 /* Adjust this to get the C prompt from YMODEM giving
                            * one C every second or so */
#else
#define TIMER_INCREMENT 1
#endif

void SysTick_Handler_h0_IRQHandler(void)
{
    g_10ms_count += TIMER_INCREMENT;
    if(g_10ms_count < TIMER_INCREMENT)
        g_10ms_count = 0;
}
