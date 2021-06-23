/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on E51
 *
 * Example project demonstrating using I2C interface to read the voltage and
 * current data from the PAC1934 sensor on the Icicle kit.
 * Please refer README.txt in the root folder of this example project for details.
 *
 */

#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"

#define RX_BUFF_SIZE    64

const uint8_t g_message3[] =
"\r\n***    PolarFire SoC Icicle Kit PAC1934 sensor example    ***\r\n\n\
Please observe messages on COM port interface1 for the application messages\r\n";

/* Created for convenience. Uses Polled method of UART TX */
void uart_tx_with_mutex (mss_uart_instance_t * this_uart,
                                uint64_t mutex_addr,
                                const uint8_t * pbuff)
{
    MSS_UART_polled_tx_string(this_uart, pbuff);
}

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here. */
void e51(void)
{
    volatile uint32_t icount = 0U;
	   
    mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) 1, PERIPHERAL_ON);

    MSS_UART_init(&g_mss_uart0_lo,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* Receive interrupt is enabled now. Please see uart1_rx_handler() for
     * more details */
    MSS_UART_polled_tx(&g_mss_uart0_lo, g_message3, sizeof(g_message3));

    /* Wake-up the U54_1 from WIF. U54_1 will read sensor data over I2C and
     * display it on the terminal. Since floating point operations are involved
     * U54_1 will carry these out; Keeping SoftConsole project target settings
     * common.*/
    raise_soft_interrupt(1u);

    while(1U)
    {
        icount++;

        if(0x100000U == icount)
        {
            icount = 0U;
        }
    }
}
