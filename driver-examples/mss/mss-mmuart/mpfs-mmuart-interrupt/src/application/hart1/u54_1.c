/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * Example project demonstrating the use of polled and interrupt driven
 * transmission and reception over MMUART. Please refer README.md in the root
 * folder of this example project
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

const uint8_t polled_message[] =
        "This message has been transmitted using polled method. \r\n";

const uint8_t intr_message[] =
        " This message has been transmitted using interrupt method. \r\n";


#define RX_BUFF_SIZE    64U
uint8_t g_rx_buff[RX_BUFF_SIZE] = { 0 };
volatile uint32_t count_sw_ints_h1 = 0U;
volatile uint8_t g_rx_size = 0U;
static volatile uint32_t irq_cnt = 0;
uint8_t info_string[100];

/* This is the handler function for the UART RX interrupt.
 * In this example project UART0 local interrupt is enabled on hart0.
 */
void uart1_rx_handler(mss_uart_instance_t *this_uart) {
    uint32_t hart_id = read_csr(mhartid);
    int8_t info_string[50];

    irq_cnt++;
    sprintf(info_string, "UART1 Interrupt count = 0x%x \r\n\r\n", irq_cnt);

    /* This will execute when interrupt from hart 1 is raised */
    g_rx_size = MSS_UART_get_rx(this_uart, g_rx_buff, sizeof(g_rx_buff));

    MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,
            strlen(info_string));
}

/* Main function for the hart1(U54 processor).
 * Application code running on hart1 is placed here.
 * MMUART1 local interrupt is enabled on hart1.
 * In the respective U54 harts, local interrupts of the corresponding MMUART
 * are enabled. e.g. in U54_1.c local interrupt of MMUART1 is enabled. */

void u54_1(void) {
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
    while (0u == MSS_UART_tx_complete(&g_mss_uart1_lo)) {
        ;
    }

    mcycle_start = readmcycle();
    while (1u) {
        if (g_rx_size > 0u) {
            switch (g_rx_buff[0u]) {

            case '0':
                mcycle_end = readmcycle();
                delta_mcycle = mcycle_end - mcycle_start;
                sprintf(info_string, "hart %ld, %ld delta_mcycle \r\n", hartid,
                        delta_mcycle);
                MSS_UART_polled_tx(&g_mss_uart1_lo, info_string,
                        strlen(info_string));
                break;
            case '1':
                /* show menu */
                MSS_UART_irq_tx(&g_mss_uart1_lo, g_message2,
                        sizeof(g_message2));
                break;
            case '2':

                /* polled method of transmission */
                MSS_UART_polled_tx(&g_mss_uart1_lo, polled_message,
                        sizeof(polled_message));

                break;
            case '3':

                /* interrupt method of transmission */
                MSS_UART_irq_tx(&g_mss_uart1_lo, intr_message,
                        sizeof(intr_message));
                break;

            default:
                MSS_UART_polled_tx(&g_mss_uart1_lo, g_rx_buff,
                        g_rx_size);
                break;
            }

            g_rx_size = 0u;
        }
    }
}

/* hart1 Software interrupt handler */

void Software_h1_IRQHandler(void) {
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}
