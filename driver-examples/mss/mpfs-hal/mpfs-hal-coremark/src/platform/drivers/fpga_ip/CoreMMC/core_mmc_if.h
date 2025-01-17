/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_mmc_if.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreMMC interface header file
 *
 * This CoreMMC Interface header file provides access to functions which
 * configure and program the eMMC Slave device to allow data transfers to be
 * performed with the eMMc Master.
 *
 */

#ifndef __CORE_MMC_IF_H
#define __CORE_MMC_IF_H

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/cpu_types.h"

#else
#include "cpu_types.h"
#endif

#ifdef __cplusplus
extern "C"
#endif

#define NO_ERROR 0u
#define NO_STATUS 0u
#define CLEAR_BIT 1u
#define CMD_SENT 1u
#define RESP_RX 1u
#define READY 1u
#define ISR_RESP_REG_MASK 0x78u /** No user error, RSP received */

/**
  The `cif_response` type specifies the status of the MMC command transfer
  to the eMMC device. A value of this type is returned by `cif_send_cmd()`.
 */

typedef enum {
  TRANSFER_IF_FAIL = 0u,
  TRANSFER_IF_SUCCESS = 1u,
  DEVICE_BUSY = 2u
} cif_response_t;

typedef enum {
  CHECK_IF_CMD_SENT_POLL = 0u,
  CHECK_IF_CMD_SENT_INT = 1u,
  CHECK_IF_CMD_SENT_NO = 2u
} cmd_response_check_options;

/**
  `cif_send_cmd()` handles the interface level command and response
  data for communicating with the eMMC device. This function issues
  configuration and control commands to the device, waits on the status
  register to update, indicating there was a response received (where expected)
  and parses the response to determine the outcome of the transfer.

  @param addr
  The `addr` parameter specifies the base address of the instance of CoreMMC in
  hardware which the command and argument are to be transmitted to.

  @param cmd_arg
  The `cmd_arg` parameter specifies the MMC argument to be passed to the eMMC
  device.

  @param cmd_type
  The `cmd_type` parameter specifies the MMC Command type to be passed to the
  eMMC device.

  @return
  This function returns a value of `cif_response_t` type representing the
  outcome of the transfer.
  If this return value indicates that the eMMC device is busy, subsequent
  actions must be taken to ensure that a command is not issued until the device
  returns to idle.

  @example
  @code
    #define MMC_DW_CSD        0x03B70300u
    #define MMC_CMD_SWITCH    6u
    cif_response_t response_status

    response_status = cif_send_cmd(BASE_ADDR, MMC_DW_CSD, MMC_CMD_SWITCH)

    while(DEVICE_BUSY == response_status)
    {
        response_status = cif_send_cmd(BASE_ADDR,
                                       RCA_VALUE,
                                       MMC_CMD_SEND_STATUS)
    }
  @endcode
 */
cif_response_t cif_send_cmd(addr_t addr, uint32_t cmd_arg, uint8_t cmd_type);

/**
  `send_mmc_cmd()` transfers the MMC bus command and argument to the MMC device
  and waits until the core indicates that the command has been transferred
  successfully.

  @param addrr
  The addr parameter specifies the base address of the instance of CoreMMC in
  hardware which the command and argument are to be transmitted to.

  @param cmd_arg
  The `cmd_arg` parameter specifies the MMC argument to be passed to the eMMC
  device.

  @param cmd_type
  The `cmd_type` parameter specifies the MMC Command type to be passed to the
  eMMC device.

  @param cmd_option
  The `cmd_option` parameter specifies if the function checks if CoreMMC has
  sent the command or not before returning. There is no need to check if you
  are expecting a response, just check for the response.

  @return
  This function does not return any value.

  @example
  Send Command to the MMC device.

  @code
    send_mmc_cmd(this_mmc->base_addr,    RCA_VALUE,    MMC_CMD_13_SEND_STATUS,
                 CHECK_IF_CMD_SENT_NO)
  @endcode
 */
void send_mmc_cmd(addr_t addr, uint32_t cmd_arg, uint8_t cmd_type,
                  cmd_response_check_options cmd_option);

#ifdef __cplusplus
}
#endif

#endif /* __CORE_MMC_IF_H */
