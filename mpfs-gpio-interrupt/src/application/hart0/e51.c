/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 * PolarFire SoC MSS GPIO interrupt example project
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

/******************************************************************************
 * Instruction message. This message will be displayed on the UART terminal 
   when the program starts.
 *****************************************************************************/
const uint8_t g_message1[] =
"";

uint8_t g_message2[] =
"\r\n\r\n\r\n **** PolarFire SoC MSS GPIO example ****\r\n\r\n\r\n\
This program is running on E51.\r\n\r\n\
Observe the LEDs blinking. LEDs toggle every time the SYSTICK timer expires\r\n\
\r\n\
Press 1 to generate interrupt on GPIO2 pin 30.\r\n\
Press 2 to generate interrupt on GPIO2 pin 31.\r\n\
Press 3 to generate interrupt on F2M_0 signal.\r\n";

#define RX_BUFF_SIZE    64
uint8_t g_rx_buff[RX_BUFF_SIZE] = {0};
volatile uint32_t count_sw_ints_h0 = 0U;
volatile uint8_t g_rx_size = 0;
uint64_t uart_lock;

/* Created for convenience. Uses Polled method of UART TX */
static void uart_tx_with_mutex (mss_uart_instance_t * this_uart,
                                uint64_t mutex_addr,
                                const uint8_t * pbuff,
                                uint32_t tx_size)
{
    mss_take_mutex(mutex_addr);
    MSS_UART_polled_tx(this_uart, pbuff, tx_size);
    mss_release_mutex(mutex_addr);
}

/* Main function for the hart0(E51 processor).
   Application code running on hart0 is placed here.

   On Icicle kit, apart from the UART menu, you can also use push button
   switches to generate GPIO interrupts. The mapping is as follows:
        push button SW1 - MSS_INT_F2M[0]
        push button SW2 - GPIO2_30
        push button SW3 - GPIO2_31
    */
void e51(void)
{
    int8_t info_string[100];
    uint64_t mcycle_start = 0U;
    uint64_t mcycle_end = 0U;
    uint64_t delta_mcycle = 0U;
    uint64_t hartid = read_csr(mhartid);
    uint8_t cnt, int_num;

    PLIC_init();

    __enable_irq();

    /* Turn on peripheral clocks */
    SYSREG->SUBBLK_CLOCK_CR |= (SUBBLK_CLOCK_CR_MMUART0_MASK |\
            SUBBLK_CLOCK_CR_MMUART1_MASK |\
            SUBBLK_CLOCK_CR_MMUART2_MASK |\
            SUBBLK_CLOCK_CR_MMUART3_MASK |\
            SUBBLK_CLOCK_CR_MMUART4_MASK |\
            SUBBLK_CLOCK_CR_GPIO0_MASK |\
            SUBBLK_CLOCK_CR_GPIO1_MASK |\
            SUBBLK_CLOCK_CR_GPIO2_MASK |\
            SUBBLK_CLOCK_CR_CFM_MASK);

    /* Remove soft reset */
    SYSREG->SOFT_RESET_CR   &= (uint32_t)~(SUBBLK_CLOCK_CR_MMUART0_MASK |\
            SUBBLK_CLOCK_CR_MMUART1_MASK |\
            SUBBLK_CLOCK_CR_MMUART2_MASK |\
            SUBBLK_CLOCK_CR_MMUART3_MASK |\
            SUBBLK_CLOCK_CR_MMUART4_MASK |\
            SUBBLK_CLOCK_CR_GPIO0_MASK |\
            SUBBLK_CLOCK_CR_GPIO1_MASK |\
            SUBBLK_CLOCK_CR_GPIO2_MASK |\
            SUBBLK_CLOCK_CR_CFM_MASK);

    /* All harts use MMUART0 to display messages on Terminal. This mutex helps
     * serializing MMUART0 accesses from multiple harts.*/
    mss_init_mutex((uint64_t)&uart_lock);

    MSS_UART_init( &g_mss_uart0_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
                       g_message1, sizeof(g_message1));

    uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
                       g_message2, sizeof(g_message2));


    mcycle_start = readmcycle();

    /* Configure Systick. The tick rate is configured in mss_sw_config.h */
    SysTick_Config();

    /* Making sure that the GPIO2 interrupts are routed to the PLIC instead of
     * GPIO0 and GPIO1.
     * Please see the mss_gpio.h for more description on how GPIO interrupts
     * are routed to the PLIC */
    SYSREG->GPIO_INTERRUPT_FAB_CR = 0xFFFFFFFFUL;

    PLIC_SetPriority_Threshold(0);

    for (int_num = 0u; int_num <= GPIO2_NON_DIRECT_PLIC; int_num++)
    {
        PLIC_SetPriority(GPIO0_BIT0_or_GPIO2_BIT0_PLIC_0 + int_num, 2u);
    }

    MSS_GPIO_init(GPIO2_LO);

    for (int cnt = 16u; cnt< 20u; cnt++)
    {
        MSS_GPIO_config(GPIO2_LO,
                        cnt,
                        MSS_GPIO_OUTPUT_MODE);
    }

    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_26, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_27, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_28, MSS_GPIO_OUTPUT_MODE);

    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_30,
                    MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_LEVEL_HIGH);

    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_31,
                    MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_LEVEL_HIGH);

    MSS_GPIO_set_outputs(GPIO2_LO, 0u);

    MSS_GPIO_enable_irq(GPIO2_LO, MSS_GPIO_30);
    MSS_GPIO_enable_irq(GPIO2_LO, MSS_GPIO_31);
    PLIC_SetPriority(FABRIC_F2H_0_PLIC, 2);
    PLIC_EnableIRQ(FABRIC_F2H_0_PLIC);

    while (1u)
    {
        g_rx_size = MSS_UART_get_rx(&g_mss_uart0_lo, g_rx_buff, sizeof(g_rx_buff));

        if (g_rx_size > 0u)
        {
            switch (g_rx_buff[0u])
            {
            case '1':
                MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_26, 1u);
                break;

            case '2':
                MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_27, 1u);
                break;

            case '3':
                MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_28, 1);
                break;

            default:
                /* Echo the characters received from the terminal */
                uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
                                   g_rx_buff, g_rx_size);
                break;
            }

            g_rx_size = 0;
        }
    }
}

uint8_t  gpio1_bit16_or_gpio2_bit30_plic_30_IRQHandler(void)
{
    uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
           "gpio2_pin 30 Interrupt\r\n", sizeof("gpio2_pin 30 Interrupt\r\n"));
    MSS_GPIO_set_outputs(GPIO2_LO, 0u);
    return EXT_IRQ_KEEP_ENABLED;
}

uint8_t gpio1_bit17_or_gpio2_bit31_plic_31_IRQHandler(void)
{
    uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
           "gpio2_pin 31 Interrupt\r\n", sizeof("gpio2_pin 31 Interrupt\r\n"));
    MSS_GPIO_set_outputs(GPIO2_LO, 0u);
    return EXT_IRQ_KEEP_ENABLED;
}

uint8_t fabric_f2h_0_plic_IRQHandler(void)
{
    uart_tx_with_mutex(&g_mss_uart0_lo, (uint64_t)&uart_lock,
               "f2h_0 interrupt\r\n", sizeof("f2h_0 interrupt\r\n"));
    MSS_GPIO_set_outputs(GPIO2_LO, 0u);
    return EXT_IRQ_KEEP_ENABLED;
}

void SysTick_Handler_h0_IRQHandler(void)
{
    uint32_t hart_id = read_csr(mhartid);
    static volatile uint8_t value = 0u;

    if (0u == hart_id)
    {
        if(0u == value)
        {
            value = 0x01u;
        }
        else
        {
            value = 0x00u;
        }

        MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_16, value);
    }
}
