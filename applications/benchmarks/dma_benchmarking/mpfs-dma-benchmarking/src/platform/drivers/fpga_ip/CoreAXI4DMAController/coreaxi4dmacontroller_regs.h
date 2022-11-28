/*******************************************************************************
 * Copyright 2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Register bit offsets and masks definitions for CoreAXI4DMAController.
 *
 */
#ifndef CORE_AXI4DMACTRL_REGS_H_
#define CORE_AXI4DMACTRL_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 * Version Register
 */
#define VER_REG_OFFSET                          0x00u

#define VER_BLDN_OFFSET                         0x00u
#define VER_BLDN_MASK                           0x000000FFu
#define VER_BLDN_SHIFT                          0u

#define VER_MNRN_OFFSET                         0x00u
#define VER_MNRN_MASK                           0x0000FF00u
#define VER_MNRN_SHIFT                          8u

#define VER_MJRN_OFFSET                         0x00u
#define VER_MJRN_MASK                           0x00FF0000u
#define VER_MJRN_SHIFT                          16u

/*------------------------------------------------------------------------------
 * Start Operation Register
 */
#define START_REG_OFFSET                        0x04u

#define START_ID0_OFFSET                        0x04u
#define START_ID0_MASK                          0x00000001u
#define START_ID0_SHIFT                         0u

#define START_ID1_OFFSET                        0x04u
#define START_ID1_MASK                          0x00000002u
#define START_ID1_SHIFT                         1u

#define START_ID2_OFFSET                        0x04u
#define START_ID2_MASK                          0x00000004u
#define START_ID2_SHIFT                         2u

#define START_ID3_OFFSET                        0x04u
#define START_ID3_MASK                          0x00000008u
#define START_ID3_SHIFT                         3u

#define START_ID4_OFFSET                        0x04u
#define START_ID4_MASK                          0x00000010u
#define START_ID4_SHIFT                         4u

#define START_ID5_OFFSET                        0x04u
#define START_ID5_MASK                          0x00000020u
#define START_ID5_SHIFT                         5u

#define START_ID6_OFFSET                        0x04u
#define START_ID6_MASK                          0x00000040u
#define START_ID6_SHIFT                         6u

#define START_ID7_OFFSET                        0x04u
#define START_ID7_MASK                          0x00000080u
#define START_ID7_SHIFT                         7u

#define START_ID8_OFFSET                        0x04u
#define START_ID8_MASK                          0x00000100u
#define START_ID8_SHIFT                         8u

#define START_ID9_OFFSET                        0x04u
#define START_ID9_MASK                          0x00000200u
#define START_ID9_SHIFT                         9u

#define START_ID10_OFFSET                       0x04u
#define START_ID10_MASK                         0x00000400u
#define START_ID10_SHIFT                        10u

#define START_ID11_OFFSET                       0x04u
#define START_ID11_MASK                         0x00000800u
#define START_ID11_SHIFT                        11u

#define START_ID12_OFFSET                       0x04u
#define START_ID12_MASK                         0x00001000u
#define START_ID12_SHIFT                        12u

#define START_ID13_OFFSET                       0x04u
#define START_ID13_MASK                         0x00002000u
#define START_ID13_SHIFT                        13u

#define START_ID14_OFFSET                       0x04u
#define START_ID14_MASK                         0x00004000u
#define START_ID14_SHIFT                        14u

#define START_ID15_OFFSET                       0x04u
#define START_ID15_MASK                         0x00008000u
#define START_ID15_SHIFT                        15u

#define START_ID16_OFFSET                       0x04u
#define START_ID16_MASK                         0x00010000u
#define START_ID16_SHIFT                        16u

#define START_ID17_OFFSET                       0x04u
#define START_ID17_MASK                         0x00020000u
#define START_ID17_SHIFT                        17u

#define START_ID18_OFFSET                       0x04u
#define START_ID18_MASK                         0x00040000u
#define START_ID18_SHIFT                        18u

#define START_ID19_OFFSET                       0x04u
#define START_ID19_MASK                         0x00080000u
#define START_ID19_SHIFT                        19u

#define START_ID20_OFFSET                       0x04u
#define START_ID20_MASK                         0x00100000u
#define START_ID20_SHIFT                        20u

#define START_ID21_OFFSET                       0x04u
#define START_ID21_MASK                         0x00200000u
#define START_ID21_SHIFT                        21u

#define START_ID22_OFFSET                       0x04u
#define START_ID22_MASK                         0x00400000u
#define START_ID22_SHIFT                        22u

#define START_ID23_OFFSET                       0x04u
#define START_ID23_MASK                         0x00800000u
#define START_ID23_SHIFT                        23u

#define START_ID24_OFFSET                       0x04u
#define START_ID24_MASK                         0x01000000u
#define START_ID24_SHIFT                        24u

#define START_ID25_OFFSET                       0x04u
#define START_ID25_MASK                         0x02000000u
#define START_ID25_SHIFT                        25u

#define START_ID26_OFFSET                       0x04u
#define START_ID26_MASK                         0x04000000u
#define START_ID26_SHIFT                        26u

#define START_ID27_OFFSET                       0x04u
#define START_ID27_MASK                         0x08000000u
#define START_ID27_SHIFT                        27u

#define START_ID28_OFFSET                       0x04u
#define START_ID28_MASK                         0x10000000u
#define START_ID28_SHIFT                        28u

#define START_ID29_OFFSET                       0x04u
#define START_ID29_MASK                         0x20000000u
#define START_ID29_SHIFT                        29u

#define START_ID30_OFFSET                       0x04u
#define START_ID30_MASK                         0x40000000u
#define START_ID30_SHIFT                        30u

#define START_ID31_OFFSET                       0x04u
#define START_ID31_MASK                         0x40000000u
#define START_ID31_SHIFT                        31u

/*------------------------------------------------------------------------------
 * Interrupt 0 Status Register
 */
#define I0ST_REG_OFFSET                         0x10u

#define I0ST_CPLT_OFFSET                        0x10u
#define I0ST_CPLT_MASK                          0x00000001u
#define I0ST_CPLT_SHIF                          0u

#define I0ST_DWERR_OFFSET                       0x10u
#define I0ST_DWERR_MASK                         0x00000002u
#define I0ST_DWERR_SHIFT                        1u

#define I0ST_DRERR_OFFSET                       0x10u
#define I0ST_DRERR_MASK                         0x00000004u
#define I0ST_DRERR_SHIFT                        2u

#define I0ST_INVDESC_OFFSET                     0x10u
#define I0ST_INVDESC_MASK                       0x00000008u
#define I0ST_INVDESC_SHIFT                      3u

#define I0ST_DESCNO_OFFSET                      0x10u
#define I0ST_DESCNO_MASK                        0x000003F0u
#define I0ST_DESCNO_SHIFT                       4u

/*------------------------------------------------------------------------------
 * Interrupt 0 mask Register
 */
#define I0MASK_REG_OFFSET                       0x14u

#define I0MASK_CPLT_OFFSET                      0x14u
#define I0MASK_CPLT_MASK                        0x00000001u
#define I0MASK_CPLT_SHIFT                       0u

#define I0MASK_DMAW_OFFSET                      0x14u
#define I0MASK_DMAW_MASK                        0x00000002u
#define I0MASK_DMAW_SHIFT                       1u

#define I0MASK_DMAR_OFFSET                      0x14u
#define I0MASK_DMAR_MASK                        0x00000004u
#define I0MASK_DMAR_SHIFT                       2u

#define I0MASK_INVDESC_OFFSET                   0x14u
#define I0MASK_INVDESC_MASK                     0x00000008u
#define I0MASK_INVDESC_SHIFT                    3u

/*------------------------------------------------------------------------------
 * Interrupt 0 clear Register
 */
#define I0CLR_REG_OFFSET                        0x18u

#define I0CLR_CPLT_OFFSET                       0x18u
#define I0CLR_CPLT_MASK                         0x00000001u
#define I0CLR_CPLT_SHIFT                        0u

#define I0CLR_DMAW_OFFSET                       0x18u
#define I0CLR_DMAW_MASK                         0x00000002u
#define I0CLR_DMAW_SHIFT                        1u

#define I0CLR_DMAR_OFFSET                       0x18u
#define I0CLR_DMAR_MASK                         0x00000004u
#define I0CLR_DMAR_SHIFT                        2u

#define I0CLR_INVDESC_OFFSET                    0x18u
#define I0CLR_INVDESC_MASK                      0x00000008u
#define I0CLR_INVDESC_SHIFT                     3u

/*------------------------------------------------------------------------------
 * Interrupt 0 External Address Register
 */
#define I0EXADRR_REG_OFFSET                     0x1cu

#define I0EXADRR_OFFSET                         0x1cu
#define I0EXADRR_MASK                           0xFFFFFFFFu
#define I0EXADRR_SHIFT                          0u

/*------------------------------------------------------------------------------
 * Internal Descriptor 0 register details
 */
//ID0CFG
#define ID0CFG_REG_OFFSET                       0x60u

#define ID0CFG_SRC_OP_OFFSET                    0x60u
#define ID0CFG_SRC_OP_MASK                      0x00000003u
#define ID0CFG_SRC_OP_SHIFT                     0u

#define ID0CFG_DEST_OP_OFFSET                   0x60u
#define ID0CFG_DEST_OP_MASK                     0x0000000Cu
#define ID0CFG_DEST_OP_SHIFT                    2u

#define ID0CFG_CHAIN_OFFSET                     0x60u
#define ID0CFG_CHAIN_MASK                       0x00000400u
#define ID0CFG_CHAIN_SHIFT                      10u

#define ID0CFG_EXDESC_OFFSET                    0x60u
#define ID0CFG_EXDESC_MASK                      0x00000800u
#define ID0CFG_EXDESC_SHIFT                     11u

#define ID0CFG_INTR_OFFSET                      0x60u
#define ID0CFG_INTR_MASK                        0x00001000u
#define ID0CFG_INTR_SHIFT                       12u

#define ID0CFG_SRCDVALID_OFFSET                 0x60u
#define ID0CFG_SRCDVALID_MASK                   0x00002000u
#define ID0CFG_SRCDVALID_SHIFT                  13u

#define ID0CFG_DESTDRDY_OFFSET                  0x60u
#define ID0CFG_DESTDRDY_MASK                    0x00004000u
#define ID0CFG_DESTDRDY_SHIFT                   14u

#define ID0CFG_DESCVALID_OFFSET                 0x60u
#define ID0CFG_DESCVALID_MASK                   0x00008000u
#define ID0CFG_DESCVALID_SHIFT                  15u

#define ID0CFG_SDDVALID_OFFSET                  0x60u
#define ID0CFG_SDDVALID_MASK                    0x0000E000u
#define ID0CFG_SDDVALID_SHIFT                   13u

#define ID0CFG_FLCTRL_OFFSET                    0x60u
#define ID0CFG_FLCTRL_MASK                      0x0000E000u
#define ID0CFG_FLCTRL_SHIFT                     13u

//ID0BYTECNT
#define ID0BYTECNT_REG_OFFSET                   0x64u

#define ID0BYTECNT_NUM_OFFSET                   0x64u
#define ID0BYTECNT_NUM_MASK                     0x007FFFFFu
#define ID0BYTECNT_NUM_SHIFT                    0u

//ID0SRCADD
#define ID0SRCADDR_REG_OFFSET                   0x68u

#define ID0SRCADDR_ADDR_OFFSET                  0x68u
#define ID0SRCADDR_ADDR_MASK                    0xFFFFFFFFu
#define ID0SRCADDR_ADDR_SHIFT                   0u

//ID0DESTADD
#define ID0DESTADDR_REG_OFFSET                  0x6Cu

#define ID0DESTADDR_ADDR_OFFSET                 0x6Cu
#define ID0DESTADDR_ADDR_MASK                   0xFFFFFFFFu
#define ID0DESTADDR_ADDR_SHIFT                  0u

//ID0NEXTDESC
#define ID0NEXTDESC_REG_OFFSET                  0x70u

#define ID0NEXTDESC_ADDR_OFFSET                 0x70u
#define ID0NEXTDESC_ADDR_MASK                   0xFFFFFFFFu
#define ID0NEXTDESC_ADDR_SHIFT                  0u

/*------------------------------------------------------------------------------
 * Internal Descriptor 1 register details
 */
#define ID1CFG_REG_OFFSET                       0x80u

#define ID1CFG_SRC_OP_OFFSET                    0x80u
#define ID1CFG_SRC_OP_MASK                      0x00000003u
#define ID1CFG_SRC_OP_SHIFT                     0u

#define ID1CFG_DEST_OP_OFFSET                   0x80u
#define ID1CFG_DEST_OP_MASK                     0x0000000Cu
#define ID1CFG_DEST_OP_SHIFT                    2u

#define ID1CFG_CHAIN_OFFSET                     0x80u
#define ID1CFG_CHAIN_MASK                       0x00000400u
#define ID1CFG_CHAIN_SHIFT                      10u

#define ID1CFG_EXDESC_OFFSET                    0x80u
#define ID1CFG_EXDESC_MASK                      0x00000800u
#define ID1CFG_EXDESC_SHIFT                     11u

#define ID1CFG_INTR_OFFSET                      0x80u
#define ID1CFG_INTR_MASK                        0x00001000u
#define ID1CFG_INTR_SHIFT                       12u

#define ID1CFG_DRDY_OFFSET                      0x80u
#define ID1CFG_DRDY_MASK                        0x00002000u
#define ID1CFG_DRDY_SHIFT                       13u

#define ID1CFG_DESCVAL_OFFSET                   0x80u
#define ID1CFG_DESCVAL_MASK                     0x00004000u
#define ID1CFG_DESCVAL_SHIFT                    14u

#define ID1CFG_DESCVAL_OFFSET                   0x80u
#define ID1CFG_DESCVAL_MASK                     0x00004000u
#define ID1CFG_DESCVAL_SHIFT                    14u

//ID1BYTECNT
#define ID1BYTECNT_REG_OFFSET                   0x84u
#define ID1BYTECNT_CNT_OFFSET                   0x84u
#define ID1BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID1BYTECNT_CNT_SHIFT                    0u

//ID1SRCADD
#define ID1SRCADDR_REG_OFFSET                   0x88u
#define ID1SRCADDR_ADDR_OFFSET                  0x88u
#define ID1SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID1SRCADDR_ADDR_SHIFT                   0u

//ID1DESTADD
#define ID1DESTADDR_REG_OFFSET                  0x8Cu
#define ID1DESTADDR_ADDR_OFFSET                 0x8Cu
#define ID1DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID1DESTADDR_ADDR_SHIFT                  0u

//ID1NEXTDESC
#define ID1NEXTDESC_REG_OFFSET                  0x90u
#define ID1NEXTDESC_ADDR_OFFSET                 0x90u
#define ID1NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID1NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 2 register details
 */
#define ID2CFG_REG_OFFSET                       0xA0u

#define ID2CFG_SRC_OP_OFFSET                    0xA0u
#define ID2CFG_SRC_OP_MASK                      0x00000003u
#define ID2CFG_SRC_OP_SHIFT                     0u

#define ID2CFG_DEST_OP_OFFSET                   0xA0u
#define ID2CFG_DEST_OP_MASK                     0x0000000Cu
#define ID2CFG_DEST_OP_SHIFT                    2u

#define ID2CFG_CHAIN_OFFSET                     0xA0u
#define ID2CFG_CHAIN_MASK                       0x00000400u
#define ID2CFG_CHAIN_SHIFT                      10u

#define ID2CFG_EXDESC_OFFSET                    0xA0u
#define ID2CFG_EXDESC_MASK                      0x00000800u
#define ID2CFG_EXDESC_SHIFT                     11u

#define ID2CFG_INTR_OFFSET                      0xA0u
#define ID2CFG_INTR_MASK                        0x00001000u
#define ID2CFG_INTR_SHIFT                       12u

#define ID2CFG_DRDY_OFFSET                      0xA0u
#define ID2CFG_DRDY_MASK                        0x00002000u
#define ID2CFG_DRDY_SHIFT                       13u

#define ID2CFG_DESCVAL_OFFSET                   0xA0u
#define ID2CFG_DESCVAL_MASK                     0x00004000u
#define ID2CFG_DESCVAL_SHIFT                    14u

#define ID2CFG_DESCVAL_OFFSET                   0xA0u
#define ID2CFG_DESCVAL_MASK                     0x00004000u
#define ID2CFG_DESCVAL_SHIFT                    14u

//ID2BYTECNT
#define ID2BYTECNT_REG_OFFSET                   0xA4u
#define ID2BYTECNT_CNT_OFFSET                   0xA4u
#define ID2BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID2BYTECNT_CNT_SHIFT                    0u

//ID2SRCADD
#define ID2SRCADDR_REG_OFFSET                   0xA8u
#define ID2SRCADDR_ADDR_OFFSET                  0xA8u
#define ID2SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID2SRCADDR_ADDR_SHIFT                   0u

//ID2DESTADD
#define ID2DESTADDR_REG_OFFSET                  0xACu
#define ID2DESTADDR_ADDR_OFFSET                 0xACu
#define ID2DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID2DESTADDR_ADDR_SHIFT                  0u

//ID2NEXTDESC
#define ID2NEXTDESC_REG_OFFSET                  0xB0u
#define ID2NEXTDESC_ADDR_OFFSET                 0xB0u
#define ID2NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID2NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 3 register details
 */
#define ID3CFG_REG_OFFSET                       0xC0u

#define ID3CFG_SRC_OP_OFFSET                    0xC0u
#define ID3CFG_SRC_OP_MASK                      0x00000003u
#define ID3CFG_SRC_OP_SHIFT                     0u

#define ID3CFG_DEST_OP_OFFSET                   0xC0u
#define ID3CFG_DEST_OP_MASK                     0x0000000Cu
#define ID3CFG_DEST_OP_SHIFT                    2u

#define ID3CFG_CHAIN_OFFSET                     0xc0u
#define ID3CFG_CHAIN_MASK                       0x00000400u
#define ID3CFG_CHAIN_SHIFT                      10u

#define ID3CFG_EXDESC_OFFSET                    0xC0u
#define ID3CFG_EXDESC_MASK                      0x00000800u
#define ID3CFG_EXDESC_SHIFT                     11u

#define ID3CFG_INTR_OFFSET                      0xC0u
#define ID3CFG_INTR_MASK                        0x00001000u
#define ID3CFG_INTR_SHIFT                       12u

#define ID3CFG_DRDY_OFFSET                      0xC0u
#define ID3CFG_DRDY_MASK                        0x00002000u
#define ID3CFG_DRDY_SHIFT                       13u

#define ID3CFG_DESCVAL_OFFSET                   0xC0u
#define ID3CFG_DESCVAL_MASK                     0x00004000u
#define ID3CFG_DESCVAL_SHIFT                    14u

#define ID3CFG_DESCVAL_OFFSET                   0xC0u
#define ID3CFG_DESCVAL_MASK                     0x00004000u
#define ID3CFG_DESCVAL_SHIFT                    14u

//ID3BYTECNT
#define ID3BYTECNT_REG_OFFSET                   0xC4u
#define ID3BYTECNT_CNT_OFFSET                   0xC4u
#define ID3BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID3BYTECNT_CNT_SHIFT                    0u

//ID3SRCADD
#define ID3SRCADDR_REG_OFFSET                   0xC8u
#define ID3SRCADDR_ADDR_OFFSET                  0xC8u
#define ID3SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID3SRCADDR_ADDR_SHIFT                   0u

//ID3DESTADD
#define ID3DESTADDR_REG_OFFSET                  0xCCu
#define ID3DESTADDR_ADDR_OFFSET                 0xCCu
#define ID3DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID3DESTADDR_ADDR_SHIFT                  0u

//ID3NEXTDESC
#define ID3NEXTDESC_REG_OFFSET                  0xD0u
#define ID3NEXTDESC_ADDR_OFFSET                 0xD0u
#define ID3NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID3NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 4 register details
 */
#define ID4CFG_REG_OFFSET                       0xE0u

#define ID4CFG_SRC_OP_OFFSET                    0xE0u
#define ID4CFG_SRC_OP_MASK                      0x00000003u
#define ID4CFG_SRC_OP_SHIFT                     0u

#define ID4CFG_DEST_OP_OFFSET                   0xE0u
#define ID4CFG_DEST_OP_MASK                     0x0000000Cu
#define ID4CFG_DEST_OP_SHIFT                    2u

#define ID4CFG_CHAIN_OFFSET                     0xE0u
#define ID4CFG_CHAIN_MASK                       0x00000400u
#define ID4CFG_CHAIN_SHIFT                      10u

#define ID4CFG_EXDESC_OFFSET                    0xE0u
#define ID4CFG_EXDESC_MASK                      0x00000800u
#define ID4CFG_EXDESC_SHIFT                     11u

#define ID4CFG_INTR_OFFSET                      0xE0u
#define ID4CFG_INTR_MASK                        0x00001000u
#define ID4CFG_INTR_SHIFT                       12u

#define ID4CFG_DRDY_OFFSET                      0xE0u
#define ID4CFG_DRDY_MASK                        0x00002000u
#define ID4CFG_DRDY_SHIFT                       13u

#define ID4CFG_DESCVAL_OFFSET                   0xE0u
#define ID4CFG_DESCVAL_MASK                     0x00004000u
#define ID4CFG_DESCVAL_SHIFT                    14u

#define ID4CFG_DESCVAL_OFFSET                   0xE0u
#define ID4CFG_DESCVAL_MASK                     0x00004000u
#define ID4CFG_DESCVAL_SHIFT                    14u

//ID4BYTECNT
#define ID4BYTECNT_REG_OFFSET                   0xE4u
#define ID4BYTECNT_CNT_OFFSET                   0xE4u
#define ID4BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID4BYTECNT_CNT_SHIFT                    0u

//ID4SRCADD
#define ID4SRCADDR_REG_OFFSET                   0xE8u
#define ID4SRCADDR_ADDR_OFFSET                  0xE8u
#define ID4SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID4SRCADDR_ADDR_SHIFT                   0u

//ID4DESTADD
#define ID4DESTADDR_REG_OFFSET                  0xECu
#define ID4DESTADDR_ADDR_OFFSET                 0xECu
#define ID4DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID4DESTADDR_ADDR_SHIFT                  0u

//ID4NEXTDESC
#define ID4NEXTDESC_REG_OFFSET                  0xF0u
#define ID4NEXTDESC_ADDR_OFFSET                 0xF0u
#define ID4NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID4NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 5 register details
 */
#define ID5CFG_REG_OFFSET                       0x100u

#define ID5CFG_SRC_OP_OFFSET                    0x100u
#define ID5CFG_SRC_OP_MASK                      0x00000003u
#define ID5CFG_SRC_OP_SHIFT                     0u

#define ID5CFG_DEST_OP_OFFSET                   0x100u
#define ID5CFG_DEST_OP_MASK                     0x0000000Cu
#define ID5CFG_DEST_OP_SHIFT                    2u

#define ID5CFG_CHAIN_OFFSET                     0x100u
#define ID5CFG_CHAIN_MASK                       0x00000400u
#define ID5CFG_CHAIN_SHIFT                      10u

#define ID5CFG_EXDESC_OFFSET                    0x100u
#define ID5CFG_EXDESC_MASK                      0x00000800u
#define ID5CFG_EXDESC_SHIFT                     11u

#define ID5CFG_INTR_OFFSET                      0x100u
#define ID5CFG_INTR_MASK                        0x00001000u
#define ID5CFG_INTR_SHIFT                       12u

#define ID5CFG_DRDY_OFFSET                      0x100u
#define ID5CFG_DRDY_MASK                        0x00002000u
#define ID5CFG_DRDY_SHIFT                       13u

#define ID5CFG_DESCVAL_OFFSET                   0x100u
#define ID5CFG_DESCVAL_MASK                     0x00004000u
#define ID5CFG_DESCVAL_SHIFT                    14u

#define ID5CFG_DESCVAL_OFFSET                   0x100u
#define ID5CFG_DESCVAL_MASK                     0x00004000u
#define ID5CFG_DESCVAL_SHIFT                    14u

//ID5BYTECNT
#define ID5BYTECNT_REG_OFFSET                   0x104u
#define ID5BYTECNT_CNT_OFFSET                   0x104u
#define ID5BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID5BYTECNT_CNT_SHIFT                    0u

//ID5SRCADD
#define ID5SRCADDR_REG_OFFSET                   0x108u
#define ID5SRCADDR_ADDR_OFFSET                  0x108u
#define ID5SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID5SRCADDR_ADDR_SHIFT                   0u

//ID5DESTADD
#define ID5DESTADDR_REG_OFFSET                  0x10Cu
#define ID5DESTADDR_ADDR_OFFSET                 0x10Cu
#define ID5DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID5DESTADDR_ADDR_SHIFT                  0u

//ID5NEXTDESC
#define ID5NEXTDESC_REG_OFFSET                  0x110u
#define ID5NEXTDESC_ADDR_OFFSET                 0x110u
#define ID5NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID5NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 6 register details
 */
#define ID6CFG_REG_OFFSET                       0x120u

#define ID6CFG_SRC_OP_OFFSET                    0x120u
#define ID6CFG_SRC_OP_MASK                      0x00000003u
#define ID6CFG_SRC_OP_SHIFT                     0u

#define ID6CFG_DEST_OP_OFFSET                   0x120u
#define ID6CFG_DEST_OP_MASK                     0x0000000Cu
#define ID6CFG_DEST_OP_SHIFT                    2u

#define ID6CFG_CHAIN_OFFSET                     0x120u
#define ID6CFG_CHAIN_MASK                       0x00000400u
#define ID6CFG_CHAIN_SHIFT                      10u

#define ID6CFG_EXDESC_OFFSET                    0x120u
#define ID6CFG_EXDESC_MASK                      0x00000800u
#define ID6CFG_EXDESC_SHIFT                     11u

#define ID6CFG_INTR_OFFSET                      0x120u
#define ID6CFG_INTR_MASK                        0x00001000u
#define ID6CFG_INTR_SHIFT                       12u

#define ID6CFG_DRDY_OFFSET                      0x120u
#define ID6CFG_DRDY_MASK                        0x00002000u
#define ID6CFG_DRDY_SHIFT                       13u

#define ID6CFG_DESCVAL_OFFSET                   0x120u
#define ID6CFG_DESCVAL_MASK                     0x00004000u
#define ID6CFG_DESCVAL_SHIFT                    14u

#define ID6CFG_DESCVAL_OFFSET                   0x120u
#define ID6CFG_DESCVAL_MASK                     0x00004000u
#define ID6CFG_DESCVAL_SHIFT                    14u

//ID6BYTECNT
#define ID6BYTECNT_REG_OFFSET                   0x124u
#define ID6BYTECNT_CNT_OFFSET                   0x124u
#define ID6BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID6BYTECNT_CNT_SHIFT                    0u

//ID6SRCADD
#define ID6SRCADDR_REG_OFFSET                   0x128u
#define ID6SRCADDR_ADDR_OFFSET                  0x128u
#define ID6SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID6SRCADDR_ADDR_SHIFT                   0u

//ID6DESTADD
#define ID6DESTADDR_REG_OFFSET                  0x12Cu
#define ID6DESTADDR_ADDR_OFFSET                 0x12Cu
#define ID6DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID6DESTADDR_ADDR_SHIFT                  0u

//ID6NEXTDESC
#define ID6NEXTDESC_REG_OFFSET                  0x130u
#define ID6NEXTDESC_ADDR_OFFSET                 0x130u
#define ID6NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID6NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 7 register details
 */
#define ID7CFG_REG_OFFSET                       0x140u

#define ID7CFG_SRC_OP_OFFSET                    0x140u
#define ID7CFG_SRC_OP_MASK                      0x00000003u
#define ID7CFG_SRC_OP_SHIFT                     0u

#define ID7CFG_DEST_OP_OFFSET                   0x140u
#define ID7CFG_DEST_OP_MASK                     0x0000000Cu
#define ID7CFG_DEST_OP_SHIFT                    2u

#define ID7CFG_CHAIN_OFFSET                     0x140u
#define ID7CFG_CHAIN_MASK                       0x00000400u
#define ID7CFG_CHAIN_SHIFT                      10u

#define ID7CFG_EXDESC_OFFSET                    0x140u
#define ID7CFG_EXDESC_MASK                      0x00000800u
#define ID7CFG_EXDESC_SHIFT                     11u

#define ID7CFG_INTR_OFFSET                      0x140u
#define ID7CFG_INTR_MASK                        0x00001000u
#define ID7CFG_INTR_SHIFT                       12u

#define ID7CFG_DRDY_OFFSET                      0x140u
#define ID7CFG_DRDY_MASK                        0x00002000u
#define ID7CFG_DRDY_SHIFT                       13u

#define ID7CFG_DESCVAL_OFFSET                   0x140u
#define ID7CFG_DESCVAL_MASK                     0x00004000u
#define ID7CFG_DESCVAL_SHIFT                    14u

#define ID7CFG_DESCVAL_OFFSET                   0x140u
#define ID7CFG_DESCVAL_MASK                     0x00004000u
#define ID7CFG_DESCVAL_SHIFT                    14u

//ID7BYTECNT
#define ID7BYTECNT_REG_OFFSET                   0x144u
#define ID7BYTECNT_CNT_OFFSET                   0x144u
#define ID7BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID7BYTECNT_CNT_SHIFT                    0u

//ID7SRCADD
#define ID7SRCADDR_REG_OFFSET                   0x148u
#define ID7SRCADDR_ADDR_OFFSET                  0x148u
#define ID7SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID7SRCADDR_ADDR_SHIFT                   0u

//ID7DESTADD
#define ID7DESTADDR_REG_OFFSET                  0x14Cu
#define ID7DESTADDR_ADDR_OFFSET                 0x14Cu
#define ID7DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID7DESTADDR_ADDR_SHIFT                  0u

//ID7NEXTDESC
#define ID7NEXTDESC_REG_OFFSET                  0x150u
#define ID7NEXTDESC_ADDR_OFFSET                 0x150u
#define ID7NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID7NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 8 register details
 */
#define ID8CFG_REG_OFFSET                       0x160u

#define ID8CFG_SRC_OP_OFFSET                    0x160u
#define ID8CFG_SRC_OP_MASK                      0x00000003u
#define ID8CFG_SRC_OP_SHIFT                     0u

#define ID8CFG_DEST_OP_OFFSET                   0x160u
#define ID8CFG_DEST_OP_MASK                     0x0000000Cu
#define ID8CFG_DEST_OP_SHIFT                    2u

#define ID8CFG_CHAIN_OFFSET                     0x160u
#define ID8CFG_CHAIN_MASK                       0x00000400u
#define ID8CFG_CHAIN_SHIFT                      10u

#define ID8CFG_EXDESC_OFFSET                    0x160u
#define ID8CFG_EXDESC_MASK                      0x00000800u
#define ID8CFG_EXDESC_SHIFT                     11u

#define ID8CFG_INTR_OFFSET                      0x160u
#define ID8CFG_INTR_MASK                        0x00001000u
#define ID8CFG_INTR_SHIFT                       12u

#define ID8CFG_DRDY_OFFSET                      0x160u
#define ID8CFG_DRDY_MASK                        0x00002000u
#define ID8CFG_DRDY_SHIFT                       13u

#define ID8CFG_DESCVAL_OFFSET                   0x160u
#define ID8CFG_DESCVAL_MASK                     0x00004000u
#define ID8CFG_DESCVAL_SHIFT                    14u

#define ID8CFG_DESCVAL_OFFSET                   0x160u
#define ID8CFG_DESCVAL_MASK                     0x00004000u
#define ID8CFG_DESCVAL_SHIFT                    14u

//ID8BYTECNT
#define ID8BYTECNT_REG_OFFSET                   0x164u
#define ID8BYTECNT_CNT_OFFSET                   0x164u
#define ID8BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID8BYTECNT_CNT_SHIFT                    0u

//ID8SRCADD
#define ID8SRCADDR_REG_OFFSET                   0x168u
#define ID8SRCADDR_ADDR_OFFSET                  0x168u
#define ID8SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID8SRCADDR_ADDR_SHIFT                   0u

//ID8DESTADD
#define ID8DESTADDR_REG_OFFSET                  0x16Cu
#define ID8DESTADDR_ADDR_OFFSET                 0x16Cu
#define ID8DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID8DESTADDR_ADDR_SHIFT                  0u

//ID8NEXTDESC
#define ID8NEXTDESC_REG_OFFSET                  0x170u
#define ID8NEXTDESC_ADDR_OFFSET                 0x170u
#define ID8NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID8NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 9 register details
 */
#define ID9CFG_REG_OFFSET                       0x180u

#define ID9CFG_SRC_OP_OFFSET                    0x180u
#define ID9CFG_SRC_OP_MASK                      0x00000003u
#define ID9CFG_SRC_OP_SHIFT                     0u

#define ID9CFG_DEST_OP_OFFSET                   0x180u
#define ID9CFG_DEST_OP_MASK                     0x0000000Cu
#define ID9CFG_DEST_OP_SHIFT                    2u

#define ID9CFG_CHAIN_OFFSET                     0x180u
#define ID9CFG_CHAIN_MASK                       0x00000400u
#define ID9CFG_CHAIN_SHIFT                      10u

#define ID9CFG_EXDESC_OFFSET                    0x180u
#define ID9CFG_EXDESC_MASK                      0x00000800u
#define ID9CFG_EXDESC_SHIFT                     11u

#define ID9CFG_INTR_OFFSET                      0x180u
#define ID9CFG_INTR_MASK                        0x00001000u
#define ID9CFG_INTR_SHIFT                       12u

#define ID9CFG_DRDY_OFFSET                      0x180u
#define ID9CFG_DRDY_MASK                        0x00002000u
#define ID9CFG_DRDY_SHIFT                       13u

#define ID9CFG_DESCVAL_OFFSET                   0x180u
#define ID9CFG_DESCVAL_MASK                     0x00004000u
#define ID9CFG_DESCVAL_SHIFT                    14u

#define ID9CFG_DESCVAL_OFFSET                   0x180u
#define ID9CFG_DESCVAL_MASK                     0x00004000u
#define ID9CFG_DESCVAL_SHIFT                    14u

//ID9BYTECNT
#define ID9BYTECNT_REG_OFFSET                   0x184u
#define ID9BYTECNT_CNT_OFFSET                   0x184u
#define ID9BYTECNT_CNT_MASK                     0x007FFFFFu
#define ID9BYTECNT_CNT_SHIFT                    0u

//ID9SRCADD
#define ID9SRCADDR_REG_OFFSET                   0x188u
#define ID9SRCADDR_ADDR_OFFSET                  0x188u
#define ID9SRCADDR_ADDR_MASK                    0x007FFFFFu
#define ID9SRCADDR_ADDR_SHIFT                   0u

//ID9DESTADD
#define ID9DESTADDR_REG_OFFSET                  0x18Cu
#define ID9DESTADDR_ADDR_OFFSET                 0x18Cu
#define ID9DESTADDR_ADDR_MASK                   0x007FFFFFu
#define ID9DESTADDR_ADDR_SHIFT                  0u

//ID9NEXTDESC
#define ID9NEXTDESC_REG_OFFSET                  0x190u
#define ID9NEXTDESC_ADDR_OFFSET                 0x190u
#define ID9NEXTDESC_ADDR_MASK                   0x007FFFFFu
#define ID9NEXTDESC_ADDR_SHIFT                  0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 10 register details
 */
#define ID10CFG_REG_OFFSET                      0x1A0u

#define ID10CFG_SRC_OP_OFFSET                   0x1A0u
#define ID10CFG_SRC_OP_MASK                     0x00000003u
#define ID10CFG_SRC_OP_SHIFT                    0u

#define ID10CFG_DEST_OP_OFFSET                  0x1A0u
#define ID10CFG_DEST_OP_MASK                    0x0000000Cu
#define ID10CFG_DEST_OP_SHIFT                   2u

#define ID10CFG_CHAIN_OFFSET                    0x1A0u
#define ID10CFG_CHAIN_MASK                      0x00000400u
#define ID10CFG_CHAIN_SHIFT                     10u

#define ID10CFG_EXDESC_OFFSET                   0x1A0u
#define ID10CFG_EXDESC_MASK                     0x00000800u
#define ID10CFG_EXDESC_SHIFT                    11u

#define ID10CFG_INTR_OFFSET                     0x1A0u
#define ID10CFG_INTR_MASK                       0x00001000u
#define ID10CFG_INTR_SHIFT                      12u

#define ID10CFG_DRDY_OFFSET                     0x1A0u
#define ID10CFG_DRDY_MASK                       0x00002000u
#define ID10CFG_DRDY_SHIFT                      13u

#define ID10CFG_DESCVAL_OFFSET                  0x1A0u
#define ID10CFG_DESCVAL_MASK                    0x00004000u
#define ID10CFG_DESCVAL_SHIFT                   14u

#define ID10CFG_DESCVAL_OFFSET                  0x1A0u
#define ID10CFG_DESCVAL_MASK                    0x00004000u
#define ID10CFG_DESCVAL_SHIFT                   14u

//ID10BYTECNT
#define ID10BYTECNT_REG_OFFSET                  0x1A4u
#define ID10BYTECNT_CNT_OFFSET                  0x1A4u
#define ID10BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID10BYTECNT_CNT_SHIFT                   0u

//ID10SRCADD
#define ID10SRCADDR_REG_OFFSET                  0x1A8u
#define ID10SRCADDR_ADDR_OFFSET                 0x1A8u
#define ID10SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID10SRCADDR_ADDR_SHIFT                  0u

//ID10DESTADD
#define ID10DESTADDR_REG_OFFSET                 0x1ACu
#define ID10DESTADDR_ADDR_OFFSET                0x1ACu
#define ID10DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID10DESTADDR_ADDR_SHIFT                 0u

//ID10NEXTDESC
#define ID10NEXTDESC_REG_OFFSET                 0x1B0u
#define ID10NEXTDESC_ADDR_OFFSET                0x1B0u
#define ID10NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID10NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 11 register details
 */
#define ID11CFG_REG_OFFSET                      0x1C0u

#define ID11CFG_SRC_OP_OFFSET                   0x1C0u
#define ID11CFG_SRC_OP_MASK                     0x00000003u
#define ID11CFG_SRC_OP_SHIFT                    0u

#define ID11CFG_DEST_OP_OFFSET                  0x1C0u
#define ID11CFG_DEST_OP_MASK                    0x0000000Cu
#define ID11CFG_DEST_OP_SHIFT                   2u

#define ID11CFG_CHAIN_OFFSET                    0x1C0u
#define ID11CFG_CHAIN_MASK                      0x00000400u
#define ID11CFG_CHAIN_SHIFT                     10u

#define ID11CFG_EXDESC_OFFSET                   0x1C0u
#define ID11CFG_EXDESC_MASK                     0x00000800u
#define ID11CFG_EXDESC_SHIFT                    11u

#define ID11CFG_INTR_OFFSET                     0x1C0u
#define ID11CFG_INTR_MASK                       0x00001000u
#define ID11CFG_INTR_SHIFT                      12u

#define ID11CFG_DRDY_OFFSET                     0x1C0u
#define ID11CFG_DRDY_MASK                       0x00002000u
#define ID11CFG_DRDY_SHIFT                      13u

#define ID11CFG_DESCVAL_OFFSET                  0x1C0u
#define ID11CFG_DESCVAL_MASK                    0x00004000u
#define ID11CFG_DESCVAL_SHIFT                   14u

#define ID11CFG_DESCVAL_OFFSET                  0x1C0u
#define ID11CFG_DESCVAL_MASK                    0x00004000u
#define ID11CFG_DESCVAL_SHIFT                   14u

//ID11BYTECNT
#define ID11BYTECNT_REG_OFFSET                  0x1C4u
#define ID11BYTECNT_CNT_OFFSET                  0x1C4u
#define ID11BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID11BYTECNT_CNT_SHIFT                   0u

//ID11SRCADD
#define ID11SRCADDR_REG_OFFSET                  0x1C8u
#define ID11SRCADDR_ADDR_OFFSET                 0x1C8u
#define ID11SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID11SRCADDR_ADDR_SHIFT                  0u

//ID11DESTADD
#define ID11DESTADDR_REG_OFFSET                 0x1CCu
#define ID11DESTADDR_ADDR_OFFSET                0x1CCu
#define ID11DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID11DESTADDR_ADDR_SHIFT                 0u

//ID11NEXTDESC
#define ID11NEXTDESC_REG_OFFSET                 0x1D0u
#define ID11NEXTDESC_ADDR_OFFSET                0x1D0u
#define ID11NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID11NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 12 register details
 */
#define ID12CFG_REG_OFFSET                      0x1E0u

#define ID12CFG_SRC_OP_OFFSET                   0x1E0u
#define ID12CFG_SRC_OP_MASK                     0x00000003u
#define ID12CFG_SRC_OP_SHIFT                    0u

#define ID12CFG_DEST_OP_OFFSET                  0x1E0u
#define ID12CFG_DEST_OP_MASK                    0x0000000Cu
#define ID12CFG_DEST_OP_SHIFT                   2u

#define ID12CFG_CHAIN_OFFSET                    0x1E0u
#define ID12CFG_CHAIN_MASK                      0x00000400u
#define ID12CFG_CHAIN_SHIFT                     10u

#define ID12CFG_EXDESC_OFFSET                   0x1E0u
#define ID12CFG_EXDESC_MASK                     0x00000800u
#define ID12CFG_EXDESC_SHIFT                    11u

#define ID12CFG_INTR_OFFSET                     0x1E0u
#define ID12CFG_INTR_MASK                       0x00001000u
#define ID12CFG_INTR_SHIFT                      12u

#define ID12CFG_DRDY_OFFSET                     0x1E0u
#define ID12CFG_DRDY_MASK                       0x00002000u
#define ID12CFG_DRDY_SHIFT                      13u

#define ID12CFG_DESCVAL_OFFSET                  0x1E0u
#define ID12CFG_DESCVAL_MASK                    0x00004000u
#define ID12CFG_DESCVAL_SHIFT                   14u

#define ID12CFG_DESCVAL_OFFSET                  0x1E0u
#define ID12CFG_DESCVAL_MASK                    0x00004000u
#define ID12CFG_DESCVAL_SHIFT                   14u

//ID12BYTECNT
#define ID12BYTECNT_REG_OFFSET                  0x1E4u
#define ID12BYTECNT_CNT_OFFSET                  0x1E4u
#define ID12BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID12BYTECNT_CNT_SHIFT                   0u

//ID12SRCADD
#define ID12SRCADDR_REG_OFFSET                  0x1E8u
#define ID12SRCADDR_ADDR_OFFSET                 0x1E8u
#define ID12SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID12SRCADDR_ADDR_SHIFT                  0u

//ID12DESTADD
#define ID12DESTADDR_REG_OFFSET                 0x1ECu
#define ID12DESTADDR_ADDR_OFFSET                0x1ECu
#define ID12DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID12DESTADDR_ADDR_SHIFT                 0u

//ID12NEXTDESC
#define ID12NEXTDESC_REG_OFFSET                 0x1F0u
#define ID12NEXTDESC_ADDR_OFFSET                0x1F0u
#define ID12NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID12NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 13 register details
 */
#define ID13CFG_REG_OFFSET                      0x200u

#define ID13CFG_SRC_OP_OFFSET                   0x200u
#define ID13CFG_SRC_OP_MASK                     0x00000003u
#define ID13CFG_SRC_OP_SHIFT                    0u

#define ID13CFG_DEST_OP_OFFSET                  0x200u
#define ID13CFG_DEST_OP_MASK                    0x0000000Cu
#define ID13CFG_DEST_OP_SHIFT                   2u

#define ID13CFG_CHAIN_OFFSET                    0x200u
#define ID13CFG_CHAIN_MASK                      0x00000400u
#define ID13CFG_CHAIN_SHIFT                     10u

#define ID13CFG_EXDESC_OFFSET                   0x200u
#define ID13CFG_EXDESC_MASK                     0x00000800u
#define ID13CFG_EXDESC_SHIFT                    11u

#define ID13CFG_INTR_OFFSET                     0x200u
#define ID13CFG_INTR_MASK                       0x00001000u
#define ID13CFG_INTR_SHIFT                      12u

#define ID13CFG_DRDY_OFFSET                     0x200u
#define ID13CFG_DRDY_MASK                       0x00002000u
#define ID13CFG_DRDY_SHIFT                      13u

#define ID13CFG_DESCVAL_OFFSET                  0x200u
#define ID13CFG_DESCVAL_MASK                    0x00004000u
#define ID13CFG_DESCVAL_SHIFT                   14u

#define ID13CFG_DESCVAL_OFFSET                  0x200u
#define ID13CFG_DESCVAL_MASK                    0x00004000u
#define ID13CFG_DESCVAL_SHIFT                   14u

//ID13BYTECNT
#define ID13BYTECNT_REG_OFFSET                  0x204u
#define ID13BYTECNT_CNT_OFFSET                  0x204u
#define ID13BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID13BYTECNT_CNT_SHIFT                   0u

//ID13SRCADD
#define ID13SRCADDR_REG_OFFSET                  0x208u
#define ID13SRCADDR_ADDR_OFFSET                 0x208u
#define ID13SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID13SRCADDR_ADDR_SHIFT                  0u

//ID13DESTADD
#define ID13DESTADDR_REG_OFFSET                 0x20Cu
#define ID13DESTADDR_ADDR_OFFSET                0x20Cu
#define ID13DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID13DESTADDR_ADDR_SHIFT                 0u

//ID13NEXTDESC
#define ID13NEXTDESC_REG_OFFSET                 0x210u
#define ID13NEXTDESC_ADDR_OFFSET                0x210u
#define ID13NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID13NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 14 register details
 */
#define ID14CFG_REG_OFFSET                      0x220u

#define ID14CFG_SRC_OP_OFFSET                   0x220u
#define ID14CFG_SRC_OP_MASK                     0x00000003u
#define ID14CFG_SRC_OP_SHIFT                    0u

#define ID14CFG_DEST_OP_OFFSET                  0x220u
#define ID14CFG_DEST_OP_MASK                    0x0000000Cu
#define ID14CFG_DEST_OP_SHIFT                   2u

#define ID14CFG_CHAIN_OFFSET                    0x220u
#define ID14CFG_CHAIN_MASK                      0x00000400u
#define ID14CFG_CHAIN_SHIFT                     10u

#define ID14CFG_EXDESC_OFFSET                   0x220u
#define ID14CFG_EXDESC_MASK                     0x00000800u
#define ID14CFG_EXDESC_SHIFT                    11u

#define ID14CFG_INTR_OFFSET                     0x220u
#define ID14CFG_INTR_MASK                       0x00001000u
#define ID14CFG_INTR_SHIFT                      12u

#define ID14CFG_DRDY_OFFSET                     0x220u
#define ID14CFG_DRDY_MASK                       0x00002000u
#define ID14CFG_DRDY_SHIFT                      13u

#define ID14CFG_DESCVAL_OFFSET                  0x220u
#define ID14CFG_DESCVAL_MASK                    0x00004000u
#define ID14CFG_DESCVAL_SHIFT                   14u

#define ID14CFG_DESCVAL_OFFSET                  0x220u
#define ID14CFG_DESCVAL_MASK                    0x00004000u
#define ID14CFG_DESCVAL_SHIFT                   14u

//ID14BYTECNT
#define ID14BYTECNT_REG_OFFSET                  0x224u
#define ID14BYTECNT_CNT_OFFSET                  0x224u
#define ID14BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID14BYTECNT_CNT_SHIFT                   0u

//ID14SRCADD
#define ID14SRCADDR_REG_OFFSET                  0x228u
#define ID14SRCADDR_ADDR_OFFSET                 0x228u
#define ID14SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID14SRCADDR_ADDR_SHIFT                  0u

//ID14DESTADD
#define ID14DESTADDR_REG_OFFSET                 0x22Cu
#define ID14DESTADDR_ADDR_OFFSET                0x22Cu
#define ID14DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID14DESTADDR_ADDR_SHIFT                 0u

//ID14NEXTDESC
#define ID14NEXTDESC_REG_OFFSET                 0x230u
#define ID14NEXTDESC_ADDR_OFFSET                0x230u
#define ID14NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID14NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 15 register details
 */
#define ID15CFG_REG_OFFSET                      0x240u

#define ID15CFG_SRC_OP_OFFSET                   0x240u
#define ID15CFG_SRC_OP_MASK                     0x00000003u
#define ID15CFG_SRC_OP_SHIFT                    0u

#define ID15CFG_DEST_OP_OFFSET                  0x240u
#define ID15CFG_DEST_OP_MASK                    0x0000000Cu
#define ID15CFG_DEST_OP_SHIFT                   2u

#define ID15CFG_CHAIN_OFFSET                    0x240u
#define ID15CFG_CHAIN_MASK                      0x00000400u
#define ID15CFG_CHAIN_SHIFT                     10u

#define ID15CFG_EXDESC_OFFSET                   0x240u
#define ID15CFG_EXDESC_MASK                     0x00000800u
#define ID15CFG_EXDESC_SHIFT                    11u

#define ID15CFG_INTR_OFFSET                     0x240u
#define ID15CFG_INTR_MASK                       0x00001000u
#define ID15CFG_INTR_SHIFT                      12u

#define ID15CFG_DRDY_OFFSET                     0x240u
#define ID15CFG_DRDY_MASK                       0x00002000u
#define ID15CFG_DRDY_SHIFT                      13u

#define ID15CFG_DESCVAL_OFFSET                  0x240u
#define ID15CFG_DESCVAL_MASK                    0x00004000u
#define ID15CFG_DESCVAL_SHIFT                   14u

#define ID15CFG_DESCVAL_OFFSET                  0x240u
#define ID15CFG_DESCVAL_MASK                    0x00004000u
#define ID15CFG_DESCVAL_SHIFT                   14u

//ID15BYTECNT
#define ID15BYTECNT_REG_OFFSET                  0x244u
#define ID15BYTECNT_CNT_OFFSET                  0x244u
#define ID15BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID15BYTECNT_CNT_SHIFT                   0u

//ID15SRCADD
#define ID15SRCADDR_REG_OFFSET                  0x248u
#define ID15SRCADDR_ADDR_OFFSET                 0x248u
#define ID15SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID15SRCADDR_ADDR_SHIFT                  0u

//ID15DESTADD
#define ID15DESTADDR_REG_OFFSET                 0x24Cu
#define ID15DESTADDR_ADDR_OFFSET                0x24Cu
#define ID15DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID15DESTADDR_ADDR_SHIFT                 0u

//ID15NEXTDESC
#define ID15NEXTDESC_REG_OFFSET                 0x250u
#define ID15NEXTDESC_ADDR_OFFSET                0x250u
#define ID15NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID15NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 16 register details
 */
#define ID16CFG_REG_OFFSET                      0x260u

#define ID16CFG_SRC_OP_OFFSET                   0x260u
#define ID16CFG_SRC_OP_MASK                     0x00000003u
#define ID16CFG_SRC_OP_SHIFT                    0u

#define ID16CFG_DEST_OP_OFFSET                  0x260u
#define ID16CFG_DEST_OP_MASK                    0x0000000Cu
#define ID16CFG_DEST_OP_SHIFT                   2u

#define ID16CFG_CHAIN_OFFSET                    0x260u
#define ID16CFG_CHAIN_MASK                      0x00000400u
#define ID16CFG_CHAIN_SHIFT                     10u

#define ID16CFG_EXDESC_OFFSET                   0x260u
#define ID16CFG_EXDESC_MASK                     0x00000800u
#define ID16CFG_EXDESC_SHIFT                    11u

#define ID16CFG_INTR_OFFSET                     0x260u
#define ID16CFG_INTR_MASK                       0x00001000u
#define ID16CFG_INTR_SHIFT                      12u

#define ID16CFG_DRDY_OFFSET                     0x260u
#define ID16CFG_DRDY_MASK                       0x00002000u
#define ID16CFG_DRDY_SHIFT                      13u

#define ID16CFG_DESCVAL_OFFSET                  0x260u
#define ID16CFG_DESCVAL_MASK                    0x00004000u
#define ID16CFG_DESCVAL_SHIFT                   14u

#define ID16CFG_DESCVAL_OFFSET                  0x260u
#define ID16CFG_DESCVAL_MASK                    0x00004000u
#define ID16CFG_DESCVAL_SHIFT                   14u

//ID16BYTECNT
#define ID16BYTECNT_REG_OFFSET                  0x264u
#define ID16BYTECNT_CNT_OFFSET                  0x264u
#define ID16BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID16BYTECNT_CNT_SHIFT                   0u

//ID16SRCADD
#define ID16SRCADDR_REG_OFFSET                  0x268u
#define ID16SRCADDR_ADDR_OFFSET                 0x268u
#define ID16SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID16SRCADDR_ADDR_SHIFT                  0u

//ID16DESTADD
#define ID16DESTADDR_REG_OFFSET                 0x26Cu
#define ID16DESTADDR_ADDR_OFFSET                0x26Cu
#define ID16DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID16DESTADDR_ADDR_SHIFT                 0u

//ID16NEXTDESC
#define ID16NEXTDESC_REG_OFFSET                 0x270u
#define ID16NEXTDESC_ADDR_OFFSET                0x270u
#define ID16NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID16NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 17 register details
 */
#define ID17CFG_REG_OFFSET                      0x280u

#define ID17CFG_SRC_OP_OFFSET                   0x280u
#define ID17CFG_SRC_OP_MASK                     0x00000003u
#define ID17CFG_SRC_OP_SHIFT                    0u

#define ID17CFG_DEST_OP_OFFSET                  0x280u
#define ID17CFG_DEST_OP_MASK                    0x0000000Cu
#define ID17CFG_DEST_OP_SHIFT                   2u

#define ID17CFG_CHAIN_OFFSET                    0x280u
#define ID17CFG_CHAIN_MASK                      0x00000400u
#define ID17CFG_CHAIN_SHIFT                     10u

#define ID17CFG_EXDESC_OFFSET                   0x280u
#define ID17CFG_EXDESC_MASK                     0x00000800u
#define ID17CFG_EXDESC_SHIFT                    11u

#define ID17CFG_INTR_OFFSET                     0x280u
#define ID17CFG_INTR_MASK                       0x00001000u
#define ID17CFG_INTR_SHIFT                      12u

#define ID17CFG_DRDY_OFFSET                     0x280u
#define ID17CFG_DRDY_MASK                       0x00002000u
#define ID17CFG_DRDY_SHIFT                      13u

#define ID17CFG_DESCVAL_OFFSET                  0x280u
#define ID17CFG_DESCVAL_MASK                    0x00004000u
#define ID17CFG_DESCVAL_SHIFT                   14u

#define ID17CFG_DESCVAL_OFFSET                  0x280u
#define ID17CFG_DESCVAL_MASK                    0x00004000u
#define ID17CFG_DESCVAL_SHIFT                   14u

//ID17BYTECNT
#define ID17BYTECNT_REG_OFFSET                  0x284u
#define ID17BYTECNT_CNT_OFFSET                  0x284u
#define ID17BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID17BYTECNT_CNT_SHIFT                   0u

//ID17SRCADD
#define ID17SRCADDR_REG_OFFSET                  0x288u
#define ID17SRCADDR_ADDR_OFFSET                 0x288u
#define ID17SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID17SRCADDR_ADDR_SHIFT                  0u

//ID17DESTADD
#define ID17DESTADDR_REG_OFFSET                 0x28Cu
#define ID17DESTADDR_ADDR_OFFSET                0x28Cu
#define ID17DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID17DESTADDR_ADDR_SHIFT                 0u

//ID17NEXTDESC
#define ID17NEXTDESC_REG_OFFSET                 0x290u
#define ID17NEXTDESC_ADDR_OFFSET                0x290u
#define ID17NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID17NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 18 register details
 */
#define ID18CFG_REG_OFFSET                      0x2A0u

#define ID18CFG_SRC_OP_OFFSET                   0x2A0u
#define ID18CFG_SRC_OP_MASK                     0x00000003u
#define ID18CFG_SRC_OP_SHIFT                    0u

#define ID18CFG_DEST_OP_OFFSET                  0x2A0u
#define ID18CFG_DEST_OP_MASK                    0x0000000Cu
#define ID18CFG_DEST_OP_SHIFT                   2u

#define ID18CFG_CHAIN_OFFSET                    0x2A0u
#define ID18CFG_CHAIN_MASK                      0x00000400u
#define ID18CFG_CHAIN_SHIFT                     10u

#define ID18CFG_EXDESC_OFFSET                   0x2A0u
#define ID18CFG_EXDESC_MASK                     0x00000800u
#define ID18CFG_EXDESC_SHIFT                    11u

#define ID18CFG_INTR_OFFSET                     0x2A0u
#define ID18CFG_INTR_MASK                       0x00001000u
#define ID18CFG_INTR_SHIFT                      12u

#define ID18CFG_DRDY_OFFSET                     0x2A0u
#define ID18CFG_DRDY_MASK                       0x00002000u
#define ID18CFG_DRDY_SHIFT                      13u

#define ID18CFG_DESCVAL_OFFSET                  0x2A0u
#define ID18CFG_DESCVAL_MASK                    0x00004000u
#define ID18CFG_DESCVAL_SHIFT                   14u

#define ID18CFG_DESCVAL_OFFSET                  0x2A0u
#define ID18CFG_DESCVAL_MASK                    0x00004000u
#define ID18CFG_DESCVAL_SHIFT                   14u

//ID18BYTECNT
#define ID18BYTECNT_REG_OFFSET                  0x2A4u
#define ID18BYTECNT_CNT_OFFSET                  0x2A4u
#define ID18BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID18BYTECNT_CNT_SHIFT                   0u

//ID18DESTADD
#define ID18DESTADDR_REG_OFFSET                 0x2ACu
#define ID18DESTADDR_ADDR_OFFSET                0x2ACu
#define ID18DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID18DESTADDR_ADDR_SHIFT                 0u

//ID18NEXTDESC
#define ID18NEXTDESC_REG_OFFSET                 0x2B0u
#define ID18NEXTDESC_ADDR_OFFSET                0x2B0u
#define ID18NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID18NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 19 register details
 */
#define ID19CFG_REG_OFFSET                      0x2C0u

#define ID19CFG_SRC_OP_OFFSET                   0x2C0u
#define ID19CFG_SRC_OP_MASK                     0x00000003u
#define ID19CFG_SRC_OP_SHIFT                    0u

#define ID19CFG_DEST_OP_OFFSET                  0x2C0u
#define ID19CFG_DEST_OP_MASK                    0x0000000Cu
#define ID19CFG_DEST_OP_SHIFT                   2u

#define ID19CFG_CHAIN_OFFSET                    0x2C0u
#define ID19CFG_CHAIN_MASK                      0x00000400u
#define ID19CFG_CHAIN_SHIFT                     10u

#define ID19CFG_EXDESC_OFFSET                   0x2C0u
#define ID19CFG_EXDESC_MASK                     0x00000800u
#define ID19CFG_EXDESC_SHIFT                    11u

#define ID19CFG_INTR_OFFSET                     0x2C0u
#define ID19CFG_INTR_MASK                       0x00001000u
#define ID19CFG_INTR_SHIFT                      12u

#define ID19CFG_DRDY_OFFSET                     0x2C0u
#define ID19CFG_DRDY_MASK                       0x00002000u
#define ID19CFG_DRDY_SHIFT                      13u

#define ID19CFG_DESCVAL_OFFSET                  0x2C0u
#define ID19CFG_DESCVAL_MASK                    0x00004000u
#define ID19CFG_DESCVAL_SHIFT                   14u

#define ID19CFG_DESCVAL_OFFSET                  0x2C0u
#define ID19CFG_DESCVAL_MASK                    0x00004000u
#define ID19CFG_DESCVAL_SHIFT                   14u

//ID19BYTECNT
#define ID19BYTECNT_REG_OFFSET                  0x2C4u
#define ID19BYTECNT_CNT_OFFSET                  0x2C4u
#define ID19BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID19BYTECNT_CNT_SHIFT                   0u

//ID19SRCADD
#define ID19SRCADDR_REG_OFFSET                  0x2C8u
#define ID19SRCADDR_ADDR_OFFSET                 0x2C8u
#define ID19SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID19SRCADDR_ADDR_SHIFT                  0u

//ID19DESTADD
#define ID19DESTADDR_REG_OFFSET                 0x2CCu
#define ID19DESTADDR_ADDR_OFFSET                0x2CCu
#define ID19DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID19DESTADDR_ADDR_SHIFT                 0u

//ID19NEXTDESC
#define ID19NEXTDESC_REG_OFFSET                 0x2D0u
#define ID19NEXTDESC_ADDR_OFFSET                0x2D0u
#define ID19NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID19NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 20 register details
 */
#define ID20CFG_REG_OFFSET                      0x2E0u

#define ID20CFG_SRC_OP_OFFSET                   0x2E0u
#define ID20CFG_SRC_OP_MASK                     0x00000003u
#define ID20CFG_SRC_OP_SHIFT                    0u

#define ID20CFG_DEST_OP_OFFSET                  0x2E0u
#define ID20CFG_DEST_OP_MASK                    0x0000000Cu
#define ID20CFG_DEST_OP_SHIFT                   2u

#define ID20CFG_CHAIN_OFFSET                    0x2E0u
#define ID20CFG_CHAIN_MASK                      0x00000400u
#define ID20CFG_CHAIN_SHIFT                     10u

#define ID20CFG_EXDESC_OFFSET                   0x2E0u
#define ID20CFG_EXDESC_MASK                     0x00000800u
#define ID20CFG_EXDESC_SHIFT                    11u

#define ID20CFG_INTR_OFFSET                     0x2E0u
#define ID20CFG_INTR_MASK                       0x00001000u
#define ID20CFG_INTR_SHIFT                      12u

#define ID20CFG_DRDY_OFFSET                     0x2E0u
#define ID20CFG_DRDY_MASK                       0x00002000u
#define ID20CFG_DRDY_SHIFT                      13u

#define ID20CFG_DESCVAL_OFFSET                  0x2E0u
#define ID20CFG_DESCVAL_MASK                    0x00004000u
#define ID20CFG_DESCVAL_SHIFT                   14u

#define ID20CFG_DESCVAL_OFFSET                  0x2E0u
#define ID20CFG_DESCVAL_MASK                    0x00004000u
#define ID20CFG_DESCVAL_SHIFT                   14u

//ID20BYTECNT
#define ID20BYTECNT_REG_OFFSET                  0x2E4u
#define ID20BYTECNT_CNT_OFFSET                  0x2E4u
#define ID20BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID20BYTECNT_CNT_SHIFT                   0u

//ID20SRCADD
#define ID20SRCADDR_REG_OFFSET                  0x2E8u
#define ID20SRCADDR_ADDR_OFFSET                 0x2E8u
#define ID20SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID20SRCADDR_ADDR_SHIFT                  0u

//ID20DESTADD
#define ID20DESTADDR_REG_OFFSET                 0x2ECu
#define ID20DESTADDR_ADDR_OFFSET                0x2ECu
#define ID20DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID20DESTADDR_ADDR_SHIFT                 0u

//ID20NEXTDESC
#define ID20NEXTDESC_REG_OFFSET                 0x2F0u
#define ID20NEXTDESC_ADDR_OFFSET                0x2F0u
#define ID20NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID20NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 21 register details
 */
#define ID21CFG_REG_OFFSET                      0x300u

#define ID21CFG_SRC_OP_OFFSET                   0x300u
#define ID21CFG_SRC_OP_MASK                     0x00000003u
#define ID21CFG_SRC_OP_SHIFT                    0u

#define ID21CFG_DEST_OP_OFFSET                  0x300u
#define ID21CFG_DEST_OP_MASK                    0x0000000Cu
#define ID21CFG_DEST_OP_SHIFT                   2u

#define ID21CFG_CHAIN_OFFSET                    0x300u
#define ID21CFG_CHAIN_MASK                      0x00000400u
#define ID21CFG_CHAIN_SHIFT                     10u

#define ID21CFG_EXDESC_OFFSET                   0x300u
#define ID21CFG_EXDESC_MASK                     0x00000800u
#define ID21CFG_EXDESC_SHIFT                    11u

#define ID21CFG_INTR_OFFSET                     0x300u
#define ID21CFG_INTR_MASK                       0x00001000u
#define ID21CFG_INTR_SHIFT                      12u

#define ID21CFG_DRDY_OFFSET                     0x300u
#define ID21CFG_DRDY_MASK                       0x00002000u
#define ID21CFG_DRDY_SHIFT                      13u

#define ID21CFG_DESCVAL_OFFSET                  0x300u
#define ID21CFG_DESCVAL_MASK                    0x00004000u
#define ID21CFG_DESCVAL_SHIFT                   14u

#define ID21CFG_DESCVAL_OFFSET                  0x300u
#define ID21CFG_DESCVAL_MASK                    0x00004000u
#define ID21CFG_DESCVAL_SHIFT                   14u

//ID21BYTECNT
#define ID21BYTECNT_REG_OFFSET                  0x304u
#define ID21BYTECNT_CNT_OFFSET                  0x304u
#define ID21BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID21BYTECNT_CNT_SHIFT                   0u

//ID21SRCADD
#define ID21SRCADDR_REG_OFFSET                  0x308u
#define ID21SRCADDR_ADDR_OFFSET                 0x308u
#define ID21SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID21SRCADDR_ADDR_SHIFT                  0u

//ID21DESTADD
#define ID21DESTADDR_REG_OFFSET                 0x30Cu
#define ID21DESTADDR_ADDR_OFFSET                0x30Cu
#define ID21DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID21DESTADDR_ADDR_SHIFT                 0u

//ID21NEXTDESC
#define ID21NEXTDESC_REG_OFFSET                 0x310u
#define ID21NEXTDESC_ADDR_OFFSET                0x310u
#define ID21NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID21NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 22 register details
 */
#define ID22CFG_REG_OFFSET                      0x320u

#define ID22CFG_SRC_OP_OFFSET                   0x320u
#define ID22CFG_SRC_OP_MASK                     0x00000003u
#define ID22CFG_SRC_OP_SHIFT                    0u

#define ID22CFG_DEST_OP_OFFSET                  0x320u
#define ID22CFG_DEST_OP_MASK                    0x0000000Cu
#define ID22CFG_DEST_OP_SHIFT                   2u

#define ID22CFG_CHAIN_OFFSET                    0x320u
#define ID22CFG_CHAIN_MASK                      0x00000400u
#define ID22CFG_CHAIN_SHIFT                     10u

#define ID22CFG_EXDESC_OFFSET                   0x320u
#define ID22CFG_EXDESC_MASK                     0x00000800u
#define ID22CFG_EXDESC_SHIFT                    11u

#define ID22CFG_INTR_OFFSET                     0x320u
#define ID22CFG_INTR_MASK                       0x00001000u
#define ID22CFG_INTR_SHIFT                      12u

#define ID22CFG_DRDY_OFFSET                     0x320u
#define ID22CFG_DRDY_MASK                       0x00002000u
#define ID22CFG_DRDY_SHIFT                      13u

#define ID22CFG_DESCVAL_OFFSET                  0x320u
#define ID22CFG_DESCVAL_MASK                    0x00004000u
#define ID22CFG_DESCVAL_SHIFT                   14u

#define ID22CFG_DESCVAL_OFFSET                  0x320u
#define ID22CFG_DESCVAL_MASK                    0x00004000u
#define ID22CFG_DESCVAL_SHIFT                   14u

//ID22BYTECNT
#define ID22BYTECNT_REG_OFFSET                  0x324u
#define ID22BYTECNT_CNT_OFFSET                  0x324u
#define ID22BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID22BYTECNT_CNT_SHIFT                   0u

//ID22SRCADD
#define ID22SRCADDR_REG_OFFSET                  0x328u
#define ID22SRCADDR_ADDR_OFFSET                 0x328u
#define ID22SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID22SRCADDR_ADDR_SHIFT                  0u

//ID22DESTADD
#define ID22DESTADDR_REG_OFFSET                 0x32Cu
#define ID22DESTADDR_ADDR_OFFSET                0x32Cu
#define ID22DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID22DESTADDR_ADDR_SHIFT                 0u

//ID22NEXTDESC
#define ID22NEXTDESC_REG_OFFSET                 0x330u
#define ID22NEXTDESC_ADDR_OFFSET                0x330u
#define ID22NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID22NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 23 register details
 */
#define ID23CFG_REG_OFFSET                      0x340u

#define ID23CFG_SRC_OP_OFFSET                   0x340u
#define ID23CFG_SRC_OP_MASK                     0x00000003u
#define ID23CFG_SRC_OP_SHIFT                    0u

#define ID23CFG_DEST_OP_OFFSET                  0x340u
#define ID23CFG_DEST_OP_MASK                    0x0000000Cu
#define ID23CFG_DEST_OP_SHIFT                   2u

#define ID23CFG_CHAIN_OFFSET                    0x340u
#define ID23CFG_CHAIN_MASK                      0x00000400u
#define ID23CFG_CHAIN_SHIFT                     10u

#define ID23CFG_EXDESC_OFFSET                   0x340u
#define ID23CFG_EXDESC_MASK                     0x00000800u
#define ID23CFG_EXDESC_SHIFT                    11u

#define ID23CFG_INTR_OFFSET                     0x340u
#define ID23CFG_INTR_MASK                       0x00001000u
#define ID23CFG_INTR_SHIFT                      12u

#define ID23CFG_DRDY_OFFSET                     0x340u
#define ID23CFG_DRDY_MASK                       0x00002000u
#define ID23CFG_DRDY_SHIFT                      13u

#define ID23CFG_DESCVAL_OFFSET                  0x340u
#define ID23CFG_DESCVAL_MASK                    0x00004000u
#define ID23CFG_DESCVAL_SHIFT                   14u

#define ID23CFG_DESCVAL_OFFSET                  0x340u
#define ID23CFG_DESCVAL_MASK                    0x00004000u
#define ID23CFG_DESCVAL_SHIFT                   14u

//ID23BYTECNT
#define ID23BYTECNT_REG_OFFSET                      0x344u
#define ID23BYTECNT_CNT_OFFSET                  0x344u
#define ID23BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID23BYTECNT_CNT_SHIFT                   0u

//ID23SRCADD
#define ID23SRCADDR_REG_OFFSET                  0x348u
#define ID23SRCADDR_ADDR_OFFSET                 0x348u
#define ID23SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID23SRCADDR_ADDR_SHIFT                  0u

//ID23DESTADD
#define ID23DESTADDR_REG_OFFSET                 0x34Cu
#define ID23DESTADDR_ADDR_OFFSET                0x34Cu
#define ID23DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID23DESTADDR_ADDR_SHIFT                 0u

//ID23NEXTDESC
#define ID23NEXTDESC_REG_OFFSET                 0x350u
#define ID23NEXTDESC_ADDR_OFFSET                0x350u
#define ID23NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID23NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 24 register details
 */
#define ID24CFG_REG_OFFSET                      0x360u

#define ID24CFG_SRC_OP_OFFSET                   0x360u
#define ID24CFG_SRC_OP_MASK                     0x00000003u
#define ID24CFG_SRC_OP_SHIFT                    0u

#define ID24CFG_DEST_OP_OFFSET                  0x360u
#define ID24CFG_DEST_OP_MASK                    0x0000000Cu
#define ID24CFG_DEST_OP_SHIFT                   2u

#define ID24CFG_CHAIN_OFFSET                    0x360u
#define ID24CFG_CHAIN_MASK                      0x00000400u
#define ID24CFG_CHAIN_SHIFT                     10u

#define ID24CFG_EXDESC_OFFSET                   0x360u
#define ID24CFG_EXDESC_MASK                     0x00000800u
#define ID24CFG_EXDESC_SHIFT                    11u

#define ID24CFG_INTR_OFFSET                     0x360u
#define ID24CFG_INTR_MASK                       0x00001000u
#define ID24CFG_INTR_SHIFT                      12u

#define ID24CFG_DRDY_OFFSET                     0x360u
#define ID24CFG_DRDY_MASK                       0x00002000u
#define ID24CFG_DRDY_SHIFT                      13u

#define ID24CFG_DESCVAL_OFFSET                  0x360u
#define ID24CFG_DESCVAL_MASK                    0x00004000u
#define ID24CFG_DESCVAL_SHIFT                   14u

#define ID24CFG_DESCVAL_OFFSET                  0x360u
#define ID24CFG_DESCVAL_MASK                    0x00004000u
#define ID24CFG_DESCVAL_SHIFT                   14u

//ID24BYTECNT
#define ID24BYTECNT_REG_OFFSET                  0x364u
#define ID24BYTECNT_CNT_OFFSET                  0x364u
#define ID24BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID24BYTECNT_CNT_SHIFT                   0u

//ID24SRCADD
#define ID24SRCADDR_REG_OFFSET                  0x368u
#define ID24SRCADDR_ADDR_OFFSET                 0x368u
#define ID24SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID24SRCADDR_ADDR_SHIFT                  0u

//ID24DESTADD
#define ID24DESTADDR_REG_OFFSET                 0x36Cu
#define ID24DESTADDR_ADDR_OFFSET                0x36Cu
#define ID24DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID24DESTADDR_ADDR_SHIFT                 0u

//ID24NEXTDESC
#define ID24NEXTDESC_REG_OFFSET                 0x370u
#define ID24NEXTDESC_ADDR_OFFSET                0x370u
#define ID24NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID24NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 25 register details
 */
#define ID25CFG_REG_OFFSET                      0x380u

#define ID25CFG_SRC_OP_OFFSET                   0x380u
#define ID25CFG_SRC_OP_MASK                     0x00000003u
#define ID25CFG_SRC_OP_SHIFT                    0u

#define ID25CFG_DEST_OP_OFFSET                  0x380u
#define ID25CFG_DEST_OP_MASK                    0x0000000Cu
#define ID25CFG_DEST_OP_SHIFT                   2u

#define ID25CFG_CHAIN_OFFSET                    0x380u
#define ID25CFG_CHAIN_MASK                      0x00000400u
#define ID25CFG_CHAIN_SHIFT                     10u

#define ID25CFG_EXDESC_OFFSET                   0x380u
#define ID25CFG_EXDESC_MASK                     0x00000800u
#define ID25CFG_EXDESC_SHIFT                    11u

#define ID25CFG_INTR_OFFSET                     0x380u
#define ID25CFG_INTR_MASK                       0x00001000u
#define ID25CFG_INTR_SHIFT                      12u

#define ID25CFG_DRDY_OFFSET                     0x380u
#define ID25CFG_DRDY_MASK                       0x00002000u
#define ID25CFG_DRDY_SHIFT                      13u

#define ID25CFG_DESCVAL_OFFSET                  0x380u
#define ID25CFG_DESCVAL_MASK                    0x00004000u
#define ID25CFG_DESCVAL_SHIFT                   14u

#define ID25CFG_DESCVAL_OFFSET                  0x380u
#define ID25CFG_DESCVAL_MASK                    0x00004000u
#define ID25CFG_DESCVAL_SHIFT                   14u

//ID25BYTECNT
#define ID25BYTECNT_REG_OFFSET                  0x384u
#define ID25BYTECNT_CNT_OFFSET                  0x384u
#define ID25BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID25BYTECNT_CNT_SHIFT                   0u

//ID25SRCADD
#define ID25SRCADDR_REG_OFFSET                  0x388u
#define ID25SRCADDR_ADDR_OFFSET                 0x388u
#define ID25SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID25SRCADDR_ADDR_SHIFT                  0u

//ID25DESTADD
#define ID25DESTADDR_REG_OFFSET                 0x38Cu
#define ID25DESTADDR_ADDR_OFFSET                0x38Cu
#define ID25DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID25DESTADDR_ADDR_SHIFT                 0u

//ID25NEXTDESC
#define ID25NEXTDESC_REG_OFFSET                 0x390u
#define ID25NEXTDESC_ADDR_OFFSET                0x390u
#define ID25NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID25NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 26 register details
 */
#define ID26CFG_REG_OFFSET                      0x3A0u

#define ID26CFG_SRC_OP_OFFSET                   0x3A0u
#define ID26CFG_SRC_OP_MASK                     0x00000003u
#define ID26CFG_SRC_OP_SHIFT                    0u

#define ID26CFG_DEST_OP_OFFSET                  0x3A0u
#define ID26CFG_DEST_OP_MASK                    0x0000000Cu
#define ID26CFG_DEST_OP_SHIFT                   2u

#define ID26CFG_CHAIN_OFFSET                    0x3A0u
#define ID26CFG_CHAIN_MASK                      0x00000400u
#define ID26CFG_CHAIN_SHIFT                     10u

#define ID26CFG_EXDESC_OFFSET                   0x3A0u
#define ID26CFG_EXDESC_MASK                     0x00000800u
#define ID26CFG_EXDESC_SHIFT                    11u

#define ID26CFG_INTR_OFFSET                     0x3A0u
#define ID26CFG_INTR_MASK                       0x00001000u
#define ID26CFG_INTR_SHIFT                      12u

#define ID26CFG_DRDY_OFFSET                     0x3A0u
#define ID26CFG_DRDY_MASK                       0x00002000u
#define ID26CFG_DRDY_SHIFT                      13u

#define ID26CFG_DESCVAL_OFFSET                  0x3A0u
#define ID26CFG_DESCVAL_MASK                    0x00004000u
#define ID26CFG_DESCVAL_SHIFT                   14u

#define ID26CFG_DESCVAL_OFFSET                  0x3A0u
#define ID26CFG_DESCVAL_MASK                    0x00004000u
#define ID26CFG_DESCVAL_SHIFT                   14u

//ID26BYTECNT
#define ID26BYTECNT_REG_OFFSET                  0x3A4u
#define ID26BYTECNT_CNT_OFFSET                  0x3A4u
#define ID26BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID26BYTECNT_CNT_SHIFT                   0u

//ID26SRCADD
#define ID26SRCADDR_REG_OFFSET                  0x3A8u
#define ID26SRCADDR_ADDR_OFFSET                 0x3A8u
#define ID26SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID26SRCADDR_ADDR_SHIFT                  0u

//ID26DESTADD
#define ID26DESTADDR_REG_OFFSET                 0x3ACu
#define ID26DESTADDR_ADDR_OFFSET                0x3ACu
#define ID26DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID26DESTADDR_ADDR_SHIFT                 0u

//ID26NEXTDESC
#define ID26NEXTDESC_REG_OFFSET                 0x3B0u
#define ID26NEXTDESC_ADDR_OFFSET                0x3B0u
#define ID26NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID26NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 27 register details
 */
#define ID27CFG_REG_OFFSET                      0x3C0u

#define ID27CFG_SRC_OP_OFFSET                   0x3C0u
#define ID27CFG_SRC_OP_MASK                     0x00000003u
#define ID27CFG_SRC_OP_SHIFT                    0u

#define ID27CFG_DEST_OP_OFFSET                  0x3C0u
#define ID27CFG_DEST_OP_MASK                    0x0000000Cu
#define ID27CFG_DEST_OP_SHIFT                   2u

#define ID27CFG_CHAIN_OFFSET                    0x3C0u
#define ID27CFG_CHAIN_MASK                      0x00000400u
#define ID27CFG_CHAIN_SHIFT                     10u

#define ID27CFG_EXDESC_OFFSET                   0x3C0u
#define ID27CFG_EXDESC_MASK                     0x00000800u
#define ID27CFG_EXDESC_SHIFT                    11u

#define ID27CFG_INTR_OFFSET                     0x3C0u
#define ID27CFG_INTR_MASK                       0x00001000u
#define ID27CFG_INTR_SHIFT                      12u

#define ID27CFG_DRDY_OFFSET                     0x3C0u
#define ID27CFG_DRDY_MASK                       0x00002000u
#define ID27CFG_DRDY_SHIFT                      13u

#define ID27CFG_DESCVAL_OFFSET                  0x3C0u
#define ID27CFG_DESCVAL_MASK                    0x00004000u
#define ID27CFG_DESCVAL_SHIFT                   14u

#define ID27CFG_DESCVAL_OFFSET                  0x3C0u
#define ID27CFG_DESCVAL_MASK                    0x00004000u
#define ID27CFG_DESCVAL_SHIFT                   14u

//ID27BYTECNT
#define ID27BYTECNT_REG_OFFSET                  0x3C4u
#define ID27BYTECNT_CNT_OFFSET                  0x3C4u
#define ID27BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID27BYTECNT_CNT_SHIFT                   0u

//ID27SRCADD
#define ID27SRCADDR_REG_OFFSET                  0x3C8u
#define ID27SRCADDR_ADDR_OFFSET                 0x3C8u
#define ID27SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID27SRCADDR_ADDR_SHIFT                  0u

//ID27DESTADD
#define ID27DESTADDR_REG_OFFSET                 0x3CCu
#define ID27DESTADDR_ADDR_OFFSET                0x3CCu
#define ID27DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID27DESTADDR_ADDR_SHIFT                 0u

//ID27NEXTDESC
#define ID27NEXTDESC_REG_OFFSET                 0x3D0u
#define ID27NEXTDESC_ADDR_OFFSET                0x3D0u
#define ID27NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID27NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 28 register details
 */
#define ID28CFG_REG_OFFSET                      0x3E0u

#define ID28CFG_SRC_OP_OFFSET                   0x3E0u
#define ID28CFG_SRC_OP_MASK                     0x00000003u
#define ID28CFG_SRC_OP_SHIFT                    0u

#define ID28CFG_DEST_OP_OFFSET                  0x3E0u
#define ID28CFG_DEST_OP_MASK                    0x0000000Cu
#define ID28CFG_DEST_OP_SHIFT                   2u

#define ID28CFG_CHAIN_OFFSET                    0x3E0u
#define ID28CFG_CHAIN_MASK                      0x00000400u
#define ID28CFG_CHAIN_SHIFT                     10u

#define ID28CFG_EXDESC_OFFSET                   0x3E0u
#define ID28CFG_EXDESC_MASK                     0x00000800u
#define ID28CFG_EXDESC_SHIFT                    11u

#define ID28CFG_INTR_OFFSET                     0x3E0u
#define ID28CFG_INTR_MASK                       0x00001000u
#define ID28CFG_INTR_SHIFT                      12u

#define ID28CFG_DRDY_OFFSET                     0x3E0u
#define ID28CFG_DRDY_MASK                       0x00002000u
#define ID28CFG_DRDY_SHIFT                      13u

#define ID28CFG_DESCVAL_OFFSET                  0x3E0u
#define ID28CFG_DESCVAL_MASK                    0x00004000u
#define ID28CFG_DESCVAL_SHIFT                   14u

#define ID28CFG_DESCVAL_OFFSET                  0x3E0u
#define ID28CFG_DESCVAL_MASK                    0x00004000u
#define ID28CFG_DESCVAL_SHIFT                   14u

//ID28BYTECNT
#define ID28BYTECNT_REG_OFFSET                  0x3E4u
#define ID28BYTECNT_CNT_OFFSET                  0x3E4u
#define ID28BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID28BYTECNT_CNT_SHIFT                   0u

//ID28SRCADD
#define ID28SRCADDR_REG_OFFSET                  0x3E8u
#define ID28SRCADDR_ADDR_OFFSET                 0x3E8u
#define ID28SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID28SRCADDR_ADDR_SHIFT                  0u

//ID28DESTADD
#define ID28DESTADDR_REG_OFFSET                 0x3ECu
#define ID28DESTADDR_ADDR_OFFSET                0x3ECu
#define ID28DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID28DESTADDR_ADDR_SHIFT                 0u

//ID28NEXTDESC
#define ID28NEXTDESC_REG_OFFSET                 0x3F0u
#define ID28NEXTDESC_ADDR_OFFSET                0x3F0u
#define ID28NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID28NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 29 register details
 */
#define ID29CFG_REG_OFFSET                      0x400u

#define ID29CFG_SRC_OP_OFFSET                   0x400u
#define ID29CFG_SRC_OP_MASK                     0x00000003u
#define ID29CFG_SRC_OP_SHIFT                    0u

#define ID29CFG_DEST_OP_OFFSET                  0x400u
#define ID29CFG_DEST_OP_MASK                    0x0000000Cu
#define ID29CFG_DEST_OP_SHIFT                   2u

#define ID29CFG_CHAIN_OFFSET                    0x400u
#define ID29CFG_CHAIN_MASK                      0x00000400u
#define ID29CFG_CHAIN_SHIFT                     10u

#define ID29CFG_EXDESC_OFFSET                   0x400u
#define ID29CFG_EXDESC_MASK                     0x00000800u
#define ID29CFG_EXDESC_SHIFT                    11u

#define ID29CFG_INTR_OFFSET                     0x400u
#define ID29CFG_INTR_MASK                       0x00001000u
#define ID29CFG_INTR_SHIFT                      12u

#define ID29CFG_DRDY_OFFSET                     0x400u
#define ID29CFG_DRDY_MASK                       0x00002000u
#define ID29CFG_DRDY_SHIFT                      13u

#define ID29CFG_DESCVAL_OFFSET                  0x400u
#define ID29CFG_DESCVAL_MASK                    0x00004000u
#define ID29CFG_DESCVAL_SHIFT                   14u

#define ID29CFG_DESCVAL_OFFSET                  0x400u
#define ID29CFG_DESCVAL_MASK                    0x00004000u
#define ID29CFG_DESCVAL_SHIFT                   14u

//ID29BYTECNT
#define ID29BYTECNT_REG_OFFSET                  0x404u
#define ID29BYTECNT_CNT_OFFSET                  0x404u
#define ID29BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID29BYTECNT_CNT_SHIFT                   0u

//ID29SRCADD
#define ID29SRCADDR_REG_OFFSET                  0x408u
#define ID29SRCADDR_ADDR_OFFSET                 0x408u
#define ID29SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID29SRCADDR_ADDR_SHIFT                  0u

//ID29DESTADD
#define ID29DESTADDR_REG_OFFSET                 0x40Cu
#define ID29DESTADDR_ADDR_OFFSET                0x40Cu
#define ID29DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID29DESTADDR_ADDR_SHIFT                 0u

//ID29NEXTDESC
#define ID29NEXTDESC_REG_OFFSET                 0x410u
#define ID29NEXTDESC_ADDR_OFFSET                0x410u
#define ID29NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID29NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 30 register details
 */
#define ID30CFG_REG_OFFSET                      0x420u

#define ID30CFG_SRC_OP_OFFSET                   0x420u
#define ID30CFG_SRC_OP_MASK                     0x00000003u
#define ID30CFG_SRC_OP_SHIFT                    0u

#define ID30CFG_DEST_OP_OFFSET                  0x420u
#define ID30CFG_DEST_OP_MASK                    0x0000000Cu
#define ID30CFG_DEST_OP_SHIFT                   2u

#define ID30CFG_CHAIN_OFFSET                    0x420u
#define ID30CFG_CHAIN_MASK                      0x00000400u
#define ID30CFG_CHAIN_SHIFT                     10u

#define ID30CFG_EXDESC_OFFSET                   0x420u
#define ID30CFG_EXDESC_MASK                     0x00000800u
#define ID30CFG_EXDESC_SHIFT                    11u

#define ID30CFG_INTR_OFFSET                     0x420u
#define ID30CFG_INTR_MASK                       0x00001000u
#define ID30CFG_INTR_SHIFT                      12u

#define ID30CFG_DRDY_OFFSET                     0x420u
#define ID30CFG_DRDY_MASK                       0x00002000u
#define ID30CFG_DRDY_SHIFT                      13u

#define ID30CFG_DESCVAL_OFFSET                  0x420u
#define ID30CFG_DESCVAL_MASK                    0x00004000u
#define ID30CFG_DESCVAL_SHIFT                   14u

#define ID30CFG_DESCVAL_OFFSET                  0x420u
#define ID30CFG_DESCVAL_MASK                    0x00004000u
#define ID30CFG_DESCVAL_SHIFT                   14u

//ID30BYTECNT
#define ID30BYTECNT_REG_OFFSET                  0x424u
#define ID30BYTECNT_CNT_OFFSET                  0x424u
#define ID30BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID30BYTECNT_CNT_SHIFT                   0u

//ID30SRCADD
#define ID30SRCADDR_REG_OFFSET                  0x428u
#define ID30SRCADDR_ADDR_OFFSET                 0x428u
#define ID30SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID30SRCADDR_ADDR_SHIFT                  0u

//ID30DESTADD
#define ID30DESTADDR_REG_OFFSET                 0x42Cu
#define ID30DESTADDR_ADDR_OFFSET                0x42Cu
#define ID30DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID30DESTADDR_ADDR_SHIFT                 0u

//ID30NEXTDESC
#define ID30NEXTDESC_REG_OFFSET                 0x430u
#define ID30NEXTDESC_ADDR_OFFSET                0x430u
#define ID30NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID30NEXTDESC_ADDR_SHIFT                 0u
/*------------------------------------------------------------------------------
 * Internal Descriptor 31 register details
 */
#define ID31CFG_REG_OFFSET                      0x440u

#define ID31CFG_SRC_OP_OFFSET                   0x440u
#define ID31CFG_SRC_OP_MASK                     0x00000003u
#define ID31CFG_SRC_OP_SHIFT                    0u

#define ID31CFG_DEST_OP_OFFSET                  0x440u
#define ID31CFG_DEST_OP_MASK                    0x0000000Cu
#define ID31CFG_DEST_OP_SHIFT                   2u

#define ID31CFG_CHAIN_OFFSET                    0x440u
#define ID31CFG_CHAIN_MASK                      0x00000400u
#define ID31CFG_CHAIN_SHIFT                     10u

#define ID31CFG_EXDESC_OFFSET                   0x440u
#define ID31CFG_EXDESC_MASK                     0x00000800u
#define ID31CFG_EXDESC_SHIFT                    11u

#define ID31CFG_INTR_OFFSET                     0x440u
#define ID31CFG_INTR_MASK                       0x00001000u
#define ID31CFG_INTR_SHIFT                      12u

#define ID31CFG_DRDY_OFFSET                     0x440u
#define ID31CFG_DRDY_MASK                       0x00002000u
#define ID31CFG_DRDY_SHIFT                      13u

#define ID31CFG_DESCVAL_OFFSET                  0x440u
#define ID31CFG_DESCVAL_MASK                    0x00004000u
#define ID31CFG_DESCVAL_SHIFT                   14u

#define ID31CFG_DESCVAL_OFFSET                  0x440u
#define ID31CFG_DESCVAL_MASK                    0x00004000u
#define ID31CFG_DESCVAL_SHIFT                   14u

//ID31BYTECNT
#define ID31BYTECNT_REG_OFFSET                  0x444u
#define ID31BYTECNT_CNT_OFFSET                  0x444u
#define ID31BYTECNT_CNT_MASK                    0x007FFFFFu
#define ID31BYTECNT_CNT_SHIFT                   0u

//ID31SRCADD
#define ID31SRCADDR_REG_OFFSET                  0x448u
#define ID31SRCADDR_ADDR_OFFSET                 0x448u
#define ID31SRCADDR_ADDR_MASK                   0x007FFFFFu
#define ID31SRCADDR_ADDR_SHIFT                  0u

//ID31DESTADD
#define ID31DESTADDR_REG_OFFSET                 0x44Cu
#define ID31DESTADDR_ADDR_OFFSET                0x44Cu
#define ID31DESTADDR_ADDR_MASK                  0x007FFFFFu
#define ID31DESTADDR_ADDR_SHIFT                 0u

//ID31NEXTDESC
#define ID31NEXTDESC_REG_OFFSET                 0x450u
#define ID31NEXTDESC_ADDR_OFFSET                0x450u
#define ID31NEXTDESC_ADDR_MASK                  0x007FFFFFu
#define ID31NEXTDESC_ADDR_SHIFT                 0u


//STR0ADD
#define STR0ADDR_REG_OFFSET                     0x460u

#define STR0ADDR_REG_OFFSET                     0x460u
#define STR0ADDR_REG_MASK                       0xFFFFFFFFu
#define STR0ADDR_REG_SHIFT                      0u

//STR1ADD
#define STR1ADDR_REG_OFFSET                     0x464u

#define STR1ADDR_REG_OFFSET                     0x464u
#define STR1ADDR_REG_MASK                       0xFFFFFFFFu
#define STR1ADDR_REG_SHIFT                      0u

//STR2ADD
#define STR2ADDR_REG_OFFSET                     0x468u

#define STR2ADDR_REG_OFFSET                     0x468u
#define STR2ADDR_REG_MASK                       0xFFFFFFFFu
#define STR2ADDR_REG_SHIFT                      0u

//STR3ADD
#define STR3ADDR_REG_OFFSET                     0x46Cu

#define STR3ADDR_REG_OFFSET                     0x46Cu
#define STR3ADDR_REG_MASK                       0xFFFFFFFFu
#define STR3ADDR_REG_SHIFT                      0u

#ifdef __cplusplus
}
#endif

#endif /* CORE_AXI4DMACTRL_REGS_H_ */
