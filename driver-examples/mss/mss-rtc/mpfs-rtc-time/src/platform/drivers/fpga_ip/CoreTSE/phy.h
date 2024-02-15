
/*******************************************************************************
 * Copyright 2014 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file phy.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Register bit definitions for MII STA (station management entity) standard
 * interface. All basic MII register bits and enhanced capability register bits
 * are defined.
 * Complies with Clauses 22, 28, 37, 40 of IEEE RFC 802.3
 *
 */

#ifndef PHY_H_
#define PHY_H_

#include "coretse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/* Public definitions                                                     */
/**************************************************************************/

/* Generic MII registers. */
#define MII_BMCR                     0x00u /* Basic mode control register */
#define MII_BMSR                     0x01u /* Basic mode status register  */
#define MII_PHYSID1                  0x02u /* PHYS ID 1                   */
#define MII_PHYSID2                  0x03u /* PHYS ID 2                   */
#define MII_ADVERTISE                0x04u /* Advertisement control reg   */
#define MII_LPA                      0x05u /* Link partner ability reg    */
#define MII_EXPANSION                0x06u /* Expansion register          */
#define MII_NPAR                     0x07u
#define MII_LPNPA                    0x08u
#define MII_CTRL1000                 0x09u /* 1000BASE-T control          */
#define MII_STAT1000                 0x0au /* 1000BASE-T status           */
#define MII_ESTATUS                  0x0fu /* Extended Status */
#define MII_DCOUNTER                 0x12u /* Disconnect counter          */
#define MII_FCSCOUNTER               0x13u /* False carrier counter       */
#define MII_EXTEND                   0x14u /* extended PHY specific ctrl  */
#define MII_RERRCOUNTER              0x15u /* Receive error counter       */
#define MII_SREVISION                0x16u /* Silicon revision            */
#define MII_RESV1                    0x17u /* Reserved...                 */
#define MII_LBRERROR                 0x18u /* Lpback, rx, bypass error    */
#define MII_PHYADDR                  0x19u /* PHY address                 */
#define MII_RESV2                    0x1au /* Reserved...                 */
#define MII_TPISTATUS                0x1bu /* TPI status for 10mbps       */
#define MII_NCONFIG                  0x1cu /* Network interface config    */
#define MII_LMCS                     0x1du
#define MII_PHYCTRL1                 0x1eu
#define MII_PHYCTRL2                 0x1fu

/* Basic mode control register. */
#define BMCR_RESV                    0x003fu /* Unused...                   */
#define BMCR_SPEED1000               0x0040u /* MSB of Speed (1000)         */
#define BMCR_CTST                    0x0080u /* Collision test              */
#define BMCR_FULLDPLX                0x0100u /* Full duplex                 */
#define BMCR_ANRESTART               0x0200u /* Auto negotiation restart    */
#define BMCR_ISOLATE                 0x0400u /* Disconnect DP83840 from MII */
#define BMCR_PDOWN                   0x0800u /* Powerdown the DP83840       */
#define BMCR_ANENABLE                0x1000u /* Enable auto negotiation     */
#define BMCR_SPEED100                0x2000u /* Select 100Mbps              */
#define BMCR_LOOPBACK                0x4000u /* TXD loopback bits           */
#define BMCR_RESET                   0x8000u /* Reset the DP83840           */

/* Basic mode status register. */
#define BMSR_ERCAP                   0x0001u /* Ext-reg capability          */
#define BMSR_JCD                     0x0002u /* Jabber detected             */
#define BMSR_LSTATUS                 0x0004u /* Link status                 */
#define BMSR_ANEGCAPABLE             0x0008u /* Able to do auto-negotiation */
#define BMSR_RFAULT                  0x0010u /* Remote fault detected       */
#define BMSR_ANEGCOMPLETE            0x0020u /* Auto-negotiation complete   */
#define BMSR_RESV                    0x00c0u /* Unused...                   */
#define BMSR_ESTATEN                 0x0100u /* Extended Status in R15      */
#define BMSR_100HALF2                0x0200u /* Can do 100BASE-T2 HDX       */
#define BMSR_100FULL2                0x0400u /* Can do 100BASE-T2 FDX       */
#define BMSR_10HALF                  0x0800u /* Can do 10mbps, half-duplex  */
#define BMSR_10FULL                  0x1000u /* Can do 10mbps, full-duplex  */
#define BMSR_100HALF                 0x2000u /* Can do 100mbps, half-duplex */
#define BMSR_100FULL                 0x4000u /* Can do 100mbps, full-duplex */
#define BMSR_100BASE4                0x8000u /* Can do 100mbps, 4k packets  */

/* Advertisement control register. */
#define ADVERTISE_SLCT               0x001fu /* Selector bits               */
#define ADVERTISE_CSMA               0x0001u /* Only selector supported     */
#define ADVERTISE_10HALF             0x0020u /* Try for 10mbps half-duplex  */
#define ADVERTISE_1000XFULL          0x0020u /* Try for 1000BASE-X full-duplex */
#define ADVERTISE_10FULL             0x0040u /* Try for 10mbps full-duplex  */
#define ADVERTISE_1000XHALF          0x0040u /* Try for 1000BASE-X half-duplex */
#define ADVERTISE_100HALF            0x0080u /* Try for 100mbps half-duplex */
#define ADVERTISE_1000XPAUSE         0x0080u /* Try for 1000BASE-X pause    */
#define ADVERTISE_100FULL            0x0100u /* Try for 100mbps full-duplex */
#define ADVERTISE_1000XPSE_ASYM      0x0100u /* Try for 1000BASE-X asym pause */
#define ADVERTISE_100BASE4           0x0200u /* Try for 100mbps 4k packets  */
#define ADVERTISE_PAUSE_CAP          0x0400u /* Try for pause               */
#define ADVERTISE_PAUSE_ASYM         0x0800u /* Try for asymetric pause     */
#define ADVERTISE_RESV               0x1000u /* Unused...                   */
#define ADVERTISE_RFAULT             0x2000u /* Say we can detect faults    */
#define ADVERTISE_LPACK              0x4000u /* Ack link partners response  */
#define ADVERTISE_NPAGE              0x8000u /* Next page bit               */

#define ADVERTISE_FULL               (ADVERTISE_100FULL | ADVERTISE_10FULL | ADVERTISE_CSMA)
#define ADVERTISE_ALL                (ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF | ADVERTISE_100FULL)

/* Link partner ability register. */
#define LPA_SLCT                     0x001fu /* Same as advertise selector  */
#define LPA_10HALF                   0x0020u /* Can do 10mbps half-duplex   */
#define LPA_1000XFULL                0x0020u /* Can do 1000BASE-X full-duplex */
#define LPA_10FULL                   0x0040u /* Can do 10mbps full-duplex   */
#define LPA_1000XHALF                0x0040u /* Can do 1000BASE-X half-duplex */
#define LPA_100HALF                  0x0080u /* Can do 100mbps half-duplex  */
#define LPA_1000XPAUSE               0x0080u /* Can do 1000BASE-X pause     */
#define LPA_100FULL                  0x0100u /* Can do 100mbps full-duplex  */
#define LPA_1000XPAUSE_ASYM          0x0100u /* Can do 1000BASE-X pause asym*/
#define LPA_100BASE4                 0x0200u /* Can do 100mbps 4k packets   */
#define LPA_PAUSE_CAP                0x0400u /* Can pause                   */
#define LPA_PAUSE_ASYM               0x0800u /* Can pause asymetrically     */
#define LPA_RESV                     0x1000u /* Unused...                   */
#define LPA_RFAULT                   0x2000u /* Link partner faulted        */
#define LPA_LPACK                    0x4000u /* Link partner acked us       */
#define LPA_NPAGE                    0x8000u /* Next page bit               */

#define LPA_DUPLEX                   (LPA_10FULL | LPA_100FULL)
#define LPA_100                      (LPA_100FULL | LPA_100HALF | LPA_100BASE4)

/* Expansion register for auto-negotiation. */
#define EXPANSION_NWAY               0x0001u /* Can do N-way auto-nego      */
#define EXPANSION_LCWP               0x0002u /* Got new RX page code word   */
#define EXPANSION_ENABLENPAGE        0x0004u /* This enables npage words    */
#define EXPANSION_NPCAPABLE          0x0008u /* Link partner supports npage */
#define EXPANSION_MFAULTS            0x0010u /* Multiple faults detected    */
#define EXPANSION_RESV               0xffe0u /* Unused...                   */

#define ESTATUS_1000_TFULL           0x2000u /* Can do 1000BT Full */
#define ESTATUS_1000_THALF           0x1000u /* Can do 1000BT Half */

/* N-way test register. */
#define NWAYTEST_RESV1               0x00ffu /* Unused...                   */
#define NWAYTEST_LOOPBACK            0x0100u /* Enable loopback for N-way   */
#define NWAYTEST_RESV2               0xfe00u /* Unused...                   */

/* 1000BASE-T Control register */
#define ADVERTISE_1000FULL           0x0200u /* Advertise 1000BASE-T full duplex */
#define ADVERTISE_1000HALF           0x0100u /* Advertise 1000BASE-T half duplex */

/* 1000BASE-T Status register */
#define LPA_1000LOCALRXOK            0x2000u /* Link partner local receiver status */
#define LPA_1000REMRXOK              0x1000u /* Link partner remote receiver status */
#define LPA_1000FULL                 0x0800u /* Link partner 1000BASE-T full duplex */
#define LPA_1000HALF                 0x0400u /* Link partner 1000BASE-T half duplex */

/* Indicates what features are supported by the interface. */
#define SUPPORTED_10baseT_Half       (1 << 0)
#define SUPPORTED_10baseT_Full       (1 << 1)
#define SUPPORTED_100baseT_Half      (1 << 2)
#define SUPPORTED_100baseT_Full      (1 << 3)
#define SUPPORTED_1000baseT_Half     (1 << 4)
#define SUPPORTED_1000baseT_Full     (1 << 5)
#define SUPPORTED_Autoneg            (1 << 6)
#define SUPPORTED_TP                 (1 << 7)
#define SUPPORTED_AUI                (1 << 8)
#define SUPPORTED_MII                (1 << 9)
#define SUPPORTED_FIBRE              (1 << 10)
#define SUPPORTED_BNC                (1 << 11)
#define SUPPORTED_10000baseT_Full    (1 << 12)
#define SUPPORTED_Pause              (1 << 13)
#define SUPPORTED_Asym_Pause         (1 << 14)
#define SUPPORTED_2500baseX_Full     (1 << 15)
#define SUPPORTED_Backplane          (1 << 16)
#define SUPPORTED_1000baseKX_Full    (1 << 17)
#define SUPPORTED_10000baseKX4_Full  (1 << 18)
#define SUPPORTED_10000baseKR_Full   (1 << 19)
#define SUPPORTED_10000baseR_FEC     (1 << 20)

/* Indicates what features are advertised by the interface. */
#define ADVERTISED_10baseT_Half      (1 << 0)
#define ADVERTISED_10baseT_Full      (1 << 1)
#define ADVERTISED_100baseT_Half     (1 << 2)
#define ADVERTISED_100baseT_Full     (1 << 3)
#define ADVERTISED_1000baseT_Half    (1 << 4)
#define ADVERTISED_1000baseT_Full    (1 << 5)
#define ADVERTISED_Autoneg           (1 << 6)
#define ADVERTISED_TP                (1 << 7)
#define ADVERTISED_AUI               (1 << 8)
#define ADVERTISED_MII               (1 << 9)
#define ADVERTISED_FIBRE             (1 << 10)
#define ADVERTISED_BNC               (1 << 11)
#define ADVERTISED_10000baseT_Full   (1 << 12)
#define ADVERTISED_Pause             (1 << 13)
#define ADVERTISED_Asym_Pause        (1 << 14)
#define ADVERTISED_2500baseX_Full    (1 << 15)
#define ADVERTISED_Backplane         (1 << 16)
#define ADVERTISED_1000baseKX_Full   (1 << 17)
#define ADVERTISED_10000baseKX4_Full (1 << 18)
#define ADVERTISED_10000baseKR_Full  (1 << 19)
#define ADVERTISED_10000baseR_FEC    (1 << 20)

/**************************************************************************/
/* Public function declarations                                           */
/**************************************************************************/

/***************************************************************************/ /**

  */
void TSE_phy_init(tse_instance_t *this_tse, uint8_t phy_addr);

/***************************************************************************/ /**

  */
void TSE_phy_set_link_speed(tse_instance_t *this_tse,
                            uint8_t phy_addr,
                            uint32_t speed_duplex_select);

/***************************************************************************/ /**

  */
void TSE_phy_autonegotiate(tse_instance_t *this_tse, uint8_t phy_addr);

/***************************************************************************/ /**

  */
uint8_t TSE_phy_get_link_status(tse_instance_t *this_tse,
                                uint8_t phy_addr,
                                tse_speed_t *speed,
                                uint8_t *fullduplex);

#ifdef __cplusplus
}
#endif

#endif /* PHY_H_ */
