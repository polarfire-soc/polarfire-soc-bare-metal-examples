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
 * @file core10gbasekr_phy_reg.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core10GBaseKR PHY memory map
 * 
 */

#ifndef CORE10GBASEKR_PHY_REG_H_
#define CORE10GBASEKR_PHY_REG_H_

#ifdef __cplusplus
extern "C" {
#endif

/// @cond @private

#include "uint_32_bit_masks.h"

#define __I  const volatile
#define __O  volatile
#define __IO volatile

/******************************************************************************/
/*             Core10GBaseKR PHY Clause 73 (Auto Negotiation)                 */
/******************************************************************************/

/*------------------------------------------------------------------------------
 * AN_CONTROL register details
 */
#define C10GB_AN_CONTROL_REG_OFFSET	           (0x0U << 2U)

/*
 *  Auto Negotiation Restart
 *
 *  Restart_negotiation. Self clears, and always reads as 0.
 */
#define C10GB_AN_RESTART_OFFSET                C10GB_AN_CONTROL_REG_OFFSET
#define C10GB_AN_RESTART_MASK                  MASK_BIT_9
#define C10GB_AN_RESTART_SHIFT                 (9U)

/*
 *  Auto Negotiation Enable
 *
 *  Auto-negotiation Enable. Writing “1” in this register enables
 *  auto-negotiation
 */
#define C10GB_AN_ENABLE_OFFSET                 C10GB_AN_CONTROL_REG_OFFSET
#define C10GB_AN_ENABLE_MASK                   MASK_BIT_12
#define C10GB_AN_ENABLE_SHIFT                  (12U)

/*
 *  Auto Negotiation Reset
 *
 *  Main_reset. Self clears. When reads as 0 the reset is complete.
 */
#define C10GB_AN_RESET_OFFSET                  C10GB_AN_CONTROL_REG_OFFSET
#define C10GB_AN_RESET_MASK                    MASK_BIT_15
#define C10GB_AN_RESET_SHIFT                   (15U)


/*------------------------------------------------------------------------------
 * AN_STATUS register details
 */
#define C10GB_AN_STATUS_REG_OFFSET	           (0x1U << 2U)

/*
 *  Auto Negotiation Able
 *
 *  If “1” indicates that Link Partner is Autonegotiation Able
 */
#define C10GB_AN_CAPABLE_OFFSET                C10GB_AN_STATUS_REG_OFFSET
#define C10GB_AN_CAPABLE_MASK                  MASK_BIT_0
#define C10GB_AN_CAPABLE_SHIFT                 (0U)

/*
 *  Auto Negotiation Support
 *
 *  If “1” indicates that Autonegotiation is supported
 */
#define C10GB_AN_SUPPORT_OFFSET                C10GB_AN_STATUS_REG_OFFSET
#define C10GB_AN_SUPPORT_MASK                  MASK_BIT_3
#define C10GB_AN_SUPPORT_SHIFT                 (3U)

/*
 *  Auto Negotiation Complete
 *
 *  If “1” indicates that AN is complete
 */
#define C10GB_AN_COMPLETE_OFFSET               C10GB_AN_STATUS_REG_OFFSET
#define C10GB_AN_COMPLETE_MASK                 MASK_BIT_5
#define C10GB_AN_COMPLETE_SHIFT                (5U)

/*
 *  Auto Negotiation Page Received
 *
 *  Page_RX. If “1” that Page is received from the link partner. Clear on read
 */
#define C10GB_AN_PAGE_RX_OFFSET                C10GB_AN_STATUS_REG_OFFSET
#define C10GB_AN_PAGE_RX_MASK                  MASK_BIT_6
#define C10GB_AN_PAGE_RX_SHIFT                 (6U)

/*
 *  Auto Negotiation Parallel Fault Detected
 *
 *  Parallel_Detect_fault. Clear on read
 */
#define C10GB_AN_PARALLEL_DETECT_OFFSET        C10GB_AN_STATUS_REG_OFFSET
#define C10GB_AN_PARALLEL_DETECT_MASK          MASK_BIT_9
#define C10GB_AN_PARALLEL_DETECT_SHIFT         (9U)

/*
 *  Auto Negotiation State Variable
 *
 *  This register returns the state variables of the Auto-Negotiation.
 *  ST_AUTO_NEG_ENABLE          = 0x0;
 *  ST_TRANSMIT_DISABLE         = 0x1;
 *  ST_CAPABILITY_DETECT           = 0x2;
 *  ST_ACKNOWLEDGE_DETECT       = 0x3;
 *  ST_COMPLETE_ACKNOWLE        = 0x4;
 *  ST_AN_GOOD_CHECK            = 0x5;
 *  ST_AN_GOOD                  = 0x6;
 *  ST_NEXT_PAGE_WAIT           = 0x7;
 *  ST_NEXT_PAGE_WAIT_TX_IDLE   = 0x8;
 *  ST_LINK_STATUS_CHECK        = 0x9;
 *  ST_PARALLEL_DETECTION_FAULT = 0xA;
 */
#define C10GB_AN_STATE_OFFSET                  C10GB_AN_STATUS_REG_OFFSET
#define C10GB_AN_STATE_SHIFT                   (12U)
#define C10GB_AN_STATE_MASK                    BIT_MASK_4_BITS << \
                                                    C10GB_AN_STATE_SHIFT


/*------------------------------------------------------------------------------
 * MR_ADV_CAPABILITY_1 Bits [16:1]
 *
 * This register gives the value of the bits 16:1 of the Advertisement Ability
 * Register
 * Bits [4:0] - Selector Field (S[4:0]) is a five-bit wide field, encoding 32
 * possible messages. Selector Field encoding definitions are shown in Annex
 * 28A of the IEEE 802.3 specification
 * Bits[9:5] -Echoed Nonce Field (E[4:0]) is a 5-bit wide field containing
 * the nonce received from the link partner
 * Bits[12:10]  - Pause Encoding as defined in Annexure 28B of the IEEE 802.3
 * specification
 * Bit 13 - Remote Fault (RF) of the base link codeword. The default value is
 * logical zero
 * Bit 14 -Acknowledge (Ack) is used by the Auto-Negotiation function to
 * indicate that a device has successfully received its link partner’s link
 * codeword.
 * Bit 15 -Next Page (NP) bit. Support of Next Pages is mandatory. If the
 * device does not have any Next Pages to send, the NP bit shall be set to
 * logical zero
 */
#define C10GB_AN_MR_ADV_CAPABILITY_1_REG_OFFSET \
                                               (0x10U << 2U)



/*------------------------------------------------------------------------------
 * MR_ADV_CAPABILITY_2 Bits [32:17]
 *
 * This register gives the value of the bits 32:17 of the Advertisement Ability
 * Register
 * Bits[20:16] – Transmitted Nonce Field
 * Bits[31:21] – Technology Ability Field bits[ 10:0] . Technology Ability
 * Field is a 25-bit wide field containing information indicating supported
 * technologies specific to the selector field value when used with the
 * Auto-Negotiation for Backplane Ethernet.
 */
#define C10GB_AN_MR_ADV_CAPABILITY_2_REG_OFFSET \
                                               (0x11U << 2U)


/*------------------------------------------------------------------------------
 * MR_ADV_CAPABILITY_3 Bits [48:33]
 *
 * This register gives the value of the bits 48:33 of the Advertisement Ability
 * Register
 * Bits [45:32] – Technology Ability Field bits[ 25:11]. For Technology Ability
 * Field encoding refer to Table 73-4 of the IEEE specification
 * Bits [47:46] -  FEC Capability (FEC Not supported in current implementation).
 * Bit 46 -  FEC ability, if set to ‘1’ PHY has FEC Ability
 * Bit 47 - FEC requested When the FEC requested bit is set to logical one, it
 * indicates a request to enable FEC on the link.
 */
#define C10GB_AN_MR_ADV_CAPABILITY_3_REG_OFFSET \
                                               (0x12U << 2U)


/*
 *  Adevertisment Ability Registers Details
 */
#define C10GB_AN_ADV_CAPABILITY_SHIFT          (0U)
#define C10GB_AN_ADV_CAPABILITY_MASK           BIT_MASK_16_BITS << \
                                                C10GB_AN_ADV_CAPABILITY_SHIFT


/*------------------------------------------------------------------------------
 * MR_LP_BASE_PAGE_CAPABILITY_1 Bits [16:1]
 *
 * This register gives the value of the bits 16:1 of the Link Partner Base Page
 * Ability Register. The bits definition are same as Advertisement ability
 * register -1
 */
#define C10GB_AN_MR_LP_BASE_PG_CAPABILITY_1_REG_OFFSET \
                                               (0x13U << 2U)


/*------------------------------------------------------------------------------
 * MR_LP_BASE_PAGE_CAPABILITY_2 Bits [32:17]
 *
 * This register gives the value of the bits 32:17 of the Link Partner Base Page
 * Ability Register. The bits definition are same as Advertisement ability
 * register -2
 */
#define C10GB_AN_MR_LP_BASE_PG_CAPABILITY_2_REG_OFFSET \
                                               (0x14U << 2U)


/*------------------------------------------------------------------------------
 * MR_LP_BASE_PAGE_CAPABILITY_3 Bits [48:33]
 *
 * This register gives the value of the bits 48:33 of the Link Partner Base Page
 * Ability Register. The bits definition are same as Advertisement ability
 * register -3
 *
 */
#define C10GB_AN_MR_LP_BASE_PG_CAPABILITY_3_REG_OFFSET \
                                               (0x15U << 2U)

/*
 *  Link Partner Base Page Ability Registers Details
 */
#define C10GB_AN_LP_BASE_PAGE_CAPABILITY_SHIFT (0U)
#define C10GB_AN_LP_BASE_PAGE_CAPABILITY_MASK  BIT_MASK_16_BITS << \
                                               C10GB_AN_LP_BASE_PAGE_CAPABILITY_SHIFT

/*------------------------------------------------------------------------------
 * MR_XNP_TRANSMIT_1 Bits [16:1]
 *
 * This register gives the value of the bits 16:1 of the XNP(next page)Transmit
 * Register.
 * Bit[10:0] – Message Code Field (M[10:0]) is an eleven bit wide field,
 * encoding 2048 possible messages. Message Code Field definitions are shown in
 * Annex 28C of the IEEE 802.3 specification
 * Bit 11 – Toggle (T) is used by the Arbitration function to ensure
 * synchronization with the Link Partner during Next Page exchange. This bit
 * shall always take the opposite value of the Toggle bit in the previously
 * exchanged link codeword
 * Bit 12 – Acknowledge 2 (Ack2) is used by the Next Page function to indicate
 * that a device has the ability to comply with the message. Ack2 shall be set
 * as follows:
 * 0 - cannot comply with message
 * 1 - will comply with message
 * Bit 13 – Message Page bit
 * 0 – Unformatted Page
 * 1 – Message Page
 * Bit 14 – Acknowledge. Acknowledge (Ack) is used by the Auto-Negotiation
 * function to indicate that a device has successfully received its Link
 * Partner’s link codeword
 * Bit 15 - Next Page Bit
 * 0 - last page
 * 1 - additional Next Page(s) will follow
 */
#define C10GB_AN_MR_XNP_TRANSMIT_1_REG_OFFSET  (0x16U << 2U)


/*------------------------------------------------------------------------------
 * MR_XNP_TRANSMIT_2 Bits [32:17]
 *
 * This register gives the value of the bits 32:17 of the XNP Transmit Register.
 * This value corresponds to the Bits[31:16] of the Unformatted Code Field of
 * the Next Page Register.
 */
#define C10GB_AN_MR_XNP_TRANSMIT_2_REG_OFFSET	(0x17U << 2U)


/*------------------------------------------------------------------------------
 * MR_XNP_TRANSMIT_3 Bits [48:33]
 *
 * This register gives the value of the bits 48:33 of the XNP Transmit Register.
 * This value corresponds to the Bits[47:32] of the Unformatted Code Field of
 * the Next Page Register.
 */
#define C10GB_AN_MR_XNP_TRANSMIT_3_REG_OFFSET  (0x18U << 2U)

/*
 *  XNP Transmit Registers Details
 */
#define C10GB_AN_XNP_TRANSMIT_SHIFT            (0U)
#define C10GB_AN_XNP_TRANSMIT_MASK             BIT_MASK_16_BITS << \
                                                   C10GB_AN_XNP_TRANSMIT_SHIFT

/*------------------------------------------------------------------------------
 * LP_XNP_CAPABILITY_1 Bits [16:1]
 *
 * This register gives the value of the bits 16:1 of the Link Partner next page
 * Ability register. The bits definition are same as XNP Transmit register -1
 */
#define C10GB_AN_LP_XNP_CAPABILITY_1_REG_OFFSET \
                                               (0x19U << 2U)


/*------------------------------------------------------------------------------
 * LP_XNP_CAPABILITY_2 Bits [32:17]
 *
 * This register gives the value of the bits 32:17 of the Link Partner next page
 * Ability register. The bits definition are same as XNP Transmit register -2
 */
#define C10GB_AN_LP_XNP_CAPABILITY_2_REG_OFFSET \
                                               (0x1aU << 2U)


/*------------------------------------------------------------------------------
 * LP_XNP_CAPABILITY_3 Bits [48:33]
 *
 * This register gives the value of the bits 48:33 of the Link Partner next page
 * Ability register. The bits definition are same as XNP Transmit register -3
 */
#define C10GB_AN_LP_XNP_CAPABILITY_3_REG_OFFSET \
                                               (0x1bU << 2U)

/*
 *  Link Partner Next Page Ability Registers Details
 */
#define C10GB_AN_LP_XNP_CAPABILITY_SHIFT       (0U)
#define C10GB_AN_LP_XNP_CAPABILITY_MASK        BIT_MASK_16_BITS << \
                                                C10GB_AN_LP_XNP_CAPABILITY_SHIFT

/*------------------------------------------------------------------------------
 * AN Link Fail Inhibit Timer
 *
 * An_link_fail_inhibit_timer is 500ms timer which starts incrementing in
 * AN GOOD CHECK state  (This state is indication of AN completion) and if it
 * elapses in during this state then the Auto negotiation process starts again
 */
#define C10GB_AN_LINK_FAIL_INHIBIT_TIMER_REG_OFFSET \
                                               (0x0dU << 2U)

/******************************************************************************/
/*             Core10GBaseKR PHY Clause 72 (Link Training)                    */
/******************************************************************************/

/*------------------------------------------------------------------------------
 * Link Training Control Register
 */
#define C10GB_LT_CTRL_REG_OFFSET               (0x0U << 2U)

/*
 *  Link Training Initialize
 *
 *  If “1” initialize coefficient is transmitted to the link partner
 */
#define C10GB_LT_INIT_OFFSET                   C10GB_LT_CTRL_REG_OFFSET
#define C10GB_LT_INIT_SHIFT                    (3U)
#define C10GB_LT_INIT_MASK                     MASK_BIT_3

/*
 *  Link Training Preset
 *
 *  If “1” preset coefficient is transmitted to the link partner
 */
#define C10GB_LT_PRESET_OFFSET                 C10GB_LT_CTRL_REG_OFFSET
#define C10GB_LT_PRESET_SHIFT                  (2U)
#define C10GB_LT_PRESET_MASK                   MASK_BIT_2

/*
 *  Link Restart Training
 *
 *  A write of “1” to this field brings the link training IP in reset mode.
 *  To bring the Link training IP out of reset, write a value of 0x0 into this
 *  register
 */
#define C10GB_LT_RESTART_TRAINING_OFFSET       C10GB_LT_CTRL_REG_OFFSET
#define C10GB_LT_RESTART_TRAINING_SHIFT        (1U)
#define C10GB_LT_RESTART_TRAINING_MASK         MASK_BIT_1

/*
 *  Link Training Enable
 *
 *  If “1” enables the link training. This bit should be enabled from the
 *  software after the AN_GOOD_CHK interrupt is received by the Firmware
 */
#define C10GB_LT_RESTART_EN_OFFSET             C10GB_LT_CTRL_REG_OFFSET
#define C10GB_LT_RESTART_EN_SHIFT              (0U)
#define C10GB_LT_RESTART_EN_MASK               MASK_BIT_0

/*------------------------------------------------------------------------------
 * Max Wait Timer Configuration
 *
 * The value in the MAX wait timer should be configured to 500ms, the training
 * state diagram enters into the training failure state once the timer reaches
 * 500ms.
 */
#define C10GB_LT_MAX_WAIT_TIMER_REG_OFFSET     (0x1U << 2U)

#define C10GB_LT_MAX_WAIT_TIMER_OFFSET         C10GB_LT_MAX_WAIT_TIMER_REG_OFFSET
#define C10GB_LT_MAX_WAIT_TIMER_SHIFT          (0U)
#define C10GB_LT_MAX_WAIT_TIMER_MASK           BIT_MASK_32_BITS << \
                                                   C10GB_LT_MAX_WAIT_TIMER_SHIFT

/*------------------------------------------------------------------------------
 * Frame WAIT Timer Configuration Register
 *
 * The value in the frame WAIT Timer register should be between 100 to 300
 * frames. The default value is configured to 100 frames.
 */
#define C10GB_LT_FRM_WAIT_TIMER_REG_OFFSET     (0x2U << 2U)

#define C10GB_LT_FRM_WAIT_TIMER_OFFSET         C10GB_LT_FRM_WAIT_TIMER_REG_OFFSET
#define C10GB_LT_FRM_WAIT_TIMER_SHIFT          (0U)
#define C10GB_LT_FRM_WAIT_TIMER_MASK           BIT_MASK_32_BITS << \
                                                 C10GB_LT_FRM_WAIT_TIMER_SHIFT

/*------------------------------------------------------------------------------
 * Preset MAIN-TAP Configuration Register
 *
 * The value in this register gives the main TAP value, when the preset
 * Coefficient is received from the Link Partner. The Link Training Algorithm
 * starts from this TAP value.
 */
#define C10GB_LT_PRESET_MAIN_TAP_REG_OFFSET    (0x3U << 2U)

#define C10GB_LT_PRESET_MAIN_TAP_OFFSET        C10GB_LT_PRESET_MAIN_TAP_REG_OFFSET
#define C10GB_LT_PRESET_MAIN_TAP_SHIFT         (0U)
#define C10GB_LT_PRESET_MAIN_TAP_MASK          BIT_MASK_16_BITS << \
                                                  C10GB_LT_PRESET_MAIN_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Preset Post-TAP Configuration Register
 *
 * The value in this register gives the post-TAP value when the preset
 * Coefficient is received from the Link Partner. The Link Training Algorithm
 * starts from this TAP value.
 */
#define C10GB_LT_PRESET_POST_TAP_REG_OFFSET    (0x4U << 2U)

#define C10GB_LT_PRESET_POST_TAP_OFFSET        C10GB_LT_PRESET_POST_TAP_REG_OFFSET
#define C10GB_LT_PRESET_POST_TAP_SHIFT         (0U)
#define C10GB_LT_PRESET_POST_TAP_MASK          BIT_MASK_16_BITS << \
                                                  C10GB_LT_PRESET_POST_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Preset Pre-TAP Configuration Register
 *
 * The value in this register gives the pre-TAP when the preset Coefficient is
 * received from the Link Partner. The Link Training Algorithm starts from this
 * TAP value.
 */
#define C10GB_LT_PRESET_PRE_TAP_REG_OFFSET     (0x5U << 2U)

#define C10GB_LT_PRESET_PRE_TAP_OFFSET         C10GB_LT_PRESET_PRE_TAP_REG_OFFSET
#define C10GB_LT_PRESET_PRE_TAP_SHIFT          (0U)
#define C10GB_LT_PRESET_PRE_TAP_MASK           BIT_MASK_16_BITS << \
                                                   C10GB_LT_PRESET_PRE_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Initialize MAIN-TAP Configuration Register
 *
 * The value in this register gives the main TAP value, when the Initialize
 * Coefficient is received from the Link Partner.
 */
#define C10GB_LT_INIT_MAIN_TAP_REG_OFFSET      (0x6U << 2U)

#define C10GB_LT_INIT_MAIN_TAP_OFFSET          C10GB_LT_INIT_MAIN_TAP_REG_OFFSET
#define C10GB_LT_INIT_MAIN_TAP_SHIFT           (0U)
#define C10GB_LT_INIT_MAIN_TAP_MASK            BIT_MASK_16_BITS << \
                                                    C10GB_LT_INIT_MAIN_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Initialize Post-TAP Configuration Register
 *
 * The value in this register gives the post-TAP value when the Initialize
 * Coefficient is received from the Link Partner.
 */
#define C10GB_LT_INIT_POST_TAP_REG_OFFSET      (0x7U << 2U)

#define C10GB_LT_INIT_POST_TAP_OFFSET          C10GB_LT_INIT_POST_TAP_REG_OFFSET
#define C10GB_LT_INIT_POST_TAP_SHIFT           (0U)
#define C10GB_LT_INIT_POST_TAP_MASK            BIT_MASK_16_BITS << \
                                                    C10GB_LT_INIT_POST_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Initialize Pre-TAP Configuration Register
 *
 * The value in this register gives the pre-TAP when the Initialize Coefficient
 * is received from the Link Partner.
 */
#define C10GB_LT_INIT_PRE_TAP_REG_OFFSET       (0x8U << 2U)

#define C10GB_LT_INIT_PRE_TAP_OFFSET           C10GB_LT_INIT_PRE_TAP_REG_OFFSET
#define C10GB_LT_INIT_PRE_TAP_SHIFT            (0U)
#define C10GB_LT_INIT_PRE_TAP_MASK             BIT_MASK_16_BITS << \
                                                    C10GB_LT_INIT_PRE_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Maximum Coefficient Limit of Main Tap Configuration Register
 *
 * The value in this register gives the Maximum value of the main TAP setting
 * which will be tested for the TX equalization for the optimum main tap
 * settings.
 */
#define C10GB_LT_MAX_MAIN_TAP_REG_OFFSET       (0x9U << 2U)

#define C10GB_LT_MAX_MAIN_TAP_OFFSET           C10GB_LT_MAX_MAIN_TAP_REG_OFFSET
#define C10GB_LT_MAX_MAIN_TAP_SHIFT            (0U)
#define C10GB_LT_MAX_MAIN_TAP_MASK             BIT_MASK_16_BITS << \
                                                    C10GB_LT_MAX_MAIN_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Minimum Coefficient Limit of Main Tap Configuration Register
 *
 * The value in this register gives the minimum value of the main TAP setting
 * which will be tested for the TX equalization for the optimum main tap
 * settings.
 */
#define C10GB_LT_MIN_MAIN_TAP_REG_OFFSET       (0xAU << 2U)

#define C10GB_LT_MIN_MAIN_TAP_OFFSET           C10GB_LT_MIN_MAIN_TAP_REG_OFFSET
#define C10GB_LT_MIN_MAIN_TAP_SHIFT            (0U)
#define C10GB_LT_MIN_MAIN_TAP_MASK             BIT_MASK_16_BITS << \
                                                     C10GB_LT_MIN_MAIN_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Maximum Coefficient Limit of Post Tap Configuration Register
 *
 * The value in this register gives the Maximum value of the post TAP setting
 * which will be tested for the TX equalization for the optimum post tap
 * settings.
 */
#define C10GB_LT_MAX_POST_TAP_REG_OFFSET       (0xBU << 2U)

#define C10GB_LT_MAX_POST_TAP_OFFSET           C10GB_LT_MAX_POST_TAP_REG_OFFSET
#define C10GB_LT_MAX_POST_TAP_SHIFT            (0U)
#define C10GB_LT_MAX_POST_TAP_MASK             BIT_MASK_16_BITS << \
                                                     C10GB_LT_MAX_POST_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Minimum Coefficient Limit of Post Tap Configuration Register
 *
 * The value in this register gives the minimum value of the post TAP setting
 * which will be tested for the TX equalization for the optimum post tap
 * settings.
 */
#define C10GB_LT_MIN_POST_TAP_REG_OFFSET       (0xCU << 2U)

#define C10GB_LT_MIN_POST_TAP_OFFSET           C10GB_LT_MIN_POST_TAP_REG_OFFSET
#define C10GB_LT_MIN_POST_TAP_SHIFT            (0U)
#define C10GB_LT_MIN_POST_TAP_MASK             BIT_MASK_16_BITS << \
                                                     C10GB_LT_MIN_POST_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Maximum Coefficient Limit of Pre Tap Configuration Register
 *
 * The value in this register gives the Maximum value of the pre TAP setting
 * which will be tested for the TX equalization for the optimum pre tap
 * settings.
 */
#define C10GB_LT_MAX_PRE_TAP_REG_OFFSET        (0xDU << 2U)

#define C10GB_LT_MAX_PRE_TAP_OFFSET            C10GB_LT_MAX_PRE_TAP_REG_OFFSET
#define C10GB_LT_MAX_PRE_TAP_SHIFT             (0U)
#define C10GB_LT_MAX_PRE_TAP_MASK              BIT_MASK_16_BITS << \
                                                    C10GB_LT_MAX_PRE_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Minimum Coefficient Limit of Pre Tap Configuration Register
 *
 * The value in this register gives the minimum value of the pre TAP setting
 * which will be tested for the TX equalization for the optimum pre tap
 * settings.
 */
#define C10GB_LT_MIN_PRE_TAP_REG_OFFSET        (0xEU << 2U)

#define C10GB_LT_MIN_PRE_TAP_OFFSET            C10GB_LT_MIN_PRE_TAP_REG_OFFSET
#define C10GB_LT_MIN_PRE_TAP_SHIFT             (0U)
#define C10GB_LT_MIN_PRE_TAP_MASK              BIT_MASK_16_BITS << \
                                                    C10GB_LT_MIN_PRE_TAP_SHIFT

/*------------------------------------------------------------------------------
 * TX Equalization Register
 */
#define C10GB_LT_TX_EQUAL_REG_OFFSET           (0xFU << 2U)

/*
 *  TX Equalization Pre Tap Done
 *
 *  A write to “1” in this field indicates that that tx_equalization is done for
 *  the PRE TAP
 */
#define C10GB_LT_TX_EQUAL_PRE_DONE_OFFSET      C10GB_LT_TX_EQUAL_REG_OFFSET
#define C10GB_LT_TX_EQUAL_PRE_DONE_SHIFT       (3U)
#define C10GB_LT_TX_EQUAL_PRE_DONE_MASK        MASK_BIT_3

/*
 *  TX Equalization Post Tap Done
 *
 *  A write to “1” in this field indicates that that tx_equalization is done for
 *  the post TAP
 */
#define C10GB_LT_TX_EQUAL_POST_DONE_OFFSET     C10GB_LT_TX_EQUAL_REG_OFFSET
#define C10GB_LT_TX_EQUAL_POST_DONE_SHIFT      (2U)
#define C10GB_LT_TX_EQUAL_POST_DONE_MASK       MASK_BIT_2

/*
 *  TX Equalization Main Tap Done
 *
 *  A write to “1” in this field indicates that that tx_equalization is done for
 *  the main TAP
 */
#define C10GB_LT_TX_EQUAL_MAIN_DONE_OFFSET     C10GB_LT_TX_EQUAL_REG_OFFSET
#define C10GB_LT_TX_EQUAL_MAIN_DONE_SHIFT      (1U)
#define C10GB_LT_TX_EQUAL_MAIN_DONE_MASK       MASK_BIT_1

/*
 *  TX Equalization Done
 *
 *  A write to “1” in this field indicates that that tx_equalization is done for
 *  all the TAPs (pre, post and main)
 */
#define C10GB_LT_TX_EQUAL_DONE_OFFSET          C10GB_LT_TX_EQUAL_REG_OFFSET
#define C10GB_LT_TX_EQUAL_DONE_SHIFT           (0U)
#define C10GB_LT_TX_EQUAL_DONE_MASK            MASK_BIT_0

/*------------------------------------------------------------------------------
 * Local Receiver Lock Register
 */
#define C10GB_LT_LOCAL_RCVR_LOCK_REG_OFFSET    (0x10U << 2U)

/*
 *  Local Receiver Locked
 *
 *  Write “1” to this register from the software once the local receiver is
 *  ready
 */
#define C10GB_LT_LOCAL_RCVR_LOCKED_OFFSET      C10GB_LT_LOCAL_RCVR_LOCK_REG_OFFSET
#define C10GB_LT_LOCAL_RCVR_LOCKED_SHIFT       (0U)
#define C10GB_LT_LOCAL_RCVR_LOCKED_MASK        MASK_BIT_0

/*------------------------------------------------------------------------------
 * TX New Main Tap Register
 */
#define C10GB_LT_TX_NEW_MAIN_TAP_REG_OFFSET    (0x11U << 2U)

/*
 *  TX New Main Tap
 *
 *  A value in this register gives the value of the new main tap value, which
 *  is used to update the TX equalization SerDes registers.
 */
#define C10GB_LT_TX_NEW_MAIN_TAP_OFFSET        C10GB_LT_TX_NEW_MAIN_TAP_REG_OFFSET
#define C10GB_LT_TX_NEW_MAIN_TAP_SHIFT         (0U)
#define C10GB_LT_TX_NEW_MAIN_TAP_MASK          BIT_MASK_8_BITS << \
                                                  C10GB_LT_TX_NEW_MAIN_TAP_SHIFT

/*------------------------------------------------------------------------------
 * TX New Post Tap Register
 */
#define C10GB_LT_TX_NEW_POST_TAP_REG_OFFSET    (0x12U << 2U)

/*
 *  TX New Post Tap
 *
 *  A value in this register gives the value of the new Post tap value, which
 *  is used to update the TX equalization SerDes registers.
 */
#define C10GB_LT_TX_NEW_POST_TAP_OFFSET        C10GB_LT_TX_NEW_POST_TAP_REG_OFFSET
#define C10GB_LT_TX_NEW_POST_TAP_SHIFT         (0U)
#define C10GB_LT_TX_NEW_POST_TAP_MASK          BIT_MASK_8_BITS << \
                                                  C10GB_LT_TX_NEW_POST_TAP_SHIFT

/*------------------------------------------------------------------------------
 * TX New Pre Tap Register
 */
#define C10GB_LT_TX_NEW_PRE_TAP_REG_OFFSET     (0x13U << 2U)

/*
 *  TX New Pre Tap
 *
 *  A value in this register gives the value of the new Pre tap value, which is
 *  used to update the TX equalization SerDes registers.
 */
#define C10GB_LT_TX_NEW_PRE_TAP_OFFSET         C10GB_LT_TX_NEW_PRE_TAP_REG_OFFSET
#define C10GB_LT_TX_NEW_PRE_TAP_SHIFT          (0U)
#define C10GB_LT_TX_NEW_PRE_TAP_MASK           BIT_MASK_8_BITS << \
                                                   C10GB_LT_TX_NEW_PRE_TAP_SHIFT

/*------------------------------------------------------------------------------
 * Training State Machine Status
 */
#define C10GB_LT_TRAINING_SM_STATUS_REG_OFFSET (0x14U << 2U)

/*
 *  Remote Receiver Ready
 *
 *  If “1” it indicates that remote receiver is ready
 */
#define C10GB_LT_REMOTE_TRAINED_OFFSET         C10GB_LT_TRAINING_SM_STATUS_REG_OFFSET
#define C10GB_LT_REMOTE_TRAINED_SHIFT          (4U)
#define C10GB_LT_REMOTE_TRAINED_MASK           MASK_BIT_4

/*
 *  Link Up
 *
 *  If “1” it indicates that Link is ready
 */
#define C10GB_LT_LINK_UP_OFFSET                C10GB_LT_TRAINING_SM_STATUS_REG_OFFSET
#define C10GB_LT_LINK_UP_SHIFT                 (3U)
#define C10GB_LT_LINK_UP_MASK                  MASK_BIT_3

/*
 *  Training State Machine
 *
 *  The value in this register gives the current values of the training state
 *  machine.
 *  IDLE            = 3'b000
 *  INITIALIZE      = 3'b001
 *  SEND_TRAINING   = 3'b011
 *  TRAIN_LOCAL     = 3'b010
 *  TRAIN_REMOTE    = 3'b110
 *  LINK_READY      = 3'b111
 *  SEND_DATA       = 3'b101
 *  RAINING_FAILURE = 3'b100

 */
#define C10GB_LT_TRAINING_SM_OFFSET            C10GB_LT_TRAINING_SM_STATUS_REG_OFFSET
#define C10GB_LT_TRAINING_SM_SHIFT             (0U)
#define C10GB_LT_TRAINING_SM_MASK              BIT_MASK_2_BITS << \
                                                      C10GB_LT_TRAINING_SM_SHIFT

/*------------------------------------------------------------------------------
 * TX Updated Status Register
 */
#define C10GB_LT_TX_UPDATED_STATUS_REG_OFFSET  (0x15U << 2U)

/*
 *  Tx Updated Status Pre Tap
 *
 *  A value in this register gives the value of the current status transmitted
 *  to the link partner for the pre Tap
 *  1 1 = maximum
 *  1 0 = minimum
 *  0 1 = updated
 *  0 0 = not_updated
 */
#define C10GB_LT_TX_UPDATED_STAT_PRE_OFFSET    C10GB_LT_TX_UPDATED_STATUS_REG_OFFSET
#define C10GB_LT_TX_UPDATED_STAT_PRE_SHIFT     (16U)
#define C10GB_LT_TX_UPDATED_STAT_PRE_MASK      BIT_MASK_2_BITS << \
                                               C10GB_LT_TX_UPDATED_STAT_PRE_SHIFT

/*
 *  Tx Updated Status Post Tap
 *
 *  A value in this register gives the value of the current status transmitted
 *  to the link partner for the post Tap
 *  1 1 = maximum
 *  1 0 = minimum
 *  0 1 = updated
 *  0 0 = not_updated
 */
#define C10GB_LT_TX_UPDATED_STAT_POST_OFFSET   C10GB_LT_TX_UPDATED_STATUS_REG_OFFSET
#define C10GB_LT_TX_UPDATED_STAT_POST_SHIFT    (14U)
#define C10GB_LT_TX_UPDATED_STAT_POST_MASK     BIT_MASK_2_BITS << \
                                               C10GB_LT_TX_UPDATED_STAT_POST_SHIFT

/*
 *  Tx Updated Status Main Tap
 *
 *  A value in this register gives the value of the current status transmitted
 *  to the link partner for the main Tap
 *  1 1 = maximum
 *  1 0 = minimum
 *  0 1 = updated
 *  0 0 = not_updated
 */
#define C10GB_LT_TX_UPDATED_STAT_MAIN_OFFSET   C10GB_LT_TX_UPDATED_STATUS_REG_OFFSET
#define C10GB_LT_TX_UPDATED_STAT_MAIN_SHIFT    (12U)
#define C10GB_LT_TX_UPDATED_STAT_MAIN_MASK     BIT_MASK_2_BITS << \
                                               C10GB_LT_TX_UPDATED_STAT_MAIN_SHIFT

/*
 *  Link Training State Pre Tap
 *
 *  A value in this register gives the value of the coefficient update state
 *  machine for the pre Tap
 *  IDLE         = 3'b000
 *  NOT_UPDATED  = 3'b001
 *  UPDATE_COEFF = 3'b011
 *  MAXIMUM      = 3'b010
 *  UPDATED      = 3'b110
 *  MINIMUM      = 3'b100
 */
#define C10GB_LT_STATE_PRE_OFFSET              C10GB_LT_TX_UPDATED_STATUS_REG_OFFSET
#define C10GB_LT_STATE_PRE_SHIFT               (8U)
#define C10GB_LT_STATE_PRE_MASK                BIT_MASK_3_BITS << \
                                                        C10GB_LT_STATE_PRE_SHIFT

/*
 *  Link Training State Post Tap
 *
 *  A value in this register gives the value of the coefficient update state
 *  machine for the post Tap
 *  IDLE         = 3'b000
 *  NOT_UPDATED  = 3'b001
 *  UPDATE_COEFF = 3'b011
 *  MAXIMUM      = 3'b010
 *  UPDATED      = 3'b110
 *  MINIMUM      = 3'b100
 */
#define C10GB_LT_STATE_POST_OFFSET             C10GB_LT_TX_UPDATED_STATUS_REG_OFFSET
#define C10GB_LT_STATE_POST_SHIFT              (4U)
#define C10GB_LT_STATE_POST_MASK               BIT_MASK_3_BITS << \
                                                    C10GB_LT_STATE_POST_SHIFT

/*
 *  Link Training State Main Tap
 *
 *  A value in this register gives the value of the coefficient update state
 *  machine for the main Tap
 *  IDLE         = 3'b000
 *  NOT_UPDATED  = 3'b001
 *  UPDATE_COEFF = 3'b011
 *  MAXIMUM      = 3'b010
 *  UPDATED      = 3'b110
 *  MINIMUM      = 3'b100
 */
#define C10GB_LT_STATE_MAIN_OFFSET             C10GB_LT_TX_UPDATED_STATUS_REG_OFFSET
#define C10GB_LT_STATE_MAIN_SHIFT              (0U)
#define C10GB_LT_STATE_MAIN_MASK               BIT_MASK_3_BITS << \
                                                       C10GB_LT_STATE_MAIN_SHIFT

/*------------------------------------------------------------------------------
 * Received Coefficient Status Register
 */
#define C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET  (0x16U << 2U)

/*
 *  New Coefficient Received
 *
 *  A value in this register gives the value of the new coefficient received
 *  from the link partner
 */
#define C10GB_LT_NEW_RCVD_COEFF_OFFSET         C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_NEW_RCVD_COEFF_SHIFT          (16U)
#define C10GB_LT_NEW_RCVD_COEFF_MASK           BIT_MASK_16_BITS << \
                                                   C10GB_LT_NEW_RCVD_COEFF_SHIFT

/*
 *  Preset
 *
 *  1 = Pre-set coefficients
 *  0 = Normal operation
 */
#define C10GB_LT_RCVD_COEFF_PRESET_OFFSET      C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_PRESET_SHIFT       (29U)
#define C10GB_LT_RCVD_COEFF_PRESET_MASK        MASK_BIT_29

/*
 *  Initialize
 *
 *  1 = Initialize coefficients
 *  0 = Normal operation
 */
#define C10GB_LT_RCVD_COEFF_INIT_OFFSET        C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_INIT_SHIFT         (28U)
#define C10GB_LT_RCVD_COEFF_INIT_MASK          MASK_BIT_28

/*
 *  Post tap coefficient update
 *
 *  1 1 = Reserved
 *  0 1 = Increment
 *  1 0 = Decrement
 *  0 0 = Hold
 */
#define C10GB_LT_RCVD_COEFF_POST_UPDATE_OFFSET C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_POST_UPDATE_SHIFT  (20U)
#define C10GB_LT_RCVD_COEFF_POST_UPDATE_MASK   BIT_MASK_2_BITS << \
                                                C10GB_LT_RCVD_COEFF_POST_UPDATE_SHIFT

/*
 *  Main tap coefficient update
 *
 *  1 1 = Reserved
 *  0 1 = Increment
 *  1 0 = Decrement
 *  0 0 = Hold
 */
#define C10GB_LT_RCVD_COEFF_MAIN_UPDATE_OFFSET C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_MAIN_UPDATE_SHIFT  (18U)
#define C10GB_LT_RCVD_COEFF_MAIN_UPDATE_MASK   BIT_MASK_2_BITS << \
                                                C10GB_LT_RCVD_COEFF_MAIN_UPDATE_SHIFT

/*
 *  Pre tap coefficient update
 *
 *  1 1 = Reserved
 *  0 1 = Increment
 *  1 0 = Decrement
 *  0 0 = Hold
 */
#define C10GB_LT_RCVD_COEFF_PRE_UPDATE_OFFSET  C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_PRE_UPDATE_SHIFT   (16U)
#define C10GB_LT_RCVD_COEFF_PRE_UPDATE_MASK    BIT_MASK_2_BITS << \
                                                C10GB_LT_RCVD_COEFF_PRE_UPDATE_SHIFT

/*
 *  New Status Received
 *
 *  A value in this register gives the value of the new status received from the
 *  link partner
 */
#define C10GB_LT_NEW_RCVD_STATUS_OFFSET        C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_NEW_RCVD_STATUS_SHIFT         (0U)
#define C10GB_LT_NEW_RCVD_STATUS_MASK          BIT_MASK_16_BITS << \
                                                  C10GB_LT_NEW_RCVD_STATUS_SHIFT

/*
 *  Receiver Ready
 *
 *  1 = The remote receiver has determined that training is completed and is
 *  prepared to receive data.
 *  0 = The remote receiver is requesting that training continue.
 */
#define C10GB_LT_RCVD_COEFF_RCVR_READY_OFFSET  C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_RCVR_READY_SHIFT   (15U)
#define C10GB_LT_RCVD_COEFF_RCVR_READY_MASK    MASK_BIT_15

/*
 *  Post tap coefficient value
 *
 *  1 1 = Maximum
 *  1 0 = Minimum
 *  0 1 = updated
 *  0 0 = Not Updated
 */
#define C10GB_LT_RCVD_COEFF_POST_VALUE_OFFSET  C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_POST_VALUE_SHIFT   (4U)
#define C10GB_LT_RCVD_COEFF_POST_VALUE_MASK    BIT_MASK_2_BITS << \
                                                C10GB_LT_RCVD_COEFF_POST_VALUE_SHIFT

/*
 *  Main tap coefficient value
 *
 *  1 1 = Maximum
 *  1 0 = Minimum
 *  0 1 = updated
 *  0 0 = Not Updated
 */
#define C10GB_LT_RCVD_COEFF_MAIN_VALUE_OFFSET  C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_MAIN_VALUE_SHIFT   (2U)
#define C10GB_LT_RCVD_COEFF_MAIN_VALUE_MASK    BIT_MASK_2_BITS << \
                                                C10GB_LT_RCVD_COEFF_MAIN_VALUE_SHIFT

/*
 *  Pre tap coefficient value
 *
 *  1 1 = Maximum
 *  1 0 = Minimum
 *  0 1 = updated
 *  0 0 = Not Updated
 */
#define C10GB_LT_RCVD_COEFF_PRE_VALUE_OFFSET   C10GB_LT_RCVD_COEFF_STATUS_REG_OFFSET
#define C10GB_LT_RCVD_COEFF_PRE_VALUE_SHIFT    (0U)
#define C10GB_LT_RCVD_COEFF_PRE_VALUE_MASK     BIT_MASK_2_BITS << \
                                                C10GB_LT_RCVD_COEFF_PRE_VALUE_SHIFT

/*------------------------------------------------------------------------------
 * TX Coefficient Configuration Register
 */
#define C10GB_LT_TX_COEFF_CFG_REG_OFFSET       (0x18U << 2U)

/*
 *  Hold Pre Cursor Send
 *
 *  If “1” then hold coefficient is sent to the link partner for pre cursor
 */
#define C10GB_LT_TX_HOLD_PRE_OFFSET            C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_HOLD_PRE_SHIFT             (10U)
#define C10GB_LT_TX_HOLD_PRE_MASK              MASK_BIT_10

/*
 *  Decrement Pre Cursor Send
 *
 *  If “1” then the decrement coefficient is sent to the link partner for pre
 *  cursor
 */
#define C10GB_LT_TX_DEC_PRE_OFFSET             C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_DEC_PRE_SHIFT              (9U)
#define C10GB_LT_TX_DEC_PRE_MASK               MASK_BIT_9

/*
 *  Increment Pre Cursor Send
 *
 *  If “1” then the increment coefficient is sent to the link partner for pre
 *  cursor
 */
#define C10GB_LT_TX_INC_PRE_OFFSET             C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_INC_PRE_SHIFT              (8U)
#define C10GB_LT_TX_INC_PRE_MASK               MASK_BIT_8

/*
 *  Hold Post Cursor Send
 *
 *  If “1” then the hold coefficient is sent to the link partner for post
 *  cursor
 */
#define C10GB_LT_TX_HOLD_POST_OFFSET           C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_HOLD_POST_SHIFT            (6U)
#define C10GB_LT_TX_HOLD_POST_MASK             MASK_BIT_6

/*
 *  Decrement Post Cursor Send
 *
 *  If “1” then the decrement coefficient is sent to the link partner for post
 *  cursor
 */
#define C10GB_LT_TX_DEC_POST_OFFSET            C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_DEC_POST_SHIFT             (5U)
#define C10GB_LT_TX_DEC_POST_MASK              MASK_BIT_5

/*
 *  Increment Post Cursor Send
 *
 *  If “1” then the increment coefficient is sent to the link partner for post
 *  cursor
 */
#define C10GB_LT_TX_INC_POST_OFFSET            C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_INC_POST_SHIFT             (4U)
#define C10GB_LT_TX_INC_POST_MASK              MASK_BIT_4

/*
 *  Hold Main Cursor Send
 *
 *  If “1” then the hold coefficient is sent to the link partner for mian
 *  cursor
 */
#define C10GB_LT_TX_HOLD_MAIN_OFFSET           C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_HOLD_MAIN_SHIFT            (2U)
#define C10GB_LT_TX_HOLD_MAIN_MASK             MASK_BIT_2

/*
 *  Decrement Main Cursor Send
 *
 *  If “1” then the decrement coefficient is sent to the link partner for main
 *  cursor
 */
#define C10GB_LT_TX_DEC_MAIN_OFFSET            C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_DEC_MAIN_SHIFT             (1U)
#define C10GB_LT_TX_DEC_MAIN_MASK              MASK_BIT_1

/*
 *  Increment Main Cursor Send
 *
 *  If “1” then the increment coefficient is sent to the link partner for main
 *  cursor
 */
#define C10GB_LT_TX_INC_MAIN_OFFSET            C10GB_LT_TX_COEFF_CFG_REG_OFFSET
#define C10GB_LT_TX_INC_MAIN_SHIFT             (0U)
#define C10GB_LT_TX_INC_MAIN_MASK              MASK_BIT_0

/*------------------------------------------------------------------------------
 * PRBS error word count register
 */
#define C10GB_LT_PRBS_ERR_WRD_REG_OFFSET   (0x1FU << 2U)

/*
 *  PRBS error word count
 */
#define C10GB_LT_PRBS_ERR_WRD_CNT_OFFSET       C10GB_LT_PRBS_ERR_WRD_REG_OFFSET
#define C10GB_LT_PRBS_ERR_WRD_CNT_SHIFT        (0U)
#define C10GB_LT_PRBS_ERR_WRD_CNT_MASK         BIT_MASK_32_BITS << \
                                                C10GB_LT_PRBS_ERR_WRD_CNT_SHIFT

/*------------------------------------------------------------------------------
 * 10GBASE-KR Status
 */
#define C10GB_LT_STATUS_REG_OFFSET             (0x26U << 2U)

/*
 *  RX calibration done
 *
 *  1’ Indicates remote receiver calibration is done. The Firmware should set
 *  this bit in response to the RX calibration request.
 *  This Bit will clear the Bit 2 of this Register.
 *
 */
#define C10GB_LT_RX_CAL_DONE_OFFSET            C10GB_LT_STATUS_REG_OFFSET
#define C10GB_LT_RX_CAL_DONE_SHIFT             (6U)
#define C10GB_LT_RX_CAL_DONE_MASK              MASK_BIT_6

/*
 *  Request TX equalization
 *
 *  ‘1’ indicates local receiver is responded for remote receiver rx calibration
 *  request.
 *
 */
#define C10GB_LT_REQ_TX_EQUAL_OFFSET           C10GB_LT_STATUS_REG_OFFSET
#define C10GB_LT_REQ_TX_EQUAL_SHIFT            (5U)
#define C10GB_LT_REQ_TX_EQUAL_MASK             MASK_BIT_5

/*
 *  Signal detect
 *
 *  ‘1’ indicated both local and remote receiver ready.
 *
 */
#define C10GB_LT_SIGNAL_DETECT_OFFSET          C10GB_LT_STATUS_REG_OFFSET
#define C10GB_LT_SIGNAL_DETECT_SHIFT           (4U)
#define C10GB_LT_SIGNAL_DETECT_MASK            MASK_BIT_4

/*
 *  Training Fail
 *
 *  ‘1’ indicates 500ms of time is expired during link training
 *
 */
#define C10GB_LT_TRAINING_FAIL_OFFSET          C10GB_LT_STATUS_REG_OFFSET
#define C10GB_LT_TRAINING_FAIL_SHIFT           (3U)
#define C10GB_LT_TRAINING_FAIL_MASK            MASK_BIT_3

/*
 *  Request RX calibration
 *
 *  ‘1’ Indicates remote receiver is requested for its calibration
 *
 */
#define C10GB_LT_REQ_RX_CAL_OFFSET             C10GB_LT_STATUS_REG_OFFSET
#define C10GB_LT_REQ_RX_CAL_SHIFT              (2U)
#define C10GB_LT_REQ_RX_CAL_MASK               MASK_BIT_2

/*
 *  Link training frame lock
 *
 *  ‘1’ indicates link training frame is detected
 *
 */
#define C10GB_LT_FRAME_LOCK_OFFSET             C10GB_LT_STATUS_REG_OFFSET
#define C10GB_LT_FRAME_LOCK_SHIFT              (1U)
#define C10GB_LT_FRAME_LOCK_MASK               MASK_BIT_1

/*
 *  Auto-negotiation good link check
 *
 *  ‘1’ indicates Auto negotiation is completed
 *
 */
#define C10GB_AN_GOOD_CHECK_OFFSET             C10GB_LT_STATUS_REG_OFFSET
#define C10GB_AN_GOOD_CHECK_SHIFT              (0U)
#define C10GB_AN_GOOD_CHECK_MASK               MASK_BIT_0

/******************************************************************************/
/*                    Core10GBaseKR PHY Transmit Control                      */
/******************************************************************************/

/*------------------------------------------------------------------------------
 * Transmit Control Register
 */
#define C10GB_TX_CTRL_REG_OFFSET               (0x0U << 2U)

/*
 *  PMA Data Select
 *
 *  PMA TX Data Select. Used to select the TX Raw data from the TX Clause Blocks
 *  to Serdes Interface.
 *  2’b00 – PCS Sublayer clause 49 data will be transmitted to the serdes
 *  interface
 *  2’b10 – Auto Negotiation block, PCS sublayer Clause 73 data will be
 *  transmitted to the serdes interface
 *  2’b11 – Link Training block, PCS sublayer Clause 72 data will be transmitted
 *  to the serdes interface
 *  2’b01 – Reserved

 */
#define C10GB_TX_CTRL_PMA_DATA_OFFSET          C10GB_TX_CTRL_REG_OFFSET
#define C10GB_TX_CTRL_PMA_DATA_SHIFT           (0U)
#define C10GB_TX_CTRL_PMA_DATA_MASK            BIT_MASK_2_BITS << \
                                                    C10GB_TX_CTRL_PMA_DATA_SHIFT

/*
 * XCVR LOS
 *
 * Loss of sync signal to the XCVR
 * 1 - LOS signal is enabled, XCVR will lock to reference
 * 0 - LOS signal is disabled, XCVR will lock to data
 */
#define C10GB_TX_CTRL_XCVR_LOS_OFFSET          C10GB_TX_CTRL_REG_OFFSET
#define C10GB_TX_CTRL_XCVR_LOS_SHIFT           (4U)
#define C10GB_TX_CTRL_XCVR_LOS_MASK            MASK_BIT_4

/*
 * PCS Tx Reset
 *
 * Soft reset bit for PCS reset
 * 1 - TX logic is reset
 * 0 - TX logic is not reset
 * Self clearing
 */
#define C10GB_TX_CTRL_TX_RESET_OFFSET          C10GB_TX_CTRL_REG_OFFSET
#define C10GB_TX_CTRL_TX_RESET_SHIFT           (5U)
#define C10GB_TX_CTRL_TX_RESET_MASK            MASK_BIT_5

/*
 * PCS Rx Reset
 *
 * Soft reset bit for PCS reset
 * 1 - RX logic is reset
 * 0 - RX logic is not reset
 * Self clearing
 */
#define C10GB_TX_CTRL_RX_RESET_OFFSET          C10GB_TX_CTRL_REG_OFFSET
#define C10GB_TX_CTRL_RX_RESET_SHIFT           (6U)
#define C10GB_TX_CTRL_RX_RESET_MASK            MASK_BIT_6

/*------------------------------------------------------------------------------
 * IP version register
 */
#define C10GB_IP_VERSION_REG_OFFSET            (0x1U << 2U)

#define C10GB_IP_VERSION_OFFSET                C10GB_IP_VERSION_REG_OFFSET
#define C10GB_IP_VERSION_SHIFT                 (0U)
#define C10GB_IP_VERSION_MASK                  BIT_MASK_32_BITS << \
                                                   C10GB_IP_VERSION_SHIFT


/******************************************************************************/
/*                    Core10GBaseKR PHY Receive Status                        */
/******************************************************************************/

/*------------------------------------------------------------------------------
 * Receive Status Register
 */
#define C10GB_RX_STATUS_REG_OFFSET             (0x0U << 2U)

/*
 *  PCS49 Status
 *
 *  Receive Status signal
 *  1 – Receiver has attained Block Lock
 *  0 – Receiver has not attained the Block Lock
 */
#define C10GB_RX_STATUS_PCS49_OFFSET           C10GB_RX_STATUS_REG_OFFSET
#define C10GB_RX_STATUS_PCS49_SHIFT            (0U)
#define C10GB_RX_STATUS_PCS49_MASK             BIT_MASK_2_BITS << \
                                                     C10GB_RX_STATUS_PCS49_SHIFT

/******************************************************************************/
/*                    Core10GBaseKR_PHY Memory Map                            */
/******************************************************************************/
 #define C10GB_AN_BASE_OFFSET			       (0x0U << 8U)
 #define C10GB_LT_BASE_OFFSET		           (0x4U << 8U)
 #define C10GB_TX_CTRL_BASE_OFFSET             (0x8U << 8U)
 #define C10GB_RX_STATUS_BASE_OFFSET           (0x9U << 8U)

/// @endcond

#ifdef __cplusplus
}
#endif

#endif /* CORE10GBASEKR_PHY_REG_H_ */
