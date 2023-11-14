/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on U54_1. Application demonstrating the use
 * of message authentication services. Please refer to README.md file for more 
 * details.
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
#include "cal/mac.h"


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
******* PolarFire SoC User Crypto message authentication Example Project *****\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the User Crypto Message \r\n\
 authentication services and hash services. The following User Crypto service are\r\n\
 demonstrated:\r\n\
  1 - Galois/Counter Mode, GCM Message Authentication.\r\n\
  2 - Message authentication code - HMAC-SHA-256.\r\n\
  3 - Message authentication code - AES-CMAC-256.\r\n\
  4 - SHA-256 service.\r\n";
const uint8_t g_select_operation_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select the Cryptographic operation to perform:\r\n\
    Press Key '1' to perform AES-GCM-256 encryption \r\n\
    Press Key '2' to perform HMAC SHA 256  \r\n\
    Press Key '3' to perform HMAC AES-CMAC-256 \r\n\
    Press Key '4' to perform hash operation \r\n\
------------------------------------------------------------------------------\r\n";
static const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";
const uint8_t read_dma_enable_ip[] =
"\r\n Enter 1 to perform Symmetric encryption with DMA or \r\n\
       0 to perform Symmetric encryption without DMA: \r\n";

/*==============================================================================
  Global Variables.
 */
/* AES-128 encryption/decryption. */
 uint8_t g_iv[16];
 uint8_t g_plain_text[128];

 uint8_t g_key_256bit[32];
 uint8_t g_auth_data[128];
 uint8_t tag[32];

 uint8_t g_mac[32];
 uint8_t g_encrypted_text[128];

 uint8_t g_hash_msg[128] = {0x00};
 uint8_t g_hash_result[32];

/* Helper function.*/
static void clr_hmac_sha_var(void);
static void clr_hmac_aes_cmac_var(void);
static void clr_gcm_msg_auth_var(void);
static void clear_sha256_var(void);

/*==============================================================================
  Hash operation using SHA-256
 */
void sha_256bit_hash(void)
{
    uint16_t count = 0u;
    uint8_t status;
    uint8_t use_dma = 0;

    const uint8_t sha_msg[] =
    "\r\n Enter message to perform hashing (max 128 Bytes): \r\n";

    /* Clear input and output variables */
    clear_sha256_var();

    /* Get the input Text/Data from the user and store it for Hashing. */
    count = get_input_data(&g_hash_msg[0],sizeof(g_hash_msg), sha_msg,
                           sizeof(sha_msg));

    /* Use dma or not */
    use_dma = enable_dma(read_dma_enable_ip, sizeof(read_dma_enable_ip));

    if(use_dma != 1)
    {
        /* Without DMA */
        status = CALHash(SATHASHTYPE_SHA256, g_hash_msg, count, g_hash_result);
    }
    else
    {
        /* With DMA */
        status = CALHashDMA(SATHASHTYPE_SHA256, g_hash_msg, count, g_hash_result,
                            X52CCR_DEFAULT);
    }

    if(SATR_SUCCESS == status)
    {
        if(use_dma == 1)
        {
            CALPKTrfRes(SAT_TRUE);
        }
        /* Display the calculated Hash value */
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t*)"\r\n Calculated hash value for the input data:\r\n",
                  sizeof("\r\n Calculated hash value for the input data:\r\n"));
        display_output(g_hash_result, sizeof(g_hash_result),REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        /* Display the error information */
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n SHA-256 service fail\r\n",
                  sizeof("\r\n SHA-256 service fail\r\n"));
    }
}

/*==============================================================================
  Message authentication code - AES-CMAC-256
 */
static void hmac_aes_cmac_256(void)
{
    uint8_t status = 0u;
    uint16_t msg_len = 0;

    const uint8_t read_key_msg[] =
    "\r\n Enter the 256-bit/32-byte key: \r\n";
    const uint8_t read_data_msg[] =
    "\r\n Enter the input data (max 128 bytes):\r\n";

    /* Clear all global variable. */
    clr_hmac_aes_cmac_var();

    /* Get key. key size 256 bit*/
    get_input_data(&g_key_256bit[0], sizeof(g_key_256bit), read_key_msg, sizeof(read_key_msg));

    /* Read the 128 bytes of input data from UART terminal. */
    msg_len = get_input_data(&g_plain_text[0], sizeof(g_plain_text), read_data_msg, sizeof(read_data_msg));

    /* Without DMA */
    status = CALMAC(SATMACTYPE_AESCMAC256,(uint32_t*)&g_key_256bit, sizeof(g_key_256bit),
                    g_plain_text, msg_len, g_mac);

    /* Display the encrypted data in hex format. */
    if(SATR_SUCCESS == status)
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Message Authentication successful \r\n Generated MAC:\r\n",
        sizeof("\r\n Message Authentication successful \r\n Generated MAC:\r\n"));
        display_output(g_mac, 8,REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Message Authentication fail\r\n",
                  sizeof("\r\n Message Authentication fail\r\n"));
    }
}

/*==============================================================================
  Message authentication code - HMAC-SHA-256
 */
static void hmac_sha_256(void)
{
    uint8_t status = 0u;
    uint16_t msg_len = 0;
    uint8_t use_dma = 0;

    const uint8_t read_key_msg[] =
    "\r\n Enter the 256 bit key to be used: \r\n";
    const uint8_t read_data_msg[] =
    "\r\n Enter input data(max 128 Bytes):\r\n";
    const uint8_t read_dma_enable_ip[] =
    "\r\n Enter 1 to perform Symmetric encryption with DMA or \r\n\
       0 to perform Symmetric encryption without DMA: \r\n";

    /* Clear Global variables. */
    clr_hmac_sha_var();

    /* Get key. key size 256 bit*/
    get_input_data(&g_key_256bit[0], sizeof(g_key_256bit), read_key_msg,
                   sizeof(read_key_msg));

    /* Read the 32 bytes of input data from UART terminal. */
    msg_len = get_input_data(&g_plain_text[0],sizeof(g_plain_text),
                             read_data_msg, sizeof(read_data_msg));

    /* Use dma or not */
    use_dma = enable_dma(read_dma_enable_ip, sizeof(read_dma_enable_ip));

    if(use_dma != 1)
    {
        /* Without DMA */
        status = CALMAC(SATMACTYPE_SHA256, (uint32_t*)&g_key_256bit, sizeof(g_key_256bit),
                        g_plain_text, msg_len, g_encrypted_text);
    }
    else
    {
        /* With DMA */
        status = CALMACDMA(SATMACTYPE_SHA256, (uint32_t*)&g_key_256bit, sizeof(g_key_256bit),
                           g_plain_text, msg_len, g_encrypted_text, X52CCR_DEFAULT);
    }

    /* Display the encrypted data in hex format. */
    if(SATR_SUCCESS == status)
    {
        if(use_dma == 1)
        {
            CALPKTrfRes(SAT_TRUE);
        }
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart,
                  (const uint8_t *)"\r\n Message Authentication successful \r\n Output data:\r\n",
                  sizeof("\r\n Message Authentication successful \r\n Output data:\r\n"));
        display_output(g_encrypted_text, 32u,REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Error\r\n", sizeof("\r\n Error\r\n"));
    }
}

/*==============================================================================
  GCM Message Authentication - AES-GCM-256
 */

static void gcm_msg_auth(void)
{
    uint8_t status = 0u;
    uint8_t use_dma = 0;
    uint16_t msg_len = 0;
    uint16_t aad_len = 0;
    uint32_t g_tag_len = 0;

    const uint8_t read_dma_enable_ip[] =
    "\r\n Enter 1 to perform Symmetric encryption with DMA or \r\n\
       0 to perform Symmetric encryption without DMA: \r\n";
    const uint8_t read_key_msg[] =
    "\r\n Enter the 256-bit/32-byte key: \r\n";
    const uint8_t read_data_msg[] =
    "\r\n Enter the input data to encrypt(PT) (max: 128 Bytes):\r\n";
    const uint8_t read_iv_msg[] =
    "\r\n Enter the 16 bytes(128 bit) initialization vector(IV): \r\n";
    const uint8_t read_auth_data_msg[] =
    "\r\n Enter the 16 bytes of authentication data(AAD): \r\n";

    /* Clear all global variable related to AES GCM */
    clr_gcm_msg_auth_var();

    /* Get key used for encryption. key size 256 bit*/
    get_input_data(&g_key_256bit[0], sizeof(g_key_256bit), read_key_msg,
                   sizeof(read_key_msg));

    /* Get the Initialization Vector value. IV size is 128 bit */
    get_input_data(g_iv, sizeof(g_iv), read_iv_msg, sizeof(read_iv_msg));

    /* Get the data to be encrypted. Data size is 16 bytes*/
    msg_len = get_input_data(&g_plain_text[0], sizeof(g_plain_text),
                             read_data_msg, sizeof(read_data_msg));

    /* Get additional authentication data (AAD). AAD size is 16 bytes*/
    aad_len = get_input_data(&g_auth_data[0], sizeof(g_auth_data), read_auth_data_msg,
                             sizeof(read_auth_data_msg));

    /* Use dma or not */
    use_dma = enable_dma(read_dma_enable_ip, sizeof(read_dma_enable_ip));

    if(use_dma != 1)
    {
        /* Without DMA */
        status  = CALSymEncAuth(SATSYMTYPE_AES256, (uint32_t*)&g_key_256bit[0],
                                SATSYMMODE_GCM, g_iv, g_plain_text,
                                g_encrypted_text, msg_len, g_auth_data, aad_len,
                                tag, 12);
    }
    else
    {
        /* With DMA */
        status  = CALSymEncAuthDMA(SATSYMTYPE_AES256, (uint32_t*)&g_key_256bit[0],
                                   SATSYMMODE_GCM, g_iv, g_plain_text,
                                   g_encrypted_text, msg_len, g_auth_data,
                                   aad_len, tag, 12, X52CCR_DEFAULT);
    }

    /* Display the encrypted data in hex format. */
    if(SATR_SUCCESS == status)
    {
        CALPKTrfRes(SAT_TRUE);
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Encrypted data(CT):\r\n",
                  sizeof("\r\n Encrypted data(CT):\r\n"));
        display_output(g_encrypted_text, sizeof(g_encrypted_text)/2,REVERSE_FALSE);
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n\n Message Authentication Code(Tag):\r\n",
                  sizeof("\r\n\n Message Authentication Code(Tag):\r\n"));
        display_output(tag, sizeof(tag),REVERSE_FALSE);
    }
    else
    {
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Error\r\n", sizeof("\r\n Error\r\n"));
    }
    /* GCM verification */

    /* clear the plaintext buffer */
    uint16_t bc ;
    for(bc = 0 ; bc < sizeof(g_plain_text) ; bc++)
    {
        g_plain_text[bc] = 0 ;
    }

    status = CALSymDecVerify(SATSYMTYPE_AES256, (uint32_t*)&g_key_256bit[0], SATSYMMODE_GCM, g_iv
            , g_encrypted_text, g_plain_text, msg_len, g_auth_data, aad_len, tag, 12);
    if(SATR_SUCCESS == status)
    {
        status = CALSymTrfRes(SAT_TRUE);
        if(SATR_SUCCESS == status)
        {

        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Decrypted data(CT):\r\n",
                  sizeof("\r\n Decrypted data(CT):\r\n"));
        display_output(g_plain_text, sizeof(g_plain_text)/2,REVERSE_FALSE);
        MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n\n Message Authentication successful(Tag):\r\n",
                  sizeof("\r\n\n Message Authentication successful(Tag):\r\n"));
        display_output(tag, sizeof(tag),REVERSE_FALSE);
        }
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
static void clr_hmac_sha_var(void)
{
    uint16_t var = 0;

    for(var = 0; var < sizeof(g_key_256bit); var++)
    {
        g_key_256bit[var] = 0;
    }
    for(var = 0; var < sizeof(g_plain_text); var++)
    {
        g_plain_text[var] = 0;
    }
}

static void clr_gcm_msg_auth_var(void)
{
    uint16_t index = 0;

    clr_hmac_sha_var();

    for(index = 0; index < sizeof(g_iv); index++)
    {
        g_iv[index] = 0;
    }
    for(index = 0; index < sizeof(g_auth_data); index++)
    {
        g_auth_data[index] = 0;
    }
}

static void clr_hmac_aes_cmac_var(void)
{
    uint16_t var = 0;

    clr_hmac_sha_var();

    for(var = 0; var < sizeof(g_mac); var++)
    {
        g_mac[var] = 0;
    }
}

static void clear_sha256_var(void)
{
    volatile uint32_t index;

    for(index = 0u; index < sizeof(g_hash_msg); index++)
    {
        g_hash_msg[index] = 0u;
    }
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
                  /* Galois/Counter Mode, GCM Message Authentication */
                  gcm_msg_auth();
                  display_option();
                  display_greeting();
                  display_operation_choices();
                break;

                case '2':
                  /* Perform HMAC SHA 256 */
                  hmac_sha_256();
                  display_option();
                  display_greeting();
                  display_operation_choices();
                break;

                case '3':
                  /* Perform HMAC AES-CMAC-256 */
                  hmac_aes_cmac_256();
                  display_option();
                  display_greeting();
                  display_operation_choices();
                break;

                case '4':
                  /* Perform SHA-256*/
                  sha_256bit_hash();
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
