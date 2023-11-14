/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file ccm_mac.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC User Crypto MAC-CCM algorithm - MAC implementation. 
 *
 */
#include "cal/calpolicy.h"
#include "cal/caltypes.h"
#include "cal/calenum.h"
#include "cal/utils.h"
#include "cal/hash.h"
#include "cal/calcontext.h"

#include "cal/shaf5200.h"
#include "cal/sym.h"
#include "cal/aesf5200.h"

static SATMACTYPE CurMAC=SATMACTYPE_NULL;
static SATSYMTYPE CurSym=SATSYMTYPE_NULL;
static SATUINT32_t uiIV[4];
static SATUINT32_t uiCMACK[4];
static SATUINT32_t uiSymMsgLen;

static SATR AES_MAC_Write(const void *pBuffer, SATUINT32_t uiBufLen)
{
    SATR rReturn;

    SATUINT32_t uiBytesCBC,i;
    SATUINT8_t *pui8Buffer, *pui8IV, *pui8K;
    pui8Buffer=(SATUINT8_t *)pBuffer;

    /* Validate length */
    if(uiBufLen > uiSymMsgLen)
    {
        return SATR_BADLEN;
    }

    /* Calculate number of CBC bytes to process before final block */
    if((uiSymMsgLen - uiBufLen) > 0)
    {
        uiBytesCBC = uiBufLen;
    }
    else
    {
        if(uiBufLen > 16)
        {
            if((uiBufLen % 16) != 0)
            {
                uiBytesCBC = uiBufLen - (uiBufLen % 16);
            }
            else
            {
                uiBytesCBC = uiBufLen - 16;
            }
        }
        else
        {
            uiBytesCBC = 0;
        }
    }

    /* Update message length state */
    uiSymMsgLen -= uiBufLen;

    /* Process all message blocks except last */   
    if (uiBytesCBC >= 16)
    {
        aesf5200aes (CurSym, SATSYMMODE_CBC, uiIV, SAT_TRUE, pui8Buffer,
                     SAT_NULL, uiBytesCBC, SAT_FALSE);

        rReturn = CALSymTrfRes(SAT_TRUE);

        pui8Buffer += uiBytesCBC;
    }

    /* XOR final message with subkey */
    if(uiSymMsgLen == 0)
    {
        /* Calculate size of final block */ 
        pui8IV = (SATUINT8_t*)uiIV; 
        pui8K = (SATUINT8_t*)uiCMACK;
        for (i = 0; i < (uiBufLen - uiBytesCBC); i++)
        {
            *pui8IV = *pui8IV ^ *pui8Buffer;
            pui8IV++; 
            pui8Buffer++;
        }
        if (i<16)
        {
            *pui8IV = *pui8IV ^ 0x80;
            pui8IV++;  
            i++;
        }
        for (; i < 16; i++)
        {
            pui8IV++;
        }

        /* Encrypt final block */    
        rReturn = aesf5200aes(CurSym, SATSYMMODE_ECB, SAT_NULL, SAT_FALSE,
                              uiIV, uiIV, 16, SAT_FALSE);
        if(rReturn != SATR_SUCCESS)
        {
            return rReturn;
        }

        rReturn = CALSymTrfRes(SAT_TRUE);
        if(rReturn != SATR_SUCCESS)
        {
            return rReturn;
        }
    }

    return rReturn;
}

SATR MACWrite(const void *pBuffer, SATUINT32_t uiBufLen)
{
    SATR rRet;

    if(CurMAC == SATMACTYPE_NULL)
    {
        return SATR_BADMACTYPE;
    }

    if(pBuffer == SAT_NULL)
    {
        return SATR_BADPARAM;
    }

    if(CurMAC >= SATMACTYPE_AESCMAC128 && CurMAC <= SATMACTYPE_AESCMAC256)
    {
        rRet = AES_MAC_Write(pBuffer, uiBufLen);
    }
    /* No default else statement - CALMACIni checks type and sets it to SAT_NULL 
     if it's invalid. Then this function checks & returns if type ==SAT_NULL */
    return rRet;
}

static void macsubkey(const void *pSubKeyIn, void *pSubKeyOut)
{
    SATUINT8_t uiC,uiCNew;
    SATUINT8_t *pui8In,*pui8Out;
    SATINT8_t i;
    uiC = 0;
    
    pui8In  = ((SATUINT8_t *)pSubKeyIn)  + 15;
    pui8Out = ((SATUINT8_t *)pSubKeyOut) + 15;
    for( i = 15; i >= 0; i--) 
    {
        uiCNew = (*pui8In & 0x80) >> 7;
        *pui8Out = *pui8In << 1 | uiC;
        pui8Out--;
        pui8In--;
        uiC = uiCNew;
    }
    pui8In  = ((SATUINT8_t *)pSubKeyIn)  + 15;
    pui8Out = ((SATUINT8_t *)pSubKeyOut) + 15;
    if (uiC)
    { 
        *pui8Out = *pui8In ^ 0x87; 
    }
}

SATR MACIni(SATMACTYPE eMACType, const SATUINT32_t *pKey, SATUINT32_t uiKeyLen, 
  SATUINT32_t uiMsgLen)
{
    SATUINT32_t i;
    SATR rReturn = SATR_BADMACTYPE;

    if (pKey == SAT_NULL)
    {
        return SATR_BADPARAM;
    }

    for( i = 0; i < 4; i++)
    { 
        uiIV[i] = 0;
    }

    CurMAC = eMACType;
    uiSymMsgLen = uiMsgLen;
    CurSym = SATSYMTYPE_AES128;
    CALSymEncrypt(CurSym, pKey, SATSYMMODE_ECB, SAT_NULL, SAT_FALSE, uiIV, uiCMACK, 128/8);  

    CALSymTrfRes(SAT_TRUE);
    macsubkey(uiCMACK,uiCMACK);

    if ((uiMsgLen & 0xf) || (uiMsgLen == 0))
    { 
        macsubkey(uiCMACK,uiCMACK); 
    }
    
    return SATR_SUCCESS;
}

SATR MACRead(void *pMAC)
{
    SATR rReturn = SATR_SUCCESS;
    
    if(CurMAC == SATMACTYPE_NULL)
    {
        return SATR_BADMACTYPE;
    }
    else if(CurMAC >= SATMACTYPE_AESCMAC128 && CurMAC <= SATMACTYPE_AESCMAC256)
    {
        CALMemCopy(pMAC, uiIV, 16 );
        CALMemClear(uiIV, 16);
        rReturn = SATR_SUCCESS;
    }
    else
    {
        return SATR_BADMACTYPE;
    }

    /* Clear eCurMAC as data has been read */
    CurMAC = SATMACTYPE_NULL;
    return rReturn;
}

SATR CCMMAC(SATMACTYPE eMACType, const SATUINT32_t *pKey, SATUINT32_t uiKeyLen,
  const void *pMsg, SATUINT32_t uiMsgLen, void *pMAC)
{
    SATR rReturn;

    /* Initialize MAC operation */
    rReturn = MACIni(eMACType, pKey, uiKeyLen, uiMsgLen);
    if (rReturn != SATR_SUCCESS)
    {
        return rReturn;
    }

    /* Write message */
    rReturn = MACWrite(pMsg, uiMsgLen);
    if (rReturn != SATR_SUCCESS)
    {
        return rReturn;
    }

    /* Read resulting MAC */
    rReturn = MACRead(pMAC);
    return rReturn;
}
