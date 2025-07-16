/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_pdma_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Register bit offsets and masks definitions for PolarFire SoC
 * Microprocessor Subsystem (MSS) PDMA
 *
 */

#ifndef MSS_PDMA_REGS_H_
#define MSS_PDMA_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * MSS PDMA register map
 */
#define PDMA_REG_BASE                                  0x03000000U

/* PDMA Channel Register offset.*/
#define PDMA_CHL_REG_OFFSET                            0x1000U

/* MSS PDMA channel reg run bit mask */
#define MASK_PDMA_CONTROL_RUN                          0x02U

/* MSS PDMA channel reg claim bit mask */
#define MASK_CLAIM_PDMA_CHANNEL                        0x01U

/* MSS PDMA channel reg doneIE bit mask */
#define MASK_PDMA_ENABLE_DONE_INT                      0x00004000U

/* MSS PDMA channel reg errorIE bit mask */
#define MASK_PDMA_ENABLE_ERR_INT                       0x00008000U

#define SHIFT_CH_CONFIG_WSIZE                          24U
#define SHIFT_CH_CONFIG_RSIZE                          28U

/* MSS PDMA next config reg repeat bit mask */
#define MASK_REPEAT_TRANSCTION                         0x04U

/* MSS PDMA next config reg order bit mask */
#define MASK_FORCE_ORDERING                            0x08U

/* MSS PDMA transfer error mask */
#define MASK_PDMA_TRANSFER_ERROR                       0x80000000U

/* MSS PDMA transfer done mask */
#define MASK_PDMA_TRANSFER_DONE                        0x40000000U

#endif /* MSS_PDMA_REGS_H_ */
