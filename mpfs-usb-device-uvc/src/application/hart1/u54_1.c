/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/common.h"
#include "vc_app.h"

/*******************************************************************************
 * Constant definitions
 */
#define USB_DMA_PLIC_IRQ_PRIORITY           3
#define USB_MC_PLIC_IRQ_PRIORITY            3

/******************************************************************************
 * Instruction message. This message will be transmitted over the UART when
 * the program starts.
 *****************************************************************************/
const uint8_t g_message1[] =
        "\r\nMSS MPU configuration for USB failed\r\n";
const uint8_t g_message2[] =
        "\r\nMSS MPU configured for USB\r\n";
const uint8_t g_message4[] =
        "\r\nUse any webcam application to view a still image."
            " See the readme file for details.\r\n";

/* Main function for the hart1(U54 processor).
 * Application code running on hart1 is placed here. */
void u54_1(void)
{
    uint8_t mss_mpu_cfg_status = 0u;

    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /* Put this hart in WFI. */
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */
    clear_soft_interrupt();

#endif

    /* Bring the MMUART1 and USB out of Reset */
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_USB, (uint8_t) 1, PERIPHERAL_ON);

    /* All clocks ON */
    MSS_UART_init(&g_mss_uart1_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    mss_mpu_cfg_status = MSS_MPU_configure(MSS_MPU_USB, MSS_MPU_PMP_REGION1,
            0x08000000u, 0x200000,
            MPU_MODE_READ_ACCESS | MPU_MODE_WRITE_ACCESS | MPU_MODE_EXEC_ACCESS,
            MSS_MPU_AM_NAPOT, 0u);

    if(mss_mpu_cfg_status)
    {
        /* Message on uart1 */
        MSS_UART_polled_tx(&g_mss_uart1_lo, g_message1, sizeof(g_message1));
    }
    else
    {
        /* Message on uart1 */
        MSS_UART_polled_tx(&g_mss_uart1_lo, g_message2, sizeof(g_message2));
    }
    PLIC_init();

    PLIC_SetPriority(USB_DMA_PLIC, USB_DMA_PLIC_IRQ_PRIORITY);
    PLIC_SetPriority(USB_MC_PLIC, USB_MC_PLIC_IRQ_PRIORITY);

    PLIC_EnableIRQ(USB_DMA_PLIC);
    PLIC_EnableIRQ(USB_MC_PLIC);

    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (const uint8_t*) "\n\rInitialized USB driver\n\r");
    /* Message on uart1 */
    MSS_UART_polled_tx(&g_mss_uart1_lo, g_message4, sizeof(g_message4));

    __enable_irq();
    APP_Init();

    while (1U)
    {
        APP_Tasks();
    }
}

/* hart1 Software interrupt handler */

void Software_h1_IRQHandler(void)
{

}
