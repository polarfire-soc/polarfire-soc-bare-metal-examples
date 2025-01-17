
/*******************************************************************************
 * Copyright 2014 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file m88e1340_phy.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Marvell's M88E1340 PHY interface driver implementation.
 *
 */

#include "phy.h"
#include "core_tse.h"
#include "coretse_regs.h"
#include "hal/hal.h"
#include "hal/hal_assert.h"

#ifndef LEGACY_DIR_STRUCTURE
#include "fpga_design_config/fpga_design_config.h"
#else
#include "hw_platform.h"
#endif  /*LEGACY_DIR_STRUCTURE*/

#ifdef __cplusplus
extern "C" {
#endif


#if defined(M88E1340_PHY)
/***************************************************************************/ /**
                                                                               * M88E1340 PHY
                                                                               * specific register
                                                                               * offsets and bit
                                                                               * definitions
                                                                               */
#ifdef SYS_CLK_FREQ
#define SYSTEM_CORE_CLOCK (SYS_CLK_FREQ)
#endif
#ifndef SYS_CLK_FREQ
#define SYSTEM_CORE_CLOCK (LIBERO_SETTING_MSS_APB_AHB_CLK)
#endif

#define M88E1340_EXT_ADDR_PAGE_CR      0x16
#define PAGE_0                         0x00

#define M88E1340_PHY_STATUS            0x11
#define M88E1340_PHY_STATUS_1000       0x8000
#define M88E1340_PHY_STATUS_100        0x4000
#define M88E1340_PHY_STATUS_10         0x0000
#define M88E1340_PHY_STATUS_SPD_MASK   0xc000
#define M88E1340_PHY_STATUS_FULLDUPLEX 0x2000
#define M88E1340_PHY_STATUS_RESOLVED   0x0800
#define M88E1340_PHY_STATUS_LINK       0x0400

/***************************************************************************/ /**
                                                                               * Preprocessor Macros
                                                                               */
#define BMSR_AUTO_NEGOTIATION_COMPLETE 0x0020u

/***************************************************************************/ /**
                                                                               *
                                                                               */
void
TSE_phy_init(tse_instance_t *this_tse, uint8_t phy_addr)
{
    volatile uint16_t phy_reg;

    /* Reset the PHY. */
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_BMCR);
    phy_reg |= BMCR_RESET;
    TSE_write_phy_reg(this_tse, phy_addr, MII_BMCR, phy_reg);

    /* Power up the PHY. */
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_BMCR);
    phy_reg &= ~BMCR_PDOWN;
    TSE_write_phy_reg(this_tse, phy_addr, MII_BMCR, phy_reg);
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
TSE_phy_set_link_speed(tse_instance_t *this_tse, uint8_t phy_addr, uint32_t speed_duplex_select)
{
    uint16_t phy_reg;
    uint32_t inc;
    uint32_t speed_select;
    uint16_t const mii_advertise_bits[4] = {ADVERTISE_10FULL,
                                            ADVERTISE_10HALF,
                                            ADVERTISE_100FULL,
                                            ADVERTISE_100HALF};

    /* Set 10Mbps and 100Mbps advertisement. */
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_ADVERTISE);
    phy_reg &= ~(ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF | ADVERTISE_100FULL);

    speed_select = speed_duplex_select;
    for (inc = 0u; inc < 4u; ++inc)
    {
        uint32_t advertise;
        advertise = speed_select & 0x00000001u;
        if (advertise != 0u)
        {
            phy_reg |= mii_advertise_bits[inc];
        }
        speed_select = speed_select >> 1u;
    }

    TSE_write_phy_reg(this_tse, phy_addr, MII_ADVERTISE, phy_reg);

    /* Set 1000Mbps advertisement. */
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_CTRL1000);
    phy_reg &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);

    if ((speed_duplex_select & TSE_ANEG_1000M_FD) != 0u)
    {
        phy_reg |= ADVERTISE_1000FULL;
    }

    TSE_write_phy_reg(this_tse, phy_addr, MII_CTRL1000, phy_reg);
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
TSE_phy_autonegotiate(tse_instance_t *this_tse, uint8_t phy_addr)
{
    volatile uint16_t phy_reg;
    uint16_t autoneg_complete;
    uint32_t volatile copper_aneg_timeout = 1000000u;

    /* Enable auto-negotiation. */
    TSE_write_phy_reg(this_tse, phy_addr, M88E1340_EXT_ADDR_PAGE_CR, PAGE_0);

    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_BMCR);
    phy_reg |= (BMCR_ANENABLE | BMCR_ANRESTART);
    TSE_write_phy_reg(this_tse, phy_addr, MII_BMCR, phy_reg);

    /* Wait for copper auto-negotiation to complete. */
    do
    {
        phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_BMSR);
        autoneg_complete = phy_reg & BMSR_AUTO_NEGOTIATION_COMPLETE;
        --copper_aneg_timeout;
    } while ((!autoneg_complete && copper_aneg_timeout != 0u) || (0xFFFF == phy_reg));

    {
        volatile uint32_t delay = SYSTEM_CORE_CLOCK / 256u;
        while (delay != 0)
        {
            --delay;
        }
    }
}

/***************************************************************************/ /**
                                                                               *
                                                                               */
uint8_t
TSE_phy_get_link_status(tse_instance_t *this_tse,
                        uint8_t phy_addr,
                        tse_speed_t *speed,
                        uint8_t *fullduplex)
{
    volatile uint16_t phy_reg;
    uint16_t copper_link_up;
    uint8_t link_status;

    /* Find out if link is up between Marvell PHY and remote device.*/
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_BMSR);
    copper_link_up = phy_reg & BMSR_LSTATUS;

    if (copper_link_up != TSE_LINK_DOWN)
    {
        uint16_t duplex;
        uint16_t phy_speed;

        /* Link is up. */
        link_status = TSE_LINK_UP;

        phy_reg = TSE_read_phy_reg(this_tse, phy_addr, M88E1340_PHY_STATUS);
        duplex = phy_reg & M88E1340_PHY_STATUS_FULLDUPLEX;
        phy_speed = phy_reg & M88E1340_PHY_STATUS_SPD_MASK;

        if (TSE_HALF_DUPLEX == duplex)
        {
            *fullduplex = TSE_HALF_DUPLEX;
        }
        else
        {
            *fullduplex = TSE_FULL_DUPLEX;
        }

        switch (phy_speed)
        {
            case M88E1340_PHY_STATUS_1000:
                *speed = TSE_MAC1000MBPS;
                break;

            case M88E1340_PHY_STATUS_100:
                *speed = TSE_MAC100MBPS;
                break;

            case M88E1340_PHY_STATUS_10:
                *speed = TSE_MAC10MBPS;
                break;
            default:
                *speed = TSE_INVALID_SPEED;
                break;
        }
    }
    else
    {
        /* Link is down. */
        link_status = TSE_LINK_DOWN;
    }

    return link_status;
}

#endif

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
