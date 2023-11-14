/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file ccm.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC User Crypto CCM algorithm implementation. See file 
 * "ccm.h" for description of the functions implemented in this file.
 *
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hal/hal.h"
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
#include "ccm.h"

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t g_mac[128] = {0x00};
uint8_t A[128] = {0x00}; /* A_i blocks for encryption input */
static uint8_t S[128] ={0x00}; /* S_i = encrypted A_i blocks */
static uint8_t B[128] = {0x00};/* B_i blocks for CBC-MAC input */

extern SATR CCMMAC(SATMACTYPE eMACType, const SATUINT32_t *pKey, SATUINT32_t uiKeyLen,
          const void *pMsg, SATUINT32_t uiMsgLen, void *pMAC);

static inline int int_to_uint16(uint8_t *field, uint16_t value)
{
    field[0] = (value >> 8) & 0xff;
    field[1] = value & 0xff;

    return 2;
}

static inline int int_to_uint32(uint8_t *field, uint32_t value)
{
    field[0] = (value >> 24) & 0xff;
    field[1] = (value >> 16) & 0xff;
    field[2] = (value >> 8) & 0xff;
    field[3] = value & 0xff;

    return 4;
}

static inline int int_to_uint64(unsigned char *field, uint64_t value)
{
  field[0] = (value >> 56) & 0xff;
  field[1] = (value >> 48) & 0xff;
  field[2] = (value >> 40) & 0xff;
  field[3] = (value >> 32) & 0xff;
  field[4] = (value >> 24) & 0xff;
  field[5] = (value >> 16) & 0xff;
  field[6] = (value >> 8) & 0xff;
  field[7] = value & 0xff;

  return 8;
}

static void set_counter(uint8_t A[CCM_BLOCKSIZE],uint16_t L,uint16_t cnt,uint16_t C)
{
    uint8_t i_;
    volatile uint16_t temp = 0;
    memset((A) + CCM_BLOCKSIZE - (L), 0, (L));

    temp = (((uint16_t)1 << (8 * L)) - 1);

    (C) = (cnt) & 0xFFFFFFFF;

    for (i_ = CCM_BLOCKSIZE - 1; (C) && (i_ > (L)); --i_, (C) >>= 8)
    {
        (A)[i_] |= (C) & 0xFF;
    }
}

static uint8_t roundUp(uint8_t numToRound, uint8_t multiple)
{
    return ((numToRound + multiple - 1) / multiple) * multiple;
}

static inline void
block0(size_t M,       /* number of auth bytes */
       size_t L,       /* number of bytes to encode message length */
       size_t la,      /* l(a) octets additional authenticated data */
       size_t lm,      /* l(m) message length */
       uint8_t nonce[CCM_BLOCKSIZE],
       uint8_t *result)
{
    uint8_t i;

    result[0] = CCM_FLAGS(la, M, L);

    /* copy the nonce */
    memcpy(result + 1, nonce, CCM_BLOCKSIZE - L - 1);

    for (i = 0; i < L; i++)
    {
        result[15-i] = lm & 0xff;
        lm >>= 8;
    }
}

/*==============================================================================
 Form input block B1.. Bn for Authentication.
 */
static uint8_t
add_auth_data(const uint8_t *msg, size_t la, uint8_t *B1)
{
    size_t i,j;
    uint8_t status = 0u;
    uint8_t len = 0;

    memset(B1, 0, CCM_BLOCKSIZE);

    if (!la)
    {
        return 0;
    }

    if (la < 0xFF00)
    {
        j = 2;
        int_to_uint16(B1, la);
    }
    else if (la <= UINT32_MAX)
    {
        j = 6;
        int_to_uint16(B1, 0xFFFE);
        int_to_uint32(B1+2, la);
    }
    else
    {
        j = 10;
        int_to_uint16(B1, 0xFFFF);
        int_to_uint64(B1+2, la);
    }

    i = min(CCM_BLOCKSIZE - j, la);
    memcpy(B1 + j, msg, i);
    la -= i;
    msg += i;
    len = i+j;
    B1 += len;

    while (la > CCM_BLOCKSIZE)
    {
        for (i = 0; i < CCM_BLOCKSIZE; ++i)
        {
            *B1++ = *msg++;
        }
        la -= CCM_BLOCKSIZE;
        len += CCM_BLOCKSIZE;
    }

    if (la)
    {
        memcpy(B1, msg, la);
        len += CCM_BLOCKSIZE;
    }

    return len;
}

/*==============================================================================
 Encryption using AES-128
 */
static inline void
ccm_aes_encrypt(size_t L, uint16_t counter,
        uint8_t *msg, size_t len,
        uint8_t A[CCM_BLOCKSIZE],
        uint8_t S[CCM_BLOCKSIZE],
        uint8_t* key,
        uint8_t *pDest)
{
    static uint16_t counter_tmp;
    uint8_t status = 0u;

    set_counter(A, L, counter, counter_tmp);

    status = CALSymEncryptDMA(SATSYMTYPE_AES128, (uint32_t *)key,
                              SATSYMMODE_CTR, A, SAT_TRUE, msg,
							  pDest, 16, X52CCR_DEFAULT);

    CALPKTrfRes(SAT_TRUE);

}

/*==============================================================================
 Input Block B calculation for MAC
 */
uint16_t
ccm_mac_block(size_t M, size_t L, uint8_t *nonce,
              uint8_t *msg, size_t lm,
              const uint8_t *aad, size_t la)
{
    volatile uint8_t *b_ptr = (uint8_t *)&B;
    uint8_t return_len = 0;
    uint8_t len = 0;
    uint8_t i = 0;

    /*
     * Clear B array.
     */
    for(i = 0; i < sizeof(B); i++)
    {
        B[i] = 0;
    }

    /* Create the initial authentication block B0 i.e. define a sequence of
     * blocks B0
     */
    block0(M, L, la, lm, nonce, B);
    b_ptr += CCM_BLOCKSIZE;
    return_len = CCM_BLOCKSIZE;

    /* Input Block B1 ... Bn */
    len = add_auth_data( aad, la, (uint8_t *)b_ptr);
    len = roundUp(len, 16);
    b_ptr += len;
    return_len += len;

    /* Last Block calculation. */
    memcpy((uint8_t *)b_ptr, msg, lm);
    len = roundUp(lm, 16);
    return_len += len;

    return return_len;
}

/*==============================================================================
 Form Input block and perform CCM Encryption
 */
uint16_t
ccm_encrypt_message(size_t M, size_t L, uint8_t nonce[CCM_BLOCKSIZE],
                    uint8_t *msg, size_t lm, uint8_t *key, uint8_t *pDest)
{
    uint8_t status = 0u;
    uint16_t msg_len = 0;
    size_t i,j;
    size_t len;
    uint16_t counter_tmp;
    uint16_t counter = 1;

    /* Save original length */
    len = lm;

    /* Initialize block template */
    A[0] = L-1;

    /* Copy the nonce */
    memcpy(A + 1, nonce, CCM_BLOCKSIZE - L - 1);

    while (lm >= CCM_BLOCKSIZE)
    {
        /* Encrypt */
        ccm_aes_encrypt( L, counter, msg, CCM_BLOCKSIZE, A, S, key, pDest);

        /* Update local pointers */
        lm -= CCM_BLOCKSIZE;
        msg += CCM_BLOCKSIZE;
        pDest += CCM_BLOCKSIZE;
        counter++;
        msg_len += CCM_BLOCKSIZE;
    }

    if (lm)
    {
        /* Encrypt */
        ccm_aes_encrypt( L, counter, msg, lm, A, S, key, pDest);

        /* Update local pointers */
        msg += lm;
        msg_len += lm;
        pDest += lm;
    }

    /* Calculate S_0 */
    set_counter(A, L, 0, counter_tmp);

    status = CALSymEncryptDMA(SATSYMTYPE_AES128, (uint32_t *)key,
                              SATSYMMODE_CTR, A, SAT_TRUE, msg,
                              S , 16, X52CCR_DEFAULT);

    CALPKTrfRes(SAT_TRUE);

    return msg_len;
}

/*==============================================================================
 CCM Decryption
 */
uint16_t
ccm_decrypt_message(size_t M, size_t L,
                    uint8_t nonce[CCM_BLOCKSIZE],
                    uint8_t *msg, size_t lm,
                    uint8_t *key,
                    uint8_t *pDest)
{
    uint16_t counter_tmp = 0;
    uint16_t counter = 1;
    uint16_t i = 0;
    uint16_t error, n = 0;
    uint8_t status = 0;

    for(i = 0; i < 80; i++)
    {
        A[i] = 0;
    }

    if (lm < M)
      goto error;

    /* save original length */
    lm -= M;

    /* Initialize block template */
    A[0] = L-1;

    /* Copy the nonce */
    memcpy(A + 1, nonce, CCM_BLOCKSIZE - L - 1);

    while (lm >= CCM_BLOCKSIZE)
    {
        /* Decrypt */
        ccm_aes_encrypt( L, counter, msg, CCM_BLOCKSIZE, A, S, key, pDest);

        for(i = 0; i < CCM_BLOCKSIZE; i++)
        {
            S[i] = pDest[i];
        }

        /* Update local pointers */
        lm -= CCM_BLOCKSIZE;
        msg += CCM_BLOCKSIZE;
        pDest += CCM_BLOCKSIZE;
        counter++;
    }

    if (lm)
    {
        /* Decrypt */
        ccm_aes_encrypt( L, counter, msg, lm, A, S, key, pDest);

        /* update local pointers */
        msg += lm;
        pDest += lm;

    }

    /* Calculate S_0 */
    set_counter(A, L, 0, counter_tmp);
    CALSymEncryptDMA(SATSYMTYPE_AES128, (uint32_t *)key,
                         SATSYMMODE_CTR, A, SAT_TRUE, msg,
                         S , 16, X52CCR_DEFAULT);

    CALPKTrfRes(SAT_TRUE);

    return 0;

   error:
      return -1;
}

/*==============================================================================
 CCM Encryption
 */
void
MACCcmEnrypt(size_t M, size_t L, uint8_t *pNonce, uint8_t *pSrc, size_t lm,
             const uint8_t *pAad, size_t la, uint16_t *pKey, void *pDest)
{
    uint8_t i = 0;
    uint32_t len = 0;
    uint8_t *p_msg = pDest;
    uint8_t status = 0;

    /* Calculate MAC input block. */
    len = ccm_mac_block(M, L, pNonce, pSrc, lm, pAad, la);

    /* Calculate MAC. */
    status = CCMMAC(SATMACTYPE_AESCMAC128, (uint32_t *)pKey, 16, B, len, g_mac);

    /* Encrypt the message. */
    len = ccm_encrypt_message(M, L, pNonce, pSrc, lm, (uint8_t *)pKey, pDest);
    p_msg += len;

    /* The authentication value U is computed by encrypting T with the key
     * stream block S_0 and truncating it to the desired length.
     * U := T XOR first-M-bytes( S_0 )
     */
    for (i = 0; i < M; ++i)
    {
        *p_msg++ = S[i] ^ g_mac[i];
    }
}

/*==============================================================================
 Perform CCM decryption and calculate MAC value of decrypted data for
 verification.
 */
void
MACCcmDecrypt(size_t M, size_t L, uint8_t *pNonce, uint8_t *pEncMsg, size_t lm,
              const uint8_t *pAad, size_t la, uint16_t *pKey, uint8_t *pDest,
              uint8_t *pMAC)
{
    uint8_t status = 0;
    uint8_t i = 0;

    /* Decrypt the encrypted message.*/
    ccm_decrypt_message(M, L, pNonce, pEncMsg, lm, (uint8_t *)pKey, pDest);

    for(i = 0; i < M; i++)
    {
        pMAC[i] = S[i];
    }

}


#ifdef __cplusplus
}
#endif
