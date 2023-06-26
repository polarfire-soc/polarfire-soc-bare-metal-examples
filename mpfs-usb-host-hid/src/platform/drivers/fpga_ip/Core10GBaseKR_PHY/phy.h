/***************************************************************************//**
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * @file phy.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PHY prototypes
 * 
 */

/*=========================================================================*//**
  @mainpage Core10GBaseKR_PHY Bare Metal Driver

  ==============================================================================
  Introduction
  ==============================================================================
  Core10GBaseKR_PHY is designed for the IEEE® 802.3-2012 specification and
  supports the Core10GBaseKR_PHY interface for Backplane operations. This
  configurable core provides the Physical (PHY) layer when used with a
  transceiver interface. This IP interfaces with the Ten Gigabit Media
  Independent Interface (XGMII) compliant Media Access Control (MAC) at the
  system side and the transceiver block at the line side. The physical layer is
  designed to work seamlessly with the PolarFire® and PolarFire SoC transceiver
  using the Physical Medium Attachment (PMA) mode. This user guide documents the
  features provided by the Core10GBaseKR_PHY firmware driver.

  ==============================================================================
  Hardware Flow Dependencies
  ==============================================================================
  This driver covers the configuration details of features such as IEEE802.3
  clauses 73 and 72.

  See the Core10GBaseKR_PHY User Guide for a detailed description of
  design requirements when interfacing the Core10GBaseKR_PHY to a transceiver.

  ==============================================================================
  Software Flow Dependencies
  ==============================================================================
  A PHY software configuration file must be included when using this driver.
  This will include the macro definition "CORE10GBASEKR_PHY". The purpose of
  this file is to configure desired PHY model and to override driver default
  values as required. The driver configuration should be stored in a location
  away from the driver source code. The following with path is used in our
  recommended directory structure:

  \`
  <project-root>/boards/<board-name>/platform_config/driver_config/phy_sw_cfg.h
  \`

  ==============================================================================
  Theory of Operation
  ==============================================================================
  The Core10GBaseKR_PHY driver functions are grouped into the following
  categories:
    - Initialization
    - Configuration
    - Clause73: Auto-negotiation
    - Clause72: Link training
    - 10GBASE-KR

  --------------------------------
  Initialization
  --------------------------------
  The Core10GBaseKR_PHY driver is initialized through a call to the
  PHY10GKR_init() function. The PHY10GKR_init() function must be called before
  calling any other Core10GBaseKR_PHY driver functions.

  --------------------------------
  Configuration
  --------------------------------
  An instance of the Core10GBaseKR_PHY is configured with a call to the
  PHY10GKR_config(). The configuration function resets all the PHY instance
  structure members other than information such as performance counters.
  Default configurations can be overridden be defining any of the
  Core10GBaseKR_PHY constants before loading the driver using a PHY software
  configuration file.

  --------------------------------
  Clause73: Auto-negotiation
  --------------------------------
  The IEEE802.3 clause 73 auto-negotiation is enabled and executed by calling
  PHY10GKR_autonegotiate_sm() function.

  --------------------------------
  Clause72: Link Training
  --------------------------------
  The IEEE802.3 clause 72 link training is enabled and executed by a calling
  PHY10GKR_link_training_sm(). The Core10GBaseKR_PHY IP and the
  Core10GBaseKR_PHY embedded software driver together carry out the link
  training. The driver initiates the link training and takes appropriate actions
  depending on the events indicated by the 10GBaseKR status register bits during
  the link training process. The following figure shows an overview of what
  actions are taken for each 10GBaseKR status bit:

  \include resources/link_training_10gbasekr_status.txt

  Training Failure: The training failure bit is set by the IP when the
  Core10GBaseKR_PHY link training timer exceeds 500 ms. The driver also
  implements a soft timer as an additional protection layer. The
  PHY10GKR_get_current_time_ms() function must be overridden by instantiating
  this function in user code so that the current time of a timer will be
  returned in milli-seconds. When this status is set by Core10GBaseKR_PHY,
  the embedded software must reduce the XCVR data rate by calling
  PHY10GKR_serdes_an_config() and restart the auto-negotiation state machine by
  calling PHY10GKR_autonegotiate_sm().

  Rx Calibration: The IP sets this status bit to indicate that there is a
  received status report of Max/Min/Updated that the Rx calibration algorithm
  must handle. The maximum to minimum sweep algorithm described in the
  Core10GBaseKR_PHY User Guide is implemented by the functions which are defined
  within core10gbasekr_phy_link_training.h.

  After the Rx calibration algorithm completes, the driver updates the
  transmit coefficient with new transmitter tap, which will be sent to the
  link partner.

  Tx Equalization: This status bit indicates that the received coefficient
  update has been updated and that the firmware needs to update the transceiver
  transmitter taps. The driver hands off the new coefficient settings to the
  transceiver using the PHY10GKR_serdes_tx_equalization() weak function,
  which must be overridden.

  Signal Detect: When the driver identifies that this bit has been set by the
  Core10GBaseKR_PHY IP, it sets the link training complete flag. The IP updates
  this status bit when both the transmitted and received status reports have the
  receiver ready bit set. This indicates that both devices have completed their
  Rx calibration algorithm.

  --------------------------------
  10GBASE-KR
  --------------------------------
  The full 10GBASE-KR flow is handled by calling PHY10GKR_10gbasekr_sm()
  function. As this function is dependent on interacting with a transceiver,
  the following weak functions should be overridden for the specific design
  being implemented.
  - PHY10GKR_get_current_time_ms()
  - PHY10GKR_serdes_an_config()
  - PHY10GKR_serdes_lt_config()
  - PHY10GKR_serdes_cdr_lock()
  - PHY10GKR_serdes_dfe_cal()
  - PHY10GKR_serdes_tx_equalization()

 *//*=========================================================================*/
#ifndef PHY_H_
#define PHY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "phy_types.h"
#include "core10gbasekr_phy_reg.h"

#ifdef CORE10GBASEKR_PHY

/*-------------------------------------------------------------------------*//**
  These constant definitions are used as an argument for the function
  PHY10GKR_config().

 */

#define MAX_WAIT_TIMER_500MS            (161000000U)
#define LT_SOFTWARE_WAIT_TIMER_MS       (500U)

/*-------------------------------------------------------------------------*//**
  CORE10GBASEKR_PHY LT MAX/MIN LIMITS
  ============================
  The max/min limit constants define the XCVR tap coefficient limits. These
  constants can be overridden based on the XCVR, which is integrated into a
  specific design.

  Note: Post and Pre tap maximum limits are absolute.

 */
#ifndef C10GBKR_LT_MAIN_TAP_MAX_LIMIT
#define C10GBKR_LT_MAIN_TAP_MAX_LIMIT   (41U)
#endif

#ifndef C10GBKR_LT_MAIN_TAP_MIN_LIMIT
#define C10GBKR_LT_MAIN_TAP_MIN_LIMIT   (26U)
#endif

#ifndef C10GBKR_LT_POST_TAP_MAX_LIMIT
#define C10GBKR_LT_POST_TAP_MAX_LIMIT   (16U)
#endif

#ifndef C10GBKR_LT_POST_TAP_MIN_LIMIT
#define C10GBKR_LT_POST_TAP_MIN_LIMIT   (0U)
#endif

#ifndef C10GBKR_LT_PRE_TAP_MAX_LIMIT
#define C10GBKR_LT_PRE_TAP_MAX_LIMIT    (5U)
#endif

#ifndef C10GBKR_LT_PRE_TAP_MIN_LIMIT
#define C10GBKR_LT_PRE_TAP_MIN_LIMIT    (0U)
#endif

/*-------------------------------------------------------------------------*//**
  CORE10GBASEKR_PHY PRESET
  ============================
  The preset constants define the XCVR tap coefficient settings for a preset
  request. They can be overridden based on the XCVR, which is integrated into a
  specific design.

  Note: Post and Pre tap maximum limits are absolute.

 */
#ifndef C10GBKR_LT_PRESET_MAIN_TAP
#define C10GBKR_LT_PRESET_MAIN_TAP      C10GBKR_LT_MAIN_TAP_MAX_LIMIT
#endif

#ifndef C10GBKR_LT_PRESET_POST_TAP
#define C10GBKR_LT_PRESET_POST_TAP      C10GBKR_LT_POST_TAP_MAX_LIMIT
#endif

#ifndef C10GBKR_LT_PRESET_PRE_TAP
#define C10GBKR_LT_PRESET_PRE_TAP       C10GBKR_LT_PRE_TAP_MAX_LIMIT
#endif

/*-------------------------------------------------------------------------*//**
  CORE10GBASEKR_PHY INIT
  ============================
  The initialize constants define the coefficient settings, which is set
  when an initialize request is received from the link partner. These constants
  should be updated if there is no desire to calibrate the links.

 */
#ifndef C10GBKR_LT_INITIALIZE_MAIN_TAP
#define C10GBKR_LT_INITIALIZE_MAIN_TAP  C10GBKR_LT_MAIN_TAP_MIN_LIMIT
#endif

#ifndef C10GBKR_LT_INITIALIZE_POST_TAP
#define C10GBKR_LT_INITIALIZE_POST_TAP  C10GBKR_LT_POST_TAP_MIN_LIMIT
#endif

#ifndef C10GBKR_LT_INITIALIZE_PRE_TAP
#define C10GBKR_LT_INITIALIZE_PRE_TAP   C10GBKR_LT_PRE_TAP_MIN_LIMIT
#endif

/*-------------------------------------------------------------------------*//**
  CORE10GBASEKR_PHY LP REQUEST
  ============================
  This constant defines the request, which will be sent to the link partner and
  determines which algorithm will be implemented to calibrate the link partner.

 */
#ifndef C10GBKR_LT_INITIAL_REQUEST
#define C10GBKR_LT_INITIAL_REQUEST      C10GBKR_LT_PRESET
#endif

/*-------------------------------------------------------------------------*//**
  CORE10GBASEKR_PHY AN LINK FAIL INHIBIT TIMER
  ============================
  This constant defines the auto-negotiation link fail inhibit timer timeout in
  milli-seconds.

 */
#ifndef C10GBKR_AN_LINK_FAIL_INHITBIT_TIMER
#define C10GBKR_AN_LINK_FAIL_INHITBIT_TIMER\
                                        (500U)
#endif

#endif /* CORE10GBASEKR_PHY */

/*------------------------Public Function-------------------------------------*/

#ifdef CORE10GBASEKR_PHY
/***************************************************************************//**
  The PHY10GKR_init() function initializes the Core10GBaseKR_PHY bare-metal
  driver. This function sets the base address of the Auto-negotiation, link-
  training, tx control, and rx status registers.

  @param this_phy
    The this_phy parameter specifies the PHY instance.

  @param base_addr
    The base_addr specifies the base address of the IP block.

  @return
    This function does not return a value.

  @example
  @code
    #include "phy.h"
    int main(void)
    {
      PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR);
      return (0u);
    }
  @endcode
 */
void
PHY10GKR_init
(
    phy10gkr_instance_t * this_phy,
    addr_t base_addr
);

/***************************************************************************//**
  The PHY10GKR_config() function configures the PHY registers with the
  predefined defaults and user configurations. This function also resets the
  structures back to their initial conditions.

  @param this_phy
    The this_phy parameter specifies the PHY instance.

  @return
    This function does not return a value.

  @example
  @code
      #include "phy.h"
      int main(void)
      {
        PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR);
        PHY10GKR_config(&g_phy);
        return (0u);
      }
  @endcode
 */
void
PHY10GKR_config
(
    phy10gkr_instance_t * this_phy
);

/***************************************************************************//**
  The PHY10GKR_autonegotiate_sm() function enables the auto-negotiation API
  state machine, which enables the auto-negotiation registers and then checks the
  status of the auto-negotiation state machine to determine if auto-negotiation
  has complete.

  @param this_phy
    The this_phy parameter specifies the PHY instance.

  @return
    This function does not return a value.

  @example
  @code
      #include "phy.h"
      int main(void)
      {
        PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR);
        while(1)
        {
            PHY10GKR_autonegotiate_sm(&g_phy);
            if(STATUS_AN_COMPLETE == g_phy.an.complete)
            {
                break;
            }
        }
        return (0u);
      }
  @endcode
 */
void
PHY10GKR_autonegotiate_sm
(
    phy10gkr_instance_t * this_phy
);

/***************************************************************************//**
  The PHY10GKR_link_training_sm() function enables the link training API
  state machine, which enables the link training registers and then runs the
  link training algorithm.

  The connected transceiver must have a data rate of 10 Gbps and locked to a
  link partner with the same data rate for successful link training.

  @param this_phy
    The this_phy parameter specifies the PHY instance.

  @return
    This function does not return a value.

  @example
  @code
      #include "phy.h"
      int main(void)
      {
        PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR);
        while(1)
        {
            PHY10GKR_link_training_sm(&g_phy);
            if(STATUS_LT_FAILURE == g_phy.lt.status)
            {
                HAL_ASSERT(0);
            }
        }
        return (0u);
      }
  @endcode
 */
void
PHY10GKR_link_training_sm
(
    phy10gkr_instance_t * this_phy
);

/***************************************************************************//**
  The PHY10GKR_10gbasekr_sm() executes the full 10GBASE-KR flow required to
  complete the auto-negotiation and link training.

  The 10GBASE-KR status enumeration allows the user to debug the auto-negotiation
  and link training algorithms.

  @param this_phy
    The this_phy parameter specifies the PHY instance.

  @return
    This function returns a state machine status.

  @example
  @code
      #include "phy.h"
      int main(void)
      {
        uint32_t status;
        PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR);
        while(1)
        {
            status = PHY10GKR_10gbasekr_sm(&g_phy);
            if(LINK_ESTABLISHED == status)
            {
                break;
            }
        }
        return (0u);
      }
  @endcode
 */
uint32_t
PHY10GKR_10gbasekr_sm
(
    phy10gkr_instance_t * this_phy
);

/***************************************************************************//**
  The PHY10GKR_set_lane_los_signal() asserts and deasserts the transceivers
  Lane Loss of signal detection.

  @param state
    Asserts or deasserts the lane LOS

  @return
    This function does not return a value.
 */
void
PHY10GKR_set_lane_los_signal
(
    phy10gkr_instance_t * this_phy,
    uint32_t state
);

/***************************************************************************//**
  The PHY10GKR_get_current_time_ms() is a weak function that can be overridden
  by the user to get the current time in milli-seconds.

  @return
    This function returns the time in milli-seconds.
 */
uint32_t __attribute__((weak))
PHY10GKR_get_current_time_ms
(
    void
);

/***************************************************************************//**
  The PHY10GKR_serdes_an_config() is a weak function that can be overridden
  by the user to configure the XCVR instance integrated in their design
  for auto-negotiation.

  @return
    This function does not return a value.
 */
void __attribute__((weak))
PHY10GKR_serdes_an_config
(
    void
);

/***************************************************************************//**
  The PHY10GKR_serdes_lt_config() is a weak function that can be overridden
  by the user to configure the XCVR instance integrated in their design
  for link training at 10 Gbps.

  @return
    This function does not return a value.
 */
uint32_t __attribute__((weak))
PHY10GKR_serdes_lt_config
(
    void
);

/***************************************************************************//**
  The PHY10GKR_serdes_cdr_lock() is a weak function that can be overridden by
  the user to determine that the XCVR instance integrated has achieved a CDR
  lock.

  @return
    This function returns 0 on success and 1 on failure.
 */
uint32_t __attribute__((weak))
PHY10GKR_serdes_cdr_lock
(
    void
);

/***************************************************************************//**
  The PHY10GKR_serdes_dfe_cal() is a weak function that can be overridden by
  the user to determine that the XCVR instance integrated has completed DFE
  calibration.

  Note this function should constantly check if the link training
  failure time has timeout and if so exit the function.

  @return
    This function does not return a value.
 */
uint32_t __attribute__((weak))
PHY10GKR_serdes_dfe_cal
(
    void
);

/***************************************************************************//**
  The PHY10GKR_serdes_tx_equalization() is a weak function that can be
  overridden by the user to set the current XCVR tap coefficients.

  @return
    This function does not return a value.
 */
void __attribute__((weak))
PHY10GKR_serdes_tx_equalization
(
    uint32_t tx_main_tap,
    uint32_t tx_post_tap,
    uint32_t tx_pre_tap
);

#endif /* CORE10GBASEKR_PHY */

#ifdef __cplusplus
}
#endif

#endif /* PHY_H_ */
