/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file e51.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on e51
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
volatile uint32_t count_sw_ints_h0 = 0U;

#include "inc/uart_mapping.h"
extern struct mss_uart_instance* p_uartmap_e51;

/* Main function for the hart0(e51 processor).
 * Application code running on hart0 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt.
 */

#define RX_BUFF_SIZE    16U

const uint8_t g_message3[] =
        " \r\n\r\n-------------------------------------------------------------\
--------\r\n\r\nApplication code executes from the U54_1. \r\n "
        "Please observe its UART for application messages.\r\n\r\n------------------\
---------------------------------------------------\r\n";

void e51(void)
{
    volatile uint32_t icount = 0U;
    uint64_t hartid = read_csr(mhartid);

    (void) mss_config_clk_rst(MSS_PERIPH_MMUART_E51, (uint8_t) 1, PERIPHERAL_ON);

    MSS_UART_init(p_uartmap_e51,
    MSS_UART_115200_BAUD,
    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* Message on uart0 */
    MSS_UART_polled_tx(p_uartmap_e51, g_message3,
            sizeof(g_message3));

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /* Clear pending software interrupt in case there was any. */

    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Raise software interrupt to wake hart 1 */
    raise_soft_interrupt(1U);

    /* Raise software interrupt to wake hart 2 */
    raise_soft_interrupt(2U);

    __enable_irq();

#endif

    while (1U)
          {

          }
    /* never return */
}

/* hart0 software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h0++;
}
