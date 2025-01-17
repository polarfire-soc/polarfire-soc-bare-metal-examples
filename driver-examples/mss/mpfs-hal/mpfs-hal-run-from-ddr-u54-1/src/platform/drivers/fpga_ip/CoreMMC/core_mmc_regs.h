/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_mmc_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreMMC memory map
 *
 */

#ifndef COREMMC_REGS_H_
#define COREMMC_REGS_H_

/**-----------------------------------------------------------------------------
  CoreMMC Registers
 */
#define CR0_REG_OFFSET 0x04u
#define CR1_REG_OFFSET 0x08u
#define CR2_REG_OFFSET 0x09u
#define CR3_REG_OFFSET 0x0Au
#define CR4_REG_OFFSET 0x0Bu
#define RR0_REG_OFFSET 0x10u

/** Response registers */
#define RR1_REG_OFFSET 0x14u
#define RR2_REG_OFFSET 0x15u
#define RR3_REG_OFFSET 0x16u
#define RR4_REG_OFFSET 0x17u
#define RR5_REG_OFFSET 0x18u
#define RR6_REG_OFFSET 0x19u
#define RR7_REG_OFFSET 0x1Au
#define RR8_REG_OFFSET 0x1Bu
#define RR9_REG_OFFSET 0x1Cu
#define RR10_REG_OFFSET 0x1Du
#define RR11_REG_OFFSET 0x1Eu
#define RR12_REG_OFFSET 0x1Fu
#define RR13_REG_OFFSET 0x20u
#define RR14_REG_OFFSET 0x21u
#define RR15_REG_OFFSET 0x22u
#define WDR_REG_OFFSET 0x24u
#define RDR_REG_OFFSET 0x28u
#define IMR_REG_OFFSET 0x2Cu
#define BIMR_REG_OFFSET 0x2Du

#define RSPTO_REG_OFFSET 0x3Cu
#define DATATO_REG_OFFSET 0x40u
#define BLOCK_LEN_0_REG_OFFSET 0x44u
#define BLOCK_LEN_1_REG_OFFSET 0x45u
#define BLOCK_LEN_2_REG_OFFSET 0x46u
#define BLOCK_LEN_3_REG_OFFSET 0x47u
#define DCTRL_REG_OFFSET 0x48u
#define CLKR_REG_OFFSET 0x4Cu

#define MBCOUNT_REG_OFFSET 0x50u

#define CLEAR_ALL 0xFFu
#define CLEAR_ALL_32 0xFFFFFFFFu

/**
  STATUS register details
 */
#define SR_REG_OFFSET 0x00u

/**
  ECRD bits. CRC Error Detected
 */
#define SR_ECRD_OFFSET 0x00u
#define SR_ECRD_MASK 0x01u
#define SR_ECRD_SHIFT 0u

/**
  EBUCD bits. Buffer Under run Detected
 */
#define SR_EBUCD_OFFSET 0x00u
#define SR_EBUCD_MASK 0x02u
#define SR_EBUCD_SHIFT 1u

/**
  EBOD bits. Buffer Overrun Detected
 */
#define SR_EBOD_OFFSET 0x00u
#define SR_EBOD_MASK 0x04u
#define SR_EBOD_SHIFT 2u

/**
  SRFE bits. Read FIFO Empty
 */
#define SR_SRFE_OFFSET 0x00u
#define SR_SRFE_MASK 0x08u
#define SR_SRFE_SHIFT 3u

/**
  SWFE bits. Write FIFO Empty
 */
#define SR_SWFE_OFFSET 0x00u
#define SR_SWFE_MASK 0x10u
#define SR_SWFE_SHIFT 4u

/**
  SRFF bits. Read FIFO full
 */
#define SR_SRFF_OFFSET 0x00u
#define SR_SRFF_MASK 0x20u
#define SR_SRFF_SHIFT 5u

/**
  SWFF bits. Write FIFO full
 */
#define SR_SWFF_OFFSET 0x00u
#define SR_SWFF_MASK 0x40u
#define SR_SWFF_SHIFT 6u

/**
  RDRE bits. Response Data ready
 */
#define SR_RDRE_OFFSET 0x00u
#define SR_RDRE_MASK 0x80u
#define SR_RDRE_SHIFT 7u

/**
  Version Register details
 */
#define VR_REG_OFFSET 0x01u

/**
  REV bits. Hardware Revision Number
 */
#define VR_REV_OFFSET 0x01u
#define VR_REV_MASK 0x03u
#define VR_REV_SHIFT 0u

/**
  MMCDW bits. MMC Data Width
 */
#define VR_MMCDW_OFFSET 0x01u
#define VR_MMCDW_MASK 0x0Cu
#define VR_MMCDW_SHIFT 2u

/**
  MMCFD bits. MMC FIFO Depth
 */
#define VR_MMCFD_OFFSET 0x01u
#define VR_MMCFD_MASK 0x30u
#define VR_MMCFD_SHIFT 4u

/**
  CLKDIV bits. Clock Divider
 */
#define VR_CLKDIV_OFFSET 0x01u
#define VR_CLKDIV_MASK 0xC0u
#define VR_CLKDIV_SHIFT 6u

/**-----------------------------------------------------------------------------
  Major Version Register details
  Only present if Hardware Revision Number in VR_REG_OFFSET register == 0x03
  Uses all 8 bits
 */
#define VR_MAJ_REG_OFFSET 0x02u

/**-----------------------------------------------------------------------------
  Minor Version Register details
  Only present if Hardware Revision Number in VR_REG_OFFSET register == 0x03
  Uses all 8 bits
 */
#define VR_MIN_REG_OFFSET 0x03u

/**
  SBUI bits. Under-run.
 */
#define IMR_MBUI_OFFSET 0x2Cu
#define IMR_MBUI_MASK 0x01u
#define IMR_MBUI_SHIFT 0u

/**
  SBOI bits. Over-run.
 */
#define IMR_MBOI_OFFSET 0x2Cu
#define IMR_MBOI_MASK 0x02u
#define IMR_MBOI_SHIFT 1u

/**
  SCSI bits. Command Sent.
 */
#define IMR_MCSI_OFFSET 0x2Cu
#define IMR_MCSI_MASK 0x04u
#define IMR_MCSI_SHIFT 2u

/**
  SRRI bits. Response Received.
 */
#define IMR_MRRI_OFFSET 0x2Cu
#define IMR_MRRI_MASK 0x08u
#define IMR_MRRI_SHIFT 3u

/**
  STXI bits. Transmit bit error.
 */
#define IMR_MTXI_OFFSET 0x2Cu
#define IMR_MTXI_MASK 0x10u
#define IMR_MTXI_SHIFT 4u

/**
  STBI bits. Stop Bit Error.
 */
#define IMR_MTBI_OFFSET 0x2Cu
#define IMR_MTBI_MASK 0x20u
#define IMR_MTBI_SHIFT 5u

/**
  SSBI bits. Start Bit Error => No response from eMMC to command after x
  clock cycles
 */
#define IMR_MSBI_OFFSET 0x2Cu
#define IMR_MSBI_MASK 0x40u
#define IMR_MSBI_SHIFT 6u

/**
  MUER bits. User Error. FIFO has not been filled.
 */
#define IMR_MUER_OFFSET 0x2Cu
#define IMR_MUER_MASK 0x80u
#define IMR_MUER_SHIFT 7u

#define IMR_MASK_MULTI_READ                                                    \
  (0xFF &                                                                      \
   ~(IMR_MCSI_MASK /*| IMR_MUER_MASK*/)) /* Command send, user error masked */
#define IMR_MASK_MULTI_WRITE                                                   \
  (0xFF &                                                                      \
   ~(IMR_MCSI_MASK /*| IMR_MUER_MASK*/)) /* Command send, user error masked */

/**-----------------------------------------------------------------------------
  Block Interrupt Mask Register
 */
#define BIMR_REG_OFFSET 0x2Du

/**
  SBWDONE bits. Block Write Done.
 */
#define BIMR_SBWDONE_OFFSET 0x2Du
#define BIMR_SBWDONE_MASK 0x01u
#define BIMR_SBWDONE_SHIFT 0u

/**
  SBRDONE bits. Block Read Done.
 */
#define BIMR_SBRDONE_OFFSET 0x2Du
#define BIMR_SBRDONE_MASK 0x02u
#define BIMR_SBRDONE_SHIFT 1u

/**
  SBCRCERR bits. CRC Error.
 */
#define BIMR_SBCRCERR_OFFSET 0x2Du
#define BIMR_SBCRCERR_MASK 0x04u
#define BIMR_SBCRCERR_SHIFT 2u

/**
  SDATTO bits. Block Read Data Time-out.
 */
#define BIMR_SDATTO_OFFSET 0x2Du
#define BIMR_SDATTO_MASK 0x08u
#define BIMR_SDATTO_SHIFT 3u

/**
  SBRSTERR bits. Block Read Stop Bit Error.
 */
#define BIMR_SBRSTERR_OFFSET 0x2Du
#define BIMR_SBRSTERR_MASK 0x10u
#define BIMR_SBRSTERR_SHIFT 4u

/**
  SCRCSTERR bits. Block CRC Response Error.
 */
#define BIMR_SCRCSTERR_OFFSET 0x2Du
#define BIMR_SCRCSTERR_MASK 0x20u
#define BIMR_SCRCSTERR_SHIFT 5u

/**
  SWBUSYTOERR bits. Card not ready-- timeout Data0 down for too long
 */
#define BIMR_SWBUSYTOERR_OFFSET 0x2Du
#define BIMR_SWBUSYTOERR_MASK 0x40u
#define BIMR_SWBUSYTOERR_SHIFT 6u

/**
  SWBFIFOTOERR bits. Timeout on data being present in FIFO to write to eMMC
 */
#define BIMR_SWBFIFOTOERR_OFFSET 0x2Du
#define BIMR_SWBFIFOTOERR_MASK 0x80u
#define BIMR_SWBFIFOTOERR_SHIFT 7u

#define BIMR_MASK_MULTI_READ                                                   \
  (0xFF & ~0x03) /* all except block read and block write done */
#define BIMR_MASK_MULTI_WRITE                                                  \
  (0xFF & ~0x03) /* all except block read and block write done */
#define BIMR_MASK_SINGLE_READ (0xFF & ~BIMR_SBWDONE_MASK)
#define BIMR_MASK_SINGLE_WRITE (0xFF & ~BIMR_SBRDONE_MASK)

/**-----------------------------------------------------------------------------
  Multi-Block Interrupt Mask Register
 */
#define MBIMR_REG_OFFSET 0x2Eu

/**
  MBWDONE bits. Multi Block Write Done.
 */
#define MBIMR_MBWDONE_OFFSET 0x2Eu
#define MBIMR_MBWDONE_MASK 0x01u
#define MBIMR_MBWDONE_SHIFT 0u

/**
  MBRDONE bits. Multi Block Read Done.
 */
#define MBIMR_MBRDONE_OFFSET 0x2Eu
#define MBIMR_MBRDONE_MASK 0x02u
#define MBIMR_MBRDONE_SHIFT 1u

/**
  MBCRCERR bits. CRC Error.
 */
#define MBIMR_MBCRCERR_OFFSET 0x2Eu
#define MBIMR_MBCRCERR_MASK 0x04u
#define MBIMR_MBCRCERR_SHIFT 2u

/**
  MDATTO bits. MultiBlock Read Data Time-out.
 */
#define MBIMR_MDATTO_OFFSET 0x2Eu
#define MBIMR_MDATTO_MASK 0x08u
#define MBIMR_MDATTO_SHIFT 3u

/**
  MBRSTERR bits. Block Read Stop Bit Error.
 */
#define MBIMR_MBRSTERR_OFFSET 0x2Eu
#define MBIMR_MBRSTERR_MASK 0x10u
#define MBIMR_MBRSTERR_SHIFT 4u

/**
  MCRCSTERR bits. Block CRC Response Error.
 */
#define MBIMR_MCRCSTERR_OFFSET 0x2Eu
#define MBIMR_MCRCSTERR_MASK 0x20u
#define MBIMR_MCRCSTERR_SHIFT 5u

/**
  MWBUSYTOERR bits. Card not ready-- timeout Data0 down for too long
 */
#define MBIMR_MWBUSYTOERR_OFFSET 0x2Eu
#define MBIMR_MWBUSYTOERR_MASK 0x40u
#define MBIMR_MWBUSYTOERR_SHIFT 6u

/**
  MWBFIFOTOERR bits. Timeout on data being present in FIFO to write to eMMC
 */
#define MBIMR_MWBFIFOTOERR_OFFSET 0x2Eu
#define MBIMR_MWBFIFOTOERR_MASK 0x80u
#define MBIMR_MWBFIFOTOERR_SHIFT 7u

#define MBIMR_MASK_MULTI_READ                                                  \
  (0xFF & ~MBISR_MBWDONE_MASK) /* all except multi-block write done */
#define MBIMR_MASK_MULTI_WRITE                                                 \
  (0xFF & ~MBIMR_MBRDONE_MASK) /* all except multi-block read done */

/**-----------------------------------------------------------------------------
  Interrupt Status Register details
 */
#define ISR_REG_OFFSET 0x30u

/**
  SBUI bits. Under-run.
 */
#define ISR_SBUI_OFFSET 0x30u
#define ISR_SBUI_MASK 0x01u
#define ISR_SBUI_SHIFT 0u

/**
  SBUI bits. Over-run.
 */
#define ISR_SBOI_OFFSET 0x30u
#define ISR_SBOI_MASK 0x02u
#define ISR_SBOI_SHIFT 1u

/**
  SCSI bits. Command Sent.
 */
#define ISR_SCSI_OFFSET 0x30u
#define ISR_SCSI_MASK 0x04u
#define ISR_SCSI_SHIFT 2u

/**
  SRRI bits. Response Received.
 */
#define ISR_SRRI_OFFSET 0x30u
#define ISR_SRRI_MASK 0x08u
#define ISR_SRRI_SHIFT 3u

/**
  STXI bits. Transmit bit error.
 */
#define ISR_STXI_OFFSET 0x30u
#define ISR_STXI_MASK 0x10u
#define ISR_STXI_SHIFT 4u

/**
  STBI bits. Stop Bit Error.
 */
#define ISR_STBI_OFFSET 0x30u
#define ISR_STBI_MASK 0x20u
#define ISR_STBI_SHIFT 5u

/**
  SSBI bits. Start bit Bit Error => no data received from eMMC
 */
#define ISR_SSBI_OFFSET 0x30u
#define ISR_SSBI_MASK 0x40u
#define ISR_SSBI_SHIFT 6u

/**
  SUER bits. User Error. FIFI not filled/emptied as expected
 */
#define ISR_SUER_OFFSET 0x30u
#define ISR_SUER_MASK 0x80u
#define ISR_SUER_SHIFT 7u

#define ISR_ERRORS                                                             \
  (ISR_SUER_MASK | ISR_SSBI_MASK | ISR_STBI_MASK | ISR_STXI_MASK |             \
   ISR_SBOI_MASK | ISR_SBUI_MASK)

/**-----------------------------------------------------------------------------
  Block Interrupt Status Register Details
 */
#define BISR_REG_OFFSET 0x31u

/**
  SBWDONE bits. Block Write Done.
 */
#define BISR_SBWDONE_OFFSET 0x31u
#define BISR_SBWDONE_MASK 0x01u
#define BISR_SBWDONE_SHIFT 0u

/**
  SBRDONE bits. Block Read Done.
 */
#define BISR_SBRDONE_OFFSET 0x31u
#define BISR_SBRDONE_MASK 0x02u
#define BISR_SBRDONE_SHIFT 1u

/**
  SBCRCERR bits. CRC Error.
 */
#define BISR_SBCRCERR_OFFSET 0x31u
#define BISR_SBCRCERR_MASK 0x04u
#define BISR_SBCRCERR_SHIFT 2u

/**
  SBRTOUTERR bits. Block Read start Time-out.
 */
#define BISR_SBRTOUTERR_OFFSET 0x31u
#define BISR_SBRTOUTERR_MASK 0x08u
#define BISR_SBRTOUTERR_SHIFT 3u

/**
  SBRSTERR bits. Block Read Stop Bit Error.
 */
#define BISR_SBRSTERR_OFFSET 0x31u
#define BISR_SBRSTERR_MASK 0x10u
#define BISR_SBRSTERR_SHIFT 4u

/**
  SCRCSTERR bits. Block write timeout on start bit of CRC response frame or no
  stop bit received on same frame.
 */
#define BISR_SCRCSTERR_OFFSET 0x31u
#define BISR_SCRCSTERR_MASK 0x20u
#define BISR_SCRCSTERR_SHIFT 5u

/**
  SWBUSYTOERR bits. Card not ready-- timeout Data0 down for too long
 */
#define BISR_SWBUSYTOERR_OFFSET 0x31u
#define BISR_SWBUSYTOERR_MASK 0x40u
#define BISR_SWBUSYTOERR_SHIFT 6u

/**
  SWBFIFOTOERR bits. Timeout on data being present in FIFO to write to eMMC
 */
#define BISR_SWBFIFOTOERR_OFFSET 0x31u
#define BISR_SWBFIFOTOERR_MASK 0x80u
#define BISR_SWBFIFOTOERR_SHIFT 7u

#define BISR_ERRORS                                                            \
  (BISR_SWBFIFOTOERR_MASK | BISR_SWBUSYTOERR_MASK | BISR_SCRCSTERR_MASK |      \
   BISR_SBRSTERR_MASK | BISR_SBRTOUTERR_MASK | BISR_SBCRCERR_MASK)

/**-----------------------------------------------------------------------------
  Multi Block Interrupt Status Register Details
 */
#define MBISR_REG_OFFSET 0x32u

/**
  MBWDONE bits. Block Write Done.
 */
#define MBISR_MBWDONE_OFFSET 0x32u
#define MBISR_MBWDONE_MASK 0x01u
#define MBISR_MBWDONE_SHIFT 0u

/**
  MBRDONE bits. Block Read Done.
 */
#define MBISR_MBRDONE_OFFSET 0x32u
#define MBISR_MBRDONE_MASK 0x02u
#define MBISR_MBRDONE_SHIFT 1u

/**
  MBCRCERR bits. CRC Error.
 */
#define MBISR_MBCRCERR_OFFSET 0x32u
#define MBISR_MBCRCERR_MASK 0x04u
#define MBISR_MBCRCERR_SHIFT 2u

/**
  MBRTOUTERR bits. Block Read Data Time-out
 */
#define MBISR_MBRTOUTERR_OFFSET 0x32u
#define MBISR_MBRTOUTERR_MASK 0x08u
#define MBISR_MBRTOUTERR_SHIFT 3u

/**
  MBRSTERR bits. Block Read Stop Bit Error.
 */
#define MBISR_MBRSTERR_OFFSET 0x32u
#define MBISR_MBRSTERR_MASK 0x10u
#define MBISR_MBRSTERR_SHIFT 4u

/**
  MCRCSTERR bits
  Block CRC Response Error occurred with stop bit error
 */
#define MBISR_MCRCSTERR_OFFSET 0x32u
#define MBISR_MCRCSTERR_MASK 0x20u
#define MBISR_MCRCSTERR_SHIFT 5u

/**
  MWBUSYTOERR bits
  Card not ready -- timeout Data0 down for too long eMMC slave doesn't hold
  DAT0 low indefinitely in single & multiple block transfers start bit to be
  loaded onto the bus by the eMMC slave from either the SBRSTRT or the MBRSTRT
  bit being set in the SBCSR or the MBSCR.
 */
#define MBISR_MWBUSYTOERR_OFFSET 0x32u
#define MBISR_MWBUSYTOERR_MASK 0x40u
#define MBISR_MWBUSYTOERR_SHIFT 6u

/**
  MWBFIFOTOERR bits
  Timeout on data being present in FIFO to write to eMMC.
  Timeout indicating that CoreMMC waits for a block length amount of data to be
  present in the write FIFO from the setting of the SBWSTRT or MBWSTRT bits in
  the SBCSR or the MBCSR for the first block in a single or multiple block
  write transfer.
  Or from the stop bit of the CRC status frame to there being a block length
  amount of data in the write FIFO for subsequent blocks in a multiple block
  write transfer
 */
#define MBISR_MWBFIFOTOERR_OFFSET 0x32u
#define MBISR_MWBFIFOTOERR_MASK 0x80u
#define MBISR_MWBFIFOTOERR_SHIFT 7u

#define MBISR_ERRORS                                                           \
  (MBISR_MWBFIFOTOERR_MASK | MBISR_MWBUSYTOERR_MASK | MBISR_MCRCSTERR_MASK |   \
   MBISR_MBRSTERR_MASK | MBISR_MBRTOUTERR_MASK | MBISR_MBCRCERR_MASK)
#define ISR_BISR_MBSIR_ERR                                                     \
  ((MBISR_ERRORS << 16) | (BISR_ERRORS << 8) | ISR_ERRORS)

/**-----------------------------------------------------------------------------
  Interrupt Clear Register details
 */
#define ICR_REG_OFFSET 0x34u

/**
  CBUI bits. Clear Under-run.
 */
#define ICR_CBUI_OFFSET 0x34u
#define ICR_CBUI_MASK 0x01u
#define ICR_CBUI_SHIFT 0u

/**
  CBUI bits. Clear Over-run.
 */
#define ICR_CBOI_OFFSET 0x34u
#define ICR_CBOI_MASK 0x02u
#define ICR_CBOI_SHIFT 1u

/**
  CCSI bits. Clear Command Sent.
 */
#define ICR_CCSI_OFFSET 0x34u
#define ICR_CCSI_MASK 0x04u
#define ICR_CCSI_SHIFT 2u

/**
  CRRI bits. Clear Response Received.
 */
#define ICR_CRRI_OFFSET 0x34u
#define ICR_CRRI_MASK 0x08u
#define ICR_CRRI_SHIFT 3u

/**
  CTXI bits. Clear Transmit bit error.
 */
#define ICR_CTXI_OFFSET 0x34u
#define ICR_CTXI_MASK 0x10u
#define ICR_CTXI_SHIFT 4u

/**
  CTBI bits. Clear Stop Bit Error.
 */
#define ICR_CTBI_OFFSET 0x34u
#define ICR_CTBI_MASK 0x20u
#define ICR_CTBI_SHIFT 5u

/**
  CSBI bits. Clear Stop Bit Error.
 */
#define ICR_CSBI_OFFSET 0x34u
#define ICR_CSBI_MASK 0x40u
#define ICR_CSBI_SHIFT 6u

/**
  CUER bits. Clear User Error.
 */
#define ICR_CUER_OFFSET 0x34u
#define ICR_CUER_MASK 0x80u
#define ICR_CUER_SHIFT 7u

/**-----------------------------------------------------------------------------
  Block Interrupt Clear Register Details
 */
#define BICR_REG_OFFSET 0x35u

/**
  CBWDONE bits. Clear Block Write Done.
 */
#define BICR_CBWDONE_OFFSET 0x35u
#define BICR_CBWDONE_MASK 0x01u
#define BICR_CBWDONE_SHIFT 0u

/**
  CBRDONE bits. Clear Block Read Done.
 */
#define BICR_CBRDONE_OFFSET 0x35u
#define BICR_CBRDONE_MASK 0x02u
#define BICR_CBRDONE_SHIFT 1u

/**
  CBCRCERR bits. Clear CRC Error.
 */
#define BICR_CBCRCERR_OFFSET 0x35u
#define BICR_CBCRCERR_MASK 0x04u
#define BICR_CBCRCERR_SHIFT 2u

/**
  CDATTO bits. Clear Block Read Data Time-out.
 */
#define BICR_CDATTO_OFFSET 0x35u
#define BICR_CDATTO_MASK 0x08u
#define BICR_CDATTO_SHIFT 3u

/**
  CBRSTERR bits. Clear Block Read Stop Bit Error.
 */
#define BICR_CBRSTERR_OFFSET 0x35u
#define BICR_CBRSTERR_MASK 0x10u
#define BICR_CBRSTERR_SHIFT 4u

/**
  CCRCSTERR bits. Clear Block CRC Response Error.
 */
#define BICR_CCRCSTERR_OFFSET 0x35u
#define BICR_CCRCSTERR_MASK 0x20u
#define BICR_CCRCSTERR_SHIFT 5u

/**-----------------------------------------------------------------------------
  Multi Block Interrupt Clear Register Details
 */
#define MBICR_REG_OFFSET 0x36u

/**
  CBWDONE bits. Clear Block Write Done.
 */
#define MBICR_CBWDONE_OFFSET 0x36u
#define MBICR_CBWDONE_MASK 0x01u
#define MBICR_CBWDONE_SHIFT 0u

/**
  CBRDONE bits. Clear Block Read Done.
 */
#define MBICR_CBRDONE_OFFSET 0x36u
#define MBICR_CBRDONE_MASK 0x02u
#define MBICR_CBRDONE_SHIFT 1u

/**
  CBCRCERR bits. Clear CRC Error.
 */
#define MBICR_CBCRCERR_OFFSET 0x36u
#define MBICR_CBCRCERR_MASK 0x04u
#define MBICR_CBCRCERR_SHIFT 2u

/**
  CDATTO bits. Clear Block Read Data Time-out.
 */
#define MBICR_CDATTO_OFFSET 0x36u
#define MBICR_CDATTO_MASK 0x08u
#define MBICR_CDATTO_SHIFT 3u

/**
  CBRSTERR bits. Clear Block Read Stop Bit Error.
 */
#define MBICR_CBRSTERR_OFFSET 0x36u
#define MBICR_CBRSTERR_MASK 0x10u
#define MBICR_CBRSTERR_SHIFT 4u

/**
  CCRCSTERR bits. Clear Block CRC Response Error.
 */
#define MBICR_CCRCSTERR_OFFSET 0x36u
#define MBICR_CCRCSTERR_MASK 0x20u
#define MBICR_CCRCSTERR_SHIFT 5u

/**
  MWBUSYTOERR bits. Card not ready-- timeout Data0 down for too long
 */
#define MBICR_MWBUSYTOERR_OFFSET 0x36u
#define MBICR_MWBUSYTOERR_MASK 0x40u
#define MBICR_MWBUSYTOERR_SHIFT 6u

/**
  MWBFIFOTOERR bits. Timeout on data being present in FIFO to write to eMMC
 */
#define MBICR_MWBFIFOTOERR_OFFSET 0x36u
#define MBICR_MWBFIFOTOERR_MASK 0x80u
#define MBICR_MWBFIFOTOERR_SHIFT 7u

/**-----------------------------------------------------------------------------
  Control Register Details
 */
#define CTRL_REG_OFFSET 0x38u

/**
  SWRST bits. Software Reset.
 */
#define CTRL_SWRST_OFFSET 0x38u
#define CTRL_SWRST_MASK 0x01u
#define CTRL_SWRST_SHIFT 0u

/**
  SLRST bits. Slave Reset.
 */
#define CTRL_SLRST_OFFSET 0x38u
#define CTRL_SLRST_MASK 0x02u
#define CTRL_SLRST_SHIFT 1u

/**
  CLKOE bits. Clock Output Enable.
 */
#define CTRL_CLKOE_OFFSET 0x38u
#define CTRL_CLKOE_MASK 0x04u
#define CTRL_CLKOE_SHIFT 2u

/**
  MIDLE bits. CoreMMC Idle.
 */
#define CTRL_MIDLE_OFFSET 0x38u
#define CTRL_MIDLE_MASK 0x08u
#define CTRL_MIDLE_SHIFT 3u

/**
  MFCL. Force Command Line Low.
 */
#define CTRL_MFCL_OFFSET 0x38u
#define CTRL_MFCL_MASK 0x10u
#define CTRL_MFCL_SHIFT 4u

/**
  RFIFO: Reset FIFO
 */
#define CTRL_RFIFO_OFFSET 0x38u
#define CTRL_RFIFO_MASK 0x20u
#define CTRL_RFIFO_SHIFT 5u

/**
  BUSY. MMC Device Busy.
 */
#define CTRL_MBUSY_OFFSET 0x38u
#define CTRL_MBUSY_MASK 0x80u
#define CTRL_MBUSY_SHIFT 7u

/**-----------------------------------------------------------------------------
  BLOCK CONTROl and STATUS register details
 */
#define BCSR_REG_OFFSET 0x39u

/**
  BWSTRT bits. Block Write Start.
 */
#define BCSR_BWSTRT_OFFSET 0x39u
#define BCSR_BWSTRT_MASK 0x01u
#define BCSR_BWSTRT_SHIFT 0u

/**
  BRSTRT bits. Block Read Start.
 */
#define BCSR_BRSTRT_OFFSET 0x39u
#define BCSR_BRSTRT_MASK 0x02u
#define BCSR_BRSTRT_SHIFT 1u

/**
  BDONE bits. Block Done.
 */
#define BCSR_BDONE_OFFSET 0x39u
#define BCSR_BDONE_MASK 0x04u
#define BCSR_BDONE_SHIFT 2u

/**
  BCRCERR bits. Block CRC Error.
 */
#define BCSR_BCRCERR_OFFSET 0x39u
#define BCSR_BCRCERR_MASK 0x08u
#define BCSR_BCRCERR_SHIFT 3u

/**
  BWST bits. Block Write Status.
 */
#define BCSR_BWST_OFFSET 0x39u
#define BCSR_BWST_MASK 0xF0u
#define BCSR_BWST_SHIFT 4u

/**-----------------------------------------------------------------------------
  MULTI BLOCK CONTROl and STATUS register details
 */
#define MBCSR_REG_OFFSET 0x3Au

/**
  MBWSTRT bits. Block Write Start.
 */
#define MBCSR_MBWSTRT_OFFSET 0x3Au
#define MBCSR_MBWSTRT_MASK 0x01u
#define MBCSR_MBWSTRT_SHIFT 0u

/**
  MBRSTRT bits. Block Read Start.
 */
#define MBCSR_MBRSTRT_OFFSET 0x3Au
#define MBCSR_MBRSTRT_MASK 0x02u
#define MBCSR_MBRSTRT_SHIFT 1u

/**
  MBDONE bits. Block Done.
 */
#define MBCSR_MBDONE_OFFSET 0x3Au
#define MBCSR_MBDONE_MASK 0x04u
#define MBCSR_MBDONE_SHIFT 2u

/**
  MBCRCERR bits. Block CRC Error.
 */
#define MBCSR_MBCRCERR_OFFSET 0x3Au
#define MBCSR_MBCRCERR_MASK 0x08u
#define MBCSR_MBCRCERR_SHIFT 3u

/**
  MBWST bits. Block Write Status.
 */
#define MBCSR_MBWST_OFFSET 0x3Au
#define MBCSR_MBWST_MASK 0xF0u
#define MBCSR_MBWST_SHIFT 4u

#endif
