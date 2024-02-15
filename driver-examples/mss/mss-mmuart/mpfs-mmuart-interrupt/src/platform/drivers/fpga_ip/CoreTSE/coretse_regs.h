/*******************************************************************************
 * Copyright 2014 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file coretse_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Register bit offsets and masks definitions for CoreTSE.
 *
 */
#ifndef CORE_TSE_REGS_H_
#define CORE_TSE_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 * MAC Configuration register 2
 */
#define CFG2_PREAM_LEN                      12u

#define CFG2_BYTE_MASK                      ((uint32_t)1u << 9)
#define CFG2_NIBBLE_MASK                    ((uint32_t)1u << 8)

/*------------------------------------------------------------------------------
 * MAC IPG/IFG register
 */
#define IFG_MINIFGENF                       8u
#define IFG_NONBTBIPG                       16u

/*------------------------------------------------------------------------------
 * MAC half-duplex register
 */
#define HALF_DUPLEX_SLOTTIME                0u
#define HALF_DUPLEX_RETX_MAX_OFFSET         12u
#define HALF_DUPLEX_EXCS_DEFER_MASK         ((uint32_t)1u << 16)
#define HALF_DUPLEX_NO_BACKOFF_MASK         ((uint32_t)1u << 17)
#define HALF_DUPLEX_BACKPRES_NOBACKOFF_MASK ((uint32_t)1u << 18)
#define HALF_DUPLEX_ABEB_ENABLE_MASK        ((uint32_t)1u << 19)
#define HALF_DUPLEX_ABEB_TUNC_OFFSET        20u
#define HALF_DUPLEX_SLOTTIME_MASK           ((uint32_t)0x2FF)

/*------------------------------------------------------------------------------
 * MAC MII Management configuration register
 */
#define MII_CLOCK_SELECT_MASK               ((uint32_t)7u)
#define MII_PREAM_SUPRESS_MASK              ((uint32_t)1u << 4)
#define MII_SCAN_AUTO_INC_MASK              ((uint32_t)1u << 5)
#define MII_RESET_MII_MGMT_MASK             ((uint32_t)1u << 31)

/*------------------------------------------------------------------------------
 * DMA descriptor definitions
 */
#define DMA_DESC_EMPTY_FLAG_MASK            ((uint32_t)1u << 31u)
#define DMA_DESC_PKT_SIZE_MASK              0xFFFu

/*------------------------------------------------------------------------------
 * DMA descriptor packet size
 */
#define DMA_PKTCOUNT_MASK                   ((uint32_t)0xFFu << 16)

/*------------------------------------------------------------------------------
 * FIFO control registers definitions
 */
#define FIFO_CFG0_DEFVAL                    0x0000FF00u
#define FIFO_CFG1_DEFVAL                    0x0FFF0000u
#define FIFO_CFG2_DEFVAL                    0x04000180u

#define FIFO_CFG4_DEFVAL                    0x0000FFFFu /*Setting this bit means Drop frame*/
#define FIFO_CFG5_DEFVAL                    0x0007EFEFu /*Setting this bit means Don't Care*/

/*Changed cfg0 and cfg3 after looking at simulation environment.*/
#define FIFO_CFG3_DEFVAL                    0x0680FFFFu

/*------------------------------------------------------------------------------
 * Interface control register
 */
#define INF_STATS_MODULE                    (1u << 2)
#define STATS_COUNTER_CLEAR_MASK            (1u << 3)
#define STATS_COUNTER_AUTO_CLEAR_MASK       (1u << 4)

/*------------------------------------------------------------------------------
 * MAC Test register
 */
#define TEST_PAUSE                          2u

/*------------------------------------------------------------------------------
 * These constants correspond to the Misc Control register(0x1D4) of CoreTSE.
   These constants are used to fill the Misc Control register depending on the
   current link speed.
   These register bits correspond to the TSM_CONTROL[0:1] signals which can
   in-turn be used to generate the appropriate TX-RX clocks in the Libero Design
   depending on the Link Speed.
 */
#define SPEED_10M                           2u
#define SPEED_100M                          1u
#define SPEED_1000M                         0u

/*------------------------------------------------------------------------------
 * CoreTSE register offset, mask and shift definitions
 */
#define CFG1_REG_OFFSET                     0x00u

#define CFG1_TX_EN_OFFSET                   0x00u
#define CFG1_TX_EN_MASK                     0x00000001UL
#define CFG1_TX_EN_SHIFT                    0u

#define CFG1_SYNC_TX_EN_OFFSET              0x00u
#define CFG1_SYNC_TX_EN_MASK                0x00000002UL
#define CFG1_SYNC_TX_EN_SHIFT               1u

#define CFG1_RX_EN_OFFSET                   0x00u
#define CFG1_RX_EN_MASK                     0x00000004UL
#define CFG1_RX_EN_SHIFT                    2u

#define CFG1_SYNC_RX_EN_OFFSET              0x00u
#define CFG1_SYNC_RX_EN_MASK                0x00000008UL
#define CFG1_SYNC_RX_EN_SHIFT               3u

#define CFG1_TX_FCTL_OFFSET                 0x00u
#define CFG1_TX_FCTL_MASK                   0x00000010UL
#define CFG1_TX_FCTL_SHIFT                  4u

#define CFG1_RX_FCTL_OFFSET                 0x00u
#define CFG1_RX_FCTL_MASK                   0x00000020UL
#define CFG1_RX_FCTL_SHIFT                  5u

#define CFG1_LOOPBACK_OFFSET                0x00u
#define CFG1_LOOPBACK_MASK                  0x00000100UL
#define CFG1_LOOPBACK_SHIFT                 8u

#define CFG2_REG_OFFSET                     0x04u

#define CFG2_FDX_OFFSET                     0x04u
#define CFG2_FDX_MASK                       0x00000001UL
#define CFG2_FDX_SHIFT                      0u

#define CFG2_CRC_EN_OFFSET                  0x04u
#define CFG2_CRC_EN_MASK                    0x00000002UL
#define CFG2_CRC_EN_SHIFT                   1u

#define CFG2_PAD_CRC_EN_OFFSET              0x04u
#define CFG2_PAD_CRC_EN_MASK                0x00000004UL
#define CFG2_PAD_CRC_EN_SHIFT               2u

#define CFG2_LEN_CHECK_OFFSET               0x04u
#define CFG2_LEN_CHECK_MASK                 0x00000010UL
#define CFG2_LEN_CHECK_SHIFT                4u

#define CFG2_HUGE_FRAME_EN_OFFSET           0x04u
#define CFG2_HUGE_FRAME_EN_MASK             0x00000020UL
#define CFG2_HUGE_FRAME_EN_SHIFT            5u

#define CFG2_IF_MODE_OFFSET                 0x04u
#define CFG2_IF_MODE_MASK                   0x00000300UL
#define CFG2_IF_MODE_SHIFT                  8u

#define CFG2_PREAM_LEN_OFFSET               0x04u
#define CFG2_PREAM_LEN_MASK                 0x0000F000UL
#define CFG2_PREAM_LEN_SHIFT                12u

#define IFG_REG_OFFSET                      0x08u

#define IFG_BTBIPG_OFFSET                   0x08u
#define IFG_BTBIPG_MASK                     0x0000003FUL
#define IFG_BTBIPG_SHIFT                    0u

#define IFG_MINIFGENF_OFFSET                0x08u
#define IFG_MINIFGENF_MASK                  0x0000FF00UL
#define IFG_MINIFGENF_SHIFT                 8u

#define IFG_NONBTBIPG_P2_OFFSET             0x08u
#define IFG_NONBTBIPG_P2_MASK               0x007F0000UL
#define IFG_NONBTBIPG_P2_SHIFT              16u

#define IFG_NONBTBIPG_P1_OFFSET             0x08u
#define IFG_NONBTBIPG_P1_MASK               0x7F000000UL
#define IFG_NONBTBIPG_P1_SHIFT              24u

#define HDX_REG_OFFSET                      0x0Cu

#define HDX_COL_WIN_OFFSET                  0x0Cu
#define HDX_COL_WIN_MASK                    0x000003FFUL
#define HDX_COL_WIN_SHIFT                   0u

#define HDX_RETX_MAX_OFFSET                 0x0Cu
#define HDX_RETX_MAX_MASK                   0x0000F000UL
#define HDX_RETX_MAX_SHIFT                  12u

#define HDX_EX_DEF_OFFSET                   0x0Cu
#define HDX_EX_DEF_MASK                     0x00010000UL
#define HDX_EX_DEF_SHIFT                    16u

#define HDX_NOBACKOF_OFFSET                 0x0Cu
#define HDX_NOBACKOF_MASK                   0x00020000UL
#define HDX_NOBACKOF_SHIFT                  17u

#define HDX_BP_NOBACKOF_OFFSET              0x0Cu
#define HDX_BP_NOBACKOF_MASK                0x00040000UL
#define HDX_BP_NOBACKOF_SHIFT               18u

#define HDX_ALTB_EXP_BACK_EN_OFFSET         0x0Cu
#define HDX_ALTB_EXP_BACK_EN_MASK           0x00080000UL
#define HDX_ALTB_EXP_BACK_EN_SHIFT          19u

#define HDX_ALTB_EXP_BACK_TRUN_OFFSET       0x0Cu
#define HDX_ALTB_EXP_BACK_TRUN_MASK         0x00F00000UL
#define HDX_ALTB_EXP_BACK_TRUN_SHIFT        20u

#define FREMLEN_REG_OFFSET                  0x10u

#define FREMLEN_MAX_FL_OFFSET               0x10u
#define FREMLEN_MAX_FL_MASK                 0x0000FFFFUL
#define FREMLEN_MAX_FL_SHIFT                0u

#define CFEP_REG_OFFSET                     0x14u

#define CFEP_CFEP_OFFSET                    0x14u
#define CFEP_CFEP_MASK                      0x0000FFFFUL
#define CFEP_CFEP_SHIFT                     0u

#define CFP_REG_OFFSET                      0x18u

#define CFP_CFPT_OFFSET                     0x18u
#define CFP_CFPT_MASK                       0x0000FFFFUL
#define CFP_CFPT_SHIFT                      0u

#define TEST_REG_OFFSET                     0x1Cu

#define MIIMGMT_REG_OFFSET                  0x20u

#define MIIMGMT_CLK_SEL_OFFSET              0x20u
#define MIIMGMT_CLK_SEL_MASK                0x00000007UL
#define MIIMGMT_CLK_SEL_SHIFT               0u

#define MIIMGMT_PREAM_SUPRESS_OFFSET        0x20u
#define MIIMGMT_PREAM_SUPRESS_MASK          0x00000010UL
#define MIIMGMT_PREAM_SUPRESS_SHIFT         4u

#define MIIMGMT_SCAN_AUTO_INC_OFFSET        0x20u
#define MIIMGMT_SCAN_AUTO_INC_MASK          0x00000020UL
#define MIIMGMT_SCAN_AUTO_INC_SHIFT         5u

#define MIIMGMT_RESET_MII_MGMT_OFFSET       0x20u
#define MIIMGMT_RESET_MII_MGMT_MASK         0x80000000UL
#define MIIMGMT_RESET_MII_MGMT_SHIFT        31u

#define MIIMCMD_REG_OFFSET                  0x24u

#define MIIMCMD_READ_CYCLE_OFFSET           0x24u
#define MIIMCMD_READ_CYCLE_MASK             0x00000001UL
#define MIIMCMD_READ_CYCLE_SHIFT            0u

#define MIIMCMD_SCAN_CYCLE_OFFSET           0x24u
#define MIIMCMD_SCAN_CYCLE_MASK             0x00000002UL
#define MIIMCMD_SCAN_CYCLE_SHIFT            1u

#define MIIMADDR_REG_OFFSET                 0x28u

#define MIIMADDR_REG_ADDR_OFFSET            0x28u
#define MIIMADDR_REG_ADDR_MASK              0x0000001FUL
#define MIIMADDR_REG_ADDR_SHIFT             0u

#define MIIMADDR_PHY_ADDR_OFFSET            0x28u
#define MIIMADDR_PHY_ADDR_MASK              0x00001F00UL
#define MIIMADDR_PHY_ADDR_SHIFT             8u

#define MIIMCTRL_REG_OFFSET                 0x2Cu

#define MIIMCTRL_PHY_CTRL_OFFSET            0x2Cu
#define MIIMCTRL_PHY_CTRL_MASK              0x0000FFFFUL
#define MIIMCTRL_PHY_CTRL_SHIFT             0u

#define MIIMSTATUS_REG_OFFSET               0x30u

#define MIIMSTATUS_PHY_STATUS_OFFSET        0x30u
#define MIIMSTATUS_PHY_STATUS_MASK          0x0000FFFFUL
#define MIIMSTATUS_PHY_STATUS_SHIFT         0u

#define MIIMIND_REG_OFFSET                  0x34u

#define MIIMIND_BUSY_OFFSET                 0x34u
#define MIIMIND_BUSY_MASK                   0x00000001UL
#define MIIMIND_BUSY_SHIFT                  0u

#define MIIMIND_SCANNING_OFFSET             0x34u
#define MIIMIND_SCANNING_MASK               0x00000002UL
#define MIIMIND_SCANNING_SHIFT              1u

#define MIIMIND_NV_OFFSET                   0x34u
#define MIIMIND_NV_MASK                     0x00000004UL
#define MIIMIND_NV_SHIFT                    2u

#define IFC_REG_OFFSET                      0x38u

#define IFC_STATS_EN_OFFSET                 0x38u
#define IFC_STATS_EN_MASK                   0x00000004UL
#define IFC_STATS_EN_SHIFT                  2u

#define IFC_STATS_CLR_ALL_OFFSET            0x38u
#define IFC_STATS_CLR_ALL_MASK              0x00000008UL
#define IFC_STATS_CLR_ALL_SHIFT             3u

#define IFC_STATS_AUTO_CLR_EN_OFFSET        0x38u
#define IFC_STATS_AUTO_CLR_EN_MASK          0x00000010UL
#define IFC_STATS_AUTO_CLR_EN_SHIFT         4u

#define IFC_MCXWOL_ST_CLR_OFFSET            0x38u
#define IFC_MCXWOL_ST_CLR_MASK              0x00000020UL
#define IFC_MCXWOL_ST_CLR_SHIFT             5u

#define IFC_MCXWOL_MAGIC_EN_OFFSET          0x38u
#define IFC_MCXWOL_MAGIC_EN_MASK            0x00000040UL
#define IFC_MCXWOL_MAGIC_EN_SHIFT           6u

#define IFC_MCXWOL_UMATCH_EN_OFFSET         0x38u
#define IFC_MCXWOL_UMATCH_EN_MASK           0x00000080UL
#define IFC_MCXWOL_UMATCH_EN_SHIFT          7u

#define IFS_REG_OFFSET                      0x3Cu

#define IFS_LINK_FL_OFFSET                  0x3Cu
#define IFS_LINK_FL_MASK                    0x00000008UL
#define IFS_LINK_FL_SHIFT                   3u

#define IFS_EXC_DEF_OFFSET                  0x3Cu
#define IFS_EXC_DEF_MASK                    0x00000200UL
#define IFS_EXC_DEF_SHIFT                   9u

#define IFS_WOL_DET_OFFSET                  0x3Cu
#define IFS_WOL_DET_MASK                    0x00000400UL
#define IFS_WOL_DET_SHIFT                   10u

#define STADDRL_REG_OFFSET                  0x40u

#define STADDRH_REG_OFFSET                  0x44u

#define FIFOCFG0_REG_OFFSET                 0x48u

#define FIFOCFG0_WMM_RST_OFFSET             0x48u
#define FIFOCFG0_WMM_RST_MASK               0x00000001UL
#define FIFOCFG0_WMM_RST_SHIFT              0u

#define FIFOCFG0_RSYS_RST_OFFSET            0x48u
#define FIFOCFG0_RSYS_RST_MASK              0x00000002UL
#define FIFOCFG0_RSYS_RST_SHIFT             1u

#define FIFOCFG0_RFAB_RST_OFFSET            0x48u
#define FIFOCFG0_RFAB_RST_MASK              0x00000004UL
#define FIFOCFG0_RFAB_RST_SHIFT             2u

#define FIFOCFG0_TSYS_RST_OFFSET            0x48u
#define FIFOCFG0_TSYS_RST_MASK              0x00000008UL
#define FIFOCFG0_TSYS_RST_SHIFT             3u

#define FIFOCFG0_TFAB_RST_OFFSET            0x48u
#define FIFOCFG0_TFAB_RST_MASK              0x00000010UL
#define FIFOCFG0_TFAB_RST_SHIFT             4u

#define FIFOCFG0_WMM_EN_OFFSET              0x48u
#define FIFOCFG0_WMM_EN_MASK                0x00000100UL
#define FIFOCFG0_WMM_EN_SHIFT               8u

#define FIFOCFG0_RSYS_EN_OFFSET             0x48u
#define FIFOCFG0_RSYS_EN_MASK               0x00000200UL
#define FIFOCFG0_RSYS_EN_SHIFT              9u

#define FIFOCFG0_RFAB_EN_OFFSET             0x48u
#define FIFOCFG0_RFAB_EN_MASK               0x00000400UL
#define FIFOCFG0_RFAB_EN_SHIFT              10u

#define FIFOCFG0_TSYS_EN_OFFSET             0x48u
#define FIFOCFG0_TSYS_EN_MASK               0x00000800UL
#define FIFOCFG0_TSYS_EN_SHIFT              11u

#define FIFOCFG0_TFAB_EN_OFFSET             0x48u
#define FIFOCFG0_TFAB_EN_MASK               0x00001000UL
#define FIFOCFG0_TFAB_EN_SHIFT              12u

#define FIFOCFG0_WMM_RPLY_OFFSET            0x48u
#define FIFOCFG0_WMM_RPLY_MASK              0x00010000UL
#define FIFOCFG0_WMM_RPLY_SHIFT             16u

#define FIFOCFG0_RSYS_RPLY_OFFSET           0x48u
#define FIFOCFG0_RSYS_RPLY_MASK             0x00020000UL
#define FIFOCFG0_RSYS_RPLY_SHIFT            17u

#define FIFOCFG0_RFAB_RPLY_OFFSET           0x48u
#define FIFOCFG0_RFAB_RPLY_MASK             0x00040000UL
#define FIFOCFG0_RFAB_RPLY_SHIFT            18u

#define FIFOCFG0_TSYS_RPLY_OFFSET           0x48u
#define FIFOCFG0_TSYS_RPLY_MASK             0x00080000UL
#define FIFOCFG0_TSYS_RPLY_SHIFT            19u

#define FIFOCFG0_TFAB_RPLY_OFFSET           0x48u
#define FIFOCFG0_TFAB_RPLY_MASK             0x00100000UL
#define FIFOCFG0_TFAB_RPLY_SHIFT            20u

#define FIFOCFG1_REG_OFFSET                 0x4Cu

#define FIFOCFG1_CFGXOFFRTX_OFFSET          0x4Cu
#define FIFOCFG1_CFGXOFFRTX_MASK            0x0000FFFFUL
#define FIFOCFG1_CFGXOFFRTX_SHIFT           0u

#define FIFOCFG1_CFGSRTH_OFFSET             0x4Cu
#define FIFOCFG1_CFGSRTH_MASK               0x0FFF0000UL
#define FIFOCFG1_CFGSRTH_SHIFT              16u

#define FIFOCFG2_REG_OFFSET                 0x50u

#define FIFOCFG2_CFGLWM_OFFSET              0x50u
#define FIFOCFG2_CFGLWM_MASK                0x00001FFFUL
#define FIFOCFG2_CFGLWM_SHIFT               0u

#define FIFOCFG2_CFGHWM_OFFSET              0x50u
#define FIFOCFG2_CFGHWM_MASK                0x1FFF0000UL
#define FIFOCFG2_CFGHWM_SHIFT               16u

#define FIFOCFG3_REG_OFFSET                 0x54u

#define FIFOCFG3_CFGFTTH_OFFSET             0x54u
#define FIFOCFG3_CFGFTTH_MASK               0x00000FFFUL
#define FIFOCFG3_CFGFTTH_SHIFT              0u

#define FIFOCFG3_CFGHWMFT_OFFSET            0x54u
#define FIFOCFG3_CFGHWMFT_MASK              0x0FFF0000UL
#define FIFOCFG3_CFGHWMFT_SHIFT             16u

#define FIFOCFG4_REG_OFFSET                 0x58u

#define FIFOCFG4_HSTFLTRFRM_OFFSET          0x58u
#define FIFOCFG4_HSTFLTRFRM_MASK            0x0003FFFFUL
#define FIFOCFG4_HSTFLTRFRM_SHIFT           0u

#define FIFOCFG5_REG_OFFSET                 0x5Cu

#define FIFOCFG5_HSTFLTRFRMDC_OFFSET        0x5Cu
#define FIFOCFG5_HSTFLTRFRMDC_MASK          0x0003FFFFUL
#define FIFOCFG5_HSTFLTRFRMDC_SHIFT         0u

#define FIFOCFG5_HSTDRPLT64_OFFSET          0x5Cu
#define FIFOCFG5_HSTDRPLT64_MASK            0x00040000UL
#define FIFOCFG5_HSTDRPLT64_SHIFT           18u

#define FIFOCFG5_CFGBYTEMODE_OFFSET         0x5Cu
#define FIFOCFG5_CFGBYTEMODE_MASK           0x00080000UL
#define FIFOCFG5_CFGBYTEMODE_SHIFT          19u

#define FIFOCFG5_HSTSRFULLCLR_OFFSET        0x5Cu
#define FIFOCFG5_HSTSRFULLCLR_MASK          0x00100000UL
#define FIFOCFG5_HSTSRFULLCLR_SHIFT         20u

#define FIFOCFG5_HSFULL_OFFSET              0x5Cu
#define FIFOCFG5_HSFULL_MASK                0x00200000UL
#define FIFOCFG5_HSFULL_SHIFT               21u

#define FIFOCFG5_CFGHDPLX_OFFSET            0x5Cu
#define FIFOCFG5_CFGHDPLX_MASK              0x00400000UL
#define FIFOCFG5_CFGHDPLX_SHIFT             22u

#define FIFORAM0_REG_OFFSET                 0x60u

#define FIFORAM0_HSTTRAMWADX_OFFSET         0x60u
#define FIFORAM0_HSTTRAMWADX_MASK           0x00001FFFUL
#define FIFORAM0_HSTTRAMWADX_SHIFT          0u

#define FIFORAM0_HSTTRAMWDAT_OFFSET         0x60u
#define FIFORAM0_HSTTRAMWDAT_MASK           0x00FF0000UL
#define FIFORAM0_HSTTRAMWDAT_SHIFT          16u

#define FIFORAM0_HSTTRAMWACK_OFFSET         0x60u
#define FIFORAM0_HSTTRAMWACK_MASK           0x40000000UL
#define FIFORAM0_HSTTRAMWACK_SHIFT          30u

#define FIFORAM0_HSTTRAMWREQ_OFFSET         0x60u
#define FIFORAM0_HSTTRAMWREQ_MASK           0x80000000UL
#define FIFORAM0_HSTTRAMWREQ_SHIFT          31u

#define FIFORAM1_REG_OFFSET                 0x64u

#define FIFORAM2_REG_OFFSET                 0x68u

#define FIFORAM2_HSTTRAMRADX_OFFSET         0x68u
#define FIFORAM2_HSTTRAMRADX_MASK           0x00001FFFUL
#define FIFORAM2_HSTTRAMRADX_SHIFT          0u

#define FIFORAM2_HSTTRAMRDAT_OFFSET         0x68u
#define FIFORAM2_HSTTRAMRDAT_MASK           0x00FF0000UL
#define FIFORAM2_HSTTRAMRDAT_SHIFT          16u

#define FIFORAM2_HSTTRAMRACK_OFFSET         0x68u
#define FIFORAM2_HSTTRAMRACK_MASK           0x40000000UL
#define FIFORAM2_HSTTRAMRACK_SHIFT          30u

#define FIFORAM2_HSTTRAMRREQ_OFFSET         0x68u
#define FIFORAM2_HSTTRAMRREQ_MASK           0x80000000UL
#define FIFORAM2_HSTTRAMRREQ_SHIFT          31u

#define FIFORAM3_REG_OFFSET                 0x6Cu

#define FIFORAM4_REG_OFFSET                 0x70u

#define FIFORAM4_HSTTRAMWADX_OFFSET         0x70u
#define FIFORAM4_HSTTRAMWADX_MASK           0x00003FFFUL
#define FIFORAM4_HSTTRAMWADX_SHIFT          0u

#define FIFORAM4_HSTRRAMWDAT_OFFSET         0x70u
#define FIFORAM4_HSTRRAMWDAT_MASK           0x00FF0000UL
#define FIFORAM4_HSTRRAMWDAT_SHIFT          16u

#define FIFORAM4_HSTRRAMWACK_OFFSET         0x70u
#define FIFORAM4_HSTRRAMWACK_MASK           0x40000000UL
#define FIFORAM4_HSTRRAMWACK_SHIFT          30u

#define FIFORAM4_HSTRRAMWREQ_OFFSET         0x70u
#define FIFORAM4_HSTRRAMWREQ_MASK           0x80000000UL
#define FIFORAM4_HSTRRAMWREQ_SHIFT          31u

#define FIFORAM5_REG_OFFSET                 0x74u

#define FIFORAM6_REG_OFFSET                 0x78u

#define FIFORAM6_HSTRRAMRADX_OFFSET         0x78u
#define FIFORAM6_HSTRRAMRADX_MASK           0x00003FFFUL
#define FIFORAM6_HSTRRAMRADX_SHIFT          0u

#define FIFORAM6_HSTRRAMRDAT_OFFSET         0x78u
#define FIFORAM6_HSTRRAMRDAT_MASK           0x00FF0000UL
#define FIFORAM6_HSTRRAMRDAT_SHIFT          16u

#define FIFORAM6_HSTRRAMRACK_OFFSET         0x78u
#define FIFORAM6_HSTRRAMRACK_MASK           0x40000000UL
#define FIFORAM6_HSTRRAMRACK_SHIFT          30u

#define FIFORAM6_HSTRRAMRREQ_OFFSET         0x78u
#define FIFORAM6_HSTRRAMRREQ_MASK           0x80000000UL
#define FIFORAM6_HSTRRAMRREQ_SHIFT          31u

#define FIFORAM7_REG_OFFSET                 0x7Cu

#define TR64_REG_OFFSET                     0x80u
#define TR127_REG_OFFSET                    0x84u
#define TR255_REG_OFFSET                    0x88u
#define TR511_REG_OFFSET                    0x8Cu
#define TR1K_REG_OFFSET                     0x90u
#define TRMAX_REG_OFFSET                    0x94u
#define TRMGV_REG_OFFSET                    0x98u
#define RBYT_REG_OFFSET                     0x9Cu
#define RPKT_REG_OFFSET                     0xA0u
#define RFCS_REG_OFFSET                     0xA4u
#define RMCA_REG_OFFSET                     0xA8u
#define RBCA_REG_OFFSET                     0xACu
#define RXCF_REG_OFFSET                     0xB0u
#define RXPF_REG_OFFSET                     0xB4u
#define RXUO_REG_OFFSET                     0xB8u /*Unknown Opco*/
#define RALN_REG_OFFSET                     0xBCu /*Unknown Err*/
#define RFLR_REG_OFFSET                     0xC0u
#define RCDE_REG_OFFSET                     0xC4u
#define RCSE_REG_OFFSET                     0xC8u
#define RUND_REG_OFFSET                     0xCCu
#define ROVR_REG_OFFSET                     0xD0u
#define RFRG_REG_OFFSET                     0xD4u
#define RJBR_REG_OFFSET                     0xD8u
#define RDRP_REG_OFFSET                     0xDCu
#define TBYT_REG_OFFSET                     0xE0u
#define TPKT_REG_OFFSET                     0xE4u
#define TMCA_REG_OFFSET                     0xE8u
#define TBCA_REG_OFFSET                     0xECu
#define TXPF_REG_OFFSET                     0xF0u
#define TDFR_REG_OFFSET                     0xF4u
#define TEDF_REG_OFFSET                     0xF8u
#define TSCL_REG_OFFSET                     0xFCu
#define TMCL_REG_OFFSET                     0x100u
#define TLCL_REG_OFFSET                     0x104u
#define TXCL_REG_OFFSET                     0x108u
#define TNCL_REG_OFFSET                     0x10Cu
#define TPFH_REG_OFFSET                     0x110u /*0x110 -- Not used but defined for continuity*/
#define TDRP_REG_OFFSET                     0x114u
#define TJBR_REG_OFFSET                     0x118u
#define TFCS_REG_OFFSET                     0x11Cu
#define TXCF_REG_OFFSET                     0x120u
#define TOVR_REG_OFFSET                     0x124u
#define TUND_REG_OFFSET                     0x128u
#define TFRG_REG_OFFSET                     0x12Cu

#define CAR1_REG_OFFSET                     0x130u
#define CAR2_REG_OFFSET                     0x134u
#define CAM1_REG_OFFSET                     0x138u
#define CAM2_REG_OFFSET                     0x13Cu

#define DMATXCTRL_REG_OFFSET                0x180u

#define DMATXCTRL_TX_EN_OFFSET              0x180u
#define DMATXCTRL_TX_EN_MASK                0x00000001UL
#define DMATXCTRL_TX_EN_SHIFT               0u

#define DMATXCTRL_TXIRQ_COAL_CNT_OFFSET     0x180u
#define DMATXCTRL_TXIRQ_COAL_CNT_MASK       0x0000FF00UL
#define DMATXCTRL_TXIRQ_COAL_CNT_SHIFT      8u

#define DMATXDESC_REG_OFFSET                0x184u

#define DMATXSTATUS_REG_OFFSET              0x188u

#define DMATXSTATUS_TXPKT_SENT_OFFSET       0x188u
#define DMATXSTATUS_TXPKT_SENT_MASK         0x00000001UL
#define DMATXSTATUS_TXPKT_SENT_SHIFT        0u

#define DMATXSTATUS_TXPKT_UR_OFFSET         0x188u
#define DMATXSTATUS_TXPKT_UR_MASK           0x00000002UL
#define DMATXSTATUS_TXPKT_UR_SHIFT          1u

#define DMATXSTATUS_BUSERR_OFFSET           0x188u
#define DMATXSTATUS_BUSERR_MASK             0x00000008UL
#define DMATXSTATUS_BUSERR_SHIFT            3u

#define DMATXSTATUS_TXPKT_CNT_OFFSET        0x188u
#define DMATXSTATUS_TXPKT_CNT_MASK          0x00FF0000UL
#define DMATXSTATUS_TXPKT_CNT_SHIFT         16u

#define DMARXCTRL_REG_OFFSET                0x18Cu

#define DMARXCTRL_RX_EN_OFFSET              0x18Cu
#define DMARXCTRL_RX_EN_MASK                0x00000001UL
#define DMARXCTRL_RX_EN_SHIFT               0u

#define DMARXCTRL_RXIRQ_COAL_CNT_OFFSET     0x18Cu
#define DMARXCTRL_RXIRQ_COAL_CNT_MASK       0x0000FF00UL
#define DMARXCTRL_RXIRQ_COAL_CNT_SHIFT      8u

#define DMARXDESC_REG_OFFSET                0x190u

#define DMARXSTATUS_REG_OFFSET              0x194u

#define DMARXSTATUS_RXPKT_RCVD_OFFSET       0x194u
#define DMARXSTATUS_RXPKT_RCVD_MASK         0x00000001UL
#define DMARXSTATUS_RXPKT_RCVD_SHIFT        0u

#define DMARXSTATUS_RXPKT_OVR_OFFSET        0x194u
#define DMARXSTATUS_RXPKT_OVR_MASK          0x00000004UL
#define DMARXSTATUS_RXPKT_OVR_SHIFT         2u

#define DMARXSTATUS_BUSERR_OFFSET           0x194u
#define DMARXSTATUS_BUSERR_MASK             0x00000008UL
#define DMARXSTATUS_BUSERR_SHIFT            3u

#define DMARXSTATUS_RXPKT_CNT_OFFSET        0x194u
#define DMARXSTATUS_RXPKT_CNT_MASK          0x00FF0000UL
#define DMARXSTATUS_RXPKT_CNT_SHIFT         16u

#define DMAINTRMASK_REG_OFFSET              0x198u

#define DMAINTRMASK_TXPKT_SENT_OFFSET       0x198u
#define DMAINTRMASK_TXPKT_SENT_MASK         0x00000001UL
#define DMAINTRMASK_TXPKT_SENT_SHIFT        0u

#define DMAINTRMASK_TX_UR_OFFSET            0x198u
#define DMAINTRMASK_TX_UR_MASK              0x00000002UL
#define DMAINTRMASK_TX_UR_SHIFT             1u

#define DMAINTRMASK_TX_BUSERR_OFFSET        0x198u
#define DMAINTRMASK_TX_BUSERR_MASK          0x00000008UL
#define DMAINTRMASK_TX_BUSERR_SHIFT         3u

#define DMAINTRMASK_RXPKT_RCVD_OFFSET       0x198u
#define DMAINTRMASK_RXPKT_RCVD_MASK         0x00000010UL
#define DMAINTRMASK_RXPKT_RCVD_SHIFT        4u

#define DMAINTRMASK_RXPKT_OVR_OFFSET        0x198u
#define DMAINTRMASK_RXPKT_OVR_MASK          0x00000040UL
#define DMAINTRMASK_RXPKT_OVR_SHIFT         6u

#define DMAINTRMASK_RX_BUSERR_OFFSET        0x198u
#define DMAINTRMASK_RX_BUSERR_MASK          0x00000080UL
#define DMAINTRMASK_RX_BUSERR_SHIFT         7u

#define DMAINTRMASK_TXIRQ_COAL_OFFSET       0x198u
#define DMAINTRMASK_TXIRQ_COAL_MASK         0x00000100UL
#define DMAINTRMASK_TXIRQ_COAL_SHIFT        8u

#define DMAINTRMASK_RXIRQ_COAL_OFFSET       0x198u
#define DMAINTRMASK_RXIRQ_COAL_MASK         0x00000200UL
#define DMAINTRMASK_RXIRQ_COAL_SHIFT        9u

#define DMAINTR_REG_OFFSET                  0x19Cu

#define DMAINTR_TXPKT_SENT_OFFSET           0x19Cu
#define DMAINTR_TXPKT_SENT_MASK             0x00000001UL
#define DMAINTR_TXPKT_SENT_SHIFT            0u

#define DMAINTR_TX_UR_OFFSET                0x19Cu
#define DMAINTR_TX_UR_MASK                  0x00000002UL
#define DMAINTR_TX_UR_SHIFT                 1u

#define DMAINTR_TX_BUSERR_OFFSET            0x19Cu
#define DMAINTR_TX_BUSERR_MASK              0x00000008UL
#define DMAINTR_TX_BUSERR_SHIFT             3u

#define DMAINTR_RXPKT_RCVD_OFFSET           0x19Cu
#define DMAINTR_RXPKT_RCVD_MASK             0x00000010UL
#define DMAINTR_RXPKT_RCVD_SHIFT            4u

#define DMAINTR_RXPKT_OVR_OFFSET            0x19Cu
#define DMAINTR_RXPKT_OVR_MASK              0x00000040UL
#define DMAINTR_RXPKT_OVR_SHIFT             6u

#define DMAINTR_RX_BUSERR_OFFSET            0x19Cu
#define DMAINTR_RX_BUSERR_MASK              0x00000080UL
#define DMAINTR_RX_BUSERR_SHIFT             7u

#define DMAINTR_TXIRQ_COAL_OFFSET           0x19Cu
#define DMAINTR_TXIRQ_COAL_MASK             0x00000100UL
#define DMAINTR_TXIRQ_COAL_SHIFT            8u

#define DMAINTR_RXIRQ_COAL_OFFSET           0x19Cu
#define DMAINTR_RXIRQ_COAL_MASK             0x00000200UL
#define DMAINTR_RXIRQ_COAL_SHIFT            9u

#define FPC_REG_OFFSET                      0x1C0u

#define FPC_BROADCAST_OFFSET                0x1C0u
#define FPC_BROADCAST_MASK                  0x00000001UL
#define FPC_BROADCAST_SHIFT                 0u

#define FPC_MULTICAST_OFFSET                0x1C0u
#define FPC_MULTICAST_MASK                  0x00000002UL
#define FPC_MULTICAST_SHIFT                 1u

#define FPC_UNICAST_OFFSET                  0x1C0u
#define FPC_UNICAST_MASK                    0x00000004UL
#define FPC_UNICAST_SHIFT                   2u

#define FPC_PROMISCUOUS_OFFSET              0x1C0u
#define FPC_PROMISCUOUS_MASK                0x00000008UL
#define FPC_PROMISCUOUS_SHIFT               3u

#define FPC_HASH_UNICAST_OFFSET             0x1C0u
#define FPC_HASH_UNICAST_MASK               0x00000010UL
#define FPC_HASH_UNICAST_SHIFT              4u

#define FPC_HASH_MULTICAST_OFFSET           0x1C0u
#define FPC_HASH_MULTICAST_MASK             0x00000020UL
#define FPC_HASH_MULTICAST_SHIFT            5u

#define HASHTAB0_REG_OFFSET                 0x1C4u
#define HASHTAB1_REG_OFFSET                 0x1C8u
#define HASHTAB2_REG_OFFSET                 0x1CCu
#define HASHTAB3_REG_OFFSET                 0x1D0u
#define MISCC_REG_OFFSET                    0x1D4u

#ifdef __cplusplus
}
#endif

#endif /* CORE_TSE_REGS_H_ */
