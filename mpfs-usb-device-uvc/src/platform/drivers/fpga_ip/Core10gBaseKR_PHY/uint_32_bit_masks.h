/***************************************************************************//**
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * @file uint_32_bit_masks.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief unsigned 32 bit masks
 * 
 */

#ifndef UNIT_32_BIT_MASKS_H_
#define UNIT_32_BIT_MASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Single Bit Mask
 */
#define MASK_BIT_31                               ((uint32_t)(0x80000000UL))
#define MASK_BIT_30                               ((uint32_t)(0x40000000UL))
#define MASK_BIT_29                               ((uint32_t)(0x20000000UL))
#define MASK_BIT_28                               ((uint32_t)(0x10000000UL))
#define MASK_BIT_27                               ((uint32_t)(0x08000000UL))
#define MASK_BIT_26                               ((uint32_t)(0x04000000UL))
#define MASK_BIT_25                               ((uint32_t)(0x02000000UL))
#define MASK_BIT_24                               ((uint32_t)(0x01000000UL))
#define MASK_BIT_23                               ((uint32_t)(0x00800000UL))
#define MASK_BIT_22                               ((uint32_t)(0x00400000UL))
#define MASK_BIT_21                               ((uint32_t)(0x00200000UL))
#define MASK_BIT_20                               ((uint32_t)(0x00100000UL))
#define MASK_BIT_19                               ((uint32_t)(0x00080000UL))
#define MASK_BIT_18                               ((uint32_t)(0x00040000UL))
#define MASK_BIT_17                               ((uint32_t)(0x00020000UL))
#define MASK_BIT_16                               ((uint32_t)(0x00010000UL))
#define MASK_BIT_15                               ((uint32_t)(0x00008000UL))
#define MASK_BIT_14                               ((uint32_t)(0x00004000UL))
#define MASK_BIT_13                               ((uint32_t)(0x00002000UL))
#define MASK_BIT_12                               ((uint32_t)(0x00001000UL))
#define MASK_BIT_11                               ((uint32_t)(0x00000800UL))
#define MASK_BIT_10                               ((uint32_t)(0x00000400UL))
#define MASK_BIT_9                                ((uint32_t)(0x00000200UL))
#define MASK_BIT_8                                ((uint32_t)(0x00000100UL))
#define MASK_BIT_7                                ((uint32_t)(0x00000080UL))
#define MASK_BIT_6                                ((uint32_t)(0x00000040UL))
#define MASK_BIT_5                                ((uint32_t)(0x00000020UL))
#define MASK_BIT_4                                ((uint32_t)(0x00000010UL))
#define MASK_BIT_3                                ((uint32_t)(0x00000008UL))
#define MASK_BIT_2                                ((uint32_t)(0x00000004UL))
#define MASK_BIT_1                                ((uint32_t)(0x00000002UL))
#define MASK_BIT_0                                ((uint32_t)(0x00000001UL))

/*
 * Multi Bit Mask
 */
#define BIT_MASK_32_BITS                          ((uint32_t)(0xFFFFFFFFUL))
#define BIT_MASK_31_BITS                          ((uint32_t)(0x7FFFFFFFUL))
#define BIT_MASK_30_BITS                          ((uint32_t)(0x3FFFFFFFUL))
#define BIT_MASK_29_BITS                          ((uint32_t)(0x1FFFFFFFUL))
#define BIT_MASK_28_BITS                          ((uint32_t)(0x0FFFFFFFUL))
#define BIT_MASK_27_BITS                          ((uint32_t)(0x07FFFFFFUL))
#define BIT_MASK_26_BITS                          ((uint32_t)(0x03FFFFFFUL))
#define BIT_MASK_25_BITS                          ((uint32_t)(0x01FFFFFFUL))
#define BIT_MASK_24_BITS                          ((uint32_t)(0x00FFFFFFUL))
#define BIT_MASK_23_BITS                          ((uint32_t)(0x007FFFFFUL))
#define BIT_MASK_22_BITS                          ((uint32_t)(0x003FFFFFUL))
#define BIT_MASK_21_BITS                          ((uint32_t)(0x001FFFFFUL))
#define BIT_MASK_20_BITS                          ((uint32_t)(0x000FFFFFUL))
#define BIT_MASK_19_BITS                          ((uint32_t)(0x0007FFFFUL))
#define BIT_MASK_18_BITS                          ((uint32_t)(0x0003FFFFUL))
#define BIT_MASK_17_BITS                          ((uint32_t)(0x0001FFFFUL))
#define BIT_MASK_16_BITS                          ((uint32_t)(0x0000FFFFUL))
#define BIT_MASK_15_BITS                          ((uint32_t)(0x00007FFFUL))
#define BIT_MASK_14_BITS                          ((uint32_t)(0x00003FFFUL))
#define BIT_MASK_13_BITS                          ((uint32_t)(0x00001FFFUL))
#define BIT_MASK_12_BITS                          ((uint32_t)(0x00000FFFUL))
#define BIT_MASK_11_BITS                          ((uint32_t)(0x000007FFUL))
#define BIT_MASK_10_BITS                          ((uint32_t)(0x000003FFUL))
#define BIT_MASK_9_BITS                           ((uint32_t)(0x000001FFUL))
#define BIT_MASK_8_BITS                           ((uint32_t)(0x000000FFUL))
#define BIT_MASK_7_BITS                           ((uint32_t)(0x0000007FUL))
#define BIT_MASK_6_BITS                           ((uint32_t)(0x0000003FUL))
#define BIT_MASK_5_BITS                           ((uint32_t)(0x0000001FUL))
#define BIT_MASK_4_BITS                           ((uint32_t)(0x0000000FUL))
#define BIT_MASK_3_BITS                           ((uint32_t)(0x00000007UL))
#define BIT_MASK_2_BITS                           ((uint32_t)(0x00000003UL))



#ifdef __cplusplus
}
#endif

#endif /* UNIT_32_BIT_MASKS_H_ */
