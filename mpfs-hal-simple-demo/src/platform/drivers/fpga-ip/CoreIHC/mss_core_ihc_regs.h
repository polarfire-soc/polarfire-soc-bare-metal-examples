 /******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 * 
 * Register bit offsets and masks definitions for PolarFire SoC MSS MMUART
 * 
 */

#ifndef MSS_CORE_IHC_REGS_H_
#define MSS_CORE_IHC_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 Register Bit definitions
 */

/*
 * Control register bits
 */
#define RMPIE_EN                    (0x01U << 0U)
#define MPIE_EN                     (0x01U << 1U)
#define MP_MESSAGE_PRESENT          (0x01U << 2U)
#define RMP_MESSAGE_PRESENT         (0x01U << 3U)

/*
 * Control register bit MASKS
 */
/* Enable Remote Message present Empty Interrupt ( 1->0 ) */
#define RMPIE_MASK                  (0x01U << 0U)
/* Enable Message present Interrupt ( 0-1 ), we have a message */
#define MPIE_MASK                   (0x01U << 1U)
/* 0 => no message, 1 => we have a message. We set to 0 when message read. */
#define MP_MASK                     (0x01U << 2U)
/* 0 => no message, 1 => we set to 1 when sending a message */
#define RMP_MASK                    (0x01U << 3U)

/*
 * If defined in mss_sw_config.h, mss_sw_config.h define will take precedence
 */
#ifndef IHC_MESSAGE_SIZE
#define IHC_MESSAGE_SIZE                4U
#endif

#define IHC_MAX_MESSAGE_SIZE            8U

/*
 * Flags
 */
#define MP_BUSY         1U
#define MESSAGE_SENT    0U
#define MESSAGE_RX      1U
#define NO_MESSAGE_RX   0U

typedef union{                       /*!< CORE_IHC_CTL_REG register definition*/
    __IO  uint32_t                        CTL_REG;
    struct
    {
    __IO  uint32_t                       RMPIE                :1;
    __IO  uint32_t                       MPIE                 :1;
    __IO  uint32_t                       MP                   :1;
    __IO  uint32_t                       RMP                  :1;
    __IO  uint32_t                       CLR_RMP_INT          :1;
    __IO   uint32_t                       reserve28           :27;
    } bitfield;
} CORE_IHC_CTL_REG_TypeDef;

/*------------ IHC_IP register definition -----------*/
typedef struct
{
  __IO  uint32_t  version;
  __IO  CORE_IHC_CTL_REG_TypeDef CTR_REG ;                  /*!< Offset: 0x0  */
  __I   uint8_t  mesg_in[IHC_MAX_MESSAGE_SIZE];
  __IO  uint8_t  mesg_out[IHC_MAX_MESSAGE_SIZE];
  __I   uint8_t  gap1[0x80 - (24U)];  /* total size 0x80 */
} IHC_IP_TypeDef;

/*------------ Interrupt concentrator register definition -----------*/
typedef struct
{
  __IO  uint32_t  version;
  __IO  uint32_t  MSG_AVAIL_STAT;
  __IO  uint32_t  MSG_CLEARED_STAT;
  uint32_t  fill;
} IPIS_IP_TypeDef;

/*------------ IHC cluster definition -------=-----------*/
typedef struct
{
    __IO  IHC_IP_TypeDef HART_IHC[5U];           /*!< hart0-4 ihc registers  */
    __IO  IPIS_IP_TypeDef interrupt_concentrator;
} IHC_TypeDef;

#ifndef DEBUG_IHC
#define IHC_HO_BASE             0x44000000UL
#define IHC_H1_BASE             0x45000000UL
#define IHC_H2_BASE             0x46000000UL
#define IHC_H3_BASE             0x47000000UL
#define IHC_H4_BASE             0x48000000UL
#else
#define IHC_HO_BASE             0x08020000UL
#define IHC_H1_BASE             0x08020100UL
#define IHC_H2_BASE             0x08020200UL
#define IHC_H3_BASE             0x08020300UL
#define IHC_H4_BASE             0x08020400UL
#endif

extern IHC_TypeDef             * const IHC_HO;
extern IHC_TypeDef             * const IHC_H1;
extern IHC_TypeDef             * const IHC_H2;
extern IHC_TypeDef             * const IHC_H3;
extern IHC_TypeDef             * const IHC_H4;

#ifdef __cplusplus                       
}
#endif

#endif /* MSS_CORE_IHC_REGS_H_ */
