/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_2
 *
 * Example project demonstrating the use of polled and external interrupt driven
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

const uint8_t g_message4[] =
        "\r\n\r\n\r\n **** PolarFire SoC MSS MMUART example ****\r\n\r\n\r\n";

const uint8_t g_message5[] =
        "This program is run from u54_2\r\n\
        \r\n\
Type 0  Show hart 2 debug message\r\n\
Type 1  Show this menu\r\n\
Type 2  Send message using polled method\r\n\
Type 3  send message using interrupt method\r\n\
";

const uint8_t polled_message2[] =
        "This message has been transmitted using polled method. \r\n";

const uint8_t intr_message2[] =
        " This message has been transmitted using external interrupt method. \r\n";


#define RX_BUFF_SIZE               16U
#define EXTERNAL_IRQ_PRIORITY      4U
#define PLIC_PRIORITY_THRESHOLD    0U

uint8_t g_rx_buff2[RX_BUFF_SIZE] = { 0 };
volatile uint32_t count_sw_ints_h2 = 0U;
volatile uint8_t g_rx_size2 = 0U;
static volatile uint32_t irq_cnt = 0;
uint8_t info_string2[100];

/* This is the handler function for the UART2 RX interrupt.
 * In this example project UART2 external interrupt is enabled on hart2.
 */
void uart2_rx_handler(mss_uart_instance_t *this_uart) 
{
    uint32_t hart_id = read_csr(mhartid);
    int8_t info_string2[50];

    irq_cnt++;
    sprintf(info_string2, "UART2 Interrupt count = 0x%x \r\n\r\n", irq_cnt);

    /* This will execute when interrupt from hart 2 is raised */
    g_rx_size2 = MSS_UART_get_rx(this_uart, g_rx_buff2, sizeof(g_rx_buff2));

    MSS_UART_polled_tx(&g_mss_uart2_lo, info_string2,
            strlen(info_string2));
}

/* Main function for the hart2(U54 processor).
 * Application code running on hart2 is placed here.
 * MMUART2 external interrupt is enabled on hart2.
 * In the respective U54 harts, local interrupts of the corresponding MMUART
 * are enabled. e.g. in U54_2.c local interrupt of MMUART2 is enabled. */

void u54_2(void) 
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
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 2, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t) 2, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t) 2, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART4, (uint8_t) 2, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t) 2, PERIPHERAL_ON);

    /* All clocks ON */

    MSS_UART_init(&g_mss_uart2_lo,
    MSS_UART_115200_BAUD,
    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_set_rx_handler(&g_mss_uart2_lo, uart2_rx_handler,
            MSS_UART_FIFO_SINGLE_BYTE);

    PLIC_init();
    MSS_UART_enable_irq(&g_mss_uart2_lo, (MSS_UART_RBF_IRQ | MSS_UART_TBE_IRQ));
    /* It is required to set a priority for a PLIC interrupt, even if no other
     * interrupt is used */
    PLIC_SetPriority(MMUART2_PLIC, EXTERNAL_IRQ_PRIORITY);
    PLIC_SetPriority_Threshold(PLIC_PRIORITY_THRESHOLD);

    /* Demonstrating polled MMUART transmission */
    MSS_UART_polled_tx(&g_mss_uart2_lo,g_message4,
            sizeof(g_message4));

    /* Demonstrating interrupt method of transmission */
    MSS_UART_irq_tx(&g_mss_uart2_lo, g_message5,
            sizeof(g_message5));

    /* Makes sure that the previous interrupt based transmission is completed
     * Alternatively, you could register TX complete handler using
     * MSS_UART_set_tx_handler() */
    while (0u == MSS_UART_tx_complete(&g_mss_uart2_lo)) 
    {
        ;
    }

    mcycle_start = readmcycle();
    while (1u) 
    {
        if (g_rx_size2 > 0u) 
        {
            switch (g_rx_buff2[0u]) 
            {
            case '0':
                mcycle_end = readmcycle();
                delta_mcycle = mcycle_end - mcycle_start;
                sprintf(info_string2, "hart %ld, %ld delta_mcycle \r\n", hartid,
                        delta_mcycle);
                MSS_UART_polled_tx(&g_mss_uart2_lo, info_string2,
                        strlen(info_string2));
                break;
            case '1':
                /* show menu */
                MSS_UART_irq_tx(&g_mss_uart2_lo, g_message5,
                        sizeof(g_message5));
                break;
            case '2':

                /* polled method of transmission */
                MSS_UART_polled_tx(&g_mss_uart2_lo, polled_message2,
                        sizeof(polled_message2));

                break;
            case '3':

                /* interrupt method of transmission */
                MSS_UART_irq_tx(&g_mss_uart2_lo, intr_message2,
                        sizeof(intr_message2));
                break;

            default:
                MSS_UART_polled_tx(&g_mss_uart2_lo, g_rx_buff2,
                        g_rx_size2);
                break;
            }

            g_rx_size2 = 0u;
        }
    }
}

/* hart2 software interrupt handler */
void Software_h2_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h2++;
}
