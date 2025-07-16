/**
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core10gbasekr_phy.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core10GBaseKR_PHY prototypes
 *
 */

/**
 * @mainpage Core10GBaseKR_PHY Bare Metal Driver
 *
 * ==============================================================================
 * Introduction
 * ==============================================================================
 * Core10GBaseKR_PHY is designed for the IEEE® 802.3-2012 specification and
 * supports the Core10GBaseKR_PHY interface for Backplane operations. This
 * configurable core provides the Physical (PHY) layer when used with a
 * transceiver interface. This IP interfaces with the Ten Gigabit Media
 * Independent Interface (XGMII) compliant Media Access Control (MAC) at the
 * system side and the transceiver block at the line side. The physical layer is
 * designed to work seamlessly with the PolarFire® and PolarFire SoC transceiver
 * using the Physical Medium Attachment (PMA) mode. This user guide documents the
 * features provided by the Core10GBaseKR_PHY embedded software driver.
 *
 * ==============================================================================
 * Hardware Flow Dependencies
 * ==============================================================================
 * This driver covers the configuration details of features such as the clauses 72
 * (Link Training), 73 (Auto-negotiation), and 74 (Forward Error Correction) of
 * IEEE802.3.
 *
 * See the [Core10GBaseKR_PHY User
 * Guide](https://mi-v-ecosystem.github.io/redirects/miv-rv32-ip-user-guide-core10gbasekr_phy) for a
 * detailed description of design requirements when interfacing the Core10GBaseKR_PHY to a
 * transceiver.
 *
 * ==============================================================================
 * Theory of Operation
 * ==============================================================================
 * The Core10GBaseKR_PHY driver functions are grouped into the following
 * categories:
 *  - Configuration
 *  - Initialization
 *  - 10GBASE-KR
 *  - Clause74: Forward Error Correction (FEC)
 *  - Clause73: Auto-negotiation
 *  - Clause72: Link training
 *
 *  --------------------------------
 *  Configuration
 *  --------------------------------
 *  The Core10GBaseKR_PHY driver requires an instance of a PHY configuration
 *  phy10gkr_cfg_t to be initialised. To load phy10gkr_cfg_t with default
 *  configurations, this configuration instance is passed by reference to
 *  PHY10GKR_cfg_struct_def_init(). This configuration structure may be overridden
 *  with alternative configurations before initialising the driver.
 *
 *  The application must point to an instance of the Transceiver (XCVR) per instance
 *  of a phy10gkr_instance_t. The phy10gkr_instance_t structure members xcvr and
 *  xcvr_api must point to the XCVR that the Core10GBaseKR_PHY IP is interfacing with
 *  and the appropriate XCVR APIs for dynamic configuration.
 *
 *  --------------------------------
 *  Initialization
 *  --------------------------------
 *  The Core10GBaseKR_PHY driver is initialized through a call to PHY10GKR_init().
 *  The PHY10GKR_init() function must be called before calling any other
 *  Core10GBaseKR_PHY driver functions.
 *
 *  --------------------------------
 *  10GBASE-KR
 *  --------------------------------
 *  The full 10GBASE-KR flow is handled by calling PHY10GKR_10gbasekr_sm(). As
 *  this function is dependent on interacting with a transceiver, transceiver
 *  function pointers must be configured by the user to point to the transceiver
 *  APIs that dynamically configure the transceiver implemented in the hardware
 *  design.
 *
 *  Each time PHY10GKR_10gbasekr_sm() is in the AN_SERDES_CONFIG state, the
 *  private function phy_10gbasekr_reset(), is called, which will load any user
 *  configurations to the Core registers and resets all algorithm and debug counters.
 *
 *  This API uses two private function calls, phy_10gbasekr_an() and phy_10gbasekr_lt(),
 *  to enable and interact with clause 72 and 73 hardware blocks within the
 *  Core10GBaseKR_PHY IP.
 *
 *  --------------------------------
 *  Clause74: FEC
 *  --------------------------------
 *  To enable FEC, FEC must be configured in the Core10GBaseKR_PHY IP. See the
 *  [Core10GBaseKR_PHY User
 * Guide](https://mi-v-ecosystem.github.io/redirects/miv-rv32-ip-user-guide-core10gbasekr_phy) for a
 * detailed description of how to enable FEC logic using the IP configurator.
 *
 *  During auto-negotiation initialization, if the FEC block has been configured
 *  in the Core, the driver will set the FEC ability bit 46 in the advertisement
 *  ability DME page.
 *
 *  The fec_request member within the phy10gkr_cfg_t structure may be set prior
 *  to driver initialisation to indicate that the driver should set the
 *  FEC request bit 47 in the advertisement ability clause 73 DME page. The driver
 *  implements error checking which identifies if fec_request is set when FEC is
 *  not configured within the Core.
 *
 *  --------------------------------
 *  Clause73: Auto-negotiation
 *  --------------------------------
 *  The IEEE802.3 clause 73 auto-negotiation is enabled and executed by the private
 *  function, phy_10gbasekr_an(). PHY10GKR_10gbasekr() uses this private function
 *  enable auto-negotiation and determines when the hardware has reached the
 *  AN_GOOD_CHECK state.
 *
 *  --------------------------------
 *  Clause72: Link Training
 *  --------------------------------
 *  The IEEE802.3 clause 72 link training is enabled and executed by the private
 *  function phy_10gbasekr_lt(). The Core10GBaseKR_PHY IP and the
 *  Core10GBaseKR_PHY embedded software driver together carry out the link
 *  training. The driver initiates the link training and takes appropriate actions
 *  depending on the events indicated by the 10GBaseKR status register bits during
 *  the link training process.
 *
 *  Training Failure: The training failure bit is set by the IP when the
 *  Core10GBaseKR_PHY link training timer exceeds 500 ms. The driver also
 *  implements a soft timer as an additional protection layer. PHY10GKR_get_current_time_ms()
 *  must be overridden by instantiating this function in user code, so that the
 *  current time of a timer will be returned in milli-seconds. When this status
 *  is set by Core10GBaseKR_PHY, the embedded software must reduce the XCVR data rate
 *  by calling PHY10GKR_serdes_an_config() and restart the auto-negotiation state
 *  machine by calling phy_10gbasekr_an().
 *
 *  Rx Calibration: The IP sets this status bit to indicate that there is a
 *  received status report of Max/Min/Updated that the Rx calibration algorithm
 *  must handle. The maximum to minimum sweep algorithm described in the
 *  [Core10GBaseKR_PHY User
 * Guide](https://mi-v-ecosystem.github.io/redirects/miv-rv32-ip-user-guide-core10gbasekr_phy) is
 * implemented by the functions which are defined within core10gbasekr_phy_link_training.h.
 *
 *  After the Rx calibration algorithm completes, the driver updates the
 *  transmit coefficient with new transmitter tap, which is sent to the
 *  link partner.
 *
 *  Tx Equalization: This status bit indicates that the received coefficient
 *  update has been updated and that the firmware needs to update the transceiver
 *  transmitter taps. The driver hands off the new coefficient settings to the
 *  transceiver using the PHY10GKR_serdes_tx_equalization() weak function,
 *  which must be overridden.
 *
 *  Signal Detect: When the driver identifies that this bit has been set by the
 *  Core10GBaseKR_PHY IP, it sets the link training complete flag. The IP updates
 *  this status bit when both the transmitted and received status reports have the
 *  receiver ready bit set. This indicates that both devices have completed their
 *  Rx calibration algorithm.
 */
#ifndef CORE10GBASEKR_PHY_H_
#define CORE10GBASEKR_PHY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "core10gbasekr_phy_types.h"
#include "core10gbasekr_phy_reg.h"

/**
 * Hardware and software timer timeouts.
 */
#define MAX_WAIT_TIMER_500MS        (161000000U)
#define LT_SOFTWARE_WAIT_TIMER_MS   (500U)

/**
 * CORE10GBASEKR_PHY VERSION TAGS
 * ============================
 * The version tags define the major, minor and patch driver release tags
 */
#define C10GBKR_VERSION_MAJOR       (3U)
#define C10GBKR_VERSION_MINOR       (0U)
#define C10GBKR_VERSION_PATCH       (107U)

/**
 * CORE10GBASEKR_PHY FEC ABILITY
 * ============================
 * This defines that the auto-negotiation advertisement FEC ability bit should be set
 * as the IP is configured with FEC logic.
 */
#define C10GBKR_FEC_ABILITY         (1U)

/**
 * CORE10GBASEKR_PHY NO FEC ABILITY
 * ============================
 * This defines that the auto-negotiation advertisement FEC ability bit should be
 * cleared as the IP is not configured with FEC logic.
 */
#define C10GBKR_NO_FEC_ABILITY      (0U)

/**
 * CORE10GBASEKR_PHY ENABLE FEC REQUEST
 * ============================
 * This defines that the auto-negotiation advertisement FEC request bit should be set
 * if the IP is configured with the FEC logic.
 */
#define C10GBKR_ENABLE_FEC_REQUEST  (1U)

/**
 * CORE10GBASEKR_PHY DISABLE FEC REQUEST
 * ============================
 * This defines that the auto-negotiation advertisement FEC request bit should be
 * cleared if the IP is configured with the FEC logic.
 */
#define C10GBKR_DISABLE_FEC_REQUEST (0U)

/**
 * CORE10GBASEKR_PHY FEC NEGOTIATED
 * ============================
 * This defines that FEC was negotiated during auto-negotiation
 */
#define C10GBKR_FEC_NEGOTIATED      (1U)

/**
 * CORE10GBASEKR_PHY FEC NOT NEGOTIATED
 * ============================
 * This defines that FEC was not negotiated during auto-negotiation
 */
#define C10GBKR_FEC_NOT_NEGOTIATED  (0U)

/**
 * CORE10GBASEKR_PHY LT MAX/MIN LIMITS
 * ============================
 * The max/min limit constants define the XCVR tap coefficient limits. These
 * constants can be overridden based on the XCVR, which is integrated into a
 * specific design.
 *
 * Note: Post and Pre tap maximum limits are absolute.
 */
#ifndef C10GBKR_LT_MAIN_TAP_MAX_LIMIT
#define C10GBKR_LT_MAIN_TAP_MAX_LIMIT (41U)
#endif

#ifndef C10GBKR_LT_MAIN_TAP_MIN_LIMIT
#define C10GBKR_LT_MAIN_TAP_MIN_LIMIT (26U)
#endif

#ifndef C10GBKR_LT_POST_TAP_MAX_LIMIT
#define C10GBKR_LT_POST_TAP_MAX_LIMIT (16U)
#endif

#ifndef C10GBKR_LT_POST_TAP_MIN_LIMIT
#define C10GBKR_LT_POST_TAP_MIN_LIMIT (0U)
#endif

#ifndef C10GBKR_LT_PRE_TAP_MAX_LIMIT
#define C10GBKR_LT_PRE_TAP_MAX_LIMIT (5U)
#endif

#ifndef C10GBKR_LT_PRE_TAP_MIN_LIMIT
#define C10GBKR_LT_PRE_TAP_MIN_LIMIT (0U)
#endif

/**
 * CORE10GBASEKR_PHY PRESET
 * ============================
 * The preset constants define the XCVR tap coefficient settings for a preset
 * request. They can be overridden based on the XCVR, which is integrated into a
 * specific design.
 *
 * Note: Post and Pre tap maximum limits are absolute.
 */
#ifndef C10GBKR_LT_PRESET_MAIN_TAP
#define C10GBKR_LT_PRESET_MAIN_TAP C10GBKR_LT_MAIN_TAP_MAX_LIMIT
#endif

#ifndef C10GBKR_LT_PRESET_POST_TAP
#define C10GBKR_LT_PRESET_POST_TAP C10GBKR_LT_POST_TAP_MAX_LIMIT
#endif

#ifndef C10GBKR_LT_PRESET_PRE_TAP
#define C10GBKR_LT_PRESET_PRE_TAP C10GBKR_LT_PRE_TAP_MAX_LIMIT
#endif

/**
 * CORE10GBASEKR_PHY INIT
 * ============================
 * The initialize constants define the coefficient settings, which is set
 * when an initialize request is received from the link partner. These constants
 * should be updated if there is no desire to calibrate the links.
 */
#ifndef C10GBKR_LT_INITIALIZE_MAIN_TAP
#define C10GBKR_LT_INITIALIZE_MAIN_TAP C10GBKR_LT_MAIN_TAP_MIN_LIMIT
#endif

#ifndef C10GBKR_LT_INITIALIZE_POST_TAP
#define C10GBKR_LT_INITIALIZE_POST_TAP C10GBKR_LT_POST_TAP_MIN_LIMIT
#endif

#ifndef C10GBKR_LT_INITIALIZE_PRE_TAP
#define C10GBKR_LT_INITIALIZE_PRE_TAP C10GBKR_LT_PRE_TAP_MIN_LIMIT
#endif

/**
 * CORE10GBASEKR_PHY LP REQUEST
 * ============================
 * This constant defines the request, which will be sent to the link partner and
 * determines which algorithm will be implemented to calibrate the link partner.
 */
#ifndef C10GBKR_LT_INITIAL_REQUEST
#define C10GBKR_LT_INITIAL_REQUEST C10GBKR_LT_PRESET
#endif

/**
 * CORE10GBASEKR_PHY AN LINK FAIL INHIBIT TIMER
 * ============================
 * This constant defines the auto-negotiation link fail inhibit timer timeout in
 * milli-seconds.
 */
#ifndef C10GBKR_AN_LINK_FAIL_INHITBIT_TIMER
#define C10GBKR_AN_LINK_FAIL_INHITBIT_TIMER (500U)
#endif

/*------------------------Public Function-------------------------------------*/
/**
 * PHY10GKR_cfg_struct_def_init() loads the PHY configuration struct
 * as default.
 *
 * @param cfg
 * The cfg parameter specifies the PHY configuration instance.
 *
 * @return
 * This function does not return a value.
 *
 * @example
 * @code
 *   #include "core10gbasekr_phy.h"
 *   int main(void)
 *   {
 *     PHY10GKR_cfg_struct_def_init(&phy_cfg);
 *     phy_cfg.fec_requested = 1;
 *
 *     PHY10GKR_init(&xcvr, &g_phy, CORE10GBKR_0_PHY_BASE_ADDR);
 *     PHY10GKR_config(&g_phy, &phy_cfg);
 *     return (0u);
 *   }
 * @endcode
 */
void PHY10GKR_cfg_struct_def_init(phy10gkr_cfg_t *cfg);

/**
 * PHY10GKR_init() initializes the Core10GBaseKR_PHY bare-metal driver. This
 * function sets the base address of the Auto-negotiation, link-training, tx
 * control, and rx status registers.
 *
 * @param this_phy
 * The this_phy parameter specifies the PHY instance.
 *
 * @param base_addr
 * The base_addr specifies the base address of the IP block.
 *
 * @param cfg
 * The cfg parameter specifies the PHY configuration instance.
 *
 * @param xcvr
 * This is a pointer to the instance of XCVR which is connected to this IP.
 *
 * @return
 * This function returns success or failure, 0 implies success. On failure this
 * function will return phy10gkr_error_t enumeration which indicates the error type.
 *
 * @example
 * @code
 *   #include "core10gbasekr_phy.h"
 *   int main(void)
 *   {
 *     void * xcvr;
 *     xcvr.base_addr = 0xFFFFFFFF;
 *     xcvr.lane = 1;
 *     xcvr.serdes = 2;
 *
 *     PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR, &phy_cfg ,&xcvr);
 *     return (0u);
 *   }
 * @endcode
 */
uint32_t PHY10GKR_init(phy10gkr_instance_t *this_phy,
                       addr_t base_addr,
                       phy10gkr_cfg_t *cfg,
                       void *xcvr);

/**
 * PHY10GKR_10gbasekr_sm() executes the full 10GBASE-KR flow required to
 * complete the auto-negotiation and link training.
 *
 * The 10GBASE-KR status enumeration allows the user to debug the auto-negotiation
 * and link training algorithms.
 *
 * @param this_phy
 * The this_phy parameter specifies the PHY instance.
 *
 * @return
 * This function returns the phy10gkr_status_t enumeration which indicates the status
 * of the API state machine. 0 implies that a link has been established.
 *
 * @example
 * @code
 *   #include "core10gbasekr_phy.h"
 *   int main(void)
 *   {
 *     uint32_t status;
 *     PHY10GKR_cfg_struct_def_init(&phy_cfg);
 *     phy_cfg.fec_requested = 1;
 *     PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR, &phy_cfg, &xcvr);
 *     while(1)
 *     {
 *       status = PHY10GKR_10gbasekr_sm(&g_phy);
 *       if(LINK_ESTABLISHED == status)
 *       {
 *         break;
 *       }
 *     }
 *     return (0u);
 *   }
 * @endcode
 */
uint32_t PHY10GKR_10gbasekr_sm(phy10gkr_instance_t *this_phy);

/**
 * PHY10GKR_get_current_time_ms() is a weak function that can be overridden
 * by the user to get the current time in milli-seconds.
 *
 * @return
 * This function returns the time in milli-seconds.
 */
uint32_t __attribute__((weak)) PHY10GKR_get_current_time_ms(void);

/**
 * PHY10GKR_get_ip_version() retrieves the IP version tags, the tags are passed by
 * reference as parameters and updated by this function. This function must be
 * called after initialization as there is a dependency on the instance of a
 * Core10GBaseKR_PHY instance.
 *
 * @param this_phy
 * The this_phy parameter specifies the PHY instance.
 *
 * @param major
 * This parameter identifies the major version number.
 *
 * @param minor
 * This parameter identifies the minor version number.
 *
 * @param sub
 * This parameter identifies the sub version number.
 *
 * @return
 * This function returns 0 on success.
 *
 * @example
 * @code
 *   #include "core10gbasekr_phy.h"
 *   int main(void)
 *   {
 *     uint32_t major,
 *     uint32_t minor,
 *     uint32_t sub,
 *
 *     PHY10GKR_cfg_struct_def_init(&phy_cfg);
 *     PHY10GKR_init(&g_phy, CORE10GBKR_0_PHY_BASE_ADDR, &phy_cfg, &xcvr);
 *
 *     PHY10GKR_get_ip_version(&g_phy, &major, &minor, &sub);
 *     return (0u);
 *   }
 * @endcode
 */
uint8_t PHY10GKR_get_ip_version(phy10gkr_instance_t *this_phy,
                                uint32_t *major,
                                uint32_t *minor,
                                uint32_t *sub);

/**
 * PHY10GKR_get_driver_version() retrieves the driver version tags, the tags are
 * passed by reference as parameters and updated by this function.
 *
 * @param major
 * This parameter identifies the major version number.
 *
 * @param minor
 * This parameter identifies the minor version number.
 *
 * @param patch
 * This parameter identifies the patch version number.
 *
 * @return
 * This function returns 0 on success.
 *
 * @example
 * @code
 *   #include "core10gbasekr_phy.h"
 *   int main(void)
 *   {
 *     uint32_t major,
 *     uint32_t minor,
 *     uint32_t patch,

 *     PHY10GKR_get_driver_version(&major, &minor, &patch);
 *     return (0u);
 *   }
 * @endcode
 */
uint8_t PHY10GKR_get_driver_version(uint32_t *major, uint32_t *minor, uint32_t *patch);

#ifdef __cplusplus
}
#endif

#endif /* CORE10GBASEKR_PHY_H_ */
