/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_mmc_if.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreMMC interface driver file
 *
 * This CoreMMC Interface driver provides functions for transferring
 * configuration and programming commands to the eMMC device. Functions
 * contained within the CoreMMC Interface Driver are accessed through the
 * core_mmc_if.h header file.
 *
 */

#include "core_mmc_if.h"
#include "core_mmc_cmd_defs.h"
#include "core_mmc_regs.h"

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#endif

/**
 * Local Function Prototypes
 */
static cif_response_t response_1_parser(addr_t addr);

/**
 * cif_send_cmd()
 * See ".h" for details of how to use this function.
 */
cif_response_t cif_send_cmd(addr_t addr, uint32_t cmd_arg, uint8_t cmd_type) {
  cif_response_t ret_status = TRANSFER_IF_FAIL;

  /* Make sure flags cleared */
  HAL_set_32bit_reg(addr, ICR, CLEAR_ALL_32);

  /* Transfer the Command to the MMC device */
  send_mmc_cmd(addr, cmd_arg, cmd_type, CHECK_IF_CMD_SENT_POLL);

  /* No responses for CMD 0,4,15 */
  if ((MMC_CMD_0_GO_IDLE_STATE != cmd_type) &&
      (MMC_CMD_4_SET_DSR != cmd_type) &&
      (MMC_CMD_15_GOTO_INACTIVE_STATE != cmd_type)) {
    /*
     * Poll the ISR for activity. If change detected due to a response being
     * received, determine the response type and parse it accordingly.
     * Otherwise, alert the calling function that an error occurred.
     */
    volatile uint32_t trans_status_isr;

    do {
      trans_status_isr = HAL_get_32bit_reg(addr, ISR);
    } while ((!(ISR_RESP_REG_MASK & trans_status_isr)) &&
             (!(ISR_BISR_MBSIR_ERR & trans_status_isr)));

    if (RESP_RX == HAL_get_8bit_reg_field(addr, ISR_SRRI)) {
      /* If the response is an R1/B response */
      if ((MMC_CMD_1_SEND_OP_COND != cmd_type) &&
          (MMC_CMD_2_ALL_SEND_CID != cmd_type) &&
          (MMC_CMD_9_SEND_CSD != cmd_type) &&
          (MMC_CMD_10_SEND_CID != cmd_type) &&
          (MMC_CMD_39_FAST_IO != cmd_type) &&
          (MMC_CMD_40_GO_IRQ_STATE != cmd_type)) {
        ret_status = response_1_parser(addr);
      } else {
        ret_status = TRANSFER_IF_SUCCESS;
      }
    }
  } else {
    ret_status = TRANSFER_IF_SUCCESS;
  }

  /* Clear flags for the next time */
  HAL_set_32bit_reg(addr, ICR, CLEAR_ALL_32);

  return (ret_status);
}

/**
 * send_mmc_cmd()
 * See ".h" for details of how to use this function.
 */
void send_mmc_cmd(addr_t addr, uint32_t cmd_arg, uint8_t cmd_type,
                  cmd_response_check_options cmd_option) {
  HAL_set_8bit_reg(addr, CR0, cmd_type);
  HAL_set_8bit_reg(addr, CR1, (uint_fast8_t)(cmd_arg >> 24u));
  HAL_set_8bit_reg(addr, CR2, (uint_fast8_t)(cmd_arg >> 16u));
  HAL_set_8bit_reg(addr, CR3, (uint_fast8_t)(cmd_arg >> 8u));
  HAL_set_8bit_reg(addr, CR4, (uint_fast8_t)cmd_arg); /* when CR4 is written to,
                                                         transfer is initiated
                                                         by coreMMC */

  switch (cmd_option) {
  case CHECK_IF_CMD_SENT_POLL: /* only need to wait around if expecting no
                                  response */
    while (CMD_SENT != HAL_get_8bit_reg_field(addr, ISR_SCSI)) {
      ;
    }
    break;
  case CHECK_IF_CMD_SENT_INT:
    break;
  case CHECK_IF_CMD_SENT_NO: /* use when expecting a response */
    /* No check- will be checked when response received */
    break;
  default:
    /* No check */
    break;
  }
}

/**
 * The response_1_parser() returns the contents of the Card Status Register.
 * This function checks that none of the error fields are set within the CSR
 * and the status of the READY_FOR_DATA flag (Bit 8).
 */
static cif_response_t response_1_parser(addr_t addr) {
  cif_response_t ret_status = TRANSFER_IF_FAIL;
  uint32_t response;

  response = HAL_get_32bit_reg(addr, RR1);
  if (!(R1_ERR_RESP_MASK & response)) {
    if (READY_FOR_DATA & response) {
      ret_status = TRANSFER_IF_SUCCESS;
    } else {
      ret_status = DEVICE_BUSY;
    }
  }

  return (ret_status);
}
