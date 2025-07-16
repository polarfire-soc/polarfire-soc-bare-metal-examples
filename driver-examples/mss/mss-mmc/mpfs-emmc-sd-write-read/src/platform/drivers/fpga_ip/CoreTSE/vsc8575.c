
/*******************************************************************************
 * Copyright 2018 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file vsc8575.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Microchip's VSC8575 PHY driver implementation.
 *
 */

#include "phy.h"
#include "core_tse.h"
#include "coretse_regs.h"
#include "hal/hal.h"
#include "hal/hal_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VSC8575_PHY)

#define PAGE_0 0x00

/* function prototypes ********************************************************/
static void set_page(tse_instance_t *this_tse, uint8_t phy_addr, uint16_t page);

/* functions ******************************************************************/
void
TSE_phy_init(tse_instance_t *this_tse, uint8_t phy_addr)
{
    volatile uint16_t phy_reg;
    volatile uint16_t id1 = 0, id2 = 0;

    set_page(this_tse, phy_addr, 0x0000);

    /* 16E3 bit 7 setting to 1 for SERDES MAC AN EN */
    set_page(this_tse, phy_addr, 0x0003);
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x10);
    phy_reg |= 0x80;
    TSE_write_phy_reg(this_tse, phy_addr, 0x10, phy_reg);

    /* set microprocessor command to enable two MAC SGMII ports (reg 18G) */
    set_page(this_tse, phy_addr, 0x0010);
    phy_reg = 0x80F0;
    TSE_write_phy_reg(this_tse, phy_addr, 0x12, phy_reg);
    phy_reg = 0x8000;
    while ((phy_reg & 0x8000) != 0)
        phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x12);

    /* phy software reset (mode control register) */
    set_page(this_tse, phy_addr, 0x0000);
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x00);
    phy_reg = phy_reg | 0x8000;
    TSE_write_phy_reg(this_tse, phy_addr, 0x00, phy_reg);
    phy_reg = 0x8000;
    while ((phy_reg & 0x8000) != 0)
        phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x00);

    set_page(this_tse, phy_addr, 0x0000);
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x04);
    phy_reg &= ~(0x1E);
    TSE_write_phy_reg(this_tse, phy_addr, 0x04, phy_reg);

    /* 1000Base Control: FDX */
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x09);
    phy_reg |= 0x200;
    TSE_write_phy_reg(this_tse, phy_addr, 0x09, phy_reg);
}

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
            phy_reg |= mii_advertise_bits[inc];

        speed_select = speed_select >> 1u;
    }

    TSE_write_phy_reg(this_tse, phy_addr, MII_ADVERTISE, phy_reg);

    /* Set 1000Mbps advertisement. */
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_CTRL1000);
    phy_reg &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);

    if ((speed_duplex_select & TSE_ANEG_1000M_FD) != 0u)
        phy_reg |= ADVERTISE_1000FULL;

    if ((speed_duplex_select & TSE_ANEG_1000M_HD) != 0u)
        phy_reg |= ADVERTISE_1000HALF;

    TSE_write_phy_reg(this_tse, phy_addr, MII_CTRL1000, phy_reg);
}

void
TSE_phy_autonegotiate(tse_instance_t *this_tse, uint8_t phy_addr)
{
    uint32_t phy_reg;
    uint16_t autoneg_complete;
    volatile uint32_t copper_aneg_timeout = 1000000u;
    volatile uint32_t sgmii_aneg_timeout = 1000000u;

    set_page(this_tse, phy_addr, 0x0000);
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x00);
    phy_reg |= 0x1200;
    TSE_write_phy_reg(this_tse, phy_addr, 0x00, phy_reg);

    do
    {
        phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x01);
        autoneg_complete = phy_reg & 0x0020u;
        --copper_aneg_timeout;
    } while (!autoneg_complete && (copper_aneg_timeout != 0u));

    /* if copper link up */
    if ((phy_reg & 0x0004) != 0u)
    {
        uint8_t phy2_addr = 0x12;
        /* enable autonegotiation */
        phy_reg = TSE_read_phy_reg(this_tse, phy2_addr, 0x00);
        phy_reg |= 0x1000;
        TSE_write_phy_reg(this_tse, phy2_addr, 0x00, phy_reg);

        /* restart autonegotiation */
        phy_reg = TSE_read_phy_reg(this_tse, phy2_addr, 0x00);
        phy_reg |= 0x0200;
        TSE_write_phy_reg(this_tse, phy2_addr, 0x00, phy_reg);

        do
        {
            phy_reg = TSE_read_phy_reg(this_tse, phy2_addr, 0x01);
            autoneg_complete = phy_reg & 0x0020;
            --sgmii_aneg_timeout;
        } while ((!autoneg_complete) && (sgmii_aneg_timeout != 0u));
    }
}

uint8_t
TSE_phy_get_link_status(tse_instance_t *this_tse,
                        uint8_t phy_addr,
                        tse_speed_t *speed,
                        uint8_t *fullduplex)
{
    volatile uint16_t phy_reg;
    uint16_t copper_link_up;
    uint8_t link_status;

    /* Find out if link is up between PHY and remote device.*/
    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, MII_BMSR);
    copper_link_up = phy_reg & BMSR_LSTATUS;

    if (copper_link_up != TSE_LINK_DOWN)
    {
        uint16_t duplex;
        uint16_t phy_speed;

        /* Link is up. */
        link_status = TSE_LINK_UP;

        /* Reg 28: aux. ctrl & stats */
        phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x1c);
        /* FDX status - Bit 5 */
        duplex = phy_reg & (1 << 5);
        /* Speed status - Bit 4..3 */
        phy_speed = phy_reg & 0x0018;

        if (TSE_HALF_DUPLEX == duplex)
            *fullduplex = TSE_HALF_DUPLEX;
        else
            *fullduplex = TSE_FULL_DUPLEX;

        switch (phy_speed)
        {
            case 0x0010:
                *speed = TSE_MAC1000MBPS;
                break;

            case 0x0008:
                *speed = TSE_MAC100MBPS;
                break;

            case 0x0000:
                *speed = TSE_MAC10MBPS;
                break;
            default:
                *speed = TSE_INVALID_SPEED;
                break;
        }
    }
    else
    { /* Link is down. */
        link_status = TSE_LINK_DOWN;
    }

    return link_status;
}

static void
set_page(tse_instance_t *this_tse, uint8_t phy_addr, uint16_t page)
{
    TSE_write_phy_reg(this_tse, phy_addr, 0x1F, page);
}

void
phy_far_end_loopback(tse_instance_t *this_tse, uint8_t phy_addr, int mode)
{
    uint16_t phy_reg;

    phy_reg = TSE_read_phy_reg(this_tse, phy_addr, 0x17);

    if (mode == 1)
        phy_reg = phy_reg | 0x0008;
    else
        phy_reg = phy_reg & ~0x0008;

    TSE_write_phy_reg(this_tse, phy_addr, 0x17, phy_reg);
}

#endif

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
