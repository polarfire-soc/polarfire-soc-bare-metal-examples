/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
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
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/fpga_ip/CorePWM/core_pwm.h"
#include "inc/uart_mapping.h"

extern struct mss_uart_instance* p_uartmap_u54_1;
/******************************************************************************
 *  Greeting messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
     "\r\n\r\n\t  ******* PolarFire SoC CorePWM Example *******\r\n\n\
     \r\n\r\n\t  ******* Notice the LED's brightness changing *******\n\n\n\r";

/******************************************************************************
 * Delay count used to time the delay between duty cycle updates.
 *****************************************************************************/
#define DELAY_COUNT     500000

/*-------------------------------------------------------------------------*//**
 * PWM prescale and period configuration values.
 */
#define PWM_PRESCALE                    64
#define PWM_PERIOD                      100
#define COREPWM_BASE_ADDR               0x40000000

/*-------------------------------------------------------------------------*//**
 * CorePWM instance data.
 */
pwm_instance_t the_pwm;

/******************************************************************************
 * Local function prototype.
 *****************************************************************************/
static void delay(void);

/* Main function for the hart1(U54 processor).
 * Application code running on hart1 is placed here.
 */

void u54_1(void)
{
    uint32_t duty_cycle = 1;
    int direction = 1;

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

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART_U54_1,
            (uint8_t) MPFS_HAL_FIRST_HART,
            PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_FIC3,
            (uint8_t) MPFS_HAL_FIRST_HART,
            PERIPHERAL_ON);

    mss_enable_fabric();

    MSS_UART_init(p_uartmap_u54_1,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(p_uartmap_u54_1, g_greeting_msg );

    /***************************************************************************
     * Initialize the CorePWM instance by setting the
     * prescale and period values.
     * In Icicle-kit and Discovery-kit Reference design, the pclk is connected to
     *  50MHz clock.
     * System clock = 50000000 (Hz)
     *************************************************************************/
    PWM_init( &the_pwm, COREPWM_BASE_ADDR, PWM_PRESCALE, PWM_PERIOD ) ;

    /**************************************************************************
     * Set the initial duty cycle for CorePWM output 1.
     *************************************************************************/
    PWM_set_duty_cycle( &the_pwm, PWM_1, duty_cycle );

    while (1u)
    {
        /**********************************************************************
         * Update the duty cycle for CorePWM output 1.
         *********************************************************************/
        PWM_set_duty_cycle( &the_pwm, PWM_1, duty_cycle );
        /**********************************************************************
         * Wait for a short delay.
         *********************************************************************/
        delay();
        /**********************************************************************
         * Calculate the next PWM duty cycle.
         *********************************************************************/
        duty_cycle += direction;
        if ( duty_cycle >= PWM_PERIOD )
        {
            direction = -1;
        }
        else if ( duty_cycle == 0 )
        {
            direction = 1;
        }

    }
}

/******************************************************************************
 * Delay function.
 *****************************************************************************/
static void delay( void )
{
    volatile int counter = 0;

    while ( counter < DELAY_COUNT )
    {
        counter++;
    }
}
