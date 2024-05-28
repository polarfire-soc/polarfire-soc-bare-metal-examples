/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file null_phy.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief NULL PHY implementation.
 *
 * This PHY interface is used when there is a direct connection between
 * two GEM instances which does not involve the use of a PHY device.
 *
 * Also used when setting up the default config so that the pointers for the
 * PHY functions in the config always have some valid values.
 *
 */

#include "mpfs_hal/mss_hal.h"

#include "drivers/mss/mss_ethernet_mac/mss_ethernet_registers.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_regs.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_sw_cfg.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac.h"
#include "drivers/mss/mss_ethernet_mac/phy.h"

#ifdef __cplusplus
extern "C" {
#endif

#if MSS_MAC_USE_PHY_NULL

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_NULL_phy_init(/* mss_mac_instance_t */ const void *v_this_mac, uint8_t phy_addr)
{
    /* Nothing to see here... */
    (void)v_this_mac;
    (void)phy_addr;
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_NULL_phy_set_link_speed(/* mss_mac_instance_t */ void *v_this_mac,
                                uint32_t speed_duplex_select,
                                mss_mac_speed_mode_t speed_mode)
{
    /* Nothing to see here... */
    (void)v_this_mac;
    (void)speed_duplex_select;
    (void)speed_mode;
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_NULL_phy_autonegotiate(/* mss_mac_instance_ t */ const void *v_this_mac)
{
    /* Nothing to see here... */
    (void)v_this_mac;
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
MSS_MAC_NULL_phy_mac_autonegotiate(/* mss_mac_instance_t */ const void *v_this_mac)
{
    /* Nothing to see here... */
    (void)v_this_mac;
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
uint8_t
MSS_MAC_NULL_phy_get_link_status(
    /* mss_mac_instance_t */ const void *v_this_mac,
    mss_mac_speed_t *speed,
    uint8_t *fullduplex)
{
    uint8_t link_status;

    (void)v_this_mac;
    /* Assume link is up. */
    link_status = MSS_MAC_LINK_UP;

    /* Pick fastest for now... */

    *fullduplex = MSS_MAC_FULL_DUPLEX;
    *speed = MSS_MAC_1000MBPS;

    return link_status;
}
#endif /* MSS_MAC_USE_PHY_NULL */

/**************************************************************************/ /**
                                                                              *
                                                                              */
uint16_t
NULL_mmd_read_extended_regs(/* mss_mac_instance_t */ const void *v_this_mac,
                            uint16_t dev,
                            uint16_t reg)
{
    (void)v_this_mac;
    (void)dev;
    (void)reg;

    return (0);
}

/**************************************************************************/ /**
                                                                              *
                                                                              */
void
NULL_mmd_write_extended_regs(/* mss_mac_instance_t */ const void *v_this_mac,
                             uint16_t dev,
                             uint16_t reg,
                             uint16_t data)
{
    (void)v_this_mac;
    (void)dev;
    (void)reg;
    (void)data;

    /* Nothing to see here... */
}

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
