/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file corecan_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreCAN register header file
 *
 */

#ifndef CORECAN_REG_MAP_REGS_H_
#define CORECAN_REG_MAP_REGS_H_
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Macro definitions
 *
 * CORECAN_<REG_NAME>_REG_OFFSET => Register offset from base address.
 *
 * CORECAN_<REG_NAME>_REG_RESET_VALUE => Register default reset value.
 *
 * CORECAN_<REG_NAME>_REG_LENGTH => Register length in number of bytes.
 *
 * CORECAN_<REG_NAME>_RW_MASK => This identifies what read/write bits are used
 * within a register.
 *
 * CORECAN_<REG_NAME>_RO_MASK => This identifies what read only bits are used
 * within a register.
 *
 * CORECAN_<REG_NAME>_WO_MASK => This identifies what write only bits are used
 * within a register.
 *
 * CORECAN_<REG_NAME>_READ_MASK => This identifies what read bits are used
 * within a register.
 *
 * CORECAN_<REG_NAME>_WRITE_MASK => This identifies what write bits are used
 * within a register.
 *
 * CORECAN_<REG_NAME>_<FIELD_NAME>_OFFSET => This describes the register offset
 * from the base
 * address for a specific field.
 *
 * CORECAN_<REG_NAME>_<FIELD_NAME>_SHIFT => Bit field shift.
 *
 * CORECAN_<REG_NAME>_<FIELD_NAME>_NS_MASK => This describes the field mask
 * without the shift
 * included. This mask is based on the width of the bit field.
 *
 * CORECAN_<REG_NAME>_<FIELD_NAME>_MASK => This describes the field mask with
 * the shift included.
 */

/*******************************************************************************
 * Register: INTERRUPT_SOURCE_REGISTER
 *
 * Description: Interrupt Status Register
 */
#define CORECAN_INTR_SRC_REG_OFFSET                (0x0U)
#define CORECAN_INTR_SRC_REG_RESET_VALUE           ((uint32_t)(0x00000000UL))
#define CORECAN_INTR_SRC_REG_LENGTH                (0x4U)
#define CORECAN_INTR_SRC_REG_RW_MASK               (0x0000FFFCU)
#define CORECAN_INTR_SRC_REG_RO_MASK               (0x00000000U)
#define CORECAN_INTR_SRC_REG_WO_MASK               (0x00000000U)
#define CORECAN_INTR_SRC_REG_READ_MASK             (0x0000FFFCU)
#define CORECAN_INTR_SRC_REG_WRITE_MASK            (0x0000FFFCU)

/**
 * Field Name: OST_FAILURE
 *
 * Field Desc:
 *
 * 0b1 Indicates the buffer set for single shot transmission experienced an
 * arbitration loss or a bus error during transmission. 0b0 Normal operation.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_OST_FAILURE_OFFSET        (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_OST_FAILURE_SHIFT         (15U)
#define CORECAN_INTR_SRC_OST_FAILURE_NS_MASK       (0x1U)
#define CORECAN_INTR_SRC_OST_FAILURE_MASK          ((0x1U) << (15U))

/**
 * Field Name: STUCK_AT_0
 *
 * Field Desc:
 *
 * 0b1 Indicates that the RX input remains stuck at 0 dominant level for more
 * than 11 consecutive bit times. 0b0 Normal operation.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_STUCK_AT_0_OFFSET         (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_STUCK_AT_0_SHIFT          (14U)
#define CORECAN_INTR_SRC_STUCK_AT_0_NS_MASK        (0x1U)
#define CORECAN_INTR_SRC_STUCK_AT_0_MASK           ((0x1U) << (14U))

/**
 * Field Name: RTR_AUTO_MSG
 *
 * Field Desc:
 *
 * 0b1 Indicates that RTR autoreply message is sent.
 * 0b0 Indicates that RTR autoreply message is not sent.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_RTR_AUTO_MSG_OFFSET       (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_RTR_AUTO_MSG_SHIFT        (13U)
#define CORECAN_INTR_SRC_RTR_AUTO_MSG_NS_MASK      (0x1U)
#define CORECAN_INTR_SRC_RTR_AUTO_MSG_MASK         ((0x1U) << (13U))

/**
 * Field Name: RX_MSG
 *
 * Field Desc:
 *
 * 0b1 When RXINTEBL flag of receive message buffer is set to 1 this bit
 * indicates the new message is successfully received and available in a receive
 * buffer. 0b0 Indicates the new message is not received.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_RX_MSG_OFFSET             (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_RX_MSG_SHIFT              (12U)
#define CORECAN_INTR_SRC_RX_MSG_NS_MASK            (0x1U)
#define CORECAN_INTR_SRC_RX_MSG_MASK               ((0x1U) << (12U))

/**
 * Field Name: TX_MSG
 *
 * Field Desc:
 *
 * 0b1 When TXINTEBL flag of transmit message buffer is set to 1 this bit
 * indicates the message is successfully sent from the transmit buffer. 0b0
 * Indicates the message is not sent from a transmit buffer.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_TX_MSG_OFFSET             (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_TX_MSG_SHIFT              (11U)
#define CORECAN_INTR_SRC_TX_MSG_NS_MASK            (0x1U)
#define CORECAN_INTR_SRC_TX_MSG_MASK               ((0x1U) << (11U))

/**
 * Field Name: RX_MSG_LOSS
 *
 * Field Desc:
 *
 * 0b1 Newly received message cannot be stored because the target message buffer
 * is full ie target message buffers MSGAV flag is set. 0b0 Newly received
 * message is not lost.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_RX_MSG_LOSS_OFFSET        (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_RX_MSG_LOSS_SHIFT         (10U)
#define CORECAN_INTR_SRC_RX_MSG_LOSS_NS_MASK       (0x1U)
#define CORECAN_INTR_SRC_RX_MSG_LOSS_MASK          ((0x1U) << (10U))

/**
 * Field Name: BUS_OFF
 *
 * Field Desc:
 *
 * 0b1 Indicates that the CANController entered the busoff error state.
 * 0b0 Indicates normal operation.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_BUS_OFF_OFFSET            (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_BUS_OFF_SHIFT             (9U)
#define CORECAN_INTR_SRC_BUS_OFF_NS_MASK           (0x1U)
#define CORECAN_INTR_SRC_BUS_OFF_MASK              ((0x1U) << (9U))

/**
 * Field Name: CRC_ERR
 *
 * Field Desc:
 *
 * 0b1 Indicates crc error is detected.
 * 0b0 Indicates crc error is not detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_CRC_ERR_OFFSET            (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_CRC_ERR_SHIFT             (8U)
#define CORECAN_INTR_SRC_CRC_ERR_NS_MASK           (0x1U)
#define CORECAN_INTR_SRC_CRC_ERR_MASK              ((0x1U) << (8U))

/**
 * Field Name: FORM_ERR
 *
 * Field Desc:
 *
 * 0b1 Indicates form error is detected.
 * 0b0 Indicates form error is not detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_FORM_ERR_OFFSET           (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_FORM_ERR_SHIFT            (7U)
#define CORECAN_INTR_SRC_FORM_ERR_NS_MASK          (0x1U)
#define CORECAN_INTR_SRC_FORM_ERR_MASK             ((0x1U) << (7U))

/**
 * Field Name: ACK_ERR
 *
 * Field Desc:
 *
 * 0b1 Indicates acknowledgement error is detected.
 * 0b0 Indicates acknowledgement is received.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_ACK_ERR_OFFSET            (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_ACK_ERR_SHIFT             (6U)
#define CORECAN_INTR_SRC_ACK_ERR_NS_MASK           (0x1U)
#define CORECAN_INTR_SRC_ACK_ERR_MASK              ((0x1U) << (6U))

/**
 * Field Name: STUFF_ERR
 *
 * Field Desc:
 *
 * 0b1 Indicates bit stuffing error is detected.
 * 0b0 Indicates bit stuffing error is not detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_STUFF_ERR_OFFSET          (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_STUFF_ERR_SHIFT           (5U)
#define CORECAN_INTR_SRC_STUFF_ERR_NS_MASK         (0x1U)
#define CORECAN_INTR_SRC_STUFF_ERR_MASK            ((0x1U) << (5U))

/**
 * Field Name: BIT_ERR
 *
 * Field Desc:
 *
 * 0b1 Indicates bit error is detected.
 * 0b0 Indicates bit error is not detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_BIT_ERR_OFFSET            (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_BIT_ERR_SHIFT             (4U)
#define CORECAN_INTR_SRC_BIT_ERR_NS_MASK           (0x1U)
#define CORECAN_INTR_SRC_BIT_ERR_MASK              ((0x1U) << (4U))

/**
 * Field Name: OVR_LOAD
 *
 * Field Desc:
 *
 * 0b1 Indicates overload message is detected.
 * 0b0 Indicates overload message is not detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_OVR_LOAD_OFFSET           (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_OVR_LOAD_SHIFT            (3U)
#define CORECAN_INTR_SRC_OVR_LOAD_NS_MASK          (0x1U)
#define CORECAN_INTR_SRC_OVR_LOAD_MASK             ((0x1U) << (3U))

/**
 * Field Name: ARB_LOSS
 *
 * Field Desc:
 *
 * 0b1 Indicates the message arbitration is lost while sending a message The
 * message transmission will be retried once the CAN bus is idle again. 0b0
 * Indicates the arbitration is not lost and message transmitted successful.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_SRC_ARB_LOSS_OFFSET           (CORECAN_INTR_SRC_REG_OFFSET)
#define CORECAN_INTR_SRC_ARB_LOSS_SHIFT            (2U)
#define CORECAN_INTR_SRC_ARB_LOSS_NS_MASK          (0x1U)
#define CORECAN_INTR_SRC_ARB_LOSS_MASK             ((0x1U) << (2U))

/*******************************************************************************
 * Register: INTERRUPT_ENABLE_REGISTER
 *
 * Description: Interrupt Enable Register
 */
#define CORECAN_INTR_EN_REG_OFFSET                  (0x4U)
#define CORECAN_INTR_EN_REG_RESET_VALUE             ((uint32_t)(0x00000000UL))
#define CORECAN_INTR_EN_REG_LENGTH                  (0x4U)
#define CORECAN_INTR_EN_REG_RW_MASK                 (0x0000FFFDU)
#define CORECAN_INTR_EN_REG_RO_MASK                 (0x00000000U)
#define CORECAN_INTR_EN_REG_WO_MASK                 (0x00000000U)
#define CORECAN_INTR_EN_REG_READ_MASK               (0x0000FFFDU)
#define CORECAN_INTR_EN_REG_WRITE_MASK              (0x0000FFFDU)

/**
 * Field Name: OST_FAILURE_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when one shot transmission failure is
 * detected. 0b0 Disable interrupt generation when one shot transmission failure
 * is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_OST_FAILURE_IE_OFFSET       (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_OST_FAILURE_IE_SHIFT        (15U)
#define CORECAN_INTR_EN_OST_FAILURE_IE_NS_MASK      (0x1U)
#define CORECAN_INTR_EN_OST_FAILURE_IE_MASK         ((0x1U) << (15U))

/**
 * Field Name: STUCK_AT_0_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when stuck at dominant error is detected.
 * 0b0 Enable interrupt generation when stuck at dominant error is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_STUCK_AT_0_IE_OFFSET        (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_STUCK_AT_0_IE_SHIFT         (14U)
#define CORECAN_INTR_EN_STUCK_AT_0_IE_NS_MASK       (0x1U)
#define CORECAN_INTR_EN_STUCK_AT_0_IE_MASK          ((0x1U) << (14U))

/**
 * Field Name: RTR_MSG_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when RTR auto reply message is sent.
 * 0b0 Disable interrupt generation when RTR auto reply message is sent.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_RTR_MSG_IE_OFFSET           (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_RTR_MSG_IE_SHIFT            (13U)
#define CORECAN_INTR_EN_RTR_MSG_IE_NS_MASK          (0x1U)
#define CORECAN_INTR_EN_RTR_MSG_IE_MASK             ((0x1U) << (13U))

/**
 * Field Name: RX_MSG_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when new message is received in receive
 * buffers. 0b0 Disable interrupt generation when one new message is received in
 * receive buffers.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_RX_MSG_IE_OFFSET            (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_RX_MSG_IE_SHIFT             (12U)
#define CORECAN_INTR_EN_RX_MSG_IE_NS_MASK           (0x1U)
#define CORECAN_INTR_EN_RX_MSG_IE_MASK              ((0x1U) << (12U))

/**
 * Field Name: TX_MSG_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when message is transmitted from the transmit
 * buffers. 0b0 Disable interrupt generation when message is transmitted from
 * the transmit buffers.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_TX_MSG_IE_OFFSET            (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_TX_MSG_IE_SHIFT             (11U)
#define CORECAN_INTR_EN_TX_MSG_IE_NS_MASK           (0x1U)
#define CORECAN_INTR_EN_TX_MSG_IE_MASK              ((0x1U) << (11U))

/**
 * Field Name: RX_MSG_LOSS_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when newly received message is lost.
 * 0b0 Disable interrupt generation when newly received message is lost.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_RX_MSG_LOSS_IE_OFFSET       (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_RX_MSG_LOSS_IE_SHIFT        (10U)
#define CORECAN_INTR_EN_RX_MSG_LOSS_IE_NS_MASK      (0x1U)
#define CORECAN_INTR_EN_RX_MSG_LOSS_IE_MASK         ((0x1U) << (10U))

/**
 * Field Name: BUS_OFF_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when CANController goes into bus off state.
 * 0b0 Disable interrupt generation when CANController goes into bus off state.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_BUS_OFF_IE_OFFSET           (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_BUS_OFF_IE_SHIFT            (9U)
#define CORECAN_INTR_EN_BUS_OFF_IE_NS_MASK          (0x1U)
#define CORECAN_INTR_EN_BUS_OFF_IE_MASK             ((0x1U) << (9U))

/**
 * Field Name: CRC_ERR_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when crc error is detected.
 * 0b0 Disable interrupt generation when crc error is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_CRC_ERR_IE_OFFSET           (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_CRC_ERR_IE_SHIFT            (8U)
#define CORECAN_INTR_EN_CRC_ERR_IE_NS_MASK          (0x1U)
#define CORECAN_INTR_EN_CRC_ERR_IE_MASK             ((0x1U) << (8U))

/**
 * Field Name: FORM_ERR_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when form error is detected.
 * 0b0 Disable interrupt generation when form error is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_FORM_ERR_IE_OFFSET          (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_FORM_ERR_IE_SHIFT           (7U)
#define CORECAN_INTR_EN_FORM_ERR_IE_NS_MASK         (0x1U)
#define CORECAN_INTR_EN_FORM_ERR_IE_MASK            ((0x1U) << (7U))

/**
 * Field Name: ACK_ERR_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when acknowledgement is not received.
 * 0b0 Disable interrupt generation when acknowledgement is not received.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_ACK_ERR_IE_OFFSET           (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_ACK_ERR_IE_SHIFT            (6U)
#define CORECAN_INTR_EN_ACK_ERR_IE_NS_MASK          (0x1U)
#define CORECAN_INTR_EN_ACK_ERR_IE_MASK             ((0x1U) << (6U))

/**
 * Field Name: STUFF_ERR_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when stuff error is detected.
 * 0b0 Disable interrupt generation when stuff error is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_STUFF_ERR_IE_OFFSET         (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_STUFF_ERR_IE_SHIFT          (5U)
#define CORECAN_INTR_EN_STUFF_ERR_IE_NS_MASK        (0x1U)
#define CORECAN_INTR_EN_STUFF_ERR_IE_MASK           ((0x1U) << (5U))

/**
 * Field Name: BIT_ERR_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when bit error is detected.
 * 0b0 Disable interrupt generation when bit error is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_BIT_ERR_IE_OFFSET           (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_BIT_ERR_IE_SHIFT            (4U)
#define CORECAN_INTR_EN_BIT_ERR_IE_NS_MASK          (0x1U)
#define CORECAN_INTR_EN_BIT_ERR_IE_MASK             ((0x1U) << (4U))

/**
 * Field Name: OVR_LOAD_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when overload message is detected.
 * 0b0 Disable interrupt generation when overload message is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_OVR_LOAD_IE_OFFSET          (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_OVR_LOAD_IE_SHIFT           (3U)
#define CORECAN_INTR_EN_OVR_LOAD_IE_NS_MASK         (0x1U)
#define CORECAN_INTR_EN_OVR_LOAD_IE_MASK            ((0x1U) << (3U))

/**
 * Field Name: ARB_LOSS_IE
 *
 * Field Desc:
 *
 * 0b1 Enable interrupt generation when arbitration loss is detected.
 * 0b0 Disable interrupt generation when arbitration loss is detected.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_ARB_LOSS_IE_OFFSET          (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_ARB_LOSS_IE_SHIFT           (2U)
#define CORECAN_INTR_EN_ARB_LOSS_IE_NS_MASK         (0x1U)
#define CORECAN_INTR_EN_ARB_LOSS_IE_MASK            ((0x1U) << (2U))

/**
 * Field Name: INT_EBL
 *
 * Field Desc:
 *
 * 0b1 Enable global interrupt When this bit is set to 1 interrupt source
 * register is valid. 0b0 Disable all interrupts.
 *
 * Field Type: read-write
 */
#define CORECAN_INTR_EN_INT_EBL_OFFSET              (CORECAN_INTR_EN_REG_OFFSET)
#define CORECAN_INTR_EN_INT_EBL_SHIFT               (0U)
#define CORECAN_INTR_EN_INT_EBL_NS_MASK             (0x1U)
#define CORECAN_INTR_EN_INT_EBL_MASK                ((0x1U) << (0U))

/*******************************************************************************
 * Register: RXBUFFER_STATUS_REGISTER
 *
 * Description: Rx Message Memory Status Indicator Register
 */
#define CORECAN_RXBUFF_STS_REG_OFFSET              (0x8U)
#define CORECAN_RXBUFF_STS_REG_RESET_VALUE         ((uint32_t)(0x00000000UL))
#define CORECAN_RXBUFF_STS_REG_LENGTH              (0x4U)
#define CORECAN_RXBUFF_STS_REG_RW_MASK             (0x00000000U)
#define CORECAN_RXBUFF_STS_REG_RO_MASK             (0xFFFFFFFFU)
#define CORECAN_RXBUFF_STS_REG_WO_MASK             (0x00000000U)
#define CORECAN_RXBUFF_STS_REG_READ_MASK           (0xFFFFFFFFU)
#define CORECAN_RXBUFF_STS_REG_WRITE_MASK          (0x00000000U)

/**
 * Field Name: RXBUFF31_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 31 has received new message.
 * 0b0 Indicates receive message buffer 31 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF31_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF31_STS_SHIFT            (31U)
#define CORECAN_RXBUFF_STS_RXBUFF31_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF31_STS_MASK             ((0x1U) << (31U))

/**
 * Field Name: RXBUFF30_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 30 has received new message.
 * 0b0 Indicates receive message buffer 30 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF30_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF30_STS_SHIFT            (30U)
#define CORECAN_RXBUFF_STS_RXBUFF30_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF30_STS_MASK             ((0x1U) << (30U))

/**
 * Field Name: RXBUFF29_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 29 has received new message.
 * 0b0 Indicates receive message buffer 29 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF29_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF29_STS_SHIFT            (29U)
#define CORECAN_RXBUFF_STS_RXBUFF29_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF29_STS_MASK             ((0x1U) << (29U))

/**
 * Field Name: RXBUFF28_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 28 has received new message.
 * 0b0 Indicates receive message buffer 28 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF28_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF28_STS_SHIFT            (28U)
#define CORECAN_RXBUFF_STS_RXBUFF28_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF28_STS_MASK             ((0x1U) << (28U))

/**
 * Field Name: RXBUFF27_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 27 has received new message.
 * 0b0 Indicates receive message buffer 27 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF27_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF27_STS_SHIFT            (27U)
#define CORECAN_RXBUFF_STS_RXBUFF27_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF27_STS_MASK             ((0x1U) << (27U))

/**
 * Field Name: RXBUFF26_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 26 has received new message.
 * 0b0 Indicates receive message buffer 26 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF26_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF26_STS_SHIFT            (26U)
#define CORECAN_RXBUFF_STS_RXBUFF26_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF26_STS_MASK             ((0x1U) << (26U))

/**
 * Field Name: RXBUFF25_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 25 has received new message.
 * 0b0 Indicates receive message buffer 25 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF25_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF25_STS_SHIFT            (25U)
#define CORECAN_RXBUFF_STS_RXBUFF25_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF25_STS_MASK             ((0x1U) << (25U))

/**
 * Field Name: RXBUFF24_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 24 has received new message.
 * 0b0 Indicates receive message buffer 24 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF24_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF24_STS_SHIFT            (24U)
#define CORECAN_RXBUFF_STS_RXBUFF24_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF24_STS_MASK             ((0x1U) << (24U))

/**
 * Field Name: RXBUFF23_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 23 has received new message.
 * 0b0 Indicates receive message buffer 23 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF23_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF23_STS_SHIFT            (23U)
#define CORECAN_RXBUFF_STS_RXBUFF23_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF23_STS_MASK             ((0x1U) << (23U))

/**
 * Field Name: RXBUFF22_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 22 has received new message.
 * 0b0 Indicates receive message buffer 22 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF22_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF22_STS_SHIFT            (22U)
#define CORECAN_RXBUFF_STS_RXBUFF22_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF22_STS_MASK             ((0x1U) << (22U))

/**
 * Field Name: RXBUFF21_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 21 has received new message.
 * 0b0 Indicates receive message buffer 21 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF21_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF21_STS_SHIFT            (21U)
#define CORECAN_RXBUFF_STS_RXBUFF21_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF21_STS_MASK             ((0x1U) << (21U))

/**
 * Field Name: RXBUFF20_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 20 has received new message.
 * 0b0 Indicates receive message buffer 20 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF20_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF20_STS_SHIFT            (20U)
#define CORECAN_RXBUFF_STS_RXBUFF20_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF20_STS_MASK             ((0x1U) << (20U))

/**
 * Field Name: RXBUFF19_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 19 has received new message.
 * 0b0 Indicates receive message buffer 19 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF19_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF19_STS_SHIFT            (19U)
#define CORECAN_RXBUFF_STS_RXBUFF19_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF19_STS_MASK             ((0x1U) << (19U))

/**
 * Field Name: RXBUFF18_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 18 has received new message.
 * 0b0 Indicates receive message buffer 18 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF18_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF18_STS_SHIFT            (18U)
#define CORECAN_RXBUFF_STS_RXBUFF18_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF18_STS_MASK             ((0x1U) << (18U))

/**
 * Field Name: RXBUFF17_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 17 has received new message.
 * 0b0 Indicates receive message buffer 17 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF17_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF17_STS_SHIFT            (17U)
#define CORECAN_RXBUFF_STS_RXBUFF17_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF17_STS_MASK             ((0x1U) << (17U))

/**
 * Field Name: RXBUFF16_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 16 has received new message.
 * 0b0 Indicates receive message buffer 16 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF16_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF16_STS_SHIFT            (16U)
#define CORECAN_RXBUFF_STS_RXBUFF16_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF16_STS_MASK             ((0x1U) << (16U))

/**
 * Field Name: RXBUFF15_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 15 has received new message.
 * 0b0 Indicates receive message buffer 15 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF15_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF15_STS_SHIFT            (15U)
#define CORECAN_RXBUFF_STS_RXBUFF15_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF15_STS_MASK             ((0x1U) << (15U))

/**
 * Field Name: RXBUFF14_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 14 has received new message.
 * 0b0 Indicates receive message buffer 14 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF14_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF14_STS_SHIFT            (14U)
#define CORECAN_RXBUFF_STS_RXBUFF14_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF14_STS_MASK             ((0x1U) << (14U))

/**
 * Field Name: RXBUFF13_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 13 has received new message.
 * 0b0 Indicates receive message buffer 13 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF13_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF13_STS_SHIFT            (13U)
#define CORECAN_RXBUFF_STS_RXBUFF13_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF13_STS_MASK             ((0x1U) << (13U))

/**
 * Field Name: RXBUFF12_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 12 has received new message.
 * 0b0 Indicates receive message buffer 12 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF12_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF12_STS_SHIFT            (12U)
#define CORECAN_RXBUFF_STS_RXBUFF12_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF12_STS_MASK             ((0x1U) << (12U))

/**
 * Field Name: RXBUFF11_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 11 has received new message.
 * 0b0 Indicates receive message buffer 11 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF11_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF11_STS_SHIFT            (11U)
#define CORECAN_RXBUFF_STS_RXBUFF11_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF11_STS_MASK             ((0x1U) << (11U))

/**
 * Field Name: RXBUFF10_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 10 has received new message.
 * 0b0 Indicates receive message buffer 10 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF10_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF10_STS_SHIFT            (10U)
#define CORECAN_RXBUFF_STS_RXBUFF10_STS_NS_MASK          (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF10_STS_MASK             ((0x1U) << (10U))

/**
 * Field Name: RXBUFF9_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 9 has received new message.
 * 0b0 Indicates receive message buffer 9 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF9_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF9_STS_SHIFT             (9U)
#define CORECAN_RXBUFF_STS_RXBUFF9_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF9_STS_MASK              ((0x1U) << (9U))

/**
 * Field Name: RXBUFF8_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 8 has received new message.
 * 0b0 Indicates receive message buffer 8 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF8_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF8_STS_SHIFT             (8U)
#define CORECAN_RXBUFF_STS_RXBUFF8_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF8_STS_MASK              ((0x1U) << (8U))

/**
 * Field Name: RXBUFF7_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 7 has received new message.
 * 0b0 Indicates receive message buffer 7 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF7_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF7_STS_SHIFT             (7U)
#define CORECAN_RXBUFF_STS_RXBUFF7_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF7_STS_MASK              ((0x1U) << (7U))

/**
 * Field Name: RXBUFF6_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 6 has received new message.
 * 0b0 Indicates receive message buffer 6 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF6_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF6_STS_SHIFT             (6U)
#define CORECAN_RXBUFF_STS_RXBUFF6_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF6_STS_MASK              ((0x1U) << (6U))

/**
 * Field Name: RXBUFF5_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 5 has received new message.
 * 0b0 Indicates receive message buffer 5 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF5_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF5_STS_SHIFT             (5U)
#define CORECAN_RXBUFF_STS_RXBUFF5_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF5_STS_MASK              ((0x1U) << (5U))

/**
 * Field Name: RXBUFF4_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 4 has received new message.
 * 0b0 Indicates receive message buffer 4 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF4_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF4_STS_SHIFT             (4U)
#define CORECAN_RXBUFF_STS_RXBUFF4_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF4_STS_MASK              ((0x1U) << (4U))

/**
 * Field Name: RXBUFF3_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 3 has received new message.
 * 0b0 Indicates receive message buffer 3 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF3_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF3_STS_SHIFT             (3U)
#define CORECAN_RXBUFF_STS_RXBUFF3_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF3_STS_MASK              ((0x1U) << (3U))

/**
 * Field Name: RXBUFF2_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 2 has received new message.
 * 0b0 Indicates receive message buffer 2 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF2_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF2_STS_SHIFT             (2U)
#define CORECAN_RXBUFF_STS_RXBUFF2_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF2_STS_MASK              ((0x1U) << (2U))

/**
 * Field Name: RXBUFF1_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 1 has received new message.
 * 0b0 Indicates receive message buffer 1 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF1_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF1_STS_SHIFT             (1U)
#define CORECAN_RXBUFF_STS_RXBUFF1_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF1_STS_MASK              ((0x1U) << (1U))

/**
 * Field Name: RXBUFF0_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates receive message buffer 0 has received new message.
 * 0b0 Indicates receive message buffer 0 has not received new message.
 *
 * Field Type: read-only
 */
#define CORECAN_RXBUFF_STS_RXBUFF0_STS_OFFSET   (CORECAN_RXBUFF_STS_REG_OFFSET)
#define CORECAN_RXBUFF_STS_RXBUFF0_STS_SHIFT             (0U)
#define CORECAN_RXBUFF_STS_RXBUFF0_STS_NS_MASK           (0x1U)
#define CORECAN_RXBUFF_STS_RXBUFF0_STS_MASK              ((0x1U) << (0U))

/*******************************************************************************
 * Register: TXBUFFER_STATUS_REGISTER
 *
 * Description: Tx Message Memory Status Indicator Register
 */
#define CORECAN_TXBUFF_STS_REG_OFFSET                 (0xcU)
#define CORECAN_TXBUFF_STS_REG_RESET_VALUE            ((uint32_t)(0x00000000UL))
#define CORECAN_TXBUFF_STS_REG_LENGTH                 (0x4U)
#define CORECAN_TXBUFF_STS_REG_RW_MASK                (0x00000000U)
#define CORECAN_TXBUFF_STS_REG_RO_MASK                (0xFFFFFFFFU)
#define CORECAN_TXBUFF_STS_REG_WO_MASK                (0x00000000U)
#define CORECAN_TXBUFF_STS_REG_READ_MASK              (0xFFFFFFFFU)
#define CORECAN_TXBUFF_STS_REG_WRITE_MASK             (0x00000000U)

/**
 * Field Name: TXBUFF31_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 31 request is pending.
 * 0b0 Indicates transmit message buffer 31 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF31_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF31_STS_SHIFT            (31U)
#define CORECAN_TXBUFF_STS_TXBUFF31_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF31_STS_MASK             ((0x1U) << (31U))

/**
 * Field Name: TXBUFF30_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 30 request is pending.
 * 0b0 Indicates transmit message buffer 30 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF30_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF30_STS_SHIFT            (30U)
#define CORECAN_TXBUFF_STS_TXBUFF30_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF30_STS_MASK             ((0x1U) << (30U))

/**
 * Field Name: TXBUFF29_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 29 request is pending.
 * 0b0 Indicates transmit message buffer 29 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF29_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF29_STS_SHIFT            (29U)
#define CORECAN_TXBUFF_STS_TXBUFF29_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF29_STS_MASK             ((0x1U) << (29U))

/**
 * Field Name: TXBUFF28_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 28 request is pending.
 * 0b0 Indicates transmit message buffer 28 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF28_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF28_STS_SHIFT            (28U)
#define CORECAN_TXBUFF_STS_TXBUFF28_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF28_STS_MASK             ((0x1U) << (28U))

/**
 * Field Name: TXBUFF27_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 27 request is pending.
 * 0b0 Indicates transmit message buffer 27 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF27_STS_OFFSET           (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF27_STS_SHIFT            (27U)
#define CORECAN_TXBUFF_STS_TXBUFF27_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF27_STS_MASK             ((0x1U) << (27U))

/**
 * Field Name: TXBUFF26_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 26 request is pending.
 * 0b0 Indicates transmit message buffer 26 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF26_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF26_STS_SHIFT            (26U)
#define CORECAN_TXBUFF_STS_TXBUFF26_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF26_STS_MASK             ((0x1U) << (26U))

/**
 * Field Name: TXBUFF25_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 25 request is pending.
 * 0b0 Indicates transmit message buffer 25 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF25_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF25_STS_SHIFT            (25U)
#define CORECAN_TXBUFF_STS_TXBUFF25_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF25_STS_MASK             ((0x1U) << (25U))

/**
 * Field Name: TXBUFF24_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 24 request is pending.
 * 0b0 Indicates transmit message buffer 24 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF24_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF24_STS_SHIFT            (24U)
#define CORECAN_TXBUFF_STS_TXBUFF24_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF24_STS_MASK             ((0x1U) << (24U))

/**
 * Field Name: TXBUFF23_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 23 request is pending.
 * 0b0 Indicates transmit message buffer 23 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF23_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF23_STS_SHIFT            (23U)
#define CORECAN_TXBUFF_STS_TXBUFF23_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF23_STS_MASK             ((0x1U) << (23U))

/**
 * Field Name: TXBUFF22_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 22 request is pending.
 * 0b0 Indicates transmit message buffer 22 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF22_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF22_STS_SHIFT            (22U)
#define CORECAN_TXBUFF_STS_TXBUFF22_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF22_STS_MASK             ((0x1U) << (22U))

/**
 * Field Name: TXBUFF21_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 21 request is pending.
 * 0b0 Indicates transmit message buffer 21 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF21_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF21_STS_SHIFT            (21U)
#define CORECAN_TXBUFF_STS_TXBUFF21_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF21_STS_MASK             ((0x1U) << (21U))

/**
 * Field Name: TXBUFF20_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 20 request is pending.
 * 0b0 Indicates transmit message buffer 20 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF20_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF20_STS_SHIFT            (20U)
#define CORECAN_TXBUFF_STS_TXBUFF20_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF20_STS_MASK             ((0x1U) << (20U))

/**
 * Field Name: TXBUFF19_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 19 request is pending.
 * 0b0 Indicates transmit message buffer 19 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF19_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF19_STS_SHIFT            (19U)
#define CORECAN_TXBUFF_STS_TXBUFF19_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF19_STS_MASK             ((0x1U) << (19U))

/**
 * Field Name: TXBUFF18_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 18 request is pending.
 * 0b0 Indicates transmit message buffer 18 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF18_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF18_STS_SHIFT            (18U)
#define CORECAN_TXBUFF_STS_TXBUFF18_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF18_STS_MASK             ((0x1U) << (18U))

/**
 * Field Name: TXBUFF17_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 17 request is pending.
 * 0b0 Indicates transmit message buffer 17 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF17_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF17_STS_SHIFT            (17U)
#define CORECAN_TXBUFF_STS_TXBUFF17_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF17_STS_MASK             ((0x1U) << (17U))

/**
 * Field Name: TXBUFF16_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 16 request is pending.
 * 0b0 Indicates transmit message buffer 16 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF16_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF16_STS_SHIFT            (16U)
#define CORECAN_TXBUFF_STS_TXBUFF16_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF16_STS_MASK             ((0x1U) << (16U))

/**
 * Field Name: TXBUFF15_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 15 request is pending.
 * 0b0 Indicates transmit message buffer 15 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF15_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF15_STS_SHIFT            (15U)
#define CORECAN_TXBUFF_STS_TXBUFF15_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF15_STS_MASK             ((0x1U) << (15U))

/**
 * Field Name: TXBUFF14_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 14 request is pending.
 * 0b0 Indicates transmit message buffer 14 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF14_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF14_STS_SHIFT            (14U)
#define CORECAN_TXBUFF_STS_TXBUFF14_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF14_STS_MASK             ((0x1U) << (14U))

/**
 * Field Name: TXBUFF13_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 13 request is pending.
 * 0b0 Indicates transmit message buffer 13 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF13_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF13_STS_SHIFT            (13U)
#define CORECAN_TXBUFF_STS_TXBUFF13_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF13_STS_MASK             ((0x1U) << (13U))

/**
 * Field Name: TXBUFF12_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 12 request is pending.
 * 0b0 Indicates transmit message buffer 12 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF12_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF12_STS_SHIFT            (12U)
#define CORECAN_TXBUFF_STS_TXBUFF12_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF12_STS_MASK             ((0x1U) << (12U))

/**
 * Field Name: TXBUFF11_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 11 request is pending.
 * 0b0 Indicates transmit message buffer 11 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF11_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF11_STS_SHIFT            (11U)
#define CORECAN_TXBUFF_STS_TXBUFF11_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF11_STS_MASK             ((0x1U) << (11U))

/**
 * Field Name: TXBUFF10_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 10 request is pending.
 * 0b0 Indicates transmit message buffer 10 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF10_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF10_STS_SHIFT            (10U)
#define CORECAN_TXBUFF_STS_TXBUFF10_STS_NS_MASK          (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF10_STS_MASK             ((0x1U) << (10U))

/**
 * Field Name: TXBUFF9_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 9 request is pending.
 * 0b0 Indicates transmit message buffer 9 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF9_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF9_STS_SHIFT             (9U)
#define CORECAN_TXBUFF_STS_TXBUFF9_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF9_STS_MASK              ((0x1U) << (9U))

/**
 * Field Name: TXBUFF8_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 8 request is pending.
 * 0b0 Indicates transmit message buffer 8 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF8_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF8_STS_SHIFT             (8U)
#define CORECAN_TXBUFF_STS_TXBUFF8_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF8_STS_MASK              ((0x1U) << (8U))

/**
 * Field Name: TXBUFF7_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 7 request is pending.
 * 0b0 Indicates transmit message buffer 7 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF7_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF7_STS_SHIFT             (7U)
#define CORECAN_TXBUFF_STS_TXBUFF7_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF7_STS_MASK              ((0x1U) << (7U))

/**
 * Field Name: TXBUFF6_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 6 request is pending.
 * 0b0 Indicates transmit message buffer 6 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF6_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF6_STS_SHIFT             (6U)
#define CORECAN_TXBUFF_STS_TXBUFF6_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF6_STS_MASK              ((0x1U) << (6U))

/**
 * Field Name: TXBUFF5_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 5 request is pending.
 * 0b0 Indicates transmit message buffer 5 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF5_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF5_STS_SHIFT             (5U)
#define CORECAN_TXBUFF_STS_TXBUFF5_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF5_STS_MASK              ((0x1U) << (5U))

/**
 * Field Name: TXBUFF4_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 4 request is pending.
 * 0b0 Indicates transmit message buffer 4 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF4_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF4_STS_SHIFT             (4U)
#define CORECAN_TXBUFF_STS_TXBUFF4_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF4_STS_MASK              ((0x1U) << (4U))

/**
 * Field Name: TXBUFF3_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 3 request is pending.
 * 0b0 Indicates transmit message buffer 3 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF3_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF3_STS_SHIFT             (3U)
#define CORECAN_TXBUFF_STS_TXBUFF3_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF3_STS_MASK              ((0x1U) << (3U))

/**
 * Field Name: TXBUFF2_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 2 request is pending.
 * 0b0 Indicates transmit message buffer 2 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF2_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF2_STS_SHIFT             (2U)
#define CORECAN_TXBUFF_STS_TXBUFF2_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF2_STS_MASK              ((0x1U) << (2U))

/**
 * Field Name: TXBUFF1_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 1 request is pending.
 * 0b0 Indicates transmit message buffer 1 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF1_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF1_STS_SHIFT             (1U)
#define CORECAN_TXBUFF_STS_TXBUFF1_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF1_STS_MASK              ((0x1U) << (1U))

/**
 * Field Name: TXBUFF0_STS
 *
 * Field Desc:
 *
 * 0b1 Indicates transmit message buffer 0 request is pending.
 * 0b0 Indicates transmit message buffer 0 is ready to accept new request.
 *
 * Field Type: read-only
 */
#define CORECAN_TXBUFF_STS_TXBUFF0_STS_OFFSET   (CORECAN_TXBUFF_STS_REG_OFFSET)
#define CORECAN_TXBUFF_STS_TXBUFF0_STS_SHIFT             (0U)
#define CORECAN_TXBUFF_STS_TXBUFF0_STS_NS_MASK           (0x1U)
#define CORECAN_TXBUFF_STS_TXBUFF0_STS_MASK              ((0x1U) << (0U))

/*******************************************************************************
 * Register: ERROR_STATUS_REGISTER
 *
 * Description: CAN Error Status Register
 */
#define CORECAN_ERR_STS_REG_OFFSET                    (0x10U)
#define CORECAN_ERR_STS_REG_RESET_VALUE               ((uint32_t)(0x00000000UL))
#define CORECAN_ERR_STS_REG_LENGTH                    (0x4U)
#define CORECAN_ERR_STS_REG_RW_MASK                   (0x00000000U)
#define CORECAN_ERR_STS_REG_RO_MASK                   (0x000FFFFFU)
#define CORECAN_ERR_STS_REG_WO_MASK                   (0x00000000U)
#define CORECAN_ERR_STS_REG_READ_MASK                 (0x000FFFFFU)
#define CORECAN_ERR_STS_REG_WRITE_MASK                (0x00000000U)

/**
 * Field Name: RXGTE96
 *
 * Field Desc:
 *
 * 0b1 Indicates the receive error counter is greater or equal 96.
 * 0b0 Indicates the receive error counter is less than 96.
 *
 * Field Type: read-only
 */
#define CORECAN_ERR_STS_RXGTE96_OFFSET              (CORECAN_ERR_STS_REG_OFFSET)
#define CORECAN_ERR_STS_RXGTE96_SHIFT               (19U)
#define CORECAN_ERR_STS_RXGTE96_NS_MASK             (0x1U)
#define CORECAN_ERR_STS_RXGTE96_MASK                ((0x1U) << (19U))

/**
 * Field Name: TXGTE96
 *
 * Field Desc:
 *
 * 0b1 Indicates the transmit error counter is greater or equal 96.
 * 0b0 Indicates the transmit error counter is less than 96.
 *
 * Field Type: read-only
 */
#define CORECAN_ERR_STS_TXGTE96_OFFSET              (CORECAN_ERR_STS_REG_OFFSET)
#define CORECAN_ERR_STS_TXGTE96_SHIFT               (18U)
#define CORECAN_ERR_STS_TXGTE96_NS_MASK             (0x1U)
#define CORECAN_ERR_STS_TXGTE96_MASK                ((0x1U) << (18U))

/**
 * Field Name: ERROR_STATE
 *
 * Field Desc: The error state of the CoreCAN node
 *
 * 0b00 Indicates active error state normal operation.
 * 0b01 Indicates passive error state.
 * 0b1x Indicates bus off state.
 *
 * Field Type: read-only
 */
#define CORECAN_ERR_STS_ERR_STATE_OFFSET            (CORECAN_ERR_STS_REG_OFFSET)
#define CORECAN_ERR_STS_ERR_STATE_SHIFT             (16U)
#define CORECAN_ERR_STS_ERR_STATE_NS_MASK           (0x3U)
#define CORECAN_ERR_STS_ERR_STATE_MASK              ((0x3U) << (16U))

/**
 * Field Name: RX_ERR_CNT
 *
 * Field Desc: The receive error counter according to the CAN 2.0 specification.
 * When in bus-off state, this counter is used to count 128 groups of 11
 * recessive bits.
 *
 * Field Type: read-only
 */
#define CORECAN_ERR_STS_RX_ERR_CNT_OFFSET           (CORECAN_ERR_STS_REG_OFFSET)
#define CORECAN_ERR_STS_RX_ERR_CNT_SHIFT            (8U)
#define CORECAN_ERR_STS_RX_ERR_CNT_NS_MASK          (0xFFU)
#define CORECAN_ERR_STS_RX_ERR_CNT_MASK             ((0xFFU) << (8U))

/**
 * Field Name: TX_ERR_CNT
 *
 * Field Desc: The transmitter error counter according to the CAN 2.0 standard.
 * When it is greater than 255, it is fixed at 255.
 *
 * Field Type: read-only
 */
#define CORECAN_ERR_STS_TX_ERR_CNT_OFFSET           (CORECAN_ERR_STS_REG_OFFSET)
#define CORECAN_ERR_STS_TX_ERR_CNT_SHIFT            (0U)
#define CORECAN_ERR_STS_TX_ERR_CNT_NS_MASK          (0xFFU)
#define CORECAN_ERR_STS_TX_ERR_CNT_MASK             ((0xFFU) << (0U))

/*******************************************************************************
 * Register: COMMAND_REGISTER
 *
 * Description: CAN Command Register
 */
#define CORECAN_CMD_REG_OFFSET                        (0x14U)
#define CORECAN_CMD_REG_RESET_VALUE                   ((uint32_t)(0x00000000UL))
#define CORECAN_CMD_REG_LENGTH                        (0x4U)
#define CORECAN_CMD_REG_RW_MASK                       (0x00000007U)
#define CORECAN_CMD_REG_RO_MASK                       (0xFFFF0000U)
#define CORECAN_CMD_REG_WO_MASK                       (0x00000000U)
#define CORECAN_CMD_REG_READ_MASK                     (0xFFFF0007U)
#define CORECAN_CMD_REG_WRITE_MASK                    (0x00000007U)

/**
 * Field Name: MAJOR_VER
 *
 * Field Desc: Major version
 *
 * Field Type: read-only
 */
#define CORECAN_CMD_MAJOR_VER_OFFSET                  (CORECAN_CMD_REG_OFFSET)
#define CORECAN_CMD_MAJOR_VER_SHIFT                   (28U)
#define CORECAN_CMD_MAJOR_VER_NS_MASK                 (0xFU)
#define CORECAN_CMD_MAJOR_VER_MASK                    ((0xFU) << (28U))

/**
 * Field Name: MINOR_VER
 *
 * Field Desc: Minor version
 *
 * Field Type: read-only
 */
#define CORECAN_CMD_MINOR_VER_OFFSET                  (CORECAN_CMD_REG_OFFSET)
#define CORECAN_CMD_MINOR_VER_SHIFT                   (24U)
#define CORECAN_CMD_MINOR_VER_NS_MASK                 (0xFU)
#define CORECAN_CMD_MINOR_VER_MASK                    ((0xFU) << (24U))

/**
 * Field Name: REV_NUM
 *
 * Field Desc: Revision number
 *
 * Field Type: read-only
 */
#define CORECAN_CMD_REV_NUM_OFFSET                    (CORECAN_CMD_REG_OFFSET)
#define CORECAN_CMD_REV_NUM_SHIFT                     (16U)
#define CORECAN_CMD_REV_NUM_NS_MASK                   (0xFFU)
#define CORECAN_CMD_REV_NUM_MASK                      ((0xFFU) << (16U))

/**
 * Field Name: LPBK_MODE
 *
 * Field Desc:
 *
 * 0b0 Indicates normal operation.
 * 0b1 Indicates loopback mode is enabled.
 *
 * Field Type: read-write
 */
#define CORECAN_CMD_LPBK_MODE_OFFSET                  (CORECAN_CMD_REG_OFFSET)
#define CORECAN_CMD_LPBK_MODE_SHIFT                   (2U)
#define CORECAN_CMD_LPBK_MODE_NS_MASK                 (0x1U)
#define CORECAN_CMD_LPBK_MODE_MASK                    ((0x1U) << (2U))

/**
 * Field Name: LISTEN_MODE
 *
 * Field Desc:
 *
 * 0b0 Indicates CoreCAN is in active mode.
 * 0b1 Indicates CoreCAN is in listen only mode The output CANTX port is driven at R level.
 *
 * Field Type: read-write
 */
#define CORECAN_CMD_LISTEN_MODE_OFFSET               (CORECAN_CMD_REG_OFFSET)
#define CORECAN_CMD_LISTEN_MODE_SHIFT                (1U)
#define CORECAN_CMD_LISTEN_MODE_NS_MASK              (0x1U)
#define CORECAN_CMD_LISTEN_MODE_MASK                 ((0x1U) << (1U))

/**
 * Field Name: RUN_STOP_MODE
 *
 * Field Desc:
 *
 * 0b0 Write 0 to set the CoreCAN into stop mode Returns 0 when stopped.
 * 0b1 Write 1 to set the CoreCAN into run mode Returns 1 when running.
 *
 * Field Type: read-write
 */
#define CORECAN_CMD_RUN_STOP_MODE_OFFSET             (CORECAN_CMD_REG_OFFSET)
#define CORECAN_CMD_RUN_STOP_MODE_SHIFT              (0U)
#define CORECAN_CMD_RUN_STOP_MODE_NS_MASK            (0x1U)
#define CORECAN_CMD_RUN_STOP_MODE_MASK               ((0x1U) << (0U))

/*******************************************************************************
 * Register: CONFIGURATION_REGISTER
 *
 * Description: CAN Configuration Register
 */
#define CORECAN_CFG_REG_OFFSET                       (0x18U)
#define CORECAN_CFG_REG_RESET_VALUE                  ((uint32_t)(0x00000000UL))
#define CORECAN_CFG_REG_LENGTH                       (0x4U)
#define CORECAN_CFG_REG_RW_MASK                      (0x7FFF3FFCU)
#define CORECAN_CFG_REG_RO_MASK                      (0x00000000U)
#define CORECAN_CFG_REG_WO_MASK                      (0x00000000U)
#define CORECAN_CFG_REG_READ_MASK                    (0x7FFF3FFCU)
#define CORECAN_CFG_REG_WRITE_MASK                   (0x7FFF3FFCU)

/**
 * Field Name: CFG_BITRATE
 *
 * Field Desc: Prescaler for generating the time quantum which defines the TQ
 *
 *
 * Field Type: read-write
 */
#define CORECAN_CFG_CFG_BITRATE_OFFSET               (CORECAN_CFG_REG_OFFSET)
#define CORECAN_CFG_CFG_BITRATE_SHIFT                (16U)
#define CORECAN_CFG_CFG_BITRATE_NS_MASK              (0x7FFFU)
#define CORECAN_CFG_CFG_BITRATE_MASK                 ((0x7FFFU) << (16U))

/**
 * Field Name: BYTE_ORDER
 *
 * Field Desc: The byte position of the CAN receives and transmit data fields
 * can be modified to match the endian setting of the processor or the used CAN
 * protocol
 *
 * 0b0 Data byte position is not swapped big endian.
 * 0b1 Data byte position is swapped little endian.
 *
 * Field Type: read-write
 */
#define CORECAN_CFG_BYTE_ORDER_OFFSET                (CORECAN_CFG_REG_OFFSET)
#define CORECAN_CFG_BYTE_ORDER_SHIFT                 (13U)
#define CORECAN_CFG_BYTE_ORDER_NS_MASK               (0x1U)
#define CORECAN_CFG_BYTE_ORDER_MASK                  ((0x1U) << (13U))

/**
 * Field Name: CFG_ARBITER
 *
 * Field Desc: Transmit buffer arbiter
 *
 * 0b0 Round robin arbitration.
 * 0b1 Fixed priority arbitration.
 *
 * Field Type: read-write
 */
#define CORECAN_CFG_CFG_ARBITER_OFFSET               (CORECAN_CFG_REG_OFFSET)
#define CORECAN_CFG_CFG_ARBITER_SHIFT                (12U)
#define CORECAN_CFG_CFG_ARBITER_NS_MASK              (0x1U)
#define CORECAN_CFG_CFG_ARBITER_MASK                 ((0x1U) << (12U))

/**
 * Field Name: CFG_TSEG1
 *
 * Field Desc: Time segment 1. Length of the first-time segment
 * TSEG1=CFG_TSEG1+1 Time segment 1 includes the propagation time. CFG_TSEG1=0
 * and CFG_TSEG1=1 are not allowed.
 *
 * Field Type: read-write
 */
#define CORECAN_CFG_CFG_TSEG1_OFFSET                 (CORECAN_CFG_REG_OFFSET)
#define CORECAN_CFG_CFG_TSEG1_SHIFT                  (8U)
#define CORECAN_CFG_CFG_TSEG1_NS_MASK                (0xFU)
#define CORECAN_CFG_CFG_TSEG1_MASK                   ((0xFU) << (8U))

/**
 * Field Name: CFG_TSEG2
 *
 * Field Desc: Time segment 2. Length of the second time segment TSEG2 =
 * CFG_TSEG2 + 1 CFG_TSEG2=0 is not allowed; CFG_TSEG2=1 is only allowed in
 * direct sampling mode.
 *
 * Field Type: read-write
 */
#define CORECAN_CFG_CFG_TSEG2_OFFSET                 (CORECAN_CFG_REG_OFFSET)
#define CORECAN_CFG_CFG_TSEG2_SHIFT                  (5U)
#define CORECAN_CFG_CFG_TSEG2_NS_MASK                (0x7U)
#define CORECAN_CFG_CFG_TSEG2_MASK                   ((0x7U) << (5U))

/**
 * Field Name: AUTO_RESTART
 *
 * Field Desc:
 *
 * 0b0 After busoff the CoreCANController must be restarted This is the
 * recommended setting. 0b1 After the busoff the CoreCANController is restarting
 * automatically after 128 groups of 11 recessive bits.
 *
 * Field Type: read-write
 */
#define CORECAN_CFG_AUTO_RESTART_OFFSET              (CORECAN_CFG_REG_OFFSET)
#define CORECAN_CFG_AUTO_RESTART_SHIFT               (4U)
#define CORECAN_CFG_AUTO_RESTART_NS_MASK             (0x1U)
#define CORECAN_CFG_AUTO_RESTART_MASK                ((0x1U) << (4U))

/**
 * Field Name: CFG_SJW
 *
 * Field Desc: Synchronization jump width = CFG_SJW â€“ 1. The following
 * conditions must be satisfied  CFG_SJW<=CFG_TSEG1 and CFG_SJW <= CFG_TSEG2
 *
 * Field Type: read-write
 */
#define CORECAN_CFG_CFG_SJW_OFFSET                   (CORECAN_CFG_REG_OFFSET)
#define CORECAN_CFG_CFG_SJW_SHIFT                    (2U)
#define CORECAN_CFG_CFG_SJW_NS_MASK                  (0x3U)
#define CORECAN_CFG_CFG_SJW_MASK                     ((0x3U) << (2U))

/*******************************************************************************
 * Register: TXMESSAGE_CONTROL_REGISTER
 *
 * Description: TxMessage Buffer: Control Flags Register
 */
#define CORECAN_TXMSG_CTRL_REG_OFFSET(n)              (0x20U + (n * 0x10U))
#define CORECAN_TXMSG_CTRL_REG_RESET_VALUE            ((uint32_t)(0x00000000UL))
#define CORECAN_TXMSG_CTRL_REG_LENGTH                 (0x4U)
#define CORECAN_TXMSG_CTRL_REG_RW_MASK                (0x00000003U)
#define CORECAN_TXMSG_CTRL_REG_RO_MASK                (0x00000000U)
#define CORECAN_TXMSG_CTRL_REG_WO_MASK                (0x00BF000CU)
#define CORECAN_TXMSG_CTRL_REG_READ_MASK              (0x00000003U)
#define CORECAN_TXMSG_CTRL_REG_WRITE_MASK             (0x00BF000FU)

/**
 * Field Name: WPN
 *
 * Field Desc: WPN Write Protect Not The readback value of this bit is undefined
 * Info Using the WPN flag enables simple retransmission of the same message by
 * only having to set the TXREQ flag without taking care of the special flags
 *
 * 0b0 Bits 2116 of this register remain unchanged.
 * 0b1 Bits 2116 of this register are modified default.
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_CTRL_WPN_OFFSET(n)      (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_WPN_SHIFT                  (23U)
#define CORECAN_TXMSG_CTRL_WPN_NS_MASK                (0x1U)
#define CORECAN_TXMSG_CTRL_WPN_MASK                   ((0x1U) << (23U))

/**
 * Field Name: RTR
 *
 * Field Desc: Remote Bit
 *
 * 0b0 Standard message.
 * 0b1 RTR message.
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_CTRL_RTR_OFFSET(n)      (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_RTR_SHIFT                     (21U)
#define CORECAN_TXMSG_CTRL_RTR_NS_MASK                   (0x1U)
#define CORECAN_TXMSG_CTRL_RTR_MASK                      ((0x1U) << (21U))

/**
 * Field Name: IDE
 *
 * Field Desc: Extended Identifier Bit
 *
 * 0b0 Standard format message.
 * 0b1 Extended format message.
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_CTRL_IDE_OFFSET(n)      (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_IDE_SHIFT                     (20U)
#define CORECAN_TXMSG_CTRL_IDE_NS_MASK                   (0x1U)
#define CORECAN_TXMSG_CTRL_IDE_MASK                      ((0x1U) << (20U))

/**
 * Field Name: DLC
 *
 * Field Desc: Data Length Code Invalid values are transmitted as they are but
 * the number of data bytes is limited to eight
 *
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_CTRL_DLC_OFFSET(n)      (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_DLC_SHIFT                     (16U)
#define CORECAN_TXMSG_CTRL_DLC_NS_MASK                   (0xFU)
#define CORECAN_TXMSG_CTRL_DLC_MASK                      ((0xFU) << (16U))

/**
 * Field Name: TXINTR_WPN
 *
 * Field Desc: Transmit Interrupt Write Protect Not
 *
 * 0b0 Bit 2 of this register remains unchanged.
 * 0b1 Bit 2 of this register is modified default.
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_CTRL_TXINTR_WPN_OFFSET(n) (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_TXINTR_WPN_SHIFT              (3U)
#define CORECAN_TXMSG_CTRL_TXINTR_WPN_NS_MASK            (0x1U)
#define CORECAN_TXMSG_CTRL_TXINTR_WPN_MASK               ((0x1U) << (3U))

/**
 * Field Name: TX_INTEBL
 *
 * Field Desc: Transmit Interrupt Enable
 *
 * 0b0 Disable interrupt.
 * 0b1 Enable interrupt Successful message transmission sets the TXMSG flag in
 * the interrupt source register.
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_CTRL_TX_INTEBL_OFFSET(n)   (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_TX_INTEBL_SHIFT               (2U)
#define CORECAN_TXMSG_CTRL_TX_INTEBL_NS_MASK             (0x1U)
#define CORECAN_TXMSG_CTRL_TX_INTEBL_MASK                ((0x1U) << (2U))

/**
 * Field Name: TX_ABORT
 *
 * Field Desc: Transmit Abort Request
 *
 * 0b0 When this bit is set to 0 CoreCANController retransmits the message when
 * arbitration loss happens. 0b1 Requests removal of a pending message The
 * message is removed the next time an arbitration loss happens The flag is
 * cleared when the message is removed or when the message won arbitration The
 * TXREQ flag is released at the same time.
 *
 * Field Type: read-write
 */
#define CORECAN_TXMSG_CTRL_TX_ABORT_OFFSET(n)    (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_TX_ABORT_SHIFT                (1U)
#define CORECAN_TXMSG_CTRL_TX_ABORT_NS_MASK              (0x1U)
#define CORECAN_TXMSG_CTRL_TX_ABORT_MASK                 ((0x1U) << (1U))

/**
 * Field Name: TX_REQ
 *
 * Field Desc: Transmit Request
 *
 * 0b0 When this bit is programmed to 0 CoreCANController doesnt initiate
 * message transmissionCoreCANController set this bit 0 when transmit request is
 * served. 0b1 When this bit is programmed to 1 CoreCANController initiates
 * message transmissionCoreCANController set this bit 1 when transmit request is
 * pending.
 *
 * Field Type: read-write
 */
#define CORECAN_TXMSG_CTRL_TX_REQ_OFFSET(n)   (CORECAN_TXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_TXMSG_CTRL_TX_REQ_SHIFT                  (0U)
#define CORECAN_TXMSG_CTRL_TX_REQ_NS_MASK                (0x1U)
#define CORECAN_TXMSG_CTRL_TX_REQ_MASK                   ((0x1U) << (0U))

/*******************************************************************************
 * Register: TXMESSAGE_ID_REGISTER
 *
 * Description: TxMessage Buffer: Identifier Register
 */
#define CORECAN_TXMSG_ID_REG_OFFSET(n)                (0x24U + (n * 0x10U))
#define CORECAN_TXMSG_ID_REG_RESET_VALUE              ((uint32_t)(0x00000000UL))
#define CORECAN_TXMSG_ID_REG_LENGTH                   (0x4U)
#define CORECAN_TXMSG_ID_REG_RW_MASK                  (0x00000000U)
#define CORECAN_TXMSG_ID_REG_RO_MASK                  (0x00000000U)
#define CORECAN_TXMSG_ID_REG_WO_MASK                  (0xFFFFFFF8U)
#define CORECAN_TXMSG_ID_REG_READ_MASK                (0x00000000U)
#define CORECAN_TXMSG_ID_REG_WRITE_MASK               (0xFFFFFFF8U)

/**
 * Field Name: TX_ID
 *
 * Field Desc: Identifier MSB 11 bits [28:18] is used from MSB to LSB for the
 * standard message format and bits [28:0] is used from MSB to LSB for the
 * extended message format.
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_ID_TX_ID_OFFSET(n)        (CORECAN_TXMSG_ID_REG_OFFSET(n))
#define CORECAN_TXMSG_ID_TX_ID_SHIFT                  (3U)
#define CORECAN_TXMSG_ID_TX_ID_NS_MASK                (0x1FFFFFFFU)
#define CORECAN_TXMSG_ID_TX_ID_MASK                   ((0x1FFFFFFFU) << (3U))

/*******************************************************************************
 * Register: TXMESSAGE_WORD1_DATA_REGISTER
 *
 * Description: TxMessage Buffer: TxMessage Word1 Data Register
 */
#define CORECAN_TXMSG_WORD1_DATA_REG_OFFSET(n)        (0x28U + (n * 0x10U))
#define CORECAN_TXMSG_WORD1_DATA_REG_RESET_VALUE      ((uint32_t)(0x00000000UL))
#define CORECAN_TXMSG_WORD1_DATA_REG_LENGTH           (0x4U)
#define CORECAN_TXMSG_WORD1_DATA_REG_RW_MASK          (0x00000000U)
#define CORECAN_TXMSG_WORD1_DATA_REG_RO_MASK          (0x00000000U)
#define CORECAN_TXMSG_WORD1_DATA_REG_WO_MASK          (0xFFFFFFFFU)
#define CORECAN_TXMSG_WORD1_DATA_REG_READ_MASK        (0x00000000U)
#define CORECAN_TXMSG_WORD1_DATA_REG_WRITE_MASK       (0xFFFFFFFFU)

/**
 * Field Name: WORD1_TXDATA
 *
 * Field Desc: The byte mapping can be set using the BYTEORDER configuration bit
 * When BYTEORDER 0
 *
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_WORD1_DATA_WORD1_TXDATA_OFFSET(n)  \
                                       (CORECAN_TXMSG_WORD1_DATA_REG_OFFSET(n))
#define CORECAN_TXMSG_WORD1_DATA_WORD1_TXDATA_SHIFT      (0U)
#define CORECAN_TXMSG_WORD1_DATA_WORD1_TXDATA_NS_MASK    (0xFFFFFFFFU)
#define CORECAN_TXMSG_WORD1_DATA_WORD1_TXDATA_MASK       ((0xFFFFFFFFU) << (0U))

/*******************************************************************************
 * Register: TXMESSAGE_WORD0_DATA_REGISTER
 *
 * Description: TxMessage Buffer: TxMessage Word0 Data Register
 */
#define CORECAN_TXMSG_WORD0_DATA_REG_OFFSET(n)        (0x2cU + (n * 0x10U))
#define CORECAN_TXMSG_WORD0_DATA_REG_RESET_VALUE      ((uint32_t)(0x00000000UL))
#define CORECAN_TXMSG_WORD0_DATA_REG_LENGTH           (0x4U)
#define CORECAN_TXMSG_WORD0_DATA_REG_RW_MASK          (0x00000000U)
#define CORECAN_TXMSG_WORD0_DATA_REG_RO_MASK          (0x00000000U)
#define CORECAN_TXMSG_WORD0_DATA_REG_WO_MASK          (0xFFFFFFFFU)
#define CORECAN_TXMSG_WORD0_DATA_REG_READ_MASK        (0x00000000U)
#define CORECAN_TXMSG_WORD0_DATA_REG_WRITE_MASK       (0xFFFFFFFFU)

/**
 * Field Name: WORD0_TXDATA
 *
 * Field Desc: The byte mapping can be set using the BYTEORDER configuration bit
 * When BYTEORDER 0
 *
 *
 * Field Type: write-only
 */
#define CORECAN_TXMSG_WORD0_DATA_WORD0_TXDATA_OFFSET(n)  \
                                      (CORECAN_TXMSG_WORD0_DATA_REG_OFFSET(n))
#define CORECAN_TXMSG_WORD0_DATA_WORD0_TXDATA_SHIFT      (0U)
#define CORECAN_TXMSG_WORD0_DATA_WORD0_TXDATA_NS_MASK    (0xFFFFFFFFU)
#define CORECAN_TXMSG_WORD0_DATA_WORD0_TXDATA_MASK       ((0xFFFFFFFFU) << (0U))

/*******************************************************************************
 * Register: RXMESSAGE_CONTROL_REGISTER
 *
 * Description: RxMessage Buffer:: Congrol Flags Register
 */
#define CORECAN_RXMSG_CTRL_REG_OFFSET(n)              (0x220U + (n * 0x20U))
#define CORECAN_RXMSG_CTRL_REG_RESET_VALUE            ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_CTRL_REG_LENGTH                 (0x4U)
#define CORECAN_RXMSG_CTRL_REG_RW_MASK                (0x003F007DU)
#define CORECAN_RXMSG_CTRL_REG_RO_MASK                (0x00000002U)
#define CORECAN_RXMSG_CTRL_REG_WO_MASK                (0x00020000U)
#define CORECAN_RXMSG_CTRL_REG_READ_MASK              (0x003F007FU)
#define CORECAN_RXMSG_CTRL_REG_WRITE_MASK             (0x003F007DU)

/**
 * Field Name: RTR
 *
 * Field Desc: Remote Bit
 *
 * 0b0 Indicates regular message is received.
 * 0b1 Indicates RTR message is received.
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_RTR_OFFSET(n)      (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_RTR_SHIFT          (21U)
#define CORECAN_RXMSG_CTRL_RTR_NS_MASK        (0x1U)
#define CORECAN_RXMSG_CTRL_RTR_MASK           ((0x1U) << (21U))

/**
 * Field Name: IDE
 *
 * Field Desc: Extended Identifier Bit
 *
 * 0b0 Indicates standard format message is received.
 * 0b1 Indicates extended format message is received.
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_IDE_OFFSET(n)      (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_IDE_SHIFT          (20U)
#define CORECAN_RXMSG_CTRL_IDE_NS_MASK        (0x1U)
#define CORECAN_RXMSG_CTRL_IDE_MASK           ((0x1U) << (20U))

/**
 * Field Name: DLC
 *
 * Field Desc: Data Length Code
 *
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_DLC_OFFSET(n)      (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_DLC_SHIFT          (16U)
#define CORECAN_RXMSG_CTRL_DLC_NS_MASK        (0xFU)
#define CORECAN_RXMSG_CTRL_DLC_MASK           ((0xFU) << (16U))

/**
 * Field Name: WPN
 *
 * Field Desc: Write Protect Not Low The readback value of this bit is undefined
 *
 * 0b0 Bit 63 of this register remain unchanged.
 * 0b1 Bit 63 of this register are modified default.
 *
 * Field Type: write-only
 */
#define CORECAN_RXMSG_CTRL_WPN_OFFSET(n)      (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_WPN_SHIFT          (7U)
#define CORECAN_RXMSG_CTRL_WPN_NS_MASK        (0x1U)
#define CORECAN_RXMSG_CTRL_WPN_MASK           ((0x1U) << (7U))

/**
 * Field Name: LINK_FLAG
 *
 * Field Desc: Link Flag
 *
 * 0b0 This buffer is not linked to the next buffer.
 * 0b1 This buffer is linked with next buffer.
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_LINK_FLAG_OFFSET(n)  (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_LINK_FLAG_SHIFT      (6U)
#define CORECAN_RXMSG_CTRL_LINK_FLAG_NS_MASK    (0x1U)
#define CORECAN_RXMSG_CTRL_LINK_FLAG_MASK       ((0x1U) << (6U))

/**
 * Field Name: RX_INTEBL
 *
 * Field Desc: Receive Interrupt Enable
 *
 * 0b0 Disable interrupt.
 * 0b1 Enable interrupt Successful message reception sets the RXMSG flag in the
 * interrupt source
 * register.
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_RX_INTEBL_OFFSET(n)  (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_RX_INTEBL_SHIFT      (5U)
#define CORECAN_RXMSG_CTRL_RX_INTEBL_NS_MASK    (0x1U)
#define CORECAN_RXMSG_CTRL_RX_INTEBL_MASK       ((0x1U) << (5U))

/**
 * Field Name: RTR_REPLY
 *
 * Field Desc: Automatic message reply upon receipt of an RTR message
 *
 * 0b0 Disable automatic RTR message handling.
 * 0b1 Enable automatic RTR message handling.
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_RTR_REPLY_OFFSET(n)  (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_RTR_REPLY_SHIFT      (4U)
#define CORECAN_RXMSG_CTRL_RTR_REPLY_NS_MASK    (0x1U)
#define CORECAN_RXMSG_CTRL_RTR_REPLY_MASK       ((0x1U) << (4U))

/**
 * Field Name: BUFFER_ENABLE
 *
 * Field Desc: Receive Buffern Enable
 *
 * 0b0 Receive buffer0 is disabled.
 * 0b1 Receive buffer0 is enabled.
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_BUFFER_ENABLE_OFFSET(n) \
                                             (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_BUFFER_ENABLE_SHIFT           (3U)
#define CORECAN_RXMSG_CTRL_BUFFER_ENABLE_NS_MASK         (0x1U)
#define CORECAN_RXMSG_CTRL_BUFFER_ENABLE_MASK            ((0x1U) << (3U))

/**
 * Field Name: RTRABORT
 *
 * Field Desc: RTR Abort Request
 *
 * 0b0 When this bit is set to 0 CoreCAN retransmits the RTR message when
 * arbitration loss happens.
 * 0b1 Requests removal of a pending RTR message reply The message is removed
 * the next time an arbitration loss happens The flag is cleared when the
 * message is removed or when the message won arbitration The TXREQ flag is
 * released at the same time.
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_RTRABORT_OFFSET(n) (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_RTRABORT_SHIFT                (2U)
#define CORECAN_RXMSG_CTRL_RTRABORT_NS_MASK              (0x1U)
#define CORECAN_RXMSG_CTRL_RTRABORT_MASK                 ((0x1U) << (2U))

/**
 * Field Name: RTREPLY_PENDING
 *
 * Field Desc: RTReplypending
 *
 * 0b0 When RTRREPLY is set to 1 this bit indicates that after receiving the RTR
 * message RTR reply request has been sent. 0b1 When RTRREPLY is set to 1 this
 * bit indicates that after receiving the RTR message RTR reply request is
 * pending.
 *
 * Field Type: read-only
 */
#define CORECAN_RXMSG_CTRL_RTREPLY_PENDING_OFFSET(n) \
                                             (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_RTREPLY_PENDING_SHIFT         (1U)
#define CORECAN_RXMSG_CTRL_RTREPLY_PENDING_NS_MASK       (0x1U)
#define CORECAN_RXMSG_CTRL_RTREPLY_PENDING_MASK          ((0x1U) << (1U))

/**
 * Field Name: MSGAV
 *
 * Field Desc: Message Available  When RTR_REPLY is set to 0, this bit is set
 * to 1 by CoreCAN when buffer contains a valid message.   When RTR_REPLY is set
 * to 1, this bit is set to 1 by the CoreCAN when RTR reply request is sent.
 * Writing 1, clears this bit
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_CTRL_MSGAV_OFFSET(n)    (CORECAN_RXMSG_CTRL_REG_OFFSET(n))
#define CORECAN_RXMSG_CTRL_MSGAV_SHIFT                   (0U)
#define CORECAN_RXMSG_CTRL_MSGAV_NS_MASK                 (0x1U)
#define CORECAN_RXMSG_CTRL_MSGAV_MASK                    ((0x1U) << (0U))

/*******************************************************************************
 * Register: RXMESSAGE_ID_REGISTER
 *
 * Description: RxMessage Buffer:. RxMessage Identifier Register
 */
#define CORECAN_RXMSG_ID_REG_OFFSET(n)                (0x224U + (n * 0x20U))
#define CORECAN_RXMSG_ID_REG_RESET_VALUE              ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_ID_REG_LENGTH                   (0x4U)
#define CORECAN_RXMSG_ID_REG_RW_MASK                  (0xFFFFFFF8U)
#define CORECAN_RXMSG_ID_REG_RO_MASK                  (0x00000000U)
#define CORECAN_RXMSG_ID_REG_WO_MASK                  (0x00000000U)
#define CORECAN_RXMSG_ID_REG_READ_MASK                (0xFFFFFFF8U)
#define CORECAN_RXMSG_ID_REG_WRITE_MASK               (0xFFFFFFF8U)

/**
 * Field Name: RX_ID
 *
 * Field Desc: Identifier ID [28:0]  For the standard message format, only MSB
 * 11 bits [28:18] of identifier are valid. For the extended message format, all
 * 29 bits of the identifier are valid
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_ID_RX_ID_OFFSET(n)      (CORECAN_RXMSG_ID_REG_OFFSET(n))
#define CORECAN_RXMSG_ID_RX_ID_SHIFT                     (3U)
#define CORECAN_RXMSG_ID_RX_ID_NS_MASK                   (0x1FFFFFFFU)
#define CORECAN_RXMSG_ID_RX_ID_MASK                      ((0x1FFFFFFFU) << (3U))

/*******************************************************************************
 * Register: RXMESSAGE_WORD1_DATA_REGISTER
 *
 * Description: RxMessage Buffer:. RxMessage Word1 Data Register
 */
#define CORECAN_RXMSG_WORD1_DATA_REG_OFFSET(n)        (0x228U + (n * 0x20U))
#define CORECAN_RXMSG_WORD1_DATA_REG_RESET_VALUE      ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_WORD1_DATA_REG_LENGTH           (0x4U)
#define CORECAN_RXMSG_WORD1_DATA_REG_RW_MASK          (0xFFFFFFFFU)
#define CORECAN_RXMSG_WORD1_DATA_REG_RO_MASK          (0x00000000U)
#define CORECAN_RXMSG_WORD1_DATA_REG_WO_MASK          (0x00000000U)
#define CORECAN_RXMSG_WORD1_DATA_REG_READ_MASK        (0xFFFFFFFFU)
#define CORECAN_RXMSG_WORD1_DATA_REG_WRITE_MASK       (0xFFFFFFFFU)

/**
 * Field Name: WORD1_RXDATA
 *
 * Field Desc: The byte mapping can be set using the BYTEORDER configuration bit
 * When BYTEORDER 0
 * the byte mapping will be as shown in following table
 *
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_WORD1_DATA_WORD1_RXDATA_OFFSET(n) \
                                       (CORECAN_RXMSG_WORD1_DATA_REG_OFFSET(n))
#define CORECAN_RXMSG_WORD1_DATA_WORD1_RXDATA_SHIFT      (0U)
#define CORECAN_RXMSG_WORD1_DATA_WORD1_RXDATA_NS_MASK    (0xFFFFFFFFU)
#define CORECAN_RXMSG_WORD1_DATA_WORD1_RXDATA_MASK       ((0xFFFFFFFFU) << (0U))

/*******************************************************************************
 * Register: RXMESSAGE_WORD0_DATA_REGISTER
 *
 * Description: RxMessage Buffer: RxMessage Word0  Data Register
 */
#define CORECAN_RXMSG_WORD0_DATA_REG_OFFSET(n)        (0x22cU + (n * 0x20U))
#define CORECAN_RXMSG_WORD0_DATA_REG_RESET_VALUE      ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_WORD0_DATA_REG_LENGTH           (0x4U)
#define CORECAN_RXMSG_WORD0_DATA_REG_RW_MASK          (0xFFFFFFFFU)
#define CORECAN_RXMSG_WORD0_DATA_REG_RO_MASK          (0x00000000U)
#define CORECAN_RXMSG_WORD0_DATA_REG_WO_MASK          (0x00000000U)
#define CORECAN_RXMSG_WORD0_DATA_REG_READ_MASK        (0xFFFFFFFFU)
#define CORECAN_RXMSG_WORD0_DATA_REG_WRITE_MASK       (0xFFFFFFFFU)

/**
 * Field Name: WORD0_RXDATA
 *
 * Field Desc: The byte mapping can be set using the BYTEORDER configuration bit
 * When BYTEORDER 0 the byte mapping will be as shown in following table
 *
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_WORD0_DATA_WORD0_RXDATA_OFFSET(n) \
                                       (CORECAN_RXMSG_WORD0_DATA_REG_OFFSET(n))
#define CORECAN_RXMSG_WORD0_DATA_WORD0_RXDATA_SHIFT      (0U)
#define CORECAN_RXMSG_WORD0_DATA_WORD0_RXDATA_NS_MASK    (0xFFFFFFFFU)
#define CORECAN_RXMSG_WORD0_DATA_WORD0_RXDATA_MASK       ((0xFFFFFFFFU) << (0U))

/*******************************************************************************
 * Register: RXMESSAGE_AMR_REGISTER
 *
 * Description: RxMessage Buffer: RxMessage Acceptance Mask Register
 */
#define CORECAN_RXMSG_AMR_REG_OFFSET(n)               (0x230U + (n * 0x20U))
#define CORECAN_RXMSG_AMR_REG_RESET_VALUE             ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_AMR_REG_LENGTH                  (0x4U)
#define CORECAN_RXMSG_AMR_REG_RW_MASK                 (0xFFFFFFFEU)
#define CORECAN_RXMSG_AMR_REG_RO_MASK                 (0x00000000U)
#define CORECAN_RXMSG_AMR_REG_WO_MASK                 (0x00000000U)
#define CORECAN_RXMSG_AMR_REG_READ_MASK               (0xFFFFFFFEU)
#define CORECAN_RXMSG_AMR_REG_WRITE_MASK              (0xFFFFFFFEU)

/**
 * Field Name: ACPT_MASK_ID
 *
 * Field Desc: Acceptance mask for identifier
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_AMR_ACPT_MASK_ID_OFFSET(n) \
                                           (CORECAN_RXMSG_AMR_REG_OFFSET(n))
#define CORECAN_RXMSG_AMR_ACPT_MASK_ID_SHIFT          (3U)
#define CORECAN_RXMSG_AMR_ACPT_MASK_ID_NS_MASK        (0x1FFFFFFFU)
#define CORECAN_RXMSG_AMR_ACPT_MASK_ID_MASK           ((0x1FFFFFFFU) << (3U))

/**
 * Field Name: ACPT_MASK_IDE
 *
 * Field Desc: Acceptance mask for IDE
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_AMR_ACPT_MASK_IDE_OFFSET(n) \
                                              (CORECAN_RXMSG_AMR_REG_OFFSET(n))
#define CORECAN_RXMSG_AMR_ACPT_MASK_IDE_SHIFT         (2U)
#define CORECAN_RXMSG_AMR_ACPT_MASK_IDE_NS_MASK       (0x1U)
#define CORECAN_RXMSG_AMR_ACPT_MASK_IDE_MASK          ((0x1U) << (2U))

/**
 * Field Name: ACPT_MASK_RTR
 *
 * Field Desc: Acceptance mask for RTR
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_AMR_ACPT_MASK_RTR_OFFSET(n) \
										(CORECAN_RXMSG_AMR_REG_OFFSET(n))
#define CORECAN_RXMSG_AMR_ACPT_MASK_RTR_SHIFT            (1U)
#define CORECAN_RXMSG_AMR_ACPT_MASK_RTR_NS_MASK          (0x1U)
#define CORECAN_RXMSG_AMR_ACPT_MASK_RTR_MASK             ((0x1U) << (1U))

/*******************************************************************************
 * Register: RXMESSAGE_ACR_REGISTER
 *
 * Description: RxMessage Buffer: RxMessage Acceptance Code Register
 */
#define CORECAN_RXMSG_ACR_REG_OFFSET(n)               (0x234U + (n * 0x20U))
#define CORECAN_RXMSG_ACR_REG_RESET_VALUE             ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_ACR_REG_LENGTH                  (0x4U)
#define CORECAN_RXMSG_ACR_REG_RW_MASK                 (0xFFFFFFFEU)
#define CORECAN_RXMSG_ACR_REG_RO_MASK                 (0x00000000U)
#define CORECAN_RXMSG_ACR_REG_WO_MASK                 (0x00000000U)
#define CORECAN_RXMSG_ACR_REG_READ_MASK               (0xFFFFFFFEU)
#define CORECAN_RXMSG_ACR_REG_WRITE_MASK              (0xFFFFFFFEU)

/**
 * Field Name: ACPT_CODE_ID
 *
 * Field Desc: Acceptance code for identifier
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_ACR_ACPT_CODE_ID_OFFSET(n) \
                                             (CORECAN_RXMSG_ACR_REG_OFFSET(n))
#define CORECAN_RXMSG_ACR_ACPT_CODE_ID_SHIFT             (3U)
#define CORECAN_RXMSG_ACR_ACPT_CODE_ID_NS_MASK           (0x1FFFFFFFU)
#define CORECAN_RXMSG_ACR_ACPT_CODE_ID_MASK              ((0x1FFFFFFFU) << (3U))

/**
 * Field Name: ACPT_CODE_IDE
 *
 * Field Desc: Acceptance code for IDE
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_ACR_ACPT_CODE_IDE_OFFSET(n) \
                                              (CORECAN_RXMSG_ACR_REG_OFFSET(n))
#define CORECAN_RXMSG_ACR_ACPT_CODE_IDE_SHIFT            (2U)
#define CORECAN_RXMSG_ACR_ACPT_CODE_IDE_NS_MASK          (0x1U)
#define CORECAN_RXMSG_ACR_ACPT_CODE_IDE_MASK             ((0x1U) << (2U))

/**
 * Field Name: ACPT_CODE_RTR
 *
 * Field Desc: Acceptance code for RTR
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_ACR_ACPT_CODE_RTR_OFFSET(n) \
                                             (CORECAN_RXMSG_ACR_REG_OFFSET(n))
#define CORECAN_RXMSG_ACR_ACPT_CODE_RTR_SHIFT            (1U)
#define CORECAN_RXMSG_ACR_ACPT_CODE_RTR_NS_MASK          (0x1U)
#define CORECAN_RXMSG_ACR_ACPT_CODE_RTR_MASK             ((0x1U) << (1U))

/*******************************************************************************
 * Register: RXMESSAGE_AMR_DATA_REGISTER
 *
 * Description: RxMessage Buffer:. RxMessage Acceptance Mask Data Register
 */
#define CORECAN_RXMSG_AMR_DATA_REG_OFFSET(n)          (0x238U + (n * 0x20U))
#define CORECAN_RXMSG_AMR_DATA_REG_RESET_VALUE        ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_AMR_DATA_REG_LENGTH             (0x4U)
#define CORECAN_RXMSG_AMR_DATA_REG_RW_MASK            (0x0000FFFFU)
#define CORECAN_RXMSG_AMR_DATA_REG_RO_MASK            (0x00000000U)
#define CORECAN_RXMSG_AMR_DATA_REG_WO_MASK            (0x00000000U)
#define CORECAN_RXMSG_AMR_DATA_REG_READ_MASK          (0x0000FFFFU)
#define CORECAN_RXMSG_AMR_DATA_REG_WRITE_MASK         (0x0000FFFFU)

/**
 * Field Name: ACPT_MASK_BYTE1
 *
 * Field Desc: Acceptance mask for data byte 1
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE1_OFFSET(n) \
                                        (CORECAN_RXMSG_AMR_DATA_REG_OFFSET(n))
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE1_SHIFT     (8U)
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE1_NS_MASK   (0xFFU)
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE1_MASK      ((0xFFU) << (8U))

/**
 * Field Name: ACPT_MASK_BYTE2
 *
 * Field Desc: Acceptance mask for data byte 2
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE2_OFFSET(n) \
                                         (CORECAN_RXMSG_AMR_DATA_REG_OFFSET(n))
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE2_SHIFT     (0U)
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE2_NS_MASK   (0xFFU)
#define CORECAN_RXMSG_AMR_DATA_ACPT_MASK_BYTE2_MASK      ((0xFFU) << (0U))

/*******************************************************************************
 * Register: RXMESSAGE_ACR_DATA_REGISTER
 *
 * Description: RxMessage Buffer:. RxMessage Acceptance Code Data Register
 */
#define CORECAN_RXMSG_ACR_DATA_REG_OFFSET(n)          (0x23cU + (n * 0x20U))
#define CORECAN_RXMSG_ACR_DATA_REG_RESET_VALUE        ((uint32_t)(0x00000000UL))
#define CORECAN_RXMSG_ACR_DATA_REG_LENGTH             (0x4U)
#define CORECAN_RXMSG_ACR_DATA_REG_RW_MASK            (0x0000FFFFU)
#define CORECAN_RXMSG_ACR_DATA_REG_RO_MASK            (0x00000000U)
#define CORECAN_RXMSG_ACR_DATA_REG_WO_MASK            (0x00000000U)
#define CORECAN_RXMSG_ACR_DATA_REG_READ_MASK          (0x0000FFFFU)
#define CORECAN_RXMSG_ACR_DATA_REG_WRITE_MASK         (0x0000FFFFU)

/**
 * Field Name: ACPT_CODE_BYTE1
 *
 * Field Desc: Acceptance mask for data byte 1
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE1_OFFSET(n) \
                                         (CORECAN_RXMSG_ACR_DATA_REG_OFFSET(n))
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE1_SHIFT     (8U)
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE1_NS_MASK   (0xFFU)
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE1_MASK      ((0xFFU) << (8U))

/**
 * Field Name: ACPT_CODE_BYTE2
 *
 * Field Desc: Acceptance mask for data byte 2
 *
 * Field Type: read-write
 */
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE2_OFFSET(n) \
                                         (CORECAN_RXMSG_ACR_DATA_REG_OFFSET(n))
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE2_SHIFT     (0U)
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE2_NS_MASK   (0xFFU)
#define CORECAN_RXMSG_ACR_DATA_ACPT_CODE_BYTE2_MASK      ((0xFFU) << (0U))

#ifdef __cplusplus
}
#endif

#endif /* CORECAN_REG_MAP_REGS_H_ */
