/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 * Application code running on U54_1
 */

#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_rtc/mss_rtc.h"

/* Constant used for setting RTC control register. */
#define BIT_SET 0x00010000U

/* 1MHz clock is RTC clock source. */
#define RTC_PERIPH_PRESCALER              (1000000u - 1u)

mss_uart_instance_t *g_uart= &g_mss_uart1_lo;
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
    MSS_UART_polled_tx_string(g_uart, "RTC interrupt\n\r");

    /* Clear RTC match interrupt. */
    MSS_RTC_clear_irq();

    return EXT_IRQ_KEEP_ENABLED;
}

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here.
 */
void u54_1(void)
{
    uint32_t alarm_period = 1u;

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

    MSS_UART_init(g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(g_uart, g_greeting_msg);

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
