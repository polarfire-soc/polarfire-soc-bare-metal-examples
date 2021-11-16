/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 * PolarFire SoC MSS RTC Time example project
 * Application code running on U54_1
 */

#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_rtc/mss_rtc.h"

/* Constant used for setting RTC control register. */
#define BIT_SET 0x00010000U

/* 1MHz clock is RTC clock source. */
#define RTC_PERIPH_PRESCALER              (1000000u - 1u)

/* Constant used for setting RTC control register. */
#define BIT_SET 0x00010000U

/* 1MHz clock is RTC clock source. */
#define RTC_PERIPH_PRESCALER              (1000000u - 1u)

uint8_t display_buffer[100];

/******************************************************************************
 *  Greeting messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
        "\r\n\r\n\t  ******* PolarFire SoC RTC Time Example *******\n\n\n\r\
The example project demonstrate the RTC time mode. The UART\r\n\
message will be displayed at each second. \r\n\n\n\
";

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here.
 */
void u54_1(void)
{
    mss_rtc_calender_t calendar_count;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /*Put this hart into WFI.*/
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();
#endif

    PLIC_init();
    __enable_irq();

    PLIC_SetPriority(RTC_WAKEUP_PLIC, 2);

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_LAST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_RTC, (uint8_t) MPFS_HAL_LAST_HART, PERIPHERAL_ON);

    MSS_UART_init(&g_mss_uart1_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
    );

    MSS_UART_polled_tx_string(&g_mss_uart1_lo, g_greeting_msg);

    SYSREG->RTC_CLOCK_CR &= ~BIT_SET;
    SYSREG->RTC_CLOCK_CR = LIBERO_SETTING_MSS_EXT_SGMII_REF_CLK / LIBERO_SETTING_MSS_RTC_TOGGLE_CLK;
    SYSREG->RTC_CLOCK_CR |= BIT_SET;

    /* Initialize RTC. */
    MSS_RTC_init(MSS_RTC_LO_BASE, MSS_RTC_CALENDAR_MODE, RTC_PERIPH_PRESCALER );

    /* Enable RTC to start incrementing. */
    MSS_RTC_start();

    for (;;)
    {
        volatile uint32_t rtc_count_updated;

        /* Update displayed time if value read from RTC changed since last read.*/
        rtc_count_updated = MSS_RTC_get_update_flag();
        if(rtc_count_updated)
        {
            MSS_RTC_get_calendar_count(&calendar_count);
            snprintf((char *)display_buffer, sizeof(display_buffer),
                      "Seconds: %02d",(int)(calendar_count.second));

            MSS_UART_polled_tx_string (&g_mss_uart1_lo, display_buffer);
            MSS_UART_polled_tx_string (&g_mss_uart1_lo, "\r\n");
            MSS_RTC_clear_update_flag();
        }
    }
    /* never return*/
}

