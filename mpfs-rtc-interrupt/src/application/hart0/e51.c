/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 * PolarFire SoC MSS RTC interrupt example project
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"

volatile uint32_t count_sw_ints_h0 = 0U;

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here
 *
 * The hart0 is used in the application for bootup and wakeup the U54_1 when
 * the application is running from LIM/eNVM memory. In case of DDR the
 * bootloader application will perform the booting and start executing this
 * application from U54_1.
 */
void e51(void)
{
    volatile uint32_t icount = 0U;
    uint64_t hartid = read_csr(mhartid);
    uint32_t pattern_offset = 12U;
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /* Clear pending software interrupt in case there was any. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) MPFS_HAL_FIRST_HART,
                                                           PERIPHERAL_ON);

    MSS_UART_init( &g_mss_uart0_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(&g_mss_uart0_lo ,
            (const uint8_t*)"\r\nPlease observe UART-1 for application messages\r\n");

    /* Raise software interrupt to wake hart 1 */
    raise_soft_interrupt(1U);

    __enable_irq();
#endif

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
