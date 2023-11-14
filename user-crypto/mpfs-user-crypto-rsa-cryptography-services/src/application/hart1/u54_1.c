/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on U54_1. Application demonstrating the use
 * of RSA services to encrypt and decrypt the message. Please refer to README.md
 * file for more details.
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

/******************************************************************************
 * MSS MMUART instance data.
 *****************************************************************************/
mss_uart_instance_t *g_uart = &g_mss_uart1_lo;

/******************************************************************************
 * User Crypto base address. This will be used in config_user.h in CAL.
 *****************************************************************************/
uint32_t g_user_crypto_base_addr = 0x22000000UL;

static const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";
 SATR CAL_EXECUTE(SATR result) {

                return (result == SATR_SUCCESS) ? CALPKTrfRes(SAT_TRUE) : result;
}

/*==============================================================================
  Messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\
******************************************************************************\r\n\
* PolarFire SoC User Crypto RSA Encryption Decryption Service Example Project *\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the PolarFire RSA service to \r\n\
 encrypt and decrypt the message. The following User Athena service are \r\n\
 demonstrated:\r\n\
  1 - RSA encryption.\r\n\
  2 - RSA decryption.\r\n";
const uint8_t g_select_operation_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select the RSA operation to perform:\r\n\
    Press Key '1' to perform RSA encryption \r\n\
    Press Key '2' to perform RSA decryption \r\n\
------------------------------------------------------------------------------\r\n";

/* RSA encryption message */
const uint8_t encrpt_msg[] =
"\r\n Enter message (max size: 256 bytes): \r\n";
const uint8_t ecrypt_modval_msg[] =
"\r\n Enter modulus n value (max size: 256 bytes): \r\n";
const uint8_t encrypt_dval_msg[] =
"\r\n Enter parameter E value (max size: 256 bytes): \r\n";
const uint8_t precomputes_fail_msg[] =
"\r\n Modulus pre-compute fails. \r\n";
const uint8_t encrypt_success[] =
"\r\n RSA encryption successful. \r\n Generated Cipher Text: \r\n";
const uint8_t encrypt_fail[] =
"\r\n RSA encryption fail. \r\n";

/* RSA decryption message */
const uint8_t decrpt_msg[] =
"\r\n Enter cipher text (max size: 256 bytes): \r\n";
const uint8_t read_prvtkey_msg[] =
"\r\n Enter parameter D value (max size: 256 bytes): \r\n";
const uint8_t read_paramP_msg[] =
"\r\n Enter parameter P value (max size: 256 bytes): \r\n";
const uint8_t read_paramQ_msg[] =
"\r\n Enter parameter Q value (max size: 256 bytes): \r\n";
const uint8_t decrypt_modval_msg[] =
"\r\n Enter modulus n value (max size: 256 bytes): \r\n";
const uint8_t decrypt_success[] =
"\r\n RSA decryption successful. \r\n Generated Plain Text: \r\n";
const uint8_t decrypt_fail[] =
"\r\n RSA decryption fail.\r\n";

/*==============================================================================
  Global Variables
 */
/* RSA encryption */
const SATUINT32_t plaintext[96] = { 0x00 };
const SATUINT32_t n_m3072_ecrypt[96]={ 0x00 };
const SATUINT32_t e_m3072_ecrypt[96] = { 0x00 };
const SATUINT32_t encrypt_NMu[97] = {0x00};
const SATUINT32_t ciphertext[96] = {0x00};

/*  RSA decryption */
uint32_t decrypt_plaintext[96] = { 0x00 };
uint32_t decrypt_cipher[96] = { 0x00 };
uint32_t decrypt_D[96] = { 0x00 };
uint32_t decrypt_QInv[48] = { 0x00 };
uint32_t decrypt_DP[48] = { 0x00 };
uint32_t decrypt_DQ[48] = { 0x00 };
uint32_t decrypt_P[48] = { 0x00 };
uint32_t decrypt_Pm1[48] = { 0x00 };
uint32_t decrypt_Pm2[48] = { 0x00 };
uint32_t decrypt_precomP[49] = { 0x00 };
uint32_t decrypt_precomPm1[49] = { 0x00 };
uint32_t decrypt_Q[48] = { 0x00 };
uint32_t decrypt_Qm1[48] = { 0x00 };
uint32_t decrypt_precomQ[49] = { 0x00 };
uint32_t decrypt_precomQm1[49] = { 0x00 };

/*==============================================================================
  Local function.
 */
static void display_greeting(void);
static void display_operation_choices(void);
static void display_option(void);

/*==============================================================================
  Message encryption using RSA.
 */
static void rsa_encrypt(void)
{
    SATR result;

    /* Read message value to be encrypted using RSA encryption. */
    get_input_data((uint8_t*)&plaintext, sizeof(plaintext),
                   encrpt_msg, sizeof(encrpt_msg));

    /* Read module n value. */
    get_input_data((uint8_t*)&n_m3072_ecrypt, sizeof(n_m3072_ecrypt),
                   ecrypt_modval_msg, sizeof(ecrypt_modval_msg));

    /* Read E parameter. - RSA public key */
    get_input_data((uint8_t*)&e_m3072_ecrypt, sizeof(e_m3072_ecrypt),
                   encrypt_dval_msg, sizeof(encrypt_dval_msg));

    /* Change the endianness of data received from UART terminal. */
    CALByteReverseWord((uint32_t*)&plaintext, (sizeof(plaintext)/4));
    CALByteReverseWord((uint32_t*)&n_m3072_ecrypt, (sizeof(n_m3072_ecrypt)/4));
    CALByteReverseWord((uint32_t*)&e_m3072_ecrypt, (sizeof(e_m3072_ecrypt)/4));

    result = CALPreCompute(n_m3072_ecrypt,(uint32_t*)&encrypt_NMu[0],96u);
    if(result == SATR_SUCCESS)
    {
        CALPKTrfRes(SAT_TRUE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    result = CALExpo(plaintext, e_m3072_ecrypt, n_m3072_ecrypt,
                     (uint32_t*)&encrypt_NMu[0],  96u, 96u,
                     (uint32_t*)&ciphertext[0]);

    /* Display encrypted message. */
    if(result == SATR_SUCCESS)
    {
        CALPKTrfRes(SAT_TRUE);
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, encrypt_success, sizeof(encrypt_success));
        CALByteReverseWord((uint32_t*)&ciphertext, (sizeof(ciphertext)/4));
        display_output((uint8_t*)&ciphertext[0], sizeof(ciphertext),REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }
}

/*==============================================================================
  Message decryption using RSA.
 */
static SATR rsa_decryption(void)
{
    SATR result;
    uint16_t p_len = 0;
    uint16_t q_len = 0;
    uint16_t index = 0;

    /* Read cipher text to be decrypted using RSA decryption. */
    get_input_data((uint8_t*)&decrypt_cipher, sizeof(decrypt_cipher),
                   decrpt_msg, sizeof(decrpt_msg));

    /* Read D parameter. i.e Private key */
    get_input_data((uint8_t*)&decrypt_D, sizeof(decrypt_D),
                   read_prvtkey_msg, sizeof(read_prvtkey_msg));

    /* Read P parameter */
    p_len = get_input_data((uint8_t*)&decrypt_P, sizeof(decrypt_P),
                           read_paramP_msg, sizeof(read_paramP_msg));

    /* Read Q parameter */
    q_len = get_input_data((uint8_t*)&decrypt_Q, sizeof(decrypt_Q),
                           read_paramQ_msg, sizeof(read_paramQ_msg));

    /* Read E parameter. - RSA public key */
    get_input_data((uint8_t*)&e_m3072_ecrypt, sizeof(e_m3072_ecrypt),
                       encrypt_dval_msg, sizeof(encrypt_dval_msg));

    /* Read module n value. */
    get_input_data((uint8_t*)&n_m3072_ecrypt, sizeof(n_m3072_ecrypt),
                   ecrypt_modval_msg, sizeof(ecrypt_modval_msg));

    /* Change the endianness of data received from UART terminal. */
    CALByteReverseWord((uint32_t*)&decrypt_cipher, (sizeof(decrypt_cipher)/4));
    CALByteReverseWord((uint32_t*)&decrypt_D, (sizeof(decrypt_D)/4));
    CALByteReverseWord((uint32_t*)&decrypt_P, (sizeof(decrypt_P)/4));
    CALByteReverseWord((uint32_t*)&decrypt_Q, (sizeof(decrypt_Q)/4));
    CALByteReverseWord((uint32_t*)&n_m3072_ecrypt, (sizeof(n_m3072_ecrypt)/4));
    CALByteReverseWord((uint32_t*)&e_m3072_ecrypt, (sizeof(e_m3072_ecrypt)/4));

    /* Initiate generation of pre-compute value for P. */
    result = CAL_EXECUTE(CALPreCompute(decrypt_P, decrypt_precomP, 48u));
    if(result != SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* Initiate generation of pre-compute value for Q. */
    result = CAL_EXECUTE(CALPreCompute(decrypt_Q, decrypt_precomQ, 48u));
    if(result!=SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* Calculate P - 1 and P - 2 */
    for(index = 0; index < (p_len/4); index++)
    {
        decrypt_Pm1[index] = decrypt_P[index];
        decrypt_Pm2[index] = decrypt_P[index];
    }
    if(index == (p_len/4))
    {
        decrypt_Pm1[0]--;
        decrypt_Pm2[0] = decrypt_P[0] - 2;
    }

    /* Calculate Q - 1 */
    for(index = 0; index < q_len/4; index++)
    {
        decrypt_Qm1[index] = decrypt_Q[index];
    }
    if(index == (q_len/4))
    {
        decrypt_Qm1[0]--;
    }

    result = CALPreCompute(n_m3072_ecrypt,(uint32_t*)&encrypt_NMu[0],96u);
    if(result == SATR_SUCCESS)
    {
        CALPKTrfRes(SAT_TRUE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* Initiate generation of pre-compute value for P - 1. */
    result = CAL_EXECUTE( CALPreCompute(decrypt_Pm1, decrypt_precomPm1, 48u));
    if(result != SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* Initiate generation of pre-compute value for Q - 1. */
    result = CAL_EXECUTE(CALPreCompute(decrypt_Qm1, decrypt_precomQm1, 48u));
    if(result != SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* puiDP - Derive values d mod p-1 */
    result = CAL_EXECUTE(CALModRed(decrypt_D, decrypt_Pm1, decrypt_precomPm1,
                                 96u, 48u, decrypt_DP));
    if(result != SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* puiDQ - Derive values d mod q-1 (puiDQ) */
    result = CAL_EXECUTE(CALModRed(decrypt_D, decrypt_Qm1, decrypt_precomQm1,
                                   96u, 48u, decrypt_DQ));
    if(result != SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* ipQInv - Derive value q^(-1) mod p  = q**(p-2) mod p */
    result = CAL_EXECUTE(CALExpo(decrypt_Q, decrypt_Pm2, decrypt_P,
                                 decrypt_precomP, 48u, 48u, decrypt_QInv));
    if(result != SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, precomputes_fail_msg, sizeof(precomputes_fail_msg));
    }

    /* Decrypt cipher text. */
    result = CAL_EXECUTE( CALRSACRTCM(decrypt_cipher, decrypt_QInv, decrypt_DP,
                                   decrypt_DQ, e_m3072_ecrypt, decrypt_P,
                                   decrypt_Q, n_m3072_ecrypt, encrypt_NMu,
                                   48u, 1u, decrypt_plaintext));

    /* Display the decryption output. i.e. plain text. */
    if(result == SATR_SUCCESS)
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, decrypt_success, sizeof(decrypt_success));
        CALByteReverseWord((uint32_t*)&decrypt_plaintext, (sizeof(decrypt_plaintext)/4));
        display_output((uint8_t*)&decrypt_plaintext[0], sizeof(decrypt_plaintext),REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, decrypt_fail, sizeof(decrypt_fail));
    }

    return 1;
}
/*==============================================================================
  Display greeting message when application is started.
 */
static void display_greeting(void)
{
    MSS_UART_polled_tx(g_uart, g_greeting_msg,sizeof(g_greeting_msg));
}

/*==============================================================================
  Display the choice of cryptographic operation to perform.
 */
static void display_operation_choices(void)
{
    MSS_UART_polled_tx(g_uart, g_select_operation_msg, sizeof(g_select_operation_msg));
}

/*==============================================================================
  Display the Option to continue or exit.
 */
static void display_option(void)
{
    uint8_t rx_size;
    uint8_t rx_buff[1];

    MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
    MSS_UART_polled_tx(g_uart, (const uint8_t*)"\r\n Press any key to continue.\r\n",
              sizeof("\r\n Press any key to continue.\r\n"));
    do
    {
        rx_size = MSS_UART_get_rx(g_uart, rx_buff, sizeof(rx_buff));
    } while(0u == rx_size);
}

/******************************************************************************
 * main function.
 *****************************************************************************/
void u54_1( void )
{
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;

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

    /* Select cryptographic operation to perform */
    display_operation_choices();

    for(;;)
    {
        /* Read input from UART terminal. */
        rx_size = MSS_UART_get_rx(g_uart, rx_buff, sizeof(rx_buff));
        if(rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case '1':
                    /* Perform RSA encryption. - RSA-2048 */
                    rsa_encrypt();
                    display_option();
                    display_greeting();
                    display_operation_choices();
                break;

                case '2':
                    /* Perform RSA decryption - Decrypt, RSA-2048, CRT*/
                    rsa_decryption();
                    display_option();
                    display_greeting();
                    display_operation_choices();
                break;

              default:
              break;
            }
        }
    }
}

