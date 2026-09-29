/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1.
 *
 */

#include <stdio.h>
#include "inc/common.h"
#include "mpfs_hal/mss_hal.h"
#include "drivers/off_chip/pac1934/pac1934.h"

extern void uart_tx_with_mutex (mss_uart_instance_t * this_uart,
                                uint64_t mutex_addr,
                                const uint8_t * pbuff);

uint8_t rx_size = 0;
uint8_t g_rx_buff[1] = {0};

/* g_uart is required by pac1934.c for UART output */
mss_uart_instance_t *g_uart = NULL;

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * In non-bootloader flows (lim-debug, lim-release, eNVM-scratchpad):
 *   hart1 goes into WFI and hart0 (E51) brings it out by raising a software
 *   interrupt after hardware initialisation is complete.
 *
 * In bootloader flows (ddr-release):
 *   The HSS bootloader has already initialised all hardware and jumps hart1
 *   directly to this function. E51 is not running in the application, so the
 *   WFI/software-interrupt gate is skipped.
 */
void u54_1(void)
{
    volatile uint32_t icount = 0U;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart into WFI. */
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));
#endif /* IMAGE_LOADED_BY_BOOTLOADER == 0 */

    /* The hart is out of WFI, clear the SW interrupt. Here onwards application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();
    PLIC_init();

    (void) mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_I2C1, (uint8_t) 1, PERIPHERAL_ON);

    g_uart = &g_mss_uart1_lo;

    MSS_UART_init(&g_mss_uart1_lo,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    if (0 == pac1934_sensor_probe())
    {
        uart_tx_with_mutex (&g_mss_uart1_lo, 0u,
                            "\r\nPAC1934 sensor probe successful \r\n");
        pac1934_draw_vb();
        pac1934_draw_isense();
        uart_tx_with_mutex (&g_mss_uart1_lo, 0u,
                            "\r\nPress a key to refresh the sensor data \r\n");
    }
    else
    {
        uart_tx_with_mutex (&g_mss_uart1_lo, 0u,
                            "\r\nPAC1934 sensor probe failed \r\n");
    }

    while (1u)
    {
        rx_size = MSS_UART_get_rx(&g_mss_uart1_lo, g_rx_buff, sizeof(g_rx_buff));

        if(rx_size > 0)
        {
            switch (g_rx_buff[0u])
            {
                case 1:
                default:
                pac1934_draw_vb();
                pac1934_draw_isense();
                break;
            }
        }
    }

    /* never return */
}

/* hart1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
}
