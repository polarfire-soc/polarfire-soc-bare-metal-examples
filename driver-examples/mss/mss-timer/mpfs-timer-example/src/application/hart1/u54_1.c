/***********************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 * Application code running on u54_1
 *
 * PolarFire SoC MSS TIMER example
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_timer/mss_timer.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/uart_mapping.h"

/* Sequence of delays */
#define SEQUENCE_LENGTH     5U
#define RX_BUFF_SIZE        64U

uint8_t g_rx_buff[RX_BUFF_SIZE] = {0};
volatile uint8_t g_rx_size = 0 ;
extern struct mss_uart_instance* p_uartmap_u54_1;
extern struct mss_uart_instance* p_uartmap_u54_2;

/******************************************************************************
 * Instruction message. This message will be transmitted to the UART terminal
 * when the program starts.
 *****************************************************************************/
uint8_t g_message[] =
"\r\n****************************************************** \
\r\n\r\n      PolarFire SoC MSS TIMER example     \r\n\r\n\
******************************************************\r\n\r\n\
\r\nThis example project demonstrates "
"the use of the PolarFire SoC MSS timer \r\n ";

uint8_t g_menu[] =
"Choose one of the following options to "
"observe its corresponding Timer mode. \r\n\n\
Type 0: Show this menu\r\n\
Type 1: Configure Timer1 as 32 bit timer in periodic mode (default) \r\n\
Type 2: Configure Timer1 as 32 bit timer in one-shot mode \r\n\
Type 3: Configure Timer as 64 bit timer in periodic mode \r\n\
Type 4: Configure Timer1 to generate interrupts at "
"non uniform interval using background load API \r\n\r\n ";

uint8_t g_message2[] =
     "\r\r\nObserve the messages on UART2 terminal .\
     \r\nThe messages are displayed when the timer interrupt occurs \r\n\n";

static const uint32_t g_sequence_delays[SEQUENCE_LENGTH] =
{
    25000000,
    83000000,
    166000000,
    25000000,
    166000000
};

uint8_t timer_config = 0 ;

typedef enum _timer_menu_options_t
{
    PERIODIC = 0,
    ONE_SHOT,
    TIMER64_PERIODIC,
    BACKGROUND_LOAD
}timer_menu_options_t;

timer_menu_options_t g_current_option = 0;

void uart1_rx_handler(mss_uart_instance_t *this_uart);
static void timer_interrupt_handler(timer_menu_options_t menu_opt);

/* Main function for the hart1(u54_1 processor).
 * Application code running on hart1 is placed here.
 */
void u54_1(void)
{
    uint32_t hartid = read_csr(mhartid);
    uint32_t timer_load_value ;
    uint8_t p_buff[100];
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

    mss_config_clk_rst(
            MSS_PERIPH_MMUART_E51,
            (uint8_t) MPFS_HAL_FIRST_HART,
            PERIPHERAL_ON);
    mss_config_clk_rst(
            MSS_PERIPH_MMUART_U54_1,
            (uint8_t) MPFS_HAL_FIRST_HART,
            PERIPHERAL_ON);
    mss_config_clk_rst(
            MSS_PERIPH_MMUART_U54_2,
            (uint8_t) MPFS_HAL_FIRST_HART,
            PERIPHERAL_ON);
    mss_config_clk_rst(MSS_PERIPH_TIMER,
            (uint8_t) MPFS_HAL_FIRST_HART,
            PERIPHERAL_ON);

    MSS_UART_init( p_uartmap_u54_1,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* register rx handler for uart1*/
    MSS_UART_set_rx_handler(
            p_uartmap_u54_1,
            uart1_rx_handler,
            MSS_UART_FIFO_SINGLE_BYTE);

    MSS_UART_init( p_uartmap_u54_2,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    
    /* set priorities */
    PLIC_SetPriority(PLIC_IRQ_MMUART_U54_1, 1);
    PLIC_SetPriority(TIMER1_PLIC, 2);
    PLIC_SetPriority(TIMER2_PLIC, 2);

    /* display greeting and menu messages */
    MSS_UART_polled_tx(p_uartmap_u54_1, g_message,strlen(g_message));
    MSS_UART_polled_tx(p_uartmap_u54_1, g_menu,strlen(g_menu));
    MSS_UART_polled_tx(p_uartmap_u54_1, g_message2,strlen(g_message2));

/* UART1 Interrupt based menu which will change the Timer configurations.*/
    while (1u)
    {
        if (g_rx_size > 0u)
        {
            switch (g_rx_buff[0u])
            {

                case '0':
                    /* display the menu */
                    MSS_UART_polled_tx(p_uartmap_u54_1, g_menu,strlen(g_menu));
                    break;

                case '1':
                    /*Timer configured in periodic mode */

                    timer_load_value = 25000000;
                    MSS_TIM1_init(TIMER_LO, MSS_TIMER_PERIODIC_MODE);
                    MSS_TIM1_load_immediate(TIMER_LO, timer_load_value);
                    MSS_TIM1_start(TIMER_LO);
                    MSS_TIM1_enable_irq(TIMER_LO);

                    g_current_option = PERIODIC;
                    sprintf(p_buff,"Timer is configured in PERIODIC_MODE \
                            \r\nPlease observe the Uart2 terminal for "
                            "interrupt messages\r\n\r\n ");
                    MSS_UART_polled_tx(p_uartmap_u54_1,p_buff,strlen(p_buff));
                    MSS_UART_polled_tx_string(p_uartmap_u54_2,
                    "\r\n**************************************************\r\n"
                            );
                    break ;


                case '2':
                    /*Timer configured in one shot mode  */

                    timer_load_value = 25000000;
                    MSS_TIM1_init(TIMER_LO, MSS_TIMER_ONE_SHOT_MODE);
                    MSS_TIM1_load_immediate(TIMER_LO, timer_load_value);
                    MSS_TIM1_start(TIMER_LO);
                    MSS_TIM1_enable_irq(TIMER_LO);
                    g_current_option = ONE_SHOT;

                    sprintf(p_buff,"Timer is configured in ONE_SHOT_MODE \
                            \r\nPlease observe the Uart2 terminal for "
                            "interrupt messages\r\n\r\n");
                    MSS_UART_polled_tx(p_uartmap_u54_1, p_buff,strlen(p_buff));
                    MSS_UART_polled_tx_string(p_uartmap_u54_2,
                    "\r\n**************************************************\r\n"
                            );
                    break ;

                case '3':
                    /*Timer configured in Timer64 periodic mode */

                    timer_load_value = 25000000;
                    MSS_TIM64_init(TIMER_LO, MSS_TIMER_PERIODIC_MODE);
                    MSS_TIM64_load_immediate(TIMER_LO, 0, timer_load_value);
                    MSS_TIM64_start(TIMER_LO);
                    MSS_TIM64_enable_irq(TIMER_LO);
                    g_current_option = TIMER64_PERIODIC;

                    sprintf(p_buff,"Timer is configured in TIMER64_PERIODIC \
                            \r\nPlease observe the Uart2 terminal for "
                            "interrupt messages\r\n\r\n");
                    MSS_UART_polled_tx(p_uartmap_u54_1, p_buff,strlen(p_buff));
                    MSS_UART_polled_tx_string(p_uartmap_u54_2,
                    "\r\n**************************************************\r\n"
                            );
                    break ;

                case '4':
                    /* demonstrate background load function for timer1 */

                    MSS_TIM1_init(TIMER_LO, MSS_TIMER_PERIODIC_MODE);
                    MSS_TIM1_load_immediate(TIMER_LO, g_sequence_delays[0]);
                    MSS_TIM1_start(TIMER_LO);
                    MSS_TIM1_enable_irq(TIMER_LO);
                    g_current_option = BACKGROUND_LOAD;

                    sprintf(p_buff,"Timer is configured in BACKGROUND_LOAD \
                            \r\nPlease observe the Uart2 terminal for "
                            "interrupt messages\r\n\r\n");
                    MSS_UART_polled_tx(p_uartmap_u54_1, p_buff,strlen(p_buff));
                    MSS_UART_polled_tx_string(p_uartmap_u54_2,
                    "\r\n**************************************************\r\n"
                            );
                    break;

                default:
                    break;
            }
        
            g_rx_size = 0u;
        }
    }
}

void uart1_rx_handler(mss_uart_instance_t *this_uart)
{
    /* This will execute when interrupt from hart 1 is raised */
    g_rx_size = MSS_UART_get_rx(this_uart, g_rx_buff, sizeof(g_rx_buff));
}

/* Timer PLIC interrupt */
uint8_t timer1_plic_IRQHandler()
{

    timer_interrupt_handler(g_current_option);

    return EXT_IRQ_KEEP_ENABLED;
}

/* print the uart message based on Timer configurations */
static void timer_interrupt_handler(timer_menu_options_t menu_opt){

    static uint32_t delay_idx = 0;

    switch(menu_opt){

        case PERIODIC:

            MSS_UART_polled_tx_string(p_uartmap_u54_2,
                              "Timer Periodic interrupt tick\r\n");

            break ;

        case ONE_SHOT:

            MSS_UART_polled_tx_string(p_uartmap_u54_2,
                              "Timer_2 One shot interrupt tick\r\n");
            break ;

        case TIMER64_PERIODIC:

            MSS_UART_polled_tx_string(p_uartmap_u54_2,
                              "Timer_64 interrupt tick\r\n");

            /* clear TIM64 interrupt */
            MSS_TIM64_clear_irq(TIMER_LO);
            break;

        case BACKGROUND_LOAD:

            ++delay_idx;
            if (delay_idx >= SEQUENCE_LENGTH)
            {
                delay_idx = 0;
            }

            MSS_TIM1_load_background(TIMER_LO, g_sequence_delays[delay_idx]);

            MSS_UART_polled_tx_string(p_uartmap_u54_2,
                                      "Timer Background load tick\r\n");
            break;
    }

    /* Clear TIM1 interrupt */
    MSS_TIM1_clear_irq(TIMER_LO);
}
