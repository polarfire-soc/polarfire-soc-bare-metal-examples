
/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file null_phy.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief NULL PHY implementation 
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

#if !defined(M88E1340_PHY) && !defined(M88E1111_PHY)

void
TSE_phy_init(tse_instance_t *this_tse, uint8_t phy_addr)
{
    /* Nothing to see here... */
}

/***************************************************************************/ /**

  */
void
TSE_phy_set_link_speed(tse_instance_t *this_tse, uint8_t phy_addr, uint32_t speed_duplex_select)
{
    /* Nothing to see here... */
}

/***************************************************************************/ /**

  */
void
TSE_phy_autonegotiate(tse_instance_t *this_tse, uint8_t phy_addr)
{
    /* Nothing to see here... */
}

/***************************************************************************/ /**

  */
uint8_t
TSE_phy_get_link_status(tse_instance_t *this_tse,
                        uint8_t phy_addr,
                        tse_speed_t *speed,
                        uint8_t *fullduplex)
{
    /* Nothing to see here... */
    return 0;
}

#endif

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
