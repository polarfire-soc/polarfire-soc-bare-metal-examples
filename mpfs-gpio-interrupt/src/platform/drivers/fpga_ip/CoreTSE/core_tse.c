
/*******************************************************************************
 * Copyright 2014 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_tse.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreTSE 10/100/1000 Mbps Ethernet MAC bare metal software driver implementation.
 *
 */

#include "coretse_regs.h"
#include "core_tse.h"
#include "phy.h"
#include "crc32.h"

#include "hal/hal.h"
#include "hal/hal_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/* Preprocessor Macros                                                    */
/**************************************************************************/
#define NULL_POINTER           0

#define MAC_ADDRESS_LENGTH     6u

#define DUPLEX_MODE_MASK       0x01u

#define BROADCAST_COMPARE_BITS 0xFFu
#define MULTICAST_ADDRESS_MASK 0x01u
#define MULTICAST_ADDRESS      0x01u
#define UNICAST_ADDRESS        0x00u

#if !defined(NDEBUG)
#define IS_STATE(x)        (((x) == TSE_ENABLE) || ((x) == TSE_DISABLE))

#define IS_WORD_ALIGNED(x) ((uint32_t)0 == ((uint32_t)x & (uint32_t)3))
#endif /* NDEBUG */

#define INVALID_INDEX                    (-1)

#define DMA_TX_DISABLED                  0u
#define DMA_TX_ENABLED                   1u

#define PHY_ADDRESS_MIN                  0
#define PHY_ADDRESS_MAX                  31

#define TSE_MII                          0x00
#define TSE_GMII                         0x01
#define TSE_SGMII                        0x02
#define TSE_1000BASEX                    0x03
#define TSE_RGMII                        0x04
#define TSE_RMII                         0x05

/*******************************************************************************
 * MAC interrupt definitions
 */
#define TSE_TXPKTSENT_IRQ                0u
#define TSE_TXUNDRRUN_IRQ                1u
#define TSE_TXBUSERR_IRQ                 3u
#define TSE_RXPKTRCVD_IRQ                4u
#define TSE_RXOVRFLOW_IRQ                6u
#define TSE_RXBUSERR_IRQ                 7u
#define TSE_INVALID_IRQ                  255u

#define TSE_TXPKTSENT_IRQ_MASK           0x01u
#define TSE_RXPKTRCVD_IRQ_MASK           ((uint32_t)0x01u << TSE_RXPKTRCVD_IRQ)

#define MSGMII_PHY_STATUS                0x0F
#define MSGMII_1000BASEX_FD              0x8000
#define MSGMII_1000BASEX_HD              0x4000

#define MSGMII_AUTO_NEGOTIATION_COMPLETE 0x0020u
/**************************************************************************/
/* Private Functions declarations                                         */
/**************************************************************************/
static void mac_reset(tse_instance_t *this_tse);
static void config_mac_hw(tse_instance_t *this_tse, const tse_cfg_t *cfg);
static void tx_desc_ring_init(tse_instance_t *this_tse);
static void rx_desc_ring_init(tse_instance_t *this_tse);
static void assign_station_addr(tse_instance_t *this_tse, const uint8_t mac_addr[6]);
static void rxpkt_handler(tse_instance_t *this_tse);
static void txpkt_handler(tse_instance_t *this_tse);
static void update_mac_cfg(tse_instance_t *this_tse, uint8_t phy_addr);
static uint8_t phy_probe(tse_instance_t *this_tse);

#if ((TSE_PHY_INTERFACE == TSE_SGMII) || (TSE_PHY_INTERFACE == TSE_1000BASEX))
static void msgmii_init(tse_instance_t *this_tse);
static void msgmii_autonegotiate(tse_instance_t *this_tse);
#endif

#if (TSE_PHY_INTERFACE == TSE_1000BASEX)
static uint8_t msgmii_get_link_status(tse_instance_t *this_tse,
                                      tse_speed_t *speed,
                                      uint8_t *fullduplex);
#endif

#if (TSE_PHY_INTERFACE == TSE_RGMII)
static void rgmii_set_link_speed(tse_instance_t *this_tse, tse_speed_t speed);
#endif

/**************************************************************************/
/* Public Functions                                                       */
/**************************************************************************/

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
void
TSE_init(tse_instance_t *this_tse, uint32_t base_addr, tse_cfg_t *cfg)
{
    HAL_ASSERT(cfg != NULL_POINTER);

    HAL_set_32bit_reg_field(base_addr, CFG2_IF_MODE, 0x00u);
    if (cfg != NULL_POINTER)
    {
        HAL_set_32bit_reg(base_addr, DMAINTRMASK, 0x00000000UL);

        this_tse->base_addr = base_addr;
        mac_reset(this_tse);

        HAL_set_32bit_reg_field(base_addr, CFG2_IF_MODE, 0x00u);

        config_mac_hw(this_tse, cfg);

        /* Assign MAC station address */
        assign_station_addr(this_tse, cfg->mac_addr);

        /* Intialize Tx & Rx descriptor rings */
        tx_desc_ring_init(this_tse);
        rx_desc_ring_init(this_tse);

        /* Initialize Tx descriptors related variables. */
        this_tse->first_tx_index = INVALID_INDEX;
        this_tse->last_tx_index = INVALID_INDEX;
        this_tse->next_tx_index = 0;
        this_tse->nb_available_tx_desc = TSE_TX_RING_SIZE;

        /* Initialize Rx descriptors related variables. */
        this_tse->nb_available_rx_desc = TSE_RX_RING_SIZE;
        this_tse->next_free_rx_desc_index = 0;
        this_tse->first_rx_desc_index = INVALID_INDEX;

        /* initialize default interrupt handlers */
        this_tse->tx_complete_handler = NULL_POINTER;
        this_tse->pckt_rx_callback = NULL_POINTER;

        /* Detect PHY */
        if (cfg->phy_addr == TSE_PHY_ADDRESS_AUTO_DETECT)
        {
            this_tse->phy_addr = phy_probe(this_tse);
            HAL_ASSERT(this_tse->phy_addr <= PHY_ADDRESS_MAX);
        }
        else
        {
            if (cfg->phy_addr <= PHY_ADDRESS_MAX)
            {
                this_tse->phy_addr = cfg->phy_addr;
            }
            else
            {
                HAL_ASSERT(0); /*User provided a Invalid PHY address*/
            }
        }
#if (TSE_PHY_INTERFACE != TSE_1000BASEX)
        /* Initialize PHY interface */
        TSE_phy_init(this_tse, this_tse->phy_addr);
#endif

#if ((TSE_PHY_INTERFACE == TSE_SGMII) || (TSE_PHY_INTERFACE == TSE_1000BASEX))
        msgmii_init(this_tse);
#endif

#if (TSE_PHY_INTERFACE != TSE_1000BASEX)
        TSE_phy_set_link_speed(this_tse, this_tse->phy_addr, cfg->speed_duplex_select);

        if (TSE_ENABLE == cfg->aneg_enable)
        {
            TSE_phy_autonegotiate(this_tse, this_tse->phy_addr);
        }
#endif

#if ((TSE_PHY_INTERFACE == TSE_SGMII) || (TSE_PHY_INTERFACE == TSE_1000BASEX))
        if (TSE_ENABLE == cfg->aneg_enable)
        {
            msgmii_autonegotiate(this_tse);
        }
#endif

        update_mac_cfg(this_tse, this_tse->phy_addr);

        /*Enable Stats module*/
        HAL_set_32bit_reg_field(base_addr, IFC_STATS_EN, 0x01u);

        /*Need to write 1 and then 0 to correctly clear and start all counters*/
        HAL_set_32bit_reg_field(base_addr, IFC_STATS_CLR_ALL, 0x01u);
        HAL_set_32bit_reg_field(base_addr, IFC_STATS_CLR_ALL, 0x00u);

        /* Enable default Flow Control at MAC level.    */
        HAL_set_32bit_reg(base_addr, FPC, cfg->framefilter);

        /* Enable transmission at MAC level. */
        HAL_set_32bit_reg_field(base_addr, CFG1_TX_EN, 0x01u);

        /* Enable reception at MAC level.    */
        HAL_set_32bit_reg_field(base_addr, CFG1_RX_EN, 0x01u);
    }
}

static void
update_mac_cfg(tse_instance_t *this_tse, uint8_t phy_addr)
{
    tse_speed_t speed;
    uint8_t fullduplex;
    uint8_t link_up;

#if (TSE_PHY_INTERFACE == TSE_1000BASEX)
    link_up = msgmii_get_link_status(this_tse, &speed, &fullduplex);
#else
    link_up = TSE_phy_get_link_status(this_tse, phy_addr, &speed, &fullduplex);
#endif

    if (TSE_LINK_DOWN != link_up)
    {
#if (TSE_PHY_INTERFACE == TSE_RGMII)
        rgmii_set_link_speed(this_tse, speed);
#endif

        /* Set byte/nibble mode based on interface type and link speed. */
        HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x00u);

        if (TSE_MAC1000MBPS == speed)
        {
            /* Set interface to byte mode. */
            HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x02);

            /* RB */
            HAL_set_32bit_reg(this_tse->base_addr, MISCC, SPEED_1000M);
            HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG5_CFGBYTEMODE, 0x01);
        }
        else
        {
            /* Set interface to nibble mode. */
            HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x00u);

            if (TSE_MAC100MBPS == speed)
            {
                HAL_set_32bit_reg(this_tse->base_addr, MISCC, SPEED_100M);
                HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x01);
            }
            else
            {
                HAL_set_32bit_reg(this_tse->base_addr, MISCC, SPEED_10M);
            }

            HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG5_CFGBYTEMODE, 0x00);
        }

        /* Configure duplex mode */
        if (TSE_HALF_DUPLEX == fullduplex)
        {
            /* half duplex */
            HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_FDX, 0x0);
        }
        else
        {
            /* full duplex */
            HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_FDX, 0x01);
        }
    }
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
uint8_t
TSE_get_link_status(tse_instance_t *this_tse, tse_speed_t *speed, uint8_t *fullduplex)
{
    static tse_speed_t previous_speed = TSE_INVALID_SPEED;
    static uint8_t previous_duplex = 0xAA;
    tse_speed_t link_speed;
    uint8_t link_fullduplex;
    uint8_t link_up;
    uint16_t phy_reg;

#if (TSE_PHY_INTERFACE == TSE_1000BASEX)
    link_up = msgmii_get_link_status(this_tse, &link_speed, &link_fullduplex);
#else
    link_up = TSE_phy_get_link_status(this_tse, this_tse->phy_addr, &link_speed, &link_fullduplex);
#endif

    if (link_up != TSE_LINK_DOWN)
    {
        if (link_speed != previous_speed)
        {
            /* Set byte/nibble mode based on interface type and link speed. */
            HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x00u);

            if (TSE_MAC1000MBPS == link_speed)
            {
                /* Set interface to byte mode. */
                HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x02);

                /* RB */
                HAL_set_32bit_reg(this_tse->base_addr, MISCC, SPEED_1000M);
                HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG5_CFGBYTEMODE, 0x01);
            }
            else
            {
                /* Set interface to nibble mode. */
                HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x00u);

                if (TSE_MAC100MBPS == link_speed)
                {
                    HAL_set_32bit_reg(this_tse->base_addr, MISCC, SPEED_100M);
                    HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_IF_MODE, 0x01);
                }
                else
                {
                    HAL_set_32bit_reg(this_tse->base_addr, MISCC, SPEED_10M);
                }

                HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG5_CFGBYTEMODE, 0x00);
            }
#if (TSE_PHY_INTERFACE == TSE_RGMII)
            rgmii_set_link_speed(this_tse, link_speed);
#endif
        }

        previous_speed = link_speed;

        if (link_fullduplex != previous_duplex)
        {
            /* Configure duplex mode */
            if (TSE_HALF_DUPLEX == link_fullduplex)
            {
                /* half duplex */
                HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_FDX, 0x0);
            }
            else
            {
                /* full duplex */
                HAL_set_32bit_reg_field(this_tse->base_addr, CFG2_FDX, 0x01);
            }
        }

        previous_duplex = link_fullduplex;

        /* Return current link speed and duplex mode.*/
        if (speed != 0u)
        {
            *speed = link_speed;
        }

        if (fullduplex != 0u)
        {
            *fullduplex = link_fullduplex;
        }

        /* Check if ANEG was originally enabled. */
        phy_reg = TSE_read_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_BMCR);

        if ((phy_reg & BMCR_ANENABLE) &&
            ((TSE_PHY_INTERFACE == TSE_SGMII) || (TSE_PHY_INTERFACE == TSE_1000BASEX)))
        {
            uint16_t sgmii_link_up;

            /* Find out if link is up on SGMII link between MAC and external PHY. */
            phy_reg = TSE_read_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_BMSR);
            sgmii_link_up = phy_reg & BMSR_LSTATUS;

            if (TSE_LINK_DOWN == sgmii_link_up)
            {
                /* Initiate auto-negotiation on the SGMII link. */
                phy_reg |= BMCR_ANRESTART;
                TSE_write_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_BMCR, phy_reg);
            }
        }
    }

    return link_up;
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
void
TSE_cfg_struct_def_init(tse_cfg_t *cfg)
{
    const uint8_t g_default_mac_address[6] = {0xC0u, 0xB1u, 0x3Cu, 0x88u, 0x88u, 0x88u};

    HAL_ASSERT(NULL_POINTER != cfg);

    if (NULL_POINTER != cfg)
    {
        cfg->framefilter = TSE_FC_DEFAULT_MASK;
        cfg->aneg_enable = TSE_ENABLE;
        cfg->speed_duplex_select = TSE_ANEG_ALL_SPEEDS;
        cfg->phy_addr = 0u;
        cfg->tx_edc_enable = TSE_ERR_DET_CORR_ENABLE;
        cfg->rx_edc_enable = TSE_ERR_DET_CORR_ENABLE;
        cfg->preamble_length = TSE_PREAMLEN_DEFVAL;
        cfg->hugeframe_enable = TSE_HUGE_FRAME_DISABLE;
        cfg->length_field_check = TSE_LENGTH_FIELD_CHECK_ENABLE;
        cfg->pad_n_CRC = TSE_PAD_N_CRC_ENABLE;
        cfg->append_CRC = TSE_CRC_ENABLE;
        cfg->fullduplex = TSE_FULLDUPLEX_ENABLE;
        cfg->loopback = TSE_LOOPBACK_DISABLE;
        cfg->rx_flow_ctrl = TSE_RX_FLOW_CTRL_DISABLE;
        cfg->tx_flow_ctrl = TSE_TX_FLOW_CTRL_DISABLE;
        cfg->min_IFG = TSE_MINIFG_DEFVAL;
        cfg->btb_IFG = TSE_BTBIFG_DEFVAL;
        cfg->max_retx_tries = TSE_MAXRETX_DEFVAL;
        cfg->excessive_defer = TSE_EXSS_DEFER_DISABLE;
        cfg->nobackoff = TSE_NO_BACKOFF_DISABLE;
        cfg->backpres_nobackoff = TSE_BACKPRESS_NO_BACKOFF_DISABLE;
        cfg->ABEB_enable = TSE_ABEB_DISABLE;
        cfg->ABEB_truncvalue = TSE_ABEBTRUNC_DEFVAL;
        cfg->phyclk = TSE_DEF_PHY_CLK;
        cfg->supress_preamble = TSE_SUPPRESS_PREAMBLE_DISABLE;
        cfg->autoscan_phys = TSE_PHY_AUTOSCAN_DISABLE;
        cfg->max_frame_length = TSE_MAXFRAMELEN_DEFVAL;
        cfg->non_btb_IFG = TSE_NONBTBIFG_DEFVAL;
        cfg->slottime = TSE_SLOTTIME_DEFVAL;
        cfg->framedrop_mask = TSE_DEFVAL_FRAMEDROP_MASK;
        cfg->wol_enable = TSE_WOL_DETECT_DISABLE;

        cfg->mac_addr[0] = g_default_mac_address[0];
        cfg->mac_addr[1] = g_default_mac_address[1];
        cfg->mac_addr[2] = g_default_mac_address[2];
        cfg->mac_addr[3] = g_default_mac_address[3];
        cfg->mac_addr[4] = g_default_mac_address[4];
        cfg->mac_addr[5] = g_default_mac_address[5];
    }
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
static void
mac_reset(tse_instance_t *this_tse)
{
    /* Reset MIIMGMT  */
    HAL_set_32bit_reg_field(this_tse->base_addr, MIIMGMT_RESET_MII_MGMT, 0x01u);

    /* Reset FIFO watermark module */
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_WMM_RST, 0x01u);

    /* Reset FIFO Rx system module */
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_RSYS_RST, 0x01u);

    /* Reset FIFO Rx fab module */
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_RFAB_RST, 0x01u);

    /* Reset FIFO Tx system module */
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_TSYS_RST, 0x01u);

    /* Reset FIFO Tx system module */
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_TFAB_RST, 0x01u);
}

static void
config_mac_hw(tse_instance_t *this_tse, const tse_cfg_t *cfg)
{
    uint32_t tempreg;

    /* Check for validity of configuration parameters */
    HAL_ASSERT(IS_STATE(cfg->tx_edc_enable));
    HAL_ASSERT(IS_STATE(cfg->rx_edc_enable));
    HAL_ASSERT(TSE_PREAMLEN_MAXVAL >= cfg->preamble_length);
    HAL_ASSERT(IS_STATE(cfg->hugeframe_enable));
    HAL_ASSERT(IS_STATE(cfg->length_field_check));
    HAL_ASSERT(IS_STATE(cfg->pad_n_CRC));
    HAL_ASSERT(IS_STATE(cfg->append_CRC));
    HAL_ASSERT(IS_STATE(cfg->loopback));
    HAL_ASSERT(IS_STATE(cfg->rx_flow_ctrl));
    HAL_ASSERT(IS_STATE(cfg->tx_flow_ctrl));

    HAL_ASSERT(TSE_BTBIFG_MAXVAL >= cfg->btb_IFG);
    HAL_ASSERT(TSE_MAXRETX_MAXVAL >= cfg->max_retx_tries);
    HAL_ASSERT(IS_STATE(cfg->excessive_defer));
    HAL_ASSERT(IS_STATE(cfg->nobackoff));
    HAL_ASSERT(IS_STATE(cfg->backpres_nobackoff));
    HAL_ASSERT(IS_STATE(cfg->ABEB_enable));
    HAL_ASSERT(TSE_ABEBTRUNC_MAXVAL >= cfg->ABEB_truncvalue);
    HAL_ASSERT(TSE_BY28_PHY_CLK >= cfg->phyclk);
    HAL_ASSERT(IS_STATE(cfg->supress_preamble));
    HAL_ASSERT(IS_STATE(cfg->autoscan_phys));
    HAL_ASSERT(TSE_MAXFRAMELEN_MAXVAL >= cfg->max_frame_length);
    HAL_ASSERT(TSE_SLOTTIME_MAXVAL >= cfg->slottime);

    /* Configure PHY related MII MGMT registers */
    tempreg = (uint32_t)cfg->phyclk & MII_CLOCK_SELECT_MASK;

    if (TSE_ENABLE == cfg->supress_preamble)
    {
        tempreg |= MII_PREAM_SUPRESS_MASK;
    }

    if (TSE_ENABLE == cfg->autoscan_phys)
    {
        tempreg |= MII_SCAN_AUTO_INC_MASK;
    }

    HAL_set_32bit_reg(this_tse->base_addr, MIIMGMT, tempreg);

    /* Clear all reset bits */

    /* Clear MIIMGMT */
    HAL_set_32bit_reg_field(this_tse->base_addr, MIIMGMT_RESET_MII_MGMT, 0x00u);

    /* Clear FIFO resets. */
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_WMM_RST, 0x00u);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_RSYS_RST, 0x00u);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_RFAB_RST, 0x00u);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_TSYS_RST, 0x00u);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_TFAB_RST, 0x00u);

    /* configure loppback and flow control enables. */
    if (TSE_ENABLE == cfg->loopback)
    {
        tempreg = CFG1_LOOPBACK_MASK;
    }
    else
    {
        tempreg = 0u;
    }

    if (TSE_ENABLE == cfg->rx_flow_ctrl)
    {
        tempreg |= CFG1_RX_FCTL_MASK;
    }

    if (TSE_ENABLE == cfg->tx_flow_ctrl)
    {
        tempreg |= CFG1_TX_FCTL_MASK;
    }

    HAL_set_32bit_reg(this_tse->base_addr, CFG1, tempreg);

    if (TSE_ENABLE == cfg->fullduplex)
    {
        tempreg = CFG2_FDX_MASK;
    }
    else
    {
        tempreg = 0u;
    }

    if (TSE_ENABLE == cfg->append_CRC)
    {
        tempreg |= CFG2_CRC_EN_MASK;
    }

    if (TSE_ENABLE == cfg->pad_n_CRC)
    {
        tempreg |= CFG2_PAD_CRC_EN_MASK;
    }

    if (TSE_ENABLE == cfg->length_field_check)
    {
        tempreg |= CFG2_LEN_CHECK_MASK;
    }

    if (TSE_ENABLE == cfg->hugeframe_enable)
    {
        tempreg |= CFG2_HUGE_FRAME_EN_MASK;
    }

    if ((TSE_PHY_INTERFACE == TSE_MII) || (TSE_PHY_INTERFACE == TSE_RMII))
    {
        /* MII and RMII use nibble mode interface. */
        tempreg |= CFG2_NIBBLE_MASK;
    }
    else
    {
        /* TBI and GMII use byte interface. */
        tempreg |= CFG2_BYTE_MASK;
    }

    tempreg |= (((uint32_t)cfg->preamble_length) << CFG2_PREAM_LEN);
    HAL_set_32bit_reg(this_tse->base_addr, CFG2, tempreg);

    tempreg = cfg->btb_IFG;
    tempreg |= ((uint32_t)cfg->min_IFG << IFG_MINIFGENF);
    tempreg |= ((uint32_t)cfg->non_btb_IFG << IFG_NONBTBIPG);

    HAL_set_32bit_reg(this_tse->base_addr, IFG, tempreg);

    tempreg = (uint32_t)cfg->slottime & HALF_DUPLEX_SLOTTIME_MASK;

    tempreg |= (uint32_t)cfg->max_retx_tries << HALF_DUPLEX_RETX_MAX_OFFSET;

    if (TSE_ENABLE == cfg->excessive_defer)
    {
        tempreg |= HALF_DUPLEX_EXCS_DEFER_MASK;
    }

    if (TSE_ENABLE == cfg->nobackoff)
    {
        tempreg |= HALF_DUPLEX_NO_BACKOFF_MASK;
    }

    if (TSE_ENABLE == cfg->backpres_nobackoff)
    {
        tempreg |= HALF_DUPLEX_BACKPRES_NOBACKOFF_MASK;
    }

    if (TSE_ENABLE == cfg->ABEB_enable)
    {
        tempreg |= HALF_DUPLEX_ABEB_ENABLE_MASK;
    }

    tempreg |= (uint32_t)cfg->ABEB_truncvalue << HALF_DUPLEX_ABEB_TUNC_OFFSET;

    HAL_set_32bit_reg(this_tse->base_addr, HDX, tempreg);
    HAL_set_32bit_reg(this_tse->base_addr, FREMLEN, (uint32_t)cfg->max_frame_length);

    /*Enable WoL*/
    if (TSE_WOL_DETECT_DISABLE != cfg->wol_enable)
    {
        if (TSE_WOL_UNICAST_FRAME_DETECT_EN & cfg->wol_enable)
        {
            HAL_set_32bit_reg_field(this_tse->base_addr, IFC_MCXWOL_UMATCH_EN, 0x01u);
        }

        if (TSE_WOL_MAGIC_FRAME_DETECT_EN & cfg->wol_enable)
        {
            HAL_set_32bit_reg_field(this_tse->base_addr, IFC_MCXWOL_MAGIC_EN, 0x01u);
        }
    }

    /*--------------------------------------------------------------------------
     * Configure FIFOs
     */
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_WMM_EN, 0x01);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_RSYS_EN, 0x01);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_RFAB_EN, 0x01);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_TSYS_EN, 0x01);
    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG0_TFAB_EN, 0x01);

    /* RX FIFO size : 8KB  */
    HAL_set_32bit_reg(this_tse->base_addr, FIFOCFG1, FIFO_CFG1_DEFVAL);

    /* Rx FIFO watermark */
    HAL_set_32bit_reg(this_tse->base_addr, FIFOCFG2, FIFO_CFG2_DEFVAL);

    /* Tx FIFO watermark: 4KB Tx FIFO */
    HAL_set_32bit_reg(this_tse->base_addr, FIFOCFG3, FIFO_CFG3_DEFVAL);

    tempreg = HAL_get_32bit_reg_field(this_tse->base_addr, FIFOCFG5_HSTFLTRFRMDC);

    tempreg |= 0x0003FFFF;

    tempreg &= ~(cfg->framedrop_mask);

    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG5_HSTFLTRFRMDC, tempreg);

    HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG4_HSTFLTRFRM, cfg->framedrop_mask);

    if (cfg->framedrop_mask & TSE_SHORT_FRAME_FRAMEDROP_MASK)
    {
        HAL_set_32bit_reg_field(this_tse->base_addr, FIFOCFG5_HSTDRPLT64, 0x01);
    }
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
void
TSE_write_phy_reg(tse_instance_t *this_tse, uint8_t phyaddr, uint8_t regaddr, uint16_t regval)
{
    HAL_ASSERT(TSE_PHYADDR_MAXVAL >= phyaddr);
    HAL_ASSERT(TSE_PHYREGADDR_MAXVAL >= regaddr);

    /*
      Write PHY address in MII Mgmt address register. Makes previous register
      address 0 & invalid.
     */
    if ((TSE_PHYADDR_MAXVAL >= phyaddr) && (TSE_PHYREGADDR_MAXVAL >= regaddr))
    {
        /* Wait for MII Mgmt interface to complete previous operation. */
        do
        {
            ;
        } while (HAL_get_32bit_reg_field(this_tse->base_addr, MIIMIND_BUSY));

        /* Load PHY address in MII Mgmt address register */
        HAL_set_32bit_reg_field(this_tse->base_addr, MIIMADDR_PHY_ADDR, phyaddr);

        /* Load register address in MII Mgmt address register */
        HAL_set_32bit_reg_field(this_tse->base_addr, MIIMADDR_REG_ADDR, regaddr);

        /* Load value to be written in MII Mgmt control register */
        HAL_set_32bit_reg_field(this_tse->base_addr, MIIMCTRL_PHY_CTRL, regval);
    }
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
uint16_t
TSE_read_phy_reg(tse_instance_t *this_tse, uint8_t phyaddr, uint8_t regaddr)
{
    uint32_t timeout = 100000u;
    uint16_t read_val = 0;

    HAL_ASSERT(TSE_PHYADDR_MAXVAL >= phyaddr);
    HAL_ASSERT(TSE_PHYREGADDR_MAXVAL >= regaddr);

    /*
      Write PHY address in MII Mgmt address register. Makes previous register
      address 0 & invalid.
     */
    if ((TSE_PHYADDR_MAXVAL >= phyaddr) && (TSE_PHYREGADDR_MAXVAL >= regaddr))
    {
        volatile uint32_t mii_not_valid;
        volatile uint32_t mii_busy;

        /* Wait for MII Mgmt interface to complete previous operation. */
        do
        {
            ;
        } while (HAL_get_32bit_reg_field(this_tse->base_addr, MIIMIND_BUSY));

        HAL_set_32bit_reg_field(this_tse->base_addr, MIIMADDR_PHY_ADDR, phyaddr);

        /* Load PHY register address in MII Mgmt address register */
        HAL_set_32bit_reg_field(this_tse->base_addr, MIIMADDR_REG_ADDR, regaddr);

        /* Issue read command to PHY */
        HAL_set_32bit_reg_field(this_tse->base_addr, MIIMCMD_READ_CYCLE, 0x01);

        /* Poll till PHY read cycle is completed or timeout expired */
        do
        {
            --timeout;
            mii_not_valid = HAL_get_32bit_reg_field(this_tse->base_addr, MIIMIND_NV);
            mii_busy = HAL_get_32bit_reg_field(this_tse->base_addr, MIIMIND_BUSY);
        } while ((timeout > 0u) && ((mii_busy != 0u) || (mii_not_valid != 0u)));

        /*
          Read value from MII Mgmt status register only if timeout is not expired
          but the read the read cycle is completed
         */
        HAL_ASSERT(timeout > 0u);

        if (timeout != 0u)
        {
            read_val = HAL_get_32bit_reg_field(this_tse->base_addr, MIIMSTATUS_PHY_STATUS);
        }
    }

    HAL_set_32bit_reg_field(this_tse->base_addr, MIIMCMD_READ_CYCLE, 0x00);

    return read_val;
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
uint32_t
TSE_read_stat(tse_instance_t *this_tse, tse_stat_t stat)
{
    uint32_t stat_val = 0u;
    uint32_t cnt = 0;

    if (TSE_MAC_LAST_STAT < stat)
    {
        cnt = this_tse->base_addr + TR64_REG_OFFSET + cnt;
    }

    switch (stat)
    {
        case TSE_FRAME_CNT_64:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TR64);
            break;
        case TSE_FRAME_CNT_127:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TR127);
            break;
        case TSE_FRAME_CNT_255:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TR255);
            break;
        case TSE_FRAME_CNT_511:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TR511);
            break;
        case TSE_FRAME_CNT_1K:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TR1K);
            break;
        case TSE_FRAME_CNT_MAX:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TRMAX);
            break;
        case TSE_FRAME_CNT_VLAN:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TRMGV);
            break;
        case TSE_RX_BYTE_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RBYT);
            break;
        case TSE_RX_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RPKT);
            break;
        case TSE_RX_FCS_ERR_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RFCS);
            break;
        case TSE_RX_MULTICAST_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RMCA);
            break;
        case TSE_RX_BROADCAST_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RBCA);
            break;
        case TSE_RX_CTRL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RXCF);
            break;
        case TSE_RX_PAUSE_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RXPF);
            break;
        case TSE_RX_UNKNOWN_OPCODE_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RXUO);
            break;
        case TSE_RX_ALIGN_ERR_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RALN);
            break;
        case TSE_RX_FRAMELENGTH_ERR_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RFLR);
            break;
        case TSE_RX_CODE_ERR_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RCDE);
            break;
        case TSE_RX_CS_ERR_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RCSE);
            break;
        case TSE_RX_UNDERSIZE_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RUND);
            break;
        case TSE_RX_OVERSIZE_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, ROVR);
            break;
        case TSE_RX_FRAGMENT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RFRG);
            break;
        case TSE_RX_JABBER_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RJBR);
            break;
        case TSE_RX_DROP_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, RDRP);
            break;
        case TSE_TX_BYTE_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TBYT);
            break;
        case TSE_TX_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TPKT);
            break;
        case TSE_TX_MULTICAST_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TMCA);
            break;
        case TSE_TX_BROADCAST_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TBCA);
            break;
        case TSE_TX_PAUSE_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TXPF);
            break;
        case TSE_TX_DEFFERAL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TDFR);
            break;
        case TSE_TX_EXCS_DEFFERAL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TEDF);
            break;
        case TSE_TX_SINGLE_COLL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TSCL);
            break;
        case TSE_TX_MULTI_COLL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TMCL);
            break;
        case TSE_TX_LATE_COLL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TLCL);
            break;
        case TSE_TX_EXCSS_COLL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TXCL);
            break;
        case TSE_TX_TOTAL_COLL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TNCL);
            break;
        case TSE_TX_PAUSE_HONORED_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TPFH); /*This is not used*/
            break;
        case TSE_TX_DROP_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TDRP);
            break;
        case TSE_TX_JABBER_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TJBR);
            break;
        case TSE_TX_FCS_ERR_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TFCS);
            break;
        case TSE_TX_CNTRL_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TXCF);
            break;
        case TSE_TX_OVERSIZE_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TOVR);
            break;
        case TSE_TX_UNDERSIZE_PKT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TUND);
            break;
        case TSE_TX_FRAGMENT_CNT:
            stat_val = HAL_get_32bit_reg(this_tse->base_addr, TFRG);
            break;
        default:
            HAL_ASSERT(TSE_MAC_LAST_STAT > stat);
    }

    return stat_val;
}

/***************************************************************************/ /**
  See core_tse.h for details of how to use this function
 */
void
TSE_clear_statistics(tse_instance_t *instance)
{
    HAL_set_32bit_reg_field(instance->base_addr, IFC_STATS_CLR_ALL, 0x01u);
    HAL_set_32bit_reg_field(instance->base_addr, IFC_STATS_CLR_ALL, 0x00u);
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
uint8_t
TSE_receive_pkt(tse_instance_t *this_tse, uint8_t *rx_pkt_buffer, void *p_user_data)
{
    uint8_t status = TSE_FAILED;

    /* Make this function atomic w.r.to RxPktRcvd interrupt */
    HAL_set_32bit_reg_field(this_tse->base_addr, DMAINTRMASK_RXPKT_RCVD, 0x0);

    HAL_ASSERT(NULL_POINTER != rx_pkt_buffer);
    HAL_ASSERT(IS_WORD_ALIGNED(rx_pkt_buffer));

    if (this_tse->nb_available_rx_desc > 0)
    {
        int16_t next_rx_desc_index;

        --this_tse->nb_available_rx_desc;
        next_rx_desc_index = this_tse->next_free_rx_desc_index;

        this_tse->rx_desc_tab[next_rx_desc_index].pkt_start_addr = (uint32_t)rx_pkt_buffer;

        this_tse->rx_desc_tab[next_rx_desc_index].caller_info = p_user_data;

        /* Set own flag of this descriptor to indicate ready for RX */
        this_tse->rx_desc_tab[next_rx_desc_index].pkt_size |= DMA_DESC_EMPTY_FLAG_MASK;

        /*
          If the RX is found disabled, then it might be because this is the
          first time a packet is scheduled for reception or the RX ENABLE is
          made zero by RX overflow or RX bus error. In either case, this
          function tries to schedule the current packet for reception.
         */
        if (0 == HAL_get_32bit_reg_field(this_tse->base_addr, DMARXCTRL_RX_EN))
        {
            /* Make current descriptor to point to the packet requested */
            this_tse->first_rx_desc_index = next_rx_desc_index;

            HAL_set_32bit_reg(this_tse->base_addr,
                              DMARXDESC,
                              (uint32_t)&this_tse->rx_desc_tab[next_rx_desc_index]);

            HAL_set_32bit_reg_field(this_tse->base_addr, DMARXSTATUS_RXPKT_OVR, 0x01);
            HAL_set_32bit_reg_field(this_tse->base_addr, DMARXCTRL_RX_EN, 0x01);
        }

        /*
          Point the next_rx_desc to next free descriptor in the ring. Wrap around
          in case next descriptor is pointing to last in the ring
         */
        ++this_tse->next_free_rx_desc_index;

        if (this_tse->next_free_rx_desc_index >= TSE_RX_RING_SIZE)
        {
            this_tse->next_free_rx_desc_index = 0;
        }

        /*
          Packet scheduled for reception successfully only when there is no new
          RX bus error
         */
        if (0u == HAL_get_32bit_reg_field(this_tse->base_addr, DMARXSTATUS_BUSERR))
        {
            status = TSE_SUCCESS;
        }
    }

    /* RxPktRcvd Interrupt Enable */
    HAL_set_32bit_reg_field(this_tse->base_addr, DMAINTRMASK_RXPKT_RCVD, 0x1);

    return status;
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
uint8_t
TSE_send_pkt(tse_instance_t *this_tse,
             uint8_t const *tx_buffer,
             uint32_t tx_length,
             void *p_user_data)
{
    uint8_t status = TSE_FAILED;

    /* Make this function atomic w.r.to TxPktSent interrupt */
    HAL_set_32bit_reg_field(this_tse->base_addr, DMAINTRMASK_TXPKT_SENT, 0x0);
    HAL_ASSERT(NULL_POINTER != tx_buffer);
    HAL_ASSERT(0 != tx_length);
    HAL_ASSERT(IS_WORD_ALIGNED(tx_buffer));

    if (this_tse->nb_available_tx_desc > 0)
    {
        tse_desc_t *p_next_tx_desc;

        --this_tse->nb_available_tx_desc;

        if (INVALID_INDEX == this_tse->first_tx_index)
        {
            this_tse->first_tx_index = this_tse->next_tx_index;
        }

        this_tse->last_tx_index = this_tse->next_tx_index;

        p_next_tx_desc = &this_tse->tx_desc_tab[this_tse->next_tx_index];
        p_next_tx_desc->pkt_start_addr = (uint32_t)tx_buffer;
        p_next_tx_desc->caller_info = p_user_data;

        /* Set the packet length, packet overrides and clear descriptor empty: */
        p_next_tx_desc->pkt_size = tx_length;

        /*
          If TX is found disabled, this might be because this is the first
          time packet is being sent or the DMA completed previous transfer and
          stopped transmission by itself caused by TX underrun or bus error.
          This function neglects the errors and tries to send the current packet.
         */
        if (0 == HAL_get_32bit_reg_field(this_tse->base_addr, DMATXCTRL_TX_EN))
        {
            HAL_set_32bit_reg(this_tse->base_addr, DMATXDESC, (uint32_t)p_next_tx_desc);
        }

        /*
          Enable DMA transmit anyway to cover the case where Tx completed after
          the read of DMA_TX_CTRL.
         */
        HAL_set_32bit_reg_field(this_tse->base_addr, DMATXCTRL_TX_EN, 0x01);

        /*
          Point the next_tx_desc to next free descriptor in the ring. Wrap around
          in case next descriptor is pointing to last in the ring
         */
        if ((TSE_TX_RING_SIZE - 1) == this_tse->next_tx_index)
        {
            this_tse->next_tx_index = 0;
        }
        else
        {
            ++this_tse->next_tx_index;
        }

        /*
          Packet scheduled for transmission successfully only when there is no
          TX bus error
         */
        if (0 == HAL_get_32bit_reg_field(this_tse->base_addr, DMARXSTATUS_BUSERR))
        {
            status = TSE_SUCCESS;
        }
    }

    /* TxPktSent Interrupt Enable. */
    HAL_set_32bit_reg_field(this_tse->base_addr, DMAINTRMASK_TXPKT_SENT, 0x1);

    return status;
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
void
TSE_isr(tse_instance_t *this_tse)
{
    uint32_t dma_irq;
    uint32_t packet_sent;
    uint32_t packet_received;

    dma_irq = HAL_get_32bit_reg(this_tse->base_addr, DMAINTR);

    /* Transmit packet sent interrupt */
    packet_sent = dma_irq & TSE_TXPKTSENT_IRQ_MASK;

    if (packet_sent != 0u)
    {
        txpkt_handler(this_tse);
    }

    /* Packet received interrupt */
    packet_received = dma_irq & TSE_RXPKTRCVD_IRQ_MASK;

    if (packet_received != 0u)
    {
        rxpkt_handler(this_tse);
    }

    if (dma_irq & (1u << TSE_TXUNDRRUN_IRQ))
    {
        /* Clear the tx packet sent interrupt. */
        HAL_set_32bit_reg_field(this_tse->base_addr, DMATXSTATUS_TXPKT_UR, 0x1);
    }

    if (dma_irq & (1u << TSE_TXBUSERR_IRQ))
    {
        /* Clear the tx packet sent interrupt. */
        HAL_set_32bit_reg_field(this_tse->base_addr, DMATXSTATUS_BUSERR, 0x1);
    }

    if (dma_irq & (1u << TSE_RXOVRFLOW_IRQ))
    {
        HAL_set_32bit_reg_field(this_tse->base_addr, DMARXSTATUS_RXPKT_OVR, 0x1);
        HAL_set_32bit_reg_field(this_tse->base_addr, DMARXCTRL_RX_EN, 0x1);
    }

    if (dma_irq & (1u << TSE_RXBUSERR_IRQ))
    {
        /*
          Clear the rx packet received interrupt once. If this bit still persists,
          then another rx packet received interrupt will be generated. Rx count
          will be decremented.
         */
        HAL_set_32bit_reg_field(this_tse->base_addr, DMARXSTATUS_BUSERR, 0x1);
    }

    if (HAL_get_32bit_reg_field(this_tse->base_addr, IFS_WOL_DET))
    {
        /*call the application callback handler*/
        if (NULL_POINTER != this_tse->wol_callback)
        {
            this_tse->wol_callback();
        }

        /* Clear the WoL interrupt */
        HAL_set_32bit_reg_field(this_tse->base_addr, IFC_MCXWOL_ST_CLR, 0x01);
        HAL_set_32bit_reg_field(this_tse->base_addr, IFC_MCXWOL_ST_CLR, 0x00);
    }
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
void
TSE_set_tx_callback(tse_instance_t *this_tse, tse_transmit_callback_t tx_complete_handler)
{
    this_tse->tx_complete_handler = tx_complete_handler;
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
void
TSE_set_rx_callback(tse_instance_t *this_tse, tse_receive_callback_t rx_callback)
{
    this_tse->pckt_rx_callback = rx_callback;
}

/**************************************************************************/ /**
                                                                              * See core_tse.h for
                                                                              * details of how to
                                                                              * use this function.
                                                                              */
void
TSE_set_wol_callback(tse_instance_t *this_tse, tse_wol_callback_t wol_callback)
{
    this_tse->wol_callback = wol_callback;
}
/***************************************************************************/ /**
  See core_tse.h for details of how to use this function
 */
void
TSE_set_address_filter(tse_instance_t *this_tse,
                       const uint8_t *mac_addresses,
                       uint32_t nb_addresses)
{
    uint32_t reg_value;
    uint8_t hash_mulicast_set = TSE_DISABLE;
    uint8_t hash_unicast_set = TSE_DISABLE;
    uint8_t broadcast_set = TSE_DISABLE;
    uint8_t unicast_set = TSE_DISABLE;
    uint16_t count;
    uint32_t hash_table_index = TSE_DISABLE;
    uint32_t mac_hash_table[4] = {TSE_DISABLE};

    HAL_ASSERT(mac_addresses != NULL_POINTER);

    HAL_ASSERT((nb_addresses != 0));

    for (count = 0; count < (nb_addresses * MAC_ADDRESS_LENGTH); count += MAC_ADDRESS_LENGTH)
    {
        /* Checking whether the MAC address is Broadcast or not */
        if ((BROADCAST_COMPARE_BITS == mac_addresses[count]) &&
            (BROADCAST_COMPARE_BITS == mac_addresses[count + 1]) &&
            (BROADCAST_COMPARE_BITS == mac_addresses[count + 2]) &&
            (BROADCAST_COMPARE_BITS == mac_addresses[count + 3]) &&
            (BROADCAST_COMPARE_BITS == mac_addresses[count + 4]) &&
            (BROADCAST_COMPARE_BITS == mac_addresses[count + 5]))
        {
            if (broadcast_set == TSE_DISABLE)
            {
                broadcast_set = TSE_ENABLE;
            }
        }

        /*Checking whether the MAC address is local base station address or not*/
        else if ((mac_addresses[count] ==
                  (uint8_t)(HAL_get_32bit_reg(this_tse->base_addr, STADDRH) >> 16)) &&
                 (mac_addresses[count + 1] ==
                  (uint8_t)(HAL_get_32bit_reg(this_tse->base_addr, STADDRH) >> 24)) &&
                 (mac_addresses[count + 2] ==
                  (uint8_t)(HAL_get_32bit_reg(this_tse->base_addr, STADDRL))) &&
                 (mac_addresses[count + 3] ==
                  (uint8_t)(HAL_get_32bit_reg(this_tse->base_addr, STADDRL) >> 8)) &&
                 (mac_addresses[count + 4] ==
                  (uint8_t)(HAL_get_32bit_reg(this_tse->base_addr, STADDRL) >> 16)) &&
                 (mac_addresses[count + 5] ==
                  (uint8_t)(HAL_get_32bit_reg(this_tse->base_addr, STADDRL) >> 24)))
        {
            if (unicast_set == TSE_DISABLE)
            {
                unicast_set = TSE_ENABLE;
            }
        }
        else
        {
            if ((hash_mulicast_set == TSE_DISABLE) || (hash_unicast_set == TSE_DISABLE))
            {
                /* Checking whether the MAC address is Multicast  or not*/
                if (((mac_addresses[count] & MULTICAST_ADDRESS_MASK) == MULTICAST_ADDRESS) &&
                    (hash_mulicast_set == TSE_DISABLE))
                {
                    hash_mulicast_set = TSE_ENABLE;
                }

                /* Checking whether the MAC address is Unicast or not */
                if ((((mac_addresses[count] & MULTICAST_ADDRESS_MASK) == UNICAST_ADDRESS) &&
                     (hash_unicast_set == TSE_DISABLE)))
                {
                    hash_unicast_set = TSE_ENABLE;
                }
            }

            /* CRC calculation for MAC DA */
            hash_table_index = TSE_ethernet_crc(&mac_addresses[count], MAC_ADDRESS_LENGTH);

            /* Using upper 7-bits of the CRC */
            hash_table_index = hash_table_index >> 25u;
            mac_hash_table[hash_table_index / 0x20u] |= 1u << (hash_table_index & 0x1Fu);
        }
    }

    reg_value =
        (uint32_t)((hash_unicast_set << FPC_HASH_UNICAST_SHIFT) |
                   (hash_mulicast_set << FPC_HASH_MULTICAST_SHIFT) |
                   (unicast_set << FPC_UNICAST_SHIFT) | (broadcast_set << FPC_BROADCAST_SHIFT));

    /* Enable Hash-multicast, Hash-unicast, unicast and broadcast frames */
    HAL_set_32bit_reg(this_tse->base_addr, FPC, reg_value);

    /* Set the Hash table for Hash uni/multicast frames */
    HAL_set_32bit_reg(this_tse->base_addr, HASHTAB0, mac_hash_table[0]);
    HAL_set_32bit_reg(this_tse->base_addr, HASHTAB1, mac_hash_table[1]);
    HAL_set_32bit_reg(this_tse->base_addr, HASHTAB2, mac_hash_table[2]);
    HAL_set_32bit_reg(this_tse->base_addr, HASHTAB3, mac_hash_table[3]);
}

/**************************************************************************/
/* Private Function definitions                                           */
/**************************************************************************/

/**************************************************************************/ /**
                                                                              * This is default
                                                                              * "Receive packet
                                                                              * interrupt handler.
                                                                              * This function finds
                                                                              * the descriptor that
                                                                              * received the packet
                                                                              * and caused the
                                                                              * interrupt. This
                                                                              * informs the received
                                                                              * packet size to the
                                                                              * application and
                                                                              * relinquishes the
                                                                              * packet buffer from
                                                                              * the associated DMA
                                                                              * descriptor.
                                                                              */
static void
rxpkt_handler(tse_instance_t *this_tse)
{
    uint32_t rxcnt = 0u;

    /*
      Execution comes here because at-least one packet is received.
      Hence rxcnt can be read after freeing one descriptor
     */
    do
    {
        tse_desc_t *cdesc = &this_tse->rx_desc_tab[this_tse->first_rx_desc_index];

        /*
          Point the curr_rx_desc to next descriptor in the ring.
          Wrap around in case next descriptor is pointing to last in the ring
         */
        ++this_tse->first_rx_desc_index;

        if (this_tse->first_rx_desc_index >= TSE_RX_RING_SIZE)
        {
            this_tse->first_rx_desc_index = 0;
        }

        /*
          Clear the rx packet received interrupt once. If this bit still persists,
          then another rx packet received interrupt will be generated. Rx count
          will be decremented.
         */
        HAL_set_32bit_reg_field(this_tse->base_addr, DMARXSTATUS_RXPKT_RCVD, 0x1);

        /* Get the latest current received count */
        rxcnt = HAL_get_32bit_reg_field(this_tse->base_addr, DMARXSTATUS_RXPKT_CNT);

        ++this_tse->nb_available_rx_desc;

        /* Pass received packet up to application layer. */
        if (NULL_POINTER != this_tse->pckt_rx_callback)
        {
            uint8_t *p_rx_packet;
            uint32_t pckt_length;

            p_rx_packet = (uint8_t *)cdesc->pkt_start_addr;
            pckt_length = (cdesc->pkt_size & DMA_DESC_PKT_SIZE_MASK) - 4u;

            if (NULL_POINTER != this_tse->pckt_rx_callback)
            {
                this_tse->pckt_rx_callback(p_rx_packet, pckt_length, cdesc->caller_info);
            }
        }

    } while (0u != rxcnt);
}

/**************************************************************************/ /**
                                                                              * This is default
                                                                              * "Transmit packet
                                                                              * interrupt handler.
                                                                              * This function finds
                                                                              * the descriptor that
                                                                              * transmitted the
                                                                              * packet and caused
                                                                              * the interrupt. This
                                                                              * relinquishes the
                                                                              * packet buffer from
                                                                              * the associated DMA
                                                                              * descriptor.
                                                                              */
static void
txpkt_handler(tse_instance_t *this_tse)
{
    uint32_t empty_flag;
    int16_t index;
    uint32_t completed = 0u;

    HAL_ASSERT(this_tse->first_tx_index != INVALID_INDEX);
    HAL_ASSERT(this_tse->last_tx_index != INVALID_INDEX);

    index = this_tse->first_tx_index;
    do
    {
        ++this_tse->nb_available_tx_desc;

        /* Call packet Tx completion handler if it exists. */
        if (NULL_POINTER != this_tse->tx_complete_handler)
        {
            this_tse->tx_complete_handler(this_tse->tx_desc_tab[index].caller_info);
        }

        if (index == this_tse->last_tx_index)
        {
            /* all pending tx packets sent. */
            this_tse->first_tx_index = INVALID_INDEX;
            this_tse->last_tx_index = INVALID_INDEX;
            completed = 1u;
        }
        else
        {
            /* Move on to next transmit descriptor. */
            ++index;

            if (index >= TSE_TX_RING_SIZE)
            {
                index = 0;
            }

            this_tse->first_tx_index = index;

            /* Check if we reached a descriptor still pending tx. */
            empty_flag = this_tse->tx_desc_tab[index].pkt_size & DMA_DESC_EMPTY_FLAG_MASK;

            if (0u == empty_flag)
            {
                completed = 1u;
            }
        }

        /*
          Clear the tx packet sent interrupt. Please note that this must be done
          for every packet sent as it decrements the TXPKTCOUNT.
         */
        HAL_set_32bit_reg_field(this_tse->base_addr, DMATXSTATUS_TXPKT_SENT, 0x1);

    } while (0u == completed);
}

static void
tx_desc_ring_init(tse_instance_t *this_tse)
{
    int32_t inc;

    for (inc = 0; inc < TSE_TX_RING_SIZE; ++inc)
    {
        this_tse->tx_desc_tab[inc].pkt_start_addr = 0u;
        this_tse->tx_desc_tab[inc].pkt_size = DMA_DESC_EMPTY_FLAG_MASK;

        if ((TSE_TX_RING_SIZE - 1) == inc)
        {
            this_tse->tx_desc_tab[inc].next_desriptor = (uint32_t)&this_tse->tx_desc_tab[0];
        }
        else
        {
            this_tse->tx_desc_tab[inc].next_desriptor = (uint32_t)&this_tse->tx_desc_tab[inc + 1];
        }

        this_tse->tx_desc_tab[inc].index = inc;
    }
}

static void
rx_desc_ring_init(tse_instance_t *this_tse)
{
    int32_t inc;

    for (inc = 0; inc < TSE_RX_RING_SIZE; ++inc)
    {
        this_tse->rx_desc_tab[inc].pkt_start_addr = 0u;
        this_tse->rx_desc_tab[inc].pkt_size = 0u;

        if ((TSE_RX_RING_SIZE - 1) == inc)
        {
            this_tse->rx_desc_tab[inc].next_desriptor = (uint32_t)&this_tse->rx_desc_tab[0];
        }
        else
        {
            this_tse->rx_desc_tab[inc].next_desriptor = (uint32_t)&this_tse->rx_desc_tab[inc + 1];
        }

        this_tse->rx_desc_tab[inc].index = inc;
    }
}

static void
assign_station_addr(tse_instance_t *this_tse, const uint8_t mac_addr[6])
{
    uint32_t address32;

    HAL_ASSERT(NULL_POINTER != mac_addr);

    if (NULL_POINTER != mac_addr)
    {
        address32 = ((uint32_t)mac_addr[5]) << 24u;
        address32 |= ((uint32_t)mac_addr[4]) << 16u;
        address32 |= ((uint32_t)mac_addr[3]) << 8u;
        address32 |= ((uint32_t)mac_addr[2]);

        HAL_set_32bit_reg(this_tse->base_addr, STADDRL, address32);

        address32 = ((uint32_t)mac_addr[1]) << 24u;
        address32 |= ((uint32_t)mac_addr[0]) << 16u;

        HAL_set_32bit_reg(this_tse->base_addr, STADDRH, address32);
    }
}

/***************************************************************************/ /**
                                                                               * Probe used PHY.
                                                                               *
                                                                               * return    PHY
                                                                               * address. If PHY is
                                                                               * not found, function
                                                                               * returns 255.
                                                                               */
static uint8_t
phy_probe(tse_instance_t *this_tse)
{
    uint8_t phy;
    uint8_t phy_address = 0;
    uint16_t reg;

    for (phy = PHY_ADDRESS_MIN; phy <= PHY_ADDRESS_MAX; phy++)
    {
        reg = TSE_read_phy_reg(this_tse, phy, MII_PHYSID1);

        if ((reg != 0x0000ffffUL) && (reg != 0x00000000UL))
        {
            phy_address = phy;
            phy = PHY_ADDRESS_MAX + 1;
        }
    }

    return phy_address;
}

/***************************************************************************/ /**
  This function initializes the TBI/1000BaseX module
  */
#if ((TSE_PHY_INTERFACE == TSE_SGMII) || (TSE_PHY_INTERFACE == TSE_1000BASEX))
static void
msgmii_init(tse_instance_t *this_tse)
{
    volatile uint16_t phy_reg;

    /* Reset M-SGMII. */
    TSE_write_phy_reg(this_tse, TSE_MSGMII_ADDR, 0x00, 0x8000u);

#if (TSE_PHY_INTERFACE == TSE_1000BASEX)
    /*Configure MSGMII 1000BaseX Advertisement reg*/
    TSE_write_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_ADVERTISE, 0x0020u);

    /*Configure MSGMII TBI control reg for 1000BaseX*/
    TSE_write_phy_reg(this_tse, TSE_MSGMII_ADDR, 0x11, 0x0800u);
#endif

#if (TSE_PHY_INTERFACE == TSE_SGMII)
    /* Register 0x04 of M-SGMII must be always be set to 0x0001. */
    TSE_write_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_ADVERTISE, 0x0001);
#endif

    /*Enable auto-negotiation inside MSGMII block.*/
    phy_reg = TSE_read_phy_reg(this_tse, TSE_MSGMII_ADDR, 0x00);
    phy_reg |= 0x1000;
    TSE_write_phy_reg(this_tse, TSE_MSGMII_ADDR, 0x00, phy_reg);
}

/***************************************************************************/ /**
                                                                               *
                                                                               */
static void
msgmii_autonegotiate(tse_instance_t *this_tse)
{
    volatile uint16_t phy_reg;
    uint16_t autoneg_complete;
    uint32_t volatile aneg_timeout = 1000000u;

    phy_reg = TSE_read_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_BMCR);
    phy_reg |= (BMCR_ANENABLE | BMCR_ANRESTART);
    TSE_write_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_BMCR, phy_reg);

    /* Wait for msgmii auto-negotiation to complete. */
    do
    {
        phy_reg = TSE_read_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_BMSR);
        autoneg_complete = phy_reg & MSGMII_AUTO_NEGOTIATION_COMPLETE;
        --aneg_timeout;
    } while ((!autoneg_complete && aneg_timeout != 0u) || (0xFFFF == phy_reg));
}

#endif /*#if ((TSE_PHY_INTERFACE == TSE_SGMII) || (TSE_PHY_INTERFACE == TSE_1000BASEX))*/

#if (TSE_PHY_INTERFACE == TSE_1000BASEX)
static uint8_t
msgmii_get_link_status(tse_instance_t *this_tse, tse_speed_t *speed, uint8_t *fullduplex)
{
    volatile uint16_t phy_reg;
    uint16_t link_up;
    uint8_t link_status;

    /* Find out if link is up.*/
    phy_reg = TSE_read_phy_reg(this_tse, TSE_MSGMII_ADDR, MII_BMSR);
    link_up = phy_reg & BMSR_LSTATUS;

    if (link_up != TSE_LINK_DOWN)
    {
        /* Link is up. */
        link_status = TSE_LINK_UP;

        phy_reg = TSE_read_phy_reg(this_tse, TSE_MSGMII_ADDR, MSGMII_PHY_STATUS);

        if (phy_reg & MSGMII_1000BASEX_FD)
        {
            *fullduplex = TSE_FULL_DUPLEX;
        }
        else if (phy_reg & MSGMII_1000BASEX_HD)
        {
            *fullduplex = TSE_HALF_DUPLEX;
        }

        *speed = TSE_MAC1000MBPS;
    }
    else
    {
        /* Link is down. */
        link_status = TSE_LINK_DOWN;
    }

    return link_status;
}
#endif /*#if(TSE_PHY_INTERFACE == TSE_1000BASEX)*/

#if (TSE_PHY_INTERFACE == TSE_RGMII)
static void
rgmii_set_link_speed(tse_instance_t *this_tse, tse_speed_t speed)
{
    if (TSE_MAC1000MBPS == speed)
    {
        TSE_write_phy_reg(this_tse, TSE_RGMII_MDIO_ADDR, 0x0u, 0x0040u);
    }
    if (TSE_MAC100MBPS == speed)
    {
        TSE_write_phy_reg(this_tse, TSE_RGMII_MDIO_ADDR, 0x0u, 0x2000u);
    }
    if (TSE_MAC10MBPS == speed)
    {
        TSE_write_phy_reg(this_tse, TSE_RGMII_MDIO_ADDR, 0x0u, 0x0000u);
    }
}
#endif /*#if (TSE_PHY_INTERFACE == TSE_RGMII)*/

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
