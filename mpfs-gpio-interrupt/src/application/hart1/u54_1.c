/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * PolarFire SoC MSS GPIO interrupt example project
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_gpio/mss_gpio.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

/******************************************************************************
 * Instruction message. These message will be displayed on the UART terminal 
   when the program starts.
 *****************************************************************************/


uint8_t g_message2[] =
"\r\n\r\n\r\n **** PolarFire SoC MSS GPIO example ****\r\n\r\n\r\n\
This program is running on u54_1.\r\n\r\n\
Observe the LEDs blinking. LEDs toggle every time the SYSTICK timer expires\r\n\
\r\n\
Press 1 to generate interrupt on GPIO2 pin 30.\r\n\
Press 2 to generate interrupt on GPIO2 pin 31.\r\n\
Press 3 to generate interrupt on F2M_0 signal.\r\n";

#define RX_BUFF_SIZE    64U

uint8_t g_rx_buff[RX_BUFF_SIZE] = {0};
volatile uint8_t g_rx_size = 0U;

/* Main function for the hart1(U54 processor).
 * Application code running on hart1 is placed here.
 * On Icicle kit, apart from the UART menu, you can also use push button
 * switches to generate GPIO interrupts. The mapping is as follows
 * push button SW1 - MSS_INT_F2M[0]
 * push button SW2 - GPIO2_30
 * push button SW3 - GPIO2_31
 */

void u54_1(void)
{
    uint64_t mcycle_start = 0U;
    uint64_t mcycle_end = 0U;
    uint64_t delta_mcycle = 0U;
    uint64_t hartid = read_csr(mhartid);
    uint8_t cnt = 16U, int_num = 0U;

    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt In case of external,
     * bootloader not present
     */

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

    /* Reset the peripherals turn on the clocks */

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_GPIO0, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_GPIO1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_GPIO2, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

   /* mmuart0 initialization */


   /* mmuart1 initialization */

    MSS_UART_init( &g_mss_uart1_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                       g_message2 );

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

    for (cnt = 16u; cnt< 20u; cnt++)
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
        g_rx_size = MSS_UART_get_rx(&g_mss_uart1_lo, g_rx_buff,
                                            sizeof(g_rx_buff));

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

                MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                   g_rx_buff );
            }

            g_rx_size = 0;
        }
    }
}

/* GPIO interrupt handlers */

uint8_t  gpio1_bit16_or_gpio2_bit30_plic_30_IRQHandler(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
           "gpio2_pin 30 Interrupt\r\n");
    MSS_GPIO_set_outputs(GPIO2_LO, 0u);
    return EXT_IRQ_KEEP_ENABLED;
}

uint8_t gpio1_bit17_or_gpio2_bit31_plic_31_IRQHandler(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
           "gpio2_pin 31 Interrupt\r\n" );
    MSS_GPIO_set_outputs(GPIO2_LO, 0u);
    return EXT_IRQ_KEEP_ENABLED;
}

uint8_t fabric_f2h_0_plic_IRQHandler(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
               "f2h_0 interrupt\r\n" );
    MSS_GPIO_set_outputs(GPIO2_LO, 0u);
    return EXT_IRQ_KEEP_ENABLED;
}

void SysTick_Handler_h1_IRQHandler(void)
{
    uint32_t hart_id = read_csr(mhartid);
    static volatile uint8_t value = 0u;

    if (1u == hart_id)
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
