/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 * PolarFire SoC MSS RTC time example project
 */

#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"
#include "drivers/mss_rtc/mss_rtc.h"

/* Constant used for setting RTC control register. */
#define BIT_SET 0x00010000U

/* 1MHz clock is RTC clock source. */
#define RTC_PERIPH_PRESCALER              (1000000u - 1u)

/* Constant used for setting RTC control register. */
#define BIT_SET 0x00010000U

/* 1MHz clock is RTC clock source. */
#define RTC_PERIPH_PRESCALER              (1000000u - 1u)

uint64_t uart_lock;
uint8_t display_buffer[100];

/******************************************************************************
 *  Greeting messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
        "\r\n\r\n\t  ******* PolarFire SoC RTC Time Example *******\n\n\n\r\
The example project demonstrate the RTC time mode. The UART\r\n\
message will be displayed at each second. \r\n\n\n\
";

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here.
 */
void e51(void)
{
    mss_rtc_calender_t calendar_count;

    /* All clocks ON */
    SYSREG->SUBBLK_CLOCK_CR = 0xffffffff;

    SYSREG->SOFT_RESET_CR &= ~((1u << 5u) | (1u << 18u)) ; /* RTC and MMUART0 */

    MSS_UART_init(&g_mss_uart0_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
    );

    PLIC_init();
    __enable_irq();
    PLIC_SetPriority(RTC_WAKEUP_PLIC, 2);

    MSS_UART_polled_tx_string(&g_mss_uart0_lo, g_greeting_msg);

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

            MSS_UART_polled_tx_string (&g_mss_uart0_lo, display_buffer);
            MSS_UART_polled_tx_string (&g_mss_uart0_lo, "\r\n");
            MSS_RTC_clear_update_flag();
        }
    }
    /* never return*/
}

