/*******************************************************************************
 * Copyright 2019-2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_ethernet_mac_types.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief This file contains type definitions used throughout the PolarFire SoC
 * Microprocessor Subsystem (MSS) Ethernet MAC and PHY device drivers. User need
 * not include this file in application source code.
 * Inclusion of mss_ethernet_mac.h inherits these types.
 * 
 */

#ifndef MSS_ETHERNET_MAC_TYPES_H_
#define MSS_ETHERNET_MAC_TYPES_H_
#include <stdint.h>

#if defined(MSS_MAC_PHY_HW_RESET) || defined(MSS_MAC_PHY_HW_SRESET)
#include "drivers/mss_gpio/mss_gpio.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
/* Public type definitions                                                     */
/*******************************************************************************/

/***************************************************************************//**
 * MAC interface speed indicator
 *
 * This enumeration specifies the various interface speeds supported by the MAC 
 * hardware. These values are used by _MSS_MAC_get_link_status()_ to indicate the
 * current link speed.
 */
typedef enum __mss_mac_speed_t
{
    MSS_MAC_10MBPS     = 0x00,
    MSS_MAC_100MBPS    = 0x01,
    MSS_MAC_1000MBPS   = 0x02,
    INVALID_SPEED      = 0x03
} mss_mac_speed_t;

/***************************************************************************//**
 * MAC interface speed mode selector
 *
 * This enumeration specifies the various interface speed mode to use when 
 * calling _MSS_MAC_change_speed()_. Anything other than _MSS_MAC_SPEED_AN_
 * results in a fixed speed/duplex configuration and the link partner must be
 * configured the same way for proper communications to be achieved.
 *
 * __Note:__ If the link partner is configured for autonegotiation, then
 * selection of a fixed speed, full duplex mode will probably result in a half
 * duplex mismatched mode of operation due to the way autonegotiation works.
 */
typedef enum __mss_mac_speed_mode_t
{
    MSS_MAC_SPEED_AN   = 0x00, /*!< Link operates in autonegotiation mode */
    MSS_MAC_10_HDX     = 0x01, /*!< Link operates in 10M half duplex mode */
    MSS_MAC_10_FDX     = 0x02, /*!< Link operates in 10M full duplex mode */
    MSS_MAC_100_HDX    = 0x03, /*!< Link operates in 100M half duplex mode */
    MSS_MAC_100_FDX    = 0x04, /*!< Link operates in 100M full duplex mode */
    MSS_MAC_1000_HDX   = 0x05, /*!< Link operates in 1000M half duplex mode */
    MSS_MAC_1000_FDX   = 0x06, /*!< Link operates in 1000M full duplex mode */
    INVALID_SPEED_MODE = 0x07
} mss_mac_speed_mode_t;

/****************************************************************************//**
 * MAC RX interrupt control
 *
 * This enumeration indicates the action to take in relation to the RX interrupt
 * when configuring an RX buffer with _MSS_MAC_receive_pkt()_. These values
 * enable control of the interrupt setup so that the receive descriptors can be
 * initialized safely.
 */
typedef enum __mss_mac_rx_int_ctrl_t
{
    MSS_MAC_INT_ARM     = -1, /*!< Last buffer in chain so arm the RX interrupt */
    MSS_MAC_INT_DISABLE =  0, /*!< Disable interrupts on exit */
    MSS_MAC_INT_ENABLE  =  1, /*!< Leave interrupts enabled on exit */
} mss_mac_rx_int_ctrl_t;

/****************************************************************************//**
 * MAC PHY Reset type
 *
 * This enumeration specifies the type of reset signal to act on when calling
 * _MSS_MAC_phy_reset()_.
 */
typedef enum __mss_mac_phy_reset_t
{
    MSS_MAC_SOFT_RESET = 0,
    MSS_MAC_HARD_RESET = 1
} mss_mac_phy_reset_t;



/***************************************************************************//**
 * AMBA burst length defines. These are used when setting the
 * _amba_burst_length_ field in the _mss_mac_cfg_t structure_. 
 */

#define MSS_MAC_AMBA_BURST_256  (0U)
#define MSS_MAC_AMBA_BURST_1    (1U)
#define MSS_MAC_AMBA_BURST_4    (4U)
#define MSS_MAC_AMBA_BURST_8    (8U)
#define MSS_MAC_AMBA_BURST_16   (16U)
#define MSS_MAC_AMBA_BURST_MASK (31U)


/***************************************************************************//**
 * Pointer to PHY init function
 *
 * This defines the prototype for the PHY driver function which is called by the
 * MSS Ethernet MAC driver to initialize the PHY. This function is registered
 * with the MSS Ethernet MAC driver via the _phy_init_ entry in the
 * _mss_mac_cfg_t_ structure passed to _MSS_MAC_init()_.
 * 
 * The function should match the following prototype: 
 *     `void MSS_MAC_phy_init(const void *this_mac, uint8_t phy_addr);`
 * 
 * - The _this_mac_ parameter is actually a pointer to the
 * _mss_mac_instance_t_ for the GEM to which the PHY is attached. It is declared
 * as `void *` to avoid issues with recursive structure name references.
 *
 * - The _phy_addr_ parameter is the address of the PHY on the MDIO interface.
 *
 */
typedef void (*mss_mac_phy_init_t)(/* mss_mac_instance_t */ const void *this_mac, uint8_t phy_addr);


/***************************************************************************//**
 * Pointer to PHY set link speed function
 *
 * This defines the prototype for the PHY driver function which is called by the
 * MSS Ethernet MAC driver to set the link speed for the PHY. This function is
 * registered with the MSS Ethernet MAC driver via the _phy_set_link_speed_
 * entry in the _mss_mac_cfg_t_ structure passed to _MSS_MAC_init()_.
 * 
 * The function should match the following prototype: 
 * `void MSS_MAC_phy_set_link_speed
 * (
 *      const void *this_mac,
 *      uint32_t speed_duplex_select,
 *      mss_mac_speed_mode_t speed_mode
 * );`
 *
 * - The _this_mac_ parameter is actually a pointer to the
 * _mss_mac_instance_t_ for the GEM to which the PHY is attached. It is declared
 * as `void *` to avoid issues with recursive structure name references.
 *
 * - The _speed_duplex_select_ parameter indicates the link speed and duplex
 * combinations to allow when autonegotiation is selected via speed_mode. 
 *
 * - The _speed_mode_ parameter either selects a specific single duplex and
 * speed combination or autonegotiation as the mode to use. 
 */
typedef void (*mss_mac_phy_set_speed_t)(/* mss_mac_instance_t */ void *this_mac, uint32_t speed_duplex_select, mss_mac_speed_mode_t speed_mode);


/***************************************************************************//**
 * Pointer to PHY autonegotiate function
 *
 * This defines the prototype for the PHY driver functions which are called by
 * the MSS Ethernet MAC driver to instruct the PHY to carry out an
 * autonegotiation operation on the Ethernet link or the SGMII link. These
 * functions are registered with the MSS Ethernet MAC driver via the
 * _phy_autonegotiate_ and _phy_mac_autonegotiate_ entries in the 
 * _mss_mac_cfg_t_ structure passed to _MSS_MAC_init()_.
 * 
 * The function should match the following prototype: 
 *     `void MSS_MAC_phy_autonegotiate(const void *this_mac);`
 *
 * - The _this_mac_ parameter is actually a pointer to the
 * _mss_mac_instance_t_ for the GEM to which the PHY is attached. It is declared
 * as `void *` to avoid issues with recursive structure name references.
 */
typedef void (*mss_mac_phy_autonegotiate_t)(/* mss_mac_instance_t */ const void *this_mac);


/***************************************************************************//**
 * Pointer to PHY get link status function
 *
 * This defines the prototype for the PHY driver function which is called by the
 * MSS Ethernet MAC driver to fetch the link status from the PHY. This function
 * is registered with the MSS Ethernet MAC driver via the _phy_get_link_status_
 * entry in the _mss_mac_cfg_t_ structure passed to _MSS_MAC_init()_.
 * 
 * The function should match the following prototype: 
 * `uint8_t MSS_MAC_phy_get_link_status
 * (
 *     const void *this_mac,
 *     mss_mac_speed_t * speed,
 *     uint8_t * fullduplex
 * );`
 *
 * - The _this_mac_ parameter is actually a pointer to the
 * _mss_mac_instance_t_ for the GEM to which the PHY is attached. It is declared
 * as `void *` to avoid issues with recursive structure name references.
 *
 * - The _speed parameter_ is a pointer to where to store the current link
 * speed.
 *
 * - The _full_duplex_ parameter is a pointer to where to store the current link
 * duplex state - 0 for half duplex and 1 for full duplex.
 *
 * - The function returns 0 if the link is currently down and 1 if it is currently
 * active. 
 *
 * __Note:__ If the link is not active, the speed and duplex values will not be
 * updated.
 */
typedef uint8_t (*mss_mac_phy_get_link_status_t)
(
    /* mss_mac_instance_t*/ const void *this_mac,
    mss_mac_speed_t * speed,
    uint8_t * fullduplex
);


/***************************************************************************//**
 * Pointer to PHY extended read function.
 *
 * Many of the example PHY devices support access to registers outside of the
 * 0 to 31 address range available via the management interface through the use
 * of paging registers and this can be done via the normal PHY register access
 * routines. The DP83867 PHYs use a different addressing mechanism for accessing
 * extended registers and this function implements the read support.
 *
 * The function should match the following prototype: 
 * `uint16_t ti_read_extended_regs(const void * this_mac, uint16_t reg);`
 *
 * - The _this_mac_ parameter is actually a pointer to the
 * _mss_mac_instance_t_ for the GEM to which the PHY is attached. It is declared
 * as `void *` to avoid issues with recursive structure name references.
 *
 * - The _reg_ parameter identifies the register to read from.
 */
typedef uint16_t (*mss_mac_phy_extended_read_t)(/* mss_mac_instance_t */ const void *this_mac, uint16_t dev, uint16_t reg);


/***************************************************************************//**
 * Pointer to PHY extended write function
 *
 * Many of the example PHY devices support access to registers outside of the
 * 0 to 31 address range available via the management interface through the use
 * of paging registers and this can be done via the normal PHY register access
 * routines. The DP83867 PHYs use a different addressing mechanism for accessing
 * extended registers and this function implements the write  support.
 *
 * The function should match the following prototype: 
 * `void ti_write_extended_regs(mss_mac_instance_t * this_mac, uint16_t reg);`
 *
 * - The _this_mac_ parameter is actually a pointer to the
 * _mss_mac_instance_t_ for the GEM to which the PHY is attached. It is declared
 * as `void *` to avoid issues with recursive structure name references.
 *
 * - The _reg_ parameter identifies the register to write to.
 *
 * - The _data_ parameter is the value to write to the register.
 */
typedef void (*mss_mac_phy_extended_write_t)(/* mss_mac_instance_t */ const void *this_mac, uint16_t dev, uint16_t reg, uint16_t data);


/***************************************************************************//**
  PolarFire SoC MSS Ethernet MAC Configuration Structure.
  
  The _mss_mac_cfg_t_ type contains the initial configuration values for the
  MPFS Ethernet MAC. You need to create a record of this type to hold the
  configuration of the MAC. _MSS_MAC_cfg_struct_def_init()_ is used to 
  initialize the configuration record to default values. Later, the
  configuration elements in the record can be changed to desired values before
  passing them to _MSS_MAC_init()_.

  __Note:__ Even though most of these values are small, we use _uint32_t_ for
  most values here as they will be used in calculations that are based on
  _uint32_t_ values and this avoids having to put casts everywhere...

  ___interface_type___:
    This indicates the type of interface between the MAC and the PHY. The
    currently supported values are:
  
      - _NULL_PHY_   – No PHY involved, usually for direct connection via the
                       fabric.
      - _GMII_       – Connection via GMII routed through the fabric to external
                       PHY device.
      - _TBI_        – Connection via SGMII block to external PHY device.
      - _GMII_SGMII_ – Emulation platform specific option using SGMII to GMII
                       bridge.

  ___phy_type___:
    This indicates the type of PHY device connected to the MAC. The currently
    supported values are:

      - _MSS_MAC_DEV_PHY_NULL_         – No PHY device.
      - _MSS_MAC_DEV_PHY_VSC8575_      – VSC8575 with full VTSS API.
      - _MSS_MAC_DEV_PHY_VSC8575_LITE_ – VSC8757 with Lite VTSS API.
      - _MSS_MAC_DEV_PHY_VSC8541_      – VSC8541 without VTSS API.
      - _MSS_MAC_DEV_PHY_VSC8662_      – VSC88662 without VTSS API.
      - _MSS_MAC_DEV_PHY_DP83867_      – TI DP83867.

  ___phy_init___  
  ___phy_set_link_speed___  
  ___phy_autonegotiate___  
  ___phy_autonegotiate_mac___  
  ___phy_get_link_status___  
  ___phy_extended_read___  
  ___phy_init_extended_write___:  
    These are callback functions for the PHY support within the driver. These
    should be set to the appropriate PHY driver functions for the attached PHY
    device. See _mss_mac_phy_init_t_, _mss_mac_phy_set_speed_t_,
    _mss_mac_phy_autonegotiate_t_, _mss_mac_phy_get_link_status_t_,
    _mss_mac_phy_extended_read_t_ and _mss_mac_phy_extended_write_t_ for
    details.

  ___queue_enable___:
    This array of values of length _MSS_MAC_QUEUE_COUNT_, indicates which queues
    are to be enabled. 0 in an entry indicates disabled and 1 indicates enabled.

  ___speed_mode___:
    This parameter specifies the mode of operation for the Ethernet interface.
    The following values are supported:

      - _MSS_MAC_SPEED_AN_
      - _MSS_MAC_10_HDX_
      - _MSS_MAC_10_FDX_
      - _MSS_MAC_100_HDX_
      - _MSS_MAC_100_FDX_
      - _MSS_MAC_1000_HDX_
      - _MSS_MAC_1000_FDX_

    If _MSS_MAC_SPEED_AN_ is selected then the _speed_duplex_select_ 
    configuration parameter indicates the allowed speed combinations. For all
    other modes, the link autonegotiation is disabled.

  ___speed_duplex_select___:
    This configuration parameter specifies the allowed link speeds when
    autonegotiation is enabled. It is a bit-mask of the various link speed and
    duplex modes. The speed_duplex_select configuration can be set to a bitmask
    of the following defines to specify the allowed link speed and duplex mode:

      - _MSS_MAC_ANEG_10M_FD_
      - _MSS_MAC_ANEG_10M_HD_
      - _MSS_MAC_ANEG_100M_FD_
      - _MSS_MAC_ANEG_100M_HD_
      - _MSS_MAC_ANEG_1000M_FD_
      - _MSS_MAC_ANEG_1000M_HD_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_ANEG_ALL_SPEEDS_ indicating that a link will be setup
    for best available speed and duplex combination.

  ___mac_addr___:
    This configuration parameter is a 6-byte array containing the local MAC
    address of the Ethernet MAC. For correct operation, this value should be
    unique for each of the 4 MAC elements within the MSS and if more than one
    system containing an MPFS device is present on a LAN then the MAC addresses
    must all be unique. Tha management and allocation of the MAC addresses is
    left up to the user.

  ___phy_address___:
    This parameter specifies the address of the PHY device, usually set in
    hardware by the address pins of the PHY device.

  ___pcs_phy_address___:
    This parameter specifies the address of the control device for hidden SGMII
    type interfaces such as that in the G5 SoC emulation platform.

  ___phy_soft_reset_gpio___:
    Identifies the MSS GPIO device that the PHY soft reset pin is connected to.
    A Value of _NULL_ indicates the soft reset signal is not available for 
    control via an MSS GPIO pin.

  ___phy_soft_reset_pin___:
    Identifies the MSS GPIO pin that the PHY soft reset pin is connected to.

  ___phy_hard_reset_gpio___:
    Identifies the MSS GPIO device that the PHY hard reset pin is connected to.
    A Value of _NULL_ indicates the hard reset signal is not available for
    control via an MSS GPIO pin.

  ___phy_hard_reset_pin___:
    Identifies the MSS GPIO pin that the PHY hard reset pin is connected to.

  ___phy_controller___:
    Pointer to the MAC instance structure to which the PHY is connected. This
    supports the use of multi-port PHY devices which may be connected to a
    single MAC device or configurations where only a single MDIO interface is
    exposed to the outside world. If _NULL_, the current MAC device is used.

    In configurations that use this feature, it is important that the MAC device
    that the PHY is connected to is initialized first.

  ___tx_edc_enable___:
    This parameter specifies enable or disable error detection and correction
    for tx FIFOs. The allowed values for the tx_edc_enable configuration
    parameter are:

      - _MSS_MAC_ERR_DET_CORR_ENABLE_
      - _MSS_MAC_ERR_DET_CORR_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_ERR_DET_CORR_DISABLE_.

  ___rx_edc_enable___:
    This parameter specifies enable or disable error detection and correction
    for rx FIFOs. The allowed values for the _rx_edc_enable_ configuration
    parameter are:

      - _MSS_MAC_ERR_DET_CORR_ENABLE_
      - _MSS_MAC_ERR_DET_CORR_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_ERR_DET_CORR_DISABLE_.

  ___jumbo_frame_enable___:
    This parameter allows enabling or disabling jumbo frame support. When
    enabled, it allows frames longer than the standard 1536 byte maximum frame
    length to be transmitted and received. When disabled, the MAC limits the
    length of frames at the maximum frame length. The allowed values for the
    _jumbo_frame_enable_ configuration parameter are:

      - _MSS_MAC_JUMBO_FRAME_ENABLE_
      - _MSS_MAC_JUMBO_FRAME_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_JUMBO_FRAME_DISABLE_.

  ___jumbo_frame_default___:
    This parameter sets the initial maximum jumbo frame length. The
    _MSS_MAC_cfg_struct_def_init()_ function sets this configuration parameter
    to _MSS_MAC_MAX_PACKET_SIZE_.

  ___length_field_check___:
    This parameter specifies enable or disable length field check. When enabled,
    the MAC checks the frame length field of received frames to ensure it
    matches the actual data field length. The allowed values for the 
    _length_field_check_ configuration parameter are:

      - _MSS_MAC_LENGTH_FIELD_CHECK_ENABLE_
      - _MSS_MAC_LENGTH_FIELD_CHECK_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_LENGTH_FIELD_CHECK_ENABLE_

  ___append_CRC___:
    This parameter specifies enable or disable appending a CRC to transmitted
    packets. When enabled, the MAC appends a CRC to all frames. When disabled,
    frames presented to the MAC must have a valid length and contain a valid
    CRC. The allowed values for the _append_CRC_ parameter are:

      - _MSS_MAC_CRC_ENABLE_
      - _MSS_MAC_CRC_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_CRC_ENABLE_.

  ___fullduplex___:
    This specifies enable or disable full duplex. When enabled, the MAC operates
    in full duplex mode. When disabled, the MAC operates in half duplex mode.
    The allowed values for the _fullduplex_ configuration parameter are:

      - _MSS_MAC_FULLDUPLEX_ENABLE_
      - _MSS_MAC_FULLDUPLEX_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_FULLDUPLEX_ENABLE_.

  ___loopback___:
    This parameter specifies enable or disable loop back mode. When enabled, the
    MAC transmit outputs to be looped back to its receiving inputs. The allowed
    values for the _loopback_ configuration parameter are:

      - _MSS_MAC_LOOPBACK_ENABLE_
      - _MSS_MAC_LOOPBACK_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_LOOPBACK_DISABLE_.

  ___rx_flow_ctrl___:
    This parameter allows enabling or disabling receiver flow control. When
    enabled, the MAC detects and acts on PAUSE flow control frames. When
    disabled, it ignores PAUSE flow control frames. The allowed values for the
    _rx_flow_ctrl_ configuration parameter are:

      - _MSS_MAC_RX_FLOW_CTRL_ENABLE_
      - _MSS_MAC_RX_FLOW_CTRL_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_RX_FLOW_CTRL_ENABLE_.

  ___tx_flow_ctrl___:
    This parameter allows enabling or disabling transmitter flow control. When
    enabled, the transmitter sends PAUSE flow control frames when requested by
    the system. When disabled, prevents the transmitter from sending flow
    control frames. The allowed values for the _tx_flow_ctrl_ configuration
    parameter are:

      - _MSS_MAC_TX_FLOW_CTRL_ENABLE_
      - _MSS_MAC_TX_FLOW_CTRL_DISABLE_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_TX_FLOW_CTRL_ENABLE_.

  ___ipg_multiplier___  
  ___ipg_divisor___:  
    These parameters specify the minimum size of gap (IPG/IFG) to enforce
    between frames (expressed in bit times). They are both 8 bit values and are
    used to calculate an IPG value based on the last packet sent by multiplying
    the length by _ipg_multiplier_ and dividing the result by _ipg_divisor_. The
    resulting number of bits is used if it is greater than the default 96 bits.

    To select standard 96 bit IPG, set _ipg_multiplier_ to _MSS_MAC_IPG_DEFVAL_.

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_IPG_DEFVAL_.

  ___phyclk___:
    This parameter specifies the MII management clock divider value. PCLK
    is the source clock. This should be chosen to ensure that MDC has a
    frequency that is no greater than 2.5MHz. The allowed values for the phyclk
    configuration parameter are:

      - _MSS_MAC_DEF_PHY_CLK_
      - _MSS_MAC_BY8_PHY_CLK_
      - _MSS_MAC_BY16_PHY_CLK_
      - _MSS_MAC_BY32_PHY_CLK_
      - _MSS_MAC_BY48_PHY_CLK_
      - _MSS_MAC_BY64_PHY_CLK_
      - _MSS_MAC_BY96_PHY_CLK_
      - _MSS_MAC_BY128_PHY_CLK_
      - _MSS_MAC_BY224_PHY_CLK_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_DEF_PHY_CLK_.

  ___max_frame_length___:
    This parameter specifies the maximum frame size in both the transmit and
    receive directions. The allowed values for the _max_frame_length_
    configuration parameter are:

      - _MSS_MAC_MAXFRAMELEN_DEFVAL_
      - _MSS_MAC_MAXFRAMELEN_MAXVAL_

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to _MSS_MAC_MAXFRAMELEN_DEFVAL_.

  ___use_hi_address___:
    When set to 0, _use_hi_address_ selects the default AXI slave slot 5 address
    for the location of the registers of the GEM device. When set to non 0, AXI
    slave slot 6 is used to access the device.

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to 0.

  ___use_local_ints___:
    When set to 0, _use_local_ints_ selects the PLIC interrupts as the source
    for interrupts from the GEM. When set to non 0, local interrupts are used.
    GEM0 is connected to the local interrupts of U54 numbers 1 and 2. GEM1 is
    connected to the local interrupts of U54 numbers 3 and 4.

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to 0.

  ___queue0_int_priority___:  
  ___queue1_int_priority___:  
  ___queue2_int_priority___:  
  ___queue3_int_priority___:  
  ___mmsl_int_priority___:  
    These parameters indicate the interrupt priority to use for each of the
    interrupt sources that the GEM supports. A priority of 0 effectively
    disables an interrupt and a priority of 7 is the highest priority that can
    be assigned.

    _queue0_int_priority_ is the priority for the primary queue for the pMAC and
    the only queue for the eMAC.

    The _MSS_MAC_cfg_struct_def_init()_ function sets these configuration
    parameters to 7.

  ___tsu_clock_select___:
    Selects the TSU clock source. 0 is the default TSU clock source and 1 is the
    alternative TSU clock source driven from the fabric.

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration
    parameter to 0.
    
  ___amba_burst_length___:
    Sets the burst length for the DMA AXI data access transfers. Valid values
    are:

    - 0x00 - Attempt to use bursts up to 256
    - 0x01 - Always use single bursts
    - 0x02 - Always use single bursts
    - 0x04 - Attempt to use bursts up to 4
    - 0x08 - Attempt to use bursts up to 8
    - 0x10 - Attempt to use bursts up to 16

    The _MSS_MAC_cfg_struct_def_init()_ function sets this configuration parameter
    to 0x10 for bursts up to 16.
 */

typedef struct __mss_mac_cfg_t
{
    uint32_t interface_type;            /*!< Type of network interface associated with this GEM */
    uint32_t phy_type;                    /*!< PHY device type associated with this GEM */
    /* PHY interface functions */
    mss_mac_phy_init_t            phy_init; /*!< Pointer to PHY init function */
    mss_mac_phy_set_speed_t       phy_set_link_speed; /*!< Pointer to PHY set link speed function */
    mss_mac_phy_autonegotiate_t   phy_autonegotiate; /*!< Pointer to PHY initiate autonegotiation function */
    mss_mac_phy_autonegotiate_t   phy_mac_autonegotiate; /*!< Pointer to PHY initiate autonegotiation for SGMII link function */
    mss_mac_phy_get_link_status_t phy_get_link_status; /*!< Pointer to PHY get link status function */
    mss_mac_phy_extended_read_t   phy_extended_read; /*!< Pointer to PHY extended read function */
    mss_mac_phy_extended_write_t  phy_extended_write; /*!< Pointer to PHY extended write function */
    uint32_t queue_enable[MSS_MAC_QUEUE_COUNT]; /*!< Enables for additional queues */
    mss_mac_speed_mode_t speed_mode;    /*!< Link speed mode of operation */
    uint32_t speed_duplex_select;       /*!< Link speed and duplex mode allowed to setup a link when autonegotiation is enabled. */
    uint8_t  mac_addr[6];               /*!< Station's MAC address */
    uint32_t phy_addr;                  /*!< Address of Ethernet PHY on MII management interface. */
    uint32_t pcs_phy_addr;              /*!< Address of SGMII interface controller on MII management interface. */
#if defined(MSS_MAC_PHY_HW_RESET) || defined(MSS_MAC_PHY_HW_SRESET)
    GPIO_TypeDef  *phy_soft_reset_gpio; /*!< GPIO device soft reset for PHY is connected to */
    mss_gpio_id_t  phy_soft_reset_pin;  /*!< GPIO pin soft reset for PHY is connected to */
    GPIO_TypeDef  *phy_hard_reset_gpio; /*!< GPIO device hard reset for PHY is connected to */
    mss_gpio_id_t  phy_hard_reset_pin;  /*!< GPIO pin hard reset for PHY is connected to */
#endif
    /* Use struct instead of typedef as compiler gets confused otherwise... */
    struct mss_mac_instance *phy_controller; /* Which MAC structure PHY is connected to */
    uint32_t tx_edc_enable;             /*!< Enable / disable error detection and correction for tx FIFOs */
    uint32_t rx_edc_enable;             /*!< Enable / disable error detection and correction for rx FIFOs */
    uint32_t jumbo_frame_enable;        /*!< Enable / disable jumbo frame support: default is disable 0 */
    uint32_t jumbo_frame_default;       /*!< Default maximum size for jumbo frames */
    uint32_t length_field_check;        /*!< Enable / disable length field checking */
    uint32_t append_CRC;                /*!< Enable / disable appending CRC */
    uint32_t fullduplex;                /*!< Enable / disable full duplex: default is disable 0 */
    uint32_t loopback;                  /*!< Enable / disable loopback mode: default is disable 0 */
    uint32_t rx_flow_ctrl;              /*!< Enable / disable receiver flow control: default is disable 0 */
    uint32_t tx_flow_ctrl;              /*!< Enable / disable transmitter flow control: default is disable 0 */
    uint32_t ipg_multiplier;            /*!< 8-bit IPG multiplication factor, if 0 we disable IPG stretching */
    uint32_t ipg_divisor;               /*!< 8-bit back to back inter-frame gap value */
    uint32_t phyclk;                    /*!< 3-bit MGMT clock divider value */
    uint32_t max_frame_length;          /*!< Maximum frame length: default value is 0x0600(1536d) */
    uint32_t use_hi_address;            /*!< Non 0 means use upper address range for this device */
    uint32_t use_local_ints;            /*!< non 0 means use local interrupts for MAC instead of PLIC */
    uint32_t queue0_int_priority;       /*!< Main MAC interrupt */
    uint32_t queue1_int_priority;       /*!< Queue 1 interrupt */
    uint32_t queue2_int_priority;       /*!< Queue 2 interrupt */
    uint32_t queue3_int_priority;       /*!< Queue 3 interrupt */
    uint32_t mmsl_int_priority;         /*!< MMSL interrupt */
    uint32_t tsu_clock_select;          /*!< 0 for default TSU clock, 1 for fabric tsu clock */
    uint32_t amba_burst_length;         /*!< AXI burst length for DMA data transfers */
} mss_mac_cfg_t;

/***************************************************************************//**
 * This enumeration is used for accessing Transmit and Receive statistics. The
 * statistic which is desired to be read is passed to _MSS_MAC_read_stat()_. The
 * width of the returned statistic value is indicated in the comment against the
 * statistic.
 *
 * These registers are cleared when read, so in order to maintain a proper 
 * running total, the software should maintain counts which are updated by
 * adding the values each time a statistic count is read.
 *
 * __Note:__ Counts do not roll over when the maximum is reached so a reading of
 * 2^n - 1 indicates that at least that many of the indicated event have
 * occurred since the last read.
 */
typedef enum __mss_mac_stat_t
{
    MSS_MAC_TX_OCTETS_LOW,                /*!< 32-bit - LSBs of transmitted octets count */
    MSS_MAC_TX_OCTETS_HIGH,               /*!< 16-bit - MSBs of transmitted octets count */
    MSS_MAC_TX_FRAMES_OK,                 /*!< 32-bit - Overall count of frames successfully transmitted */
    MSS_MAC_TX_BCAST_FRAMES_OK,           /*!< 32-bit - Count of broadcast frames successfully transmitted */
    MSS_MAC_TX_MCAST_FRAMES_OK,           /*!< 32-bit - Count of multicast frames successfully transmitted */
    MSS_MAC_TX_PAUSE_FRAMES_OK,           /*!< 32-bit - Count of pause successfully transmitted */
    MSS_MAC_TX_64_BYTE_FRAMES_OK,         /*!< 32-bit - Count of 64 byte frames successfully transmitted (excluding pause frames) */
    MSS_MAC_TX_65_BYTE_FRAMES_OK,         /*!< 32-bit - Count of 65 to 127 byte frames successfully transmitted */
    MSS_MAC_TX_128_BYTE_FRAMES_OK,        /*!< 32-bit - Count of 128 to 255 byte frames successfully transmitted */
    MSS_MAC_TX_256_BYTE_FRAMES_OK,        /*!< 32-bit - Count of 256 to 511 byte frames successfully transmitted */
    MSS_MAC_TX_512_BYTE_FRAMES_OK,        /*!< 32-bit - Count of 512 to 1023 byte frames successfully transmitted */
    MSS_MAC_TX_1024_BYTE_FRAMES_OK,       /*!< 32-bit - Count of 1024 to 1518 byte frames successfully transmitted */
    MSS_MAC_TX_1519_BYTE_FRAMES_OK,       /*!< 32-bit - Count of frames longer than 1518 bytes successfully transmitted */
    MSS_MAC_TX_UNDERRUNS,                 /*!< 10-bit - Count of frames not transmitted due to under runs */
    MSS_MAC_TX_SINGLE_COLLISIONS,         /*!< 18-bit - Count of single collision transmit events */
    MSS_MAC_TX_MULTIPLE_COLLISIONS,       /*!< 18-bit - Count of 2 to 15 collision burst events */
    MSS_MAC_TX_EXCESSIVE_COLLISIONS,      /*!< 10-bit - Count of frames discarded because 16 collisions occurred */
    MSS_MAC_TX_LATE_COLLISIONS,           /*!< 10-bit - Count of collisions detected after the 512 bit slot time */
    MSS_MAC_TX_DEFERRED_FRAMES,           /*!< 18-bit - Count of frames deferred due to carrier sense being active */
    MSS_MAC_TX_CRS_ERRORS,                /*!< 10-bit - Count of Carrier Sense Errors  */

    MSS_MAC_RX_OCTETS_LOW,                /*!< 32-bit - LSBs of received octets count */
    MSS_MAC_RX_OCTETS_HIGH,               /*!< 16-bit - MSBs of received octets count */
    MSS_MAC_RX_FRAMES_OK,                 /*!< 32-bit - Overall count of frames successfully received */
    MSS_MAC_RX_BCAST_FRAMES_OK,           /*!< 32-bit - Count of broadcast frames successfully received */
    MSS_MAC_RX_MCAST_FRAMES_OK,           /*!< 32-bit - Count of multicast cast frames successfully received */
    MSS_MAC_RX_PAUSE_FRAMES_OK,           /*!< 32-bit - Count of pause frames successfully received */
    MSS_MAC_RX_64_BYTE_FRAMES_OK,         /*!< 32-bit - Count of 64 byte frames successfully received (excluding pause frames) */
    MSS_MAC_RX_65_BYTE_FRAMES_OK,         /*!< 32-bit - Count of 65 to 127 byte frames successfully received */
    MSS_MAC_RX_128_BYTE_FRAMES_OK,        /*!< 32-bit - Count of 128 to 255 byte frames successfully received */
    MSS_MAC_RX_256_BYTE_FRAMES_OK,        /*!< 32-bit - Count of 256 to 511 byte frames successfully received */
    MSS_MAC_RX_512_BYTE_FRAMES_OK,        /*!< 32-bit - Count of 512 to 1023 byte frames successfully received */
    MSS_MAC_RX_1024_BYTE_FRAMES_OK,       /*!< 32-bit - Count of 1024 to 1518 byte frames successfully received */
    MSS_MAC_RX_1519_BYTE_FRAMES_OK,       /*!< 32-bit - Count of frames longer than 1518 bytes successfully received */
    MSS_MAC_RX_UNDERSIZE_FRAMES_OK,       /*!< 10-bit - Count of undersize frames received */
    MSS_MAC_RX_OVERSIZE_FRAMES_OK,        /*!< 10-bit - Count of frames received over the maximum allowed length (1518, 1536 or 10240 depending on settings) */
    MSS_MAC_RX_JABBERS,                   /*!< 10-bit - Count of received jabber frames - excess length with errors */
    MSS_MAC_RX_FCS_ERRORS,                /*!< 10-bit - Count of frames received with incorrect CRC values */
    MSS_MAC_RX_LENGTH_ERRORS,             /*!< 10-bit - Count of frames received ok where the length received does not match the length field */
    MSS_MAC_RX_SYMBOL_ERRORS,             /*!< 10-bit - Count of frames received with symbol errors */
    MSS_MAC_RX_ALIGNMENT_ERRORS,          /*!< 10-bit - Count of frames received with non integral byte counts */
    MSS_MAC_RX_RESOURCE_ERRORS,           /*!< 18-bit - Count of times a receive buffer descriptor was read with the used bit set */
    MSS_MAC_RX_OVERRUNS,                  /*!< 10-bit - Count of frames not copied to memory due to receive overrun */
    MSS_MAC_RX_IP_CHECKSUM_ERRORS,        /*!<  8-bit - Count of frames received with IP header checksum errors */
    MSS_MAC_RX_TCP_CHECKSUM_ERRORS,       /*!<  8-bit - Count of frames received with TCP checksum errors */
    MSS_MAC_RX_UDP_CHECKSUM_ERRORS,       /*!<  8-bit - Count of frames received with UDP checksum errors */
    MSS_MAC_RX_AUTO_FLUSHED_PACKETS,      /*!< 16-bit - Count of frames flushed from receive SRAM buffers  */

    MSS_MAC_LAST_STAT                     /*!< Marker for end of valid statistics references */
} mss_mac_stat_t;

/***************************************************************************//**
 * This enumeration indicates which direction unicast packet is being
 * referenced. This is used with the _MSS_MAC_set_TSU_unicast_addr()_ and
 * _MSS_MAC_get_TSU_unicast_addr()_ functions.
 */
typedef enum __mss_mac_tsu_addr_t
{
    MSS_MAC_TSU_UNICAST_RX,
    MSS_MAC_TSU_UNICAST_TX
} mss_mac_tsu_addr_t;

/***************************************************************************//**
 * This enumeration indicates DMA descriptor time stamp insertion modes. This
 * indicates which types of packets should have their IEEE 1588 timestamps
 * recorded in the transmit or receive descriptors.
 *
 * 
 * This value is used with the _MSS_MAC_get_TSU_rx_mode()_,
 * _MSS_MAC_set_TSU_rx_mode()_, _MSS_MAC_get_TSU_tx_mode()_ and
 * _MSS_MAC_set_TSU_tx_mode()_ functions.
 */
typedef enum __mss_mac_tsu_mode_t
{
    MSS_MAC_TSU_MODE_DISABLED,  /*!< Time stamp insertion disabled */
    MSS_MAC_TSU_MODE_PTP_EVENT, /*!< Time stamp insertion for PTP Event frames only */
    MSS_MAC_TSU_MODE_PTP_ALL,   /*!< Time stamp insertion all PTP frames */
    MSS_MAC_TSU_MODE_ALL,       /*!< Time stamp insertion for all frames */
    MSS_MAC_TSU_MODE_END
} mss_mac_tsu_mode_t;

/***************************************************************************//**
 * This enumeration indicates hash matching modes for accepting received frames.
 * This value is used with the _MSS_MAC_set_hash_mode()_ and 
 * _MSS_MAC_get_hash_mode()_ functions.
 */
typedef enum __mss_mac_hash_mode_t
{
    MSS_MAC_HASH_NONE      = 0x00,     /*!< Hash matching disabled */
    MSS_MAC_HASH_MULTICAST = 0x40,     /*!< Multicast matching enabled */
    MSS_MAC_HASH_UNIICAST  = 0x80,     /*!< Unicast matching enabled */
    MSS_MAC_HASH_ALL       = 0xC0      /*!< Multicast and Unicast matching enabled */
} mss_mac_hash_mode_t;

/***************************************************************************//**
 * This enumeration indicates sync time stamp adjust modes to use for One Step
 * Synce operation (OSS). This value is used with the 
 * _MSS_MAC_set_TSU_oss_mode()_ and _MSS_MAC_get_TSU_oss_mode()_ functions.
 */
typedef enum __mss_mac_oss_mode_t
{
    MSS_MAC_OSS_MODE_DISABLED,  /*!< Sync time stamp adjust disabled */
    MSS_MAC_OSS_MODE_REPLACE,   /*!< Sync time stamp replace mode */
    MSS_MAC_OSS_MODE_ADJUST,    /*!< Sync time stamp adjust correction field mode */
    MSS_MAC_OSS_MODE_INVALID,   /*!< Only for reporting mis-configured setup, not setting things... */
    MSS_MAC_OSS_MODE_END
} mss_mac_oss_mode_t;


/***************************************************************************//**
 * This enumeration indicates the pre-emption minimum fragment size which 
 * also determines the minimum number of bytes which the pMAC sends before
 * pre-emption is allowed. 
 *
 * This value is used in the _mss_mac_mssl_config_t_ structure _frag_size_
 * field.
 */
typedef enum __mss_mac_frag_size_t
{
    MSS_MAC_FRAG_SIZE_64,
    MSS_MAC_FRAG_SIZE_128,
    MSS_MAC_FRAG_SIZE_192,
    MSS_MAC_FRAG_SIZE_256,
    MSS_MAC_FRAG_SIZE_END
} mss_mac_frag_size_t;


/***************************************************************************//**
 * DMA Descriptor control and status bit field defines.
 *
 * The MSS Ethernet MAC driver creates and manages two descriptor rings for each
 * queue to control the transmission and reception of packets. These defines are
 * used when configuring the descriptors and when decoding the results of a
 * packet receive or transmit operation.
 *
 * __Note:__ The driver currently uses a buffer scheme of one buffer per packet
 * but it is possible to use multiple buffers which each contain a portion of
 * the packet and some of the bit fields are of more importance in that type of 
 * buffer scheme.
 *
 * __Note:__ Some bits have different meanings based on the active configuration
 * of the GEM and these defines represent the values for the configuration(s)
 * used by the driver.
 * 
 * The following are the transmit offload error codes and their meanings:
 *  - 000 - No Error
 *  - 001 - The Packet was identified as a VLAN type, but the header was not
 *          fully complete, or had an error in it.
 *  - 010 - The Packet was identified as a SNAP type, but the header was not
 *          fully complete, or had an error in it.
 *  - 011 - The Packet was not of an IP type, or the IP packet was invalidly
 *          short, or the IP was not of type IPv4/IPv6.
 *  - 100 - The Packet was not identified as VLAN, SNAP or IP.
 *  - 101 - Non supported packet fragmentation occurred. For IPv4 packets, the
 *          IP checksum was generated and inserted.
 *  - 110 - Packet type detected was not TCP or UDP. TCP/UDP checksum was
 *          therefore not generated. For IPv4 packets, the IP checksum was
 *          generated and inserted.
 *  - 111 - A premature end of packet was detected and the TCP/UDP checksum
 *          could not be generated.
 */
 
#define GEM_RX_DMA_TS_PRESENT     BIT_02 /*!< @brief This descriptor contains a
                                           valid time stamp which indicates the
                                           receive time. */
#define GEM_RX_DMA_WRAP           BIT_01 /*!< @brief This is the last descriptor
                                           in the chain and the DMA controller
                                           will next access the first descriptor
                                           in the chain. */
#define GEM_RX_DMA_USED           BIT_00 /*!< @brief Set to 0 by the driver to
                                           indicate associated buffer is free.
                                           Set to 1 by the GEM once a packet
                                           or packet fragment is received into
                                           the buffer. */
#define GEM_RX_DMA_BCAST          BIT_31 /*!< @brief  The received packet was
                                           sent to the Ethernet global
                                           broadcast address. */
#define GEM_RX_DMA_MULTICAST_HASH BIT_30 /*!< @brief The recieved packet matched
                                           a multicast hash address. */
#define GEM_RX_DMA_UNICAST_HASH   BIT_29 /*!< @brief The recieved packet matched
                                           a unicast hash address. */
#define GEM_RX_DMA_EXT_ADDR_MATCH BIT_28 /*!< @brief The recieved packet matched
                                           an external address matching filter. */
#define GEM_RX_DMA_SPECIFIC_ADDR  BIT_27 /*!< @brief The recieved packet matched
                                           an internal specific address matching
                                           filter. */
#define GEM_RX_DMA_ADDR_REGISTER  (BIT_25 | BIT_26) /*!< @brief Bitfield
                                                      indicating the specific
                                                      address register the
                                                      packet matched. */
#define GEM_RX_DMA_TYPE_ID_MATCH  BIT_24 /*!< @brief  If set a type ID register
                                           match occurred. */
#define GEM_RX_DMA_TYPE_ID        (BIT_22 | BIT_23) /*!< @brief Bitfield 
                                                      indicating which ID
                                                      register was matched. */
#define GEM_RX_DMA_VLAN_TAG       BIT_21 /*!< @brief Set if a VLAN tag was
                                           detected in the packet. */
#define GEM_RX_DMA_PRIORITY_TAG   BIT_20 /*!< @brief Priority tag detected — 
                                           type ID of 0x8100 and null VLAN
                                           identifier. For packets incorporating
                                           the stacked VLAN processing feature,
                                           this bit will be set if the second
                                           VLAN tag received has a type ID of
                                           0x8100 and a null VLAN identifier. */
#define GEM_RX_DMA_VLAN_PRIORITY  (BITS_03 << 17) /*!< @brief Bitfield
                                                    indicating the VLAN
                                                    priority. */
#define GEM_RX_DMA_FCS_ERROR      BIT_16 /*!< @brief Received FCS did not match
                                           calculated FCS. */
#define GEM_RX_DMA_START_OF_FRAME BIT_15 /*!< @brief Set when this buffer holds
                                           the start of a packet. */
#define GEM_RX_DMA_END_OF_FRAME   BIT_14 /*!< @brief Set when this buffer holds
                                           the end of a packet. */
#define GEM_RX_DMA_JUMBO_BIT_13   BIT_13 /*!< @brief If jumbo frame reception
                                           is enabled this is b13 of the packet
                                           length. */
#define GEM_RX_DMA_BUFF_LEN       BITS_13 /*!< @brief b0 to b12 of the packet
                                            length.  */

#define GEM_TX_DMA_USED           BIT_31 /*!< @brief Set to 0 by the driver to
                                           indicate buffer contains data to be
                                           transmitted. Set to 1 by the GEM once
                                           packet or packet fragment has been
                                           sent from the buffer. */
#define GEM_TX_DMA_WRAP           BIT_30 /*!< @brief This is the last descriptor
                                           in the chain and the DMA controller
                                           will next access the first descriptor
                                           in the chain. */
#define GEM_TX_DMA_RETRY_ERROR    BIT_29 /*!< @brief Retry limit exceeded, tx
                                           error detected. */
#define GEM_TX_DMA_UNDERRUN       BIT_28 /*!< @brief Transmit underrun. This is
                                           not used as the GEM DMA is configured
                                           for packet buffer mode. */
#define GEM_TX_DMA_BUS_ERROR      BIT_27 /*!< @brief Transmit frame corruption
                                           due to AHB/AXI error. */
#define GEM_TX_DMA_LATE_COL_ERROR BIT_26 /*!< @brief Late collision transmit
                                           error detected. */
#define GEM_TX_DMA_TS_PRESENT     BIT_23 /*!< @brief This descriptor holds a
                                           valid time stamp which indicates the
                                           transmit time. */
#define GEM_TX_DMA_OFFLOAD_ERRORS (BITS_03 << 20) /*!< @brief Transmit
                                                    IP/TCP/UDP checksum
                                                    generation offload errors:
                                                    See above for details. */
#define GEM_TX_DMA_NO_CRC         BIT_16 /*!< @brief If set, GEM assumes buffer
                                           contains a CRC and does not calculate
                                           and append one. */
#define GEM_TX_DMA_LAST           BIT_15 /*!< @brief When set indicates this is
                                           the last buffer in a frame. */
#define GEM_TX_DMA_BUFF_LEN       BITS_14 /*!< @brief 14 bit buffer length. */
 
/*******************************************************************************
 * __Note:__ In the following definitions we use void * for the this_mac
 * parameter as they are used in the definition of the _mss_mac_instance_t_
 * structure and this confuses the compiler if we try to use the proper
 * structure pointer in the definition of these function pointers...
 */


/***************************************************************************//**
 * Transmit DMA descriptor.
 *
 * This structure is used to configure the transmit descriptors for the DMA
 * engine. The exact format depends on the _MSS_MAC_64_BIT_ADDRESS_MODE_ and
 * _MSS_MAC_TIME_STAMPED_MODE_ macros and the driver only supports compile time
 * configuration of the descriptor type.
 */
typedef struct mss_mac_tx_desc mss_mac_tx_desc_t;
struct mss_mac_tx_desc
{
    uint32_t          addr_low;     /*!< Buffer address low portion */
    volatile uint32_t status;       /*!< Status and options for transmit operation */
#if defined(MSS_MAC_64_BIT_ADDRESS_MODE)
    uint32_t          addr_high;    /*!< High portion of address in 64bit addressing mode */
    uint32_t          unused;       /*!< Unused word in 64bit mode */
#endif 
#if defined(MSS_MAC_TIME_STAMPED_MODE)
    volatile uint32_t nano_seconds; /*!< Nanoseconds and LSBs of seconds for timestamp */
    volatile uint32_t seconds;      /*!< MSBs of timestamp seconds */
#endif
};

/***************************************************************************//**
 * Receive DMA descriptor.
 *
 * This structure is used to configure the receive descriptors for the DMA
 * engine. The exact format depends on the _MSS_MAC_64_BIT_ADDRESS_MODE_ and
 * _MSS_MAC_TIME_STAMPED_MODE_ macros and the driver only supports compile time
 * configuration of the descriptor type.
 */
typedef struct mss_mac_rx_desc mss_mac_rx_desc_t;
struct mss_mac_rx_desc
{
    uint32_t          addr_low;     /*!< Buffer address low portion */
    volatile uint32_t status;       /*!< Status and options for transmit operation */
#if defined(MSS_MAC_64_BIT_ADDRESS_MODE)
    uint32_t          addr_high;    /*!< High portion of address in 64bit addressing mode */
    uint32_t          unused;       /*!< Unused word in 64bit mode */
#endif 
#if defined(MSS_MAC_TIME_STAMPED_MODE)
    volatile uint32_t nano_seconds; /*!< Nanoseconds and LSBs of seconds for timestamp */
    volatile uint32_t seconds;      /*!< MSBs of timestamp seconds */
#endif
};

/***************************************************************************//**
 * Transmit callback function.
 *
 * This is the prototype for the user function which the MSS Ethernet MAC driver
 * calls when a packet has been transmitted. The users function is responsible
 * for processing any post transmit processing required for the packets.
 *
 *   - ___this_mac___    - pointer to global structure for the MAC in question.
 *   - ___queue_no___    - 0 to 3 for pMAC and always 0 for eMAC.
 *   - ___cdesc___       - pointer to the DMA descriptor associated with this
 *                         packet.
 *   - ___p_user_data___ - original user data pointer associated with the packet
 *                         buffer.
 */
typedef void (*mss_mac_transmit_callback_t)(/* mss_mac_instance_t*/ void *this_mac,
                                       uint32_t queue_no,
                                       mss_mac_tx_desc_t *cdesc,
                                       void * p_user_data);

/***************************************************************************//**
 * Receive callback function.
 *
 * This is the prototype for the user function which the MSS Ethernet MAC driver
 * calls when a packet has been received. The users function is responsible for
 * processing any post receive processing required for the packets and for
 * returning the packet buffer to the receive chain so that further packets can
 * be received with this buffer.
 *
 *   - ___this_mac___    - pointer to global structure for the MAC in question.
 *   - ___queue_no___    - 0 to 3 for pMAC and always 0 for eMAC.
 *   - ___p_rx_packet___ - pointer to the buffer for the packet to be processed.
 *   - ___pckt_length___ - length of packet to be processed.
 *   - ___cdesc___       - pointer to the DMA descriptor associated with this
 *                         packet.
 *   - ___p_user_data___ - original user data pointer associated with the packet
 *                         buffer.
 */
typedef void (*mss_mac_receive_callback_t)(/* mss_mac_instance_t*/ void *this_mac,
                                       uint32_t queue_no,
                                       uint8_t *p_rx_packet,
                                       uint32_t pckt_length,
                                       mss_mac_rx_desc_t *cdesc,
                                       void *p_user_data);

/***************************************************************************//**
 * TSU timer time value.
 *
 * This structure represents the time value from the TSU and is used with the
 * _MSS_MAC_read_TSU() function_.
 */
typedef struct mss_mac_tsu_time mss_mac_tsu_time_t;
struct mss_mac_tsu_time
{
    uint32_t secs_msb;        /*!< Most significant bits of seconds count */
    uint32_t secs_lsb;        /*!< Least significant bits of seconds count */
    uint32_t nanoseconds;     /*!< Nanoseconds count */
};


/***************************************************************************//**
 * TSU configuration structure.
 *
 * This structure is used with the _MSS_MAC_init_TSU()_ function to configure 
 * the initial count value and increment values for the TSU. The driver
 * configures the timer to operate in Increment Mode.
 */
typedef struct mss_mac_tsu_config mss_mac_tsu_config_t;
struct mss_mac_tsu_config
{
    /* initial values */
    uint32_t secs_msb;        /*!< Most significant bits of seconds count */
    uint32_t secs_lsb;        /*!< Least significant bits of seconds count */
    uint32_t nanoseconds;     /*!< Nanoseconds count  */

    /* Per TSU tick values */
    uint32_t ns_inc;          /*!< Nanoseconds TSU increment value */
    uint32_t sub_ns_inc;      /*!< Sub-nanoseconds TSU increment value */
};


/***************************************************************************//**
 * Type 1 filter structure.
 *
 * This structure is used to configure Type 1 Screening Filters which allow 
 * routing or dropping of packets based on a UDP port number and the 
 * differentiated services/traffic class field.
 *
 * The match results are ANDed together which means all enabled filters must
 * match for a successful match. Packets can either be dropped or sent to the
 * indicated queue on a match.
 *
 * Both the eMAC and pMAC support 4 Type 1 Screening Filters
 */
typedef struct mss_mac_type_1_filter mss_mac_type_1_filter_t;
struct mss_mac_type_1_filter
{
    uint16_t udp_port;        /*!< UDP port number to match */
    uint8_t  dstc;            /*!< Designated Services/Traffic Class value to match */
    uint8_t  queue_no;        /*!< Queue to send to on match */
    uint8_t  drop_on_match;   /*!< Drop packet instead of routing to queue */
    uint8_t  dstc_enable;     /*!< Enable DS/TC matching */
    uint8_t  udp_port_enable; /*!< Enable UDP port matching */
};

/***************************************************************************//**
 * Type 2 filter structure.
 *
 * This structure is used to configure Type 2 Screening Filters which allow 
 * routing or dropping of packets based on Ethertype field values, VLAN priority
 * and arbitrary packet data patterns.
 *
 * Ethertype matching is perfromed using a combination of a Type 2 Filter and
 * and an Ethertype screening register. 
 *
 * Arbitrary data matching is perfromed using a combination of a Type 2 Filter
 * and between 1 and 3 Type 2 Comparer blocks. 
 *
 * The match results are ANDed together which means all enabled filters must
 * match for a successful match. Packets can either be dropped or sent to the
 * indicated queue on a match.
 *
 * The eMAC supports 2 Type 2 Screening Filters and the pMAC supports 4.
 */
typedef struct mss_mac_type_2_filter mss_mac_type_2_filter_t;
struct mss_mac_type_2_filter
{
    uint8_t  ethertype_enable;     /*!< Enable Ethertype matching */
    uint8_t  ethertype_index;      /*!< Which Ethertype compare block to use */
    uint8_t  vlan_priority_enable; /*!< Enable VLAN priority matching */
    uint8_t  vlan_priority;        /*!< VLAN priority level to use. */
    uint8_t  drop_on_match;        /*!< Drop packet instead of routing to queue */
    uint8_t  compare_a_enable;     /*!< Enable data comparer A */
    uint8_t  compare_a_index;      /*!< Index to data comparator to use for A */
    uint8_t  compare_b_enable;     /*!< Enable data comparer B */
    uint8_t  compare_b_index;      /*!< Index to data comparator to use for B */
    uint8_t  compare_c_enable;     /*!< Enable data comparer C */
    uint8_t  compare_c_index;      /*!< Index to data comparator to use for C */
    uint8_t  queue_no;             /*!< The queue to route packet to on match */
};

/***************************************************************************//**
 * Type 2 filter comparer structures.
 *
 * This structure is used to configure Type 2 Screening Filter data comparers 
 * which allow filtering on arbitrary packet data patterns. Alternatively the
 * comparer can be configured to match a normal or stacked VLAN tag.
 *
 * For data comparision, the user can select between a straight 32 bit compare
 * operation and a 16 bit compare operation with a 16 bit mask value. When the
 * masked compare is used, the mask value is used to determine which bits are
 * included in the match - a value of zero in a mask bit masks the corresponding
 * data bit, a value of 1 enables the comparison.
 *
 * For data compare operations, b0-b7 correspond to the byte at the offset,
 * b8-b15 correspond to the byte at offset + 1 and so on.
 *
 * If either compare_vlan_c_id or compare_vlan_s_id is set then the data value
 * represents the VLAN tag. __Note:__ The byte order is such that if 81 00 00 20
 * is received to indicate a C-TAG frame with VID 020 0x00200FFF would be
 * written to the compare register. So in the special case of VLAN comparison
 * the last byte received is the least significant byte in the compare register
 *
 * The eMAC supports 6 Type 2 comparers and the pMAC supports 12.
 */
typedef struct mss_mac_type_2_compare mss_mac_type_2_compare_t;
struct mss_mac_type_2_compare
{
    uint32_t data;              /*!< 32 bits of data or 16 bits of data */
    uint16_t mask;              /*!< 16 bit mask if data is 16 bit */
    uint8_t  disable_mask;      /*!< True to select raw 32 bit data match */
    uint8_t  compare_vlan_c_id; /*!< Compare VLAN C tag - higher precedence than S tag */
    uint8_t  compare_vlan_s_id; /*!< Compare VLAN S tag */
    uint8_t  compare_offset;    /*!< Offset type - see MSS_MAC_OFFSET_* definitions */
    uint8_t  offset_value;      /*!< Offset value */
};

/***************************************************************************//**
 * Media Merge Sublayer configuration structure.
 *
 * This structure is used to configure the operation of the MMSL via the 
 * _MSS_MAC_set_mmsl_mode()_ function and retrieve the current settings via the
 * _MSS_MAC_get_mmsl_mode()_ function.
 *
 * The default operation mode on reset, is no pre-emption and all receive 
 * traffic routed to the pMAC.
 *
 * If verify_disable is true then the link will switch to pre-emption without
 * negotitation of capabilities with the link partner when preemption is set to 
 * true.
 */
typedef struct mss_mac_mmsl_config mss_mac_mmsl_config_t;
struct mss_mac_mmsl_config
{
    mss_mac_frag_size_t frag_size; /*!< See  mss_mac_frag_size_t for details */
    uint8_t preemption;            /*!< Enable preemption */
    uint8_t verify_disable;        /*!< Set true to force preemption without testing link */
    uint8_t use_pmac;              /*!< Receive all to pMAC if not preempting */
};

/***************************************************************************//**
 * Media Merge Sublayer statistics structure.
 *
 * This structure is used to return the MMSL statistics counts via the 
 * _MSS_MAC_get_mmsl_stats()_ function.
 *
 * __Note:__ These values are actually a mix of 8 and 17 bits but we return them
 * all as 32 bits to simplify collecting the stats.
 *
 * These registers are cleared when read so in order to maintain a proper 
 * running total, the software should maintain counts which are updated by
 * adding the values each time a statistic count is read.
 *
 * Counts do not roll over when the maximum is reached so a reading of 2^n - 1
 * indicates that at least that many of the indicated event have occurred since
 * the last read.
 */

typedef struct mss_mac_mmsl_stats mss_mac_mmsl_stats_t;

struct mss_mac_mmsl_stats
{
    uint32_t smd_err_count; /*!< 8 bit count of unknown SMD values received */
    uint32_t ass_err_count; /*!< 8 bit count of frames with reassembly errors */
    uint32_t ass_ok_count;  /*!< 17 bit count of frames reassembled ok */
    uint32_t frag_count_rx; /*!< 17 bit count of mPackets received */
    uint32_t frag_count_tx; /*!< 17 bit count of mPackets sent */
};


/***************************************************************************//**
 * Multi packet transmit structure
 *
 * This structure is used with the _MSS_MAC_send_pkts()_ function to define the
 * list of packets to tranjsmit. The application is responsible for allocating
 * the list and populating it with the details for each packet.
 *
 * To send n packets, the list needs to contain n+1 entries with the last entry
 * having a 0 length and a null pointer for the packet data buffer address.
 */
typedef struct mss_mac_tx_pkt_info mss_mac_tx_pkt_info_t;

struct mss_mac_tx_pkt_info
{
    uint32_t  queue_no;    /*!< Queue number for this packet */
    uint32_t  length;      /*!< Length of this packet - 0 for end of list */
    uint8_t  *tx_buffer;   /*!< Pointer to packet data - 0 for end of list */
    void     *p_user_data; /*!< Pointer to user data for this packet */
};


/***************************************************************************//**
 * Per queue specific info for device management structure.
 *
 * This structure instantiates the queue specific information for managing the
 * MSS Ethernet MAC driver. 
 */
typedef struct mss_mac_queue
{
#if defined(MSS_MAC_USE_DDR)
    mss_mac_tx_desc_t            *tx_desc_tab;  /*!< Transmit descriptor table */
#else
    mss_mac_tx_desc_t            tx_desc_tab[MSS_MAC_TX_RING_SIZE];  /*!< Transmit descriptor table */
#endif

#if defined(MSS_MAC_USE_DDR)
    mss_mac_rx_desc_t            *rx_desc_tab;  /*!< Receive descriptor table */
#else
    mss_mac_rx_desc_t            rx_desc_tab[MSS_MAC_RX_RING_SIZE];  /*!< Receive descriptor table */
#endif
    void                        *tx_caller_info[MSS_MAC_TX_RING_SIZE]; /*!< Pointers to tx user specific data */
    void                        *rx_caller_info[MSS_MAC_RX_RING_SIZE]; /*!< Pointers to rx user specific data */
    mss_mac_transmit_callback_t  pckt_tx_callback; /*!< Pointer to transmit handler call back function */
    mss_mac_receive_callback_t   pckt_rx_callback; /*!< Pointer to receive handler call back function */
    volatile uint32_t            nb_available_tx_desc; /*!< Number of free TX descriptors available */
    volatile uint32_t            current_tx_desc; /*!< Oldest in the queue... */
    volatile uint32_t            nb_available_rx_desc; /*!< Number of free RX descriptors available */
    volatile uint32_t            next_free_rx_desc_index; /*!< Next RX descriptor to allocate */
    volatile uint32_t            first_rx_desc_index; /*!< Descriptor to process next when receive handler called */
    uint32_t                     rx_discard; /*!< If set, silently discard incoming packets */
    uint32_t                     overflow_counter; /*!< Overflows since last normal receive operation */
    uint32_t                     tries;  /*!< Keeps track of failure to sends... */
    volatile int32_t             in_isr; /*!< Set when processing ISR so functions don't call PLIC enable/disable for protection */

    /* Queue specific register addresses to simplify the driver code */
    volatile uint32_t           *int_status;        /*!< interrupt status */
    volatile uint32_t           *int_mask;          /*!< interrupt mask */
    volatile uint32_t           *int_enable;        /*!< interrupt enable */
    volatile uint32_t           *int_disable;       /*!< interrupt disable */
    volatile uint32_t           *receive_q_ptr;     /*!< RX queue pointer */
    volatile uint32_t           *transmit_q_ptr;    /*!< TX queue pointer */
    volatile uint32_t           *dma_rxbuf_size;    /*!< RX queue buffer size */

    /* Statistics counters */

    volatile uint64_t ingress; /*!< Count of bytes received on this queue */
    volatile uint64_t egress; /*!< Count of bytes transmitted on this queue */
    volatile uint64_t rx_overflow; /*!< Number of receive overflow events on this queue */
    volatile uint64_t hresp_error; /*!< Number of receive hresp error events on this queue*/
    volatile uint64_t rx_restart; /*!< Number of times reception has been restarted on this queue */
    volatile uint64_t tx_amba_errors; /*!< Number of receive amba error events on this queue */
    volatile uint64_t tx_restart; /*!< Number of times transmission has been restarted on this queue */
    volatile uint64_t tx_reenable; /*!< Number of times transmission has been reenabled on this queue */
} mss_mac_queue_t;


/***************************************************************************//**
 * G5SoC Ethernet MAC instance
 * A local record of this type will be created and maintained by the driver for
 * each pMAC and eMAC.
 */
typedef struct mss_mac_instance
{
    uint32_t          is_emac;    /*!< 0 for primary MAC and non zero for eMAC */
    MAC_TypeDef       *mac_base;  /*!< Register start address - _NULL_ if eMAC */
    eMAC_TypeDef      *emac_base; /*!< Register start address - _NULL_ if primary MAC */
    PLIC_IRQn_Type    mac_q_int[MSS_MAC_QUEUE_COUNT]; /*!< Interrupt numbers for each queue */
#if defined(TARGET_G5_SOC)
    PLIC_IRQn_Type    mmsl_int; /*!< interrupt number for MMSL interrupt */
#endif

    mss_mac_queue_t   queue[MSS_MAC_QUEUE_COUNT]; /*!< Queue specific information */

    volatile uint64_t tx_pause; /*!< Count of pause frames sent */
    volatile uint64_t rx_pause; /*!< Count of pause frames received */
    volatile uint64_t pause_elapsed; /*!< Count of pause frame elapsed events */

    uint32_t          rx_discard; /*!< Flag for discarding all received data */
    volatile uint32_t mac_available; /*!< Flag to show init is done and MAC and PHY can be used */

    /* These are set from the cfg structure */

    uint32_t jumbo_frame_enable;        /*!< Enable / disable jumbo frame support: */
    uint32_t append_CRC;                /*!< Enable / disable GEM CRC calculation */
    uint32_t interface_type;            /*!< Type of network interface associated with this GEM */
    uint32_t phy_type;                  /*!< PHY device type associated with this GEM */
    uint32_t phy_addr;                  /*!< Address of Ethernet PHY on MII management interface. */
    uint32_t pcs_phy_addr;              /*!< Address of SGMII interface controller on MII management interface. */
#if defined(MSS_MAC_PHY_HW_RESET) || defined(MSS_MAC_PHY_HW_SRESET)

    GPIO_TypeDef  *phy_soft_reset_gpio; /*!< GPIO device soft reset for PHY is connected to */
    mss_gpio_id_t  phy_soft_reset_pin;  /*!< GPIO pin soft reset for PHY is connected to */
    uint32_t       phy_soft_reset_done; /*!< Flag to indicate soft reset is done so multi-port devices
                                         *   can avoid additional resets - set by phy driver */
    GPIO_TypeDef  *phy_hard_reset_gpio; /*!< GPIO device hard reset for PHY is connected to */
    mss_gpio_id_t  phy_hard_reset_pin;  /*!< GPIO pin hard reset for PHY is connected to */
    uint32_t       phy_hard_reset_done; /*!< Flag to indicate hard reset is done so multi-port devices
                                         *   can avoid additional resets - set by phy driver */
#endif
    /* Use struct instead of typedef as compiler gets confused otherwise... */
    struct mss_mac_instance *phy_controller; /*!< Which MAC structure PHY is connected to */
    uint32_t use_hi_address;            /*!< Non 0 means use upper address range for this device */
    uint32_t use_local_ints;            /*!< non 0 meams use local interrupts for MAC instead of PLIC */
    uint8_t  mac_addr[6];               /*!< Station's MAC address */
    mss_mac_speed_mode_t speed_mode;    /*!< Link speed mode of operation */
    uint32_t speed_duplex_select;       /*!< Link speed and duplex mode allowed to setup a link when autonegotiation is enabled. */

    /* PHY interface functions */
    mss_mac_phy_init_t            phy_init;              /*!< Pointer to PHY init function */
    mss_mac_phy_set_speed_t       phy_set_link_speed;    /*!< Pointer to PHY set link speed function */
    mss_mac_phy_autonegotiate_t   phy_autonegotiate;     /*!< Pointer to PHY autonegotiate function */
    mss_mac_phy_autonegotiate_t   phy_mac_autonegotiate; /*!< Pointer to PHY MAC link autonegotiate function */
    mss_mac_phy_get_link_status_t phy_get_link_status;   /*!< Pointer to PHY get link status function */
#if MSS_MAC_USE_PHY_DP83867
    mss_mac_phy_extended_read_t   phy_extended_read;     /*!< Pointer to PHY extended read function */
    mss_mac_phy_extended_write_t  phy_extended_write;    /*!< Pointer to PHY extended write function */
#endif

} mss_mac_instance_t;

/***************************************************************************//**
 * Flag value to indicate this MAC has been set up and is safe to call functions
 */
#define MSS_MAC_AVAILABLE (0XAAF1D055U)

//#define DEBUG_SPEED_CHANGE
#if defined(DEBUG_SPEED_CHANGE)
typedef struct mac_speed_info
{
    uint64_t             time_stamp;
    mss_mac_instance_t  *mac_ref;
    mss_mac_speed_t      speed;
    uint8_t              location;
    uint8_t              duplex;
    uint8_t              link_state;
} mac_speed_info_t;

#define MAX_SPEED_HISTORY 200
extern volatile uint64_t g_tick_counter;

extern mac_speed_info_t mac_speed_history[];
extern uint32_t mac_speed_history_count;

#endif
#ifdef __cplusplus
}
#endif

#endif /* MSS_ETHERNET_MAC_TYPES_H_ */
