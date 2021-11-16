/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 * Application code running on U54_1
 *
 * PolarFire SoC MSS PDMA Driver example project
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_pdma/mss_pdma.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

/* Enable this constant to use the PDMA in interrupt mode.
 *
 * Set enable_done_int and enable_err_int to '1' in pdma_channel_config to
 * configure the pdma channel in interrupt mode.
 */
#define MSS_PDMA_INTERRUPT_MODE 1u

/* Local buffers to store the destination and source address data. */
uint8_t g_src_arr[1024] = {0};
uint8_t g_dst_arr[1024] = {0};

/* Global variable to store the error code in PDMA channel configuration. */
uint8_t g_pdma_error_code = 0u;

/* This function will ensure that the configuration of PDMA channel is success.
 * In case of error, appropriate UART message will be generated. */
static uint8_t verify(uint8_t* g_dst_arr,uint8_t* g_src_arr,uint32_t num_loops);

/* This function will compare the data in source and destination address. Any
 * mismatch in the data is reported to the main function. */
static void check_pdma_error(uint8_t g_pdma_error_code);

#ifdef MSS_PDMA_INTERRUPT_MODE

/* This callback function will handle the application specific isr operations.
 * The pdma_isr() function is registered to the driver with a call to
 * pdma_setup_transfer() and it will get called when the pdma transfer complete
 * and/or error interrupt event occurs.
 */
static void pdma_isr(uint8_t interrupt_type);

/* Track received pdma interrupts  */
uint8_t g_pdma_interrupt = 0xFFu;

#endif

/* Debug mask to track interrupt processing. */
uint8_t g_done_int_processed = 0u;
uint8_t g_err_int_processed = 0u;

uint64_t uart_lock;
mss_uart_instance_t *g_uart= &g_mss_uart1_lo;

/* Void NULL pointer for polling mode
 * The interrupts are disabled in polling mode.
 * It is not mandatory that in polling mode user should pass the handler
 * function as a parameter to pdma_setup_transfer().
 */
void *g_null_ptr = (( uint8_t* ) 0);

/******************************************************************************
 *  Greeting messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\t  ******* PolarFire SoC PDMA Driver Example *******\n\n\n\r\
Select options below to select the DMA channel for transactions. After\r\n\
selecting the channel, the following messages on terminal will notify about \r\n\
success / failure about the DMA transactions \r\n\
\n\n\r 0--> Initiate PDMA transaction on channel 0 \n\r\
\n\n\r 1--> Initiate PDMA transaction on channel 1 \n\r\
\n\n\r 2--> Initiate PDMA transaction on channel 2 \n\r\
\n\n\r 3--> Initiate PDMA transaction on channel 3 \n\r\
";

/* Main function for the hart0(E51 processor).
 * Application code running on hart0 is placed here.
 */
void u54_1(void)
{
    uint8_t rx_buff[1];
    uint8_t rx_size = 0u;
    uint32_t errors;
    uint32_t mem_idx;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
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

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART4, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    PLIC_init();
    __enable_irq();

    PLIC_SetPriority(DMA_CH0_DONE_IRQn, 1u);
    PLIC_SetPriority(DMA_CH0_ERR_IRQn, 1u);
    PLIC_SetPriority(DMA_CH1_DONE_IRQn, 1u);
    PLIC_SetPriority(DMA_CH1_ERR_IRQn, 1u);
    PLIC_SetPriority(DMA_CH2_DONE_IRQn, 1u);
    PLIC_SetPriority(DMA_CH2_ERR_IRQn, 1u);
    PLIC_SetPriority(DMA_CH3_DONE_IRQn, 1u);
    PLIC_SetPriority(DMA_CH3_ERR_IRQn, 1u);

    /* Enable PDMA Interrupts. */
    PLIC_EnableIRQ(DMA_CH0_DONE_IRQn);
    PLIC_EnableIRQ(DMA_CH0_ERR_IRQn);
    PLIC_EnableIRQ(DMA_CH1_DONE_IRQn);
    PLIC_EnableIRQ(DMA_CH1_ERR_IRQn);
    PLIC_EnableIRQ(DMA_CH2_DONE_IRQn);
    PLIC_EnableIRQ(DMA_CH2_ERR_IRQn);
    PLIC_EnableIRQ(DMA_CH3_DONE_IRQn);
    PLIC_EnableIRQ(DMA_CH3_ERR_IRQn);

    MSS_UART_init( g_uart, MSS_UART_115200_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string (g_uart, g_greeting_msg);

    g_done_int_processed = 0u;
    g_err_int_processed = 0u;

    /* Example MEMORY 2 MEMORY DMA Transaction. */
    for(mem_idx = 0u; mem_idx < 1024u; mem_idx++)
    {
        g_src_arr[mem_idx] = (mem_idx & 0xFFu);
    }

    /* Setup the channel configuration structure.
     * Simple memory to memory transaction.
     */
    mss_pdma_channel_config_t pdma_config_ch;
    pdma_config_ch.src_addr  = (uint64_t) &g_src_arr;
    pdma_config_ch.dest_addr = (uint64_t)&g_dst_arr;
    pdma_config_ch.num_bytes = 1024u;
    pdma_config_ch.enable_done_int = 1u;
    pdma_config_ch.enable_err_int = 1u;
    pdma_config_ch.force_order = 0u;
    pdma_config_ch.repeat = 0u;

    while(1)
    {
        rx_size = MSS_UART_get_rx(g_uart, rx_buff, sizeof(rx_buff));
        if (rx_size > 0u)
        {
            MSS_UART_polled_tx_string (g_uart, "\n\r");
            memset(g_dst_arr, 0x00, sizeof(g_dst_arr));

            /* Every Loop set correct Source and Destination */
            pdma_config_ch.src_addr  = (uint64_t) &g_src_arr;
            pdma_config_ch.dest_addr = (uint64_t)&g_dst_arr;

            if (rx_buff[0] == '0')
            {
#ifdef MSS_PDMA_INTERRUPT_MODE
                g_pdma_error_code = MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_0,
                                                          &pdma_config_ch,
                                                          pdma_isr);
#else
                MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_0,
                                        &pdma_config_ch,
                                        g_null_ptr);
#endif
                if (g_pdma_error_code != 0u)
                {
                    check_pdma_error(g_pdma_error_code);
                }

                g_pdma_error_code = MSS_PDMA_start_transfer(MSS_PDMA_CHANNEL_0);
                if (g_pdma_error_code != 0u)
                {
                    check_pdma_error(g_pdma_error_code);
                }

                MSS_UART_polled_tx_string (g_uart, "DMA CH '0' ");
            }
            else if (rx_buff[0] == '1')
            {
#ifdef MSS_PDMA_INTERRUPT_MODE
                MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_1,
                                        &pdma_config_ch,
                                        pdma_isr);
#else
                MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_1,
                                        &pdma_config_ch,
                                        g_null_ptr);
#endif
                MSS_PDMA_start_transfer(MSS_PDMA_CHANNEL_1);

                MSS_UART_polled_tx_string (g_uart, "DMA CH '1' ");
            }
            else if (rx_buff[0] == '2')
            {
#ifdef MSS_PDMA_INTERRUPT_MODE
                MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_2,
                                        &pdma_config_ch,
                                        pdma_isr);
#else
                MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_2,
                                        &pdma_config_ch,
                                        g_null_ptr);
#endif

                MSS_PDMA_start_transfer(MSS_PDMA_CHANNEL_2);

                MSS_UART_polled_tx_string (g_uart, "DMA CH '2' ");
            }
            else if (rx_buff[0] == '3')
            {
#ifdef MSS_PDMA_INTERRUPT_MODE
                MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_3,
                                        &pdma_config_ch,
                                        pdma_isr);
#else
                MSS_PDMA_setup_transfer(MSS_PDMA_CHANNEL_3,
                                        &pdma_config_ch,
                                        g_null_ptr);
#endif
                MSS_PDMA_start_transfer(MSS_PDMA_CHANNEL_3);

                MSS_UART_polled_tx_string (g_uart, "DMA CH '3' ");
            }
            else
            {
                MSS_UART_polled_tx_string (g_uart, "Please Select Correct Channel  \n\r");
            }
        }

#ifdef MSS_PDMA_INTERRUPT_MODE

        if (g_pdma_interrupt != 0xFF)
        {
            errors = verify((uint8_t*)&g_dst_arr[0], (uint8_t*)&g_src_arr[0],
                                                      sizeof(g_dst_arr));
            if(errors == 0)
            {
                MSS_UART_polled_tx_string (g_uart,
                        "\n\rMemory to Memory DMA Transaction successful \n\r");
            }
            else
            {   MSS_UART_polled_tx_string (g_uart,
                    "\n\rMemory to Memory DMA Transaction failed \n\r");
            }

            if (g_pdma_interrupt == 0x0u)
            {
                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH0 Done Int\n\r");
            }

            else if (g_pdma_interrupt == 0x1u)
            {
                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH1 Done Int\n\r");
            }

            else if (g_pdma_interrupt == 0x2u)
            {

                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH2 Done Int\n\r");
            }
            else if (g_pdma_interrupt == 0x3u)
            {
                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH3 Done Int\n\r");
            }
            else if (g_pdma_interrupt == 0x10u)
            {
                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH0 Err Int\n\r");
            }
            else if (g_pdma_interrupt == 0x11u)
            {
                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH1 Err Int\n\r");
            }
            else if (g_pdma_interrupt == 0x12u)
            {
                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH2 Err Int\n\r");
            }
            else if (g_pdma_interrupt == 0x13u)
            {
                g_pdma_interrupt = 0xFFu;
                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH3 Err Int\n\r");
            }
        }
#else

        /* Poll to get to see did we get the done interrupt flag, and report
         * status.
         * g_done_int_processed: Bit0 for Channel 0
         * g_done_int_processed: Bit1 for Channel 1
         * g_done_int_processed: Bit2 for Channel 2
         * g_done_int_processed: Bit3 for Channel 3
         */
        g_done_int_processed |= MSS_PDMA_clear_transfer_complete_status(MSS_PDMA_CHANNEL_0)
                                                              << MSS_PDMA_CHANNEL_0;
        g_done_int_processed |= MSS_PDMA_clear_transfer_complete_status(MSS_PDMA_CHANNEL_1)
                                                              << MSS_PDMA_CHANNEL_1;
        g_done_int_processed |= MSS_PDMA_clear_transfer_complete_status(MSS_PDMA_CHANNEL_2)
                                                              << MSS_PDMA_CHANNEL_2;
        g_done_int_processed |= MSS_PDMA_clear_transfer_complete_status(MSS_PDMA_CHANNEL_3)
                                                              << MSS_PDMA_CHANNEL_3;

        /* UART Reporting and debug messages. */
         if (g_done_int_processed)
         {
             /* UART Reporting and debug messages. */
             errors = verify((uint8_t*)&g_dst_arr[0],(uint8_t*)&g_src_arr[0],
                              sizeof(g_dst_arr));

             if (errors == 0u)
                 MSS_UART_polled_tx_string (g_uart,
                         "\n\rMemory to Memory DMA Transaction successful \n\r");
            else
                MSS_UART_polled_tx_string (g_uart,
                        "\n\rMemory to Memory DMA Transaction failed \n\r");

             if (g_done_int_processed & (0x01 << MSS_PDMA_CHANNEL_0))
             {
                 g_done_int_processed &= (~(g_done_int_processed &
                         (0x01 << MSS_PDMA_CHANNEL_0)));

                 MSS_UART_polled_tx_string (g_uart,
                         "PDMA CH0 Done Int\n\r");
             }
             else if (g_done_int_processed & (0x01 << MSS_PDMA_CHANNEL_1))
             {
                 g_done_int_processed &= (~(g_done_int_processed &
                         (0x01 << MSS_PDMA_CHANNEL_1)));

                 MSS_UART_polled_tx_string (g_uart,
                         "PDMA CH1 Done Int\n\r");
             }
             else if (g_done_int_processed & (0x01 << MSS_PDMA_CHANNEL_2))
             {
                g_done_int_processed &= (~(g_done_int_processed &
                        (0x01 << MSS_PDMA_CHANNEL_2)));

                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH2 Done Int\n\r");
             }
             else if (g_done_int_processed & (0x01 << MSS_PDMA_CHANNEL_3))
             {
                g_done_int_processed &= (~(g_done_int_processed &
                        (0x01 << MSS_PDMA_CHANNEL_3)));

                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH3 Done Int\n\r");
             }
         }

         /* Poll to get to see did we get the error interrupt flag, and report
          * status.
          * g_err_int_processed: Bit0 for Channel 0
          * g_err_int_processed: Bit1 for Channel 1
          * g_err_int_processed: Bit2 for Channel 2
          * g_err_int_processed: Bit3 for Channel 3
          */
         g_err_int_processed |= MSS_PDMA_clear_transfer_error_status(MSS_PDMA_CHANNEL_0)
                                                              << MSS_PDMA_CHANNEL_0;

         g_err_int_processed |= MSS_PDMA_clear_transfer_error_status(MSS_PDMA_CHANNEL_1)
                                                              << MSS_PDMA_CHANNEL_1;

         g_err_int_processed |= MSS_PDMA_clear_transfer_error_status(MSS_PDMA_CHANNEL_2)
                                                              << MSS_PDMA_CHANNEL_2;

         g_err_int_processed |= MSS_PDMA_clear_transfer_error_status(MSS_PDMA_CHANNEL_3)
                                                              << MSS_PDMA_CHANNEL_3;

         /* UART Reporting and debug messages. */
         if (g_err_int_processed)
         {
             if (g_err_int_processed & (0x01 << MSS_PDMA_CHANNEL_0))
             {
                 g_err_int_processed &= (~(g_err_int_processed &
                         (0x01 << MSS_PDMA_CHANNEL_0)));

                 MSS_UART_polled_tx_string (g_uart,
                         "PDMA CH0 Err Int\n\r");
             }
             else if (g_err_int_processed & (0x01 << MSS_PDMA_CHANNEL_1))
             {
                 g_err_int_processed &= (~(g_err_int_processed &
                         (0x01 << MSS_PDMA_CHANNEL_1)));

                 MSS_UART_polled_tx_string (g_uart,
                         "PDMA CH1 Err Int\n\r");
             }
             else if (g_err_int_processed & (0x01 << MSS_PDMA_CHANNEL_2))
             {
                 g_err_int_processed &= (~(g_err_int_processed &
                         (0x01 << MSS_PDMA_CHANNEL_2)));

                 MSS_UART_polled_tx_string (g_uart,
                         "PDMA CH2 Err Int\n\r");
             }
             else if (g_err_int_processed & (0x01 << MSS_PDMA_CHANNEL_3))
             {
                 g_err_int_processed &= (~(g_err_int_processed &
                         (0x01 << MSS_PDMA_CHANNEL_3)));

                MSS_UART_polled_tx_string (g_uart,
                        "PDMA CH3 Err Int\n\r");
             }
         }
#endif
    }
}

/* Local function to verify the PDMA transaction. */
static uint8_t
verify
(
    uint8_t* g_dst_arr,
    uint8_t* g_src_arr,
    uint32_t num_loops
)
{
    uint8_t error = 0u;
    uint32_t idx = 0u;
    while (num_loops != 0u)
    {
        if (g_dst_arr[idx] != g_src_arr[idx])
        {
            error = 1u;
            break;
        }
        idx++;
        num_loops--;
    }

    return error;
}

/* Local function to check the errors in PDMA channel configuration */
static void
check_pdma_error
(
    uint8_t g_pdma_error_code
)
{
    if (g_pdma_error_code == 1u)
    {
        MSS_UART_polled_tx_string (g_uart, "Invalid source address\n\r");
    }
    else if (g_pdma_error_code == 2u)
    {
        MSS_UART_polled_tx_string (g_uart, "Invalid destination address\n\r");
    }
    else if (g_pdma_error_code == 3u)
    {
        MSS_UART_polled_tx_string (g_uart, "Transaction in progress\n\r");
    }
    else if (g_pdma_error_code == 4u)
    {
        MSS_UART_polled_tx_string (g_uart, "Invalid Channel ID\n\r");
    }
    else if (g_pdma_error_code == 5u)
    {
        MSS_UART_polled_tx_string (g_uart, "Invalid write size\n\r");
    }
    else if (g_pdma_error_code == 6u)
    {
        MSS_UART_polled_tx_string (g_uart, "Invalid Read size\n\r");
    }
    else if (g_pdma_error_code == 7u)
    {
        MSS_UART_polled_tx_string (g_uart, "Last ID\n\r");
    }
}

#ifdef MSS_PDMA_INTERRUPT_MODE
static void
pdma_isr( uint8_t interrupt_type)
{

    g_pdma_interrupt = interrupt_type;

    /* Clear DONE interrupt flag */
    if ((interrupt_type >= PDMA_CH0_DONE_INT) && (interrupt_type <= PDMA_CH3_DONE_INT))
    {
        MSS_PDMA_clear_transfer_complete_status(interrupt_type);
    }

    /* Clear ERROR interrupt flag */
    else if((interrupt_type >= PDMA_CH0_ERROR_INT) && (interrupt_type <= PDMA_CH3_ERROR_INT))
    {
        interrupt_type &= (interrupt_type << 4u);

        MSS_PDMA_clear_transfer_error_status(interrupt_type);
    }
}
#endif
