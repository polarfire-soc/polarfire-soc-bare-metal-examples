/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 * PolarFire SoC MSS RTC interrupt example project
 */

#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"
#include "drivers/mss_rtc/mss_rtc.h"

/* Constant used for setting RTC control register. */
#define BIT_SET 0x00010000U

/* 1MHz clock is RTC clock source. */
#define RTC_PERIPH_PRESCALER              (1000000u - 1u)

uint64_t uart_lock;

/******************************************************************************
 *  Greeting messages displayed over the UART terminal.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\t  ******* PolarFire SoC RTC Interrupt Example *******\n\n\n\r\
The example project demonstrate the periodic RTC interrupt. The UART\r\n\
message will be printed at each RTC interrupt occurrence. \r\n\n\n\
";

/* RTC wakeup PLIC handler.
 */
uint8_t rtc_wakeup_plic_IRQHandler(void)
{
    /* Trigger the arrow display function. */
    MSS_UART_polled_tx_string(&g_mss_uart0_lo, "RTC interrupt\n\r");

    /* Clear RTC match interrupt. */
    MSS_RTC_clear_irq();

    return EXT_IRQ_KEEP_ENABLED;
}

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here.
 */
void e51(void)
{
    uint32_t alarm_period = 1u;
    uint32_t temp;

    PLIC_init();
    __enable_irq();
    PLIC_SetPriority(RTC_WAKEUP_PLIC, 2);

    SYSREG->SUBBLK_CLOCK_CR = 0xffffffff;
    SYSREG->SOFT_RESET_CR &= ~((1u << 5u) | (1u << 18u)) ; /* RTC and MMUART0 */

    MSS_UART_init(&g_mss_uart0_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(&g_mss_uart0_lo, g_greeting_msg);

    temp = BIT_SET;
    SYSREG->RTC_CLOCK_CR &= ~BIT_SET;
    SYSREG->RTC_CLOCK_CR = LIBERO_SETTING_MSS_EXT_SGMII_REF_CLK / LIBERO_SETTING_MSS_RTC_TOGGLE_CLK;
    SYSREG->RTC_CLOCK_CR |= BIT_SET;

    /* Initialize RTC. */
    MSS_RTC_init(MSS_RTC_LO_BASE, MSS_RTC_BINARY_MODE, RTC_PERIPH_PRESCALER);

    /* Set initial RTC count and match values. */
    MSS_RTC_reset_counter();
    MSS_RTC_set_binary_count_alarm(alarm_period, MSS_RTC_PERIODIC_ALARM);

    /* Enable RTC wakeup interrupt. */
    MSS_RTC_enable_irq();

    /* Enable RTC to start incrementing. */
    MSS_RTC_start();

    /* The RTC periodic alarm is now set. You should be able to see
     * that the rtc_wakeup_plic_IRQHandler is getting called periodically.
     */
    for (;;)
    {

    }

}
