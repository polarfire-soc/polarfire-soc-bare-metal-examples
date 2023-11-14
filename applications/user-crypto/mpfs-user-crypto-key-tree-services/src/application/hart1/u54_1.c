/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on U54_1. Application demonstrating the use
 * of key generation services. Please refer to README.md file for more details.
 *
 */
#include <stdio.h>
#include <string.h>
#include "inc/helper.h"
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_sys_services/mss_sys_services.h"
#include "inc/common.h"
#include "inc/config_athena.h"

#include "cal/calpolicy.h"
#include "cal/pk.h"
#include "cal/pkx.h"
#include "cal/pkxlib.h"
#include "cal/calini.h"
#include "cal/utils.h"
#include "cal/hash.h"
#include "cal/drbgf5200.h"
#include "cal/drbg.h"
#include "cal/nrbg.h"
#include "cal/sym.h"
#include "cal/shaf5200.h"
#include "cal/calenum.h"

/* change output endianness in display_output function
 */
#define REVERSE_FALSE    0
#define REVERSE_TRUE     1
#define DATA_LENGTH_32_BYTES  32

/******************************************************************************
 * MSS MMUART instance data.
 *****************************************************************************/
mss_uart_instance_t *g_uart = &g_mss_uart1_lo;

/******************************************************************************
 * User Crypto base address. This will be used in config_user.h in CAL.
 *****************************************************************************/
uint32_t g_user_crypto_base_addr = 0x22000000UL;

/*==============================================================================
  Messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\
******************************************************************************\r\n\
************* PolarFire SoC User Crypto Key Tree Example Project *************\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the User Crypto Key Tree service.\r\n";

static const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";
const uint8_t read_key_input[] =
"\r\n Enter the 256 bit key to be used: \r\n";
const uint8_t read_op_type[] =
"\r\n Enter user operation type:\r\n";
const uint8_t read_path_nonce_input[] =
"\r\n Enter the 256 bit path nonce: \r\n";
const uint8_t path_nonce_option[] =
"\r\n\r\n\
--------------------------------------------------------------------------------\r\n\
1. Press 1 to enter 256 bit path nonce value \r\n\
2. press 2 to generate 256 bit path nonce \r\n ";

/*==============================================================================
  Key Tree service
 */
void keytree(void)
{
    uint8_t status = 0x00u;
    uint32_t key[8] = {0x00};
    uint8_t op_type = 0;
    uint8_t opt = 0 ;
    uint8_t path_nonce[MSS_SYS_NONCE_SERVICE_RESP_LEN] = {0x00};
    uint32_t g_key_out[100] = {0x00};

    /* Read the 32 bytes of key input data from UART terminal. */
    get_input_data((uint8_t*)&key[0], sizeof(key), read_key_input,
                   sizeof(read_key_input));

    /* Read the 1 bytes of operation type data from UART terminal. */
    get_input_data(&op_type, sizeof(op_type), read_op_type, sizeof(read_op_type));

    /* As bPathSizeSel is set to SAT_TRUE, read the 16 bytes of path nonce
     * input data from UART terminal.
     */
    get_input_data(&opt, sizeof(opt), path_nonce_option, sizeof(path_nonce_option));

    if( opt != 1  )
    {
        status = MSS_SYS_nonce_service(path_nonce, 0);
        if (status == MSS_SYS_SUCCESS)
        {
            MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
            MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\nGenerated Path_Nonce: \r\n");
            display_output((uint8_t*)&path_nonce[0], DATA_LENGTH_32_BYTES,REVERSE_TRUE);
        }
        else
        {

            MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
            MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\n Error In path Nonce generation \r\n");
        }
    }
    else
    {
        get_input_data((uint8_t*)&path_nonce[0], sizeof(path_nonce), read_path_nonce_input,
                   sizeof(read_path_nonce_input));

        /* Change the endianness of data received from UART terminal. */
        CALWordReverse((uint32_t*)&path_nonce, (sizeof(path_nonce)/4));
        CALByteReverseWord((uint32_t*)&path_nonce, (sizeof(path_nonce)/4));
    }
    /* Change the endianness of data received from UART terminal. */
    CALWordReverse((uint32_t*)&key, (sizeof(key)/4));
    CALByteReverseWord((uint32_t*)&key, (sizeof(key)/4));

    /* Derive a secret key */
    status = CALKeyTree(1, key, op_type, (uint32_t*)&path_nonce[0], g_key_out);

    /* Display the secret key in hex format. */
    if(SATR_SUCCESS == status)
    {
        status = CALPKTrfRes(SAT_TRUE);
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Key tree successful \
                   \r\n Secret key:\r\n", sizeof("\r\n Key tree successful \
                   \r\n Secret key:\r\n"));
        display_output((uint8_t*)&g_key_out[0], DATA_LENGTH_32_BYTES,REVERSE_TRUE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Key tree service fail\r\n",
                  sizeof("\r\n Key tree service fail\r\n"));
    }
}

/*==============================================================================
  Display greeting message when application is started.
 */
static void display_greeting(void)
{
    MSS_UART_polled_tx(g_uart, g_greeting_msg,sizeof(g_greeting_msg));
}

/******************************************************************************
 * main function.
 *****************************************************************************/
void u54_1( void )
{
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
    } while (0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Here onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();
#endif

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CRYPTO, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_ATHENA, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    /* Using UART0 with interrupt routed to PLIC. */
    MSS_UART_init( g_uart,
                   MSS_UART_115200_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* Initialize the Athena core. To enable user crypto bring out off reset
	 *  and enable RINGOSC on. */
    ATHENAREG->ATHENA_CR = SYSREG_ATHENACR_RESET | SYSREG_ATHENACR_RINGOSCON;
    ATHENAREG->ATHENA_CR = SYSREG_ATHENACR_RINGOSCON;

    /* Initializes the Athena Processor. */
    CALIni();

    /* Display greeting message. */
    display_greeting();

    for(;;)
    {
        keytree();
        display_greeting();
    }
}

