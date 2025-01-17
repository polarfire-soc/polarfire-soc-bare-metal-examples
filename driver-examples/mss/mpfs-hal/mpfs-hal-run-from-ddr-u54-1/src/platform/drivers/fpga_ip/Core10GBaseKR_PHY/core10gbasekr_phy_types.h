/**
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core10gbasekr_phy_types.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core10GBaseKR_PHY Types
 *
 */

#ifndef CORE10GBASEKR_PHY_TYPES_H_
#define CORE10GBASEKR_PHY_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hal/hal.h"

/**
 * The phy10gkr_error_t enumeration acts as an error lookup.
 *
 * - PHY10GKR_ERR_USER_CONFIG implies that the config structure is Null.
 * - PHY10GKR_ERR_NO_XCVR implies that there is no pointer to an instance of a XCVR.
 * - PHY10GKR_ERR_XCVR_API_FUCNTION_POINTER implies that at least one XCVR API
 *   function pointer is null.
 */
typedef enum __phy_error
{
    PHY10GKR_ERR_USER_CONFIG = 1,
    PHY10GKR_ERR_NO_XCVR = 2,
    PHY10GKR_ERR_XCVR_API_FUNCTION_POINTER = 3,
} phy10gkr_error_t;

/**
 * The phy10gkr_lane_los_t enumeration identifies the state of the lane loss
 * detection.
 */
typedef enum __lane_los_state
{
    LANE_LOS_LOCK_TO_DATA = 0,
    LANE_LOS_LOCK_TO_REF = 1
} phy10gkr_lane_los_t;

/**
 * The phy10gkr_timer_t structure describes the start and end time instance of
 * the measurement with the PHY driver.
 */
typedef struct __phy_timer
{
    uint32_t start;
    uint32_t end;
} phy10gkr_timer_t;

/**
 * The phy10gkr_an_state_t enumeration is used to identify the state of the auto-
 * negotiation arbitration state machine, which is implemented by the
 * Core10GBaseKR_PHY.
 */
typedef enum __an_state
{
    ST_AUTO_NEG_ENABLE = 0x0,
    ST_TRANSMIT_DISABLE = 0x1,
    ST_ABILITY_DETECT = 0x2,
    ST_ACKNOWLEDGE_DETECT = 0x3,
    ST_COMPLETE_ACKNOWLEDEGE = 0x4,
    ST_AN_GOOD_CHECK = 0x5,
    ST_AN_GOOD = 0x6,
    ST_NEXT_PAGE_WAIT = 0x7,
    ST_NEXT_PAGE_WAIT_TX_IDLE = 0x8,
    ST_LINK_STATUS_CHECK = 0x9,
    ST_PARALLEL_DETECTION_FAULT = 0xA
} phy10gkr_an_state_t;

/**
 * The phy10gkr_an_status_t enumeration specifies the status of auto-negotiation.
 */
typedef enum __an_status
{
    STATUS_AN_INCOMPLETE,
    STATUS_AN_COMPLETE
} phy10gkr_an_status_t;

/**
 * The phy10gkr_an_api_state_t enumeration identifies the state of the
 * auto-negotiation state machine API.
 */
typedef enum __api_status
{
    AN_API_SM_INIT,
    AN_API_SM_STATUS_UPDATE
} phy10gkr_an_api_state_t;

/**
 * The phy10gkr_an_instance_t struct describes an instance of the
 * auto-negotiation parameters.
 */
typedef struct __an_instance
{
    phy10gkr_an_state_t state;
    phy10gkr_an_api_state_t api_state;

    uint32_t complete_cnt;
    phy10gkr_an_status_t status;

    uint64_t adv_ability;
    uint64_t lp_bp_adv_ability;
} phy10gkr_an_instance_t;

/**
 * The phy10gkr_lt_state_t enumeration identifies the link training state machine
 * state implemented by the Core10GBaseKr_PHY IP block.
 */
typedef enum __lt_state
{
    LT_STATE_IDLE = 0,
    LT_STATE_INITIALIZE = 1,
    LT_STATE_SEND_TRAINING = 3,
    LT_STATE_TRAIN_LOCAL = 2,
    LT_STATE_TRAIN_REMOTE = 6,
    LT_STATE_LINK_READY = 7,
    LT_STATE_SEND_DATA = 5,
    LT_STATE_FAILURE = 4
} phy10gkr_lt_state_t;

/**
 * The phy10gkr_lt_link_status_t enumeration identifies the status and state of
 * the link with the link partner.
 */
typedef enum __lt_link_status
{
    STATUS_LT_INCOMPLETE,
    STATUS_LT_COMPLETE,
    STATUS_LT_LINK_MAINTAINED,
    STATUS_LT_FAILURE
} phy10gkr_lt_link_status_t;

/**
 * The phy10gkr_lt_api_state_t enumeration identifies the link training API state
 * machine state.
 */
typedef enum __lt_api_status
{
    LT_API_SM_INIT,
    LT_API_SM_STATUS_UPDATE
} phy10gkr_lt_api_state_t;

/**
 * The phy10gkr_coeff_update_status_t enumeration identifies the status of the
 * coefficient sweep algorithm.
 */
typedef enum coeff_update_status
{
    SWEEP_NOT_STARTED,
    SWEEP_START,
    SWEEP_INCOMPLETE,
    SWEEP_COMPLETE,
} phy10gkr_coeff_update_status_t;

/**
 * The phy10gkr_calirbation_request_t enumeration identifies the initial conditions of a device. For
 * example, the local device will calibrate using a preset request.
 */
typedef enum __calirbation_request
{
    C10GBKR_LT_PRESET = 0U,
    C10GBKR_LT_INITALISE = 1U
} phy10gkr_calirbation_request_t;

/**
 * The phy10gkr_coeff_status_report_t enumeration identifies the link training
 * status report update.
 */
typedef enum coeff_status_report
{
    LT_COEFF_STATUS_NOT_UPDATED = 0U,
    LT_COEFF_STATUS_UPDATED = 1U,
    LT_COEFF_STATUS_MIN = 2U,
    LT_COEFF_STATUS_MAX = 3U
} phy10gkr_coeff_status_report_t;

/**
 * The phy10gkr_tx_equalizer_tap_t enumeration specifies the three different transmitter taps.
 */
typedef enum tx_equalizer_tap
{
    PRE_TAP,
    MAIN_TAP,
    POST_TAP
} phy10gkr_tx_equalizer_tap_t;

/**
 * The phy10gkr_tap_cal_state_t enumeration specifies the state of the link partner calibration
 * algorithm.
 */
typedef enum tap_cal_state
{
    TAP_MAX_CAL,
    TAP_MIN_CAL,
    TAP_OPTIMISE_CAL
} phy10gkr_tap_cal_state_t;

/**
 * The phy10gkr_local_rxcvr_lock_t enumeration specifies the condition of the
 * link partner calibration algorithm. During training, when the link partner has
 * been calibrated, the local receiver ready lock is locked and the status report
 * is updated to notify the link partner.
 */
typedef enum local_rxcvr_lock
{
    LOCAL_RXCVR_UNLOCKED = 0,
    LOCAL_RXCVR_LOCKED = 1
} phy10gkr_local_rxcvr_lock_t;

/**
 * The phy10gkr_coeff_update_t struct describes an instance of the link training
 * coefficient update. This structure supports calibrating the link partners
 * transmitter taps.
 */
typedef struct coeff_update
{
    uint32_t cnt;
    uint32_t inc_cnt;
    uint32_t dec_cnt;

    phy10gkr_tap_cal_state_t lp_tap_cal_state;

    uint32_t optimal_index;
    uint32_t optimal_cnt;
} phy10gkr_coeff_update_t;

/**
 * The phy10gkr_lt_instance_t struct describes an instance of the link training
 * parameters.
 */
typedef struct __lt_instance
{
    phy10gkr_lt_state_t state;
    phy10gkr_lt_api_state_t api_state;
    phy10gkr_lt_link_status_t status;

    phy10gkr_timer_t timer;

    uint32_t fail_cnt;
    uint32_t complete_cnt;
    uint32_t tx_equ_cnt;
    uint32_t rx_cal_cnt;
    uint32_t sig_cnt;
    uint32_t rcvr_cnt;
    uint32_t sm_cycle_cnt;

    phy10gkr_tx_equalizer_tap_t lp_cal_sweep_state;
    phy10gkr_coeff_update_t main;
    phy10gkr_coeff_update_t post;
    phy10gkr_coeff_update_t pre;

    phy10gkr_local_rxcvr_lock_t local_rxcvr;
} phy10gkr_lt_instance_t;

/**
 * The phy10gkr_state_t enumeration identifies the state of the 10GBASE-KR state
 * machine.
 */
typedef enum __c10gbkr_state
{
    AN_SERDES_CONFIG,
    AN_SM,
    LT_SERDES_CONFIG,
    LT_SM,
    LINK_ESTABLISHED_CHECK
} phy10gkr_state_t;

/**
 * The phy10gkr_status_t enumeration identifies the status of the 10GBASE-KR
 * state machine.
 *
 * This enumeration can identify failures encountered by the 10GBASE-KR
 * algorithm.
 */
typedef enum __c10gbkr_status
{
    AN_SERDES_CONFIGURATION = 1,
    AN_IN_PROGRESS = 2,
    AN_COMPLETE = 3,
    LT_SERDES_CONFIGURATION = 4,
    LT_SERDES_CAL_FAILURE = 5,
    LT_SERDES_CAL_COMPLETE = 6,
    LT_IN_PROGRESS = 7,
    LT_FAILURE = 8,
    LINK_BROKEN = 9,
    LINK_ESTABLISHED = 0
} phy10gkr_status_t;

/**
 * The phy10gkr_xcvr_api_t structure identifies the required XCVR APIs that this
 * driver requires to complete IEEE802.3 Clause 72 and 73. All function pointers
 * require a return value of type uint8_t. Each function that the pointers point to
 * require void pointer to an XCVR instance, with the exception of tx_equalization
 * which requires three additional parameters, absolute tap coefficients.
 *
 * - init: Initialise the XCVR which is implemented in the hardware design.
 * - auto_neg_data_rate: Configure the XCVR for auto-negotiation.
 * - link_training_data_rate: Configure the XCVR for link training.
 * - cdr_lock: Check if CDR is locked and return success or failure, where 0 is
 *   success.
 * - ctle_cal: Start CTLE calibration.
 * - ctle_cal_status: Check if CTLE is complete and return success or failure, where 0 is
 *   success.
 * - dfe_cal: Start DFE calibration.
 * - dfe_cal_status: Check if DFE is complete and return success or failure, where 0 is
 *   success.
 * - reset_pcs_rx: Reset the XCVR PCS RX path.
 * - tx_equalization: Set the XCVR coefficients which are passed as parameters,
 *   where tx_main_tap implies C(0), tx_post_tap implies C(+1) and tx_pre_tap implies
 *   C(-1). These coefficients are absolute but the XCVR may require the signed
 *   value.
 */
typedef struct __phy10gkr_xcvr_api
{
    uint8_t (*init)(void *xcvr);
    uint8_t (*auto_neg_data_rate)(void *xcvr);
    uint8_t (*link_training_data_rate)(void *xcvr);
    uint8_t (*cdr_lock)(void *xcvr);
    uint8_t (*ctle_cal)(void *xcvr);
    uint8_t (*ctle_cal_status)(void *xcvr);
    uint8_t (*dfe_cal)(void *xcvr);
    uint8_t (*dfe_cal_status)(void *xcvr);
    uint8_t (*reset_pcs_rx)(void *xcvr);
    uint8_t (*tx_equalization)(void *xcvr,
                               uint32_t tx_main_tap,
                               uint32_t tx_post_tap,
                               uint32_t tx_pre_tap);
} phy10gkr_xcvr_api_t;

/**
 * The phy10gkr_cfg_t struct describes an instance of the Core10GBaseKR_PHY
 * configuration parameters.
 */
typedef struct __phy10gkr_cfg
{
    /* XCVR APIs */
    phy10gkr_xcvr_api_t xcvr_api;

    /* PHY Configurations */
    uint32_t fec_request;

    /* Link Training */
    uint32_t rx_calibration_request;

    uint32_t main_preset_tap_coeff;
    uint32_t post_preset_tap_coeff;
    uint32_t pre_preset_tap_coeff;

    uint32_t main_initialize_tap_coeff;
    uint32_t post_initialize_tap_coeff;
    uint32_t pre_initialize_tap_coeff;

    uint32_t main_max_tap_ceoff;
    uint32_t main_min_tap_ceoff;
    uint32_t post_max_tap_ceoff;
    uint32_t post_min_tap_ceoff;
    uint32_t pre_max_tap_ceoff;
    uint32_t pre_min_tap_ceoff;
} phy10gkr_cfg_t;

/**
 * The phy10gkr_instance_t struct describes an instance of the Core10GBaseKR_PHY
 * parameters.
 */
typedef struct __phy10gkr_instance
{
    addr_t base_addr;
    addr_t an_base_addr;
    addr_t lt_base_addr;
    addr_t tx_ctrl_base_addr;
    addr_t rx_status_base_addr;

    phy10gkr_an_instance_t an;
    phy10gkr_lt_instance_t lt;

    phy10gkr_state_t c10gbkr_state;
    phy10gkr_status_t c10gbkr_status;

    uint32_t serdes_id;
    uint32_t serdes_lane_id;

    uint32_t fec_configured; /* FEC configured in RTL */
    uint32_t fec_negotiated; /* FEC negotiated with link partner */

    /* Configurations */
    void *xcvr;
    phy10gkr_xcvr_api_t xcvr_api;

    uint32_t fec_request; /* Request FEC */
    phy10gkr_calirbation_request_t rx_calibration_request;

    uint32_t main_preset_tap_coeff;
    uint32_t post_preset_tap_coeff;
    uint32_t pre_preset_tap_coeff;

    uint32_t main_initialize_tap_coeff;
    uint32_t post_initialize_tap_coeff;
    uint32_t pre_initialize_tap_coeff;

    uint32_t main_max_tap_ceoff;
    uint32_t main_min_tap_ceoff;
    uint32_t post_max_tap_ceoff;
    uint32_t post_min_tap_ceoff;
    uint32_t pre_max_tap_ceoff;
    uint32_t pre_min_tap_ceoff;
} phy10gkr_instance_t;

#ifdef __cplusplus
}
#endif

#endif /* CORE10GBASEKR_PHY_TYPES_H_ */
