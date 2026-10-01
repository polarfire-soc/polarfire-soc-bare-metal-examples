/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core1588_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core1588 register header file
 *
 */

#ifndef CORE1588_REGS_H_
#define CORE1588_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
 * Macro definitions
 *
 * CORE1588_REGS_<REG_NAME>_REG_OFFSET => Register offset from base address.
 *
 * CORE1588_REGS_<REG_NAME>_REG_LENGTH => Register length in number of bytes.
 *
 * CORE1588_REGS_<REG_NAME>_RW_MASK => This identifies what read/write bits are
 * used within a register.
 *
 * CORE1588_REGS_<REG_NAME>_RO_MASK => This identifies what read only bits are
 * used within a register.
 *
 * CORE1588_REGS_<REG_NAME>_WO_MASK => This identifies what write only bits are
 * used within a register.
 *
 * CORE1588_REGS_<REG_NAME>_READ_MASK => This identifies what read bits are used
 * within a register.
 *
 * CORE1588_REGS_<REG_NAME>_WRITE_MASK => This identifies what write bits are
 * used within a register.
 *
 * CORE1588_REGS_<REG_NAME>_<FIELD_NAME>_OFFSET => This describes the register
 * offset from the base address for a specific field.
 *
 * CORE1588_REGS_<REG_NAME>_<FIELD_NAME>_SHIFT => Bit field shift.
 *
 * CORE1588_REGS_<REG_NAME>_<FIELD_NAME>_NS_MASK => This describes the field
 * mask without the shift included. This mask is based on the width of the bit
 * field.
 *
 * CORE1588_REGS_<REG_NAME>_<FIELD_NAME>_MASK => This describes the field mask
 * with the shift included.
 */

/***************************************************************************************************
 * Register: GCFG
 * Description: General configuration register
 */
#define CORE1588_REGS_GCFG_REG_OFFSET (0x0U)
#define CORE1588_REGS_GCFG_REG_LENGTH (0x4U)
#define CORE1588_REGS_GCFG_REG_RW_MASK (0x0000FC1BU)
#define CORE1588_REGS_GCFG_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_GCFG_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_GCFG_REG_READ_MASK (0x0000FC1BU)
#define CORE1588_REGS_GCFG_REG_WRITE_MASK (0x0000FC1BU)

/**
 * Field Name: LT2_EN
 * Field Desc:  0: Disable latch 2 timestamp registers 1: Enable latch 2
 * timestamp registers This bit must be set to 1 in order for the LT2L and LT2M
 * registers to be updated with the RTC value when a rising edge is observed on
 * the LATCH2 input. Field Type: read-write
 */
#define CORE1588_REGS_GCFG_LT2_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_LT2_EN_SHIFT (15U)
#define CORE1588_REGS_GCFG_LT2_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_LT2_EN_MASK                                         \
  (CORE1588_REGS_GCFG_LT2_EN_NS_MASK << CORE1588_REGS_GCFG_LT2_EN_SHIFT)

/**
 * Field Name: LT1_EN
 * Field Desc:  0: Disable latch 1 timestamp registers 1: Enable latch 1
 * timestamp registers This bit must be set to 1 in order for the LT1L and LT1M
 * registers to be updated with the RTC value when a rising edge is observed on
 * the LATCH1 input. Field Type: read-write
 */
#define CORE1588_REGS_GCFG_LT1_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_LT1_EN_SHIFT (14U)
#define CORE1588_REGS_GCFG_LT1_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_LT1_EN_MASK                                         \
  (CORE1588_REGS_GCFG_LT1_EN_NS_MASK << CORE1588_REGS_GCFG_LT1_EN_SHIFT)

/**
 * Field Name: LT0_EN
 * Field Desc:  0: Disable latch 0 timestamp registers 1: Enable latch 0
 * timestamp registers This bit must be set to 1 in order for the LT0L and LT0M
 * registers to be updated with the RTC value when a rising edge is observed on
 * the LATCH0 input Field Type: read-write
 */
#define CORE1588_REGS_GCFG_LT0_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_LT0_EN_SHIFT (13U)
#define CORE1588_REGS_GCFG_LT0_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_LT0_EN_MASK                                         \
  (CORE1588_REGS_GCFG_LT0_EN_NS_MASK << CORE1588_REGS_GCFG_LT0_EN_SHIFT)

/**
 * Field Name: TT2_EN
 * Field Desc:  0: Disable TRIG2 output 1: Enable TRIG2 output This bit must be
 * set to 1 in order for a pulse to be generated on the TRIG2 output when the
 * RTC value reaches the time set in the TT2L and TT2M registers. Field Type:
 * read-write
 */
#define CORE1588_REGS_GCFG_TT2_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_TT2_EN_SHIFT (12U)
#define CORE1588_REGS_GCFG_TT2_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_TT2_EN_MASK                                         \
  (CORE1588_REGS_GCFG_TT2_EN_NS_MASK << CORE1588_REGS_GCFG_TT2_EN_SHIFT)

/**
 * Field Name: TT1_EN
 * Field Desc:  0: Disable TRIG1 output 1: Enable TRIG1 output This bit must be
 * set to 1 in order for a pulse to be generated on the TRIG1 output when the
 * RTC value reaches the time set in the TT1L and TT1M registers. Field Type:
 * read-write
 */
#define CORE1588_REGS_GCFG_TT1_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_TT1_EN_SHIFT (11U)
#define CORE1588_REGS_GCFG_TT1_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_TT1_EN_MASK                                         \
  (CORE1588_REGS_GCFG_TT1_EN_NS_MASK << CORE1588_REGS_GCFG_TT1_EN_SHIFT)

/**
 * Field Name: TT0_EN
 * Field Desc:  0: Disable TRIG0 output 1: Enable TRIG0 output This bit must be
 * set to 1 in order for a pulse to be generated on the TRIG0 output when the
 * RTC value reaches the time set in the TT0L and TT0M registers. Field Type:
 * read-write
 */
#define CORE1588_REGS_GCFG_TT0_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_TT0_EN_SHIFT (10U)
#define CORE1588_REGS_GCFG_TT0_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_TT0_EN_MASK                                         \
  (CORE1588_REGS_GCFG_TT0_EN_NS_MASK << CORE1588_REGS_GCFG_TT0_EN_SHIFT)

/**
 * Field Name: PTP_UNICAST_EN
 * Field Desc:  Enable detection of PTP frames which uses unicast address. User
 * can configure Unicast IP destination address for PTP transmit and receive
 * frames in registers PTP_TX_UNICAST_ADDR and PTP_RX_UNICAST_ADDR respectively
 * Field Type: read-write
 */
#define CORE1588_REGS_GCFG_PTP_UNICAST_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_PTP_UNICAST_EN_SHIFT (4U)
#define CORE1588_REGS_GCFG_PTP_UNICAST_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_PTP_UNICAST_EN_MASK                                 \
  (CORE1588_REGS_GCFG_PTP_UNICAST_EN_NS_MASK                                   \
   << CORE1588_REGS_GCFG_PTP_UNICAST_EN_SHIFT)

/**
 * Field Name: REQ_EN
 * Field Desc:  0: Core is operating in responder mode 1: Core is operating in
 * requestor mode Field Type: read-write
 */
#define CORE1588_REGS_GCFG_REQ_EN_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_REQ_EN_SHIFT (3U)
#define CORE1588_REGS_GCFG_REQ_EN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_REQ_EN_MASK                                         \
  (CORE1588_REGS_GCFG_REQ_EN_NS_MASK << CORE1588_REGS_GCFG_REQ_EN_SHIFT)

/**
 * Field Name: ONE_STEP_SYNC
 * Field Desc:  One Step Sync Mode. Write 1 to enable. When set to 1, core
 * replace timestamp field in the 1588 header for TX Sync Frames with current
 * RTC value and updates the FCS. Field Type: read-write
 */
#define CORE1588_REGS_GCFG_ONE_STEP_SYNC_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_ONE_STEP_SYNC_SHIFT (1U)
#define CORE1588_REGS_GCFG_ONE_STEP_SYNC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_ONE_STEP_SYNC_MASK                                  \
  (CORE1588_REGS_GCFG_ONE_STEP_SYNC_NS_MASK                                    \
   << CORE1588_REGS_GCFG_ONE_STEP_SYNC_SHIFT)

/**
 * Field Name: ENCOR
 * Field Desc:  0: Disable core 1: Enable core
 * Field Type: read-write
 */
#define CORE1588_REGS_GCFG_ENCOR_OFFSET (CORE1588_REGS_GCFG_REG_OFFSET)
#define CORE1588_REGS_GCFG_ENCOR_SHIFT (0U)
#define CORE1588_REGS_GCFG_ENCOR_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_GCFG_ENCOR_MASK                                          \
  (CORE1588_REGS_GCFG_ENCOR_NS_MASK << CORE1588_REGS_GCFG_ENCOR_SHIFT)

/***************************************************************************************************
 * Register: IER
 * Description: Interrupt enable register
 */
#define CORE1588_REGS_IER_REG_OFFSET (0x4U)
#define CORE1588_REGS_IER_REG_LENGTH (0x4U)
#define CORE1588_REGS_IER_REG_RW_MASK (0x007FFFFFU)
#define CORE1588_REGS_IER_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_IER_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_IER_REG_READ_MASK (0x007FFFFFU)
#define CORE1588_REGS_IER_REG_WRITE_MASK (0x007FFFFFU)

/**
 * Field Name: IEPEERRTSID
 * Field Desc:  0: Disable interrupt generation when sequenceID and
 * sourcePortIdentity of PTP peer frame (PDELAY REQ or PDELAY RESP) are received
 * at GMII RX interface 1: Enable interrupt generation when sequenceID and
 * sourcePortIdentity of PTP peer frame (PDELAY REQ or PDELAY RESP) are received
 * at GMII RX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPEERRTSID_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPEERRTSID_SHIFT (22U)
#define CORE1588_REGS_IER_IEPEERRTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPEERRTSID_MASK                                     \
  (CORE1588_REGS_IER_IEPEERRTSID_NS_MASK << CORE1588_REGS_IER_IEPEERRTSID_SHIFT)

/**
 * Field Name: IEPEERTTSID
 * Field Desc:  0: Disable interrupt generation when sequenceID of PTP peer
 * frame (PDELAY REQ or PDELAY RESP) is transmitted at GMII TX interface 1:
 * Enable interrupt generation when sequenceID of PTP frame is (PDELAY REQ or
 * PDELAY RESP) is transmitted at GMII TX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPEERTTSID_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPEERTTSID_SHIFT (21U)
#define CORE1588_REGS_IER_IEPEERTTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPEERTTSID_MASK                                     \
  (CORE1588_REGS_IER_IEPEERTTSID_NS_MASK << CORE1588_REGS_IER_IEPEERTTSID_SHIFT)

/**
 * Field Name: IERTSID
 * Field Desc:  0: Disable interrupt generation when sequenceID and
 * sourcePortIdentity of PTP frame (SYNC REQ, DELAY REQ or DELAY RESP) are
 * received at GMII RX interface 1: Enable interrupt generation when sequenceID
 * and sourcePortIdentity of PTP frame (SYNC REQ, DELAY REQ or PDELAY RESP) are
 * received at GMII RX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IERTSID_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IERTSID_SHIFT (20U)
#define CORE1588_REGS_IER_IERTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IERTSID_MASK                                         \
  (CORE1588_REGS_IER_IERTSID_NS_MASK << CORE1588_REGS_IER_IERTSID_SHIFT)

/**
 * Field Name: IETTSID
 * Field Desc:  0: Disable interrupt generation when sequenceID of PTP frame
 * (SYNC REQ,DELAY REQ or DELAY RESP) is transmitted at GMII TX interface 1:
 * Enable interrupt generation when sequenceID of PTP frame is (SYNC REQ,DELAY
 * REQ or DELAY RESP) transmitted at GMII TX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IETTSID_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IETTSID_SHIFT (19U)
#define CORE1588_REGS_IER_IETTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IETTSID_MASK                                         \
  (CORE1588_REGS_IER_IETTSID_NS_MASK << CORE1588_REGS_IER_IETTSID_SHIFT)

/**
 * Field Name: IEPTPRXPDELAYRESP
 * Field Desc:  0: Disable interrupt generation when PTP PDELAY RESPONSE frame
 * is received at GMII RX interface 1: Enable interrupt generation when PTP
 * PDELAY RESPONSE frame is received at GMII RX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPRXPDELAYRESP_OFFSET                             \
  (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPRXPDELAYRESP_SHIFT (18U)
#define CORE1588_REGS_IER_IEPTPRXPDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPRXPDELAYRESP_MASK                               \
  (CORE1588_REGS_IER_IEPTPRXPDELAYRESP_NS_MASK                                 \
   << CORE1588_REGS_IER_IEPTPRXPDELAYRESP_SHIFT)

/**
 * Field Name: IEPTPRXPDELAYREQ
 * Field Desc:  0: Disable interrupt generation when PTP PDELAY REQ frame is
 * received at GMII RX interface 1: Enable interrupt generation when PTP PDELAY
 * REQ frame is received at GMII RX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPRXPDELAYREQ_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPRXPDELAYREQ_SHIFT (17U)
#define CORE1588_REGS_IER_IEPTPRXPDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPRXPDELAYREQ_MASK                                \
  (CORE1588_REGS_IER_IEPTPRXPDELAYREQ_NS_MASK                                  \
   << CORE1588_REGS_IER_IEPTPRXPDELAYREQ_SHIFT)

/**
 * Field Name: IEPTPRXDELAYRESP
 * Field Desc:  0: Disable interrupt generation when PTP DELAY RESPONSE frame is
 * received at GMII RX interface 1: Enable interrupt generation when PTP DELAY
 * RESPONSE frame is received at GMII RX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPRXDELAYRESP_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPRXDELAYRESP_SHIFT (16U)
#define CORE1588_REGS_IER_IEPTPRXDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPRXDELAYRESP_MASK                                \
  (CORE1588_REGS_IER_IEPTPRXDELAYRESP_NS_MASK                                  \
   << CORE1588_REGS_IER_IEPTPRXDELAYRESP_SHIFT)

/**
 * Field Name: IEPTPRXDELAYREQ
 * Field Desc:  0: Disable interrupt generation when PTP DELAY REQ frame is
 * received at GMII RX interface 1: Enable interrupt generation when PTP DELAY
 * REQ frame is received at GMII RX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPRXDELAYREQ_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPRXDELAYREQ_SHIFT (15U)
#define CORE1588_REGS_IER_IEPTPRXDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPRXDELAYREQ_MASK                                 \
  (CORE1588_REGS_IER_IEPTPRXDELAYREQ_NS_MASK                                   \
   << CORE1588_REGS_IER_IEPTPRXDELAYREQ_SHIFT)

/**
 * Field Name: IEPTPRXSYNC
 * Field Desc:  0: Disable interrupt generation when PTP SYNC frame is received
 * at GMII RX interface 1: Enable interrupt generation when PTP SYNC frame is
 * received at GMII RX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPRXSYNC_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPRXSYNC_SHIFT (14U)
#define CORE1588_REGS_IER_IEPTPRXSYNC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPRXSYNC_MASK                                     \
  (CORE1588_REGS_IER_IEPTPRXSYNC_NS_MASK << CORE1588_REGS_IER_IEPTPRXSYNC_SHIFT)

/**
 * Field Name: IEPTPTXPDELAYRESP
 * Field Desc:  0: Disable interrupt generation when PTP PDELAY RESPONSE frame
 * is transmitted at GMII TX interface 1: Enable interrupt generation when PTP
 * PDELAY RESPONSE frame is transmitted at GMII TX interface Field Type:
 * read-write
 */
#define CORE1588_REGS_IER_IEPTPTXPDELAYRESP_OFFSET                             \
  (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPTXPDELAYRESP_SHIFT (13U)
#define CORE1588_REGS_IER_IEPTPTXPDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPTXPDELAYRESP_MASK                               \
  (CORE1588_REGS_IER_IEPTPTXPDELAYRESP_NS_MASK                                 \
   << CORE1588_REGS_IER_IEPTPTXPDELAYRESP_SHIFT)

/**
 * Field Name: IEPTPTXPDELAYREQ
 * Field Desc:  0: Disable interrupt generation when PTP PDELAY REQ frame is
 * transmitted at GMII TX interface 1: Enable interrupt generation when PTP
 * PDELAY REQ frame is transmitted at GMII TX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPTXPDELAYREQ_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPTXPDELAYREQ_SHIFT (12U)
#define CORE1588_REGS_IER_IEPTPTXPDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPTXPDELAYREQ_MASK                                \
  (CORE1588_REGS_IER_IEPTPTXPDELAYREQ_NS_MASK                                  \
   << CORE1588_REGS_IER_IEPTPTXPDELAYREQ_SHIFT)

/**
 * Field Name: IEPTPTXDELAYRESP
 * Field Desc:  0: Disable interrupt generation when PTP DELAY RESPONSE frame is
 * transmitted at GMII TX interface 1: Enable interrupt generation when PTP
 * DELAY RESPONSE frame is transmitted at GMII TX interface Field Type:
 * read-write
 */
#define CORE1588_REGS_IER_IEPTPTXDELAYRESP_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPTXDELAYRESP_SHIFT (11U)
#define CORE1588_REGS_IER_IEPTPTXDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPTXDELAYRESP_MASK                                \
  (CORE1588_REGS_IER_IEPTPTXDELAYRESP_NS_MASK                                  \
   << CORE1588_REGS_IER_IEPTPTXDELAYRESP_SHIFT)

/**
 * Field Name: IEPTPTXDELAYREQ
 * Field Desc:  0: Disable interrupt generation when PTP DELAY REQ frame is
 * transmitted at GMII TX interface 1: Enable interrupt generation when PTP
 * DELAY REQ frame is transmitted at GMII TX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPTXDELAYREQ_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPTXDELAYREQ_SHIFT (10U)
#define CORE1588_REGS_IER_IEPTPTXDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPTXDELAYREQ_MASK                                 \
  (CORE1588_REGS_IER_IEPTPTXDELAYREQ_NS_MASK                                   \
   << CORE1588_REGS_IER_IEPTPTXDELAYREQ_SHIFT)

/**
 * Field Name: IEPTPTXSYNC
 * Field Desc:  0: Disable interrupt generation when PTP SYNC frame is
 * transmitted at GMII TX interface 1: Enable interrupt generation when PTP SYNC
 * frame is transmitted at GMII TX interface Field Type: read-write
 */
#define CORE1588_REGS_IER_IEPTPTXSYNC_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IEPTPTXSYNC_SHIFT (9U)
#define CORE1588_REGS_IER_IEPTPTXSYNC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IEPTPTXSYNC_MASK                                     \
  (CORE1588_REGS_IER_IEPTPTXSYNC_NS_MASK << CORE1588_REGS_IER_IEPTPTXSYNC_SHIFT)

/**
 * Field Name: IERTCSEC
 * Field Desc:  0: Disable interrupt generation when RTC second counter
 * increment by 1 1: Enable interrupt generation when RTC second counter
 * increment by 1 Field Type: read-write
 */
#define CORE1588_REGS_IER_IERTCSEC_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IERTCSEC_SHIFT (8U)
#define CORE1588_REGS_IER_IERTCSEC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IERTCSEC_MASK                                        \
  (CORE1588_REGS_IER_IERTCSEC_NS_MASK << CORE1588_REGS_IER_IERTCSEC_SHIFT)

/**
 * Field Name: IELT2
 * Field Desc:  0: Disable ILT2 Interrupt. 1: Enable ILT2 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IELT2_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IELT2_SHIFT (7U)
#define CORE1588_REGS_IER_IELT2_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IELT2_MASK                                           \
  (CORE1588_REGS_IER_IELT2_NS_MASK << CORE1588_REGS_IER_IELT2_SHIFT)

/**
 * Field Name: IELT1
 * Field Desc:  0: Disable ILT1 Interrupt. 1: Enable ILT1 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IELT1_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IELT1_SHIFT (6U)
#define CORE1588_REGS_IER_IELT1_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IELT1_MASK                                           \
  (CORE1588_REGS_IER_IELT1_NS_MASK << CORE1588_REGS_IER_IELT1_SHIFT)

/**
 * Field Name: IELT0
 * Field Desc:  0: Disable ILT0 Interrupt. 1: Enable ILT0 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IELT0_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IELT0_SHIFT (5U)
#define CORE1588_REGS_IER_IELT0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IELT0_MASK                                           \
  (CORE1588_REGS_IER_IELT0_NS_MASK << CORE1588_REGS_IER_IELT0_SHIFT)

/**
 * Field Name: IETT2
 * Field Desc:  0: Disable ITT2 Interrupt. 1: Enable ITT2 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IETT2_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IETT2_SHIFT (4U)
#define CORE1588_REGS_IER_IETT2_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IETT2_MASK                                           \
  (CORE1588_REGS_IER_IETT2_NS_MASK << CORE1588_REGS_IER_IETT2_SHIFT)

/**
 * Field Name: IETT1
 * Field Desc:  0: Disable ITT1 Interrupt. 1: Enable ITT1 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IETT1_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IETT1_SHIFT (3U)
#define CORE1588_REGS_IER_IETT1_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IETT1_MASK                                           \
  (CORE1588_REGS_IER_IETT1_NS_MASK << CORE1588_REGS_IER_IETT1_SHIFT)

/**
 * Field Name: IETT0
 * Field Desc:  0: Disable ITT0 Interrupt. 1: Enable ITT0 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IETT0_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IETT0_SHIFT (2U)
#define CORE1588_REGS_IER_IETT0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IETT0_MASK                                           \
  (CORE1588_REGS_IER_IETT0_NS_MASK << CORE1588_REGS_IER_IETT0_SHIFT)

/**
 * Field Name: IERTS0
 * Field Desc:  0: Disable IRTS0 Interrupt. 1: Enable IRTS0 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IERTS0_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IERTS0_SHIFT (1U)
#define CORE1588_REGS_IER_IERTS0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IERTS0_MASK                                          \
  (CORE1588_REGS_IER_IERTS0_NS_MASK << CORE1588_REGS_IER_IERTS0_SHIFT)

/**
 * Field Name: IETTS0
 * Field Desc:  0: Disable ITTS0 Interrupt. 1: Enable ITTS0 Interrupt.
 * Field Type: read-write
 */
#define CORE1588_REGS_IER_IETTS0_OFFSET (CORE1588_REGS_IER_REG_OFFSET)
#define CORE1588_REGS_IER_IETTS0_SHIFT (0U)
#define CORE1588_REGS_IER_IETTS0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_IER_IETTS0_MASK                                          \
  (CORE1588_REGS_IER_IETTS0_NS_MASK << CORE1588_REGS_IER_IETTS0_SHIFT)

/***************************************************************************************************
 * Register: MIS
 * Description: Masked interrupt status register
 */
#define CORE1588_REGS_MIS_REG_OFFSET (0x8U)
#define CORE1588_REGS_MIS_REG_LENGTH (0x4U)
#define CORE1588_REGS_MIS_REG_RW_MASK (0x007FFFFFU)
#define CORE1588_REGS_MIS_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_MIS_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_MIS_REG_READ_MASK (0x007FFFFFU)
#define CORE1588_REGS_MIS_REG_WRITE_MASK (0x007FFFFFU)

/**
 * Field Name: IPEERRTSID
 * Field Desc:  This bit is the logical AND of bit 22 of IER and bit 22 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPEERRTSID_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPEERRTSID_SHIFT (22U)
#define CORE1588_REGS_MIS_IPEERRTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPEERRTSID_MASK                                      \
  (CORE1588_REGS_MIS_IPEERRTSID_NS_MASK << CORE1588_REGS_MIS_IPEERRTSID_SHIFT)

/**
 * Field Name: IPEERTTSID
 * Field Desc:  This bit is the logical AND of bit 21 of IER and bit 21 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPEERTTSID_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPEERTTSID_SHIFT (21U)
#define CORE1588_REGS_MIS_IPEERTTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPEERTTSID_MASK                                      \
  (CORE1588_REGS_MIS_IPEERTTSID_NS_MASK << CORE1588_REGS_MIS_IPEERTTSID_SHIFT)

/**
 * Field Name: IRTSID
 * Field Desc:  This bit is the logical AND of bit 20 of IER and bit 20 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IRTSID_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IRTSID_SHIFT (20U)
#define CORE1588_REGS_MIS_IRTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IRTSID_MASK                                          \
  (CORE1588_REGS_MIS_IRTSID_NS_MASK << CORE1588_REGS_MIS_IRTSID_SHIFT)

/**
 * Field Name: ITTSID
 * Field Desc:  This bit is the logical AND of bit 19 of IER and bit 19 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ITTSID_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ITTSID_SHIFT (19U)
#define CORE1588_REGS_MIS_ITTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ITTSID_MASK                                          \
  (CORE1588_REGS_MIS_ITTSID_NS_MASK << CORE1588_REGS_MIS_ITTSID_SHIFT)

/**
 * Field Name: IPTPRXPDELAYRESP
 * Field Desc:  This bit is the logical AND of bit 18 of IER and bit 18 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPRXPDELAYRESP_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPRXPDELAYRESP_SHIFT (18U)
#define CORE1588_REGS_MIS_IPTPRXPDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPRXPDELAYRESP_MASK                                \
  (CORE1588_REGS_MIS_IPTPRXPDELAYRESP_NS_MASK                                  \
   << CORE1588_REGS_MIS_IPTPRXPDELAYRESP_SHIFT)

/**
 * Field Name: IPTPRXPDELAYREQ
 * Field Desc:  This bit is the logical AND of bit 17 of IER and bit 17 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPRXPDELAYREQ_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPRXPDELAYREQ_SHIFT (17U)
#define CORE1588_REGS_MIS_IPTPRXPDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPRXPDELAYREQ_MASK                                 \
  (CORE1588_REGS_MIS_IPTPRXPDELAYREQ_NS_MASK                                   \
   << CORE1588_REGS_MIS_IPTPRXPDELAYREQ_SHIFT)

/**
 * Field Name: IPTPRXDELAYRESP
 * Field Desc:  This bit is the logical AND of bit 16 of IER and bit 16 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPRXDELAYRESP_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPRXDELAYRESP_SHIFT (16U)
#define CORE1588_REGS_MIS_IPTPRXDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPRXDELAYRESP_MASK                                 \
  (CORE1588_REGS_MIS_IPTPRXDELAYRESP_NS_MASK                                   \
   << CORE1588_REGS_MIS_IPTPRXDELAYRESP_SHIFT)

/**
 * Field Name: IPTPRXDELAYREQ
 * Field Desc:  This bit is the logical AND of bit 15 of IER and bit 15 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPRXDELAYREQ_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPRXDELAYREQ_SHIFT (15U)
#define CORE1588_REGS_MIS_IPTPRXDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPRXDELAYREQ_MASK                                  \
  (CORE1588_REGS_MIS_IPTPRXDELAYREQ_NS_MASK                                    \
   << CORE1588_REGS_MIS_IPTPRXDELAYREQ_SHIFT)

/**
 * Field Name: IPTPRXSYNC
 * Field Desc:  This bit is the logical AND of bit 14 of IER and bit 14 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPRXSYNC_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPRXSYNC_SHIFT (14U)
#define CORE1588_REGS_MIS_IPTPRXSYNC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPRXSYNC_MASK                                      \
  (CORE1588_REGS_MIS_IPTPRXSYNC_NS_MASK << CORE1588_REGS_MIS_IPTPRXSYNC_SHIFT)

/**
 * Field Name: IPTPTXPDELAYRESP
 * Field Desc:  This bit is the logical AND of bit 13 of IER and bit 13 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPTXPDELAYRESP_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPTXPDELAYRESP_SHIFT (13U)
#define CORE1588_REGS_MIS_IPTPTXPDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPTXPDELAYRESP_MASK                                \
  (CORE1588_REGS_MIS_IPTPTXPDELAYRESP_NS_MASK                                  \
   << CORE1588_REGS_MIS_IPTPTXPDELAYRESP_SHIFT)

/**
 * Field Name: IPTPTXPDELAYREQ
 * Field Desc:  This bit is the logical AND of bit 12 of IER and bit 12 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPTXPDELAYREQ_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPTXPDELAYREQ_SHIFT (12U)
#define CORE1588_REGS_MIS_IPTPTXPDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPTXPDELAYREQ_MASK                                 \
  (CORE1588_REGS_MIS_IPTPTXPDELAYREQ_NS_MASK                                   \
   << CORE1588_REGS_MIS_IPTPTXPDELAYREQ_SHIFT)

/**
 * Field Name: IPTPTXDELAYRESP
 * Field Desc:  This bit is the logical AND of bit 11 of IER and bit 11 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPTXDELAYRESP_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPTXDELAYRESP_SHIFT (11U)
#define CORE1588_REGS_MIS_IPTPTXDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPTXDELAYRESP_MASK                                 \
  (CORE1588_REGS_MIS_IPTPTXDELAYRESP_NS_MASK                                   \
   << CORE1588_REGS_MIS_IPTPTXDELAYRESP_SHIFT)

/**
 * Field Name: IPTPTXDELAYREQ
 * Field Desc:  This bit is the logical AND of bit 10 of IER and bit 10 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPTXDELAYREQ_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPTXDELAYREQ_SHIFT (10U)
#define CORE1588_REGS_MIS_IPTPTXDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPTXDELAYREQ_MASK                                  \
  (CORE1588_REGS_MIS_IPTPTXDELAYREQ_NS_MASK                                    \
   << CORE1588_REGS_MIS_IPTPTXDELAYREQ_SHIFT)

/**
 * Field Name: IPTPTXSYNC
 * Field Desc:  This bit is the logical AND of bit 9 of IER and bit 9 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IPTPTXSYNC_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IPTPTXSYNC_SHIFT (9U)
#define CORE1588_REGS_MIS_IPTPTXSYNC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IPTPTXSYNC_MASK                                      \
  (CORE1588_REGS_MIS_IPTPTXSYNC_NS_MASK << CORE1588_REGS_MIS_IPTPTXSYNC_SHIFT)

/**
 * Field Name: IRTCSEC
 * Field Desc:  This bit is the logical AND of bit 8 of IER and bit 8 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IRTCSEC_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IRTCSEC_SHIFT (8U)
#define CORE1588_REGS_MIS_IRTCSEC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IRTCSEC_MASK                                         \
  (CORE1588_REGS_MIS_IRTCSEC_NS_MASK << CORE1588_REGS_MIS_IRTCSEC_SHIFT)

/**
 * Field Name: ILT2
 * Field Desc:  This bit is the logical AND of bit 7 of IER and bit 7 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ILT2_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ILT2_SHIFT (7U)
#define CORE1588_REGS_MIS_ILT2_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ILT2_MASK                                            \
  (CORE1588_REGS_MIS_ILT2_NS_MASK << CORE1588_REGS_MIS_ILT2_SHIFT)

/**
 * Field Name: ILT1
 * Field Desc:  This bit is the logical AND of bit 6 of IER and bit 6 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ILT1_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ILT1_SHIFT (6U)
#define CORE1588_REGS_MIS_ILT1_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ILT1_MASK                                            \
  (CORE1588_REGS_MIS_ILT1_NS_MASK << CORE1588_REGS_MIS_ILT1_SHIFT)

/**
 * Field Name: ILT0
 * Field Desc:  This bit is the logical AND of bit 5 of IER and bit 5 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ILT0_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ILT0_SHIFT (5U)
#define CORE1588_REGS_MIS_ILT0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ILT0_MASK                                            \
  (CORE1588_REGS_MIS_ILT0_NS_MASK << CORE1588_REGS_MIS_ILT0_SHIFT)

/**
 * Field Name: ITT2
 * Field Desc:  This bit is the logical AND of bit 4 of IER and bit 4 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ITT2_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ITT2_SHIFT (4U)
#define CORE1588_REGS_MIS_ITT2_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ITT2_MASK                                            \
  (CORE1588_REGS_MIS_ITT2_NS_MASK << CORE1588_REGS_MIS_ITT2_SHIFT)

/**
 * Field Name: ITT1
 * Field Desc:  This bit is the logical AND of bit 3 of IER and bit 3 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ITT1_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ITT1_SHIFT (3U)
#define CORE1588_REGS_MIS_ITT1_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ITT1_MASK                                            \
  (CORE1588_REGS_MIS_ITT1_NS_MASK << CORE1588_REGS_MIS_ITT1_SHIFT)

/**
 * Field Name: ITT0
 * Field Desc:  This bit is the logical AND of bit 2 of IER and bit 2 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ITT0_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ITT0_SHIFT (2U)
#define CORE1588_REGS_MIS_ITT0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ITT0_MASK                                            \
  (CORE1588_REGS_MIS_ITT0_NS_MASK << CORE1588_REGS_MIS_ITT0_SHIFT)

/**
 * Field Name: IRTS
 * Field Desc:  This bit is the logical AND of bit 1 of IER and bit 1 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_IRTS_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_IRTS_SHIFT (1U)
#define CORE1588_REGS_MIS_IRTS_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_IRTS_MASK                                            \
  (CORE1588_REGS_MIS_IRTS_NS_MASK << CORE1588_REGS_MIS_IRTS_SHIFT)

/**
 * Field Name: ITTS
 * Field Desc:  This bit is the logical AND of bit 0 of IER and bit 0 of RIS
 * Field Type: read-write
 */
#define CORE1588_REGS_MIS_ITTS_OFFSET (CORE1588_REGS_MIS_REG_OFFSET)
#define CORE1588_REGS_MIS_ITTS_SHIFT (0U)
#define CORE1588_REGS_MIS_ITTS_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_MIS_ITTS_MASK                                            \
  (CORE1588_REGS_MIS_ITTS_NS_MASK << CORE1588_REGS_MIS_ITTS_SHIFT)

/***************************************************************************************************
 * Register: RIS
 * Description: Raw interrupt status register
 */
#define CORE1588_REGS_RIS_REG_OFFSET (0xcU)
#define CORE1588_REGS_RIS_REG_LENGTH (0x4U)
#define CORE1588_REGS_RIS_REG_RW_MASK (0x007FFFFFU)
#define CORE1588_REGS_RIS_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RIS_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RIS_REG_READ_MASK (0x007FFFFFU)
#define CORE1588_REGS_RIS_REG_WRITE_MASK (0x007FFFFFU)

/**
 * Field Name: RIPEERRTSID
 * Field Desc:  When this bit has a value of 1 it indicates that a sequenceID
 * and sourcePortIdentity of PTP peer frame (PDELAY REQ or PDELAY RESP) are
 * received at GMII RX interface This bit is cleared by writing 1 to it. Writing
 * 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPEERRTSID_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPEERRTSID_SHIFT (22U)
#define CORE1588_REGS_RIS_RIPEERRTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPEERRTSID_MASK                                     \
  (CORE1588_REGS_RIS_RIPEERRTSID_NS_MASK << CORE1588_REGS_RIS_RIPEERRTSID_SHIFT)

/**
 * Field Name: RIPEERTTSID
 * Field Desc:  When this bit has a value of 1 it indicates that a sequenceID of
 * PTP peer frame (PDELAY REQ or PDELAY RESP) is transmitted at GMII RX
 * interface This bit is cleared by writing 1 to it. Writing 0 has no effect.
 * Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPEERTTSID_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPEERTTSID_SHIFT (21U)
#define CORE1588_REGS_RIS_RIPEERTTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPEERTTSID_MASK                                     \
  (CORE1588_REGS_RIS_RIPEERTTSID_NS_MASK << CORE1588_REGS_RIS_RIPEERTTSID_SHIFT)

/**
 * Field Name: RIRTSID
 * Field Desc:  When this bit has a value of 1 it indicates that a sequenceID
 * and sourcePortIdentity of PTP frame (SYNC REQ, DELAY REQ or DELAY RESP) are
 * received at GMII RX interface This bit is cleared by writing 1 to it. Writing
 * 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIRTSID_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIRTSID_SHIFT (20U)
#define CORE1588_REGS_RIS_RIRTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIRTSID_MASK                                         \
  (CORE1588_REGS_RIS_RIRTSID_NS_MASK << CORE1588_REGS_RIS_RIRTSID_SHIFT)

/**
 * Field Name: RITTSID
 * Field Desc:  When this bit has a value of 1 it indicates that a sequenceID of
 * PTP frame (SYNC REQ, DELAY REQ or DELAY RESP) is transmitted at GMII RX
 * interface This bit is cleared by writing 1 to it. Writing 0 has no effect.
 * Field Type: read-write
 */
#define CORE1588_REGS_RIS_RITTSID_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RITTSID_SHIFT (19U)
#define CORE1588_REGS_RIS_RITTSID_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RITTSID_MASK                                         \
  (CORE1588_REGS_RIS_RITTSID_NS_MASK << CORE1588_REGS_RIS_RITTSID_SHIFT)

/**
 * Field Name: RIPTPRXPDELAYRESP
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP PDELAY
 * RESPONSE frame is received at GMII RX interface This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPRXPDELAYRESP_OFFSET                             \
  (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPRXPDELAYRESP_SHIFT (18U)
#define CORE1588_REGS_RIS_RIPTPRXPDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPRXPDELAYRESP_MASK                               \
  (CORE1588_REGS_RIS_RIPTPRXPDELAYRESP_NS_MASK                                 \
   << CORE1588_REGS_RIS_RIPTPRXPDELAYRESP_SHIFT)

/**
 * Field Name: RIPTPRXPDELAYREQ
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP PDLAY
 * REQUEST is received at GMII RX interface This bit is cleared by writing 1 to
 * it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPRXPDELAYREQ_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPRXPDELAYREQ_SHIFT (17U)
#define CORE1588_REGS_RIS_RIPTPRXPDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPRXPDELAYREQ_MASK                                \
  (CORE1588_REGS_RIS_RIPTPRXPDELAYREQ_NS_MASK                                  \
   << CORE1588_REGS_RIS_RIPTPRXPDELAYREQ_SHIFT)

/**
 * Field Name: RIPTPRXDELAYRESP
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP DELAY
 * RESPONSE frame is received at GMII RX interface This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPRXDELAYRESP_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPRXDELAYRESP_SHIFT (16U)
#define CORE1588_REGS_RIS_RIPTPRXDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPRXDELAYRESP_MASK                                \
  (CORE1588_REGS_RIS_RIPTPRXDELAYRESP_NS_MASK                                  \
   << CORE1588_REGS_RIS_RIPTPRXDELAYRESP_SHIFT)

/**
 * Field Name: RIPTPRXDELAYREQ
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP DELAY
 * REQUEST frame is received at GMII RX interface This bit is cleared by writing
 * 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPRXDELAYREQ_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPRXDELAYREQ_SHIFT (15U)
#define CORE1588_REGS_RIS_RIPTPRXDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPRXDELAYREQ_MASK                                 \
  (CORE1588_REGS_RIS_RIPTPRXDELAYREQ_NS_MASK                                   \
   << CORE1588_REGS_RIS_RIPTPRXDELAYREQ_SHIFT)

/**
 * Field Name: RIPTPRXSYNC
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP SYNC
 * frame is received at GMII RX interface This bit is cleared by writing 1 to
 * it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPRXSYNC_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPRXSYNC_SHIFT (14U)
#define CORE1588_REGS_RIS_RIPTPRXSYNC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPRXSYNC_MASK                                     \
  (CORE1588_REGS_RIS_RIPTPRXSYNC_NS_MASK << CORE1588_REGS_RIS_RIPTPRXSYNC_SHIFT)

/**
 * Field Name: RIPTPTXPDELAYRESP
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP PDELAY
 * RESPONSE frame is transmitted at GMII TX interface This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPTXPDELAYRESP_OFFSET                             \
  (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPTXPDELAYRESP_SHIFT (13U)
#define CORE1588_REGS_RIS_RIPTPTXPDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPTXPDELAYRESP_MASK                               \
  (CORE1588_REGS_RIS_RIPTPTXPDELAYRESP_NS_MASK                                 \
   << CORE1588_REGS_RIS_RIPTPTXPDELAYRESP_SHIFT)

/**
 * Field Name: RIPTPTXPDELAYREQ
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP PDELAY
 * REQUEST frame is transmitted at GMII TX interface This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPTXPDELAYREQ_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPTXPDELAYREQ_SHIFT (12U)
#define CORE1588_REGS_RIS_RIPTPTXPDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPTXPDELAYREQ_MASK                                \
  (CORE1588_REGS_RIS_RIPTPTXPDELAYREQ_NS_MASK                                  \
   << CORE1588_REGS_RIS_RIPTPTXPDELAYREQ_SHIFT)

/**
 * Field Name: RIPTPTXDELAYRESP
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP DELAY
 * RESPONSE frame is transmitted at GMII TX interface This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPTXDELAYRESP_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPTXDELAYRESP_SHIFT (11U)
#define CORE1588_REGS_RIS_RIPTPTXDELAYRESP_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPTXDELAYRESP_MASK                                \
  (CORE1588_REGS_RIS_RIPTPTXDELAYRESP_NS_MASK                                  \
   << CORE1588_REGS_RIS_RIPTPTXDELAYRESP_SHIFT)

/**
 * Field Name: RIPTPTXDELAYREQ
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP DELAY
 * REQUEST frame is transmitted at GMII TX interface This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPTXDELAYREQ_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPTXDELAYREQ_SHIFT (10U)
#define CORE1588_REGS_RIS_RIPTPTXDELAYREQ_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPTXDELAYREQ_MASK                                 \
  (CORE1588_REGS_RIS_RIPTPTXDELAYREQ_NS_MASK                                   \
   << CORE1588_REGS_RIS_RIPTPTXDELAYREQ_SHIFT)

/**
 * Field Name: RIPTPTXSYNC
 * Field Desc:  When this bit has a value of 1 it indicates that a PTP SYNC
 * frame is transmitted at GMII TX interface This bit is cleared by writing 1 to
 * it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIPTPTXSYNC_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIPTPTXSYNC_SHIFT (9U)
#define CORE1588_REGS_RIS_RIPTPTXSYNC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIPTPTXSYNC_MASK                                     \
  (CORE1588_REGS_RIS_RIPTPTXSYNC_NS_MASK << CORE1588_REGS_RIS_RIPTPTXSYNC_SHIFT)

/**
 * Field Name: RIRTCSEC
 * Field Desc:  When this bit has a value of 1 it indicates that a RTC second
 * counter is incremented by 1 This bit is cleared by writing 1 to it. Writing 0
 * has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIRTCSEC_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIRTCSEC_SHIFT (8U)
#define CORE1588_REGS_RIS_RIRTCSEC_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIRTCSEC_MASK                                        \
  (CORE1588_REGS_RIS_RIRTCSEC_NS_MASK << CORE1588_REGS_RIS_RIRTCSEC_SHIFT)

/**
 * Field Name: RILT2
 * Field Desc:  When this bit has a value of 1 it indicates that a rising edge
 * has been observed on the LATCH2 input since this bit was last 0. This bit is
 * cleared by writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RILT2_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RILT2_SHIFT (7U)
#define CORE1588_REGS_RIS_RILT2_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RILT2_MASK                                           \
  (CORE1588_REGS_RIS_RILT2_NS_MASK << CORE1588_REGS_RIS_RILT2_SHIFT)

/**
 * Field Name: RILT1
 * Field Desc:  When this bit has a value of 1 it indicates that a rising edge
 * has been observed on the LATCH1 input since this bit was last 0. This bit is
 * cleared by writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RILT1_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RILT1_SHIFT (6U)
#define CORE1588_REGS_RIS_RILT1_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RILT1_MASK                                           \
  (CORE1588_REGS_RIS_RILT1_NS_MASK << CORE1588_REGS_RIS_RILT1_SHIFT)

/**
 * Field Name: RILT0
 * Field Desc:  When this bit has a value of 1 it indicates that a rising edge
 * has been observed on the LATCH0 input since this bit was last 0. This bit is
 * cleared by writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RILT0_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RILT0_SHIFT (5U)
#define CORE1588_REGS_RIS_RILT0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RILT0_MASK                                           \
  (CORE1588_REGS_RIS_RILT0_NS_MASK << CORE1588_REGS_RIS_RILT0_SHIFT)

/**
 * Field Name: RITT2
 * Field Desc:  When this bit has a value of 1 it indicates that the time set in
 * the trigger 2 registers (TT2L, TT2M and TT2MSBSEC) has been reached since
 * this bit was last 0. This bit is cleared by writing 1 to it. Writing 0 has no
 * effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RITT2_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RITT2_SHIFT (4U)
#define CORE1588_REGS_RIS_RITT2_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RITT2_MASK                                           \
  (CORE1588_REGS_RIS_RITT2_NS_MASK << CORE1588_REGS_RIS_RITT2_SHIFT)

/**
 * Field Name: RITT1
 * Field Desc:  When this bit has a value of 1 it indicates that the time set in
 * the trigger 1 registers (TT1L, TT1M and TT1MSBSEC) has been reached since
 * this bit was last 0. This bit is cleared by writing 1 to it. Writing 0 has no
 * effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RITT1_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RITT1_SHIFT (3U)
#define CORE1588_REGS_RIS_RITT1_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RITT1_MASK                                           \
  (CORE1588_REGS_RIS_RITT1_NS_MASK << CORE1588_REGS_RIS_RITT1_SHIFT)

/**
 * Field Name: RITT0
 * Field Desc:  When this bit has a value of 1 it indicates that the time set in
 * the trigger 0 registers (TT0L, TT0M and TT0MSBSEC) has been reached since
 * this bit was last 0. This bit is cleared by writing 1 to it. Writing 0 has no
 * effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RITT0_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RITT0_SHIFT (2U)
#define CORE1588_REGS_RIS_RITT0_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RITT0_MASK                                           \
  (CORE1588_REGS_RIS_RITT0_NS_MASK << CORE1588_REGS_RIS_RITT0_SHIFT)

/**
 * Field Name: RIRTS
 * Field Desc:  When this bit has a value of 1 it indicates that, since this bit
 * was last 0, receipt of an IEEE 1588 frame has been detected and that the
 * receive timestamp registers (RTSL, RTSM and RTSMSBSECSUBNS or PEERRTSL,
 * PEERRTSM and PEERRTSMSBSECSUBNS) have been updated. This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RIRTS_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RIRTS_SHIFT (1U)
#define CORE1588_REGS_RIS_RIRTS_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RIRTS_MASK                                           \
  (CORE1588_REGS_RIS_RIRTS_NS_MASK << CORE1588_REGS_RIS_RIRTS_SHIFT)

/**
 * Field Name: RITTS
 * Field Desc:  When this bit has a value of 1 it indicates that, since this bit
 * was last 0, transmission of an IEEE 1588 frame has been detected and that the
 * transmit timestamp registers (TTSL, TTSM and TTSMSBSECSUBNS or PEERTTSL,
 * PEERTTSM and PEERTTSMSBSECSUBNS) have been updated. This bit is cleared by
 * writing 1 to it. Writing 0 has no effect. Field Type: read-write
 */
#define CORE1588_REGS_RIS_RITTS_OFFSET (CORE1588_REGS_RIS_REG_OFFSET)
#define CORE1588_REGS_RIS_RITTS_SHIFT (0U)
#define CORE1588_REGS_RIS_RITTS_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RIS_RITTS_MASK                                           \
  (CORE1588_REGS_RIS_RITTS_NS_MASK << CORE1588_REGS_RIS_RITTS_SHIFT)

/***************************************************************************************************
 * Register: TTSL
 * Description: PTP frame transmitted timestamp nanoseconds register
 */
#define CORE1588_REGS_TTSL_REG_OFFSET (0x10U)
#define CORE1588_REGS_TTSL_REG_LENGTH (0x4U)
#define CORE1588_REGS_TTSL_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TTSL_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TTSL_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TTSL_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TTSL_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_TX_NSEC
 * Field Desc:  PTP frame transmitted timestamp nanoseconds. The register is
 * updated with the nanosecond count of RTC when SFD of a PTP transmit frame is
 * detected at GMII interface. An interrupt is issued when the register is
 * updated. Field Type: read-write
 */
#define CORE1588_REGS_TTSL_PTP_TX_NSEC_OFFSET (CORE1588_REGS_TTSL_REG_OFFSET)
#define CORE1588_REGS_TTSL_PTP_TX_NSEC_SHIFT (0U)
#define CORE1588_REGS_TTSL_PTP_TX_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TTSL_PTP_TX_NSEC_MASK                                    \
  (CORE1588_REGS_TTSL_PTP_TX_NSEC_NS_MASK                                      \
   << CORE1588_REGS_TTSL_PTP_TX_NSEC_SHIFT)

/***************************************************************************************************
 * Register: TTSM
 * Description: PTP frame transmitted timestamp seconds register [31:0].Note:
 * TTSMSBSECSUBNS register contains upper 16 bits [47:32] of seconds
 */
#define CORE1588_REGS_TTSM_REG_OFFSET (0x14U)
#define CORE1588_REGS_TTSM_REG_LENGTH (0x4U)
#define CORE1588_REGS_TTSM_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TTSM_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TTSM_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TTSM_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TTSM_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_TX_SEC_LSB_32BITS
 * Field Desc:  PTP frame transmitted timestamp seconds [31:0]. The register is
 * updated with the lower 32 bits [31:0] of second count of RTC when SFD of a
 * PTP transmit frame is detected at GMII interface. An interrupt is issued when
 * the register is updated. Field Type: read-write
 */
#define CORE1588_REGS_TTSM_PTP_TX_SEC_LSB_32BITS_OFFSET                        \
  (CORE1588_REGS_TTSM_REG_OFFSET)
#define CORE1588_REGS_TTSM_PTP_TX_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_TTSM_PTP_TX_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TTSM_PTP_TX_SEC_LSB_32BITS_MASK                          \
  (CORE1588_REGS_TTSM_PTP_TX_SEC_LSB_32BITS_NS_MASK                            \
   << CORE1588_REGS_TTSM_PTP_TX_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: TTSID
 * Description: PTP frame transmitted identification register. Contains the
 * 16-bit sequence ID
 */
#define CORE1588_REGS_TTSID_REG_OFFSET (0x18U)
#define CORE1588_REGS_TTSID_REG_LENGTH (0x4U)
#define CORE1588_REGS_TTSID_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_TTSID_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TTSID_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TTSID_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_TTSID_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: PTP_TX_SEQ_ID
 * Field Desc:  Contains the 16 bits sequenceID from the message header of the
 * transmitted ptp frame associated with the current transmitted timestamp (in
 * the TTSL, TTSM and TTSMSBSECSUBNS registers) Field Type: read-write
 */
#define CORE1588_REGS_TTSID_PTP_TX_SEQ_ID_OFFSET                               \
  (CORE1588_REGS_TTSID_REG_OFFSET)
#define CORE1588_REGS_TTSID_PTP_TX_SEQ_ID_SHIFT (0U)
#define CORE1588_REGS_TTSID_PTP_TX_SEQ_ID_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_TTSID_PTP_TX_SEQ_ID_MASK                                 \
  (CORE1588_REGS_TTSID_PTP_TX_SEQ_ID_NS_MASK                                   \
   << CORE1588_REGS_TTSID_PTP_TX_SEQ_ID_SHIFT)

/***************************************************************************************************
 * Register: RTSL
 * Description: PTP frame received timestamp nanoseconds register
 */
#define CORE1588_REGS_RTSL_REG_OFFSET (0x1cU)
#define CORE1588_REGS_RTSL_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTSL_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSL_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTSL_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTSL_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSL_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_RX_NSEC
 * Field Desc:  PTP frame received timestamp nanoseconds. The register is
 * updated with the nanosecond count of RTC when SFD of a PTP frame is received
 * at GMII interface. An interrupt is issued when the register is updated. Field
 * Type: read-write
 */
#define CORE1588_REGS_RTSL_PTP_RX_NSEC_OFFSET (CORE1588_REGS_RTSL_REG_OFFSET)
#define CORE1588_REGS_RTSL_PTP_RX_NSEC_SHIFT (0U)
#define CORE1588_REGS_RTSL_PTP_RX_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_RTSL_PTP_RX_NSEC_MASK                                    \
  (CORE1588_REGS_RTSL_PTP_RX_NSEC_NS_MASK                                      \
   << CORE1588_REGS_RTSL_PTP_RX_NSEC_SHIFT)

/***************************************************************************************************
 * Register: RTSM
 * Description: PTP frame received timestamp seconds register [31:0].Note:
 * RTSMSBSECSUBNS register contains upper 16 bits [47:32] of seconds
 */
#define CORE1588_REGS_RTSM_REG_OFFSET (0x20U)
#define CORE1588_REGS_RTSM_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTSM_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSM_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTSM_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTSM_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSM_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_RX_SEC_LSB_32BITS
 * Field Desc:  PTP frame received timestamp seconds [31:0]. The register is
 * updated with the lower 32 bits [31:0] of second count of RTC when SFD of a
 * PTP frame is received at GMII interface. An interrupt is issued when the
 * register is updated. Field Type: read-write
 */
#define CORE1588_REGS_RTSM_PTP_RX_SEC_LSB_32BITS_OFFSET                        \
  (CORE1588_REGS_RTSM_REG_OFFSET)
#define CORE1588_REGS_RTSM_PTP_RX_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_RTSM_PTP_RX_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_RTSM_PTP_RX_SEC_LSB_32BITS_MASK                          \
  (CORE1588_REGS_RTSM_PTP_RX_SEC_LSB_32BITS_NS_MASK                            \
   << CORE1588_REGS_RTSM_PTP_RX_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: RTSID2
 * Description: PTP frame received identification register2. Contains the 16-bit
 * sequence ID and the bits [79:64] of the sourcePortIdentity
 */
#define CORE1588_REGS_RTSID2_REG_OFFSET (0x24U)
#define CORE1588_REGS_RTSID2_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTSID2_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSID2_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTSID2_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTSID2_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSID2_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_RX_SEQ_ID
 * Field Desc:  Contains the 16 bits sequenceID from the message header of the
 * ptp frame associated with the current receive timestamp (in the RTSL, RTSM
 * and RTSMSBSECSUBNS egisters) Field Type: read-write
 */
#define CORE1588_REGS_RTSID2_PTP_RX_SEQ_ID_OFFSET                              \
  (CORE1588_REGS_RTSID2_REG_OFFSET)
#define CORE1588_REGS_RTSID2_PTP_RX_SEQ_ID_SHIFT (16U)
#define CORE1588_REGS_RTSID2_PTP_RX_SEQ_ID_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_RTSID2_PTP_RX_SEQ_ID_MASK                                \
  (CORE1588_REGS_RTSID2_PTP_RX_SEQ_ID_NS_MASK                                  \
   << CORE1588_REGS_RTSID2_PTP_RX_SEQ_ID_SHIFT)

/**
 * Field Name: PTP_RX_SRC_PORTID_UPPER_16BITS
 * Field Desc:  Contains the upper 16 bits [79:64] of sourcePortIdentity from
 * the message header of the received ptp frame associated with the current
 * receive timestamp (in the RTSL, RTSM and RTSMSBSEC registers) Field Type:
 * read-write
 */
#define CORE1588_REGS_RTSID2_PTP_RX_SRC_PORTID_UPPER_16BITS_OFFSET             \
  (CORE1588_REGS_RTSID2_REG_OFFSET)
#define CORE1588_REGS_RTSID2_PTP_RX_SRC_PORTID_UPPER_16BITS_SHIFT (0U)
#define CORE1588_REGS_RTSID2_PTP_RX_SRC_PORTID_UPPER_16BITS_NS_MASK            \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_RTSID2_PTP_RX_SRC_PORTID_UPPER_16BITS_MASK               \
  (CORE1588_REGS_RTSID2_PTP_RX_SRC_PORTID_UPPER_16BITS_NS_MASK                 \
   << CORE1588_REGS_RTSID2_PTP_RX_SRC_PORTID_UPPER_16BITS_SHIFT)

/***************************************************************************************************
 * Register: RTSID1
 * Description: PTP frame received identification register1. Contains bits
 * [63:32] of the sourcePortIdentity
 */
#define CORE1588_REGS_RTSID1_REG_OFFSET (0x28U)
#define CORE1588_REGS_RTSID1_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTSID1_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSID1_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTSID1_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTSID1_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSID1_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_RX_SRC_PORTID_MID_32BITS
 * Field Desc:  Contains the bits [63:32] of sourcePortIdentity from the message
 * header of the received ptp frame associated with the current receive
 * timestamp (in the RTSL, RTSM and RTSMSBSECSUBNS registers) Field Type:
 * read-write
 */
#define CORE1588_REGS_RTSID1_PTP_RX_SRC_PORTID_MID_32BITS_OFFSET               \
  (CORE1588_REGS_RTSID1_REG_OFFSET)
#define CORE1588_REGS_RTSID1_PTP_RX_SRC_PORTID_MID_32BITS_SHIFT (0U)
#define CORE1588_REGS_RTSID1_PTP_RX_SRC_PORTID_MID_32BITS_NS_MASK              \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_RTSID1_PTP_RX_SRC_PORTID_MID_32BITS_MASK                 \
  (CORE1588_REGS_RTSID1_PTP_RX_SRC_PORTID_MID_32BITS_NS_MASK                   \
   << CORE1588_REGS_RTSID1_PTP_RX_SRC_PORTID_MID_32BITS_SHIFT)

/***************************************************************************************************
 * Register: RTSID0
 * Description: PTP frame received identification register0. Contains bits
 * [31:0] of the sourcePortIdentity
 */
#define CORE1588_REGS_RTSID0_REG_OFFSET (0x2cU)
#define CORE1588_REGS_RTSID0_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTSID0_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSID0_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTSID0_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTSID0_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSID0_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_RX_SRC_PORTID_LOWER_32BITS
 * Field Desc:  Contains the bits [31:0] of sourcePortIdentity from the message
 * header of the received ptp frame associated with the current receive
 * timestamp (in the RTSL, RTSM and RTSMSBSECSUBNS registers) Field Type:
 * read-write
 */
#define CORE1588_REGS_RTSID0_PTP_RX_SRC_PORTID_LOWER_32BITS_OFFSET             \
  (CORE1588_REGS_RTSID0_REG_OFFSET)
#define CORE1588_REGS_RTSID0_PTP_RX_SRC_PORTID_LOWER_32BITS_SHIFT (0U)
#define CORE1588_REGS_RTSID0_PTP_RX_SRC_PORTID_LOWER_32BITS_NS_MASK            \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_RTSID0_PTP_RX_SRC_PORTID_LOWER_32BITS_MASK               \
  (CORE1588_REGS_RTSID0_PTP_RX_SRC_PORTID_LOWER_32BITS_NS_MASK                 \
   << CORE1588_REGS_RTSID0_PTP_RX_SRC_PORTID_LOWER_32BITS_SHIFT)

/***************************************************************************************************
 * Register: RTCL
 * Description: Internal RTC nanoseconds register.When writing to the internal
 * RTC, the RTCL register should be written first followed by a write to the
 * RTCM and RTCMSBSEC registers.When reading the internal RTC value, the RTCL
 * register should always be read first followed by a read of the RTCM and
 * RTCMSBSEC registers.
 */
#define CORE1588_REGS_RTCL_REG_OFFSET (0x30U)
#define CORE1588_REGS_RTCL_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTCL_REG_RW_MASK (0x3FFFFFFFU)
#define CORE1588_REGS_RTCL_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTCL_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTCL_REG_READ_MASK (0x3FFFFFFFU)
#define CORE1588_REGS_RTCL_REG_WRITE_MASK (0x3FFFFFFFU)

/**
 * Field Name: RTC_NSEC
 * Field Desc:  Internal RTC nanosecond count. When writing to the internal RTC,
 * RTCL should be written first followed by write to the RTCM and RTCMSBSEC
 * registers. This register can be adjusted by writing to the ADJUST register or
 * RTCCTRL register. Its increment factor can be changed by writing to the
 * RTCINCR register When reading to the internal RTC value, the RTCL register
 * should always be read first followed by a read of the RTCM and RTCMSBSEC
 * registers Field Type: read-write
 */
#define CORE1588_REGS_RTCL_RTC_NSEC_OFFSET (CORE1588_REGS_RTCL_REG_OFFSET)
#define CORE1588_REGS_RTCL_RTC_NSEC_SHIFT (0U)
#define CORE1588_REGS_RTCL_RTC_NSEC_NS_MASK (BIT_MASK_30_BITS)
#define CORE1588_REGS_RTCL_RTC_NSEC_MASK                                       \
  (CORE1588_REGS_RTCL_RTC_NSEC_NS_MASK << CORE1588_REGS_RTCL_RTC_NSEC_SHIFT)

/***************************************************************************************************
 * Register: RTCM
 * Description: Internal RTC seconds register [31:0]. Note: RTCMSBSEC register
 * contains upper 16 bits [47:32] of seconds
 */
#define CORE1588_REGS_RTCM_REG_OFFSET (0x34U)
#define CORE1588_REGS_RTCM_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTCM_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTCM_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTCM_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTCM_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTCM_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: RTC_SEC_LSB_32BITS
 * Field Desc:  Least significant 32 bits of internal RTC seconds count. When
 * writing to the internal RTC, RTCL should be written first followed by write
 * to the RTCM and RTCMSBSEC registers. This register can be
 * incremented/decremented by writing to the RTCCTRL register. When reading to
 * the internal RTC value, the RTCL register should always be read first
 * followed by a read of the RTCM and RTCMSBSEC registers Field Type: read-write
 */
#define CORE1588_REGS_RTCM_RTC_SEC_LSB_32BITS_OFFSET                           \
  (CORE1588_REGS_RTCM_REG_OFFSET)
#define CORE1588_REGS_RTCM_RTC_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_RTCM_RTC_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_RTCM_RTC_SEC_LSB_32BITS_MASK                             \
  (CORE1588_REGS_RTCM_RTC_SEC_LSB_32BITS_NS_MASK                               \
   << CORE1588_REGS_RTCM_RTC_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: ADJUST
 * Description: Internal RTC adjustment control register.This control register
 * can be used to adjust the speed of the internal RTC. See the detailed
 * description of this register.
 */
#define CORE1588_REGS_ADJUST_REG_OFFSET (0x38U)
#define CORE1588_REGS_ADJUST_REG_LENGTH (0x4U)
#define CORE1588_REGS_ADJUST_REG_RW_MASK (0xFCFFFFFFU)
#define CORE1588_REGS_ADJUST_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_ADJUST_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_ADJUST_REG_READ_MASK (0xFCFFFFFFU)
#define CORE1588_REGS_ADJUST_REG_WRITE_MASK (0xFCFFFFFFU)

/**
 * Field Name: ADJVAL
 * Field Desc:  This five-bit field holds the amount to be added to Internal RTC
 * least significant word (nanoseconds count) each time the adjustment counter
 * returns to zero (and the ADJEN bit is set to 1). The internal RTC operates in
 * the PTP_CLK clock domain and the nanoseconds count normally increments by the
 * period of PTP_CLK clock on each rising edge of PTP_CLK. When the conditions
 * described above are met, the internal RTC will be incremented by the value
 * stored in ADJVAL, instead of by the normal amount of PTP_CLK clock period The
 * value stored in ADJVAL can range from 0 to 31. Using adjustment with an
 * ADJVAL value less than PTP_CLK clock period will result in a slowing down of
 * the internal RTC. Similarly, using adjustment with an ADJVAL greater than
 * PTP_CLK clock period will cause the internal RTC to speed up. The amount of
 * slowing down or speeding up is a function of both the ADJVAL value and the
 * ADJCM value Field Type: read-write
 */
#define CORE1588_REGS_ADJUST_ADJVAL_OFFSET (CORE1588_REGS_ADJUST_REG_OFFSET)
#define CORE1588_REGS_ADJUST_ADJVAL_SHIFT (27U)
#define CORE1588_REGS_ADJUST_ADJVAL_NS_MASK (BIT_MASK_5_BITS)
#define CORE1588_REGS_ADJUST_ADJVAL_MASK                                       \
  (CORE1588_REGS_ADJUST_ADJVAL_NS_MASK << CORE1588_REGS_ADJUST_ADJVAL_SHIFT)

/**
 * Field Name: ADJEN
 * Field Desc:  0: Adjustment feature is disabled 1: Adjustment feature is
 * enabled Field Type: read-write
 */
#define CORE1588_REGS_ADJUST_ADJEN_OFFSET (CORE1588_REGS_ADJUST_REG_OFFSET)
#define CORE1588_REGS_ADJUST_ADJEN_SHIFT (26U)
#define CORE1588_REGS_ADJUST_ADJEN_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_ADJUST_ADJEN_MASK                                        \
  (CORE1588_REGS_ADJUST_ADJEN_NS_MASK << CORE1588_REGS_ADJUST_ADJEN_SHIFT)

/**
 * Field Name: ADJCM
 * Field Desc:  Adjustment counter maximum value. This field sets the rollover
 * value for a 24-bit adjustment counter clocked by PTP_CLK clock. The
 * adjustment counter is essentially a free running counter that increments by 1
 * on each rising edge of PTP_CLK. When the counter value reaches the ADJCM
 * value it returns to zero and, if the ADJEN bit is set, the internal RTC
 * nanoseconds count is incremented by the ADJVAL instead of by the normal
 * amount of PTP_CLK clock period. The ADJCM field essentially controls how
 * frequently the ADJVAL is used as the internal RTC increment. Field Type:
 * read-write
 */
#define CORE1588_REGS_ADJUST_ADJCM_OFFSET (CORE1588_REGS_ADJUST_REG_OFFSET)
#define CORE1588_REGS_ADJUST_ADJCM_SHIFT (0U)
#define CORE1588_REGS_ADJUST_ADJCM_NS_MASK (BIT_MASK_24_BITS)
#define CORE1588_REGS_ADJUST_ADJCM_MASK                                        \
  (CORE1588_REGS_ADJUST_ADJCM_NS_MASK << CORE1588_REGS_ADJUST_ADJCM_SHIFT)

/***************************************************************************************************
 * Register: TT0L
 * Description: Time trigger 0 nanoseconds register.When TRIG_NUM is greater
 * than 0 and the TT0L, TT0M and TT0MSBSEC registers match the RTC value a pulse
 * is generated on theTRIG0 output
 */
#define CORE1588_REGS_TT0L_REG_OFFSET (0x40U)
#define CORE1588_REGS_TT0L_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT0L_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT0L_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT0L_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT0L_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT0L_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TT0_NSEC
 * Field Desc:  Time trigger 0 nanoseconds count This register is only relevant
 * if the core has been configured to support 1 or more trigger outputs
 * (TRIG_NUM parameter set to 1 or greater).  When the RTC value reaches the
 * time set in the TT0L, TT0M and TT0MSBSEC registers, the RITT0 bit of the RIS
 * register will be set to 1 (or will remain at 1 if it is already 1).  An
 * interrupt can be generated on the time match (depending on the value of the
 * IETT0 bit of the IER) and/or a pulse can be generated on the TRIG0 output
 * (depending on the value of the TT0_EN bit of the GCFG register). Field Type:
 * read-write
 */
#define CORE1588_REGS_TT0L_TT0_NSEC_OFFSET (CORE1588_REGS_TT0L_REG_OFFSET)
#define CORE1588_REGS_TT0L_TT0_NSEC_SHIFT (0U)
#define CORE1588_REGS_TT0L_TT0_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TT0L_TT0_NSEC_MASK                                       \
  (CORE1588_REGS_TT0L_TT0_NSEC_NS_MASK << CORE1588_REGS_TT0L_TT0_NSEC_SHIFT)

/***************************************************************************************************
 * Register: TT0M
 * Description: Time trigger 0 seconds register [31:0].When TRIG_NUM is greater
 * than 0 and the TT0L, TT0M and TT0MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG0 output.Note: TT0MSBSEC register contains upper 16
 * bits [47:32] of seconds
 */
#define CORE1588_REGS_TT0M_REG_OFFSET (0x44U)
#define CORE1588_REGS_TT0M_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT0M_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT0M_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT0M_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT0M_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT0M_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TT0_SEC_LSB_32BITS
 * Field Desc:  Time trigger 0 least significant 32 bits of seconds count.  This
 * register is only relevant if the core has been configured to support 1 or
 * more trigger outputs (TRIG_NUM parameter set to 1 or greater).  When the RTC
 * value reaches the time set in the TT0L, TT0M and TT0MSBSEC registers, the
 * RITT0 bit of the RIS register will be set to 1 (or will remain at 1 if it is
 * already 1). An interrupt can be generated on the time match (depending on the
 * value of the IETT0 bit of the IER) and/or a pulse can be generated on the
 * TRIG0 output (depending on the value of the TT0_EN bit of the GCFG register).
 * Field Type: read-write
 */
#define CORE1588_REGS_TT0M_TT0_SEC_LSB_32BITS_OFFSET                           \
  (CORE1588_REGS_TT0M_REG_OFFSET)
#define CORE1588_REGS_TT0M_TT0_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_TT0M_TT0_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TT0M_TT0_SEC_LSB_32BITS_MASK                             \
  (CORE1588_REGS_TT0M_TT0_SEC_LSB_32BITS_NS_MASK                               \
   << CORE1588_REGS_TT0M_TT0_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: TT1L
 * Description: Time trigger 1 nanoseconds register.When TRIG_NUM is greater
 * than 1 and the TT1L, TT1M and TT1MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG1 output
 */
#define CORE1588_REGS_TT1L_REG_OFFSET (0x48U)
#define CORE1588_REGS_TT1L_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT1L_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT1L_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT1L_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT1L_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT1L_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TT1_NSEC
 * Field Desc:  Time trigger 1 nanoseconds count This register is only relevant
 * if the core has been configured to support 2 or more trigger outputs
 * (TRIG_NUM parameter set to 2 or greater).  When the RTC value reaches the
 * time set in the TT1L, TT1M and TT1MSBSEC registers, the RITT1 bit of the RIS
 * register will be set to 1 (or will remain at 1 if it is already 1).  An
 * interrupt can be generated on the time match (depending on the value of the
 * IETT1 bit of the IER) and/or a pulse can be generated on the TRIG1 output
 * (depending on the value of the TT1_EN bit of the GCFG register). Field Type:
 * read-write
 */
#define CORE1588_REGS_TT1L_TT1_NSEC_OFFSET (CORE1588_REGS_TT1L_REG_OFFSET)
#define CORE1588_REGS_TT1L_TT1_NSEC_SHIFT (0U)
#define CORE1588_REGS_TT1L_TT1_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TT1L_TT1_NSEC_MASK                                       \
  (CORE1588_REGS_TT1L_TT1_NSEC_NS_MASK << CORE1588_REGS_TT1L_TT1_NSEC_SHIFT)

/***************************************************************************************************
 * Register: TT1M
 * Description: Time trigger 1 seconds register [31:0].When TRIG_NUM is greater
 * than 1 and the TT1L, TT1M and TT1MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG1 output. Note: TT1MSBSEC register contains upper 16
 * bits [47:32] of seconds
 */
#define CORE1588_REGS_TT1M_REG_OFFSET (0x4cU)
#define CORE1588_REGS_TT1M_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT1M_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT1M_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT1M_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT1M_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT1M_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TT1_SEC_LSB_32BITS
 * Field Desc:  Time trigger 1 least significant 32 bits of seconds count.  This
 * register is only relevant if the core has been configured to support 2 or
 * more trigger outputs (TRIG_NUM parameter set to 2 or greater).  When the RTC
 * value reaches the time set in the TT1L, TT1M and TT1MSBSEC registers, the
 * RITT1 bit of the RIS register will be set to 1 (or will remain at 1 if it is
 * already 1). An interrupt can be generated on the time match (depending on the
 * value of the IETT1 bit of the IER) and/or a pulse can be generated on the
 * TRIG1 output (depending on the value of the TT1_EN bit of the GCFG register).
 * Field Type: read-write
 */
#define CORE1588_REGS_TT1M_TT1_SEC_LSB_32BITS_OFFSET                           \
  (CORE1588_REGS_TT1M_REG_OFFSET)
#define CORE1588_REGS_TT1M_TT1_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_TT1M_TT1_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TT1M_TT1_SEC_LSB_32BITS_MASK                             \
  (CORE1588_REGS_TT1M_TT1_SEC_LSB_32BITS_NS_MASK                               \
   << CORE1588_REGS_TT1M_TT1_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: TT2L
 * Description: Time trigger 2 nanoseconds register.When TRIG_NUM is greater
 * than 2 and the TT2L, TT2M and TT2MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG2 output
 */
#define CORE1588_REGS_TT2L_REG_OFFSET (0x50U)
#define CORE1588_REGS_TT2L_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT2L_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT2L_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT2L_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT2L_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT2L_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TT2_NSEC
 * Field Desc:  Time trigger 2 nanoseconds count This register is only relevant
 * if the core has been configured to support 3 trigger outputs (TRIG_NUM
 * parameter set to 3).  When the RTC value reaches the time set in the TT2L,
 * TT2M and TT2MSBSEC registers, the RITT2 bit of the RIS register will be set
 * to 1 (or will remain at 1 if it is already 1).  An interrupt can be generated
 * on the time match (depending on the value of the IETT2 bit of the IER) and/or
 * a pulse can be generated on the TRIG2 output (depending on the value of the
 * TT2_EN bit of the GCFG register). Field Type: read-write
 */
#define CORE1588_REGS_TT2L_TT2_NSEC_OFFSET (CORE1588_REGS_TT2L_REG_OFFSET)
#define CORE1588_REGS_TT2L_TT2_NSEC_SHIFT (0U)
#define CORE1588_REGS_TT2L_TT2_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TT2L_TT2_NSEC_MASK                                       \
  (CORE1588_REGS_TT2L_TT2_NSEC_NS_MASK << CORE1588_REGS_TT2L_TT2_NSEC_SHIFT)

/***************************************************************************************************
 * Register: TT2M
 * Description: Time trigger 2 seconds register [31:0].When TRIG_NUM is greater
 * than 2 and the TT2L, TT2M and TT2MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG2 output.Note: TT2MSBSEC register contains upper 16
 * bits [47:32] of seconds
 */
#define CORE1588_REGS_TT2M_REG_OFFSET (0x54U)
#define CORE1588_REGS_TT2M_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT2M_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT2M_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT2M_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT2M_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TT2M_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TT2_SEC_LSB_32BITS
 * Field Desc:  Time trigger 2 least significant 32 bits of seconds count.  This
 * register is only relevant if the core has been configured to support 3
 * trigger outputs (TRIG_NUM parameter set to 3).  When the RTC value reaches
 * the time set in the TT2L, TT2M and TT2MSBSEC registers, the RITT2 bit of the
 * RIS register will be set to 1 (or will remain at 1 if it is already 1). An
 * interrupt can be generated on the time match (depending on the value of the
 * IETT2 bit of the IER) and/or a pulse can be generated on the TRIG2 output
 * (depending on the value of the TT2_EN bit of the GCFG register). Field Type:
 * read-write
 */
#define CORE1588_REGS_TT2M_TT2_SEC_LSB_32BITS_OFFSET                           \
  (CORE1588_REGS_TT2M_REG_OFFSET)
#define CORE1588_REGS_TT2M_TT2_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_TT2M_TT2_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_TT2M_TT2_SEC_LSB_32BITS_MASK                             \
  (CORE1588_REGS_TT2M_TT2_SEC_LSB_32BITS_NS_MASK                               \
   << CORE1588_REGS_TT2M_TT2_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: LT0L
 * Description: Latch 0 nanoseconds register.When LATCH_NUM is greater than 0
 * and a rising edge is detected on the LATCH0 input, nanoseconds count of RTC
 * will be stored in LT0L
 */
#define CORE1588_REGS_LT0L_REG_OFFSET (0x58U)
#define CORE1588_REGS_LT0L_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT0L_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT0L_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT0L_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT0L_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT0L_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: LT0_NSEC
 * Field Desc:  Latch 0 nanoseconds count This register is only relevant if the
 * core has been configured to support 1 or more latch inputs (LATCH_NUM
 * parameter set to 1 or greater). When a rising edge is detected on the LATCH0
 * input, the value in the RTCL register will be stored in this register if the
 * LT0_EN bit of the GCFG register is set to 1. This register will hold its
 * current value if the LT0_EN bit of the GCFG register is 0. The detection of a
 * rising edge on the LATCH0 input will also cause the RILT0 bit of the RIS
 * register to be set to 1 (if it is not already set to 1). This can in turn
 * cause assertion of the INT interrupt output if the IELT0 bit of the IER is
 * set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT0L_LT0_NSEC_OFFSET (CORE1588_REGS_LT0L_REG_OFFSET)
#define CORE1588_REGS_LT0L_LT0_NSEC_SHIFT (0U)
#define CORE1588_REGS_LT0L_LT0_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_LT0L_LT0_NSEC_MASK                                       \
  (CORE1588_REGS_LT0L_LT0_NSEC_NS_MASK << CORE1588_REGS_LT0L_LT0_NSEC_SHIFT)

/***************************************************************************************************
 * Register: LT0M
 * Description: Latch 0 seconds register [31:0].When LATCH_NUM is greater than 0
 * and a rising edge is detected on the LATCH0 input, lower 32 bits [31:0] of
 * seconds count of RTC will be stored in LT0M
 */
#define CORE1588_REGS_LT0M_REG_OFFSET (0x5cU)
#define CORE1588_REGS_LT0M_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT0M_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT0M_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT0M_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT0M_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT0M_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: LT0_SEC_LSB_32BITS
 * Field Desc:  Latch 0 least significant 32 bits of seconds count. This
 * register is only relevant if the core has been configured to support 1 or
 * more latch inputs (LATCH_NUM parameter set to 1 or greater). When a rising
 * edge is detected on the LATCH0 input, the value in the RTCM register will be
 * stored in this register if the LT0_EN bit of the GCFG register is set to 1.
 * This register will hold its current value if the LT0_EN bit of the GCFG
 * register is 0. The detection of a rising edge on the LATCH0 input will also
 * cause the RILT0 bit of the RIS register to be set to 1 (if it is not already
 * set to 1). This can in turn cause assertion of the INT interrupt output if
 * the IELT0 bit of the IER is set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT0M_LT0_SEC_LSB_32BITS_OFFSET                           \
  (CORE1588_REGS_LT0M_REG_OFFSET)
#define CORE1588_REGS_LT0M_LT0_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_LT0M_LT0_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_LT0M_LT0_SEC_LSB_32BITS_MASK                             \
  (CORE1588_REGS_LT0M_LT0_SEC_LSB_32BITS_NS_MASK                               \
   << CORE1588_REGS_LT0M_LT0_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: LT1L
 * Description: Latch 1 nanoseconds register.When LATCH_NUM is greater than 1
 * and a rising edge is detected on the LATCH1 input, nanoseconds count of RTC
 * will be stored in LT1L
 */
#define CORE1588_REGS_LT1L_REG_OFFSET (0x60U)
#define CORE1588_REGS_LT1L_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT1L_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT1L_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT1L_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT1L_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT1L_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: LT1_NSEC
 * Field Desc:  Latch 1 nanoseconds count This register is only relevant if the
 * core has been configured to support 2 or more latch inputs (LATCH_NUM
 * parameter set to 2 or greater). When a rising edge is detected on the LATCH1
 * input, the value in the RTCL register will be stored in this register if the
 * LT1_EN bit of the GCFG register is set to 1. This register will hold its
 * current value if the LT1_EN bit of the GCFG register is 0. The detection of a
 * rising edge on the LATCH1 input will also cause the RILT1 bit of the RIS
 * register to be set to 1 (if it is not already set to 1). This can in turn
 * cause assertion of the INT interrupt output if the IELT1 bit of the IER is
 * set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT1L_LT1_NSEC_OFFSET (CORE1588_REGS_LT1L_REG_OFFSET)
#define CORE1588_REGS_LT1L_LT1_NSEC_SHIFT (0U)
#define CORE1588_REGS_LT1L_LT1_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_LT1L_LT1_NSEC_MASK                                       \
  (CORE1588_REGS_LT1L_LT1_NSEC_NS_MASK << CORE1588_REGS_LT1L_LT1_NSEC_SHIFT)

/***************************************************************************************************
 * Register: LT1M
 * Description: Latch 1 seconds register [31:0].When LATCH_NUM is greater than 1
 * and a rising edge is detected on the LATCH1 input, lower 32 bits [31:0] of
 * seconds count of RTC will be stored in LT1M
 */
#define CORE1588_REGS_LT1M_REG_OFFSET (0x64U)
#define CORE1588_REGS_LT1M_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT1M_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT1M_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT1M_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT1M_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT1M_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: LT1_SEC_LSB_32BITS
 * Field Desc:  Latch 1 least significant 32 bits of seconds count. This
 * register is only relevant if the core has been configured to support 2 or
 * more latch inputs (LATCH_NUM parameter set to 2 or greater). When a rising
 * edge is detected on the LATCH1 input, the value in the RTCM register will be
 * stored in this register if the LT1_EN bit of the GCFG register is set to 1.
 * This register will hold its current value if the LT1_EN bit of the GCFG
 * register is 0. The detection of a rising edge on the LATCH1 input will also
 * cause the RILT1 bit of the RIS register to be set to 1 (if it is not already
 * set to 1). This can in turn cause assertion of the INT interrupt output if
 * the IELT1 bit of the IER is set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT1M_LT1_SEC_LSB_32BITS_OFFSET                           \
  (CORE1588_REGS_LT1M_REG_OFFSET)
#define CORE1588_REGS_LT1M_LT1_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_LT1M_LT1_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_LT1M_LT1_SEC_LSB_32BITS_MASK                             \
  (CORE1588_REGS_LT1M_LT1_SEC_LSB_32BITS_NS_MASK                               \
   << CORE1588_REGS_LT1M_LT1_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: LT2L
 * Description: Latch 2 nanoseconds register.When LATCH_NUM is greater than 2
 * and a rising edge is detected on the LATCH2 input, nanoseconds count of RTC
 * will be stored in LT2L
 */
#define CORE1588_REGS_LT2L_REG_OFFSET (0x68U)
#define CORE1588_REGS_LT2L_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT2L_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT2L_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT2L_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT2L_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT2L_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: LT2_NSEC
 * Field Desc:  Latch 2 nanoseconds count This register is only relevant if the
 * core has been configured to support 3 latch inputs (LATCH_NUM parameter set
 * to 3). When a rising edge is detected on the LATCH2 input, the value in the
 * RTCL register will be stored in this register if the LT2_EN bit of the GCFG
 * register is set to 1. This register will hold its current value if the LT2_EN
 * bit of the GCFG register is 0. The detection of a rising edge on the LATCH2
 * input will also cause the RILT2 bit of the RIS register to be set to 1 (if it
 * is not already set to 1). This can in turn cause assertion of the INT
 * interrupt output if the IELT2 bit of the IER is set to 1. Field Type:
 * read-write
 */
#define CORE1588_REGS_LT2L_LT2_NSEC_OFFSET (CORE1588_REGS_LT2L_REG_OFFSET)
#define CORE1588_REGS_LT2L_LT2_NSEC_SHIFT (0U)
#define CORE1588_REGS_LT2L_LT2_NSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_LT2L_LT2_NSEC_MASK                                       \
  (CORE1588_REGS_LT2L_LT2_NSEC_NS_MASK << CORE1588_REGS_LT2L_LT2_NSEC_SHIFT)

/***************************************************************************************************
 * Register: LT2M
 * Description: Latch 2 lower seconds register [31:0].When LATCH_NUM is greater
 * than 2 and a rising edge is detected on the LATCH2 input, lower 32bits [31:0]
 * of seconds count of RTC will be stored in LT2M
 */
#define CORE1588_REGS_LT2M_REG_OFFSET (0x6cU)
#define CORE1588_REGS_LT2M_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT2M_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT2M_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT2M_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT2M_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_LT2M_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: LT2_SEC_LSB_32BITS
 * Field Desc:  Latch 2 least significant 32 bits of seconds count. This
 * register is only relevant if the core has been configured to support 3 latch
 * inputs (LATCH_NUM parameter set to 3). When a rising edge is detected on the
 * LATCH2 input, the value in the RTCM register will be stored in this register
 * if the LT2_EN bit of the GCFG register is set to 1. This register will hold
 * its current value if the LT2_EN bit of the GCFG register is 0. The detection
 * of a rising edge on the LATCH2 input will also cause the RILT2 bit of the RIS
 * register to be set to 1 (if it is not already set to 1). This can in turn
 * cause assertion of the INT interrupt output if the IELT2 bit of the IER is
 * set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT2M_LT2_SEC_LSB_32BITS_OFFSET                           \
  (CORE1588_REGS_LT2M_REG_OFFSET)
#define CORE1588_REGS_LT2M_LT2_SEC_LSB_32BITS_SHIFT (0U)
#define CORE1588_REGS_LT2M_LT2_SEC_LSB_32BITS_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_LT2M_LT2_SEC_LSB_32BITS_MASK                             \
  (CORE1588_REGS_LT2M_LT2_SEC_LSB_32BITS_NS_MASK                               \
   << CORE1588_REGS_LT2M_LT2_SEC_LSB_32BITS_SHIFT)

/***************************************************************************************************
 * Register: TTSMSBSECSUBNS
 * Description: PTP frame transmitted timestamp seconds [47:32] and
 * sub-nanoseconds register
 */
#define CORE1588_REGS_TTSMSBSECSUBNS_REG_OFFSET (0x70U)
#define CORE1588_REGS_TTSMSBSECSUBNS_REG_LENGTH (0x4U)
#define CORE1588_REGS_TTSMSBSECSUBNS_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TTSMSBSECSUBNS_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TTSMSBSECSUBNS_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TTSMSBSECSUBNS_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TTSMSBSECSUBNS_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_TX_SUBNSEC
 * Field Desc:  PTP frame transmitted timestamp sub-nanosecond most significant
 * 16 bits. These bits are updated with the upper 16 bits [23:8] of
 * sub-nanosecond count of RTC when SFD of a PTP frame is transmitted at GMII
 * interface. For transparent clock, residence time needs to be updated in
 * correctionField which uses lower 16 bits for sub-nanosecond. For transparent
 * and two step clock while transmitting PTP message, sub-nanosecond of egress
 * PTP message should be used in updating correctionField Field Type: read-write
 */
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SUBNSEC_OFFSET                     \
  (CORE1588_REGS_TTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SUBNSEC_SHIFT (16U)
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SUBNSEC_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SUBNSEC_MASK                       \
  (CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SUBNSEC_NS_MASK                         \
   << CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SUBNSEC_SHIFT)

/**
 * Field Name: PTP_TX_SEC_MSB_16BITS
 * Field Desc:  PTP frame transmitted timestamp most significant 16 bits [47:32]
 * of seconds count. These bits are updated with the upper 16 bits [47:32] of
 * second count of RTC when SFD of a PTP transmit frame is detected at GMII
 * interface.. Field Type: read-write
 */
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SEC_MSB_16BITS_OFFSET              \
  (CORE1588_REGS_TTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SEC_MSB_16BITS_NS_MASK             \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SEC_MSB_16BITS_MASK                \
  (CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SEC_MSB_16BITS_NS_MASK                  \
   << CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: RTSMSBSECSUBNS
 * Description: PTP frame received timestamp seconds [47:32] and sub-nanoseconds
 * register
 */
#define CORE1588_REGS_RTSMSBSECSUBNS_REG_OFFSET (0x74U)
#define CORE1588_REGS_RTSMSBSECSUBNS_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTSMSBSECSUBNS_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSMSBSECSUBNS_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTSMSBSECSUBNS_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTSMSBSECSUBNS_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTSMSBSECSUBNS_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_RX_SUBNSEC
 * Field Desc:  PTP frame received timestamp sub-nanosecond most significant 16
 * bits. This field is updated with the upper 16 bits [23:8] of sub-nanosecond
 * count of RTC when SFD of a PTP frame is received at GMII interface. For
 * transparent clock, residence time needs to be updated in correctionField
 * which uses lower 16 bits for sub-nanosecond. For transparent clock while
 * transmitting PTP message, sub-nanosecond of ingress PTP message should be
 * written into upper 16 bits of INGRSTSMSBSECSUBNS register. Field Type:
 * read-write
 */
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SUBNSEC_OFFSET                     \
  (CORE1588_REGS_RTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SUBNSEC_SHIFT (16U)
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SUBNSEC_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SUBNSEC_MASK                       \
  (CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SUBNSEC_NS_MASK                         \
   << CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SUBNSEC_SHIFT)

/**
 * Field Name: PTP_RX_SEC_MSB_16BITS
 * Field Desc:  PTP frame received timestamp most significant 16 bits [47:32] of
 * seconds count. These bits are updated with the upper 16 bits [47:32] of
 * second count of RTC when SFD of a PTP frame is received at GMII interface.
 * Field Type: read-write
 */
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SEC_MSB_16BITS_OFFSET              \
  (CORE1588_REGS_RTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SEC_MSB_16BITS_NS_MASK             \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SEC_MSB_16BITS_MASK                \
  (CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SEC_MSB_16BITS_NS_MASK                  \
   << CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: RTCMSBSEC
 * Description: Internal RTC seconds register [47:32]
 */
#define CORE1588_REGS_RTCMSBSEC_REG_OFFSET (0x78U)
#define CORE1588_REGS_RTCMSBSEC_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTCMSBSEC_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_RTCMSBSEC_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTCMSBSEC_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTCMSBSEC_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_RTCMSBSEC_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: RTC_SEC_MSB_16BITS
 * Field Desc:  Most significant 16 bits [47:32] of internal RTC seconds count.
 * When writing to the internal RTC, RTCL should be written first followed by
 * write to the RTCM and RTCMSBSEC registers. This register can be
 * incremented/decremented by writing to the RTCCTRL register. When reading to
 * the internal RTC value, the RTCL register should always be read first
 * followed by a read of the RTCM and RTCMSBSEC registers Field Type: read-write
 */
#define CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS_OFFSET                      \
  (CORE1588_REGS_RTCMSBSEC_REG_OFFSET)
#define CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS_MASK                        \
  (CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS_NS_MASK                          \
   << CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: TT0MSBSEC
 * Description: Time trigger 0 seconds register [47:32].When TRIG_NUM is greater
 * than 0 and the TT0L, TT0M and TT0MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG0 output
 */
#define CORE1588_REGS_TT0MSBSEC_REG_OFFSET (0x7cU)
#define CORE1588_REGS_TT0MSBSEC_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT0MSBSEC_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_TT0MSBSEC_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT0MSBSEC_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT0MSBSEC_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_TT0MSBSEC_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: TT0_SEC_MSB_16BITS
 * Field Desc:  Time trigger 0 most significant 16 bits [47:32] of seconds
 * count.  This register is only relevant if the core has been configured to
 * support 1 or more trigger outputs (TRIG_NUM parameter set to 1 or greater).
 * When the RTC value reaches the time set in the TT0L, TT0M and TT0MSBSEC
 * registers, the RITT0 bit of the RIS register will be set to 1 (or will remain
 * at 1 if it is already 1). An interrupt can be generated on the time match
 * (depending on the value of the IETT0 bit of the IER) and/or a pulse can be
 * generated on the TRIG0 output (depending on the value of the TT0_EN bit of
 * the GCFG register). Field Type: read-write
 */
#define CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS_OFFSET                      \
  (CORE1588_REGS_TT0MSBSEC_REG_OFFSET)
#define CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS_MASK                        \
  (CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS_NS_MASK                          \
   << CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: TT1MSBSEC
 * Description: Time trigger 1 seconds register [47:32].When TRIG_NUM is greater
 * than 1 and the TT1L, TT1M and TT1MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG1 output
 */
#define CORE1588_REGS_TT1MSBSEC_REG_OFFSET (0x80U)
#define CORE1588_REGS_TT1MSBSEC_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT1MSBSEC_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_TT1MSBSEC_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT1MSBSEC_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT1MSBSEC_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_TT1MSBSEC_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: TT1_SEC_MSB_16BITS
 * Field Desc:  Time trigger 1 most significant 16 bits [47:32] of seconds
 * count.  This register is only relevant if the core has been configured to
 * support 2 or more trigger outputs (TRIG_NUM parameter set to 2 or greater).
 * When the RTC value reaches the time set in the TT1L, TT1M and TT1MSBSEC
 * registers, the RITT1 bit of the RIS register will be set to 1 (or will remain
 * at 1 if it is already 1). An interrupt can be generated on the time match
 * (depending on the value of the IETT1 bit of the IER) and/or a pulse can be
 * generated on the TRIG1 output (depending on the value of the TT1_EN bit of
 * the GCFG register). Field Type: read-write
 */
#define CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS_OFFSET                      \
  (CORE1588_REGS_TT1MSBSEC_REG_OFFSET)
#define CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS_MASK                        \
  (CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS_NS_MASK                          \
   << CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: TT2MSBSEC
 * Description: Time trigger 2 seconds register [47:32].When TRIG_NUM is greater
 * than 2 and the TT2L, TT2M and TT2MSBSEC registers match the RTC value a pulse
 * is generated on the TRIG2 output
 */
#define CORE1588_REGS_TT2MSBSEC_REG_OFFSET (0x84U)
#define CORE1588_REGS_TT2MSBSEC_REG_LENGTH (0x4U)
#define CORE1588_REGS_TT2MSBSEC_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_TT2MSBSEC_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TT2MSBSEC_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TT2MSBSEC_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_TT2MSBSEC_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: TT2_SEC_MSB_16BITS
 * Field Desc:  Time trigger 2 most significant 16 bits [47:32] of seconds
 * count.  This register is only relevant if the core has been configured to
 * support 3 trigger outputs (TRIG_NUM parameter set to 3).  When the RTC value
 * reaches the time set in the TT2L, TT2M and TT2MSBSEC registers, the RITT2 bit
 * of the RIS register will be set to 1 (or will remain at 1 if it is already
 * 1). An interrupt can be generated on the time match (depending on the value
 * of the IETT2 bit of the IER) and/or a pulse can be generated on the TRIG2
 * output (depending on the value of the TT2_EN bit of the GCFG register). Field
 * Type: read-write
 */
#define CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS_OFFSET                      \
  (CORE1588_REGS_TT2MSBSEC_REG_OFFSET)
#define CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS_MASK                        \
  (CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS_NS_MASK                          \
   << CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: LT0MSBSEC
 * Description: Latch 0 seconds register [47:32].When LATCH_NUM is greater than
 * 0 and a rising edge is detected on the LATCH0 input, upper 16 bits [47:32] of
 * seconds count of RTC will be stored in LT0MSBSEC
 */
#define CORE1588_REGS_LT0MSBSEC_REG_OFFSET (0x88U)
#define CORE1588_REGS_LT0MSBSEC_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT0MSBSEC_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_LT0MSBSEC_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT0MSBSEC_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT0MSBSEC_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_LT0MSBSEC_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: LT0_SEC_MSB_16BITS
 * Field Desc:  Latch 0 most significant 16 bits of seconds count. This register
 * is only relevant if the core has been configured to support 1 or more latch
 * inputs (LATCH_NUM parameter set to 1 or greater). When a rising edge is
 * detected on the LATCH0 input, the value in the RTCMSBSEC register will be
 * stored in this register if the LT0_EN bit of the GCFG register is set to 1.
 * This register will hold its current value if the LT0_EN bit of the GCFG
 * register is 0. The detection of a rising edge on the LATCH0 input will also
 * cause the RILT0 bit of the RIS register to be set to 1 (if it is not already
 * set to 1). This can in turn cause assertion of the INT interrupt output if
 * the IELT0 bit of the IER is set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT0MSBSEC_LT0_SEC_MSB_16BITS_OFFSET                      \
  (CORE1588_REGS_LT0MSBSEC_REG_OFFSET)
#define CORE1588_REGS_LT0MSBSEC_LT0_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_LT0MSBSEC_LT0_SEC_MSB_16BITS_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_LT0MSBSEC_LT0_SEC_MSB_16BITS_MASK                        \
  (CORE1588_REGS_LT0MSBSEC_LT0_SEC_MSB_16BITS_NS_MASK                          \
   << CORE1588_REGS_LT0MSBSEC_LT0_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: LT1MSBSEC
 * Description: Latch 1 seconds register [47:32].When LATCH_NUM is greater than
 * 1 and a rising edge is detected on the LATCH1 input, upper 16 bits [47:32] of
 * seconds count of RTC will be stored in LT1MSBSEC
 */
#define CORE1588_REGS_LT1MSBSEC_REG_OFFSET (0x8cU)
#define CORE1588_REGS_LT1MSBSEC_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT1MSBSEC_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_LT1MSBSEC_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT1MSBSEC_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT1MSBSEC_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_LT1MSBSEC_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: LT1_SEC_MSB_16BITS
 * Field Desc:  Latch 1 most significant 16 bits of seconds count. This register
 * is only relevant if the core has been configured to support 2 or more latch
 * inputs (LATCH_NUM parameter set to 2 or greater). When a rising edge is
 * detected on the LATCH1 input, the value in the RTCMSBSEC register will be
 * stored in this register if the LT1_EN bit of the GCFG register is set to 1.
 * This register will hold its current value if the LT1_EN bit of the GCFG
 * register is 0. The detection of a rising edge on the LATCH1 input will also
 * cause the RILT1 bit of the RIS register to be set to 1 (if it is not already
 * set to 1). This can in turn cause assertion of the INT interrupt output if
 * the IELT1 bit of the IER is set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT1MSBSEC_LT1_SEC_MSB_16BITS_OFFSET                      \
  (CORE1588_REGS_LT1MSBSEC_REG_OFFSET)
#define CORE1588_REGS_LT1MSBSEC_LT1_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_LT1MSBSEC_LT1_SEC_MSB_16BITS_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_LT1MSBSEC_LT1_SEC_MSB_16BITS_MASK                        \
  (CORE1588_REGS_LT1MSBSEC_LT1_SEC_MSB_16BITS_NS_MASK                          \
   << CORE1588_REGS_LT1MSBSEC_LT1_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: LT2MSBSEC
 * Description: Latch 2 seconds register [47:32].When LATCH_NUM is greater than
 * 2 and a rising edge is detected on the LATCH2 input, upper 16 bits [47:32] of
 * seconds count of RTC will be stored in LT2MSBSEC
 */
#define CORE1588_REGS_LT2MSBSEC_REG_OFFSET (0x90U)
#define CORE1588_REGS_LT2MSBSEC_REG_LENGTH (0x4U)
#define CORE1588_REGS_LT2MSBSEC_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_LT2MSBSEC_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_LT2MSBSEC_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_LT2MSBSEC_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_LT2MSBSEC_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: LT2_SEC_MSB_16BITS
 * Field Desc:  Latch 2 most significant 16 bits of seconds count. This register
 * is only relevant if the core has been configured to support 3 latch inputs
 * (LATCH_NUM parameter set to 3). When a rising edge is detected on the LATCH2
 * input, the value in the RTCMSBSEC register will be stored in this register if
 * the LT2_EN bit of the GCFG register is set to 1. This register will hold its
 * current value if the LT2_EN bit of the GCFG register is 0. The detection of a
 * rising edge on the LATCH2 input will also cause the RILT2 bit of the RIS
 * register to be set to 1 (if it is not already set to 1). This can in turn
 * cause assertion of the INT interrupt output if the IELT2 bit of the IER is
 * set to 1. Field Type: read-write
 */
#define CORE1588_REGS_LT2MSBSEC_LT2_SEC_MSB_16BITS_OFFSET                      \
  (CORE1588_REGS_LT2MSBSEC_REG_OFFSET)
#define CORE1588_REGS_LT2MSBSEC_LT2_SEC_MSB_16BITS_SHIFT (0U)
#define CORE1588_REGS_LT2MSBSEC_LT2_SEC_MSB_16BITS_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_LT2MSBSEC_LT2_SEC_MSB_16BITS_MASK                        \
  (CORE1588_REGS_LT2MSBSEC_LT2_SEC_MSB_16BITS_NS_MASK                          \
   << CORE1588_REGS_LT2MSBSEC_LT2_SEC_MSB_16BITS_SHIFT)

/***************************************************************************************************
 * Register: RTCCTRL
 * Description: Internal RTC control register
 */
#define CORE1588_REGS_RTCCTRL_REG_OFFSET (0x94U)
#define CORE1588_REGS_RTCCTRL_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTCCTRL_REG_RW_MASK (0xBFFFFFFFU)
#define CORE1588_REGS_RTCCTRL_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTCCTRL_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTCCTRL_REG_READ_MASK (0xBFFFFFFFU)
#define CORE1588_REGS_RTCCTRL_REG_WRITE_MASK (0xBFFFFFFFU)

/**
 * Field Name: ADD_SUBTRACT
 * Field Desc:  0: Adds adj_value to internal RTC 1: Subtracts adj_value from
 * internal RTC Field Type: read-write
 */
#define CORE1588_REGS_RTCCTRL_ADD_SUBTRACT_OFFSET                              \
  (CORE1588_REGS_RTCCTRL_REG_OFFSET)
#define CORE1588_REGS_RTCCTRL_ADD_SUBTRACT_SHIFT (31U)
#define CORE1588_REGS_RTCCTRL_ADD_SUBTRACT_NS_MASK (BIT_MASK_1_BITS)
#define CORE1588_REGS_RTCCTRL_ADD_SUBTRACT_MASK                                \
  (CORE1588_REGS_RTCCTRL_ADD_SUBTRACT_NS_MASK                                  \
   << CORE1588_REGS_RTCCTRL_ADD_SUBTRACT_SHIFT)

/**
 * Field Name: ADJ_VALUE
 * Field Desc:  Internal RTC adjust value. The number of nanoseconds to
 * increment or decrement the internal RTC nanoseconds counter. If necessary the
 * internal RTC seconds counter will be incremented or decremented Field Type:
 * read-write
 */
#define CORE1588_REGS_RTCCTRL_ADJ_VALUE_OFFSET                                 \
  (CORE1588_REGS_RTCCTRL_REG_OFFSET)
#define CORE1588_REGS_RTCCTRL_ADJ_VALUE_SHIFT (0U)
#define CORE1588_REGS_RTCCTRL_ADJ_VALUE_NS_MASK (BIT_MASK_30_BITS)
#define CORE1588_REGS_RTCCTRL_ADJ_VALUE_MASK                                   \
  (CORE1588_REGS_RTCCTRL_ADJ_VALUE_NS_MASK                                     \
   << CORE1588_REGS_RTCCTRL_ADJ_VALUE_SHIFT)

/***************************************************************************************************
 * Register: RTCINCR
 * Description: Internal RTC Nanoseconds/Sub nanoseconds Count Increment
 * Register
 */
#define CORE1588_REGS_RTCINCR_REG_OFFSET (0x98U)
#define CORE1588_REGS_RTCINCR_REG_LENGTH (0x4U)
#define CORE1588_REGS_RTCINCR_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTCINCR_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RTCINCR_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RTCINCR_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RTCINCR_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: NS_INCR
 * Field Desc:  Internal RTC uses upper 8 bits of the register to increment
 * nanosecond counter. Field Type: read-write
 */
#define CORE1588_REGS_RTCINCR_NS_INCR_OFFSET (CORE1588_REGS_RTCINCR_REG_OFFSET)
#define CORE1588_REGS_RTCINCR_NS_INCR_SHIFT (24U)
#define CORE1588_REGS_RTCINCR_NS_INCR_NS_MASK (BIT_MASK_8_BITS)
#define CORE1588_REGS_RTCINCR_NS_INCR_MASK                                     \
  (CORE1588_REGS_RTCINCR_NS_INCR_NS_MASK << CORE1588_REGS_RTCINCR_NS_INCR_SHIFT)

/**
 * Field Name: SUB_NS_INCR
 * Field Desc:  Internal RTC uses lower 24 bits of the register to increment
 * sub-nanosecond counter. To calculate the value to write to the sub-nanosecond
 * register user should take the decimal value of the sub-nanosecond value, then
 * multiply it by 2 to the power of 24 and convert the result to hexadecimal.
 * For example, for a sub-nanosecond value of 0.5 user should write 0x800000
 * (0.5 * 2^24). Field Type: read-write
 */
#define CORE1588_REGS_RTCINCR_SUB_NS_INCR_OFFSET                               \
  (CORE1588_REGS_RTCINCR_REG_OFFSET)
#define CORE1588_REGS_RTCINCR_SUB_NS_INCR_SHIFT (0U)
#define CORE1588_REGS_RTCINCR_SUB_NS_INCR_NS_MASK (BIT_MASK_24_BITS)
#define CORE1588_REGS_RTCINCR_SUB_NS_INCR_MASK                                 \
  (CORE1588_REGS_RTCINCR_SUB_NS_INCR_NS_MASK                                   \
   << CORE1588_REGS_RTCINCR_SUB_NS_INCR_SHIFT)

/***************************************************************************************************
 * Register: TXUCASTADDR0
 * Description: Unicast destination IP address bits 31 down to 0 for PTP
 * transmit frames.
 */
#define CORE1588_REGS_TXUCASTADDR0_REG_OFFSET (0x9cU)
#define CORE1588_REGS_TXUCASTADDR0_REG_LENGTH (0x4U)
#define CORE1588_REGS_TXUCASTADDR0_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR0_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR0_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR0_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR0_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TX_UNICAST_IP_ADDR0
 * Field Desc:  User can configure Unicast IP destination address0 for transmit
 * PTP frames. For IPv4 frames when the address configured in this register
 * matches with the IP destination address of transmitted PTP frames and
 * PTP_UNICAST_EN bit in General Configuration register is set, PTP event frames
 * will be processed. For IPv6 frames when the address configured in registers
 * TX_UNICAST_IP_ADDR0, TX_UNICAST_IP_ADDR1, TX_UNICAST_IP_ADDR2 and
 * TX_UNICAST_IP_ADDR3 matches with the IP destination address of transmitted
 * PTP frames and PTP_UNICAST_EN bit in General Configuration register is set,
 * PTP event frames will be processed Field Type: read-write
 */
#define CORE1588_REGS_TXUCASTADDR0_TX_UNICAST_IP_ADDR0_OFFSET                  \
  (CORE1588_REGS_TXUCASTADDR0_REG_OFFSET)
#define CORE1588_REGS_TXUCASTADDR0_TX_UNICAST_IP_ADDR0_SHIFT (0U)
#define CORE1588_REGS_TXUCASTADDR0_TX_UNICAST_IP_ADDR0_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_TXUCASTADDR0_TX_UNICAST_IP_ADDR0_MASK                    \
  (CORE1588_REGS_TXUCASTADDR0_TX_UNICAST_IP_ADDR0_NS_MASK                      \
   << CORE1588_REGS_TXUCASTADDR0_TX_UNICAST_IP_ADDR0_SHIFT)

/***************************************************************************************************
 * Register: TXUCASTADDR1
 * Description: Unicast destination IP address bits 63 down to 3 for PTP
 * transmit frames.
 */
#define CORE1588_REGS_TXUCASTADDR1_REG_OFFSET (0xa0U)
#define CORE1588_REGS_TXUCASTADDR1_REG_LENGTH (0x4U)
#define CORE1588_REGS_TXUCASTADDR1_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR1_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR1_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR1_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR1_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TX_UNICAST_IP_ADDR1
 * Field Desc:  User can configure Unicast IP destination address1 for transmit
 * PTP frames. For IPv6 frames when the address configured in registers
 * TX_UNICAST_IP_ADDR0, TX_UNICAST_IP_ADDR1, TX_UNICAST_IP_ADDR2 and
 * TX_UNICAST_IP_ADDR3 matches with the IP destination address of transmitted
 * PTP frames and PTP_UNICAST_EN bit in General Configuration register is set,
 * PTP event frames will be processed. Note: For IPv4 frames, this register is
 * not required and it will be ignored by the Core1588 Field Type: read-write
 */
#define CORE1588_REGS_TXUCASTADDR1_TX_UNICAST_IP_ADDR1_OFFSET                  \
  (CORE1588_REGS_TXUCASTADDR1_REG_OFFSET)
#define CORE1588_REGS_TXUCASTADDR1_TX_UNICAST_IP_ADDR1_SHIFT (0U)
#define CORE1588_REGS_TXUCASTADDR1_TX_UNICAST_IP_ADDR1_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_TXUCASTADDR1_TX_UNICAST_IP_ADDR1_MASK                    \
  (CORE1588_REGS_TXUCASTADDR1_TX_UNICAST_IP_ADDR1_NS_MASK                      \
   << CORE1588_REGS_TXUCASTADDR1_TX_UNICAST_IP_ADDR1_SHIFT)

/***************************************************************************************************
 * Register: TXUCASTADDR2
 * Description: Unicast destination IP address bits 95 down to 64 for PTP
 * transmit frames.
 */
#define CORE1588_REGS_TXUCASTADDR2_REG_OFFSET (0xa4U)
#define CORE1588_REGS_TXUCASTADDR2_REG_LENGTH (0x4U)
#define CORE1588_REGS_TXUCASTADDR2_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR2_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR2_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR2_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR2_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TX_UNICAST_IP_ADDR2
 * Field Desc:  User can configure Unicast IP destination address2 for transmit
 * PTP frames. For IPv6 frames when the address configured in registers
 * TX_UNICAST_IP_ADDR0, TX_UNICAST_IP_ADDR1, TX_UNICAST_IP_ADDR2 and
 * TX_UNICAST_IP_ADDR3 matches with the IP destination address of transmitted
 * PTP frames and PTP_UNICAST_EN bit in General Configuration register is set,
 * PTP event frames will be processed. Note: For IPv4 frames, this register is
 * not required and it will be ignored by the Core1588 Field Type: read-write
 */
#define CORE1588_REGS_TXUCASTADDR2_TX_UNICAST_IP_ADDR2_OFFSET                  \
  (CORE1588_REGS_TXUCASTADDR2_REG_OFFSET)
#define CORE1588_REGS_TXUCASTADDR2_TX_UNICAST_IP_ADDR2_SHIFT (0U)
#define CORE1588_REGS_TXUCASTADDR2_TX_UNICAST_IP_ADDR2_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_TXUCASTADDR2_TX_UNICAST_IP_ADDR2_MASK                    \
  (CORE1588_REGS_TXUCASTADDR2_TX_UNICAST_IP_ADDR2_NS_MASK                      \
   << CORE1588_REGS_TXUCASTADDR2_TX_UNICAST_IP_ADDR2_SHIFT)

/***************************************************************************************************
 * Register: TXUCASTADDR3
 * Description: Unicast destination IP address bits 127 down to 96 for PTP
 * transmit frames.
 */
#define CORE1588_REGS_TXUCASTADDR3_REG_OFFSET (0xa8U)
#define CORE1588_REGS_TXUCASTADDR3_REG_LENGTH (0x4U)
#define CORE1588_REGS_TXUCASTADDR3_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR3_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR3_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_TXUCASTADDR3_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_TXUCASTADDR3_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: TX_UNICAST_IP_ADDR3
 * Field Desc:  User can configure Unicast IP destination address3 for transmit
 * PTP frames. For IPv6 frames when the address configured in registers
 * TX_UNICAST_IP_ADDR0, TX_UNICAST_IP_ADDR1, TX_UNICAST_IP_ADDR2 and
 * TX_UNICAST_IP_ADDR3 matches with the IP destination address of transmitted
 * PTP frames and PTP_UNICAST_EN bit in General Configuration register is set,
 * PTP event frames will be processed. Note: For IPv4 frames, this register is
 * not required and it will be ignored by the Core1588 Field Type: read-write
 */
#define CORE1588_REGS_TXUCASTADDR3_TX_UNICAST_IP_ADDR3_OFFSET                  \
  (CORE1588_REGS_TXUCASTADDR3_REG_OFFSET)
#define CORE1588_REGS_TXUCASTADDR3_TX_UNICAST_IP_ADDR3_SHIFT (0U)
#define CORE1588_REGS_TXUCASTADDR3_TX_UNICAST_IP_ADDR3_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_TXUCASTADDR3_TX_UNICAST_IP_ADDR3_MASK                    \
  (CORE1588_REGS_TXUCASTADDR3_TX_UNICAST_IP_ADDR3_NS_MASK                      \
   << CORE1588_REGS_TXUCASTADDR3_TX_UNICAST_IP_ADDR3_SHIFT)

/***************************************************************************************************
 * Register: RXUCASTADDR0
 * Description: Unicast destination IP address bits 31 down to 0 for PTP receive
 * frames.
 */
#define CORE1588_REGS_RXUCASTADDR0_REG_OFFSET (0xacU)
#define CORE1588_REGS_RXUCASTADDR0_REG_LENGTH (0x4U)
#define CORE1588_REGS_RXUCASTADDR0_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR0_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR0_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR0_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR0_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: RX_UNICAST_IP_ADDR0
 * Field Desc:  User can configure Unicast IP destination address0 for receive
 * PTP frames. For IPv4 frames when the address configured in this register
 * matches with the IP destination address of recevied PTP frames and
 * PTP_UNICAST_EN bit in General Configuration register is set, PTP event frames
 * will be processed. For IPv6 frames when the address configured in registers
 * RX_UNICAST_IP_ADDR0, RX_UNICAST_IP_ADDR1, RX_UNICAST_IP_ADDR2 and
 * RX_UNICAST_IP_ADDR3 matches with the IP destination address of received PTP
 * frames and PTP_UNICAST_EN bit in General Configuration register is set, PTP
 * event frames will be processed Field Type: read-write
 */
#define CORE1588_REGS_RXUCASTADDR0_RX_UNICAST_IP_ADDR0_OFFSET                  \
  (CORE1588_REGS_RXUCASTADDR0_REG_OFFSET)
#define CORE1588_REGS_RXUCASTADDR0_RX_UNICAST_IP_ADDR0_SHIFT (0U)
#define CORE1588_REGS_RXUCASTADDR0_RX_UNICAST_IP_ADDR0_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_RXUCASTADDR0_RX_UNICAST_IP_ADDR0_MASK                    \
  (CORE1588_REGS_RXUCASTADDR0_RX_UNICAST_IP_ADDR0_NS_MASK                      \
   << CORE1588_REGS_RXUCASTADDR0_RX_UNICAST_IP_ADDR0_SHIFT)

/***************************************************************************************************
 * Register: RXUCASTADDR1
 * Description: Unicast destination IP address bits 63 down to 32 for PTP
 * receive frames.
 */
#define CORE1588_REGS_RXUCASTADDR1_REG_OFFSET (0xb0U)
#define CORE1588_REGS_RXUCASTADDR1_REG_LENGTH (0x4U)
#define CORE1588_REGS_RXUCASTADDR1_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR1_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR1_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR1_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR1_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: RX_UNICAST_IP_ADDR1
 * Field Desc:  User can configure Unicast IP destination address1 for receive
 * PTP frames. For IPv6 frames when the address configured in registers
 * RX_UNICAST_IP_ADDR0, RX_UNICAST_IP_ADDR1, RX_UNICAST_IP_ADDR2 and
 * RX_UNICAST_IP_ADDR3 matches with the IP destination address of received PTP
 * frames and PTP_UNICAST_EN bit in General Configuration register is set, PTP
 * event frames will be processed. Note: For IPv4 frames, this register is not
 * required and it will be ignored by the Core1588 Field Type: read-write
 */
#define CORE1588_REGS_RXUCASTADDR1_RX_UNICAST_IP_ADDR1_OFFSET                  \
  (CORE1588_REGS_RXUCASTADDR1_REG_OFFSET)
#define CORE1588_REGS_RXUCASTADDR1_RX_UNICAST_IP_ADDR1_SHIFT (0U)
#define CORE1588_REGS_RXUCASTADDR1_RX_UNICAST_IP_ADDR1_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_RXUCASTADDR1_RX_UNICAST_IP_ADDR1_MASK                    \
  (CORE1588_REGS_RXUCASTADDR1_RX_UNICAST_IP_ADDR1_NS_MASK                      \
   << CORE1588_REGS_RXUCASTADDR1_RX_UNICAST_IP_ADDR1_SHIFT)

/***************************************************************************************************
 * Register: RXUCASTADDR2
 * Description: Unicast destination IP address bits 95 down to 64 for PTP
 * receive frames.
 */
#define CORE1588_REGS_RXUCASTADDR2_REG_OFFSET (0xb4U)
#define CORE1588_REGS_RXUCASTADDR2_REG_LENGTH (0x4U)
#define CORE1588_REGS_RXUCASTADDR2_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR2_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR2_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR2_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR2_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: RX_UNICAST_IP_ADDR2
 * Field Desc:  User can configure Unicast IP destination address2 for receive
 * PTP frames. For IPv6 frames when the address configured in registers
 * RX_UNICAST_IP_ADDR0, RX_UNICAST_IP_ADDR1, RX_UNICAST_IP_ADDR2 and
 * RX_UNICAST_IP_ADDR3 matches with the IP destination address of received PTP
 * frames and PTP_UNICAST_EN bit in General Configuration register is set, PTP
 * event frames will be processed. Note: For IPv4 frames, this register is not
 * required and it will be ignored by the Core1588 Field Type: read-write
 */
#define CORE1588_REGS_RXUCASTADDR2_RX_UNICAST_IP_ADDR2_OFFSET                  \
  (CORE1588_REGS_RXUCASTADDR2_REG_OFFSET)
#define CORE1588_REGS_RXUCASTADDR2_RX_UNICAST_IP_ADDR2_SHIFT (0U)
#define CORE1588_REGS_RXUCASTADDR2_RX_UNICAST_IP_ADDR2_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_RXUCASTADDR2_RX_UNICAST_IP_ADDR2_MASK                    \
  (CORE1588_REGS_RXUCASTADDR2_RX_UNICAST_IP_ADDR2_NS_MASK                      \
   << CORE1588_REGS_RXUCASTADDR2_RX_UNICAST_IP_ADDR2_SHIFT)

/***************************************************************************************************
 * Register: RXUCASTADDR3
 * Description: Unicast destination IP address bits 127 down to 96 for PTP
 * receive frames.
 */
#define CORE1588_REGS_RXUCASTADDR3_REG_OFFSET (0xb8U)
#define CORE1588_REGS_RXUCASTADDR3_REG_LENGTH (0x4U)
#define CORE1588_REGS_RXUCASTADDR3_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR3_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR3_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_RXUCASTADDR3_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_RXUCASTADDR3_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: RX_UNICAST_IP_ADDR3
 * Field Desc:  User can configure Unicast IP destination address3 for receive
 * PTP frames. For IPv6 frames when the address configured in registers
 * RX_UNICAST_IP_ADDR0, RX_UNICAST_IP_ADDR1, RX_UNICAST_IP_ADDR2 and
 * RX_UNICAST_IP_ADDR3 matches with the IP destination address of received PTP
 * frames and PTP_UNICAST_EN bit in General Configuration register is set, PTP
 * event frames will be processed. Note: For IPv4 frames, this register is not
 * required and it will be ignored by the Core1588 Field Type: read-write
 */
#define CORE1588_REGS_RXUCASTADDR3_RX_UNICAST_IP_ADDR3_OFFSET                  \
  (CORE1588_REGS_RXUCASTADDR3_REG_OFFSET)
#define CORE1588_REGS_RXUCASTADDR3_RX_UNICAST_IP_ADDR3_SHIFT (0U)
#define CORE1588_REGS_RXUCASTADDR3_RX_UNICAST_IP_ADDR3_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_RXUCASTADDR3_RX_UNICAST_IP_ADDR3_MASK                    \
  (CORE1588_REGS_RXUCASTADDR3_RX_UNICAST_IP_ADDR3_NS_MASK                      \
   << CORE1588_REGS_RXUCASTADDR3_RX_UNICAST_IP_ADDR3_SHIFT)

/***************************************************************************************************
 * Register: PEERTTSL
 * Description: PTP peer frame transmitted timestamp nanoseconds register
 */
#define CORE1588_REGS_PEERTTSL_REG_OFFSET (0xbcU)
#define CORE1588_REGS_PEERTTSL_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERTTSL_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERTTSL_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSL_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSL_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERTTSL_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_TXNSEC
 * Field Desc:  PTP peer frame transmitted timestamp nanoseconds. The register
 * is updated with the nanosecond count of RTC when SFD of a PTP peer frame
 * (PDELAY Request or PDELAY Response) is transmitted at GMII interface. An
 * interrupt is issued when the register is updated. Field Type: read-write
 */
#define CORE1588_REGS_PEERTTSL_PTP_PEER_TXNSEC_OFFSET                          \
  (CORE1588_REGS_PEERTTSL_REG_OFFSET)
#define CORE1588_REGS_PEERTTSL_PTP_PEER_TXNSEC_SHIFT (0U)
#define CORE1588_REGS_PEERTTSL_PTP_PEER_TXNSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_PEERTTSL_PTP_PEER_TXNSEC_MASK                            \
  (CORE1588_REGS_PEERTTSL_PTP_PEER_TXNSEC_NS_MASK                              \
   << CORE1588_REGS_PEERTTSL_PTP_PEER_TXNSEC_SHIFT)

/***************************************************************************************************
 * Register: PEERTTSM
 * Description: PTP peer frame transmitted timestamp seconds register
 * [31:0].Note: PEERTTSMSBSECSUBNS register contains upper 16 bits [47:32] of
 * seconds
 */
#define CORE1588_REGS_PEERTTSM_REG_OFFSET (0xc0U)
#define CORE1588_REGS_PEERTTSM_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERTTSM_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERTTSM_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSM_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSM_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERTTSM_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_TXSECLSB32BITS
 * Field Desc:  PTP peer frame transmitted timestamp seconds [31:0]. The
 * register is updated with the lower 32 bits [31:0] of second count of RTC when
 * SFD of a PTP peer frame (PDELAY Request or PDELAY Response) is transmitted at
 * GMII interface. An interrupt is issued when the register is updated. Field
 * Type: read-write
 */
#define CORE1588_REGS_PEERTTSM_PTP_PEER_TXSECLSB32BITS_OFFSET                  \
  (CORE1588_REGS_PEERTTSM_REG_OFFSET)
#define CORE1588_REGS_PEERTTSM_PTP_PEER_TXSECLSB32BITS_SHIFT (0U)
#define CORE1588_REGS_PEERTTSM_PTP_PEER_TXSECLSB32BITS_NS_MASK                 \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_PEERTTSM_PTP_PEER_TXSECLSB32BITS_MASK                    \
  (CORE1588_REGS_PEERTTSM_PTP_PEER_TXSECLSB32BITS_NS_MASK                      \
   << CORE1588_REGS_PEERTTSM_PTP_PEER_TXSECLSB32BITS_SHIFT)

/***************************************************************************************************
 * Register: PEERTTSMSBSECSUBNS
 * Description: PTP peer frame transmitted timestamp seconds [47:32] and
 * sub-nanoseconds register
 */
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_OFFSET (0xc4U)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_TXSUBNSEC
 * Field Desc:  PTP peer frame transmitted timestamp sub-nanosecond most
 * significant 16 bits. These bits are updated with the upper 16 bits [23:8] of
 * sub-nanosecond count of RTC when SFD of a PTP peer frame (PDELAY Request or
 * PDELAY Response) is transmitted at GMII interface. For transparent clock,
 * residence time needs to be updated in correctionField which uses lower 16
 * bits for sub-nanosecond. For transparent and two step clock while
 * transmitting PTP peer message, sub-nanosecond of egress PTP peer message
 * should be used in updating correctionField Field Type: read-write
 */
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSUBNSEC_OFFSET             \
  (CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSUBNSEC_SHIFT (16U)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSUBNSEC_NS_MASK            \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSUBNSEC_MASK               \
  (CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSUBNSEC_NS_MASK                 \
   << CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSUBNSEC_SHIFT)

/**
 * Field Name: PTP_PEER_TXSECMSB16BITS
 * Field Desc:  PTP peer frame transmitted timestamp most significant 16 bits
 * [47:32] of seconds count. These bits are updated with the upper 16 bits
 * [47:32] of second count of RTC when SFD of a PTP peer frame (PDELAY Request
 * or PDELAY Response) is transmitted at GMII interface. Field Type: read-write
 */
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSECMSB16BITS_OFFSET        \
  (CORE1588_REGS_PEERTTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSECMSB16BITS_SHIFT (0U)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSECMSB16BITS_NS_MASK       \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSECMSB16BITS_MASK          \
  (CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSECMSB16BITS_NS_MASK            \
   << CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSECMSB16BITS_SHIFT)

/***************************************************************************************************
 * Register: PEERTTSID
 * Description: PTP peer frame transmitted identification register. It contains
 * the 16-bit sequence ID
 */
#define CORE1588_REGS_PEERTTSID_REG_OFFSET (0xc8U)
#define CORE1588_REGS_PEERTTSID_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERTTSID_REG_RW_MASK (0x0000FFFFU)
#define CORE1588_REGS_PEERTTSID_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSID_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERTTSID_REG_READ_MASK (0x0000FFFFU)
#define CORE1588_REGS_PEERTTSID_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: PTP_PEER_TXSEQID
 * Field Desc:  Contains the 16 bits sequenceID from the message header of the
 * transmitted ptp peer frame (PDELAY Request or PDELAY Response) associated
 * with the current transmitted timestamp (in the PEERTTSL, PEERTTSM and
 * PEERTTSMSBSECSUBNS registers) Field Type: read-write
 */
#define CORE1588_REGS_PEERTTSID_PTP_PEER_TXSEQID_OFFSET                        \
  (CORE1588_REGS_PEERTTSID_REG_OFFSET)
#define CORE1588_REGS_PEERTTSID_PTP_PEER_TXSEQID_SHIFT (0U)
#define CORE1588_REGS_PEERTTSID_PTP_PEER_TXSEQID_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_PEERTTSID_PTP_PEER_TXSEQID_MASK                          \
  (CORE1588_REGS_PEERTTSID_PTP_PEER_TXSEQID_NS_MASK                            \
   << CORE1588_REGS_PEERTTSID_PTP_PEER_TXSEQID_SHIFT)

/***************************************************************************************************
 * Register: PEERRTSL
 * Description: PTP peer frame received timestamp nanoseconds register
 */
#define CORE1588_REGS_PEERRTSL_REG_OFFSET (0xccU)
#define CORE1588_REGS_PEERRTSL_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERRTSL_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSL_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSL_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSL_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSL_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_RXNSEC
 * Field Desc:  PTP peer frame received timestamp nanoseconds. The register is
 * updated with the nanosecond count of RTC when SFD of a PTP peer frame (PDELAY
 * Request or PDELAY Response) is received at GMII interface. An interrupt is
 * issued when the register is updated. Field Type: read-write
 */
#define CORE1588_REGS_PEERRTSL_PTP_PEER_RXNSEC_OFFSET                          \
  (CORE1588_REGS_PEERRTSL_REG_OFFSET)
#define CORE1588_REGS_PEERRTSL_PTP_PEER_RXNSEC_SHIFT (0U)
#define CORE1588_REGS_PEERRTSL_PTP_PEER_RXNSEC_NS_MASK (BIT_MASK_32_BITS)
#define CORE1588_REGS_PEERRTSL_PTP_PEER_RXNSEC_MASK                            \
  (CORE1588_REGS_PEERRTSL_PTP_PEER_RXNSEC_NS_MASK                              \
   << CORE1588_REGS_PEERRTSL_PTP_PEER_RXNSEC_SHIFT)

/***************************************************************************************************
 * Register: PEERRTSM
 * Description: PTP peer frame received timestamp seconds register [31:0].Note:
 * PEERRTSMSBSECSUBNS register contains upper 16 bits [47:32] of seconds
 */
#define CORE1588_REGS_PEERRTSM_REG_OFFSET (0xd0U)
#define CORE1588_REGS_PEERRTSM_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERRTSM_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSM_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSM_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSM_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSM_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_RXSEC_LSB32BITS
 * Field Desc:  PTP peer frame received timestamp seconds [31:0]. The register
 * is updated with the lower 32 bits [31:0] of second count of RTC when SFD of a
 * PTP peer frame (PDELAY Request or PDELAY Response) is received at GMII
 * interface. An interrupt is issued when the register is updated. Field Type:
 * read-write
 */
#define CORE1588_REGS_PEERRTSM_PTP_PEER_RXSEC_LSB32BITS_OFFSET                 \
  (CORE1588_REGS_PEERRTSM_REG_OFFSET)
#define CORE1588_REGS_PEERRTSM_PTP_PEER_RXSEC_LSB32BITS_SHIFT (0U)
#define CORE1588_REGS_PEERRTSM_PTP_PEER_RXSEC_LSB32BITS_NS_MASK                \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_PEERRTSM_PTP_PEER_RXSEC_LSB32BITS_MASK                   \
  (CORE1588_REGS_PEERRTSM_PTP_PEER_RXSEC_LSB32BITS_NS_MASK                     \
   << CORE1588_REGS_PEERRTSM_PTP_PEER_RXSEC_LSB32BITS_SHIFT)

/***************************************************************************************************
 * Register: PEERRTSMSBSECSUBNS
 * Description: PTP peer frame received timestamp seconds [47:32] and
 * sub-nanoseconds register
 */
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_OFFSET (0xd4U)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_RXSUBNSEC
 * Field Desc:  PTP peer frame received timestamp sub-nanosecond most
 * significant 16 bits. This field is updated with the upper 16 bits [23:8] of
 * sub-nanosecond count of RTC when SFD of a PTP peer frame (PDELAY Request or
 * PDELAY Response) is received at GMII interface. For transparent clock,
 * residence time needs to be updated in correctionField which uses lower 16
 * bits for sub-nanosecond. For transparent clock while transmitting PTP peer
 * message, sub-nanosecond of ingress PTP peer message should be written into
 * upper 16 bits of PEERINGRSTSMSBSECSUBNS register. Field Type: read-write
 */
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSUBNSEC_OFFSET             \
  (CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSUBNSEC_SHIFT (16U)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSUBNSEC_NS_MASK            \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSUBNSEC_MASK               \
  (CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSUBNSEC_NS_MASK                 \
   << CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSUBNSEC_SHIFT)

/**
 * Field Name: PTP_PEER_RXSEC_MSB16BITS
 * Field Desc:  PTP peer frame received timestamp most significant 16 bits
 * [47:32] of seconds count. These bits are updated with the upper 16 bits
 * [47:32] of second count of RTC when SFD of a PTP peer frame (PDELAY Request
 * or PDELAY Response) is received at GMII interface. Field Type: read-write
 */
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSEC_MSB16BITS_OFFSET       \
  (CORE1588_REGS_PEERRTSMSBSECSUBNS_REG_OFFSET)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSEC_MSB16BITS_SHIFT (0U)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSEC_MSB16BITS_NS_MASK      \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSEC_MSB16BITS_MASK         \
  (CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSEC_MSB16BITS_NS_MASK           \
   << CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSEC_MSB16BITS_SHIFT)

/***************************************************************************************************
 * Register: PEERRTSID2
 * Description: PTP peer frame received identification register2. Contains the
 * 16-bit sequence ID and the bits [79:64] of the sourcePortIdentity
 */
#define CORE1588_REGS_PEERRTSID2_REG_OFFSET (0xd8U)
#define CORE1588_REGS_PEERRTSID2_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERRTSID2_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSID2_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSID2_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSID2_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSID2_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_RXSEQID
 * Field Desc:  Contains the 16 bits sequenceID from the message header of the
 * ptp peer frame (PDELAY Request or PDELAY Response) associated with the
 * current receive timestamp (in the PEERRTSL, PEERRTSM and PEERRTSMSBSECSUBNS
 * egisters) Field Type: read-write
 */
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSEQID_OFFSET                       \
  (CORE1588_REGS_PEERRTSID2_REG_OFFSET)
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSEQID_SHIFT (16U)
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSEQID_NS_MASK (BIT_MASK_16_BITS)
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSEQID_MASK                         \
  (CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSEQID_NS_MASK                           \
   << CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSEQID_SHIFT)

/**
 * Field Name: PTP_PEER_RXSRCPORTID_MSB16BITS
 * Field Desc:  Contains the upper 16 bits [79:64] of sourcePortIdentity from
 * the message header of the received ptp peer frame (PDELAY Request or PDELAY
 * Response) associated with the current receive timestamp (in the PEERRTSL,
 * PEERRTSM and PEERRTSMSBSEC registers) Field Type: read-write
 */
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSRCPORTID_MSB16BITS_OFFSET         \
  (CORE1588_REGS_PEERRTSID2_REG_OFFSET)
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSRCPORTID_MSB16BITS_SHIFT (0U)
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSRCPORTID_MSB16BITS_NS_MASK        \
  (BIT_MASK_16_BITS)
#define CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSRCPORTID_MSB16BITS_MASK           \
  (CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSRCPORTID_MSB16BITS_NS_MASK             \
   << CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSRCPORTID_MSB16BITS_SHIFT)

/***************************************************************************************************
 * Register: PEERRTSID1
 * Description: PTP peer frame received identification register1. Contains bits
 * [63:32] of the sourcePortIdentity
 */
#define CORE1588_REGS_PEERRTSID1_REG_OFFSET (0xdcU)
#define CORE1588_REGS_PEERRTSID1_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERRTSID1_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSID1_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSID1_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSID1_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSID1_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_RXSRCPORTID_MID32BITS
 * Field Desc:  Contains the bits [63:32] of sourcePortIdentity from the message
 * header of the received ptp peer frame ((PDELAY Request or PDELAY Response))
 * associated with the current receive timestamp (in the PEERRTSL, PEERRTSM and
 * PEERRTSMSBSECSUBNS registers) Field Type: read-write
 */
#define CORE1588_REGS_PEERRTSID1_PTP_PEER_RXSRCPORTID_MID32BITS_OFFSET         \
  (CORE1588_REGS_PEERRTSID1_REG_OFFSET)
#define CORE1588_REGS_PEERRTSID1_PTP_PEER_RXSRCPORTID_MID32BITS_SHIFT (0U)
#define CORE1588_REGS_PEERRTSID1_PTP_PEER_RXSRCPORTID_MID32BITS_NS_MASK        \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_PEERRTSID1_PTP_PEER_RXSRCPORTID_MID32BITS_MASK           \
  (CORE1588_REGS_PEERRTSID1_PTP_PEER_RXSRCPORTID_MID32BITS_NS_MASK             \
   << CORE1588_REGS_PEERRTSID1_PTP_PEER_RXSRCPORTID_MID32BITS_SHIFT)

/***************************************************************************************************
 * Register: PEERRTSID0
 * Description: PTP peer frame received identification register0. Contains bits
 * [31:0] of the sourcePortIdentity
 */
#define CORE1588_REGS_PEERRTSID0_REG_OFFSET (0xe0U)
#define CORE1588_REGS_PEERRTSID0_REG_LENGTH (0x4U)
#define CORE1588_REGS_PEERRTSID0_REG_RW_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSID0_REG_RO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSID0_REG_WO_MASK (0x00000000U)
#define CORE1588_REGS_PEERRTSID0_REG_READ_MASK (0xFFFFFFFFU)
#define CORE1588_REGS_PEERRTSID0_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: PTP_PEER_RXSRCPORTID_LSB32BITS
 * Field Desc:  Contains the bits [31:0] of sourcePortIdentity from the message
 * header of the received ptp peer frame (PDELAY Request or PDELAY Response)
 * associated with the current receive timestamp (in the PEERRTSL, PEERRTSM and
 * PEERRTSMSBSECSUBNS registers) Field Type: read-write
 */
#define CORE1588_REGS_PEERRTSID0_PTP_PEER_RXSRCPORTID_LSB32BITS_OFFSET         \
  (CORE1588_REGS_PEERRTSID0_REG_OFFSET)
#define CORE1588_REGS_PEERRTSID0_PTP_PEER_RXSRCPORTID_LSB32BITS_SHIFT (0U)
#define CORE1588_REGS_PEERRTSID0_PTP_PEER_RXSRCPORTID_LSB32BITS_NS_MASK        \
  (BIT_MASK_32_BITS)
#define CORE1588_REGS_PEERRTSID0_PTP_PEER_RXSRCPORTID_LSB32BITS_MASK           \
  (CORE1588_REGS_PEERRTSID0_PTP_PEER_RXSRCPORTID_LSB32BITS_NS_MASK             \
   << CORE1588_REGS_PEERRTSID0_PTP_PEER_RXSRCPORTID_LSB32BITS_SHIFT)

#define MASK_BIT_31 ((uint32_t)(0x80000000UL))
#define MASK_BIT_30 ((uint32_t)(0x40000000UL))
#define MASK_BIT_29 ((uint32_t)(0x20000000UL))
#define MASK_BIT_28 ((uint32_t)(0x10000000UL))
#define MASK_BIT_27 ((uint32_t)(0x08000000UL))
#define MASK_BIT_26 ((uint32_t)(0x04000000UL))
#define MASK_BIT_25 ((uint32_t)(0x02000000UL))
#define MASK_BIT_24 ((uint32_t)(0x01000000UL))
#define MASK_BIT_23 ((uint32_t)(0x00800000UL))
#define MASK_BIT_22 ((uint32_t)(0x00400000UL))
#define MASK_BIT_21 ((uint32_t)(0x00200000UL))
#define MASK_BIT_20 ((uint32_t)(0x00100000UL))
#define MASK_BIT_19 ((uint32_t)(0x00080000UL))
#define MASK_BIT_18 ((uint32_t)(0x00040000UL))
#define MASK_BIT_17 ((uint32_t)(0x00020000UL))
#define MASK_BIT_16 ((uint32_t)(0x00010000UL))
#define MASK_BIT_15 ((uint32_t)(0x00008000UL))
#define MASK_BIT_14 ((uint32_t)(0x00004000UL))
#define MASK_BIT_13 ((uint32_t)(0x00002000UL))
#define MASK_BIT_12 ((uint32_t)(0x00001000UL))
#define MASK_BIT_11 ((uint32_t)(0x00000800UL))
#define MASK_BIT_10 ((uint32_t)(0x00000400UL))
#define MASK_BIT_9 ((uint32_t)(0x00000200UL))
#define MASK_BIT_8 ((uint32_t)(0x00000100UL))
#define MASK_BIT_7 ((uint32_t)(0x00000080UL))
#define MASK_BIT_6 ((uint32_t)(0x00000040UL))
#define MASK_BIT_5 ((uint32_t)(0x00000020UL))
#define MASK_BIT_4 ((uint32_t)(0x00000010UL))
#define MASK_BIT_3 ((uint32_t)(0x00000008UL))
#define MASK_BIT_2 ((uint32_t)(0x00000004UL))
#define MASK_BIT_1 ((uint32_t)(0x00000002UL))
#define MASK_BIT_0 ((uint32_t)(0x00000001UL))

#define BIT_MASK_32_BITS ((uint32_t)(0xFFFFFFFFUL))
#define BIT_MASK_31_BITS ((uint32_t)(0x7FFFFFFFUL))
#define BIT_MASK_30_BITS ((uint32_t)(0x3FFFFFFFUL))
#define BIT_MASK_29_BITS ((uint32_t)(0x1FFFFFFFUL))
#define BIT_MASK_28_BITS ((uint32_t)(0x0FFFFFFFUL))
#define BIT_MASK_27_BITS ((uint32_t)(0x07FFFFFFUL))
#define BIT_MASK_26_BITS ((uint32_t)(0x03FFFFFFUL))
#define BIT_MASK_25_BITS ((uint32_t)(0x01FFFFFFUL))
#define BIT_MASK_24_BITS ((uint32_t)(0x00FFFFFFUL))
#define BIT_MASK_23_BITS ((uint32_t)(0x007FFFFFUL))
#define BIT_MASK_22_BITS ((uint32_t)(0x003FFFFFUL))
#define BIT_MASK_21_BITS ((uint32_t)(0x001FFFFFUL))
#define BIT_MASK_20_BITS ((uint32_t)(0x000FFFFFUL))
#define BIT_MASK_19_BITS ((uint32_t)(0x0007FFFFUL))
#define BIT_MASK_18_BITS ((uint32_t)(0x0003FFFFUL))
#define BIT_MASK_17_BITS ((uint32_t)(0x0001FFFFUL))
#define BIT_MASK_16_BITS ((uint32_t)(0x0000FFFFUL))
#define BIT_MASK_15_BITS ((uint32_t)(0x00007FFFUL))
#define BIT_MASK_14_BITS ((uint32_t)(0x00003FFFUL))
#define BIT_MASK_13_BITS ((uint32_t)(0x00001FFFUL))
#define BIT_MASK_12_BITS ((uint32_t)(0x00000FFFUL))
#define BIT_MASK_11_BITS ((uint32_t)(0x000007FFUL))
#define BIT_MASK_10_BITS ((uint32_t)(0x000003FFUL))
#define BIT_MASK_9_BITS ((uint32_t)(0x000001FFUL))
#define BIT_MASK_8_BITS ((uint32_t)(0x000000FFUL))
#define BIT_MASK_7_BITS ((uint32_t)(0x0000007FUL))
#define BIT_MASK_6_BITS ((uint32_t)(0x0000003FUL))
#define BIT_MASK_5_BITS ((uint32_t)(0x0000001FUL))
#define BIT_MASK_4_BITS ((uint32_t)(0x0000000FUL))
#define BIT_MASK_3_BITS ((uint32_t)(0x00000007UL))
#define BIT_MASK_2_BITS ((uint32_t)(0x00000003UL))
#define BIT_MASK_1_BITS ((uint32_t)(0x00000001UL))

#ifdef __cplusplus
}
#endif

#endif /* CORE1588_REGS_H_ */
