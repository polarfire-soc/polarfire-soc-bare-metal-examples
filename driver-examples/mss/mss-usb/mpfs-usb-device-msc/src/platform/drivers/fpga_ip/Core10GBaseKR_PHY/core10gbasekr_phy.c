/**
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core10gbasekr_phy.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core10GBaseKR_PHY Source
 *
 */

#include <stddef.h>
#include "core10gbasekr_phy.h"
#include "core10gbasekr_phy_link_training.h"

/*------------------------Private Definitions---------------------------------*/
#define AN_RESET                (1U)
#define AN_ENABLE               (1U)
#define AN_RESTART              (1U)

#define TX_CTRL_DATA            (0U)
#define TX_CTRL_AN              (2U)
#define TX_CTRL_LT              (3U)

#define COEFF_UPDATE_PRESET     (1U << 13U)
#define COEFF_UPDATE_INITIALIZE (1U << 12U)

#define MR_ADV_3_BIT_OFFSET     (32U)
#define MR_ADV_FEC_ABILITY      (46U)
#define MR_ADV_FEC_REQUESTED    (47U)

/*------------------------Public Variables------------------------------------*/
extern uint32_t prbs_cnt;

/*------------------------Private Function-------------------------------------*/
/**
 * This is a private function which is called to reconfigure the core with
 * default conditions, reset states, counters and algorithm attributes.
 */
static uint32_t
phy_10gbasekr_reset(phy10gkr_instance_t *this_phy)
{
    /* set data path to AN block */
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_TX_CTRL_PMA_DATA, TX_CTRL_AN);

    /* reset phy tx and rx */
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_TX_CTRL_TX_RESET, 0x1);
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_TX_CTRL_RX_RESET, 0x1);

    /* set data path to AN block */
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_TX_CTRL_PMA_DATA, TX_CTRL_AN);

    /* transmit equalization tap limit configurations */
    /* main coefficients (P0) */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MAX_MAIN_TAP,
                            this_phy->main_max_tap_ceoff);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MIN_MAIN_TAP,
                            this_phy->main_min_tap_ceoff);

    if ((C10GBKR_LT_MAIN_TAP_MAX_LIMIT != this_phy->main_max_tap_ceoff))
    {
        this_phy->main_preset_tap_coeff = this_phy->main_max_tap_ceoff;
    }

    /* post coefficients (P1) */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MAX_POST_TAP,
                            this_phy->post_max_tap_ceoff);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MIN_POST_TAP,
                            this_phy->post_min_tap_ceoff);

    if ((C10GBKR_LT_POST_TAP_MAX_LIMIT != this_phy->post_max_tap_ceoff))
    {
        this_phy->post_preset_tap_coeff = this_phy->post_max_tap_ceoff;
    }

    /* pre coefficients (P-1) */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MAX_PRE_TAP,
                            this_phy->pre_max_tap_ceoff);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MIN_PRE_TAP,
                            this_phy->pre_min_tap_ceoff);

    if ((C10GBKR_LT_PRE_TAP_MAX_LIMIT != this_phy->pre_max_tap_ceoff))
    {
        this_phy->pre_preset_tap_coeff = this_phy->pre_max_tap_ceoff;
    }

    /* set core preset conditions */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_PRESET_MAIN_TAP,
                            this_phy->main_preset_tap_coeff);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_PRESET_POST_TAP,
                            this_phy->post_preset_tap_coeff);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_PRESET_PRE_TAP,
                            this_phy->pre_preset_tap_coeff);

    /* set core initialize conditions */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_INIT_MAIN_TAP,
                            this_phy->main_initialize_tap_coeff);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_INIT_POST_TAP,
                            this_phy->post_initialize_tap_coeff);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_INIT_PRE_TAP,
                            this_phy->pre_initialize_tap_coeff);

    /* track number of fails since begining  */
    uint32_t tmp_fail_count = this_phy->lt.fail_cnt;
    uint32_t tmp_complete_count = this_phy->lt.complete_cnt;
    this_phy->lt = (phy10gkr_lt_instance_t){0};
    this_phy->lt.fail_cnt = tmp_fail_count;
    this_phy->lt.complete_cnt = tmp_complete_count;

    /* Set AN and LT api state machine to init state */
    this_phy->an.api_state = AN_API_SM_INIT;
    this_phy->lt.api_state = LT_API_SM_INIT;

    /* Set status to original condition */
    this_phy->an.status = STATUS_AN_INCOMPLETE;
    this_phy->lt.status = STATUS_LT_INCOMPLETE;

    /* clear link training tap data */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_LOCAL_RCVR_LOCKED,
                            LOCAL_RXCVR_UNLOCKED);
    this_phy->lt.local_rxcvr = LOCAL_RXCVR_UNLOCKED;

    this_phy->lt.main = (const phy10gkr_coeff_update_t){0};
    this_phy->lt.post = (const phy10gkr_coeff_update_t){0};
    this_phy->lt.pre = (const phy10gkr_coeff_update_t){0};

    this_phy->lt.lp_cal_sweep_state = MAIN_TAP;

    this_phy->lt.tx_equ_cnt = 0;
    this_phy->lt.rx_cal_cnt = 0;

    prbs_cnt = 0;

    return 0;
}

/**
 * The private function enables the auto-negotiation state machine, this enables
 * the auto-negotiation registers and then checks the status of the auto-negotiation
 * state machine to determine if auto-negotiation has complete.
 */
static void
phy_10gbasekr_an(phy10gkr_instance_t *this_phy)
{
    switch (this_phy->an.api_state)
    {
        case AN_API_SM_INIT:
            /* Reset auto-negotiation. */
            HAL_set_32bit_reg_field(this_phy->an_base_addr, C10GB_AN_RESET, AN_RESET);

            /* FEC configuration */
            this_phy->fec_negotiated = C10GBKR_FEC_NOT_NEGOTIATED;
            ;

            uint32_t mr_adv_3 =
                HAL_get_32bit_reg(this_phy->an_base_addr, C10GB_AN_MR_ADV_CAPABILITY_3);

            if (this_phy->fec_configured)
            {
                mr_adv_3 |= 1U << (MR_ADV_FEC_ABILITY - MR_ADV_3_BIT_OFFSET);

                if (this_phy->fec_request)
                {
                    mr_adv_3 |= 1U << (MR_ADV_FEC_REQUESTED - MR_ADV_3_BIT_OFFSET);
                }
            }
            else
            {
                /* clearing FEC bits */
                mr_adv_3 &= ~(1U << (MR_ADV_FEC_ABILITY - MR_ADV_3_BIT_OFFSET) |
                              1U << (MR_ADV_FEC_REQUESTED - MR_ADV_3_BIT_OFFSET));
            }
            HAL_set_32bit_reg(this_phy->an_base_addr, C10GB_AN_MR_ADV_CAPABILITY_3, mr_adv_3);

            /* Enable auto-negotiation. */
            HAL_set_32bit_reg_field(this_phy->an_base_addr, C10GB_AN_ENABLE, AN_ENABLE);

            /* Restart auto-negotiation */
            HAL_set_32bit_reg_field(this_phy->an_base_addr, C10GB_AN_RESTART, AN_RESTART);

            /* Tx control: Auto-negotiation, PCS data transmitted to Serdes */
            HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                    C10GB_TX_CTRL_PMA_DATA,
                                    TX_CTRL_AN);

            /* set link fail inhibit timer time to 500 ms*/
            HAL_set_32bit_reg(this_phy->an_base_addr,
                              C10GB_AN_LINK_FAIL_INHIBIT_TIMER,
                              C10GBKR_AN_LINK_FAIL_INHITBIT_TIMER);

            this_phy->an.adv_ability =
                (uint64_t)HAL_get_32bit_reg(this_phy->an_base_addr, C10GB_AN_MR_ADV_CAPABILITY_1) |
                ((uint64_t)HAL_get_32bit_reg(this_phy->an_base_addr, C10GB_AN_MR_ADV_CAPABILITY_2)
                 << 16) |
                ((uint64_t)HAL_get_32bit_reg(this_phy->an_base_addr, C10GB_AN_MR_ADV_CAPABILITY_3)
                 << 32);

            this_phy->an.status = STATUS_AN_INCOMPLETE;
            this_phy->an.api_state = AN_API_SM_STATUS_UPDATE;
            break;

        case AN_API_SM_STATUS_UPDATE:
            /* Get auto-negotiation state machine state */
            this_phy->an.state = HAL_get_32bit_reg_field(this_phy->an_base_addr, C10GB_AN_STATE);
            if (ST_AN_GOOD_CHECK == this_phy->an.state)
            {
                this_phy->an.complete_cnt++;

                this_phy->an.status = STATUS_AN_COMPLETE;

                this_phy->an.lp_bp_adv_ability =
                    (uint64_t)HAL_get_32bit_reg(this_phy->an_base_addr,
                                                C10GB_AN_MR_LP_BASE_PG_CAPABILITY_1) |
                    ((uint64_t)HAL_get_32bit_reg(this_phy->an_base_addr,
                                                 C10GB_AN_MR_LP_BASE_PG_CAPABILITY_2)
                     << 16) |
                    ((uint64_t)HAL_get_32bit_reg(this_phy->an_base_addr,
                                                 C10GB_AN_MR_LP_BASE_PG_CAPABILITY_3)
                     << 32);

                /* Set data path to LT block, enable LT and timers */
                HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                        C10GB_LT_MAX_WAIT_TIMER,
                                        MAX_WAIT_TIMER_500MS);

                HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                        C10GB_TX_CTRL_PMA_DATA,
                                        TX_CTRL_LT);

                /* Have both the local device and link partner advertised FEC ability? */
                if ((this_phy->fec_configured) &&
                    (this_phy->an.lp_bp_adv_ability & (uint64_t)1U << (uint64_t)MR_ADV_FEC_ABILITY))
                {
                    /* Has either device requested FEC? */
                    if ((this_phy->an.adv_ability & (uint64_t)1U
                                                        << (uint64_t)MR_ADV_FEC_REQUESTED) ||
                        (this_phy->an.lp_bp_adv_ability & (uint64_t)1U
                                                              << (uint64_t)MR_ADV_FEC_REQUESTED))
                    {
                        this_phy->fec_negotiated = C10GBKR_FEC_NEGOTIATED;
                    }
                }

                this_phy->lt.timer.start = PHY10GKR_get_current_time_ms();
            }
            break;

        default:
            HAL_ASSERT(0);
            break;
    }
}

/**
 * This private function enables the link training state machine, this enables the
 * link training registers and then runs the link training algorithm.
 *
 * The connected transceiver must have a data rate of 10 Gbps and locked to a
 * link partner with the same data rate for successful link training.
 */
void
phy_10gbasekr_lt(phy10gkr_instance_t *this_phy)
{
    uint32_t c10gbkr_status = 0;

    this_phy->lt.state = HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_TRAINING_SM);

    switch (this_phy->lt.api_state)
    {
        case LT_API_SM_INIT:
            if (C10GBKR_LT_PRESET == this_phy->rx_calibration_request)
            {
                HAL_set_32bit_reg(this_phy->lt_base_addr,
                                  C10GB_LT_CTRL,
                                  (C10GB_LT_RESTART_EN_MASK | C10GB_LT_PRESET_MASK));
            }
            else if (C10GBKR_LT_INITALISE == this_phy->rx_calibration_request)
            {
                HAL_set_32bit_reg(this_phy->lt_base_addr,
                                  C10GB_LT_CTRL,
                                  (C10GB_LT_RESTART_EN_MASK | C10GB_LT_INIT_MASK));
            }

            this_phy->lt.api_state = LT_API_SM_STATUS_UPDATE;
            break;

        case LT_API_SM_STATUS_UPDATE:
            c10gbkr_status = HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_STATUS);

            this_phy->lt.timer.end = PHY10GKR_get_current_time_ms() - this_phy->lt.timer.start;

            if ((c10gbkr_status & C10GB_LT_TRAINING_FAIL_MASK) ||
                this_phy->lt.timer.end > LT_SOFTWARE_WAIT_TIMER_MS)
            {
                this_phy->lt.fail_cnt++;
                this_phy->lt.status = STATUS_LT_FAILURE;

                /* disable lt hardware in the case that the software timer
                 * trigger the failure
                 */
                HAL_set_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_RESTART_EN, 0x0);
                break;
            }

            if (c10gbkr_status & C10GB_LT_REQ_TX_EQUAL_MASK)
            {
                uint32_t tx_main_tap;
                uint32_t tx_post_tap;
                uint32_t tx_pre_tap;

                uint32_t rcvd_coeff;

                this_phy->lt.tx_equ_cnt++;

                rcvd_coeff =
                    HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_NEW_RCVD_COEFF);

                if (rcvd_coeff & (COEFF_UPDATE_INITIALIZE | COEFF_UPDATE_PRESET))
                {
                    if (rcvd_coeff & COEFF_UPDATE_PRESET)
                    {
                        tx_main_tap = this_phy->main_preset_tap_coeff;
                        tx_post_tap =
                            this_phy->post_preset_tap_coeff - this_phy->post_max_tap_ceoff;
                        tx_pre_tap = this_phy->pre_preset_tap_coeff - this_phy->pre_max_tap_ceoff;
                    }
                    else
                    {
                        tx_main_tap = this_phy->main_initialize_tap_coeff;
                        tx_post_tap =
                            this_phy->post_initialize_tap_coeff - this_phy->post_max_tap_ceoff;
                        tx_pre_tap =
                            this_phy->pre_initialize_tap_coeff - this_phy->pre_max_tap_ceoff;
                    }
                }
                else
                {
                    tx_main_tap =
                        HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_TX_NEW_MAIN_TAP);
                    tx_post_tap =
                        HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_TX_NEW_POST_TAP) -
                        this_phy->post_max_tap_ceoff;
                    tx_pre_tap =
                        HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_TX_NEW_PRE_TAP) -
                        this_phy->pre_max_tap_ceoff;
                }

                this_phy->xcvr_api.tx_equalization(this_phy->xcvr,
                                                   tx_main_tap,
                                                   tx_post_tap,
                                                   tx_pre_tap);

                /* signal to hardware to set tx status report field */
                HAL_set_32bit_reg(
                    this_phy->lt_base_addr,
                    C10GB_LT_TX_EQUAL,
                    C10GB_LT_TX_EQUAL_PRE_DONE_MASK | C10GB_LT_TX_EQUAL_POST_DONE_MASK |
                        C10GB_LT_TX_EQUAL_MAIN_DONE_MASK | C10GB_LT_TX_EQUAL_DONE_MASK);
            }

            if ((c10gbkr_status & C10GB_LT_REQ_RX_CAL_MASK) &&
                LOCAL_RXCVR_UNLOCKED == this_phy->lt.local_rxcvr)
            {
                /* signal to hardware to set tx coefficient update field */
                HAL_set_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_RX_CAL_DONE, 1U);

                HAL_set_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_RX_CAL_DONE, 0U);

                this_phy->lt.rx_cal_cnt++;
                if (C10GBKR_LT_PRESET == this_phy->rx_calibration_request)
                {
                    link_partner_calibration_sm(this_phy);
                }
                else if (C10GBKR_LT_INITALISE == this_phy->rx_calibration_request)
                {
                    /* check status report */

                    this_phy->lt.local_rxcvr = LOCAL_RXCVR_LOCKED;

                    this_phy->lt.rcvr_cnt++;

                    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                            C10GB_LT_LOCAL_RCVR_LOCKED,
                                            LOCAL_RXCVR_LOCKED);
                }
            }

            if (c10gbkr_status & C10GB_LT_SIGNAL_DETECT_MASK)
            {
                this_phy->lt.complete_cnt++;

                /* Tx control: CL49 Data, PCS data */
                HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                        C10GB_TX_CTRL_PMA_DATA,
                                        TX_CTRL_DATA);
                this_phy->lt.sig_cnt++;
                this_phy->lt.status = STATUS_LT_COMPLETE;
                break;
            }
            break;

        default:
            HAL_ASSERT(0);
            break;
    }
    this_phy->lt.sm_cycle_cnt++;
}

/*------------------------Public Function-------------------------------------*/
/**
 * Please see core10gbasekr_phy.h for description
 */
void
PHY10GKR_cfg_struct_def_init(phy10gkr_cfg_t *cfg)
{
    phy10gkr_xcvr_api_t xcvr_api = {NULL};

    cfg->fec_request = C10GBKR_DISABLE_FEC_REQUEST;
    cfg->rx_calibration_request = C10GBKR_LT_PRESET;

    cfg->xcvr_api = xcvr_api;

    cfg->main_max_tap_ceoff = C10GBKR_LT_MAIN_TAP_MAX_LIMIT;
    cfg->main_min_tap_ceoff = C10GBKR_LT_MAIN_TAP_MIN_LIMIT;
    cfg->post_max_tap_ceoff = C10GBKR_LT_POST_TAP_MAX_LIMIT;
    cfg->post_min_tap_ceoff = C10GBKR_LT_POST_TAP_MIN_LIMIT;
    cfg->pre_max_tap_ceoff = C10GBKR_LT_PRE_TAP_MAX_LIMIT;
    cfg->pre_min_tap_ceoff = C10GBKR_LT_PRE_TAP_MIN_LIMIT;

    cfg->main_preset_tap_coeff = C10GBKR_LT_PRESET_MAIN_TAP;
    cfg->post_preset_tap_coeff = C10GBKR_LT_PRESET_POST_TAP;
    cfg->pre_preset_tap_coeff = C10GBKR_LT_PRESET_PRE_TAP;

    cfg->main_initialize_tap_coeff = C10GBKR_LT_INITIALIZE_MAIN_TAP;
    cfg->post_initialize_tap_coeff = C10GBKR_LT_INITIALIZE_POST_TAP;
    cfg->pre_initialize_tap_coeff = C10GBKR_LT_INITIALIZE_PRE_TAP;
}

/**
 * Please see core10gbasekr_phy.h for description
 */
uint32_t
PHY10GKR_init(phy10gkr_instance_t *this_phy, addr_t base_addr, phy10gkr_cfg_t *cfg, void *xcvr)
{
    /* Set Core10GBaseKR PHY Base addresses */
    this_phy->base_addr = base_addr;
    this_phy->an_base_addr = this_phy->base_addr | C10GB_AN_BASE_OFFSET;
    this_phy->lt_base_addr = this_phy->base_addr | C10GB_LT_BASE_OFFSET;
    this_phy->tx_ctrl_base_addr = this_phy->base_addr | C10GB_TX_CTRL_BASE_OFFSET;
    this_phy->rx_status_base_addr = this_phy->base_addr | C10GB_RX_STATUS_BASE_OFFSET;

    /* User config error handling */
    if ((NULL == cfg))
    {
        HAL_ASSERT(0);
        return PHY10GKR_ERR_USER_CONFIG; /* Release build error */
    }

    if (NULL == xcvr)
    {
        HAL_ASSERT(0);
        return PHY10GKR_ERR_NO_XCVR; /* Release build error */
    }

    /* setting configurations */
    this_phy->xcvr = xcvr;
    this_phy->xcvr_api = cfg->xcvr_api;

    if (this_phy->xcvr_api.init == NULL || this_phy->xcvr_api.auto_neg_data_rate == NULL ||
        this_phy->xcvr_api.link_training_data_rate == NULL || this_phy->xcvr_api.cdr_lock == NULL ||
        this_phy->xcvr_api.ctle_cal == NULL || this_phy->xcvr_api.ctle_cal_status == NULL ||
        this_phy->xcvr_api.dfe_cal == NULL || this_phy->xcvr_api.dfe_cal_status == NULL ||
        this_phy->xcvr_api.reset_pcs_rx == NULL || this_phy->xcvr_api.tx_equalization == NULL)
    {
        /* function pointer assignment error  */
        HAL_ASSERT(0);
        return PHY10GKR_ERR_XCVR_API_FUNCTION_POINTER; /* Release build error */
    }

    /* Check if FEC configured in the IP, advertise ability if it exists */
    if (HAL_get_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_FEC_USE_CFG) != 0)
    {
        this_phy->fec_configured = C10GBKR_FEC_ABILITY;
    }
    else
    {
        this_phy->fec_configured = C10GBKR_NO_FEC_ABILITY;
    }

    this_phy->fec_request = cfg->fec_request;
    this_phy->rx_calibration_request = cfg->rx_calibration_request;

    this_phy->main_max_tap_ceoff = cfg->main_max_tap_ceoff;
    this_phy->main_min_tap_ceoff = cfg->main_min_tap_ceoff;
    this_phy->post_max_tap_ceoff = cfg->post_max_tap_ceoff;
    this_phy->post_min_tap_ceoff = cfg->post_min_tap_ceoff;
    this_phy->pre_max_tap_ceoff = cfg->pre_max_tap_ceoff;
    this_phy->pre_min_tap_ceoff = cfg->pre_min_tap_ceoff;

    this_phy->main_preset_tap_coeff = cfg->main_preset_tap_coeff;
    this_phy->post_preset_tap_coeff = cfg->post_preset_tap_coeff;
    this_phy->pre_preset_tap_coeff = cfg->pre_preset_tap_coeff;

    this_phy->main_initialize_tap_coeff = cfg->main_initialize_tap_coeff;
    this_phy->post_initialize_tap_coeff = cfg->post_initialize_tap_coeff;
    this_phy->pre_initialize_tap_coeff = cfg->pre_initialize_tap_coeff;

    phy_10gbasekr_reset(this_phy);

    this_phy->xcvr_api.init(this_phy->xcvr);

    return 0;
}

/**
 * Please see core10gbasekr_phy.h for description
 */
uint32_t
PHY10GKR_10gbasekr_sm(phy10gkr_instance_t *this_phy)
{
    if (this_phy == NULL)
    {
        HAL_ASSERT(0);
    }

    switch (this_phy->c10gbkr_state)
    {
        case AN_SERDES_CONFIG:
            this_phy->c10gbkr_status = AN_SERDES_CONFIGURATION;
            phy_10gbasekr_reset(this_phy);
            HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                    C10GB_TX_CTRL_XCVR_LOS,
                                    LANE_LOS_LOCK_TO_REF);
            this_phy->xcvr_api.auto_neg_data_rate(this_phy->xcvr);
            this_phy->c10gbkr_state = AN_SM;
            break;

        case AN_SM:
            this_phy->c10gbkr_status = AN_IN_PROGRESS;

            phy_10gbasekr_an(this_phy);
            if (STATUS_AN_COMPLETE == this_phy->an.status)
            {
                this_phy->c10gbkr_status = AN_COMPLETE;

                this_phy->c10gbkr_state = LT_SERDES_CONFIG;
            }
            break;

        case LT_SERDES_CONFIG:
            this_phy->c10gbkr_status = LT_SERDES_CONFIGURATION;
            HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                    C10GB_TX_CTRL_XCVR_LOS,
                                    LANE_LOS_LOCK_TO_DATA);
            this_phy->xcvr_api.link_training_data_rate(this_phy->xcvr);

            while (this_phy->xcvr_api.cdr_lock(this_phy->xcvr) != 0)
            {
                this_phy->lt.timer.end = PHY10GKR_get_current_time_ms() - this_phy->lt.timer.start;

                /* determine if hardware or software timed out */
                if (this_phy->lt.timer.end >= LT_SOFTWARE_WAIT_TIMER_MS ||
                    HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_TRAINING_FAIL) == 1)
                {
                    this_phy->c10gbkr_status = LT_SERDES_CAL_FAILURE;
                    break;
                }
            }

            if (this_phy->c10gbkr_status != LT_SERDES_CAL_FAILURE)
            {
                this_phy->xcvr_api.ctle_cal(this_phy->xcvr);

                while (this_phy->xcvr_api.ctle_cal_status(this_phy->xcvr) != 0)
                {
                    this_phy->lt.timer.end =
                        PHY10GKR_get_current_time_ms() - this_phy->lt.timer.start;
                    if (this_phy->lt.timer.end >= LT_SOFTWARE_WAIT_TIMER_MS ||
                        HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_TRAINING_FAIL) ==
                            1)
                    {
                        this_phy->c10gbkr_status = LT_SERDES_CAL_FAILURE;
                        break;
                    }
                }
            }

            if (this_phy->c10gbkr_status != LT_SERDES_CAL_FAILURE)
            {
                this_phy->c10gbkr_status = LT_SERDES_CAL_COMPLETE;
                this_phy->c10gbkr_state = LT_SM;
            }
            else
            {
                this_phy->c10gbkr_state = AN_SERDES_CONFIG;
            }

            break;

        case LT_SM:
            this_phy->c10gbkr_status = LT_IN_PROGRESS;
            phy_10gbasekr_lt(this_phy);

            if (STATUS_LT_FAILURE == this_phy->lt.status)
            {
                /* reset the state machine for next time its called */
                this_phy->c10gbkr_state = AN_SERDES_CONFIG;

                this_phy->c10gbkr_status = LT_FAILURE;
            }
            else if (STATUS_LT_COMPLETE == this_phy->lt.status)
            {
                this_phy->c10gbkr_state = LINK_ESTABLISHED_CHECK;
                this_phy->c10gbkr_status = LINK_ESTABLISHED;
            }
            break;

        case LINK_ESTABLISHED_CHECK:
            if (this_phy->xcvr_api.cdr_lock(this_phy->xcvr) != 0)
            {
                /* reset the state machine for next time its called */
                this_phy->c10gbkr_state = AN_SERDES_CONFIG;

                this_phy->c10gbkr_status = LINK_BROKEN;
            }
            else
            {
                this_phy->c10gbkr_status = LINK_ESTABLISHED;
            }
            break;

        default:
            HAL_ASSERT(0);
            break;
    }
    return this_phy->c10gbkr_status;
}

uint8_t
PHY10GKR_get_ip_version(phy10gkr_instance_t *this_phy,
                        uint32_t *major,
                        uint32_t *minor,
                        uint32_t *sub)
{
    if (this_phy == NULL)
    {
        HAL_ASSERT(0);
        return 1; /* release build error */
    }

    *major = HAL_get_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_IP_VERSION_MAJOR);
    *minor = HAL_get_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_IP_VERSION_MINOR);
    *sub = HAL_get_32bit_reg_field(this_phy->tx_ctrl_base_addr, C10GB_IP_VERSION_SUB);
    return 0;
}

uint8_t
PHY10GKR_get_driver_version(uint32_t *major, uint32_t *minor, uint32_t *patch)
{
    *major = C10GBKR_VERSION_MAJOR;
    *minor = C10GBKR_VERSION_MINOR;
    *patch = C10GBKR_VERSION_PATCH;
    return 0;
}
