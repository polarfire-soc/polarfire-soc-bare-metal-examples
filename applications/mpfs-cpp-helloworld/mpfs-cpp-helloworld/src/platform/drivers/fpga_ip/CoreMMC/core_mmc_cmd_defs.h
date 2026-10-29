/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_mmc_cmd_defs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreMMC interface level header file
 *
 * This MMC Interface header file provides a subset of definitions from the eMCC
 * protocol JESD84-A441
 *
 */

#ifndef __CORE_MMC_CMD_DEFD_H
#define __CORE_MMC_CMD_DEFD_H

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/cpu_types.h"

#else
#include "cpu_types.h"
#endif

#ifdef __cplusplus
extern "C"
#endif

/**
 * MMC commands
 */
#define MMC_CMD_15_GOTO_INACTIVE_STATE 15u /** No  Rsp     */
#define MMC_CMD_4_SET_DSR 4u               /** No Rsp      */
#define MMC_CMD_0_GO_IDLE_STATE 0u         /** No  Rsp     */
#define MMC_CMD_6_SWITCH 6u                /** R1b Rsp     */
#define MMC_CMD_7_SELECT_DESELECT_CARD 7u  /** R1/R1b Rsp  */

#define MMC_CMD_3_SET_RELATIVE_ADDR 3u     /** R1  Rsp    */
#define MMC_CMD_17_READ_SINGLE_BLOCK 17u   /** R1 Rsp     */
#define MMC_CMD_18_READ_MULTIPLE_BLOCK 18u /** R1 Rsp     */
#define MMC_CMD_24_WRITE_SINGLE_BLOCK 24u  /** R1 Rsp     */
#define MMC_CMD_23_SET_BLOCK_COUNT 23u     /** R1 Rsp     */
#define MMC_CMD_25_WRITE_MULTI_BLOCK 25u   /** R1 Rsp     */
#define MMC_CMD_13_SEND_STATUS 13u         /** R1 Rsp     */
#define MMC_CMD_12_STOP_TRANSMISSION 12u   /** R1/R1b Rsp */
#define MMC_CMD_8_SEND_EXT_CSD 8u          /** R1 Rsp     */

#define MMC_CMD_2_ALL_SEND_CID 2u   /** R2 Rsp     */
#define MMC_CMD_9_SEND_CSD 9u       /** R2 Rsp     */
#define MMC_CMD_10_SEND_CID 10u     /** R2 Rsp     */
#define MMC_CMD_1_SEND_OP_COND 1u   /** R3  Rsp    */
#define MMC_CMD_39_FAST_IO 39u      /** R4  Rsp    */
#define MMC_CMD_40_GO_IRQ_STATE 40u /** R5  Rsp    */

    typedef enum R1_CARD_STATUS_RESPONSE_ {
      ADDRESS_OUT_OF_RANGE = 0x00000080u, /**< RR1 */
      ADDRESS_MISALIGN = 0x00000040u,
      BLOCK_LEN_ERROR = 0x00000020u,
      ERASE_SEQ_ERROR = 0x00000010u,
      ERASE_PARAM = 0x00000008u,
      WP_VIOLATION = 0x00000004u,
      CARD_IS_LOCKED = 0x00000002u,
      LOCK_UNLOCK_FAILED = 0x00000001u,

      COM_CRC_ERROR = 0x00008000u, /**< RR2 */
      ILLEGAL_COMMAND = 0x00004000u,
      CARD_ECC_FAILED = 0x00002000u,
      CC_ERROR = 0x00001000u,
      GEN_ERROR = 0x00000800u,
      UNDERRUN = 0x00000400u,
      OVERRUN = 0x00000200u,
      CSD_OVERWRITE = 0x00000100u,

      WP_ERASE_SKIP = 0x00800000u, /**< RR3 */
      RESERVED_14 = 0x00400000u,
      ERASE_RESET = 0x00200000u,
      CURRENT_STATE12_9_MASK = ((0x0Fu << 1) << 16),
      CURRENT_STATE_RCV =
          ((0x06u << 1) << 16), /**< in this state when rx data from eMMC */
      CURRENT_STATE_DATA =
          ((0x05u << 1) << 16), /**< in this state when sending data to eMMC */
      CURRENT_STATE_TRAN = ((0x04u << 1) << 16), /**< ready to send/receive */
      READY_FOR_DATA = 0x00010000u,

      SWITCH_ERROR = 0x80000000u, /**< RR4 */
      URGENT_BKOPS = 0x40000000u,
      APP_CMD = 0x20000000u,
      RESERVED_0_4 = 0x00000000u,
    } R1_CARD_STATUS_RESPONSE;

#define R1_ERR_RESP_MASK                                                       \
  (SWITCH_ERROR | WP_ERASE_SKIP | ERASE_RESET | COM_CRC_ERROR |                \
   ILLEGAL_COMMAND | CARD_ECC_FAILED | CC_ERROR | GEN_ERROR | UNDERRUN |       \
   OVERRUN | CSD_OVERWRITE | ADDRESS_OUT_OF_RANGE | ADDRESS_MISALIGN |         \
   BLOCK_LEN_ERROR | ERASE_SEQ_ERROR | ERASE_PARAM | WP_VIOLATION |            \
   LOCK_UNLOCK_FAILED)

#ifdef __cplusplus
}
#endif

#endif /* __CORE_MMC_CMD_DEFD_H */
