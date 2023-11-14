/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on U54_1. Application demonstrating the AES
 * cryptography service. Please refer to README.md file for more details.
 * 
 */
#include <stdio.h>
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

#define KEY_SIZE 32
#define DATA_LENGTH_32_BYTES  32
#define REVERSE_FALSE          0
#define REVERSE_TRUE           1

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
********* PolarFire SoC User Crypto Cryptography Service Example Project ******\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the PolarFire User Crypto \r\n\
 Cryptography Services. The following User Athena services are demonstrated:\r\n\
  1 - AES-256 encryption.\r\n\
  2 - AES-256 decryption.\r\n";

const uint8_t g_select_operation_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select the Cryptographic operation to perform:\r\n\
    Press key '1' to perform AES-256 encryption \r\n\
    Press key '2' to perform AES-256 decryption \r\n\
------------------------------------------------------------------------------\r\n";
static const uint8_t aes_encrypt_select_msg[] =
"\r\n Selected AES-256 encryption service. \r\n";
static const uint8_t aes_decrypt_select_msg[] =
"\r\n Selected AES-256 decryption service. \r\n";
static const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";
static const uint8_t read_key_msg[] =
"\r\n Enter the 256-bit/32-byte key: \r\n";
static const uint8_t read_data_msg[] =
"\r\n Enter the 16 bytes of input data to encrypt:\r\n";
static const uint8_t read_iv_msg[] =
"\r\n Enter the 16 bytes initialization vector(IV):\r\n";
static const uint8_t read_dma_enable_ip[] =
"\r\n Enter 1 to perform Symmetric encryption with DMA or \r\n\
   0 to perform Symmetric encryption without DMA: \r\n";
static const uint8_t read_encrypted_data_msg[] =
"\r\n Enter the 16 bytes of cipher text to decrypt: \r\n";

/*==============================================================================
  Global Variables.
 */
static uint8_t g_cipher_text[KEY_SIZE/2];
static uint8_t g_key_256bit[KEY_SIZE];
static uint8_t g_iv[KEY_SIZE/2];
static uint8_t g_plain_text[KEY_SIZE];

/*==============================================================================
  Clear AES global variable.
 */
static void clear_aes_var(void)
{
    uint16_t var = 0;

    for(var = 0; var < sizeof(g_key_256bit); var++)
    {
        g_key_256bit[var] = 0;
    }
    for(var = 0; var < sizeof(g_iv); var++)
    {
        g_iv[var] = 0;
    }
    for(var = 0; var < sizeof(g_plain_text); var++)
    {
        g_plain_text[var] = 0;
    }
    for(var = 0; var < sizeof(g_cipher_text); var++)
    {
        g_cipher_text[var] = 0;
    }
}

/*==============================================================================
  AES-256 encryption.
 */
static void aes256_encryption(void)
{
    uint8_t status = 0u;
    uint8_t use_dma = 0;
    uint16_t msg_len = 0;

    /* Clear input and output variables */
    clear_aes_var();

    /* Get key. */
    get_input_data(&g_key_256bit[0], sizeof(g_key_256bit), read_key_msg,
                   sizeof(read_key_msg));

    /* Get Initialization Vector value */
    get_input_data(&g_iv[0], sizeof(g_iv), read_iv_msg, sizeof(read_iv_msg));

    /* Read input data to be encrypt. */
    msg_len = get_input_data(&g_plain_text[0], sizeof(g_plain_text),
                             read_data_msg, sizeof(read_data_msg));

    /* Use dma or not */
    use_dma = enable_dma(read_dma_enable_ip, sizeof(read_dma_enable_ip));

    if(use_dma != 1)
    {
        /* Without DMA */
        status = CALSymEncrypt(SATSYMTYPE_AES256, (uint32_t *)&g_key_256bit[0],
                               SATSYMMODE_ECB, g_iv, SAT_TRUE, g_plain_text,
                               g_cipher_text , msg_len);
    }
    else
    {
        /* With DMA */
        status = CALSymEncryptDMA(SATSYMTYPE_AES256, (uint32_t *)&g_key_256bit[0],
                                  SATSYMMODE_ECB, g_iv, SAT_TRUE,
                                  g_plain_text, g_cipher_text , msg_len,
                                  X52CCR_DEFAULT);
    }

    /* Display the encrypted data. i.e. Cipher text*/
    if(SATR_SUCCESS == status)
    {
        CALPKTrfRes(SAT_TRUE);
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Encrypted data:\r\n",
                  sizeof("\r\n Encrypted data:\r\n"));
        display_output(g_cipher_text, sizeof(g_cipher_text),REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Error\r\n",
                  sizeof("\r\n Error\r\n"));
    }
}

/*==============================================================================
  AES-256 decryption.
 */
static void aes256_decryption(void)
{
    uint8_t status = 0u;
    uint8_t use_dma = 0;
    uint16_t cipher_len = 0;


    /* Clear input and output variables. */
    clear_aes_var();

    /* Get Key from the user to perform Decryption. Note this key should be
     * same as that used for message encryption. */
    get_input_data(&g_key_256bit[0], sizeof(g_key_256bit), read_key_msg,
                   sizeof(read_key_msg));

    /* Get Initialization Vector value */
    get_input_data(&g_iv[0], sizeof(g_iv), read_iv_msg, sizeof(read_iv_msg));

    /* Read encrypted data i.e 16 bytes of cipher text to be decrypt. */
    cipher_len = get_input_data(&g_cipher_text[0], sizeof(g_cipher_text),
                   read_encrypted_data_msg, sizeof(read_encrypted_data_msg));

    /* Use dma or not */
    use_dma = enable_dma(read_dma_enable_ip, sizeof(read_dma_enable_ip));

    if(use_dma != 1)
    {
        status = CALSymDecrypt(SATSYMTYPE_AES256, (uint32_t*)&g_key_256bit[0],
                               SATSYMMODE_ECB, g_iv, SAT_TRUE, g_cipher_text,
                               g_plain_text, cipher_len);
    }
    else
    {
        status = CALSymDecryptDMA(SATSYMTYPE_AES256, (uint32_t*)&g_key_256bit[0],
                                  SATSYMMODE_ECB, g_iv, SAT_TRUE, g_cipher_text,
                                  g_plain_text, cipher_len,
                                  X52CCR_DEFAULT);
    }

    /* Display the Decrypted data i.e. Plain text. */
    if(SATR_SUCCESS == status)
    {
        CALPKTrfRes(SAT_TRUE);
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Decrypted data:\r\n",
                  sizeof("\r\n Decrypted data:\r\n"));
        display_output(g_plain_text, sizeof(g_plain_text),REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Error\r\n",
                  sizeof("\r\n Error\r\n"));
    }
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
  Display the Option to continue.
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

    /* Select cryptographic operation */
    display_operation_choices();

    for(;;)
    {
        /* Read inputs from UART terminal. */
        rx_size = MSS_UART_get_rx(g_uart, rx_buff, sizeof(rx_buff));
        if(rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case '1':
                    /* Perform encryption using AES-256 */
                    MSS_UART_polled_tx(g_uart, aes_encrypt_select_msg,
                              sizeof(aes_encrypt_select_msg));
                    aes256_encryption();
                    display_option();
                    display_greeting();
                    display_operation_choices();
                break;

                case '2':
                    /* Perform decryption using AES-256 */
                    MSS_UART_polled_tx(g_uart, aes_decrypt_select_msg,
                              sizeof(aes_decrypt_select_msg));
                    aes256_decryption();
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

