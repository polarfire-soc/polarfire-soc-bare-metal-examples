/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 * Example project demonstrating the use of Watchdog interrupts.
 * Please refer README.txt in the root folder of this example project
 *
 */

#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_watchdog/mss_watchdog.h"
#include "drivers/mss_mmuart/mss_uart.h"

static uint8_t display_buffer[100];
mss_watchdog_config_t wd0lo_config;
uint64_t uart_lock;
uint64_t wd_lock;
volatile uint8_t h0_triggered = 0u, h0_mvrp =0u, h0_plic_mvrp = 0u;
volatile uint8_t h0_plic_triggered = 0u, h1_plic_mvrp = 0u;
volatile uint8_t h1_plic_triggered = 0u, h2_plic_mvrp = 0u, h2_plic_triggered = 0u;
volatile uint8_t h1_triggered = 0u, h1_mvrp = 0u, h2_triggered = 0u, h2_mvrp = 0u;

#define RX_BUFF_SIZE    64

const uint8_t g_message2[] =
"\r\nWD0 is configured by E51 (hart0)\r\n\
WD1 is configured by U54_1(hart1)\r\n\
WD2 is configured by U54_2(hart2)\r\n\
\r\n\
The system will reset after WD0 Times out. \r\n";

/* Interrupt handlers */
void wdog0_mvrp_e51_local_IRQHandler_10(void)
{
    h0_mvrp = 1;

    /* Un-commenting the below statement will refresh the Watchdog and then the
     * timer will be reloaded to original TIME value and start decrementing.
     * This will avoid the timeout interrupt and RESET.
     *
     * Not reloading the Watchdog will make the down counter to pass below
     * trigger value and cause a timeout interrupt. When the down counter rolls
     * down to zero, it would generate a RESET.
     * */

    /* MSS_WD_reload(MSS_WDOG0_LO); */

    MSS_WD_clear_mvrp_irq(MSS_WDOG0_LO);
}

void wdog0_tout_e51_local_IRQHandler_9(void)
{
    h0_triggered = 1u;
    MSS_WD_clear_timeout_irq(MSS_WDOG0_LO);
}

uint8_t wdog0_tout_plic_IRQHandler(void)
{
    h0_plic_triggered = 1u;
    MSS_WD_clear_timeout_irq(MSS_WDOG0_LO);
    return(1u);
}

uint8_t wdog0_mvrp_plic_IRQHandler(void)
{
    h0_plic_mvrp = 1u;
    MSS_WD_clear_mvrp_irq(MSS_WDOG0_LO);
    return(1u);
}

uint8_t wdog1_tout_plic_IRQHandler(void)
{
    h1_plic_triggered = 1u;
    MSS_WD_clear_timeout_irq(MSS_WDOG0_LO);
    return(1u);
}

uint8_t wdog1_mvrp_plic_IRQHandler(void)
{
    h1_plic_mvrp = 1u;
    MSS_WD_clear_mvrp_irq(MSS_WDOG0_LO);
    return(1u);
}

uint8_t wdog2_tout_plic_IRQHandler(void)
{
    h2_plic_triggered = 1u;
    MSS_WD_clear_timeout_irq(MSS_WDOG0_LO);
    return(1u);
}

uint8_t wdog2_mvrp_plic_IRQHandler(void)
{
    h2_plic_mvrp = 1u;
    MSS_WD_clear_mvrp_irq(MSS_WDOG0_LO);
    return(1u);
}

/* Created for convenience. Uses Polled method of UART TX */
void uart_tx_with_mutex (mss_uart_instance_t * this_uart,
                                uint64_t mutex_addr,
                                const uint8_t * pbuff)
{
    mss_take_mutex(mutex_addr);
    MSS_UART_polled_tx_string(this_uart, pbuff);
    mss_release_mutex(mutex_addr);
}

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here.
 * UART0 local interrupt is enabled on hart0.
 * In the respective U54 harts, local interrupts of the corresponding UART
 * are enabled. e.g. in u54_1.c local interrupt of UART1 is enabled. */
void e51(void)
{
    uint32_t current_value;
    uint32_t icount;

    mss_init_mutex((uint64_t)&wd_lock);
    mss_init_mutex((uint64_t)&uart_lock);

    __enable_irq();

    uint32_t hartid = read_csr(mhartid);

    if(hartid == 0)
    {
        SYSREG->SOFT_RESET_CR &=  ~(1u << 5u); /* mmuart0 */

        /* Turn on peripheral clocks */
        SYSREG->SOFT_RESET_CR &= ~(SOFT_RESET_CR_MMUART0_MASK |\
                SOFT_RESET_CR_MMUART1_MASK |\
                SOFT_RESET_CR_MMUART2_MASK |\
                SOFT_RESET_CR_MMUART3_MASK |\
                SOFT_RESET_CR_MMUART4_MASK |\
                SOFT_RESET_CR_CFM_MASK);

        /* Turn on peripheral clocks */
        SYSREG->SUBBLK_CLOCK_CR |= (SUBBLK_CLOCK_CR_MMUART0_MASK |\
                SUBBLK_CLOCK_CR_MMUART1_MASK |\
                SUBBLK_CLOCK_CR_MMUART2_MASK |\
                SUBBLK_CLOCK_CR_MMUART3_MASK |\
                SUBBLK_CLOCK_CR_MMUART4_MASK |\
                SUBBLK_CLOCK_CR_CFM_MASK);

        mss_take_mutex(uart_lock);
        MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_115200_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
        mss_release_mutex(uart_lock);

        uart_tx_with_mutex (&g_mss_uart0_lo, (uint64_t)&uart_lock,
             "\n\n\r    ****     PolarFire SoC MSS Watchdog Example      ****\r\n");

        uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock, g_message2);

        PLIC_init();

        /* Enable MVRP Interrupt on PLIC */
        PLIC_SetPriority(WDOG0_MRVP_PLIC, 2);
        PLIC_EnableIRQ(WDOG0_MRVP_PLIC);

        /* Enable MVRP Interrupt on PLIC */
        PLIC_SetPriority(WDOG0_TOUT_PLIC, 2);
        PLIC_EnableIRQ(WDOG0_TOUT_PLIC);

        /* Enable the MVRP local interrupt */
        __enable_local_irq(WDOG0_MVRP_E51_INT);

        /* The timeout interrupt is enabled in WD module by default. We still
         * need to enable the corresponding local interrupt on E51 */
        __enable_local_irq((int8_t)WDOG0_TOUT_E51_INT);

        /* Software interrupt to hart2. The first software interrupt will
         * bring the hart2 out of WFI. */
        raise_soft_interrupt(2u);
        /* Bring the hart1 out of WFI.*/
        raise_soft_interrupt(1u);

        /* Reading the default config */
        MSS_WD_get_config(MSS_WDOG0_LO, &wd0lo_config);

        /* Set such that the MVRP interrupt will happen after ~13 seconds after
         * reset and the Trigger interrupt will happen after ~25 seconds */
        wd0lo_config.forbidden_en = MSS_WDOG_DISABLE;
        wd0lo_config.timeout_val = 0x3e0u;
        wd0lo_config.mvrp_val = (10000000UL);
        wd0lo_config.time_val = (2UL*10000000UL);

        mss_take_mutex((uint64_t)&wd_lock);
        MSS_WD_configure(MSS_WDOG0_LO, &wd0lo_config);
        MSS_WD_reload(MSS_WDOG2_LO);
        MSS_WD_enable_mvrp_irq(MSS_WDOG0_LO);
        mss_release_mutex((uint64_t)&wd_lock);

        uart_tx_with_mutex (&g_mss_uart0_lo,  (uint64_t)&uart_lock,
                            "\r\nMSS Watchdog0 Configured \r\n");

        while(1u)
        {
            icount++;

            if (0x90000u == icount)
            {
                icount = 0u;
                current_value = MSS_WD_current_value(MSS_WDOG0_LO);
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "WD0 value = %x\r\n", *(uint32_t*)0x20001000);

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);

                current_value = MSS_WD_current_value(MSS_WDOG1_LO);
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "WD1 value = %x\r\n", *(uint32_t*)0x20101000);

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);

                current_value = MSS_WD_current_value(MSS_WDOG2_LO);
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "WD2 value = %x\r\n", *(uint32_t*)0x20103000);

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h0_mvrp)
            {
                h0_mvrp = 0;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H0 MVRP Local\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h0_triggered)
            {
                h0_triggered = 0;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H0 timeout Local\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h1_mvrp)
            {
                h1_mvrp = 0;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H1 MVRP Local\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h1_triggered)
            {
                h1_triggered = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H1 timeout Local\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h0_plic_mvrp)
            {
                h0_plic_mvrp = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H0 MVRP PLIC\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h0_plic_triggered)
            {
                h0_plic_triggered = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                          "H0 timeout PLIC\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h1_plic_mvrp)
            {
                h1_plic_mvrp = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                          "H1 MVRP PLIC\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h1_plic_triggered)
            {
                h1_plic_triggered = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H1 timeout PLIC\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h2_plic_mvrp)
            {
                h2_plic_mvrp = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H2 MVRP PLIC\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h2_plic_triggered)
            {
                h2_plic_triggered = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                         "H2 timeout PLIC\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h2_mvrp)
            {
                h2_mvrp = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                          "H2 MVRP Local\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }

            if(h2_triggered)
            {
                h2_triggered = 0u;
                snprintf((char *)display_buffer, sizeof(display_buffer),
                          "H2 timeout Local\r\n");

                uart_tx_with_mutex (&g_mss_uart0_lo,
                                    (uint64_t)&uart_lock,
                                    display_buffer);
            }
        }
    }
}

void maintenance_e51_local_IRQHandler_0(void)
{
    volatile uint32_t reg = 0u;
    reg = SYSREG->MAINTENANCE_INT_SR;
}
