/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Realtek RTL8211FS-CG PHY interface driver implementation for use with the
 * BeagleV-Fire board.
 *
 * This system uses the SGMII interface.
 *
 * The following are the default selections from the hardware strapping:
 *
 * CFG_MODE[0 - 2] - Strap option 011, UTP <=> SGMII
 * PHYADD[0-2]     - Strap option 001, Address 1
 *  *
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

#if MSS_MAC_USE_PHY_RTL8211
/**************************************************************************/
/* Preprocessor Macros                                                    */
/**************************************************************************/

#define BMSR_AUTO_NEGOTIATION_COMPLETE (0x0020U)

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_RTL8211_phy_init(/* mss_mac_instance_t */ const void *v_this_mac, uint8_t phy_addr)
{
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    uint16_t phy_reg;
    (void)phy_addr;

    /* Start by doing a software reset of the PHY */
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMCR, BMCR_RESET);

    /* Reset SGMII core side of I/F. */
    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, MII_BMCR);
    phy_reg |= 0x9000U; /* Reset and start autonegotiation */
    phy_reg &= 0xFBFFU; /* Clear Isolate bit */
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, MII_BMCR, phy_reg);

    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, MII_BMCR);
    phy_reg &= 0xFBFFU; /* Clear Isolate bit */
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, MII_BMCR, phy_reg);

    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, MII_BMCR);
    phy_reg |= 0x1000U; /* Kick off autonegotiation - belt and braces approach...*/
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, MII_BMCR, phy_reg);
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_RTL8211_phy_set_link_speed(/* mss_mac_instance_t */ void *v_this_mac,
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
        /* Set auto-negotiation advertisement. */

        /* Set 10Mbps and 100Mbps advertisement. */
        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_ADVERTISE);
        phy_reg &= (uint16_t)(~(ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF |
                                ADVERTISE_100FULL));

        phy_reg |= 0x0C00U; /* Set Asymmetric pause and symmetric pause bits */

        speed_select = speed_duplex_select;
        for (inc = 0U; inc < 4U; ++inc)
        {
            uint32_t advertise;
            advertise = speed_select & 0x00000001U;
            if (advertise != 0U)
            {
                phy_reg |= mii_advertise_bits[inc];
            }
            speed_select = speed_select >> 1;
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
MSS_MAC_RTL8211_phy_autonegotiate(/* mss_mac_instance_t */ const void *v_this_mac)
{
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    volatile uint16_t phy_reg;
    uint16_t autoneg_complete;
    volatile uint32_t copper_aneg_timeout = 1000000U;
    volatile uint32_t sgmii_aneg_timeout = 100000U;

    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 2);
    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 3);

    /* Enable auto-negotiation. */
    phy_reg = 0x1340U;
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMCR, phy_reg);

    /* Wait for copper auto-negotiation to complete. */
    do
    {
        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMSR);
        autoneg_complete = phy_reg & BMSR_AUTO_NEGOTIATION_COMPLETE;
        --copper_aneg_timeout;
    } while ((0u == autoneg_complete) && (0U != copper_aneg_timeout) && (0xFFFFU != phy_reg));

    if (GMII_SGMII == this_mac->interface_type)
    {
        /* Initiate auto-negotiation on the SGMII link. */
        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, 0x00U);
        phy_reg |= 0x1000U;
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, 0x00U, phy_reg);
        phy_reg |= 0x0200U;
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, 0x00U, phy_reg);

        /* Wait for SGMII auto-negotiation to complete. */
        do
        {
            phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, MII_BMSR);
            autoneg_complete = phy_reg & BMSR_AUTO_NEGOTIATION_COMPLETE;
            --sgmii_aneg_timeout;
        } while ((0U == autoneg_complete) && (0U != sgmii_aneg_timeout));
    }
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_RTL8211_mac_autonegotiate(/* mss_mac_instance_t */ const void *v_this_mac)
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
MSS_MAC_RTL8211_phy_get_link_status(
    /* mss_mac_instance_t*/ const void *v_this_mac,
    mss_mac_speed_t *speed,
    uint8_t *fullduplex)
{
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    uint16_t phy_reg;
    uint16_t link_up;
    uint16_t old_page;
    uint8_t link_status;
    volatile psr_t lev;

    /* Read twice to get current link status */
    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMSR);
    phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, MII_BMSR);

    link_up = phy_reg & BMSR_LSTATUS;

    if (link_up != MSS_MAC_LINK_DOWN)
    {
        uint16_t duplex;
        uint16_t speed_field;

        /* Link is up. */
        link_status = MSS_MAC_LINK_UP;

        lev = HAL_disable_interrupts();
        old_page = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, 0x0a43U);

        phy_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1AU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, old_page);
        HAL_restore_interrupts(lev);

        duplex = phy_reg & 0x0008U;
        speed_field = phy_reg >> 4;

        if (MSS_MAC_HALF_DUPLEX == duplex)
        {
            *fullduplex = MSS_MAC_HALF_DUPLEX;
        }
        else
        {
            *fullduplex = MSS_MAC_FULL_DUPLEX;
        }

        switch (speed_field & 3)
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

/*
 * RTL8211FS-CG PHY has 32 standard registers plus additional registers accessed
 * via pages set from register 0x1F. Additionally there are MMD registers
 * accessed indirectly through registers 0x0D and 0x0E. Register 0x0D (REGCR)
 * holds the control bits which determine what device is selected and what
 * register 0x0E (ADDAR) does.
 *
 * The following extended registers are available:
 *
 * Device : Offset : Access : Name    : Description
 *    3   :    0   :   RW   : PC1R    : PCS Control 1 Register
 *    3   :    1   :   RW   : PS1R    : PCS Status 1 Register
 *    3   :   20   :   RO   : EEECR   : EEE Capability Register
 *    3   :   22   :   RC   : EEEWER  : EEE Wake Error Register
 *    7   :   60   :   RW   : EEEAR   : EEE Advertisement Register
 *    7   :   61   :   RO   : EEELPAR : EEE Link Pertner Ability Register
 *
 */

uint16_t rtl_phy_reg_list[14] = {
    0x0003U,
    0x0000U, /* 00 PCS Control 1 Register */
    0x0003U,
    0x0001U, /* 01 PCS Control 1 Register */
    0x0003U,
    0x0014U, /* 02 PCS Control 1 Register */
    0x0003U,
    0x0016U, /* 03 PCS Control 1 Register */
    0x0007U,
    0x003CU, /* 04 PCS Control 1 Register */
    0x0007U,
    0x003DU, /* 05 PCS Control 1 Register */
    0xFFFFU,
    0xFFFFU /* 06 End of list... */
};

uint16_t RTL_reg_0[32];
uint16_t RTL_reg_1[16];
uint16_t RTL_reg_2[6];
uint16_t RTL_MSS_SGMII_reg[17];
uint32_t RTL_MSS_MAC_reg[80];
uint32_t RTL_MSS_PLIC_REG[80];

void dump_rtl_regs(const mss_mac_instance_t *this_mac);
void
dump_rtl_regs(const mss_mac_instance_t *this_mac)
{
    int32_t count;
    uint16_t old_page;
    uint16_t old_ctrl;
    uint16_t old_addr;
    uint32_t *pbigdata;
    volatile psr_t lev;

    for (count = 0; count <= 0x1F; count++)
    {
        lev = HAL_disable_interrupts();
        RTL_reg_0[count] =
            MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, (uint8_t)count);
        HAL_restore_interrupts(lev);
    }

    for (count = 0x10; count <= 0x1F; count++)
    {
        lev = HAL_disable_interrupts();
        old_page = MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, 0x0a43U);

        RTL_reg_1[count - 16] =
            MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, (uint8_t)count);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 0x1FU, old_page);
        HAL_restore_interrupts(lev);
    }

    lev = HAL_disable_interrupts();
    old_ctrl = MSS_MAC_read_phy_reg(this_mac,
                                    (uint8_t)this_mac->phy_addr,
                                    MII_REGCR); /* Fetch current REGCR value */
    MSS_MAC_write_phy_reg(this_mac,
                          (uint8_t)this_mac->phy_addr,
                          MII_REGCR,
                          rtl_phy_reg_list[0]); /* Select Address mode */
    old_addr = MSS_MAC_read_phy_reg(this_mac,
                                    (uint8_t)this_mac->phy_addr,
                                    MII_ADDAR); /* Fetch current indirect address */
    HAL_restore_interrupts(lev);

    for (count = 0; 0xFFFFU != rtl_phy_reg_list[count]; count += 2)
    {
        lev = HAL_disable_interrupts();
        MSS_MAC_write_phy_reg(this_mac,
                              (uint8_t)this_mac->phy_addr,
                              MII_REGCR,
                              rtl_phy_reg_list[count]); /* Select Address mode */
        MSS_MAC_write_phy_reg(this_mac,
                              (uint8_t)this_mac->phy_addr,
                              MII_ADDAR,
                              rtl_phy_reg_list[count + 1]); /* Select new indirect Address */
        MSS_MAC_write_phy_reg(this_mac,
                              (uint8_t)this_mac->phy_addr,
                              MII_REGCR,
                              0x4000U | rtl_phy_reg_list[count]); /* Select simple data mode */
        RTL_reg_2[count >> 1] = MSS_MAC_read_phy_reg(this_mac,
                                                     (uint8_t)this_mac->phy_addr,
                                                     MII_ADDAR); /* Finally, read the data */
        HAL_restore_interrupts(lev);
    }

    lev = HAL_disable_interrupts();
    MSS_MAC_write_phy_reg(this_mac,
                          (uint8_t)this_mac->phy_addr,
                          MII_REGCR,
                          rtl_phy_reg_list[0]); /* Select Address mode */
    MSS_MAC_write_phy_reg(this_mac,
                          (uint8_t)this_mac->phy_addr,
                          MII_ADDAR,
                          old_addr); /* Restore old address */
    MSS_MAC_write_phy_reg(this_mac,
                          (uint8_t)this_mac->phy_addr,
                          MII_REGCR,
                          old_ctrl); /* Restore old control mode */
    HAL_restore_interrupts(lev);

    for (count = 0; count <= 0x10; count++)
    {
        RTL_MSS_SGMII_reg[count] =
            MSS_MAC_read_phy_reg(this_mac, (uint8_t)this_mac->pcs_phy_addr, (uint8_t)count);
    }

    pbigdata = (uint32_t *)0x20110000UL;
    for (count = 0; count < 19; count++)
    {
        RTL_MSS_MAC_reg[count] = *pbigdata;
        pbigdata++;
    }
    pbigdata = (uint32_t *)0x0C000000UL;
    for (count = 0; count < 8; count++)
    {
        RTL_MSS_PLIC_REG[count] = *pbigdata;
        pbigdata++;
    }
}

#endif /* MSS_MAC_USE_PHY_RTL8211 */
#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
