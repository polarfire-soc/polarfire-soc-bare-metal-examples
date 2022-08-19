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
 * Code running on U54 first hart
 *
 */
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"
#include "mpfs_hal/common/mss_util.h"


volatile uint32_t count_sw_ints_h1 = 0U;
mss_uart_instance_t *g_debug_uart= &g_mss_uart1_lo ;

/* Main function for the HART1(U54_1 processor).
 * Application code running on HART1 is placed here
 *
 * The HART1 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_1(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;
    uint64_t start_address = (uint64_t)(&__text_start);

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
       can enable and use any interrupts as required */
    clear_soft_interrupt();

    /* Turn on UART1 clock and take out of reset */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    MSS_UART_init( &g_mss_uart1_lo,
				   MSS_UART_115200_BAUD,
				   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(&g_mss_uart1_lo, (const uint8_t*)info_string);

    /*
     * Start the second hart
     */
    raise_soft_interrupt(2U);

    /*
     * Loop here forever
     */
    while (1U)
    {
        icount++;
        if (0x7FFFFFU == icount)
        {
            icount = 0U;
            sprintf(info_string,"Hart = %d, Readtime = %d,   ", hartid, readmtime());
            MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,strlen(info_string));

            if(start_address >= 0x08000000UL && start_address <= 0x081FFFFFUL)
            {
                sprintf(info_string,"Executing from LIM \r\n");
                MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,strlen(info_string));
            }
            else if(start_address >= 0x0A000000UL && start_address <= 0x0A1FFFFFUL)
            {
                sprintf(info_string,"Executing from Scratchpad \r\n");
                MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,strlen(info_string));
            }
            else if(start_address >= 0x80000000UL && start_address <= 0xBFFFFFFFUL)
            {
                sprintf(info_string,"Executing from DDR \r\n");
                MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,strlen(info_string));
            }
            else
            {
                ASSERT(0);
            }


        }
    }
    /* never return */
}

/* HART1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}

