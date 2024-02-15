/**
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core10gbasekr_phy_link_training.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core10GBaseKR PHY link training prototypes
 *
 */

#ifndef CORE10GBASEKR_PHY_LINK_TRAINING_H_
#define CORE10GBASEKR_PHY_LINK_TRAINING_H_

#ifdef __cplusplus
extern "C" {
#endif

/// @cond @private

#include "core10gbasekr_phy.h"

/**
 * Link partner calibration state machine.
 *
 * This function determines which of the link partners transmitter taps are to be
 * calibrated.
 *
 * @param this_phy
 * The this_phy parameter specifies the 10GBaseKR PHY block.
 *
 * @return
 * This function does not return a value.
 */
void link_partner_calibration_sm(phy10gkr_instance_t *this_phy);

/**
 * Link partner main (c(0)) cursor sweep.
 *
 * The algorithm increments the link partners transmit taps to maximum (as this
 * algorithm is only used when preset is sent the link partner main tap should be
 * maximum). When the main tap is maximum the algorithm will decrement the link
 * partner's main transmit tap to minimum before decrementing the algorithm
 * stores the PRBS for optimisation.
 *
 * When the link partners transmit main tap is minimum algorithm determines the
 * optimal tap setting based on the PRBS errors stored during sweeping. The
 * algorithm now increments the link partners transmit tap back to the optimal
 * tap setting.
 *
 * @param this_phy
 * The this_phy parameter specifies the 10GBaseKR PHY block.
 *
 * @return
 * This function does not return a value.
 */
void link_partner_main_sweep(phy10gkr_instance_t *this_phy);

/**
 * Link partner post (c(+1)) cursor sweep.
 *
 * The algorithm increments the link partners transmit taps to maximum (as this
 * algorithm is only used when preset is sent the link partner post tap should be
 * maximum). When the post tap is maximum the algorithm will decrement the link
 * partner's post transmit tap to minimum before decrementing the algorithm
 * stores the PRBS for optimisation.
 *
 * When the link partners transmit post tap is minimum algorithm determines the
 * optimal tap setting based on the PRBS errors stored during sweeping. The
 * algorithm now increments the link partners transmit tap back to the optimal
 * tap setting.
 *
 * @param this_phy
 * The this_phy parameter specifies the 10GBaseKR PHY block.
 *
 * @return
 * This function does not return a value.
 */
void link_partner_post_sweep(phy10gkr_instance_t *this_phy);

/**
 * Link partner pre (c(-1)) cursor sweep.
 *
 * The algorithm increments the link partners transmit taps to maximum (as this
 * algorithm is only used when preset is sent the link partner pre tap should be
 * maximum). When the pre tap is maximum the algorithm will decrement the link
 * partner's pre transmit tap to minimum before decrementing the algorithm
 * stores the PRBS for optimisation.
 *
 * When the link partners transmit pre tap is minimum algorithm determines the
 * optimal tap setting based on the PRBS errors stored during sweeping. The
 * algorithm now increments the link partners transmit tap back to the optimal
 * tap setting.
 *
 * Once the link partner's pre tap has incremented back to the optimal setting
 * the local receiver ready bit is set to indicate to the link partner that
 * this device is finished calibrating the link partners transmitter taps.
 *
 * @param this_phy
 * The this_phy parameter specifies the 10GBaseKR PHY block.
 *
 * @return
 * This function does not return a value.
 */
void link_partner_pre_sweep(phy10gkr_instance_t *this_phy);

/**
 * This function sets the decrement bit of a specific transmit equalizer tap
 * register which updates the transmitted coefficient update to the link partner.
 *
 * @param this_phy
 * The this_phy parameter specifies the 10GBaseKR PHY block.
 *
 * @param tap_type
 * This enumeration specifies the which transmitter tap will be decremented
 *
 * @return
 * This function does not return a value.
 */
void lt_tap_decrement(phy10gkr_instance_t *this_phy, phy10gkr_tx_equalizer_tap_t tap_type);

/**
 * This function sets the hold bit of a specific transmit equalizer tap
 * register which updates the transmitted coefficient update to the link partner.
 *
 * @param this_phy
 * The this_phy parameter specifies the 10GBaseKR PHY block.
 *
 * @param tap_type
 * This enumeration specifies the which transmitter tap will be held
 *
 * @return
 * This function does not return a value.
 */
void lt_tap_hold(phy10gkr_instance_t *this_phy, phy10gkr_tx_equalizer_tap_t tap_type);

/**
 * This function sets the increment bit of a specific transmit equalizer tap
 * register which updates the transmitted coefficient update to the link partner.
 *
 * @param this_phy
 * The this_phy parameter specifies the 10GBaseKR PHY block.
 *
 * @param tap_type
 * This enumeration specifies the which transmitter tap will be incremented
 *
 * @return
 * This function does not return a value.
 */
void lt_tap_increment(phy10gkr_instance_t *this_phy, phy10gkr_tx_equalizer_tap_t tap_type);

/**
 * This function calculates the optimal tap setting index based on the PRBS error
 * counts stored during maximum to minimum tap sweep.
 *
 * Sweeps the PRBS error array to find the minimum error, if there are multiple
 * samples that have the same error a weighting is applied to select the tap
 * setting closest to the center tap setting.
 *
 * @param prbs_arr
 * This is a pointer to the first sample of the PRBS error sample array
 *
 * @param prbs_arr_size
 * This identifies the number of samples.
 *
 * @return
 * This function returns the number of increments required to increment a tap
 * to its optimal tap setting.
 */
uint32_t lt_get_optimal_tap_index(uint32_t *prbs_arr, uint32_t prbs_arr_size);

/// @endcond

#ifdef __cplusplus
}
#endif

#endif /* CORE10GBASEKR_PHY_LINK_TRAINING_H_ */
