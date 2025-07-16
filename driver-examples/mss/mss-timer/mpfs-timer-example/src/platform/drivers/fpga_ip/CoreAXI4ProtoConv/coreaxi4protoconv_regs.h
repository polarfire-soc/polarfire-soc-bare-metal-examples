/*******************************************************************************
 * (c) Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 * 
 * SPDX-License-Identifier: MIT
 *
 * @file coreaxi4protoconv_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreAXI4ProtoConv register definitions
 *
 */

#ifndef __CORE_AXI4PC_REGISTERS_H
#define __CORE_AXI4PC_REGISTERS_H    1

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Macro definitions
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_REG_OFFSET => Register offset from base
 * address.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_REG_LENGTH => Register length in number of
 * bytes.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_RW_MASK => This identifies what read/write
 * bits are used within a register.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_RO_MASK => This identifies what read only
 * bits are used within a register.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_WO_MASK => This identifies what write only
 * bits are used within a register.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_READ_MASK => This identifies what read bits
 * are used within a register.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_WRITE_MASK => This identifies what write
 * bits are used within a register.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_<FIELD_NAME>_OFFSET => This describes the
 * register offset from the base address for a specific field.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_<FIELD_NAME>_SHIFT => Bit field shift.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_<FIELD_NAME>_NS_MASK => This describes the
 * field mask without the shift included. This mask is based on the width of the
 * bit field.
 *
 * COREAXI4PROTOCONV_REGS_<REG_NAME>_<FIELD_NAME>_MASK => This describes the
 * field mask with the shift included.
 */

/*******************************************************************************
 * Register: S2MM_VERSN
 *
 * Description: Version register. It will provide Major version and Minor
 * version of the core.
 */
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_OFFSET     (0x0U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_RW_MASK    (0xFFFF0000U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_READ_MASK  (0xFFFF0000U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_WRITE_MASK (0xFFFF0000U)

/**
 * Field Name: MINOR
 *
 * Field Desc: Minor version of the core
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MINOR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MINOR_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MINOR_NS_MASK ((uint32_t)(0x0003FFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MINOR_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_VERSN_MINOR_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_VERSN_MINOR_SHIFT)

/**
 * Field Name: MAJOR
 *
 * Field Desc: Major version of the core
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MAJOR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_VERSN_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MAJOR_SHIFT   (24U)
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MAJOR_NS_MASK ((uint32_t)(0x0003FFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_VERSN_MAJOR_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_VERSN_MAJOR_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_VERSN_MAJOR_SHIFT)

/*******************************************************************************
 * Register: S2MM_CTRL
 *
 * Description: S2MM control register.This register must be written after
 * writing S2MM length, S2MM address0 and S2MM addres1 (if GUI parameter S2MM
 * Address Width is configured to more than 32) registers.When GUI parameter
 * S2MM Command/Status FIFO Enable is not enabled, this register must not be
 * written until the current transfer is completed. User can poll Done bit of
 * S2MM status register to know the status of current transfer
 */
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_OFFSET     (0x10U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_RW_MASK    (0x03FF0007U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_READ_MASK  (0x03FF0007U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_WRITE_MASK (0x03FF0007U)

/**
 * Field Name: START
 *
 * Field Desc: Writing 1 to this bit initiates S2MM data transferWhen GUI
 * parameter S2MM CommandStatus FIFO Enable is enabled writing 1 to this bit
 * stores the descriptor into command FIFO Descriptors consist of S2MM length
 * S2MM address0 S2MM address1 and S2MM control register  bits 16 to 25
 * information S2MM Start bit is a self clearing bit.
 *
 * 0b0 There is no impact on S2MM data transfer.
 * 0b1 Initiates S2MM data transfer.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_START_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_START_SHIFT    (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_START_NS_MASK  (0x1U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_START_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_CTRL_START_NS_MASK  \
                         << COREAXI4PROTOCONV_REGS_S2MM_CTRL_START_SHIFT)

/**
 * Field Name: BURTYP
 *
 * Field Desc: AXI4Memory Mapped interface burst type  If the AXI4 length is 1
 * FIXED and INCR bursts are equivalent Fixed burst transfer should be used to
 * access peripheral device which supports fixed address like FIFO  Increment
 * burst transfer should be used to access peripheral device which supports
 * incremental address like SRAM.
 *
 * 2b00 AXI4Memory Mapped fixed burst.
 * 2b01 AXI4Memory Mapped increment burst.
 * 2b10 Reserved.
 * 2b11 Reserved.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_BURTYP_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_BURTYP_SHIFT   (1U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_BURTYP_NS_MASK ((uint32_t)(0x00000003UL))
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_BURTYP_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_CTRL_BURTYP_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_CTRL_BURTYP_SHIFT)

/**
 * Field Name: CMDID
 *
 * Field Desc: User can assign an arbitrary value to this field. On completion
 * of the S2MM transfer, IP core inserts this field into the S2MM status
 * register. IP core do not support out of order operation. Command issued in
 * the order must be reflected in the same order in the status
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_CMDID_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_CTRL_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_CMDID_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_CMDID_NS_MASK ((uint32_t)(0x000003FFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_CTRL_CMDID_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_CTRL_CMDID_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_CTRL_CMDID_SHIFT)

/*******************************************************************************
 * Register: S2MM_STS
 *
 * Description: This register contains status of the S2MM data transfers.
 */
#define COREAXI4PROTOCONV_REGS_S2MM_STS_REG_OFFSET     (0x14U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_REG_RW_MASK    (0x03FF003DU)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_REG_READ_MASK  (0x03FF003DU)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_REG_WRITE_MASK (0x03FF003DU)

/**
 * Field Name: DONE
 *
 * Field Desc: Done
 *
 * 0b0 Indicates S2MM transfer is not completed.
 * 0b1 Indicates S2MM transfer is completed.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_STS_DONE_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_DONE_SHIFT     (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_DONE_NS_MASK   ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_STS_DONE_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_STS_DONE_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_STS_DONE_SHIFT)

/**
 * Field Name: AXI4WRSP
 *
 * Field Desc: AXI4 Memory mapped write response Indicates AXI4Memory Mapped
 * interface write response
 *
 * 2b00 OK.
 * 2b01 EXOKAY.
 * 2b10 SLVERR.
 * 2b11 DECERR.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_STS_AXI4WRSP_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_AXI4WRSP_SHIFT   (2U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_AXI4WRSP_NS_MASK \
                ((uint32_t)(0x00000003UL))
#define COREAXI4PROTOCONV_REGS_S2MM_STS_AXI4WRSP_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_STS_AXI4WRSP_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_STS_AXI4WRSP_SHIFT)

/**
 * Field Name: PKTDROP_OVF
 *
 * Field Desc: Packet drop overflow
 *
 * 0b0 Indicates S2MM packets not dropped due to overflow.
 * 0b1 Indicates S2MM packets dropped due to overflow.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_OVF_OFFSET \
    (COREAXI4PROTOCONV_REGS_S2MM_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_OVF_SHIFT   (4U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_OVF_NS_MASK \
                      ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_OVF_MASK \
    (COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_OVF_NS_MASK \
     << COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_OVF_SHIFT)

/**
 * Field Name: PKTDROP_ERR
 *
 * Field Desc: Packet drop error
 *
 * 0b0 Indicates S2MM packets not dropped due to error.
 * 0b1 Indicates S2MM packets dropped due to error.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_ERR_OFFSET \
    (COREAXI4PROTOCONV_REGS_S2MM_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_ERR_SHIFT   (5U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_ERR_NS_MASK \
                       ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_ERR_MASK \
    (COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_ERR_NS_MASK \
     << COREAXI4PROTOCONV_REGS_S2MM_STS_PKTDROP_ERR_SHIFT)

/**
 * Field Name: CMDID
 *
 * Field Desc: Command ID.Provides command ID for which S2MM transfer is
 * completed.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_STS_CMDID_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_CMDID_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_S2MM_STS_CMDID_NS_MASK ((uint32_t)(0x000003FFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_STS_CMDID_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_STS_CMDID_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_STS_CMDID_SHIFT)

/*******************************************************************************
 * Register: S2MM_LEN
 *
 * Description: This register contains S2MM length
 */
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_OFFSET       (0x18U)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_LENGTH       (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_RW_MASK      (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_RO_MASK      (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_WO_MASK      (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_READ_MASK    (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_WRITE_MASK   (0xFFFFFFFFU)

/**
 * Field Name: S2MM_LEN
 *
 * Field Desc: Indicates the total number of bytes to be transmitted from the
 * AXI4-Stream interface to the AXI4-Memory Mapped interface.  Transfers can be
 * from 1 to 232 bytes. You must not program 0 value to this register. This
 * register must be programmed with byte aligned value of S2MM Data Width (in
 * bits).  For example, if S2MM Data Width (in bits) is configured to 128, then
 * this register must be programmed in multiple of (128/8) 16.  When S2MM
 * Command/Status FIFO Enable is not enabled, this register must not be written
 * until the current transfer is completed. You can poll the S2MM Done bit of
 * S2MM Status Register to know the status of current transfer.  When S2MM
 * Undefined Burst Length Enable is enabled, this register must not be
 * programmed.  Reading this register when S2MM Undefined Burst Length Enable is
 * enabled, provides number of bytes received in undefined burst transaction.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_S2MM_LEN_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_LEN_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_S2MM_LEN_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_S2MM_LEN_NS_MASK ((uint32_t)(0xFFFFFFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_LEN_S2MM_LEN_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_LEN_S2MM_LEN_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_LEN_S2MM_LEN_SHIFT)

/*******************************************************************************
 * Register: S2MM_ADDR0
 *
 * Description: This register contains lower 32 bits of the start address of the
 * S2MM data transfer
 */
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_OFFSET     (0x1cU)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_RW_MASK    (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_READ_MASK  (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: S2MM_ADDR0
 *
 * Field Desc: S2MM address 0.This register is used to configured lower 32 bits
 * address for AXI4-Memory Mapped interface. When GUI parameter  S2MM
 * Command/Status FIFO Enable is not enabled, this register must not be written
 * until the current transfer is completed. User can poll Done  bit of S2MM
 * status register to know the status of current transfer Note: S2MM address
 * register0 and S2MM address register1  (only if GUI parameter S2MM Address
 * Width is configured to more than 32) are used as start address to transfer
 * total number of bytes  configured in S2MM length register. If start address
 * and total number of bytes crosses 4K address boundary or burst length is more
 * than  256 then core breaks the S2MM transfer into multiple transfers
 * internally. The CoreAXI4ProtoConv does not support unaligned transfers so
 * start address must always be aligned with the S2MM Data Width.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_S2MM_ADDR0_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_S2MM_ADDR0_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_S2MM_ADDR0_NS_MASK \
                ((uint32_t)(0xFFFFFFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR0_S2MM_ADDR0_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_ADDR0_S2MM_ADDR0_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_ADDR0_S2MM_ADDR0_SHIFT)

/*******************************************************************************
 * Register: S2MM_ADDR1
 *
 * Description: This register contains upper 32 bits of the start address of the
 * S2MM data transfer
 */
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_OFFSET     (0x20U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_RW_MASK    (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_READ_MASK  (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: S2MM_ADDR1
 *
 * Field Desc: S2MM address 1.This register is used to configured upper 32 bits
 * of start address for AXI4-Memory Mapped interface.Note: This register must be
 * programmed only when GUI parameter S2MM Address Width configured to more than
 * 32.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_S2MM_ADDR1_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_S2MM_ADDR1_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_S2MM_ADDR1_NS_MASK ((uint32_t)(0xFFFFFFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_ADDR1_S2MM_ADDR1_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_ADDR1_S2MM_ADDR1_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_S2MM_ADDR1_S2MM_ADDR1_SHIFT)

/*******************************************************************************
 * Register: S2MM_INTRENB
 *
 * Description: Interrupt enable register
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_OFFSET     (0x24U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_RW_MASK    (0x0000001DU)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_READ_MASK  (0x0000001DU)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_WRITE_MASK (0x0000001DU)

/**
 * Field Name: S2MM_IE_DONE
 *
 * Field Desc:
 *
 * 0b0 Disable interrupt generation when S2MM done is asserted.
 * 0b1 Enable interrupt generation when S2MM done is asserted.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_DONE_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_DONE_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_DONE_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_DONE_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_DONE_NS_MASK \
                      << COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_DONE_SHIFT)

/**
 * Field Name: S2MM_IE_AXI4ERR
 *
 * Field Desc:
 *
 * 0b0 Disable interrupt generation when AXI4 bus error is detected.
 * 0b1 Enable interrupt generation when AXI4 bus error is detected.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_AXI4ERR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_AXI4ERR_SHIFT   (2U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_AXI4ERR_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_AXI4ERR_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_AXI4ERR_NS_MASK \
                   << COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_AXI4ERR_SHIFT)

/**
 * Field Name: S2MM_IE_PKTDROP_OVF
 *
 * Field Desc:
 *
 * 0b0 Disable interrupt generation when Packet is dropped due to fifo overflow.
 * 0b1 Enable interrupt generation when Packet is dropped due to fifo overflow.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_OVF_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_OVF_SHIFT   (3U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_OVF_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_OVF_MASK \
              (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_OVF_NS_MASK \
               << COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_OVF_SHIFT)

/**
 * Field Name: S2MM_IE_PKTDROP_ERR
 *
 * Field Desc:
 *
 * 0b0 Disable interrupt generation when Packet is dropped due to error
 * detection in the received packet. 0b1 Enable interrupt generation when Packet
 * is dropped due to error detection in the received packet.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_ERR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_ERR_SHIFT   (4U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_ERR_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_ERR_MASK \
              (COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_ERR_NS_MASK \
               << COREAXI4PROTOCONV_REGS_S2MM_INTRENB_S2MM_IE_PKTDROP_ERR_SHIFT)

/*******************************************************************************
 * Register: S2MM_INTRSRC
 *
 * Description: Interrupt source register
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_OFFSET     (0x28U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_RW_MASK    (0x03FF001DU)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_READ_MASK  (0x03FF001DU)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_WRITE_MASK (0x03FF001DU)

/**
 * Field Name: S2MM_IS_DONE
 *
 * Field Desc:
 *
 * 0b0 Indicates S2MM transfer is not completed Writing 0 to this bit will have
 * no effect. 0b1 Indicates S2MM transfer is completed Writing 1 to this bit
 * will clear the bit.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_DONE_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_DONE_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_DONE_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_DONE_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_DONE_NS_MASK \
                      << COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_DONE_SHIFT)

/**
 * Field Name: S2MM_IS_AXI4ERR
 *
 * Field Desc:
 *
 * 0b0 Indicates S2MM AXI4 Error is not detected Writing 0 to this bit will have
 * no effect. 0b1 Indicates S2MM AXI4 Error is detected Writing 1 to this bit
 * will clear the bit.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_AXI4ERR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_AXI4ERR_SHIFT   (2U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_AXI4ERR_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_AXI4ERR_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_AXI4ERR_NS_MASK \
                   << COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_AXI4ERR_SHIFT)

/**
 * Field Name: S2MM_IS_PKTDROP_OVF
 *
 * Field Desc:
 *
 * 0b0 Indicates S2MM packets not dropped due to overflow Writing 0 to this bit
 * will have no effect. 0b1 Indicates S2MM packets dropped due to overflow
 * Writing 1 to this bit will clear the bit.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_OVF_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_OVF_SHIFT   (3U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_OVF_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_OVF_MASK \
              (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_OVF_NS_MASK \
               << COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_OVF_SHIFT)

/**
 * Field Name: S2MM_IS_PKTDROP_ERR
 *
 * Field Desc:
 *
 * 0b0 Indicates S2MM packets not dropped due to error Writing 0 to this bit
 * will have no effect. 0b1 Indicates S2MM packets dropped due to error Writing
 * 1 to this bit will clear the bit.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_ERR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_ERR_SHIFT   (4U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_ERR_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_ERR_MASK \
              (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_ERR_NS_MASK \
               << COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_PKTDROP_ERR_SHIFT)

/**
 * Field Name: S2MM_IS_CMDID
 *
 * Field Desc: Command ID.Provides command ID for which S2MM transfer is
 * completed
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_CMDID_OFFSET \
    (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_CMDID_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_CMDID_NS_MASK \
                ((uint32_t)(0x000003FFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_CMDID_MASK \
                (COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_CMDID_NS_MASK \
                     << COREAXI4PROTOCONV_REGS_S2MM_INTRSRC_S2MM_IS_CMDID_SHIFT)

/*******************************************************************************
 * Register: S2MM_AXI4BUSERR_CNTR
 *
 * Description: This register provides S2MM AXI4 bus error count.
 */
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_OFFSET     (0x100U)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_RW_MASK    (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_READ_MASK  (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: S2MM_AXI4BUSERR_CNTR
 *
 * Field Desc: This register provides S2MM AXI4 bus error count.  S2MM AXi4 bus
 * error counter is incremented when S2MM AXI4WRSP (either SLVERR or DECERR) of
 * S2MM status register is set to 1.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_S2MM_AXI4BUSERR_CNTR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_S2MM_AXI4BUSERR_CNTR_SHIFT \
              (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_S2MM_AXI4BUSERR_CNTR_NS_MASK \
                ((uint32_t)(0x0000FFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_S2MM_AXI4BUSERR_CNTR_MASK \
     (COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_S2MM_AXI4BUSERR_CNTR_NS_MASK \
      << COREAXI4PROTOCONV_REGS_S2MM_AXI4BUSERR_CNTR_S2MM_AXI4BUSERR_CNTR_SHIFT)

/*******************************************************************************
 * Register: S2MM_PKTDRPERR_CNTR
 *
 * Description: This register provides S2MM packet drop count due to error
 * detection.
 */
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_OFFSET     (0x104U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_RW_MASK    (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_READ_MASK  (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: S2MM_PKTDRPERR_CNTR
 *
 * Field Desc: This register provides S2MM packet drop count due to error
 * detection.  It is used to count the number of packets is dropped when input
 * port S2MM_PKT_ERR is asserted at end of packet boundary  i.e when
 * T_AXI4S_TLAST of S2MM is asserted. This register should be accessed only when
 * S2MM_PKT_DROP_ERR parameter is enabled.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_S2MM_PKTDRPERR_CNTR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_S2MM_PKTDRPERR_CNTR_SHIFT (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_S2MM_PKTDRPERR_CNTR_NS_MASK \
                ((uint32_t)(0x0000FFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_S2MM_PKTDRPERR_CNTR_MASK \
       (COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_S2MM_PKTDRPERR_CNTR_NS_MASK \
        << COREAXI4PROTOCONV_REGS_S2MM_PKTDRPERR_CNTR_S2MM_PKTDRPERR_CNTR_SHIFT)

/*******************************************************************************
 * Register: S2MM_PKTDRPOVF_CNTR
 *
 * Description: This register provides S2MM packet drop count due to S2MM data
 * FIFO overflow.
 */
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_OFFSET     (0x108U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_RW_MASK    (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_READ_MASK  (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: S2MM_PKTDRPOVF_CNTR
 *
 * Field Desc: This register provides S2MM packet drop count due to S2MM data
 * FIFO overflow.  It is used to count the number of packets is dropped when
 * S2MM Data FIFO is overflow.  This register should be accessed only when
 * S2MM_PKT_DROP_OVF parameter is enabled.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_S2MM_PKTDRPOVF_CNTR_OFFSET \
                (COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_S2MM_PKTDRPOVF_CNTR_SHIFT (0U)
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_S2MM_PKTDRPOVF_CNTR_NS_MASK \
                ((uint32_t)(0x0000FFFFUL))
#define COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_S2MM_PKTDRPOVF_CNTR_MASK \
       (COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_S2MM_PKTDRPOVF_CNTR_NS_MASK \
        << COREAXI4PROTOCONV_REGS_S2MM_PKTDRPOVF_CNTR_S2MM_PKTDRPOVF_CNTR_SHIFT)

/*******************************************************************************
 * Register: MM2S_VERSN
 *
 * Description: Version register. It will provide Major version and Minor
 * version of the core.
 */
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_OFFSET     (0x400U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_RW_MASK    (0xFFFF0000U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_READ_MASK  (0xFFFF0000U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_WRITE_MASK (0xFFFF0000U)

/**
 * Field Name: MINOR
 *
 * Field Desc: Minor version of the core
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MINOR_OFFSET \
    (COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MINOR_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MINOR_NS_MASK ((uint32_t)(0x0003FFFFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MINOR_MASK \
    (COREAXI4PROTOCONV_REGS_MM2S_VERSN_MINOR_NS_MASK \
     << COREAXI4PROTOCONV_REGS_MM2S_VERSN_MINOR_SHIFT)

/**
 * Field Name: MAJOR
 *
 * Field Desc: Major version of the core
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MAJOR_OFFSET \
    (COREAXI4PROTOCONV_REGS_MM2S_VERSN_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MAJOR_SHIFT   (24U)
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MAJOR_NS_MASK ((uint32_t)(0x0003FFFFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_VERSN_MAJOR_MASK \
    (COREAXI4PROTOCONV_REGS_MM2S_VERSN_MAJOR_NS_MASK \
     << COREAXI4PROTOCONV_REGS_MM2S_VERSN_MAJOR_SHIFT)

/*******************************************************************************
 * Register: MM2S_CTRL
 *
 * Description: MM2S control register.This register must be written after
 * writing MM2S length, MM2S address0 and MM2S addres1 (if GUI parameter MM2S
 * Address Width is configured to more than 32) registers.When GUI parameter
 * MM2S Command/Status FIFO Enable is not enabled, this register must not be
 * written until the current transfer is completed. User can poll Done bit of
 * MM2S status register to know the status of current transfer.
 */
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_OFFSET     (0x410U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_RW_MASK    (0x03FF0007U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_READ_MASK  (0x03FF0007U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_WRITE_MASK (0x03FF0007U)

/**
 * Field Name: START
 *
 * Field Desc: Writing 1 to this bit initiates MM2S data transferWhen GUI
 * parameter MM2S CommandStatus FIFO Enable is enabled writing 1 to this bit
 * stores the descriptor into command FIFO Descriptors consist of MM2S length
 * MM2S address0 MM2S address1 and MM2S control register  bits 16 to 25
 * informationMM2S Start bit is a self clearing bit
 *
 * 0b0 There is no impact on MM2S data transfer.
 * 0b1 Initiates MM2S data transfer.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_START_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_START_SHIFT    (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_START_NS_MASK  (0x1U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_START_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_CTRL_START_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_CTRL_START_SHIFT)

/**
 * Field Name: BURTYP
 *
 * Field Desc: AXI4Memory Mapped interface burst type  If the AXI4 length is 1
 * FIXED and INCR bursts are equivalent  Fixed burst transfer should be used to
 * access peripheral device which supports fixed address like FIFO  Increment
 * burst transfer should be used to access peripheral device which supports
 * incremental address like SRAM
 *
 * 2b00 AXI4Memory Mapped fixed burst.
 * 2b01 AXI4Memory Mapped increment burst.
 * 2b10 Reserved.
 * 2b11 Reserved.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_BURTYP_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_BURTYP_SHIFT   (1U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_BURTYP_NS_MASK ((uint32_t)(0x00000003UL))
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_BURTYP_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_CTRL_BURTYP_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_CTRL_BURTYP_SHIFT)

/**
 * Field Name: CMDID
 *
 * Field Desc: User can assign an arbitrary value to this field. On completion
 * of the MM2S transfer, IP core inserts this field into the MM2S status
 * register. IP core do not support out of order operation. Command issued in
 * the order must be reflected in the same order in the status
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_CMDID_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_CTRL_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_CMDID_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_CMDID_NS_MASK ((uint32_t)(0x000003FFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_CTRL_CMDID_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_CTRL_CMDID_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_CTRL_CMDID_SHIFT)

/*******************************************************************************
 * Register: MM2S_STS
 *
 * Description: This register contains status of the MM2S data transfers.
 */
#define COREAXI4PROTOCONV_REGS_MM2S_STS_REG_OFFSET     (0x414U)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_REG_RW_MASK    (0x03FF000DU)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_REG_READ_MASK  (0x03FF000DU)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_REG_WRITE_MASK (0x03FF000DU)

/**
 * Field Name: DONE
 *
 * Field Desc:
 *
 * 0b0 Indicates MM2S transfer is not completed.
 * 0b1 Indicates MM2S transfer is completed.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_STS_DONE_OFFSET  \
                (COREAXI4PROTOCONV_REGS_MM2S_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_DONE_SHIFT     (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_DONE_NS_MASK   ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_MM2S_STS_DONE_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_STS_DONE_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_STS_DONE_SHIFT)

/**
 * Field Name: AXI4RRSP
 *
 * Field Desc: AXI4 Memory mapped write response Indicates AXI4Memory Mapped
 * interface write response
 *
 * 2b00 OK.
 * 2b01 EXOKAY.
 * 2b10 SLVERR.
 * 2b11 DECERR.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_STS_AXI4RRSP_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_AXI4RRSP_SHIFT   (2U)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_AXI4RRSP_NS_MASK ((uint32_t)(0x00000003UL))
#define COREAXI4PROTOCONV_REGS_MM2S_STS_AXI4RRSP_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_STS_AXI4RRSP_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_STS_AXI4RRSP_SHIFT)

/**
 * Field Name: CMDID
 *
 * Field Desc: Command ID.Provides command ID for which MM2S transfer is
 * completed
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_STS_CMDID_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_STS_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_CMDID_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_MM2S_STS_CMDID_NS_MASK ((uint32_t)(0x000003FFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_STS_CMDID_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_STS_CMDID_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_STS_CMDID_SHIFT)

/*******************************************************************************
 * Register: MM2S_LEN
 *
 * Description: This register contains MM2S length
 */
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_OFFSET       (0x418U)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_LENGTH       (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_RW_MASK      (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_RO_MASK      (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_WO_MASK      (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_READ_MASK    (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_WRITE_MASK   (0xFFFFFFFFU)

/**
 * Field Name: MM2S_LEN
 *
 * Field Desc: Indicates the total number of bytes to be transmitted from the
 * AXI4-Stream interface to the AXI4-Memory Mapped interface.  Transfers can be
 * from 1 to 232 bytes. You must not program 0 value to this register. This
 * register must be programmed with byte aligned value of MM2S Data Width (in
 * bits).  For example, if MM2S Data Width (in bits) is configured to 128, then
 * this register must be programmed in multiple of (128/8) 16.  When MM2S
 * Command/Status FIFO Enable is not enabled, this register must not be written
 * until the current transfer is completed.  You can poll the MM2S Done bit of
 * MM2S Status Register to know the status of current transfer.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_MM2S_LEN_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_LEN_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_MM2S_LEN_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_MM2S_LEN_NS_MASK \
                ((uint32_t)(0xFFFFFFFFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_LEN_MM2S_LEN_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_LEN_MM2S_LEN_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_LEN_MM2S_LEN_SHIFT)

/*******************************************************************************
 * Register: MM2S_ADDR0
 *
 * Description: This register contains lower 32 bits of the start address of the
 * MM2S data transfer
 */
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_OFFSET     (0x41cU)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_RW_MASK    (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_READ_MASK  (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: MM2S_ADDR0
 *
 * Field Desc: MM2S address 0.This register is used to configured lower 32 bits
 * address for AXI4-Memory Mapped interface. When GUI parameter  MM2S
 * Command/Status FIFO Enable is not enabled, this register must not be written
 * until the current transfer is completed. User can poll Done  bit of MM2S
 * status register to know the status of current transfer Note: MM2S address
 * register0 and MM2S address register1  (only if GUI parameter MM2S Address
 * Width is configured to more than 32) are used as start address to transfer
 * total number of bytes  configured in MM2S length register. If start address
 * and total number of bytes crosses 4K address boundary or burst length is more
 * than  256 then core breaks the MM2S transfer into multiple transfers
 * internally. The CoreAXI4ProtoConv does not support unaligned transfers so
 * start address must always be aligned with the MM2S Data Width.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_MM2S_ADDR0_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_MM2S_ADDR0_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_MM2S_ADDR0_NS_MASK \
                ((uint32_t)(0xFFFFFFFFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR0_MM2S_ADDR0_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_ADDR0_MM2S_ADDR0_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_ADDR0_MM2S_ADDR0_SHIFT)

/*******************************************************************************
 * Register: MM2S_ADDR1
 *
 * Description: This register contains upper 32 bits of the start address of the
 * MM2S data transfer
 */
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_OFFSET     (0x420U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_RW_MASK    (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_READ_MASK  (0xFFFFFFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_WRITE_MASK (0xFFFFFFFFU)

/**
 * Field Name: MM2S_ADDR1
 *
 * Field Desc: MM2S address 1.This register is used to configured upper 32 bits
 * of start address for AXI4-Memory Mapped interface.Note: This register must be
 * programmed only when GUI parameter MM2S Address Width configured to more than
 * 32
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_MM2S_ADDR1_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_MM2S_ADDR1_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_MM2S_ADDR1_NS_MASK \
                ((uint32_t)(0xFFFFFFFFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_ADDR1_MM2S_ADDR1_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_ADDR1_MM2S_ADDR1_NS_MASK \
                         << COREAXI4PROTOCONV_REGS_MM2S_ADDR1_MM2S_ADDR1_SHIFT)

/*******************************************************************************
 * Register: MM2S_INTRENB
 *
 * Description: Interrupt enable register
 */
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_OFFSET     (0x424U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_RW_MASK    (0x00000005U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_READ_MASK  (0x00000005U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_WRITE_MASK (0x00000005U)

/**
 * Field Name: MM2S_IE_DONE
 *
 * Field Desc:
 *
 * 0b0 Disable interrupt generation when MM2S done is asserted.
 * 0b1 Enable interrupt generation when MM2S done is asserted.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_DONE_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_DONE_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_DONE_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_DONE_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_DONE_NS_MASK \
                      << COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_DONE_SHIFT)

/**
 * Field Name: MM2S_IE_AXI4ERR
 *
 * Field Desc:
 *
 * 0b0 Disable interrupt generation when AXI4 bus error is detected.
 * 0b1 Enable interrupt generation when AXI4 bus error is detected.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_AXI4ERR_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRENB_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_AXI4ERR_SHIFT   (2U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_AXI4ERR_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_AXI4ERR_MASK \
                  (COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_AXI4ERR_NS_MASK \
                   << COREAXI4PROTOCONV_REGS_MM2S_INTRENB_MM2S_IE_AXI4ERR_SHIFT)

/*******************************************************************************
 * Register: MM2S_INTRSRC
 *
 * Description: Interrupt source register
 */
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_OFFSET     (0x428U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_RW_MASK    (0x03FF0005U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_READ_MASK  (0x03FF0005U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_WRITE_MASK (0x03FF0005U)

/**
 * Field Name: MM2S_IS_DONE
 *
 * Field Desc:
 *
 * 0b0 Indicates MM2S transfer is not completed Writing 0 to this bit will have
 * no effect. 0b1 Indicates MM2S transfer is completed Writing 1 to this bit
 * will clear the bit.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_DONE_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_DONE_SHIFT   (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_DONE_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_DONE_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_DONE_NS_MASK \
                      << COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_DONE_SHIFT)

/**
 * Field Name: MM2S_IS_CMDID
 *
 * Field Desc: Command ID.Provides command ID for which MM2S transfer is
 * completed
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_CMDID_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_CMDID_SHIFT   (16U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_CMDID_NS_MASK \
                ((uint32_t)(0x000003FFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_CMDID_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_CMDID_NS_MASK \
                     << COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_CMDID_SHIFT)

/**
 * Field Name: MM2S_IS_AXI4ERR
 *
 * Field Desc:
 *
 * 0b0 Indicates MM2S AXI4 Error is not detected Writing 0 to this bit will have
 * no effect. 0b1 Indicates MM2S AXI4 Error is detected Writing 1 to this bit
 * will clear the bit.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_AXI4ERR_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_AXI4ERR_SHIFT   (2U)
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_AXI4ERR_NS_MASK \
                ((uint32_t)(0x00000001UL))
#define COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_AXI4ERR_MASK \
                (COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_AXI4ERR_NS_MASK \
                   << COREAXI4PROTOCONV_REGS_MM2S_INTRSRC_MM2S_IS_AXI4ERR_SHIFT)

/*******************************************************************************
 * Register: MM2S_AXI4BUSERR_CNTR
 *
 * Description: This register provides MM2S AXI4 bus error count.
 */
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_OFFSET     (0x500U)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_LENGTH     (0x4U)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_RW_MASK    (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_RO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_WO_MASK    (0x00000000U)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_READ_MASK  (0x0000FFFFU)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_WRITE_MASK (0x0000FFFFU)

/**
 * Field Name: MM2S_AXI4BUSERR_CNTR
 *
 * Field Desc: This register provides MM2S AXI4 bus error count. MM2S AXi4 bus
 * error counter is incremented when MM2S AXI4WRSP (either SLVERR or DECERR) of
 * MM2S status register is set to 1.
 *
 * Field Type: read-write
 */
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_MM2S_AXI4BUSERR_CNTR_OFFSET \
                (COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_REG_OFFSET)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_MM2S_AXI4BUSERR_CNTR_SHIFT \
                       (0U)
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_MM2S_AXI4BUSERR_CNTR_NS_MASK \
                ((uint32_t)(0x0000FFFFUL))
#define COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_MM2S_AXI4BUSERR_CNTR_MASK \
    (COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_MM2S_AXI4BUSERR_CNTR_NS_MASK \
     << COREAXI4PROTOCONV_REGS_MM2S_AXI4BUSERR_CNTR_MM2S_AXI4BUSERR_CNTR_SHIFT)

#ifdef __cplusplus
}
#endif

#endif //__CORE_AXI4PC_REGISTERS_H
