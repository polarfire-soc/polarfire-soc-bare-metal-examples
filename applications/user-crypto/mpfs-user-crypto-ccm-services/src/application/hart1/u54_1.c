/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
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
#include "cal/calini.h"
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/common.h"
#include "inc/config_athena.h"
#include "ccm.h"

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

/*==============================================================================
  Messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\
******************************************************************************\r\n\
******* PolarFire SoC MSS User Crypto CCM Example Project ********************\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the PolarFire SoC MSS User \r\n\
 Crypto CCM service using AES cipher Services. The following CCM services \r\n\
 are demonstrated:\r\n\
  1 - Encryption and authenticated.\r\n\
  2 - Decryption and authenticated.\r\n";

const uint8_t g_select_operation_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select the CCM operation to perform:\r\n\
    Press key '1' to perform Encryption and Authenticated \r\n\
    Press key '2' to perform Decryption and Authenticated \r\n\
------------------------------------------------------------------------------\r\n";

static const uint8_t aes_encrypt_select_msg[] =
"\r\n Selected Encryption and Authentication using CCM. \r\n";

static const uint8_t aes_decrypt_select_msg[] =
"\r\n Selected Decryption and Authentication using CCM. \r\n";

static const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";

static const uint8_t read_key_msg[] =
"\r\n Enter the 128-bit/16-byte key: \r\n";

static const uint8_t read_nonce[] =
"\r\n Enter the Nonce (max : 16 Bytes): \r\n";

static const uint8_t read_aad[] =
"\r\n Enter the Additional authenticated data (max : 32 Bytes): \r\n";

static const uint8_t read_msg[] =
"\r\n Enter the input data to encrypt and authenticate (max : 64 bytes):\r\n";

static const uint8_t read_MAC_len[] =
"\r\n Enter the Number of octets in authentication field: \r\n\
  Press key '1' for 4 bytes \r\n\
  Press key '2' for 8 bytes \r\n\
  Press key '3' for 12 bytes \r\n\
  Press key '4' for 16 bytes \r\n";

static const uint8_t read_dec_msg[] =
"\r\n Enter the encrypted and authenticated message (max : 64 bytes):\r\n";

volatile struct test_vector data = { 0x00 };

static void clear_ccm_var(void)
{
    uint16_t var = 0;

    data.M = 0;
    data.L = 0;
    data.lm = 0;
    data.la = 0;

    for(var = 0; var < CCM_BLOCKSIZE; var++)
    {
        data.key[var] = 0;
        data.nonce[var] = 0;
    }

    for(var = 0; var < 100; var++)
    {
        data.msg[var] = 0;
        data.output[var] = 0;
        data.dec_mac[var] = 0;
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

/*==============================================================================
  Perform CCM encryption.
 */
void perform_ccm_encrypt( void )
{
    uint16_t len = 0;
    uint8_t addr = 0x4;
    uint8_t i = 0x4;
    uint8_t status = 0;
    uint8_t text[256] = {0x00};
    uint8_t admin[4] = {0x00};
    uint8_t input_text[256] = {0x00};
    uint8_t user_key[12] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x10, 0x11};

    /* Initializes the Athena Processor. */
    CALIni();

    /* Clear input and output variables */
    clear_ccm_var();

    /* Get key. */
    get_input_data((uint8_t*)&data.key, 16u, read_key_msg, sizeof(read_key_msg));

    /* Nonce N */
    len = get_input_data((uint8_t*)&data.nonce, 16u, read_nonce, sizeof(read_nonce));
    data.L = 15 - len;

    /* Additional authenticated data a */
    data.la = get_input_data((uint8_t*)&data.aad, 32u, read_aad, sizeof(read_aad));

    /* Message m */
    data.lm = get_input_data((uint8_t*)data.msg, 64u, read_msg, sizeof(read_msg));

    /* Read MAC length */
    get_input_data((uint8_t*)&data.M, 1u, read_MAC_len, sizeof(read_MAC_len));
    data.M *= 4;

    /* CCM Encryption */
    MACCcmEnrypt(data.M,
                 data.L,
                 (uint8_t*)data.nonce,
                 (uint8_t*)data.msg,
                 data.lm,
                 (uint8_t*)data.aad,
                 data.la,
                 (uint16_t*)data.key,
                 (uint8_t *)data.output);

    MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
    MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Encrypted and Authentication data:\r\n",
              sizeof("\r\n Encrypted and Authentication data:\r\n"));
    display_output((uint8_t*)data.output, data.lm + data.M,REVERSE_FALSE);
}

/*==============================================================================
  Perform CCM decryption.
 */
void perform_ccm_decrypt( void )
{
    uint16_t len = 0;
    uint8_t addr = 0x4;
    uint8_t i = 0x4;
    uint8_t status = 0;
    uint8_t text[256] = {0x00};
    uint8_t admin[4] = {0x00};
    uint8_t input_text[256] = {0x00};
    uint8_t user_key[12] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, \
                            0x09, 0x0a, 0xb, 0x0c};

    /* Clear input and output variables */
    clear_ccm_var();

    /* Get key. */
    get_input_data((uint8_t*)&data.key, 16u, read_key_msg, sizeof(read_key_msg));

    /* Nonce N */
    len = get_input_data((uint8_t*)&data.nonce, 16u, read_nonce,
                          sizeof(read_nonce));

    data.L = 15 - len;

    /* Additional authenticated data a */
    data.la = get_input_data((uint8_t*)&data.aad, 32u, read_aad,
                              sizeof(read_aad));

    /* Encrypted and authenticated message c */
    data.lm = get_input_data((uint8_t*)data.msg, 64u, read_dec_msg,
                             sizeof(read_dec_msg));

    /* Read MAC length */
    get_input_data((uint8_t*)&data.M, 1u, read_MAC_len, sizeof(read_MAC_len));
    data.M *= 4;

    /* Decrypt and Authenticate the input data.*/
    MACCcmDecrypt( data.M,
                   data.L,
                   (uint8_t*)data.nonce,
                   (uint8_t*)data.msg,
                   data.lm,
                   (uint8_t*)data.aad,
                   data.la,
                   (uint16_t*)&data.key,
                   (uint8_t *)&data.output,
                   (uint8_t *)&data.dec_mac);

    /* Display decrypted data and MAC value. */
    MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
    MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Decrypted  data:\r\n",
              sizeof("\r\n Decrypted  data:\r\n"));
    display_output((uint8_t*)(data.output), data.lm - data.M,REVERSE_FALSE);

    MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n Authenticated data (MAC):\r\n",
                                  sizeof("\r\n Authenticated data (MAC):\r\n"));
    display_output((uint8_t*)(data.dec_mac), data.M,REVERSE_FALSE);
}

/* Main function for the hart1(U54_1 processor). */
void u54_1(void)
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

    /* Initializes the Athena Processor */
    CALIni();

    /* Display greeting message. */
    display_greeting();

    /* Select CCM operation */
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
                    /* Perform Encryption using AES-128.*/
                    MSS_UART_polled_tx(g_uart, aes_encrypt_select_msg,
                              sizeof(aes_encrypt_select_msg));

                    /* Authenticate and encrypt the input data.*/
                    perform_ccm_encrypt();

                    display_option();
                    display_greeting();
                    display_operation_choices();
                break;

                case '2':
                    /* Perform decryption using AES-128 */
                    MSS_UART_polled_tx(g_uart, aes_decrypt_select_msg,
                              sizeof(aes_decrypt_select_msg));

                    /* Authenticate and decrypt the input data.*/
                    perform_ccm_decrypt();

                    display_option();
                    display_greeting();
                    display_operation_choices();
                break;

                default:
                break;
            }
        }
    }

    return;
}
