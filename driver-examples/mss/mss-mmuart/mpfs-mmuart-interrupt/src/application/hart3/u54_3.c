/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_3.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on u54_3
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

extern struct mss_uart_instance* p_uartmap_u54_3;

volatile uint32_t count_sw_ints_h3 = 0U;

const uint8_t polled_message3[] =
        "Hart 3 was taken out of WFI from uart2 user tx handler.\r\n";

/* Main function for the hart3(U54_3 processor).
 * Application code running on hart3 is placed here
 *
 * The hart3 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void u54_3(void)
{
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */

    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart in WFI. */
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */

    clear_soft_interrupt();

    __enable_irq();

    MSS_UART_init(p_uartmap_u54_3,
        MSS_UART_115200_BAUD,
        MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_polled_tx(p_uartmap_u54_3,polled_message3,
            sizeof(polled_message3));

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

/* hart3 software interrupt handler */
void Software_h3_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h3++;
}
