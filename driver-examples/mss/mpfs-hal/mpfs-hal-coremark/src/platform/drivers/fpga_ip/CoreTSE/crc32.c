
/*******************************************************************************
 * Copyright 2014 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file crc32.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief crc32 source file
 * 
 * CRC-32-IEEE 802.3
 * x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 +
 * x^4 +*x^2 + x + 1
 *
 */                                                                              

#include "crc32.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Calculates 32 bits CRC value of given data.*/
uint32_t
TSE_crc32(uint32_t value, const uint8_t *data, uint32_t data_length)
{
    int i, j;
    unsigned int CRC = 0xffffffff;

    for (i = 0; i < data_length; i++)
    {
        unsigned char byte = data[i];

        for (j = 0; j < 8; j++)
        {
            if ((byte & 0x1) ^ (CRC >> 31))
                CRC = (CRC << 1) ^ 0x04c11db7;
            else
                CRC <<= 1;
            byte >>= 1;
        }
    }
    return CRC;
}

/*Calculates 32 bit CRC value of given data, using standard Ethernet CRC function*/
uint32_t
TSE_ethernet_crc(const uint8_t *data, uint32_t data_length)
{
    return TSE_crc32(0xffffffffUL, data, data_length);
}

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
