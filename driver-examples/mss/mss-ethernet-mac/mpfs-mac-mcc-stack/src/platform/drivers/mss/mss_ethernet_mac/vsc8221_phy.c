/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file vsc8221_phy.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Microsemi VSC8221 PHY interface driver implementation to support the
 * PolarFire SoC Discovery Kit.
 *
 * Will need to modify FW based on MSS GPIO configuration.
 *
 * Discovery Kit CMODE strap settings are as follows:
 *
 * CMODE 0: 1011 - PHY Address = 11
 * CMODE 1: 1010 - SFP Disabled, PHY Addr[4] = 0, SIGDET = output, 50ohm SERDES
 *                 impedance
 * CMODE 2: 1111 - PHY Operating Mode = SGMii to CAT5 Media, SCLK disabled.
 * CMODE 3: 1000 - AN Advertise 10/100/1000BASE-T HDX, FDX, SQE Disabled,
 *                 LED[2:0] = {Link/Activity, Link/Activity, Fault}
 *
 * VSC_RESTET is connected to A14, GPIO127NB1 and VSC_TXDIS_SRESET is connected
 * to E20, GPIO15NB1. These are both tied high internally in the reference
 * design.
 *
 */

#include "mpfs_hal/mss_hal.h"
#include "hal/hal.h"

#include "drivers/mss/mss_ethernet_mac/mss_ethernet_registers.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_regs.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_sw_cfg.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac.h"
#include "drivers/mss/mss_ethernet_mac/phy.h"

#ifdef __cplusplus
extern "C" {
#endif
//#define MSS_MAC_USE_PHY_VSC8221 1
#if MSS_MAC_USE_PHY_VSC8221

/**************************************************************************/
/* Preprocessor Macros                                                    */
/**************************************************************************/

#define BMSR_AUTO_NEGOTIATION_COMPLETE (0x0020U)

/**************************************************************************/ /**
                                                                              * Perform required
                                                                              * setup post reset
                                                                              */
static void
vsc8221_post_reset_step(const mss_mac_instance_t *this_mac)
{
    uint32_t phy_base_address;

    phy_base_address = this_mac->phy_addr & 0xFCU; /* Calculate base address for chip */
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 31, 0x2A30U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 8,  0x0212U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 31, 0x52B5U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 2,  0x000FU);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 1,  0x472AU);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 0,  0x8FA4U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 31, 0x2A30U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 8,  0x0012U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 31, 0x0000U);
}


/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_VSC8221_phy_init(/* mss_mac_instance_t*/ const void *v_this_mac, uint8_t phy_addr)
{
    uint16_t temp_reg;
    volatile uint16_t phy_reg;
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    volatile uint32_t sgmii_aneg_timeout = 100000U;
    uint16_t autoneg_complete;

    /* Do hardware reset if possible/necessary */

#if defined(MSS_MAC_PHY_HW_RESET) || defined(MSS_MAC_PHY_HW_SRESET)
    if (0 == phy_mac->phy_hard_reset_done)
    {
        /* Active low reset pulse */
        MSS_MAC_phy_reset(phy_mac, MSS_MAC_HARD_RESET, false);
        MSS_MAC_phy_reset(phy_mac, MSS_MAC_HARD_RESET, true);

#if defined(TARGET_G5_SOC)
        /*
         * Multiple MACs may be involved and we need to ensure all relevant MACs
         * have the phy_hard_reset_done flag set
         */

        if (phy_mac != this_mac) /* Simple case, set flag for all 4 MACs */
        {
            g_mac0.phy_hard_reset_done = true;
            g_emac0.phy_hard_reset_done = true;
            g_mac1.phy_hard_reset_done = true;
            g_emac1.phy_hard_reset_done = true;
        }
        else if ((&g_mac0 == phy_mac) || (&g_emac0 == phy_mac))
        {
            g_mac0.phy_hard_reset_done = true;
            g_emac0.phy_hard_reset_done = true;
        }
        else
        {
            g_mac1.phy_hard_reset_done = true;
            g_emac1.phy_hard_reset_done = true;
        }
#else
        phy_mac->phy_hard_reset_done = true;
#endif
    }
#endif /* defined(MSS_MAC_PHY_HW_RESET) || defined(MSS_MAC_PHY_HW_RESET) */
    /* Select standard registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, MII_EXT_PAGE, 0x0000U);

    /*
     * Note from data sheet: Writes to Mii Register 23 must preserve the value
     * of bits 11:4, i.e., all writes to Mii Register 23 must be in the format
     * xA2xh.
     *
     * Select SGMII, CAT5, 625MHz Clk disabled, far end loopback disabled
     */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 23, (uint16_t)(0xAA22));

    /* Soft reset */
    temp_reg = MSS_MAC_read_phy_reg(this_mac, phy_addr, 0);
    temp_reg |= 0x8000;
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 0, temp_reg);

    /*
     * Need at least a 4uS pause here after setting reset bit before accessing
     * PHY again. Might need longer depending on some settings. Discovery board
     * for example loads initial config from EEPROM so minimum of 200mS.
     */

    if(this_mac->phy_flags & PHY_VSC8221_EEPROM_INIT)
    {
        MSS_MAC_delay(this_mac, 200000);
    }
    else
    {
        MSS_MAC_delay(this_mac, 4);
    }

    if (((struct mss_mac_instance *)v_this_mac == this_mac->phy_controller) ||
        ((struct mss_mac_instance *)0L == this_mac->phy_controller))
    {
        /* First take care of all the fixes and adjustments from the data sheet */
        vsc8221_post_reset_step(this_mac);
    }

    /* Select standard registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, MII_EXT_PAGE, 0x0000U);

    /*
     * For discovery board these are loaded from EEPROM but we show values to
     * load here.
     */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_BYPASS_CTRL,
                          (uint16_t)(0x0019));

    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_CTRL_STAT,
                          (uint16_t)(0x3200));

    /* SGMII to 1000BaseT, 625MHz clock disabled */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_PHY_CTRL_1,
                          (uint16_t)(0xAA23));

    /* IEEE mode for RX and TX - no jumbo frames  */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_PHY_CTRL_2,
                          (uint16_t)(0xE240));

    /*
     * LED 2 TX, LED 1 Link/Activity, LED 0 Error, 5KHz pulsing
     * EEPROM had LED 2 as Link/Activity but TX is more useful
     */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_LED_CTRL,
                          (uint16_t)(0xAE90));

    /* IEEE mode for RX and TX - no jumbo frames  */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_PHY_CTRL_2,
                          (uint16_t)(0xE240));

    /* Select extended registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, MII_EXT_PAGE, 0x0001U);

    /* SIGDET = active low output */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_SERDES_CTRL_2,
                          (uint16_t)(0x0002));

    /* 50R SERDES impedance */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_PHY_CTRL_3,
                          (uint16_t)(0x1045));

    /* IEEE Mode */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_EEPROM_STAT,
                          (uint16_t)(0x1800));

    /* PHY Address is 11 (RO), in-line powered device detection off */
    MSS_MAC_write_phy_reg(this_mac,
                          phy_addr,
                          MII_PHY_CTRL_4,
                          (uint16_t)(0x5800));

    /* Select standard registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, MII_EXT_PAGE, 0x0000U);

    if (MSS_MAC_SPEED_AN == this_mac->speed_mode)
    {
        /* Full duplex, autonegotiation and 1000Mbps as starting point */
        MSS_MAC_write_phy_reg(this_mac,
                              phy_addr,
                              MII_BMCR,
                              (uint16_t)(BMCR_ANENABLE | BMCR_FULLDPLX | BMCR_SPEED1000));
    }
    else
    {
        temp_reg = 0x0000U; /* Default with 10M, half duplex */

        if ((MSS_MAC_10_FDX == this_mac->speed_mode) || (MSS_MAC_100_FDX == this_mac->speed_mode) ||
            (MSS_MAC_1000_FDX == this_mac->speed_mode))
        {
            temp_reg |= BMCR_FULLDPLX;
        }

        if ((MSS_MAC_100_FDX == this_mac->speed_mode) || (MSS_MAC_100_HDX == this_mac->speed_mode))
        {
            temp_reg |= BMCR_SPEED100;
        }

        if ((MSS_MAC_1000_FDX == this_mac->speed_mode) ||
            (MSS_MAC_1000_HDX == this_mac->speed_mode))
        {
            temp_reg |= BMCR_SPEED1000;
        }

        /* Apply static speed/duplex selection */
        MSS_MAC_write_phy_reg(this_mac, phy_addr, MII_BMCR, temp_reg);
    }

    /* Full duplex modes */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, MII_ADVERTISE, (uint16_t)(ADVERTISE_FULL));

    /* Full duplex mode or half duplex, single port device */
    if ((MSS_MAC_10_FDX == this_mac->speed_mode) || (MSS_MAC_100_FDX == this_mac->speed_mode) ||
        (MSS_MAC_1000_FDX == this_mac->speed_mode))
    {
        MSS_MAC_write_phy_reg(this_mac,
                              phy_addr,
                              MII_CTRL1000,
                              (uint16_t)(ADVERTISE_1000FULL));
    }
    else
    {
        MSS_MAC_write_phy_reg(this_mac,
                              phy_addr,
                              MII_CTRL1000,
                              (uint16_t)(ADVERTISE_1000HALF));
    }

    phy_reg = (uint16_t)this_mac->mac_base->PCS_CONTROL;
    phy_reg |= 0x1000U;
    this_mac->mac_base->PCS_CONTROL = phy_reg;
    phy_reg |= 0x0200U;
    this_mac->mac_base->PCS_CONTROL = phy_reg;

    /* Wait for SGMII auto-negotiation to complete. */
    do
    {
        phy_reg = (uint16_t)this_mac->mac_base->PCS_STATUS;
        autoneg_complete = phy_reg & BMSR_AUTO_NEGOTIATION_COMPLETE;
        --sgmii_aneg_timeout;
    } while (((0U == autoneg_complete) && (0U != sgmii_aneg_timeout)) || (0xFFFFU == phy_reg));
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_VSC8221_phy_set_link_speed(/* mss_mac_instance_t */ void *v_this_mac,
                                   uint32_t speed_duplex_select,
                                   mss_mac_speed_mode_t speed_mode)
{
    mss_mac_instance_t *this_mac = (mss_mac_instance_t *)v_this_mac;
    uint16_t phy_reg;
    uint32_t inc;
    uint32_t speed_select;
    const uint16_t mii_advertise_bits[4] = {ADVERTISE_10FULL,
                                            ADVERTISE_10HALF,
                                            ADVERTISE_100FULL,
                                            ADVERTISE_100HALF};

    this_mac->speed_mode = speed_mode;

    if (MSS_MAC_SPEED_AN == speed_mode) /* Set auto-negotiation advertisement. */
    {
        /* Set 10Mbps and 100Mbps advertisement. */
        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_ADVERTISE);
        phy_reg &= (uint16_t)(~(ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF |
                                ADVERTISE_100FULL));

        speed_select = speed_duplex_select;
        for (inc = 0U; inc < 4U; ++inc)
        {
            uint32_t advertise;
            advertise = speed_select & 0x00000001U;
            if (advertise != 0U)
            {
                phy_reg |= mii_advertise_bits[inc];
            }
            speed_select = speed_select >> 1U;
        }

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_ADVERTISE, phy_reg);

        /* Set 1000Mbps advertisement. */
        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_CTRL1000);
        phy_reg &= (uint16_t)(~(ADVERTISE_1000FULL | ADVERTISE_1000HALF));

        if ((speed_duplex_select & MSS_MAC_ANEG_1000M_FD) != 0U)
        {
            phy_reg |= ADVERTISE_1000FULL;
        }

        if ((speed_duplex_select & MSS_MAC_ANEG_1000M_HD) != 0U)
        {
            phy_reg |= ADVERTISE_1000HALF;
        }

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_CTRL1000, phy_reg);
    }
    else
    {
        uint16_t temp_reg = 0x0000U; /* Default with 10M, half duplex */

        if ((MSS_MAC_10_FDX == this_mac->speed_mode) || (MSS_MAC_100_FDX == this_mac->speed_mode) ||
            (MSS_MAC_1000_FDX == this_mac->speed_mode))
        {
            temp_reg |= BMCR_FULLDPLX;
        }

        if ((MSS_MAC_100_FDX == this_mac->speed_mode) || (MSS_MAC_100_HDX == this_mac->speed_mode))
        {
            temp_reg |= BMCR_SPEED100;
        }

        if ((MSS_MAC_1000_FDX == this_mac->speed_mode) ||
            (MSS_MAC_1000_HDX == this_mac->speed_mode))
        {
            temp_reg |= BMCR_SPEED1000;
            /* Set Master mode */
            phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_CTRL1000);
            phy_reg |= 0x1800U;
            MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_CTRL1000, phy_reg);
        }

        /* Apply static speed/duplex selection */
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMCR, temp_reg);

        /* Full duplex mode or half duplex, single port device */
        if ((MSS_MAC_10_FDX == this_mac->speed_mode) || (MSS_MAC_100_FDX == this_mac->speed_mode) ||
            (MSS_MAC_1000_FDX == this_mac->speed_mode))
        {
            MSS_MAC_write_phy_reg(this_mac,
                                  (uint8_t)this_mac->phy_addr,
                                  MII_CTRL1000,
                                  (uint16_t)(ADVERTISE_1000FULL));
        }
        else
        {
            MSS_MAC_write_phy_reg(this_mac,
                                  (uint8_t)this_mac->phy_addr,
                                  MII_CTRL1000,
                                  (uint16_t)(ADVERTISE_1000HALF));
        }
    }
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_VSC8221_phy_autonegotiate(/* mss_mac_instance_t */ const void *v_this_mac)
{
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    volatile uint16_t phy_reg;
    uint16_t autoneg_complete;
    volatile uint32_t copper_aneg_timeout = 100000U;

    if (MSS_MAC_SPEED_AN == this_mac->speed_mode) /* Only do if allowed */
    {
        /* Select standard registers page */
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_EXT_PAGE, 0x0000U);

        /* Start off auto-negotiation. */
        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMCR);
        phy_reg |= 0x0200U;
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMCR, phy_reg);

        /* Wait for copper auto-negotiation to complete. */
        do
        {
            phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMSR);
            autoneg_complete = phy_reg & BMSR_AUTO_NEGOTIATION_COMPLETE;
            --copper_aneg_timeout;
        } while (((0U == autoneg_complete) && (copper_aneg_timeout != 0u)) || (0xFFFF == phy_reg));
    }
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_VSC8221_mac_autonegotiate(/* mss_mac_instance_t */ const void *v_this_mac)
{
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    volatile uint16_t phy_reg;
    uint16_t autoneg_complete;
    volatile uint32_t copper_aneg_timeout = 10000U;

    /* Enable auto-negotiation. */
    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 27);
    phy_reg |= 0x1000U;
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 27, phy_reg);

    if (0U == this_mac->is_emac)
    {
        volatile uint32_t temp_reg;

        temp_reg = this_mac->mac_base->PCS_CONTROL;
        this_mac->mac_base->PCS_CONTROL = temp_reg | GEM_RESTART_AUTO_NEG;
    }

    /* Wait for SGMII auto-negotiation to complete. */
    do
    {
        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 27);
        autoneg_complete = phy_reg & 0x0002U;
        --copper_aneg_timeout;
    } while (((0U == autoneg_complete) && (copper_aneg_timeout != 0u)) || (0xFFFF == phy_reg));
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
uint8_t
MSS_MAC_VSC8221_phy_get_link_status(
    /* mss_mac_instance_t */ const void *v_this_mac,
    mss_mac_speed_t *speed,
    uint8_t *fullduplex)
{
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    uint16_t phy_reg;
    uint16_t link_up;
    uint8_t link_status;

    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMSR);
    link_up = phy_reg & BMSR_LSTATUS;

    if (link_up != MSS_MAC_LINK_DOWN)
    {
        uint16_t duplex;
        uint16_t speed_field;

        /* Link is up. */
        link_status = MSS_MAC_LINK_UP;

        phy_reg = MSS_MAC_read_phy_reg(this_mac,
                                       (uint8_t)this_mac->phy_addr,
                                       0x1CU); /* Device Auxillary Control and Status */
        duplex = phy_reg & 0x0020U;
        speed_field = phy_reg & 0x0018U;

        if (MSS_MAC_HALF_DUPLEX == duplex)
        {
            *fullduplex = MSS_MAC_HALF_DUPLEX;
        }
        else
        {
            *fullduplex = MSS_MAC_FULL_DUPLEX;
        }

        switch (speed_field >> 3)
        {
            case 0U:
                *speed = MSS_MAC_10MBPS;
                break;

            case 1U:
                *speed = MSS_MAC_100MBPS;
                break;

            case 2U:
                *speed = MSS_MAC_1000MBPS;
                break;

            default:
                link_status = (uint8_t)MSS_MAC_LINK_DOWN;
                break;
        }
    }
    else
    {
        /* Link is down. */
        link_status = (uint8_t)MSS_MAC_LINK_DOWN;
    }

    return link_status;
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
uint16_t VSC8221_reg_0[32];
uint16_t VSC8221_reg_1[16];

void dump_vsc8221_regs(const mss_mac_instance_t *this_mac);
void
dump_vsc8221_regs(const mss_mac_instance_t *this_mac)
{
    int32_t count;
    uint16_t page;
    uint16_t old_page;
    uint16_t *pdata;
    volatile psr_t lev;

    for (page = 0U; page <= 0x1U; page++)
    {
        if (0U == page)
        {
            pdata = VSC8221_reg_0;
        }
        else if (1U == page)
        {
            pdata = VSC8221_reg_1;
        }

        if (0U == page)
        {
            for (count = 0; count <= 0x1F; count++)
            {
                lev = HAL_disable_interrupts();
                old_page = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU);

                MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, page);

                pdata[count] =
                    MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, (uint8_t)count);

                MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, old_page);
                HAL_restore_interrupts(lev);
            }
        }
        else
        {
            for (count = 0x10; count <= 0x1F; count++)
            {
                lev = HAL_disable_interrupts();
                old_page = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU);

                MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, page);

                pdata[count - 0X10] =
                    MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, (uint8_t)count);

                MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, old_page);
                HAL_restore_interrupts(lev);
            }
        }
    }
}

#endif /* #if defined(TARGET_ALOE) */
#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
