/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file ccm.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief This file contains the application programming interface for the
 * PolarFire SoC User Crypto MAC using CCM service.
 *
 */
/*=========================================================================*//**
  @mainpage PolarFire User Crypto CCM Bare Metal Driver.
 *//*=========================================================================*/
/*=========================================================================*//**
  @mainpage CCM MAC Algorithm implementation.
  CCM(Counter with CBC-MAC) is used to provide assurance of the confidentiality
  and the authenticity of computer data by combining the techniques of the 
  Counter (CTR) mode and the Cipher Block Chaining-Message Authentication Code
  (CBC-MAC) algorithm. The TeraFire functions, namely CTR-AES is used to 
  construct CCM along with some additional minor application code, to perform 
  the CCM padding requirements.

  Initialization and Configuration
  The CAL library driver is initialized through a call to the CALIni()
  function. The CALIni() function must be called before any other
  CAL Library driver or CCM functions is called.
  
  CCM services.
  The CCM driver can ebe used to execute Authentication encryption and 
  decryption services using the following function.
      • MACCcmEnrypt ()
      • MACCcmDecrypt ()
      
 *//*=========================================================================*/
  
#ifndef __CCM_H
#define __CCM_H 1


#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * Macro definitation.
 */
#define CCM_BLOCKSIZE 16
#define min(a,b) ((a) < (b) ? (a) : (b))
#define CCM_FLAGS(A,M,L) (((A > 0) << 6) | (((M - 2)/2) << 3) | (L - 1))
#define MASK_L(_L) ((1 << 8 * _L) - 1)

 /***************************************************************************//**
 * Global Structure.
 */
struct test_vector {
  size_t M, L;          /* M - Number of Authenticate bytes.
                           L - Number of bytes to encode message length. */
  size_t lm;            /* overall message length*/
  size_t la;            /* number of bytes additional data */
  uint8_t key[CCM_BLOCKSIZE];
  uint8_t nonce[CCM_BLOCKSIZE];
  uint8_t msg[128];
  uint8_t aad[128];
  uint8_t output[128];
  uint8_t dec_mac[128];
};

 /***************************************************************************//**
 * Global Functions.
 */
 /***************************************************************************//**
 * The function MACCcmEnrypt() performs authenticated encryption with associated
 * data, for confidentiality and integrity. The associated data, pointed at by 
 * pAad, with byte length la, is used in the computation of the message
 * authentication code and but not encrypted and is not included in the output 
 * of CCM mode. AAD can be used to authenticate plaintext packet headers, or
 * contextual information that affects the interpretation of the message.  Users
 * who do not wish to authenticate additional data can provide a string of 
 * length zero. The data pointed at by pSrc, is encrypted and used in the
 * computation of the message authentication code according to the 
 * SATSYMTYPE_AES128 encryption algorithm, and SATSYMMODE_CTR mode. The length of 
 * the data pointed by pSrc is specified by lm. The final result consists of the
 * encrypted message followed by the encrypted authentication value. And will be 
 * stored at the location pointed at by pDest. The byte length of the message 
 * authentication code is specified by M. The data will be encrypted using the 
 * key pointed at by pKey. The Nonce is pointed at by pNonce.

 * @param M            The M parameter specifies the the size of authentication
 *                     field.
 *
 * @param L            The L parameter specifies the number of octets in length
 *                     field.
 *
 * @param pNonce       The pNonce parameter is a pointer to the buffer where the
 *                     nonce value will be stored.
 *
 * @param pSrc         The pSrc parameter is pointer to the plaintext/authenticate
 *                     data buffer.
 *
 * @param lm           The lm parameter specifies the length of the plaintext in
 *                     bytes the encrypt/authenticate.
 *
 * @param pAad         The aad parameter is pointer the Additional authenticated
 *                     data buffer.
 *
 * @param la           The la parameter specifies the length of Additional 
 *                     authenticated data buffer.
 *
 * @param pKey         The pKey parameter is pointer to key.
 *
 * @param pDest        The pDest parameter is a pointer to ciphertext data
 *                     buffer.
 *
 * @return             This function does not return any value.
 *
 * Example:
 * @code
 *     /* CCM Encryption *//*
 *     MACCcmEnrypt(data.M,
 *                  data.L,
 *                  data.nonce,
 *                  data.msg,
 *                  data.lm,
 *                  data.aad,
 *                  data.la,
 *                  data.key,
 *                  data.output);
 * @endcode
 */
void
MACCcmEnrypt(size_t M, size_t L, uint8_t *pNonce, uint8_t *pSrc, size_t lm,
             const uint8_t *pAad, size_t la, uint16_t *pKey, void *pDest);

/***************************************************************************//**
 * The MACCcmDecrypt() function performs authenticated decryption with
 * additional authenticated data, for confidentiality and integrity. The
 * additional authenticated data data, pointed at by pAad, with byte length la,
 * is used in the computation of the message authentication code, but is not
 * decrypted. The data pointed at by pEncMsg, with byte length lm, is both
 * decrypted and used in the computation of the message authentication code
 * according to the selected decryption algorithm, based on SATSYMTYPE_AES128
 * encryption algorithm, and SATSYMMODE_CTR mode.
 * The decrypted data will be stored at the location pointed at by pDest, and
 * the message authentication code will be compared to the value stored at the
 * location pointed at by pMAC; The byte length of the message authentication
 * code is specified by la. The data will be decrypted using the key pointed at
 * by pKey variable. The Nonce value is pointed at by pNonce.

 * @param M            The M parameter specifies the length of the message
 *                     authentication code in bytes.
 *
 * @param L            The L parameter specifies the number of octets in length
 *                     field.
 *
 * @param pNonce       The pNonce parameter is a pointer to the buffer where the
 *                     nonce value will be stored.
 *
 * @param pEncMsg      The pEncMsg parameter is pointer to the ciphertext/verify
 *                     data buffer.
 *
 * @param lm           The lm parameter specifies the length of the plaintext in
 *                     bytes the encrypt/authenticate.
 *
 * @param pAad         The pAad parameter is pointer the Additional
 *                     authenticated data buffer.i.e. verify-only data buffer.
 *
 * @param la           The la parameter specifies the length of Additional 
 *                     authenticated data buffer.i.e. verify-only data buffer.
 *
 * @param pKey         The pKey parameter is pointer to key.
 *
 * @param pDest        The pDest parameter is a pointer to plaintext data
 *                     buffer.
 *
 * @param pMAC         The pMAC parameter is a pointer to the message
 *                     authentication code.
 *
 *
 * @return             This function does not return any value.
 *
 * Example:
 * @code
 * @endcode
 */
void
MACCcmDecrypt(size_t M, size_t L, uint8_t *pNonce, uint8_t *pEncMsg, size_t lm,
              const uint8_t *pAad, size_t la, uint16_t *pKey, uint8_t *pDest,
              uint8_t *pMAC);

#ifdef __cplusplus
}
#endif

#endif /* __CCM_H */
