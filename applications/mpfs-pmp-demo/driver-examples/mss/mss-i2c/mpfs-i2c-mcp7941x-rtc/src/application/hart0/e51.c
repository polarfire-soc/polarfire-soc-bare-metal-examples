/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 */

#include "mpfs_hal/mss_hal.h"

#include "drivers/mss/mss_mmuart/mss_uart.h"

const uint8_t g_info_string[] =
        " \r\n\r\n------------------------------------\
---------------------------------\r\n\r\n\
 Please observe UART1 as application is using UART1 as\
 User-Interface\r\n\r\n--------------------------------\
-------------------------------------\r\n";

void e51(void)
{

    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;


    (void) mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) 1, PERIPHERAL_ON);

    /* info message on uart0 */

    MSS_UART_init( &g_mss_uart0_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(&g_mss_uart0_lo, g_info_string);

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Clear pending software interrupt in case there was any. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Raise software interrupt to wake hart 1 */
    raise_soft_interrupt(1U);

    __enable_irq();
#endif

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */

    while(1U)
    {
        icount++;

        if(0x100000U == icount)
        {
            icount = 0U;
        }
    }

  /* Never return */
}

/* hart1 software interrupt handler */
void Software_h0_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
}
