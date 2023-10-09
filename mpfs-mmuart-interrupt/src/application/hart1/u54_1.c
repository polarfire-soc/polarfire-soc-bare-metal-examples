/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on u54_1
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/common.h"

/******************************************************************************
 * Instruction message. This message will be transmitted over the UART when
 * the program starts.
 *****************************************************************************/

const uint8_t g_message1[] =
        "\r\n\r\n\r\n **** PolarFire SoC MSS MMUART example ****\r\n\r\n\r\n";

const uint8_t g_message2[] =
        "This program is run from u54_1\r\n\
        \r\n\
Type 0  Show hart 1 debug message\r\n\
Type 1  Show this menu\r\n\
Type 2  Send message using polled method\r\n\
Type 3  send message using interrupt method\r\n\
";

const uint8_t polled_message1[] =
        "This message has been transmitted using polled method. \r\n";

const uint8_t intr_message1[] =
        " This message has been transmitted using local interrupt method. \r\n";


#define RX_BUFF_SIZE    16U
uint8_t g_rx_buff1[RX_BUFF_SIZE] = { 0 };
volatile uint32_t count_sw_ints_h1 = 0U;
volatile uint8_t g_rx_size1 = 0U;
static volatile uint32_t irq_cnt = 0;
uint8_t info_string1[100];

/* This is the handler function for the UART RX interrupt.
 * In this example project UART1 local interrupt is enabled on hart1.
 */
void uart1_rx_handler(mss_uart_instance_t *this_uart) 
{
    uint32_t hart_id = read_csr(mhartid);
    int8_t info_string1[50];

    irq_cnt++;
    sprintf(info_string1, "UART1 Interrupt count = 0x%x \r\n\r\n", irq_cnt);

    /* This will execute when interrupt from hart 1 is raised */
    g_rx_size1 = MSS_UART_get_rx(this_uart, g_rx_buff1, sizeof(g_rx_buff1));

    MSS_UART_polled_tx(&g_mss_uart1_lo, info_string1,
            strlen(info_string1));
}

/* Main function for the hart1(U54 processor).
 * Application code running on hart1 is placed here.
 * MMUART1 local interrupt is enabled on hart1.
 * In the respective U54 harts, local interrupts of the corresponding MMUART
 * are enabled. e.g. in U54_1.c local interrupt of MMUART1 is enabled. */

void u54_1(void) 
{
    uint64_t mcycle_start = 0U;
    uint64_t mcycle_end = 0U;
    uint64_t delta_mcycle = 0U;
    uint64_t hartid = read_csr(mhartid);


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

    __enable_irq();

    /* Bring all the MMUARTs out of Reset */
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART4, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t) 1, PERIPHERAL_ON);

    /* All clocks ON */

    MSS_UART_init(&g_mss_uart1_lo,
    MSS_UART_115200_BAUD,
    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_set_rx_handler(&g_mss_uart1_lo, uart1_rx_handler,
            MSS_UART_FIFO_SINGLE_BYTE);

    MSS_UART_enable_local_irq(&g_mss_uart1_lo);

    /* Demonstrating polled MMUART transmission */
    MSS_UART_polled_tx(&g_mss_uart1_lo,g_message1,
            sizeof(g_message1));

    /* Demonstrating interrupt method of transmission */
    MSS_UART_irq_tx(&g_mss_uart1_lo, g_message2,
            sizeof(g_message2));

    /* Makes sure that the previous interrupt based transmission is completed
     * Alternatively, you could register TX complete handler using
     * MSS_UART_set_tx_handler() */
    while (0u == MSS_UART_tx_complete(&g_mss_uart1_lo)) 
    {
        ;
    }

    mcycle_start = readmcycle();
    while (1u) 
    {
        if (g_rx_size1 > 0u) 
        {
            switch (g_rx_buff1[0u]) 
            {
            case '0':
                mcycle_end = readmcycle();
                delta_mcycle = mcycle_end - mcycle_start;
                sprintf(info_string1, "hart %ld, %ld delta_mcycle \r\n", hartid,
                        delta_mcycle);
                MSS_UART_polled_tx(&g_mss_uart1_lo, info_string1,
                        strlen(info_string1));
                break;
            case '1':
                /* show menu */
                MSS_UART_irq_tx(&g_mss_uart1_lo, g_message2,
                        sizeof(g_message2));
                break;
            case '2':

                /* polled method of transmission */
                MSS_UART_polled_tx(&g_mss_uart1_lo, polled_message1,
                        sizeof(polled_message1));

                break;
            case '3':

                /* interrupt method of transmission */
                MSS_UART_irq_tx(&g_mss_uart1_lo, intr_message1,
                        sizeof(intr_message1));
                break;

            default:
                MSS_UART_polled_tx(&g_mss_uart1_lo, g_rx_buff1,
                        g_rx_size1);
                break;
            }

            g_rx_size1 = 0u;
        }
    }
}

/* hart1 Software interrupt handler */

void Software_h1_IRQHandler(void) 
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}
