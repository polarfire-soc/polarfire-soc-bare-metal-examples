/*******************************************************************************
 * Copyright 2019-2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Register bit offsets and masks definitions for CoreSysServices_PF driver.
 */

#ifndef __CORE_SYSSERV_PF_REGISTERS
#define __CORE_SYSSERV_PF_REGISTERS   1

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 * SYS_SERV_CMD (offset 0x04) register details
 */
#define SS_CMD_REG_OFFSET   0x04u

#define SS_CMD_OFFSET   0x04
#define SS_CMD_MASK     0x0000FFFFu
#define SS_CMD_SHIFT    0u

/*------------------------------------------------------------------------------
 * SYS_SERV_STAT (offset 0x08) register details
 */
#define SS_STAT_REG_OFFSET   0x08u

#define SS_STAT_OFFSET   0x08
#define SS_STAT_MASK     0x0000FFFFu
#define SS_STAT_SHIFT    0u

/*------------------------------------------------------------------------------
 * SYS_SERV_REQ (offset 0x0C) register details
 */
#define SS_REQ_REG_OFFSET   0x0Cu


#define SS_REQ_REQ_OFFSET                   0x0Cu
#define SS_REQ_REQ_MASK                     0x00000001UL
#define SS_REQ_REQ_SHIFT                    0u

#define SS_REQ_ABUSY_OFFSET                 0x0Cu
#define SS_REQ_ABUSY_MASK                   0x00000002UL
#define SS_REQ_ABUSY_SHIFT                  1u

#define SS_REQ_NABUSY_OFFSET                0x0Cu
#define SS_REQ_NABUSY_MASK                  0x00000004UL
#define SS_REQ_NABUSY_SHIFT                 2u

#define SS_REQ_SSBUSY_OFFSET                0x0Cu
#define SS_REQ_SSBUSY_MASK                  0x00000008UL
#define SS_REQ_SSBUSY_SHIFT                 3u

#define SS_REQ_AREQ_OFFSET                  0x0Cu
#define SS_REQ_AREQ_MASK                    0x00000010UL
#define SS_REQ_AREQ_SHIFT                   4u

#define SS_REQ_NAREQ_OFFSET                 0x0Cu
#define SS_REQ_NAREQ_MASK                   0x00000020UL
#define SS_REQ_NAREQ_SHIFT                  5u
/*------------------------------------------------------------------------------
 * MBX_ECCSTATUS (offset 0x10) register details
 */
#define MBX_ECCSTATUS_REG_OFFSET   0x10u

#define MBX_ECCSTATUS_OFFSET   0x10
#define MBX_ECCSTATUS_MASK     0x03u
#define MBX_ECCSTATUS_SHIFT    0u


/*------------------------------------------------------------------------------
 * MBX_WCNT (offset 0x14) register details
 */
#define MBX_WCNT_REG_OFFSET   0x14u

#define MBX_WCNT_OFFSET   0x14
#define MBX_WCNT_MASK     0x000001FFu
#define MBX_WCNT_SHIFT    0u

/*------------------------------------------------------------------------------
 * MBX_RWCNT (offset 0x18) register details
 */
#define MBX_RCNT_REG_OFFSET   0x18u

#define MBX_RCNT_OFFSET   0x18
#define MBX_RCNT_MASK     0x000001FFu
#define MBX_RCNT_SHIFT    0u

/*------------------------------------------------------------------------------
 * MBX_WADRDESC (offset 0x1C) register details
 */
#define MBX_WADDR_REG_OFFSET   0x1Cu

#define MBX_WADDR_OFFSET   0x1C
#define MBX_WADDR_MASK     0x000001FFu
#define MBX_WADDR_SHIFT    0u

/*------------------------------------------------------------------------------
 * MBX_RADRDESC (offset 0x20) register details
 */
#define MBX_RADDR_REG_OFFSET   0x20u

#define MBX_RADDR_OFFSET   0x20
#define MBX_RADDR_MASK     0x000001FFu
#define MBX_RADDR_SHIFT    0u

/*------------------------------------------------------------------------------
 * MBX_WDATA (offset 0x28) register details
 */
#define MBX_WDATA_REG_OFFSET   0x28u

#define MBX_WDATA_OFFSET   0x28
#define MBX_WDATA_MASK     0xFFFFFFFFu
#define MBX_WDATA_SHIFT    0u


/*------------------------------------------------------------------------------
 * MBX_RDATA (offset 0x2C) register details
 */
#define MBX_RDATA_REG_OFFSET   0x2Cu

#define MBX_RDATA_OFFSET   0x2C
#define MBX_RDATA_MASK     0xFFFFFFFFu
#define MBX_RDATA_SHIFT    0u

/*------------------------------------------------------------------------------
 * SS_USER (offset 0x30) register details
 */
#define SS_USER_REG_OFFSET   0x30u

#define SS_USER_BUSY_OFFSET   0x30
#define SS_USER_BUSY_MASK     0x00000001u
#define SS_USER_BUSY_SHIFT    0u

#define SS_USER_RDVLD_OFFSET   0x30
#define SS_USER_RDVLD_MASK     0x00000002u
#define SS_USER_RDVLD_SHIFT    1u

#define SS_USER_CMDERR_OFFSET   0x30
#define SS_USER_CMDERR_MASK     0x00000004u
#define SS_USER_CMDERR_SHIFT    2u

#ifdef __cplusplus
}
#endif

#endif  /* __CORE_SYSSERV_PF_REGISTERS */
