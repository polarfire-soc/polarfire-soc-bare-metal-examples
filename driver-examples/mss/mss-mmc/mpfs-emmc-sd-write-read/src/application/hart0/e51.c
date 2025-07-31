/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51.
 *
 * Please refer to README.md file for more details
 */
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"
#include "inc/uart_mapping.h"

extern struct mss_uart_instance* p_uartmap_e51;
volatile uint32_t count_sw_ints_h0 = 0U;

/* Main function for the hart0(E51 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 is in WFI while booting, hart0 brings it out of WFI when it raises
 * the first Software interrupt.
 */
void e51(void)
{
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    mss_config_clk_rst(
            MSS_PERIPH_MMUART_E51,
            (uint8_t) MPFS_HAL_FIRST_HART,
            PERIPHERAL_ON);

    MSS_UART_init(p_uartmap_e51,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(
            p_uartmap_e51,
            (const uint8_t*)
                "\r\nPlease observe UART-1 for application messages\r\n"
            );

    /* Raise software interrupt to wake hart 1 */
    raise_soft_interrupt(1U);

    __enable_irq();

    while (1U)
    {

        icount++;
        if (0x100000U == icount)
        {
            icount = 0U;
        }
    }

    /* never return */
}

/* hart0 Software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h0++;
}
