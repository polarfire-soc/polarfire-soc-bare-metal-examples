/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file vsc8662_phy.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Microsemi VSC8662 PHY interface driver implementation to support the
 * silicon validation board.
 *
 * Will need to modify FW based on MSS GPIO configuration.
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

#if MSS_MAC_USE_PHY_VSC8662

/**************************************************************************/
/* Preprocessor Macros                                                    */
/**************************************************************************/

#define BMSR_AUTO_NEGOTIATION_COMPLETE (0x0020U)

/**************************************************************************/ /**
                                                                              * Ensure ports 2 and 3
                                                                              * are shut down as
                                                                              * they are not bonded
                                                                              * out for the VSC8862.
                                                                              */
static void
vsc8662_post_reset_step_01(const mss_mac_instance_t *this_mac)
{
    uint32_t phy_base_address;

    phy_base_address = this_mac->phy_addr & 0xFCU; /* Calculate base address for chip */
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 31, 0x0000U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 2), 0, 0x0800U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 3), 31, 0x0000U);
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + 3), 0, 0x0800U);
}

/**************************************************************************/ /**
                                                                              * Enable broadcast
                                                                              * writes for the
                                                                              * VSC8662
                                                                              */
static void
vsc8662_enable_bcast_writes(const mss_mac_instance_t *this_mac)
{
    uint32_t phy_base_address;
    uint16_t temp_reg;

    phy_base_address = this_mac->phy_addr & 0xFCU; /* Calculate base address for chip */
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address), 31, 0x0000U);
    temp_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)(phy_base_address), 22);
    temp_reg |= 0x0001U;
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address), 22, temp_reg);
}

/**************************************************************************/ /**
                                                                              * Disable broadcast
                                                                              * writes for the
                                                                              * VSC8662
                                                                              */
static void
vsc8662_disable_bcast_writes(const mss_mac_instance_t *this_mac)
{
    uint32_t phy_base_address;
    uint16_t temp_reg;

    phy_base_address = this_mac->phy_addr & 0xFCU; /* Calculate base address for chip */
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address), 31, 0x0000U);
    temp_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)(phy_base_address), 22);
    temp_reg &= 0xFFFEU;
    MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address), 22, temp_reg);
}

/**************************************************************************/ /**
                                                                              * Enable LED blinking
                                                                              * for the VSC8662
                                                                              */
static void
vsc8662_enable_LED_blink(const mss_mac_instance_t *this_mac)
{
    uint32_t phy_base_address;
    uint16_t temp_reg;
    uint32_t count;

    phy_base_address = this_mac->phy_addr & 0xFCU; /* Calculate base address for chip */

    for (count = 0; count != 2; count++)
    {
        /* Enable LED blink port[count] */
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x0001U);
        temp_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 19);
        temp_reg |= 0x0800U;
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 19, temp_reg);

        /* Soft reset port[count] */
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x0000U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 0, 0x9040U);
    }
}

/**************************************************************************/ /**
                                                                              * Apply 100/1000BASE-T
                                                                              * amplitude correction
                                                                              * fix for the VSC8662.
                                                                              */
static void
vsc8662_100tx_1000t_amplitude_fix(const mss_mac_instance_t *this_mac)
{
    uint32_t phy_base_address;
    uint16_t temp_reg;
    uint32_t count;

    phy_base_address = this_mac->phy_addr & 0xFCU; /* Calculate base address for chip */

    for (count = 0; count != 2; count++)
    {
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x52B5U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0000U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x003FU);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x8794U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x00F7U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0xADB4U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x879EU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0000U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0032U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87A0U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0041U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0410U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87A2U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0041U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0410U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87A4U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0041U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0284U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87A6U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0092U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0xBCB8U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87A8U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0003U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0xCFBFU);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87AAU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0049U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x2451U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87ACU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0001U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x1410U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87C0U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0010U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0xB498U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87E8U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0071U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0xE7DDU);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87EAU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0069U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x6512U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87ECU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0049U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x2451U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87EEU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0045U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0410U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87F0U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0041U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0410U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87F2U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0000U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0010U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x87F4U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x2A30U);
        temp_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 9);
        temp_reg = (uint16_t)((temp_reg & 0xFFFFU) | 0x0040U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 9, temp_reg);

        temp_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 22);
        temp_reg = (uint16_t)((temp_reg & 0xFFFFU) | 0x0010U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 22, temp_reg);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x0000U);
    }
}

/**************************************************************************/ /**
                                                                              * Apply 10BASE-T
                                                                              * performance fix for
                                                                              * the VSC8662.
                                                                              */
static void
vsc8662_10t_performance_fix(const mss_mac_instance_t *this_mac)
{
    uint32_t phy_base_address;
    uint16_t temp_reg;
    uint32_t count;

    phy_base_address = this_mac->phy_addr & 0xFCU; /* Calculate base address for chip */

    for (count = 0; count != 2; count++)
    {
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x2A30U);
        temp_reg = MSS_MAC_read_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 0);
        temp_reg = (uint16_t)((temp_reg & 0xFFEFU) | 0x0060U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 0, temp_reg);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x52B5U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0012U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x480AU);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x8F82U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0000U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x0422U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x8F86U);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x003CU);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0x3800U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x8F8AU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 18, 0x0008U);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 17, 0xE33FU);
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 16, 0x83AEU);

        MSS_MAC_write_phy_reg(this_mac, (uint8_t)(phy_base_address + count), 31, 0x0000U);
    }
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_VSC8662_phy_init(/* mss_mac_instance_t*/ const void *v_this_mac, uint8_t phy_addr)
{
    uint16_t temp_reg;
    volatile uint16_t phy_reg;
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    volatile uint32_t sgmii_aneg_timeout = 100000U;
    uint16_t autoneg_complete;
    volatile uint64_t timer_count = 1000000U;

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
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 31, 0x0000U);

    /*
     * Enable SGMII MAC link autonegotiation and Force copper media only
     *
     * According to a VSC8664 app note this register needs to be updated first
     * followed by a soft reset, the scripts and finally the rest of the user
     * setup.
     */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 23, (uint16_t)(0x2880U));

    /* Soft reset */
    temp_reg = MSS_MAC_read_phy_reg(this_mac, phy_addr, 0);
    temp_reg |= 0x8000;
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 0, temp_reg);

    /*
     * Need at least a 4uS pause here after setting reset bit before accessing
     * PHY again
     */
    while (0 != timer_count)
    {
        --timer_count;
    }

    if (((struct mss_mac_instance *)v_this_mac == this_mac->phy_controller) ||
        ((struct mss_mac_instance *)0L == this_mac->phy_controller))
    {
        /* First take care of all the fixes and adjustments from the data sheet */
        vsc8662_post_reset_step_01(this_mac);
        vsc8662_enable_LED_blink(this_mac);
        vsc8662_100tx_1000t_amplitude_fix(this_mac);
        vsc8662_10t_performance_fix(this_mac);
    }
    /* Select standard registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 31, 0x0000U);

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

    /* Full duplex mode or half duplex, multi port device */
    if ((MSS_MAC_10_FDX == this_mac->speed_mode) || (MSS_MAC_100_FDX == this_mac->speed_mode) ||
        (MSS_MAC_1000_FDX == this_mac->speed_mode))
    {
        MSS_MAC_write_phy_reg(this_mac,
                              phy_addr,
                              MII_CTRL1000,
                              (uint16_t)(ADVERTISE_1000FULL | 0x0400U));
    }
    else
    {
        MSS_MAC_write_phy_reg(this_mac,
                              phy_addr,
                              MII_CTRL1000,
                              (uint16_t)(ADVERTISE_1000HALF | 0x0400U));
    }
    /* Auto MDI/MDI-X, Do not ignore advertised ability, enable CLKOUT */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 18, (uint16_t)(0x0089U));

    /* SGMII no input preamble, 2 byte output preamble, 9/12K jumbo frames (12K for 60ppm clock) */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 24, (uint16_t)(0xC050U));

    /* Select selection of LED activity modes for 4 LEDs */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 29, (uint16_t)(0x0123U));

    /* Clear this for consistency as many bits are CMODE selections */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 30, (uint16_t)(0x0000U));

    /* Select extended registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 31, 0x0001U);

    /* Enable RX Equalization and Hysteresis on SERDES interface */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 11, (uint16_t)(0x0292U));

    /* Clear SIGDET polarity to active high */
    temp_reg = MSS_MAC_read_phy_reg(this_mac, phy_addr, 19);
    temp_reg &= 0xFFFE;
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 19, temp_reg);

    /* Ensure all CMODE bits are clear */
    temp_reg = MSS_MAC_read_phy_reg(this_mac, phy_addr, 20);
    temp_reg &= 0xFE6F;
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 20, temp_reg);

    /* Select general purpose registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 31, 0x0010U);

    /* Disable SIGDET operation */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 13, (uint16_t)(0x000FU));

#if defined(MSS_MAC_VSC8662_NWC_25)
    /* 25MHZ Recovered clock 1 and 2 enabled with ref clock source and no squelch */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 23, (uint16_t)(0x8033U));
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 24, (uint16_t)(0x8033U));
#elif defined(MSS_MAC_VSC8662_NWC_125)
    /* 125MHZ Recovered clock 1 and 2 enabled with ref clock source and no squelch */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 23, (uint16_t)(0x8133U));
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 24, (uint16_t)(0x8133U));
#endif

    /* Select standard registers page */
    MSS_MAC_write_phy_reg(this_mac, phy_addr, 31, 0x0000U);

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
MSS_MAC_VSC8662_phy_set_link_speed(/* mss_mac_instance_t */ void *v_this_mac,
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

        /* Full duplex mode or half duplex, multi port device */
        if ((MSS_MAC_10_FDX == this_mac->speed_mode) || (MSS_MAC_100_FDX == this_mac->speed_mode) ||
            (MSS_MAC_1000_FDX == this_mac->speed_mode))
        {
            MSS_MAC_write_phy_reg(this_mac,
                                  (uint8_t)this_mac->phy_addr,
                                  MII_CTRL1000,
                                  (uint16_t)(ADVERTISE_1000FULL | 0x0400U));
        }
        else
        {
            MSS_MAC_write_phy_reg(this_mac,
                                  (uint8_t)this_mac->phy_addr,
                                  MII_CTRL1000,
                                  (uint16_t)(ADVERTISE_1000HALF | 0x0400U));
        }
    }
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_VSC8662_phy_autonegotiate(/* mss_mac_instance_t */ const void *v_this_mac)
{
    const mss_mac_instance_t *this_mac = (const mss_mac_instance_t *)v_this_mac;
    volatile uint16_t phy_reg;
    uint16_t autoneg_complete;
    volatile uint32_t copper_aneg_timeout = 100000U;

    if (MSS_MAC_SPEED_AN == this_mac->speed_mode) /* Only do if allowed */
    {
        /* Select standard registers page */
        MSS_MAC_write_phy_reg(this_mac, (uint8_t)this_mac->phy_addr, 31, 0x0000U);

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
MSS_MAC_VSC8662_mac_autonegotiate(/* mss_mac_instance_t */ const void *v_this_mac)
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
MSS_MAC_VSC8662_phy_get_link_status(
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
uint16_t VSC8662_reg_0[32];
uint16_t VSC8662_reg_1[16];
uint16_t VSC8662_reg_16[32];

void dump_vsc8662_regs(const mss_mac_instance_t *this_mac);
void
dump_vsc8662_regs(const mss_mac_instance_t *this_mac)
{
    int32_t count;
    uint16_t page;
    uint16_t old_page;
    uint16_t *pdata;
    volatile psr_t lev;

    for (page = 0U; page <= 0x10U; page++)
    {
        if (0U == page)
        {
            pdata = VSC8662_reg_0;
        }
        else if (1U == page)
        {
            pdata = VSC8662_reg_1;
        }
        else if (16U == page)
        {
            pdata = VSC8662_reg_16;
        }
        else
        {
            pdata = VSC8662_reg_0;
        }

        if ((0U == page) || (0x10U == page))
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

        if (1U == page)
        {
            page = 0x0FU;
        }
    }
}

#endif /* #if defined(TARGET_ALOE) */
#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
