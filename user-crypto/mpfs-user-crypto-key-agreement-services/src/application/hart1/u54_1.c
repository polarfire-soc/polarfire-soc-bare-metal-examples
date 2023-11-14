/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on U54_1. Application demonstrating the use
 * of key agreement services. Please refer to README.md file for more details.
 * 
 */
#include <stdio.h>
#include <string.h>
#include "inc/helper.h"
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
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

/*private utility function */
static SATR CAL_EXECUTE(SATR result) {

                return (result == SATR_SUCCESS) ? CALPKTrfRes(SAT_TRUE) : result;
}

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
****** PolarFire SoC User Crypto Key Agreement Service Example Project *******\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the PolarFire User Crypto \r\n\
 Athena Key Agreement Service for generating shared secret key.\r\n";

static const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";

/*==============================================================================
  Global Variables.
 */
static uint8_t Pmod_2048[256] ={ 0x00 };
static uint8_t G_2048[256] = { 0x00 };
static uint32_t PrePmod_2048[65];
static uint32_t PublicKey_A[100];
static uint32_t SharedSecret[65];
static uint8_t PublicKey_B[400] = { 0x00 };
static uint8_t E_2048[28] = { 0x00 };

/*==============================================================================
  Display greeting message when application is started.
 */
static void display_greeting(void)
{
    MSS_UART_polled_tx(g_uart, g_greeting_msg,sizeof(g_greeting_msg));
}

/*==============================================================================
  Diffie-Hellman key agreement algorithm.
 */
static void dh_key_agreement(void)
{
    const uint8_t base_value_msg[] =
      "\r\n Enter base value (G) 256 bytes: \r\n";
    const uint8_t read_mod_param[] =
      "\r\n Enter modulus value: \r\n";
    const uint8_t read_private_key_usrA[] =
      "\r\n Enter private key of user A: \r\n";
    const uint8_t read_userB_public_key[] =
      "\r\n Enter user B public key: \r\n";

    /* Read base value value. */
    get_input_data((uint8_t*)&G_2048, sizeof(G_2048), base_value_msg,
                   sizeof(base_value_msg));

    /* Read private key of user A. i.e.puiExp parameter. */
    get_input_data((uint8_t*)&E_2048, sizeof(E_2048), read_private_key_usrA,
                   sizeof(read_private_key_usrA));

    /* Read modulus parameter. */
    get_input_data((uint8_t*)&Pmod_2048, sizeof(Pmod_2048), read_mod_param,
                   sizeof(read_mod_param));

    /* Read shared secret key from user B. */
    get_input_data((uint8_t*)&PublicKey_B, sizeof(PublicKey_B),
                   read_userB_public_key, sizeof(read_userB_public_key));

    /* Change the endianness of received parameter from UART terminal.*/
    CALWordReverse((uint32_t*)&G_2048, 64);
    CALByteReverseWord((uint32_t*)&G_2048, 64);
    CALWordReverse((uint32_t*)&E_2048, 7);
    CALByteReverseWord((uint32_t*)&E_2048, 7);
    CALWordReverse((uint32_t*)&Pmod_2048, 64);
    CALByteReverseWord((uint32_t*)&Pmod_2048, 64);
    CALWordReverse((uint32_t*)&PublicKey_B, 64);
    CALByteReverseWord((uint32_t*)&PublicKey_B, 64);

    /* Initiate generation of pre-compute value. */
    CAL_EXECUTE(CALPreCompute((uint32_t*)&Pmod_2048, PrePmod_2048, 64u));

    /* Initiate modular exponentiation operation -  Generate Public key for
     * PARTY-A. */
    CAL_EXECUTE(CALExpo((uint32_t*)&G_2048, (uint32_t*)&E_2048,
                        (uint32_t*)&Pmod_2048, PrePmod_2048, 7u, 64u,
                        PublicKey_A));

    /* Generate shared secret key using PARTY-B public key. */
    CAL_EXECUTE(CALExpo((uint32_t*)&PublicKey_B, (uint32_t*)&E_2048, SAT_NULL,
                        SAT_NULL, 7u, 64u, SharedSecret));

    /* Display Shared secret key on UART terminal.*/
    MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
    MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Secret key value: \r\n",
              sizeof("\r\n Secret key value:\r\n"));
    CALWordReverse((uint32_t*)&SharedSecret, 64);
    CALByteReverseWord((uint32_t*)&SharedSecret, 64);
    display_output((uint8_t*)&SharedSecret[0], 256,REVERSE_FALSE);

    MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
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

    /* Using UART with interrupt routed to PLIC. */
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
        /* Calculate shared secret using Diffie-Hellman key agreement algorithm.*/
        dh_key_agreement();
        display_greeting();
    }
}

