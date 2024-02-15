/**
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core10gbasekr_phy_link_training.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core10GBaseKR PHY link training prototypes
 *
 */

#include "core10gbasekr_phy.h"
#include "core10gbasekr_phy_link_training.h"

/*------------------------Private Variables-----------------------------------*/
uint32_t prbs_arr[64] = {0};
uint32_t prbs_cnt = 0;

/*------------------------Private Functions-----------------------------------*/
/**
 * This phy_10gbasekr_dfe_cal is private function which uses the XCVR APIs to set
 * and check DFE calibration for a set timeout in milli-seconds
 */
static uint32_t
phy_10gbasekr_dfe_cal(phy10gkr_instance_t *this_phy, uint32_t timeout_ms)
{
    this_phy->xcvr_api.dfe_cal(this_phy->xcvr);

    uint32_t start_time = PHY10GKR_get_current_time_ms();
    uint32_t end_time = 0;
    do
    {
        if (this_phy->xcvr_api.dfe_cal_status(this_phy->xcvr) == 0)
        {
            this_phy->xcvr_api.reset_pcs_rx(this_phy->xcvr);
            return 0;
        }
        end_time = PHY10GKR_get_current_time_ms() - start_time;
    } while (end_time < timeout_ms);

    this_phy->xcvr_api.reset_pcs_rx(this_phy->xcvr);
    return 1;
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
void
link_partner_calibration_sm(phy10gkr_instance_t *this_phy)
{
    switch (this_phy->lt.lp_cal_sweep_state)
    {
        case MAIN_TAP:
            link_partner_main_sweep(this_phy);
            break;

        case POST_TAP:
            link_partner_post_sweep(this_phy);
            break;

        case PRE_TAP:
            link_partner_pre_sweep(this_phy);
            break;

        default:
            HAL_ASSERT(0);
            break;
    }
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
void
link_partner_main_sweep(phy10gkr_instance_t *this_phy)
{
    phy10gkr_coeff_status_report_t tap_coeff =
        HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_RCVD_COEFF_MAIN_VALUE);

    if (this_phy->lt.main.lp_tap_cal_state == TAP_OPTIMISE_CAL)
    {
        if (this_phy->lt.main.optimal_cnt == (this_phy->lt.main.optimal_index))
        {
            lt_tap_increment(this_phy, POST_TAP);
            this_phy->lt.post.cnt++;
            this_phy->lt.post.inc_cnt++;
            this_phy->lt.lp_cal_sweep_state = POST_TAP;
            prbs_cnt = 0;
        }
        else
        {
            lt_tap_increment(this_phy, MAIN_TAP);
            this_phy->lt.main.optimal_cnt++;
        }
    }
    else
    {
        switch (tap_coeff)
        {
            case LT_COEFF_STATUS_MAX:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;
                lt_tap_decrement(this_phy, MAIN_TAP);
                this_phy->lt.main.cnt++;
                this_phy->lt.main.dec_cnt++;
                break;

            case LT_COEFF_STATUS_NOT_UPDATED:
                lt_tap_hold(this_phy, MAIN_TAP);
                this_phy->lt.main.cnt++;
                break;

            case LT_COEFF_STATUS_UPDATED:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;
                lt_tap_decrement(this_phy, MAIN_TAP);
                this_phy->lt.main.cnt++;
                this_phy->lt.main.dec_cnt++;
                break;

            case LT_COEFF_STATUS_MIN:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;

                this_phy->lt.main.optimal_index = lt_get_optimal_tap_index(prbs_arr, prbs_cnt);

                if (this_phy->lt.main.optimal_index == 0)
                {
                    lt_tap_increment(this_phy, POST_TAP);
                    this_phy->lt.post.cnt++;
                    this_phy->lt.post.inc_cnt++;
                    this_phy->lt.lp_cal_sweep_state = POST_TAP;
                    prbs_cnt = 0;
                }
                else
                {
                    this_phy->lt.main.lp_tap_cal_state = TAP_OPTIMISE_CAL;
                    lt_tap_increment(this_phy, MAIN_TAP);
                    this_phy->lt.main.optimal_cnt++;
                }
                break;

            default:
                HAL_ASSERT(0);
                break;
        }
    }
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
void
link_partner_post_sweep(phy10gkr_instance_t *this_phy)
{
    phy10gkr_coeff_status_report_t tap_coeff =
        HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_RCVD_COEFF_POST_VALUE);

    if (this_phy->lt.post.lp_tap_cal_state == TAP_OPTIMISE_CAL)
    {
        if (this_phy->lt.post.optimal_cnt == (this_phy->lt.post.optimal_index))
        {
            lt_tap_increment(this_phy, PRE_TAP);
            this_phy->lt.pre.cnt++;
            this_phy->lt.pre.inc_cnt++;
            this_phy->lt.lp_cal_sweep_state = PRE_TAP;
            prbs_cnt = 0;
        }
        else
        {
            lt_tap_increment(this_phy, POST_TAP);
            this_phy->lt.post.optimal_cnt++;
        }
    }
    else
    {
        switch (tap_coeff)
        {
            case LT_COEFF_STATUS_MAX:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;
                lt_tap_decrement(this_phy, POST_TAP);
                this_phy->lt.post.cnt++;
                this_phy->lt.post.dec_cnt++;
                break;

            case LT_COEFF_STATUS_NOT_UPDATED:
                lt_tap_hold(this_phy, POST_TAP);
                this_phy->lt.post.cnt++;
                break;

            case LT_COEFF_STATUS_UPDATED:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;
                lt_tap_decrement(this_phy, POST_TAP);
                this_phy->lt.post.cnt++;
                this_phy->lt.post.dec_cnt++;
                break;

            case LT_COEFF_STATUS_MIN:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;

                this_phy->lt.post.optimal_index = lt_get_optimal_tap_index(prbs_arr, prbs_cnt);

                if (this_phy->lt.post.optimal_index == 0)
                {
                    lt_tap_increment(this_phy, PRE_TAP);
                    this_phy->lt.pre.cnt++;
                    this_phy->lt.pre.inc_cnt++;
                    this_phy->lt.lp_cal_sweep_state = PRE_TAP;
                    prbs_cnt = 0;
                }
                else
                {
                    this_phy->lt.post.lp_tap_cal_state = TAP_OPTIMISE_CAL;
                    lt_tap_increment(this_phy, POST_TAP);
                    this_phy->lt.post.optimal_cnt++;
                }
                break;

            default:
                HAL_ASSERT(0);
                break;
        }
    }
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
void
link_partner_pre_sweep(phy10gkr_instance_t *this_phy)
{
    phy10gkr_coeff_status_report_t tap_coeff =
        HAL_get_32bit_reg_field(this_phy->lt_base_addr, C10GB_LT_RCVD_COEFF_PRE_VALUE);

    if (this_phy->lt.pre.lp_tap_cal_state == TAP_OPTIMISE_CAL)
    {
        if (this_phy->lt.pre.optimal_cnt == (this_phy->lt.pre.optimal_index))
        {
            while ((phy_10gbasekr_dfe_cal(this_phy, 10) != 0) &&
                   (STATUS_LT_FAILURE != this_phy->lt.status))
            {
                uint32_t c10gbkr_status =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_STATUS);

                this_phy->lt.timer.end = PHY10GKR_get_current_time_ms() - this_phy->lt.timer.start;

                if ((c10gbkr_status & C10GB_LT_TRAINING_FAIL_MASK) ||
                    this_phy->lt.timer.end > LT_SOFTWARE_WAIT_TIMER_MS)
                {
                    this_phy->lt.status = STATUS_LT_FAILURE;
                    break;
                }
            }

            if (STATUS_LT_FAILURE != this_phy->lt.status)
            {
                /* All sweeps complete set to best ber index */
                this_phy->lt.local_rxcvr = LOCAL_RXCVR_LOCKED;

                this_phy->lt.rcvr_cnt++;

                HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                        C10GB_LT_LOCAL_RCVR_LOCKED,
                                        LOCAL_RXCVR_LOCKED);
                prbs_cnt = 0;
            }
        }
        else
        {
            lt_tap_increment(this_phy, PRE_TAP);
            this_phy->lt.pre.optimal_cnt++;
        }
    }
    else
    {
        switch (tap_coeff)
        {
            case LT_COEFF_STATUS_MAX:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;
                lt_tap_decrement(this_phy, PRE_TAP);
                this_phy->lt.pre.cnt++;
                this_phy->lt.pre.dec_cnt++;
                break;

            case LT_COEFF_STATUS_NOT_UPDATED:
                lt_tap_hold(this_phy, PRE_TAP);
                this_phy->lt.pre.cnt++;
                break;

            case LT_COEFF_STATUS_UPDATED:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;
                lt_tap_decrement(this_phy, PRE_TAP);
                this_phy->lt.pre.cnt++;
                this_phy->lt.pre.dec_cnt++;
                break;

            case LT_COEFF_STATUS_MIN:
                prbs_arr[prbs_cnt] =
                    HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_PRBS_ERR_WRD);
                prbs_cnt++;

                this_phy->lt.pre.optimal_index = lt_get_optimal_tap_index(prbs_arr, prbs_cnt);

                if (this_phy->lt.post.optimal_index == 0)
                {
                    while ((phy_10gbasekr_dfe_cal(this_phy, 10) != 0) &&
                           (STATUS_LT_FAILURE != this_phy->lt.status))
                    {
                        uint32_t c10gbkr_status =
                            HAL_get_32bit_reg(this_phy->lt_base_addr, C10GB_LT_STATUS);

                        this_phy->lt.timer.end =
                            PHY10GKR_get_current_time_ms() - this_phy->lt.timer.start;

                        if ((c10gbkr_status & C10GB_LT_TRAINING_FAIL_MASK) ||
                            (this_phy->lt.timer.end > LT_SOFTWARE_WAIT_TIMER_MS))
                        {
                            this_phy->lt.status = STATUS_LT_FAILURE;
                            break;
                        }
                    }

                    if (STATUS_LT_FAILURE != this_phy->lt.status)
                    {
                        /* All sweeps complete set to best ber index */
                        this_phy->lt.local_rxcvr = LOCAL_RXCVR_LOCKED;

                        this_phy->lt.rcvr_cnt++;

                        HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                                C10GB_LT_LOCAL_RCVR_LOCKED,
                                                LOCAL_RXCVR_LOCKED);
                        prbs_cnt = 0;
                    }
                }
                else
                {
                    this_phy->lt.pre.lp_tap_cal_state = TAP_OPTIMISE_CAL;
                    lt_tap_increment(this_phy, PRE_TAP);
                    this_phy->lt.pre.optimal_cnt++;
                }
                break;

            default:
                HAL_ASSERT(0);
                break;
        }
    }
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
void
lt_tap_decrement(phy10gkr_instance_t *this_phy, phy10gkr_tx_equalizer_tap_t tap_type)
{
    switch (tap_type)
    {
        case MAIN_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_DEC_MAIN_MASK);
            break;

        case POST_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_DEC_POST_MASK);
            break;

        case PRE_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_DEC_PRE_MASK);
            break;

        default:
            HAL_ASSERT(0);
            break;
    }
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
void
lt_tap_hold(phy10gkr_instance_t *this_phy, phy10gkr_tx_equalizer_tap_t tap_type)
{
    switch (tap_type)
    {
        case MAIN_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_HOLD_MAIN_MASK);
            break;

        case POST_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_HOLD_POST_MASK);
            break;

        case PRE_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_HOLD_PRE_MASK);
            break;

        default:
            HAL_ASSERT(0);
            break;
    }
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
void
lt_tap_increment(phy10gkr_instance_t *this_phy, phy10gkr_tx_equalizer_tap_t tap_type)
{
    switch (tap_type)
    {
        case MAIN_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_INC_MAIN_MASK);
            break;

        case POST_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_INC_POST_MASK);
            break;

        case PRE_TAP:
            HAL_set_32bit_reg(this_phy->lt_base_addr,
                              C10GB_LT_TX_COEFF_CFG,
                              C10GB_LT_TX_INC_PRE_MASK);
            break;

        default:
            HAL_ASSERT(0);
            break;
    }
}

/**
 * Please see core10gbasekr_phy_link_training.h for description
 */
uint32_t
lt_get_optimal_tap_index(uint32_t *prbs_arr, uint32_t prbs_arr_size)
{
    uint32_t i = 0;
    uint32_t min_error = prbs_arr[0];
    uint32_t min_index = 0;
    uint32_t min_midpoint_distance = prbs_arr_size / 2;

    /* sweep to find min prbs error */
    for (i = 0; i < prbs_arr_size; i++)
    {
        if (prbs_arr[i] < min_error)
        {
            min_error = prbs_arr[i];
            min_index = i;
        }
    }

    /* sweep to find which index are equal the min, weighted by distance to mid
     * point of array
     */
    for (i = 0; i < prbs_arr_size; i++)
    {
        if (prbs_arr[i] == min_error)
        {
            int32_t midpoint_distance = (prbs_arr_size / 2) - i;
            if (midpoint_distance < 0)
            {
                midpoint_distance *= -1;
            }

            if (midpoint_distance < min_midpoint_distance)
            {
                min_midpoint_distance = midpoint_distance;
                min_index = i;
            }

            if (midpoint_distance == 0)
            {
                break;
            }
        }
    }

    /* index reversed as the order was set while decrementing from max to min */
    return prbs_arr_size - min_index;
}
