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
 * Code running on U54 second hart
 *
 */
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

volatile uint32_t count_sw_ints_h2 = 0U;

/* Main function for the HART2(U54_2 processor).
 * Application code running on HART2 is placed here
 *
 * The HART2 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_2(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;
    uint64_t start_address = (uint64_t)(&__text_start);

    /* Clear pending software interrupt in case there was any.
        Enable only the software interrupt so that the E51 core can bring this
        core out of WFI by raising a software interrupt. */
     clear_soft_interrupt();
     set_csr(mie, MIP_MSIP);

     /*
      * Put this hart into WFI.
      * The main hart will wake-up
      */
     do
     {
         __asm("wfi");
     }while(0 == (read_csr(mip) & MIP_MSIP));

    /* Turn on UART1 clock and take out of reset */
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    MSS_UART_init( &g_mss_uart2_lo,
                   MSS_UART_115200_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(&g_mss_uart2_lo, (const uint8_t*)info_string);

    while (1U)
    {
        icount++;

        if (0x100000U == icount)
        {
            icount = 0U;
            sprintf(info_string,"Hart = %d, Readtime = %d,   ", hartid, readmtime());
            MSS_UART_polled_tx(&g_mss_uart2_lo, info_string,strlen(info_string));

            if(start_address >= 0x08000000UL && start_address <= 0x081FFFFFUL)
            {
                sprintf(info_string,"Executing from LIM \r\n");
                MSS_UART_polled_tx(&g_mss_uart2_lo, info_string,strlen(info_string));
            }
            else if(start_address >= 0x0A000000UL && start_address <= 0x0A1FFFFFUL)
            {
                sprintf(info_string,"Executing from Scratchpad \r\n");
                MSS_UART_polled_tx(&g_mss_uart2_lo, info_string,strlen(info_string));
            }
            else if(start_address >= 0x80000000UL && start_address <= 0xBFFFFFFFUL)
            {
                sprintf(info_string,"Executing from DDR \r\n");
                MSS_UART_polled_tx(&g_mss_uart2_lo, info_string,strlen(info_string));
            }
            else
            {
                ASSERT(0);
            }

#ifdef TEST_DDR_ACCESS
            ddr_read_write (small_ver);
#endif

        }
    }
    /* never return */
}

/* HART2 Software interrupt handler */
void Software_h2_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h2++;
}
