/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file u54_1.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Application code running on U54_1. PolarFire SoC MSS User Crypto ECDSA
 * service example project. Please refer to README.md file for more details.
 *
 */
#include <stdio.h>
#include <string.h>
#include "inc/helper.h"
#include "mpfs_hal/mss_hal.h"
#include "mpfs_hal/common/mss_sysreg.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

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

#include "inc/config_athena.h"

#define KEY_SIZE 32
#define DATA_LENGTH_32_BYTES  32
#define REVERSE_FALSE          0
#define REVERSE_TRUE           1

/*flag to verify that ECDSA Signature has been generated before verification */
#define SIG_GEN_TRUE           1
#define SIG_GEN_FALSE          0
#define PARAM_WORD_SIZE        12


/******************************************************************************
 * User Crypto base address. This will be used in config_user.h in CAL.
 *****************************************************************************/
uint32_t g_user_crypto_base_addr = 0x22000000u;

/******************************************************************************
 * Global uart instance instance data.
 *****************************************************************************/
mss_uart_instance_t *g_uart = &g_mss_uart1_lo;

/*==============================================================================
  Messages displayed over the UART.
 */

const uint8_t g_greeting_msg[] =
"\r\n\r\n\
******************************************************************************\r\n\
*************** PolarFire User Crypto ECDSA Service Example Project **********\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the ECDSA service to calculate\r\n\
  an ECDSA signature on a provided message and also verifies the generated message \r\n\
signature\r\n";
const uint8_t g_select_operation_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select the Cryptographic operation to perform:\r\n\
    Press Key '1' to perform ECDSA signature generation \r\n\
    Press Key '2' to perform ECDSA signature verification \r\n\
------------------------------------------------------------------------------\r\n";
const uint8_t ecdsa_generation_msg[] =
"\r\n\
******************************************************************************\r\n\
                            ECDSA Signature Generation\r\n\
******************************************************************************\r\n" ;
const uint8_t ecdsa_verification_msg[] =
"\r\n\
******************************************************************************\r\n\
                            ECDSA Signature verification\r\n\
******************************************************************************\r\n";
const uint8_t select_private_key_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select one of the following options:\r\n\
    Press Key '1' to generate private key through CALECKeyPairGen() function \r\n\
    Press Key '2' to get private key from the terateram macro. \r\n\
------------------------------------------------------------------------------\r\n";

const uint8_t select_public_key_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select one of the following options:\r\n\
    Press Key '1' to generate public key through entered private_key \r\n\
    Press Key '2' to get public key from the terateram macro. \r\n\
------------------------------------------------------------------------------\r\n";

static const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";
static const uint8_t msg[] =
      "\r\n Enter message (max size : 12 words): \r\n";
static const uint8_t read_mod_n[] =
      "\r\n Enter modulus value n for P-384 elliptic curve: \r\n";
static const uint8_t read_private_key[] =
      "\r\n Enter private key d (max size : 12 words):\r\n";
static const uint8_t read_public_key_qx[] =
      "\r\n Enter public key qx (max size : 12 words):\r\n";
static const uint8_t read_public_key_qy[] =
      "\r\n Enter public key qy (max size : 12 words):\r\n";
static const uint8_t msg_gen_success[] = "\r\n\
------------------------------------------------------------------------------\r\n\
     \r EC-DSA signature generation successful \r\n";
static const uint8_t msg_gen_fail[] = "\r\n\
------------------------------------------------------------------------------\r\n\
     \r\n EC-DSA signature generation fail \r\n";
static const uint8_t msg_ver_success[] = "\r\n\
------------------------------------------------------------------------------\r\n\
     \r EC-DSA signature verification successful \r\n";
static const uint8_t msg_ver_fail[] = "\r\n\
------------------------------------------------------------------------------\r\n\
     \r\n EC-DSA signature verification fail \r\n";
static const uint8_t read_Gx[] =
      "\r\n Enter Curve generator point Gx for P-384 elliptic curve:\r\n";
static const uint8_t read_Gy[] =
      "\r\n Enter Curve generator point Gy for P-384 elliptic curve:\r\n";

static const uint8_t read_k[] =
      "\r\n Enter the random value k (exactly 12 words):\r\n";
/*==============================================================================
  Global Variables.
 */
uint32_t msg_g[PARAM_WORD_SIZE] = {
0x54686973,0x20697320,0x6f6e6c79,0x20612074,0x65737420,0x6d657373,0x6167652e,0x20497420,0x69732034,0x38206279,0x74657320,0x6c6f6e67
};
uint16_t msg_len = 0 ;
uint8_t sig_gen_flag ;
/* for simplicity the value of k is fixed */

uint32_t k_g[PARAM_WORD_SIZE] = {0xdc6b4403,0x6989a196,0xe39d1cda,0xc000812f,0x4bdd8b2d,0xb41bb33a,0xf5137258,0x5ebd1db6,0x3f0ce827,0x5aa1fd45,0xe2d2a735,0xf8749359
};
uint32_t private_key_d[PARAM_WORD_SIZE] = {
0x00};
uint32_t public_key_qx[PARAM_WORD_SIZE] = { 0x00 };
uint32_t public_key_qy[PARAM_WORD_SIZE] = { 0x00 };
/* signature paramters */

uint32_t r_g[PARAM_WORD_SIZE] = {0x00};
uint32_t s_g[PARAM_WORD_SIZE] = {0x00};

uint32_t P384_Gx[PARAM_WORD_SIZE] = {
      0x72760aB7,0x3a545e38,0xbf55296c,0x5502f25d,0x82542a38,0x59f741e0,
      0x8ba79b98,0x6e1d3b62,0xf320ad74,0x8eb1c71e,0xbe8b0537,0xaa87ca22
};
uint32_t P384_Gy[PARAM_WORD_SIZE] = {
      0x90ea0e5F,0x7a431d7c,0x1d7e819d,0x0a60b1ce,0xb5f0b8c0,0xe9da3113,
      0x289a147c,0xf8f41dbd,0x9292dc29,0x5d9e98bf,0x96262c6f,0x3617de4a
};
uint32_t P384_n[PARAM_WORD_SIZE] = {

    0xccc52973, 0xecec196a, 0x48b0a77a, 0x581a0db2,
    0xf4372ddf, 0xc7634d81, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};
uint32_t P384_npc[PARAM_WORD_SIZE+1] = {
    0x333ad68d, 0x1313e695, 0xb74f5885, 0xa7e5f24d,
    0x0bc8d220, 0x389cb27e, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001
};
uint32_t P384_n1[PARAM_WORD_SIZE] = {

    0xccc52973, 0xecec196a, 0x48b0a77a, 0x581a0db2,
    0xf4372ddf, 0xc7634d81, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffE
};
uint32_t P384_npc1[PARAM_WORD_SIZE+1] = {
    0x333ad68d, 0x1313e695, 0xb74f5885, 0xa7e5f24d,
    0x0bc8d220, 0x389cb27e, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001
};
uint32_t P384_b[PARAM_WORD_SIZE] = {
    0xd3ec2aef, 0x2a85c8ed, 0x8a2ed19d, 0xc656398d,
    0x5013875a, 0x0314088f, 0xfe814112, 0x181d9c6e,
    0xe3f82d19, 0x988e056b, 0xe23ee7e4, 0xb3312fa7
};

/*==============================================================================
  Local functions.
 */
void ecdsa_sign(void);
void ecdsa_verify(void);

/*==============================================================================
  Performs signature generation for ECDSA public-key cryptography.
 */
void ecdsa_sign(void)
{
    SATR result;
    uint16_t mod_len = 0;
    uint8_t opt = 0;

    /* Read message value. */
    msg_len = get_input_data((uint8_t*)&msg_g[0], sizeof(msg_g), msg,
                   sizeof(msg));

    /* Read Generator Gx. */
    get_input_data((uint8_t*)&P384_Gx[0], sizeof(P384_Gx), read_Gx,
                   sizeof(read_Gx));

    /* adjust endianness */
    CALWordReverse(P384_Gx, sizeof(P384_Gx)/4);
    CALByteReverseWord(P384_Gx, sizeof(P384_Gx)/4);

    /* Read Generator Gy */
    get_input_data((uint8_t*)&P384_Gy[0], sizeof(P384_Gy), read_Gy,
                   sizeof(read_Gy));

    /* adjust endianness */
    CALWordReverse(P384_Gy, sizeof(P384_Gy)/4);
    CALByteReverseWord(P384_Gy, sizeof(P384_Gy)/4);

    /* Read Random parameter K */
    get_input_data((uint8_t*)&k_g[0], sizeof(k_g), read_k,
                   sizeof(read_k));

    /* adjust endianness */
    CALWordReverse(k_g, sizeof(k_g)/4);
    CALByteReverseWord(k_g, sizeof(k_g)/4);

    /* Read modulus value n (length of cyclic group) */
    get_input_data((uint8_t*)&P384_n[0], sizeof(P384_n), read_mod_n,
                   sizeof(read_mod_n));

    /* adjust endianness */
    CALWordReverse(P384_n, sizeof(P384_n)/4);
    CALByteReverseWord(P384_n, sizeof(P384_n)/4);

    /*precompute of mod n */
    result = CALPreCompute(P384_n,P384_npc,PARAM_WORD_SIZE);
    if(SATR_SUCCESS == result)
    {
        /* transfer the result to P384_npc */
        CALPKTrfRes(SAT_TRUE);
    }
    else
    {
        MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n N Mod value is not correct ");
    }
    /* P384 curve b parameter is fixed for this example */

    MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
    MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\n Fixed B parameter for P384 curve: \r\n" );
    display_output((uint8_t*)&P384_b[0],sizeof(P384_b),REVERSE_TRUE);

    /* option to get the value of private key */
    get_input_data((uint8_t*)&opt, sizeof(opt),select_private_key_msg,
                   sizeof(select_private_key_msg));
    if (opt == 1)
    {

        /* copy the contents of P384_n to P384_n1 because we need mod -1 */
        memcpy(P384_n1,P384_n,sizeof(P384_n));

        /* search for first non zero word and subtract 1 from it */
        for(int wc = 0 ; wc < PARAM_WORD_SIZE; wc++)
        {
            if(P384_n1[wc] != 0)
            {
                P384_n1[0] -= 1;
                break;
            }
            else
            {
                continue ;
            }
        }

        /* precompute of mod(N-1) */
        result = CALPreCompute(P384_n1,P384_npc1,PARAM_WORD_SIZE);
        if(SATR_SUCCESS == result)
        {
            /* transfer the result to P384_npc */
            CALPKTrfRes(SAT_TRUE);
        }
        else
        {
            MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n N-1 Mod value is not correct ");
        }

        /* generate private and public key pair */
        result = CALECKeyPairGen(&k_g[0],P384_Gx,P384_Gy,P384_MOD,SAT_NULL,
                P384_n1,P384_npc1,P384_b,private_key_d,public_key_qx,public_key_qy,12);

        result = CALPKTrfRes(SAT_TRUE);
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\n Generated private key : \r\n" );
        display_output((uint8_t*)&private_key_d[0],sizeof(private_key_d),REVERSE_TRUE);
    }
    else
    {
        get_input_data((uint8_t*)&private_key_d[0], sizeof(private_key_d), read_private_key,
                       sizeof(read_private_key));

        /* adjust endianness */
        CALWordReverse(private_key_d, sizeof(private_key_d)/4);
        CALByteReverseWord(private_key_d, sizeof(private_key_d)/4);
    }

    /* Calculates ECDSA signature */
    result = CALECDSASignHash(&msg_g[0], SATHASHTYPE_SHA384, msg_len, P384_Gx,
                                P384_Gy, (const uint32_t*)&k_g,
                                (const uint32_t*)&private_key_d, P384_b,
                                P384_MOD, SAT_NULL, P384_n, P384_npc, 12, r_g,
                                s_g, SAT_TRUE, X52CCR_DEFAULT);
    if(SATR_SUCCESS == result)
    {
        result = CALPKTrfRes(SAT_TRUE);
        switch(result)
        {
            case SATR_SUCCESS:

                /* Display the generated signature in hex format. */
                MSS_UART_polled_tx(g_uart, msg_gen_success, sizeof(msg_gen_success));
                MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\n R value:\r\n\r\n");
                display_output((uint8_t*)&r_g[0],sizeof(r_g),REVERSE_TRUE);
                MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\n S value:\r\n\r\n");
                display_output((uint8_t*)&s_g[0],sizeof(s_g),REVERSE_TRUE);
                break;
            case SATR_VALPARMX:

                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n X parameter  not in range \r\n");
                MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
                break ;
            case SATR_VALPARMY:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n Y parameter not in range \r\n");
                MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
                break;

            case SATR_VALPARMB:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n B parameter greater than modulus \r\n");
                MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
                break ;

            case SATR_VALIDATEFAIL :
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n Generator point is not on the curve \r\n");
                MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
                break ;

            case SATR_SIGNPARMD:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n private_key parameter is not in range [1,N-1] \r\n");
                MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
                break ;

            case SATR_SIGNPARMK :
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n random parameter K is not in range [1,N-1] \r\n");
                MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
                break ;

            case SATR_SIGNFAIL :
                MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
                break ;
        }

    }
    else
    {
        /* ECDSA signature generation fail */
        MSS_UART_polled_tx(g_uart, msg_gen_fail, sizeof(msg_gen_fail));
    }
}

/*==============================================================================
  Performs signature verification for ECDSA public-key cryptography.
 */
void ecdsa_verify(void)
{
    SATR result;
    uint8_t opt = 0;

    /* option to get public key */
    get_input_data((uint8_t*)&opt, sizeof(opt),select_public_key_msg,
                   sizeof(select_public_key_msg));
    if (opt == 1)
    {
        /* generate private and public key pair */
        result = CALECMult(private_key_d,P384_Gx, P384_Gy, P384_b, P384_MOD, SAT_NULL,
                           12, 0,public_key_qx, public_key_qy);

        if(SATR_SUCCESS == result)
        {
            CALPKTrfRes(SAT_TRUE);

            /* display the generated public key */
            MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
            MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\n Public key generation successful \r\n" );
            MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n public_key_qx value : \r\n\r\n",
                      sizeof("\r\n public_key_qx value : \r\n"));
            display_output((uint8_t*)&public_key_qx[0], sizeof(public_key_qx),REVERSE_TRUE);
            MSS_UART_polled_tx(g_uart, (const uint8_t *)"\r\n public_key_qy value: \r\n\r\n",
                      sizeof("\r\n public_key_qy value: \r\n"));
            display_output((uint8_t*)&public_key_qy[0], sizeof(public_key_qy),REVERSE_TRUE);
        }
    }
    else
    {
        /* get public key x point */
        get_input_data((uint8_t*)&public_key_qx[0], sizeof(public_key_qx), read_public_key_qx,
                       sizeof(read_public_key_qx));

        /* adjust endianness */
        CALWordReverse(public_key_qx, sizeof(public_key_qx)/4);
        CALByteReverseWord(public_key_qx, sizeof(public_key_qx)/4);

        /* get public key y point */
        get_input_data((uint8_t*)&public_key_qy[0], sizeof(public_key_qy), read_public_key_qy,
                       sizeof(read_public_key_qy));

        /* adjust endianness */
        CALWordReverse(public_key_qy, sizeof(public_key_qy)/4);
        CALByteReverseWord(public_key_qy, sizeof(public_key_qy)/4);
    }

    // validate that public key is on the curve
    result = CALECPtValidate(public_key_qx, public_key_qy, P384_b, P384_MOD, SAT_NULL,12);

    if(SATR_SUCCESS == result)
    {
        result = CALPKTrfRes(SAT_TRUE);
        switch(result)
        {
            case SATR_SUCCESS:

                MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n Validation successful public key point is on the curve \r\n");
                break;
            case SATR_VALPARMX:

                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n X parameter  not in range \r\n");
                MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n validation Fail \r\n");
                break ;
            case SATR_VALPARMY:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n Y parameter not in range \r\n");
                MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n validation Fail \r\n");
                break;
            case SATR_VALPARMB:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n B parameter greater than modulus \r\n");
                MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n validation Fail \r\n");
                break ;
            case SATR_VALIDATEFAIL :
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n public key point is not on the curve \r\n");
                MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n validation Fail \r\n");
                break ;
        }

    }
    else
    {
        /* public key validation fail */
        MSS_UART_polled_tx(g_uart, g_separator, sizeof(g_separator));
        MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n validation Fail \r\n");
    }

    result = CALECDSAVerifyHash(&msg_g[0], SATHASHTYPE_SHA384, msg_len, P384_Gx,
                                P384_Gy, public_key_qx,public_key_qy,
                                r_g,s_g, P384_b,
                                P384_MOD, SAT_NULL, P384_n, P384_npc, 12, 0, SAT_TRUE, X52CCR_DEFAULT);

    /* Display the generated signature in hex format. */
    if(SATR_SUCCESS == result)
    {
        result = CALPKTrfRes(SAT_TRUE);
        switch(result)
        {
            case SATR_SUCCESS:

                /* Display the generated signature in hex format. */
                MSS_UART_polled_tx(g_uart, msg_ver_success, sizeof(msg_ver_success));
                break;
            case SATR_VALPARMX:

                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n X parameter  not in range \r\n");
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;
            case SATR_VALPARMY:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n Y parameter not in range \r\n");
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break;

            case SATR_VALPARMB:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n B parameter greater than modulus \r\n");
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;

            case SATR_VERPARMR:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n R parameter Not in [1,N-1]\r\n");
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;

            case SATR_VERPARMS:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n S parameter Not in [1,N-1]\r\n");
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;

            case SATR_VALIDATEFAIL :
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n Generator point is not on the curve \r\n");
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;

            case SATR_PAF:
                MSS_UART_polled_tx_string(g_uart, (uint8_t*)"\r\n\n point at infinity Generated \r\n");
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;

            case SATR_VERIFYFAIL :
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;

            case SATR_SIGNFAIL :
                MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
                break ;
        }

    }
    else
    {
        /* ECDSA signature generation fail */
        MSS_UART_polled_tx(g_uart, msg_ver_fail, sizeof(msg_ver_fail));
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
 * E51 startup.
 */
void u54_1(void)
{
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;
    uint8_t sig_gen_success = SIG_GEN_FALSE ;

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


    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CRYPTO, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_ATHENA, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);


    PLIC_init();

    /* Using UART with interrupt routed to PLIC. */
    MSS_UART_init( g_uart,
                   MSS_UART_115200_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* Initialize the Athena core */
    ATHENAREG->ATHENA_CR = SYSREG_ATHENACR_RESET | SYSREG_ATHENACR_RINGOSCON;
    ATHENAREG->ATHENA_CR = SYSREG_ATHENACR_RINGOSCON;

    /* Initializes the Athena Processor */
    CALIni();

    /* Display greeting message. */
    display_greeting();

    /* Select cryptographic operation to perform */
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
                    /* Performs signature generation for ECDSA */
                    MSS_UART_polled_tx(g_uart, ecdsa_generation_msg, sizeof(ecdsa_generation_msg));
                    ecdsa_sign();
                    sig_gen_success = SIG_GEN_TRUE ;
                    display_option();
                    display_greeting();
                    display_operation_choices();
                break;

                case '2':
                    /* Performs signature verification for ECDSA */
                    MSS_UART_polled_tx(g_uart, ecdsa_verification_msg, sizeof(ecdsa_verification_msg));
                    if(sig_gen_success == SIG_GEN_TRUE)
                    {
                        ecdsa_verify();
                    }
                    else
                    {
                        MSS_UART_polled_tx_string(g_uart,(uint8_t*)"\r\n Please perform ECDSA signature Generation at least once before ECDSA verification \r\n ");
                    }
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
