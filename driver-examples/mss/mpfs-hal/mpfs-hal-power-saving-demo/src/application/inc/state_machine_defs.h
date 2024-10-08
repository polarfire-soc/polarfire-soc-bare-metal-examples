/*******************************************************************************
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software
 *
 */

/*******************************************************************************
 * @file state_machine_defs.h
 * @author Microchip-FPGA Embedded Systems Solutions
 * @brief Defines for the the state machine.
 *
 */

#ifndef _STATE_MACHINE_DEFS_H_
#define _STATE_MACHINE_DEFS_H_ 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Request
 */
#define REQ_LOW_FREQ         1U
#define REQ_MID_FREQ         2U
#define REQ_NORMAL_FREQ      4U
#define REQ_RESET            8U

#define STATUS_HIGH_PWR             0x00000000U
#define STATUS_LOW_PWR              0x00000001U

#define FLAG_REQ_LOW_PWR            0x00000010U
#define FLAG_REQ_HIGH_PWR           0x00000020U

#define FLAG_CMD_LOW_PWR            0x00000100U
#define FLAG_CMD_HIGH_PWR           0x00000200U

#define ACK_FLAG                    0x00000008U
#define MAX_DDR_ADDRESS             0x80001000UL

typedef enum FS_SM_0_
{
    FS_SM_0_INIT = 1,
    FS_SM_0_DEFAULT_STATE,
    FS_SM_0_START_APP,
    FS_SM_0_LOW_FREQ,
    FS_SM_0_NORMAL_FREQ,
    FS_SM_0_REQUEST_FROM_OTHER_HART,

} FS_SM_0;

typedef struct FS_DATA_0_
{
    FS_SM_0 sm;
    uint32_t current_state;
    uint32_t request_state;
    uint32_t previous_state;
    uint64_t rx_request_time;
    uint64_t last_change_time;
} FS_DATA_0;

typedef enum FS_SM_1_
{
    FS_SM_1_INIT = 1,
    FS_SM_1_WAIT_START,
    FS_SM_1_RUN_APP,
    FS_SM_1_LOW_FREQ,
    FS_SM_1_NORMAL_FREQ,
    FS_SM_1_REQUEST_FROM_OTHER_HART,
} FS_SM_1;

typedef struct FS_DATA_1_
{
    FS_SM_1 sm;
    uint32_t current_state;
    uint32_t request_state;
    uint32_t previous_state;
    uint64_t rx_request_time;
    uint64_t last_change_time;
} FS_DATA_1;

#ifdef __cplusplus
}
#endif

#endif