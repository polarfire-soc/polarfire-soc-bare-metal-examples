/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_ethernet_mac.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) 10/100/1000 Mbps Ethernet
 * MAC bare metal software driver public API.
 *
 */

/*=========================================================================*//**
  @mainpage PolarFire SoC MSS Ethernet MAC Bare Metal Driver.

  @section intro_sec Introduction
    The PolarFire SoC Microcontroller Subsystem (MSS) includes two Gigabit
    Ethernet MAC (GEM) hardware peripherals which support communications at
    10/100/1000 Mbps.

    This software driver provides a set of functions for controlling the MSS
    Ethernet MAC as part of a bare metal system where no operating system is
    available. The drivers can be adapted for use as part of an operating
    system, but the implementation of the adaptation layer between the driver
    and the operating system's driver model is outside the scope of the driver.

    Each GEM consists of two Ethernet MAC devices, the pMAC (pre-emptable MAC)
    and the eMAC (express MAC) which share a single network port. The MMSL 
    (Media Merge Sub Layer) is responsible for multiplexing the network port 
    between the two MACs. When IEEE 802.3br Frame pre-emption is enabled, the
    transmission of packets via the pMAC can be interrupted by packets 
    transmitted via the eMAC. This can result in pMAC packets being split into
    two or more segments. __Note:__ For this feature to work, both link partners
    must support IEEE 802.3br Frame pre-emption.
    
    The GEMs support GMII/MII and SGMII connections to an external Ethernet PHY
    device and the device driver can configure the devices at run time to select
    which interface is active.

    The driver implements a sub-driver interface for configuring and controlling
    the PHY and example drivers for a number of different PHYs are provided with
    the driver.
    
    In addition to the standard Ethernet features, the GEMs support a number of
    advanced features including:
        - IEEE 1588 Time stamping support for PTPv1 and PTPv2.
        - IEEE 802.1Qci Receive traffic policing.
        - IEEE 802.1Qbb Priority based flow control.
        - IEEE 802.1Q VLAN tagging.
        - IEEE 802.1Qav Credit based traffic shaping.
        - IEEE 802.1Qaz Bandwidth based transmit queueing.
        - IEEE 802.1Qbv Time based priority queueing.
        - IEEE 802.1CB Frame Redundancy and Elimination.
        - IEEE 802.1AS Time stamping.
        - IEEE 802.3br Frame pre-emption.
        - TCP/UDP/IP Checksum offload.
        - TCP/IP Large Send Offload.
        - Advanced DMA system to provide high performance operation.

    The driver provides basic support for some of these features but more 
    advanced use of the features will require application level support and/or
    modifications to the driver.

    __Note:__ In terms of the standard Ethernet support, this driver is modeled
    on the MSS MAC Ethernet device drivers from previous generations of
    Microchip FPGA SoC devices to facilitate porting of designs to the MPFS
    platform. However unlike the previous MSS Ethernet MACs, the PolarFire SoC
    MSS supports up to 4 MACs. Each GEM device has 1 x pMAC and 1 x eMAC, and so
    most driver functions now have a parameter which identifies the specific GEM
    device and MAC that the function should act upon. In addition, the pMACs 
    support multiple queues and so some functions require a queue identifier as
    well.

  @section hw_dependencies Hardware Flow Dependencies
    The configuration of all features of the MSS Ethernet MAC is covered by this
    driver with the exception of the PolarFire SoC IOMUX configuration.
    PolarFire SoC allows multiple non-concurrent uses of some external pins
    through IOMUX configuration. This feature allows optimization of external
    pin usage by assigning external pins for use by either the microprocessor
    subsystem or the FPGA fabric. The MDIO signals are routed through IOMUXs to
    the PolarFire SoC device external pins. The MDIO signals may also be routed
    through IOMUXs to the PolarFire SoC FPGA fabric. The IOMUXs are configured
    using the PolarFire SoC MSS configurator tool. You must ensure that the MSS
    Ethernet MAC are enabled and configured in the PolarFire SoC MSS
    configurator if you wish to use them. For more information on IOMUX, refer
    to the I/O Configuration section of the PolarFire SoC Microprocessor
    Subsystem (MSS) User's Guide.

    The base address, register addresses and interrupt number assignment for the
    MSS Ethernet MAC are defined as constants in the PolarFire SoC HAL. You must
    ensure that the latest PolarFire SoC HAL is included in the project settings
    of the software tool chain used to build your project and that it is
    generated into your project.

    The MSS Ethernet MAC supports the following selections for the PHY
    interface:

    Table 1 PHY Interface

    ----------------------------------------------------------------------------
    |PHY Type   | Value | Explanation                                          |
    |-----------|-------|------------------------------------------------------|
    |NULL_PHY   |   1   | There is no PHY device connected. This configuration |
    |           |       | is used for testing where the GEM devices are cross  |
    |           |       | connected through the fabric or via direct external  |
    |           |       | links. This setting also supports local loopback     |
    |           |       | operation.                                           |
    |GMII       |   2   | MSS Ethernet MAC operates in GMII mode and interfaces|
    |           |       | with a GMII PHY directly.                            |
    |           |       | Maximum network speed is 1Gb.                        |
    |TBI        |   4   | MSS Ethernet MAC operates in TBI mode using the PCS  |
    |           |       | interface and the MSS in-built SGMII interface.      |
    |           |       | Maximum network speed is 1Gb.                        |
    |GMII_SGMII |   8   | MSS Ethernet MAC operates in GMII mode but the       |
    |           |       | interface to the PHY is through an SGMII to GMII     |
    |           |       | converter. This facilitates designs based on the G5  |
    |           |       | SoC Emulation Platform.                              |
    ----------------------------------------------------------------------------

  @section theory_op Theory of Operation
    The MSS Ethernet MAC driver functions are grouped into the following
    categories:
        - __Initialization and configuration__
        - __Transmit operations__
        - __Receive operations__
        - __Reading link status and statistics__
        - __Feature support__
        
    @subsection init Initialization and Configuration
    The MSS Ethernet MAC driver is initialized and configured by
    calling the _MSS_MAC_init()_ function. The _MSS_MAC_init()_ function takes
    as one of its parameters a pointer to a configuration data structure. This
    data structure contains all the configuration information required to
    initialize and configure the Ethernet MAC.
    The MSS Ethernet MAC driver provides the _MSS_MAC_cfg_struct_def_init()_
    function to initialize the configuration data structure entries to default
    values. It is recommended to use this function to retrieve the default
    configuration and then overwrite the defaults with the application specific
    settings such as PHY address, PHY type, interface type, allowed link speeds,
    link duplex mode and MAC address etc.
    
    The following functions are used as part of the initialization and
    configuration process:
        - _MSS_MAC_cfg_struct_def_init()_
        - _MSS_MAC_init()_
        
    @subsection tx_ops Transmit Operations
    The MSS Ethernet MAC driver transmit operations are interrupt driven. The
    application must register a transmit call-back function with the driver
    using the _MSS_MAC_set_tx_callback()_ function. This call-back function will
    be called by the MSS Ethernet MAC driver every time a packet has been sent.
    The application must call the _MSS_MAC_send_pkt()_ function every time it
    wants to transmit a packet. The application must pass a pointer to the
    buffer containing the packet to send. It is the application's responsibility
    to manage the memory allocated to store the transmit packets. The MSS
    Ethernet MAC driver only requires a pointer to the instance structure for
    the desired MAC, a pointer to the buffer containing the packet, the queue
    number and the packet size. The MSS Ethernet MAC driver will call the
    transmit call-back function registered using the _MSS_MAC_set_tx_callback()_
    function once a packet is sent. The transmit call-back function is supplied
    by the application and can be used to, for example, release the memory used
    to store the packet that was sent.
    
    The following functions are used as part of the transmit operations:
        - _MSS_MAC_send_pkt()_
        - _MSS_MAC_send_pkts()_
        - _MSS_MAC_set_tx_callback()_
        
    @subsection rx_ops Receive Operations
    The MSS Ethernet MAC driver receive operations are interrupt driven. The
    application must first register a receive call-back function using the
    _MSS_MAC_set_rx_callback()_ function. The application can then allocate
    receive buffers to the MSS Ethernet MAC driver by calling the
    _MSS_MAC_receive_pkt()_ function. This function can be called multiple times
    to allocate more than one receive buffer. The MSS Ethernet MAC driver will
    then call the receive call-back whenever a packet is received into one of
    the receive buffers. It will hand back the receive buffer to the application
    for packet processing. This buffer will not be reused by the MSS Ethernet
    MAC driver unless it is re-allocated to the driver by a call to
    _MSS_MAC_receive_pkt()_.
    
    The following functions are used as part of the receive operations:
        - _MSS_MAC_receive_pkt()_
        - _MSS_MAC_set_rx_callback()_
        
    @subsection stats Reading Status and Statistics
    The MSS Ethernet MAC driver provides the following functions to retrieve the
    current link status and statistics.
        - _MSS_MAC_get_link_status()_
        - _MSS_MAC_read_stat()_

    @subsection features Feature Support
    The MSS Ethernet MAC driver provides the following functions to support on
    the fly configuration and control of various features of the MSS
    Ethernet MAC devices.
        - _MSS_MAC_read_TSU()_
        - _MSS_MAC_init_TSU()_
        - _MSS_MAC_set_TSU_rx_mode()_
        - _MSS_MAC_set_TSU_tx_mode()_
        - _MSS_MAC_get_TSU_rx_mode()_
        - _MSS_MAC_get_TSU_tx_mode()_
        - _MSS_MAC_set_TSU_oss_mode()_
        - _MSS_MAC_get_TSU_oss_mode()_
        - _MSS_MAC_set_TSU_unicast_addr()_
        - _MSS_MAC_get_TSU_unicast_addr()_
        - _MSS_MAC_set_VLAN_only_mode()_
        - _MSS_MAC_get_VLAN_only_mode()_
        - _MSS_MAC_set_stacked_VLAN()_
        - _MSS_MAC_get_stacked_VLAN()_
        - _MSS_MAC_set_hash()_
        - _MSS_MAC_get_hash()_
        - _MSS_MAC_set_hash_mode()_
        - _MSS_MAC_get_hash_mode()_
        - _MSS_MAC_set_type_filter()_
        - _MSS_MAC_get_type_filter()_
        - _MSS_MAC_set_sa_filter()_
        - _MSS_MAC_get_sa_filter()_
        - _MSS_MAC_set_type_1_filter()_
        - _MSS_MAC_get_type_1_filter()_
        - _MSS_MAC_set_type_2_filter()_
        - _MSS_MAC_get_type_2_filter()_
        - _MSS_MAC_set_type_2_ethertype()_
        - _MSS_MAC_get_type_2_ethertype()_
        - _MSS_MAC_set_type_2_compare()_
        - _MSS_MAC_get_type_2_compare()_
        - _MSS_MAC_set_mmsl_mode()_
        - _MSS_MAC_get_mmsl_mode()_
        - _MSS_MAC_start_preemption_verify()_
        - _MSS_MAC_get_mmsl_status()_
        - _MSS_MAC_get_mmsl_stats()_
        - _MSS_MAC_set_tx_cutthru()_
        - _MSS_MAC_set_rx_cutthru()_
        - _MSS_MAC_get_tx_cutthru()_
        - _MSS_MAC_get_rx_cutthru()_
        - _MSS_MAC_tx_enable()_
        - _MSS_MAC_set_jumbo_frames_mode()_
        - _MSS_MAC_get_jumbo_frames_mode()_
        - _MSS_MAC_set_jumbo_frame_length()_
        - _MSS_MAC_get_jumbo_frame_length()_
        - _MSS_MAC_set_pause_frame_copy_to_mem()_
        - _MSS_MAC_get_pause_frame_copy_to_mem()_

 *//*=========================================================================*/
#ifndef MSS_ETHERNET_MAC_H_
#define MSS_ETHERNET_MAC_H_

#include <stdbool.h>
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * Length of a MAC address
 */
#define MSS_MAC_MAC_LEN (6)

/***************************************************************************//**
 *
 * False/True, Disable/Enable values used in driver
 */
#define MSS_MAC_DISABLE                     (0U)
#define MSS_MAC_ENABLE                      (1U)

/***************************************************************************//**
 * Driver API function return value definitions.
 */
#define MSS_MAC_SUCCESS         MSS_MAC_ENABLE  /*!< @brief Simple success value */
#define MSS_MAC_FAILED          MSS_MAC_DISABLE /*!< @brief Simple failure value */
#define MSS_MAC_ERR_OK          (1)             /*!< @brief Success by another name */
#define MSS_MAC_ERR_NOT_DONE    (0)             /*!< @brief Previous packet send not complete yet */
#define MSS_MAC_ERR_TX_NOT_OK   (-1)            /*!< @brief Non specific TX failure */
#define MSS_MAC_ERR_TX_TIMEOUT  (-2)            /*!< @brief Packet send did not release queue after send */
#define MSS_MAC_ERR_TX_FAIL     (-3)            /*!< @brief Packet send failed with errors */

/****************************************************************************//**
  The following definitions are used with function _MSS_MAC_get_link_status()_ to
  report the link status.
 */
#define MSS_MAC_LINK_DOWN                   (0U)
#define MSS_MAC_LINK_UP                     (1U)
#define MSS_MAC_HALF_DUPLEX                 (0U)
#define MSS_MAC_FULL_DUPLEX                 (1U) 

/***************************************************************************//**
 * Standard all 1s broadcast MAC address value
 */
#define MSS_MAC_BROADCAST_MAC_ADDRESS       0xFFU,0xFFU,0xFFU,0xFFU,0xFFU,0xFFU

/***************************************************************************//**
 * Maximum packet size that the hardware supports
 */
#define MSS_MAC_JUMBO_MAX (10240U)

/***************************************************************************//**
 * Maximum MAC frame size (packet size)
 *
 * This value should be defined based on the type of packets you intend to
 * support. For normal packets a value of 1518 is appropriate.
 *
 * If the receive_1536_byte_frames bit of the Network Config register is set
 * then the value 1536 is appropriate. This will be required if VLAN support is
 * used.
 *
 * If Jumbo frame support is enabled by setting the jumbo_frames bit of the
 * Network Config register, then a value up to 10240 bytes can be set.
 *
 * __Note:__ Default has size bumped up to support maximum jumbo packet size of
 * 10,240 bytes. The value chosen should match that of the jumbo_max_length
 * register if Jumbo frame support is enabled.
 *
 */
#if defined(MSS_MAC_USE_DDR)
#if MSS_MAC_USE_DDR == MSS_MAC_MEM_DDR
#define MSS_MAC_MAX_PACKET_SIZE                 MSS_MAC_JUMBO_MAX /* Smaller for Crypto and FIC tests */
#else
#define MSS_MAC_MAX_PACKET_SIZE                 (80U)
#endif
#else
#if (MSS_MAC_QUEUE_COUNT >= 2)
#define MSS_MAC_MAX_PACKET_SIZE                 MSS_MAC_JUMBO_MAX
#else
#define MSS_MAC_MAX_PACKET_SIZE                 MSS_MAC_JUMBO_MAX
#endif
#endif

/***************************************************************************//**
 * Calculate the RX Buffer size field value automatically by rounding 
 * _MSS_MAC_MAX_PACKET_SIZE_ up to nearest 64 bytes and dividing by 64.
 */
#define MSS_MAC_RX_BUF_VALUE ((MSS_MAC_MAX_PACKET_SIZE + 63U) / 64U)

/***************************************************************************//**
  The definition below is provided to specify that the _MSS_MAC_init()_ function
  should attempt to discover the address of the PHY connected to the MACs
  management interface. It can be used with _mss_mac_cfg_t_ configuration 
  parameter _phy_addr_.
  
  __Note:__ To auto detect the PHY address, this drivers scans the valid MDIO
  addresses starting from 0 looking for valid data. This should not be done if
  more than 1 device is connected to the MDIO interface.

 */
#define MSS_MAC_AUTO_DETECT_PHY_ADDRESS     ((uint8_t)255U)

/***************************************************************************//**
 * Transmit and receive packet buffer sizes.
 *
 * __Note:__ The odd addition/division/multiplication sequence is to ensure the
 * size is a multiple of 64 bits so that sequential buffers are 64 bit word
 * aligned in case we are using time stamp support.
 *
 */
#define MSS_MAC_MAX_TX_BUF_SIZE       (((MSS_MAC_MAX_PACKET_SIZE + 7U) / 8U) * 8U)
#define MSS_MAC_MAX_RX_BUF_SIZE       (((MSS_MAC_MAX_PACKET_SIZE + 7U) / 8U) * 8U)

/*******************************************************************************
 * Defines for configuration parameters
 */
/***************************************************************************//**
 * Queue enable / disable.
 *
 *__Note:__ Queue 0 is always enabled 
 */
#define MSS_MAC_QUEUE_ENABLE                        MSS_MAC_ENABLE
#define MSS_MAC_QUEUE_DISABLE                       MSS_MAC_DISABLE

/***************************************************************************//**
 * FIFO error detection & correction enable / disable 
 */
#define MSS_MAC_ERR_DET_CORR_ENABLE                 MSS_MAC_ENABLE
#define MSS_MAC_ERR_DET_CORR_DISABLE                MSS_MAC_DISABLE

/***************************************************************************//**
 * Jumbo frame support enable / disable
 *
 * __Note:__ This enables support for the feature but the reception of jumbo 
 * frames still needs to be explicitly enabled via
 * _MSS_MAC_set_jumbo_frames_mode()_
 */
#define MSS_MAC_JUMBO_FRAME_ENABLE                  MSS_MAC_ENABLE
#define MSS_MAC_JUMBO_FRAME_DISABLE                 MSS_MAC_DISABLE

/***************************************************************************//**
 * Length field checking enable / disable
 */
#define MSS_MAC_LENGTH_FIELD_CHECK_ENABLE           MSS_MAC_ENABLE
#define MSS_MAC_LENGTH_FIELD_CHECK_DISABLE          MSS_MAC_DISABLE

/***************************************************************************//**
 * Append CRC enable / disable
 */
#define MSS_MAC_CRC_ENABLE                          MSS_MAC_ENABLE
#define MSS_MAC_CRC_DISABLE                         MSS_MAC_DISABLE

/***************************************************************************//**
 * Full duplex mode enable / disable 
 */
#define MSS_MAC_FULLDUPLEX_ENABLE                   MSS_MAC_ENABLE
#define MSS_MAC_FULLDUPLEX_DISABLE                  MSS_MAC_DISABLE

/***************************************************************************//**
 * Loopback mode enable / disable
 */
#define MSS_MAC_LOOPBACK_ENABLE                     MSS_MAC_ENABLE
#define MSS_MAC_LOOPBACK_DISABLE                    MSS_MAC_DISABLE

/***************************************************************************//**
 * Receiver flow control enable / disable 
 */
#define MSS_MAC_RX_FLOW_CTRL_ENABLE                 MSS_MAC_ENABLE
#define MSS_MAC_RX_FLOW_CTRL_DISABLE                MSS_MAC_DISABLE

/***************************************************************************//**
 * Transmission flow control enable / disable 
 */
#define MSS_MAC_TX_FLOW_CTRL_ENABLE                 MSS_MAC_ENABLE
#define MSS_MAC_TX_FLOW_CTRL_DISABLE                MSS_MAC_DISABLE

/***************************************************************************//**
 * Default IPG/IFG value which results in the standard minimum IPG.
 */
#define MSS_MAC_IPG_DEFVAL                          (0x00U)

/***************************************************************************//**
 * PHY clock divider values.
 *
 * __Note:__ The value used should not result in a clock frequency greater than
 * 2.5MHz.
 */
#define MSS_MAC_BY8_PHY_CLK                         (0U)
#define MSS_MAC_BY16_PHY_CLK                        (1U)
#define MSS_MAC_BY32_PHY_CLK                        (2U)
#define MSS_MAC_BY48_PHY_CLK                        (3U)
#define MSS_MAC_BY64_PHY_CLK                        (4U)
#define MSS_MAC_BY96_PHY_CLK                        (5U)
#define MSS_MAC_BY128_PHY_CLK                       (6U)
#define MSS_MAC_BY224_PHY_CLK                       (7U)
#if defined(TARGET_ALOE)
#define MSS_MAC_DEF_PHY_CLK                         MSS_MAC_BY224_PHY_CLK /*!< @brief For Aloe this is divide by 96, good for up to 560MHz */
#else
#define MSS_MAC_DEF_PHY_CLK                         MSS_MAC_BY96_PHY_CLK /*!< @brief For MPFS this is divide by 96, good for up to 240MHz */
#endif

/***************************************************************************//**
 * PHY addresse and register number limits.
 */
#define MSS_MAC_PHYADDR_MAXVAL                      ( (uint8_t)(0x1FU) )
#define MSS_MAC_PHYREGADDR_MAXVAL                   ( (uint8_t)(0x1FU) )

/***************************************************************************//**
 * Maximum frame length default & maximum values.
 */
#define MSS_MAC_MAXFRAMELEN_DEFVAL                  (0x00000600U)
#define MSS_MAC_MAXFRAMELEN_MAXVAL                  (0x00002800U)

/***************************************************************************//**
 * Options for link speed and duplex configuration for autonegotiation.
 *
 * These may be or'ed together to construct a set of speed/duplex options to
 * allow. _MSS_MAC_ANEG_ALL_SPEEDS_ is the default and implies full
 * autonegotiation.
 */
#define MSS_MAC_ANEG_10M_FD             (0x00000001U)
#define MSS_MAC_ANEG_10M_HD             (0x00000002U)
#define MSS_MAC_ANEG_100M_FD            (0x00000004U)
#define MSS_MAC_ANEG_100M_HD            (0x00000008U)
#define MSS_MAC_ANEG_1000M_FD           (0x00000010U)
#define MSS_MAC_ANEG_1000M_HD           (0x00000020U)
#define MSS_MAC_ANEG_ALL_SPEEDS         (MSS_MAC_ANEG_10M_FD | MSS_MAC_ANEG_10M_HD | \
                                         MSS_MAC_ANEG_100M_FD | MSS_MAC_ANEG_100M_HD | \
                                         MSS_MAC_ANEG_1000M_FD | MSS_MAC_ANEG_1000M_HD)

#if defined(TARGET_G5_SOC)
extern mss_mac_instance_t g_mac0;
extern mss_mac_instance_t g_mac1;
extern mss_mac_instance_t g_emac0;
extern mss_mac_instance_t g_emac1;
#endif

#if defined(TARGET_ALOE)
extern mss_mac_instance_t g_mac0;
#endif

/* Support for testing access to different memory areas */
#if defined(MSS_MAC_USE_DDR)
extern uint8_t *g_mss_mac_ddr_ptr;
#endif

/***************************************************************************//**
 * Specific Address Filter support definitions.
 *
 * These may be or'ed together to construct the control value for the
 * _MSS_MAC_set_sa_filter()_ function. The value _MSS_MAC_SA_FILTER_DISABLE_ is
 * used to indicate a filter should be disabled.
 */
#define MSS_MAC_SA_FILTER_SOURCE     (0x0001U)
#define MSS_MAC_SA_FILTER_BYTE0      (0x0100U)
#define MSS_MAC_SA_FILTER_BYTE1      (0x0200U)
#define MSS_MAC_SA_FILTER_BYTE2      (0x0400U)
#define MSS_MAC_SA_FILTER_BYTE3      (0x0800U)
#define MSS_MAC_SA_FILTER_BYTE4      (0x1000U)
#define MSS_MAC_SA_FILTER_BYTE5      (0x2000U)
#define MSS_MAC_SA_FILTER_DISABLE    (0xFFFFU) /* Use this to signal we should disable filter */

/***************************************************************************//**
 * Type 2 Filter support data offset definitions. These determine the reference
 * point used to convert the offset value into an absolute location within the
 * frame.
 */
#define MSS_MAC_T2_OFFSET_FRAME      (0U)
#define MSS_MAC_T2_OFFSET_ETHERTYPE  (1U)
#define MSS_MAC_T2_OFFSET_IP         (2U)
#define MSS_MAC_T2_OFFSET_TCP_UDP    (3U)


/**************************************************************************/
/* Public Function declarations                                           */
/**************************************************************************/

/***************************************************************************//**
  The _MSS_MAC_cfg_struct_def_init()_ function initializes a _mss_mac_cfg_t_
  configuration data structure to default values. The default configuration uses
  the NULL_PHY interface connected to a PHY at address 0x00 which is set to
  auto-negotiate at all available speeds up to 1000Mbps. This default
  configuration can then be used as parameter to _MSS_MAC_init()_. Typically,
  the default configuration would be modified to suit the application before
  being passed to _MSS_MAC_init()_. 
 
  @param cfg
    This parameter is a pointer to an _mss_mac_cfg_t_ data structure that will
    be used as parameter to function _MSS_MAC_init()_.
 
  @return
    This function does not return a value.

  Example:
  The example below demonstrates the use of the _MSS_MAC_cfg_struct_def_init()_
  function. It retrieves the default MAC configuration and modifies it to
  connect through a VSC8757 SGMII Ethernet PHY at MII management interface
  address 0x01. This example also demonstrates how to assign the device's MAC
  address and force a 100Mbps full duplex link.
  @code
    mss_mac_cfg_t mac_config;
    
    MSS_MAC_cfg_struct_def_init(&mac_config);

    mac_config.interface_type = TBI;
    config.phy_type = MSS_MAC_DEV_PHY_VSC8575;
    config.phy_get_link_status = MSS_MAC_VSC8575_phy_get_link_status;
    config.phy_init = MSS_MAC_VSC8575_phy_init;
    config.phy_set_link_speed = MSS_MAC_VSC8575_phy_set_link_speed;
    config.phy_extended_read = NULL_ti_read_extended_regs;
    config.phy_extended_write = NULL_ti_write_extended_regs;
    mac_config.phy_addr = 0x01;
    mac_config.speed_duplex_select = MSS_MAC_ANEG_100M_FD;
    mac_config.mac_addr[0] = 0xC0u;
    mac_config.mac_addr[1] = 0xB1u;
    mac_config.mac_addr[2] = 0x3Cu;
    mac_config.mac_addr[3] = 0x88u;
    mac_config.mac_addr[4] = 0x88u;
    mac_config.mac_addr[5] = 0x88u;
    
    MSS_MAC_init(&g_mac0, &mac_config);
  @endcode
 */
void 
MSS_MAC_cfg_struct_def_init
(
    mss_mac_cfg_t * cfg
);

/***************************************************************************//**
  The _MSS_MAC_init()_ function initializes the Ethernet MAC hardware and driver
  internal data structures. In addition to the MAC identifier, the
  _MSS_MAC_init()_ function takes a pointer to a configuration data structure of
  type _mss_mac_cfg_t_ as parameter. This configuration data structure contains
  all the information required to configure the Ethernet MAC. The
  _MSS_MAC_init()_ function initializes the descriptor rings and their pointers
  to initial values. The _MSS_MAC_init()_ function enables DMA Rx packet
  received and Tx packet sent interrupts. The configuration passed to the
  _MSS_MAC_init()_ function specifies the type of interface used to connect the
  Ethernet MAC and Ethernet PHY as well as the PHY type and PHY MII management
  interface address. It also specifies the allowed link speed and duplex mode.
  It is at this point that the application chooses if the link  speed and duplex
  mode will be auto-negotiated with the link partner or forced to a specific 
  speed and duplex mode.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param cfg
    This parameter is a pointer to a data structure of type _mss_mac_cfg_t_
    containing the Ethernet MAC's requested configuration. You must initialize
    this data structure by first calling the _MSS_MAC_cfg_struct_def_init()_
    function to fill the configuration data structure with default values. You
    can then overwrite some of the default settings with the ones specific to
    your application before passing this data structure as parameter to the call
    to the _MSS_MAC_init()_ function. You must at a minimum overwrite the
    mac_addr[6] array of the configuration data structure to contain a unique
    value used as the device's MAC address.

  @return
    This function does not return a value.

  Example:
  @code
    mss_mac_cfg_t cfg;

    MSS_MAC_cfg_struct_def_init(&cfg);
    
    cfg.mac_addr[0] = 0xC0u;
    cfg.mac_addr[1] = 0xB1u;
    cfg.mac_addr[2] = 0x3Cu;
    cfg.mac_addr[3] = 0x88u;
    cfg.mac_addr[4] = 0x88u;
    cfg.mac_addr[5] = 0x88u;

    MSS_MAC_init(&g_mac0, &cfg);
  @endcode
 */
void 
MSS_MAC_init
( 
    mss_mac_instance_t *this_mac,
    mss_mac_cfg_t * cfg
);

/***************************************************************************//**
  The _MSS_MAC_update_hw_address()_ function updates the MAC address for the
  Ethernet MAC. In addition to the MAC identifier, the
  _MSS_MAC_update_hw_address()_ function takes a pointer to a configuration data
  structure of type _mss_mac_cfg_t_ as a parameter. This configuration data
  structure contains all the information required to configure the Ethernet MAC
  including the required MAC address. The _MSS_MAC_update_hw_address()_ function
  reconfigures the Ethernet MAC using the MAC address contained in the structure
  pointed to by cfg.

  This function should only be called after the MSS Ethernet MAC has been
  initialized as it does not perform any other initialization/configuration.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param cfg
    This parameter is a pointer to a data structure of _type mss_mac_cfg_t_
    containing the Ethernet MAC's requested configuration.
    For this function, the only field of importance is the _mac_addr_ array, all
    other fields are ignored.

  @return
    This function does not return a value.

  Example:
  @code
    mss_mac_cfg_t cfg;

    MSS_MAC_cfg_struct_def_init(&cfg);

    cfg.mac_addr[0] = 0xC0u;
    cfg.mac_addr[1] = 0xB1u;
    cfg.mac_addr[2] = 0x3Cu;
    cfg.mac_addr[3] = 0x88u;
    cfg.mac_addr[4] = 0x88u;
    cfg.mac_addr[5] = 0x88u;

    MSS_MAC_init(&g_mac_0, &cfg);

    ....

    cfg.mac_addr[0] = 0xC0u;
    cfg.mac_addr[1] = 0xB1u;
    cfg.mac_addr[2] = 0x3Cu;
    cfg.mac_addr[3] = 0x88u;
    cfg.mac_addr[4] = 0x88u;
    cfg.mac_addr[5] = 0x89u;

    MSS_MAC_update_hw_address(&g_mac0, &cfg);
  @endcode
 */
void
MSS_MAC_update_hw_address
(
    mss_mac_instance_t *this_mac,
    const mss_mac_cfg_t * cfg
);

/***************************************************************************//**
  The _MSS_MAC_set_tx_callback()_ function registers the function that will be
  called by the Ethernet MAC driver when a packet has been sent on the specified
  queue.
 
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param queue_no
    This parameter identifies the queue which this callback function will
    service. For single queue devices, this should be set to 0 for compatibility
    purposes.

  @param tx_complete_handler
    This parameter is a pointer to the function that will be called when a
    packet is sent by the Ethernet MAC on the selected queue.
  
  @return
    This function does not return a value.

 */
void MSS_MAC_set_tx_callback
(
    mss_mac_instance_t *this_mac,
    uint32_t queue_no,
    mss_mac_transmit_callback_t tx_complete_handler
);

/***************************************************************************//**
  The _MSS_MAC_set_rx_callback()_ function registers the function that will be
  called by the Ethernet MAC driver when a packet is received.
  
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param queue_no
    This parameter identifies the queue which this callback function will
    service. For single queue devices this should be set to 0 for compatibility
    purposes.

  @param rx_callback
    This parameter is a pointer to the function that will be called when the a
    packet is received by Ethernet MAC on the selected queue.

  @return
    This function does not return a value.

  Example:
  The example below demonstrates the use of the _MSS_MAC_set_rx_callback()_
  function. The _init()_ function calls the _MSS_MAC_set_rx_callback()_ function
  to register the _rx_callback()_ receive callback function with the Ethernet 
  MAC driver. The _MSS_MAC_receive_pkt()_ function is then called to assign the
  rx_buffer to an Ethernet MAC descriptor for packet reception. The
  _rx_callback()_ function will be called by the Ethernet MAC driver once a
  packet has been received into _rx_buffer_. The _rx_callback()_ function calls
  the _process_rx_packet()_ application function to process the received packet
  then calls _MSS_MAC_receive_pkt()_ to reallocate rx_buffer to receive another
  packet. The _rx_callback()_ function will be called again every time a packet
  is received to process the received packet and reallocate _rx_buffer_ for
  packet reception.
  
  @code
    uint8_t rx_buffer[MSS_MAC_MAX_RX_BUF_SIZE];
    
    void rx_callback
    (
        void *this_mac,
        uint32_t queue_no,
        uint8_t * p_rx_packet,
        uint32_t pckt_length,
        mss_mac_rx_desc_t *cdesc,
        void * caller_info
    )
    {
        process_rx_packet(p_rx_packet, pckt_length);
        MSS_MAC_receive_pkt((mss_mac_instance_t *)this_mac, queue_no,
                            p_rx_packet, caller_info, MSS_MAC_INT_ENABLE);
    }
    
    void init(void)
    {
        MSS_MAC_set_rx_callback(rx_callback);
        MSS_MAC_receive_pkt(&g_mac0, 0, rx_buffer, (void *)0, MSS_MAC_INT_ARM);
    }
  @endcode
 */
void MSS_MAC_set_rx_callback
(
    mss_mac_instance_t *this_mac,
    uint32_t queue_no,
    mss_mac_receive_callback_t rx_callback
);

/***************************************************************************//**
  The _MSS_MAC_change_speed()_ function sets the speed and duplex mode for the
  link and if autonegotiation is selected as the speed mode, also sets the speed
  and duplex options to advertise.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.
    
  @param speed_duplex_select
    This parameter identifies the speed and duplex options to advertise to the
    link partner if autonegotiation is selected as the speed mode.

  @param speed_mode
    This parameter selects between autonegotiation and one of the six speed and
    duplex options. Valid values are:
    
     - _MSS_MAC_SPEED_AN_
     - _MSS_MAC_10_HDX_
     - _MSS_MAC_10_FDX_
     - _MSS_MAC_100_HDX_
     - _MSS_MAC_100_FDX_
     - _MSS_MAC_1000_HDX_
     - _MSS_MAC_1000_FDX_

    if _MSS_MAC_SPEED_AN_ is selected when previously there was a static speed
    selection or if the autonegotiation advertisement select is changed then an
    autonegotiation operation is kicked off via the PHY which may cause
    temporary link loss.

    __Note:__ Selecting _MSS_MAC_1000_HDX_ or _MSS_MAC_1000_FDX_ is not
    recommended, if fixed 1G operation is required, the recommended approach is
    to use the _MSS_MAC_SPEED_AN_ option as the PHY and/or link partner may not
    be able to honor the request otherwise.
 */
void MSS_MAC_change_speed
(
    mss_mac_instance_t *this_mac, uint32_t speed_duplex_select, mss_mac_speed_mode_t speed_mode
);

/***************************************************************************//**
  The _MSS_MAC_send_pkt()_ function initiates the transmission of a packet. It
  places the buffer containing the packet to send into one of the Ethernet MAC's
  transmit descriptors.

  This function is non-blocking. It will return immediately without waiting for
  the packet to be sent. The Ethernet MAC driver indicates that the packet is
  sent by calling the transmit completion handler registered by a call to
  _MSS_MAC_set_tx_callback()_.
  
  This function waits for all active transmission(s) on all queues to complete
  before initiating a new transmission.

  Some of the error responses for this function refer to issues with the
  previous transmission operation and it may be appropriate to retry the current
  request in that case.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param queue_no
    This parameter identifies the queue to which this transmit operation
    applies. For single queue devices this should be set to 0 for compatibility
    purposes.

  @param tx_buffer
    This parameter is a pointer to the buffer containing the packet to send.
    
  @param tx_length
    This parameter specifies the length in bytes of the packet to send.
    
  @param p_user_data
    This parameter is a pointer to an optional application defined data
    structure. Its usage is left to the application. It is intended to help the
    application manage memory allocated to store packets. The Ethernet MAC
    driver does not make use of this pointer. The Ethernet MAC driver will pass
    back this pointer to the application as part of the call to the transmit
    completion handler registered by the application.
    
  @return
    This function returns the following values:

     - ___MSS_MAC_ERR_OK___ on successfully launching the packet.
     - ___MSS_MAC_ERR_NOT_DONE___ if the previous packet has not finished
          sending.
     - ___MSS_MAC_ERR_NOT_OK___ for general errors.
     - ___MSS_MAC_ERR_TX_TIMEOUT___ If the previous packet has not released the
          queue buffers after the MAC completes the send.
     - ___MSS_MAC_ERR_TX_FAIL___ If the previous packet has not released the
          queue buffers after the MAC completes the send and there is an error
          flagged in the tx descriptor.

  Example:
  This example demonstrates the use of the _MSS_MAC_send_pkt()_ function. The
  application registers the _tx_complete_callback()_ transmit completion
  callback function with the Ethernet MAC driver by a call to
  _MSS_MAC_set_tx_callback()_. The application dynamically allocates memory for
  an application defined _packet_t_ data structure, builds a packet and calls
  _send_packet()_. The _send_packet()_ function extracts the pointer to the
  buffer containing the data to transmit and its length from the _tx_packet_
  data structure and passes these to _MSS_MAC_send_pkt()_. It also passes the 
  pointer to _tx_packet_ as the _p_user_data_ parameter. The Ethernet MAC driver
  calls _tx_complete_callback()_ once the packet is sent. The 
  _tx_complete_callback()_ function uses _p_user_data_, which points to
  _tx_packet_, to release memory allocated by the application to store the
  transmit packet.
  @code
    
    void tx_complete_handler(void *this_mac, uint32_t queue_no,
                             mss_mac_tx_desc_t *cdesc, void * caller_info);
    
    void init(void)
    {
        MSS_MAC_set_tx_callback(tx_complete_handler);
    }
    
    void tx_complete_handler(void *this_mac, uint32_t queue_no,
                             mss_mac_tx_desc_t *cdesc, void * caller_info)
    {
        release_packet_memory(caller_info);
    }
    
    void send_packet(app_packet_t * packet)
    {
        MSS_MAC_send_pkt(void *this_mac, 0, packet->buffer, packet->length, packet);
    }
    
  @endcode
 */
int32_t
MSS_MAC_send_pkt
(
    mss_mac_instance_t *this_mac,
    uint32_t queue_no,
    uint8_t const * tx_buffer,
    uint32_t tx_length,
    void * p_user_data
);


/***************************************************************************//**
  The _MSS_MAC_send_pkts()_ function initiates the transmission of one or more
  packets on one or more queues. It initialises all the required transmit queues
  and sets the transmit operation in motion.

  Due to the requirement to keep an unused descriptor at the end of the
  per-queue descriptor chains, this function will append at most
  _MSS_MAC_TX_RING_SIZE_ - 1 packets to any queue. Any packets above this limit
  will be silently ignored.

  This function is non-blocking. It will return immediately without waiting for
  the packets to be sent. The Ethernet MAC driver indicates that each packet is
  sent by calling the transmit completion handler registered by a call to
  _MSS_MAC_set_tx_callback()_.

  This function waits for all active transmissions on all queues to complete
  before initiating a new transmission.

  Some of the error responses for this function refer to issues with the
  previous transmission operation and it may be appropriate to retry the current
  request in that case.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param queue_mask
    This parameter identifies which queues it is intended to transmit from so
    that the driver can mask the appropriate queue interrupts. Queue 0 is always
    assumed to be masked as the transmit operation must write to the queue 0
    descriptor register even when queue 0 is not being used.

    queue_mask indicates the queues which may be involved so that we can
    properly handle ISR vs normal calling.

    Set the mask to 0x0000000F to indicate all queues if there is any
    uncertainty.

    Set the mask to 0xFFFFFFF0 to indicate we are being called from a GEM ISR
    context.

    For single queue devices this should be set to 0 for compatibility
    purposes.

  @param p_packets
    This parameter is a pointer to the list of packets to send. The list
    consists of a sequence of _mss_mac_tx_pkt_info_t_ structures with the last
    entry having a length field of 0.

  @return
    This function returns the following values:

     - ___MSS_MAC_ERR_OK___ on successfully launching the packet.
     - ___MSS_MAC_ERR_NOT_DONE___ if the previous packet has not finished
          sending.
     - ___MSS_MAC_ERR_NOT_OK___ for general errors.
     - ___MSS_MAC_ERR_TX_TIMEOUT___ If the previous packet has not released the
          queue buffers after the MAC completes the send.
     - ___MSS_MAC_ERR_TX_FAIL___ If the previous packet has not released the
          queue buffers after the MAC completes the send and there is an error
          flagged in the tx descriptor.

  Example:
  This example demonstrates the use of the _MSS_MAC_send_pkts()_ function. The
  application builds a list of 10 arp packets over multiple queues and then
  calls _MSS_MAC_send_pkts()_ to transmit them in one go.

  @code

    int32_t send_arps()
    {
        int32_t tx_status;
        mss_mac_tx_pkt_info_t packet_array[11];
        int count;

        for(count = 0; count != 10; count++)
        {
            packet_array[count].tx_buffer   = tx_pak_arp;
            packet_array[count].length      = sizeof(tx_pak_arp);
            packet_array[count].queue_no    = count % 4;
            packet_array[count].p_user_data = (void *)0;
        }

        packet_array[count].tx_buffer   = (void *)0; // End of list
        packet_array[count].length      = 0;
        packet_array[count].queue_no    = 0;
        packet_array[count].p_user_data = (void *)0;

        memcpy(&tx_pak_arp[6], g_test_mac->mac_addr, 6);
        tx_status = MSS_MAC_send_pkts(g_test_mac, 1, packet_array);
    }

  @endcode
 */
int32_t
MSS_MAC_send_pkts
(
    mss_mac_instance_t    *this_mac,
    uint32_t               queue_mask,
    mss_mac_tx_pkt_info_t *p_packets
);

#if defined(MSS_MAC_SPEED_TEST)
/***************************************************************************//**
 * Non standard function for network saturation speed tests. Not for normal use.
 */
int32_t
MSS_MAC_send_pkts_fast
(
    mss_mac_instance_t    *this_mac,
    mss_mac_tx_desc_t *descriptors,
    uint32_t tx_count
);
#endif

/***************************************************************************//**
  The _MSS_MAC_receive_pkt()_ function assigns a buffer to one of  the Ethernet
  MAC's receive descriptors. The receive buffer specified as parameter will be
  used to receive one single packet. The receive buffer will be handed back to
  the application via a call to the receive callback function assigned through a
  call to _MSS_MAC_set_rx_callback()_. The _MSS_MAC_receive_pkt()_ function will
  need to be called again pointing to the same buffer if more packets are to be
  received into this same buffer after the packet has been processed by the
  application.
  
  The _MSS_MAC_receive_pkt()_ function is non-blocking. It will return
  immediately and does not wait for a packet to be received. The application 
  needs to implement a receive callback function to be notified that a packet
  has been received.
  
  The _p_user_data_ parameter can be optionally used to point to a memory
  management data structure managed by the application.
 
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param queue_no
    This parameter identifies the queue to which this transmit operation
    applies. For single queue devices this should be set to 0 for compatibility
    purposes.

  @param rx_pkt_buffer
    This parameter is a pointer to a memory buffer. It points to the memory that
    will be assigned to one of the Ethernet MAC's receive descriptors. It must
    point to a buffer large enough to contain the largest possible packet.
  
  @param p_user_data
    This parameter is intended to help the application manage memory. Its usage
    is left to the application. The Ethernet MAC driver does not make use of
    this pointer. The Ethernet MAC driver will pass this pointer back to the
    application as part of the call to the application's receive callback
    function to help the application associate the received packet with the
    memory it allocated prior to the call to _MSS_MAC_receive_pkt()_.
 
  @param enable
    This parameter controls the enabling of the Ethernet MAC interrupt. If set
    to _MSS_MAC_INT_DISABLE_, the interrupt is left disabled by this function.
    If set to _MSS_MAC_INT_ENABLE_, the interrupt is enabled on return from this
    function. This allows a series of packet buffers to be assigned without
    there being any modifications to the internal packet buffer tables. To
    achieve this, enable should be _MSS_MAC_INT_DISABLE_ for all but the last
    call to the function. When allocating a chain of buffers, the last call to
    _MSS_MAC_send_pkt()_ should use a value of _MSS_MAC_INT_ARM_ to set up the
    RX interrupt for the first time.

  @return
    This function returns 1 on successfully assigning the buffer to a receive
    descriptor. It returns 0 otherwise.
  
  Example:
  The example below demonstrates the use of the _MSS_MAC_receive_pkt()_ function
  to handle packet reception using two receive buffers. The _init()_ function
  calls the _MSS_MAC_set_rx_callback()_ function to register the _rx_callback()_
  receive callback function with the Ethernet MAC driver. The
  _MSS_MAC_receive_pkt()_ function is then called twice to assign _rx_buffer_1_
  and _rx_buffer_2_ to Ethernet MAC descriptors for packet reception. The 
  _rx_callback_ function will be called by the Ethernet MAC driver once a packet
  has been received into one of the receive buffers. The _rx_callback()_
  function calls the _process_rx_packet()_ application function to process the
  received packet then calls _MSS_MAC_receive_pkt()_ to reallocate the receive
  buffer to receive another packet. The _rx_callback()_ function will be called
  again every time a packet is received to process the received packet and
  reallocate _rx_buffer_ for packet reception.
  
  __Note:__ The use of the _p_user_data parameter_ to handle the buffer
  reassignment to the Ethernet MAC as part of the _rx_callback()_ function. This
  is a simplistic use of _p_user_data_. It is more likely that _p_user_data_
  would be useful to keep track of a pointer to a TCP/IP stack packet container
  data structure dynamically allocated. In this more complex use case, the first
  parameter of _MSS_MAC_receive_pkt()_ would point to the actual receive buffer
  and the second parameter would point to a data structure used to free the
  receive buffer memory once the packet has been consumed by the TCP/IP stack.

  @code
    uint8_t rx_buffer_1[MSS_MAC_MAX_RX_BUF_SIZE];
    uint8_t rx_buffer_2[MSS_MAC_MAX_RX_BUF_SIZE];
    
    void rx_callback
    (
        void *this_mac,
        uint32_t queue_no,
        uint8_t * p_rx_packet,
        uint32_t pckt_length,
        mss_mac_rx_desc_t *cdesc,
        void * caller_info
    )
    {
        process_rx_packet(p_rx_packet, pckt_length);
        MSS_MAC_receive_pkt((mss_mac_instance_t *)this_mac, queue_no,
                            p_rx_packet, caller_info, MSS_MAC_INT_ENABLE);
    }
    
    void init(void)
    {
        MSS_MAC_set_rx_callback(&g_mac_0, 0, rx_callback);
        MSS_MAC_receive_pkt(&g_mac_0, 0, rx_buffer_1, (void *)rx_buffer_1,
                            MSS_MAC_INT_DISABLE);
        MSS_MAC_receive_pkt(&g_mac_0, 0, rx_buffer_2, (void *)rx_buffer_2,
                            MSS_MAC_INT_ARM);
    }
  @endcode
 */
uint8_t
MSS_MAC_receive_pkt
(
    mss_mac_instance_t *this_mac,
    uint32_t queue_no,
    uint8_t * rx_pkt_buffer,
    void * p_user_data,
    mss_mac_rx_int_ctrl_t enable
);


#if defined MSS_MAC_UNH_TEST
  /***************************************************************************//**
   * Non standard function for network saturation speed tests. Not for normal use.
   */
void
MSS_MAC_receive_pkt_isr
(
  mss_mac_instance_t *this_mac
);
#endif


/***************************************************************************//**
  The _MSS_MAC_get_link_status()_ function retrieves the status of the link from
  the Ethernet PHY. It returns the current state of the Ethernet link. The speed
  and duplex mode of the link is also returned via the two pointers passed as
  parameter if the link is up, if the link is not up then these values willl not
  be updated.
  
  This function also adjusts the Ethernet MAC's internal configuration if some
  of the link characteristics have changed since the previous call to this
  function. Calling this function periodically is important for maintaining the
  link operation.
  
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param speed
    This parameter is a pointer to variable of type _mss_mac_speed_t_ where the
    current link speed will be stored if the link is up. This variable is not
    updated if the link is down. 
    
    This parameter can be set to zero if the caller does not need to find out
    the link speed.
  
  @param fullduplex
    This parameter is a pointer to an unsigned character where the current link
    duplex mode will be stored if the link is up. This variable is not updated
    if the link is down.
  
    This parameter can be set to zero if the caller does not need to find out
    the link duplex.

  @return
    This function returns 1 if the link is up. It returns 0 if the link is down.

  Example:
  @code
    uint8_t link_up;
    mss_mac_speed_t speed;
    uint8_t full_duplex
    link_up = MSS_MAC_get_link_status(&gmac_0, &speed, &full_duplex);
  @endcode
 */
uint8_t MSS_MAC_get_link_status
(
    const mss_mac_instance_t *this_mac,
    mss_mac_speed_t * speed,
    uint8_t * fullduplex
);


/***************************************************************************//**
  The _MSS_MAC_read_stat()_  function reads the transmit and receive statistics
  of the selected Ethernet MAC. This function can be used to read one of 26 
  receive statistics and 20 transmitter statistics as defined in the 
  _mss_mac_stat_t_ enumeration.

  Reading a statistics value automatically zeroes the associated statistics
  count register. If a statistics counter reaches its maximum value, it will not
  roll over to 0 so it is important to collect the statistics regularly if you
  wish to keep an accurate account of the statistics.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param stat
    This parameter of type _mss_mac_stat_t_ identifies the statistic that will
    be read.

  @return
    This function returns the value of the requested statistic.

  Example:
  @code
    uint64_t tx_pkts_cnt = 0;
    
    ...
    
    tx_pkts_cnt += MSS_MAC_read_stat(FRAMES_TXED_OK);
  @endcode
 */
uint32_t 
MSS_MAC_read_stat
(
    const mss_mac_instance_t *this_mac,
    mss_mac_stat_t stat
);

/***************************************************************************//**
  @brief
    The _MSS_MAC_clear_statistics()_ function clears all the statistics counter
    registers for the selected MAC.
    
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function does not return a value.
 */
void MSS_MAC_clear_statistics
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_read_phy_reg()_ function reads the Ethernet PHY register
  specified as parameter. It uses the MII management interface to communicate
  with the Ethernet PHY. This function is used by the Ethernet PHY drivers
  provided alongside the Ethernet MAC driver. You normally only need to use this
  function if writing your own Ethernet PHY driver.
  
  This function always accesses the pMAC registers as the same PHY is shared by
  both eMAC and pMAC and only has an MDIO connection to the pMAC.
 
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param phyaddr
    This parameter is the 5-bit address of the Ethernet PHY on the MII
    management interface. This address is typically defined through Ethernet PHY
    hardware configuration signals. Please refer to the Ethernet PHY's datasheet
    for details of how this address is assigned.
 
  @param regaddr
    This parameter is the address of the Ethernet PHY register to be read. This
    address is the offset of the register within the Ethernet PHY's register
    map.

  @return 
    This function returns the 16-bit value of the requested register.

  Example:
  @code
    #include "phy.h"
    uint16_t read_phy_status(uint8_t phy_addr)
    {
        uint16_t phy_status = MSS_MAC_read_phy_reg(&g_mac0, phy_addr, MII_BMSR);
        return phy_status;
    }
  @endcode
 */
uint16_t
MSS_MAC_read_phy_reg
(
    const mss_mac_instance_t *this_mac,
    uint8_t phyaddr,
    uint8_t regaddr
);

/***************************************************************************//**
  The _MSS_MAC_write_phy_reg()_ function writes a 16-bit value to the specified
  Ethernet PHY register. It uses the MII management interface to communicate
  with the Ethernet PHY. This function is used by the Ethernet PHY drivers
  provided alongside the Ethernet MAC driver. You normally only need to use this
  function if writing your own Ethernet PHY driver.
  
  This function always accesses the pMAC registers as the same PHY is shared by
  both eMAC and pMAC and only has an MDIO connection to the pMAC.
 
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param phyaddr
    This parameter is the 5-bit address of the Ethernet PHY on the MII
    management interface. This address is typically defined through Ethernet PHY
    hardware configuration signals. Please refer to the Ethernet PHY's datasheet
    for details of how this address is assigned.
 
  @param regaddr
    This parameter is the address of the Ethernet register that will be written
    to. This address is the offset of the register within the Ethernet PHY's
    register map.

  @param regval
    The parameter is the 16-bit value that will be written into the specified
    PHY register.

  @return
    This function does not return a value.

  Example:
  @code
    #include "mss_ethernet_sgmii_phy.h"
    #include "phy.h"

    void reset_sgmii_phy(void)
    {
        MSS_MAC_write_phy_reg(&g_mac0, SGMII_PHY_ADDR, MII_BMCR, 0x8000);
    }
  @endcode
 */
void 
MSS_MAC_write_phy_reg
(
    const mss_mac_instance_t *this_mac,
    uint8_t phyaddr,
    uint8_t regaddr,
    uint16_t regval
);

/***************************************************************************//**
  The _MSS_MAC_phy_reset_ function provides a mechanism for resetting the PHY
  device attached to a given MSS Ethernet MAC peripheral. Both soft and hard 
  reset signals can be controlled via this function.

  This function is used by the Ethernet PHY drivers provided alongside the
  Ethernet MAC driver. You normally only need to use this function if writing
  your own Ethernet PHY driver.

  This function always accesses the pMAC structure data as the same PHY is
  shared by both eMAC and pMAC.

  __Note:__ The use of this function introduces a dependency on the MSS_GPIO
  driver into the application. If this is not desirable the function can be
  disabled by ensuring the _MSS_MAC_PHY_HW_RESET_ and _MSS_MAC_PHY_HW_SRESET_
  are undefined.
  
  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param reset_type
    This parameter selects between hard and soft reset

  @param reset_state
    This parameter is the state to set the reset pin to. For active low resets
    this should be set to false to reset the PHY and true to release the PHY
    from reset and the opposite for active high resets. It is the responsibility
    of the PHY driver to select the correct values for the hardware involved.

  @return
    This function does not return a value.

  Example:
  @code
    #include "mss_ethernet_mac.h"

    void reset_phy(void)
    {
        MSS_MAC_phy_reset(&g_mac0, MSS_MAC_SOFT_RESET, false);
        // delay for a bit
        MSS_MAC_phy_reset(&g_mac0, MSS_MAC_SOFT_RESET, true);
    }
  @endcode
 */
void
MSS_MAC_phy_reset
(
    const mss_mac_instance_t *this_mac,
    mss_mac_phy_reset_t reset_type,
    bool     reset_state
);

/***************************************************************************//**
  The _MSS_MAC_init_TSU()_ function configures the Time Stamp Unit (TSU) for
  operation and sets the initial count value.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param tsu_cfg
    this parameter is a pointer to an _mss_mac_tsu_config_t_ structure which the
    application uses to set the initial timer value and the nanosecond and
    sub-nanosecond increment values to ensure the counter increments at the
    desired rate.

  @return
    This function does not return a value.

  Example:
  This example illustrates configuring the TSU for operation with a 125MHz clock
  (8nS increment). It also configures the time stamping operation for the
  Ethernet MAC to collect the times tamps for received and transmitted PTP
  packets without modifying outgoing PTP packets. The ingress and egress unicast
  PTP addresses to recognize are set to 10.1.1.2 and 10.1.1.3 respectively and
  at a later stage the current TSU count is retrieved and displayed.
  @code
    #include "mss_ethernet_mac.h"

    void init_ts(void)
    {
        mss_mac_tsu_config_t tsu_cfg;

        tsu_cfg.nanoseconds = 0;
        tsu_cfg.secs_lsb    = 0;
        tsu_cfg.secs_msb    = 0;
        tsu_cfg.ns_inc      = 8;
        tsu_cfg.sub_ns_inc  = 0;

        MSS_MAC_init_TSU(&g_mac0, &tsu_cfg);
        MSS_MAC_set_TSU_rx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_tx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_oss_mode(&g_mac0, MSS_MAC_OSS_MODE_DISABLED);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_RX,
                                     0x0A010102);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_TX,
                                     0x0A010103);

        ...

        MSS_MAC_read_TSU(&g_mac0, &tsu_val);
        printf("TSU = %u, %u, %u\n\r", tsu_val.secs_msb, tsu_val.secs_lsb,
               tsu_val.nanoseconds);
    }
  @endcode
 */
void
MSS_MAC_init_TSU
(
    const mss_mac_instance_t *this_mac,
    const mss_mac_tsu_config_t *tsu_cfg
);

/***************************************************************************//**
  The _MSS_MAC_MSS_MAC_read_TSU()_ function reads the current timer value from
  the TSU. A strict order of reading and a check for nanoseconds overflow is
  used to ensure the time is read correctly.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param tsu_time
    This parameter is a pointer to an _mss_mac_tsu_time_t_ structure which the
    driver populates with the current counter value from the TSU.

  @return
    This function does not return a value.

  Example:
  This example illustrates configuring the TSU for operation with a 125MHz clock
  (8nS increment). It also configures the time stamping operation for the
  Ethernet MAC to collect the time stamps for received and transmitted PTP
  packets without modifying outgoing PTP packets. The ingress and egress unicast
  PTP addresses to recognize are set to 10.1.1.2 and 10.1.1.3 respectively and
  at a later stage the current TSU count is retrieved and displayed.
  @code
    #include "mss_ethernet_mac.h"

    void init_ts(void)
    {
        mss_mac_tsu_config_t tsu_cfg;

        tsu_cfg.nanoseconds = 0;
        tsu_cfg.secs_lsb    = 0;
        tsu_cfg.secs_msb    = 0;
        tsu_cfg.ns_inc      = 8;
        tsu_cfg.sub_ns_inc  = 0;

        MSS_MAC_init_TSU(&g_mac0, &tsu_cfg);
        MSS_MAC_set_TSU_rx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_tx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_oss_mode(&g_mac0, MSS_MAC_OSS_MODE_DISABLED);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_RX,
                                     0x0A010102);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_TX,
                                     0x0A010103);

        ...

        MSS_MAC_read_TSU(&g_mac0, &tsu_val);
        printf("TSU = %u, %u, %u\n\r", tsu_val.secs_msb, tsu_val.secs_lsb,
               tsu_val.nanoseconds);
    }
  @endcode
 */
void
MSS_MAC_read_TSU
(
    const mss_mac_instance_t *this_mac,
    mss_mac_tsu_time_t *tsu_time
);

/***************************************************************************//**
  The _MSS_MAC_set_TSU_rx_mode()_ function configures time stamp recording for
  received packets. This allows recording the TSU value for received packets in
  the DMA descriptor for different types of packet.
  
  The receive packet callback function can retrieve the packet time stamp from
  the DMA descriptor for processing by the application.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param tsu_mode
    This parameter selects which types of packet have their time stamp recorded.
    The available options allow for:

    - _MSS_MAC_TSU_MODE_DISABLED_  - none
    - _MSS_MAC_TSU_MODE_PTP_EVENT_ - PTP event packets
    - _MSS_MAC_TSU_MODE_PTP_ALL_   - all PTP packets
    - _MSS_MAC_TSU_MODE_ALL_       - all packets.

  @return
    This function does not return a value.

  Example:
  This example illustrates configuring the TSU for operation with a 125MHz clock
  (8nS increment). It also configures the time stamping operation for the
  Ethernet MAC to collect the time stamps for received and transmitted PTP
  packets without modifying outgoing PTP packets. The ingress and egress unicast
  PTP addresses to recognize are set to 10.1.1.2 and 10.1.1.3 respectively and
  at a later stage the current TSU count is retrieved and displayed.
  @code
    #include "mss_ethernet_mac.h"

    void init_ts(void)
    {
        mss_mac_tsu_config_t tsu_cfg;

        tsu_cfg.nanoseconds = 0;
        tsu_cfg.secs_lsb    = 0;
        tsu_cfg.secs_msb    = 0;
        tsu_cfg.ns_inc      = 8;
        tsu_cfg.sub_ns_inc  = 0;

        MSS_MAC_init_TSU(&g_mac0, &tsu_cfg);
        MSS_MAC_set_TSU_rx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_tx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_oss_mode(&g_mac0, MSS_MAC_OSS_MODE_DISABLED);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_RX,
                                     0x0A010102);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_TX,
                                     0x0A010103);

        ...

        MSS_MAC_read_TSU(&g_mac0, &tsu_val);
        printf("TSU = %u, %u, %u\n\r", tsu_val.secs_msb, tsu_val.secs_lsb,
               tsu_val.nanoseconds);
    }
  @endcode
 */
void
MSS_MAC_set_TSU_rx_mode
(
    const mss_mac_instance_t *this_mac,
    mss_mac_tsu_mode_t tsu_mode
);

/***************************************************************************//**
  The _MSS_MAC_set_TSU_tx_mode()_ function configures time stamp recording for
  transmitted packets. This allows recording the TSU value for transmitted
  packets in the DMA descriptor for different types of packet.
  
  The transmit packet callback function can retrieve the packet time stamp from
  the DMA descriptor for processing by the application.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param tsu_mode
    This parameter selects which types of packet have their time stamp recorded.
    The available options allow for:

    - _MSS_MAC_TSU_MODE_DISABLED_  - none
    - _MSS_MAC_TSU_MODE_PTP_EVENT_ - PTP event packets
    - _MSS_MAC_TSU_MODE_PTP_ALL_   - all PTP packets
    - _MSS_MAC_TSU_MODE_ALL_       - all packets.

  @return
    This function does not return a value.

  Example:
  This example illustrates configuring the TSU for operation with a 125MHz clock
  (8nS increment). It also configures the time stamping operation for the
  Ethernet MAC to collect the time stamps for received and transmitted PTP
  packets without modifying outgoing PTP packets. The ingress and egress unicast
  PTP addresses to recognize are set to 10.1.1.2 and 10.1.1.3 respectively and
  at a later stage the current TSU count is retrieved and displayed.
  @code
    #include "mss_ethernet_mac.h"

    void init_ts(void)
    {
        mss_mac_tsu_config_t tsu_cfg;

        tsu_cfg.nanoseconds = 0;
        tsu_cfg.secs_lsb    = 0;
        tsu_cfg.secs_msb    = 0;
        tsu_cfg.ns_inc      = 8;
        tsu_cfg.sub_ns_inc  = 0;

        MSS_MAC_init_TSU(&g_mac0, &tsu_cfg);
        MSS_MAC_set_TSU_rx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_tx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_oss_mode(&g_mac0, MSS_MAC_OSS_MODE_DISABLED);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_RX,
                                     0x0A010102);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_TX,
                                     0x0A010103);

        ...

        MSS_MAC_read_TSU(&g_mac0, &tsu_val);
        printf("TSU = %u, %u, %u\n\r", tsu_val.secs_msb, tsu_val.secs_lsb,
               tsu_val.nanoseconds);
    }
  @endcode
 */
void
MSS_MAC_set_TSU_tx_mode
(
    const mss_mac_instance_t *this_mac,
    mss_mac_tsu_mode_t tsu_mode
);

/***************************************************************************//**
  The _MSS_MAC_get_TSU_rx_mode()_ function allows the application determine the
  current TSU receive time stamping mode.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns the current TSU receive time stamping mode setting.
 */
mss_mac_tsu_mode_t
MSS_MAC_get_TSU_rx_mode
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_get_TSU_tx_mode()_ function allows the application determine the
  current TSU tranmit time stamping mode.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns the current TSU transmit time stamping mode setting.

 */
mss_mac_tsu_mode_t
MSS_MAC_get_TSU_tx_mode
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_TSU_oss_mode()_ function configures one step sync (OSS)
  operation for the Ethernet MAC. The three options allow for disabling OSS,
  enabling OSS time stamp replacement mode and enabling OSS correction field
  adjustment mode.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param oss_mode
    This parameter selects which OSS mode to use. Valid values are:
    
    - _MSS_MAC_OSS_MODE_DISABLED_
    - _MSS_MAC_OSS_MODE_REPLACE_
    - _MSS_MAC_OSS_MODE_ADJUST_

  @return
    This function does not return a value.

  Example:
  @code
    #include "mss_ethernet_mac.h"

    void init_ts(void)
    {
        mss_mac_tsu_config_t tsu_cfg;

        tsu_cfg.nanoseconds = 0;
        tsu_cfg.secs_lsb    = 0;
        tsu_cfg.secs_msb    = 0;
        tsu_cfg.ns_inc      = 8;
        tsu_cfg.sub_ns_inc  = 0;

        MSS_MAC_init_TSU(&g_mac0, &tsu_cfg);
        MSS_MAC_set_TSU_rx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_tx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_oss_mode(&g_mac0, MSS_MAC_OSS_MODE_DISABLED);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_RX,
                                     0x0A010102);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_TX,
                                     0x0A010103);

        ...

        MSS_MAC_read_TSU(&g_mac0, &tsu_val);
        printf("TSU = %u, %u, %u\n\r", tsu_val.secs_msb, tsu_val.secs_lsb,
               tsu_val.nanoseconds);
    }
  @endcode
 */
void
MSS_MAC_set_TSU_oss_mode
(
    const mss_mac_instance_t *this_mac,
    mss_mac_oss_mode_t oss_mode
);

/***************************************************************************//**
  The _MSS_MAC_get_TSU_oss_mode()_ function returns the currently configured one
  step sync (OSS) mode for the MAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns an _mss_mac_oss_mode_t_ value which indicates the
    current OSS mode in operation.
 */
mss_mac_oss_mode_t
MSS_MAC_get_TSU_oss_mode
(
    const mss_mac_instance_t *this_mac
);


/***************************************************************************//**
  The _MSS_MAC_set_TSU_unicast_addr()_ function configures IP addresses that the
  MAC will use to identify unicast PTP frames. The receive IPv4 address and
  transmit IPv4 address can be set separately.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param select
    This parameter of type _mss_mac_tsu_addr_t_, selects whether the RX or TX
    address is being set.

  @param ip_address
    This unsigned 32 bit int holds the IP address with the 8 msb holding the
    leftmost octet i.e. 10.2.2.1 is represented as 0x0A020201.

  @return
    This function does not return a value.

  Example:
  @code
    #include "mss_ethernet_mac.h"

    void init_ts(void)
    {
        mss_mac_tsu_config_t tsu_cfg;

        tsu_cfg.nanoseconds = 0;
        tsu_cfg.secs_lsb    = 0;
        tsu_cfg.secs_msb    = 0;
        tsu_cfg.ns_inc      = 8;
        tsu_cfg.sub_ns_inc  = 0;

        MSS_MAC_init_TSU(&g_mac0, &tsu_cfg);
        MSS_MAC_set_TSU_rx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_tx_mode(&g_mac0, MSS_MAC_TSU_MODE_PTP_EVENT);
        MSS_MAC_set_TSU_oss_mode(&g_mac0, MSS_MAC_OSS_MODE_DISABLED);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_RX,
                                     0x0A010102);
        MSS_MAC_set_TSU_unicast_addr(&g_mac0, MSS_MAC_TSU_UNICAST_TX,
                                     0x0A010103);

        ...

        MSS_MAC_read_TSU(&g_mac0, &tsu_val);
        printf("TSU = %u, %u, %u\n\r", tsu_val.secs_msb, tsu_val.secs_lsb,
               tsu_val.nanoseconds);
    }
  @endcode
 */
void
MSS_MAC_set_TSU_unicast_addr
(
    const mss_mac_instance_t *this_mac,
    mss_mac_tsu_addr_t select,
    uint32_t ip_address
);

/***************************************************************************//**
  The _MSS_MAC_get_TSU_unicast_addr()_ function retrieves the IP addresses that
  the Ethernet MAC uses to identify unicast PTP frames.
  
  The receive IPv4 address and transmit IPv4 address can be retrieved
  separately.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param select
    This parameter of type _mss_mac_tsu_addr_t_, selects whether the RX or TX
    address is being retrieved.

  @return
    This function returns an unsigned 32 bit int which holds the IP address with
    the 8 msb holding the leftmost octet i.e. 10.2.2.1 is represented as
    0x0A020201.
 */
uint32_t
MSS_MAC_get_TSU_unicast_addr
(
    const mss_mac_instance_t *this_mac,
    mss_mac_tsu_addr_t select
);

/***************************************************************************//**
  The _MSS_MAC_set_VLAN_only_mode()_ function allows the application enable or
  disable VLAN only mode. When VALN only mode is enabled, non VLAN tagged
  packets will be discarded.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param enable
    This parameter of type _bool_, is set to true to enable VLAN only mode and
    false to disable it.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_VLAN_only_mode
(
    const mss_mac_instance_t *this_mac,
    bool enable
);

/***************************************************************************//**
  The _MSS_MAC_get_VLAN_only_mode()_ function retrieves the current VLAN only 
  mode status. When VALN only mode is enabled, non VLAN tagged packets will be
  discarded.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns a value of type _bool_, which is true to indicate VLAN
    only mode is currently enabled and false otherwise.
 */
bool
MSS_MAC_get_VLAN_only_mode
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_stacked_VLAN()_ function sets the stacked VLAN tag register
  in the MAC. This can be used to enable and disable stacked VLAN operation.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param tag
    This parameter of type _uint16_t_, holds the stacked VLAN tag to write to
    the Ethernet MAC. A value <= _GEM_VLAN_ETHERTYPE_MIN_ disables stacked VLAN
    mode and sets the stacked VLAN tag to 0.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_stacked_VLAN
(
    const mss_mac_instance_t *this_mac,
    uint16_t tag
);

/***************************************************************************//**
  The _MSS_MAC_get_stacked_VLAN()_ function retrieves the current stacked VLAN
  tag register from the Ethernet MAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns a value of type _uint16_t_, which is the current
    stacked VLAN tag from the Ethernet MAC. A value of 0 indicates stacked VLAN
    mode is disabled.
 */
uint16_t
MSS_MAC_get_stacked_VLAN
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_hash()_ function sets the 64 bit addressing hash field in the
  Ethernet MAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param hash
    This parameter of type _uint64_t_, holds the hash bitmap for the MAC which
    is used to enable matching against blocks of addresses based on a 6 bit
    hashing function.
    
    As a short cut, setting a value of 0 also disables hash address matching.

  @return
    This function does not return a value.

  Example:
  This example sets the hash bit for the multicast MAC address 01:80:C2:00:00:0E
  and also configures the Ethernet MAC to only use hashing to match multicast
  addresses. The _calc_gem_hash_index()_ function calculates the bit index in
  the 64 bit hash value associated with a given MAC address.
  @code
    uint32_t calc_gem_hash_index(uint8_t *p_address)
    {
        uint64_t address;
        uint32_t index;
        uint32_t count;

        p_address += 5;
        address = 0;
        for(count = 0; count != 6; count++)
        {
            address <<= 8;
            address |= (uint64_t)*p_address;
            p_address--;
        }

        index = (int32_t)address & 0x0000003Fl;
        for(count = 0; count != 7; count++)
        {
            address >>= 6;
            index ^= (uint32_t)(address & 0x0000003Fl);
        }

        return(index);
    }

    void set_gem_hash(void)
    {
        uint8_t mac_address[6] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e};
        int32_t index;
        uint64_t temp_hash;
        uint64_t current_hash;

        index = calc_gem_hash_index(mac_address);
        temp_hash = (uint64_t)(1ll << index);
        current_hash = MSS_MAC_get_hash(g_mac0);
        current_hash = current_hash | temp_hash;
        MSS_MAC_set_hash_mode(g_mac0, MSS_MAC_HASH_MULTICAST);
        MSS_MAC_set_hash(g_mac0, current_hash);
    }
    @endcode
 */
void
MSS_MAC_set_hash
(
    const mss_mac_instance_t *this_mac,
    uint64_t hash
);

/***************************************************************************//**
  The _MSS_MAC_get_hash()_ function retrieves the current 64 bit addressing hash
  field from the Ethernet MAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns a value of type _uint64_t_, representing the hash
    bitmap for the Ethernet MAC.

  Example:
  This example sets the hash bit for the multicast MAC address 01:80:C2:00:00:0E
  and also configures the Ethernet MAC to only use hashing to match multicast
  addresses. The _calc_gem_hash_index()_ function calculates the bit index in
  the 64 bit hash value associated with a given MAC address.
  @code
    uint32_t calc_gem_hash_index(uint8_t *p_address)
    {
        uint64_t address;
        uint32_t index;
        uint32_t count;

        p_address += 5;
        address = 0;
        for(count = 0; count != 6; count++)
        {
            address <<= 8;
            address |= (uint64_t)*p_address;
            p_address--;
        }

        index = (int32_t)address & 0x0000003Fl;
        for(count = 0; count != 7; count++)
        {
            address >>= 6;
            index ^= (uint32_t)(address & 0x0000003Fl);
        }

        return(index);
    }

    void set_gem_hash(void)
    {
        uint8_t mac_address[6] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e};
        int32_t index;
        uint64_t temp_hash;
        uint64_t current_hash;

        index = calc_gem_hash_index(mac_address);
        temp_hash = (uint64_t)(1ll << index);
        current_hash = MSS_MAC_get_hash(g_mac0);
        current_hash = current_hash | temp_hash;
        MSS_MAC_set_hash_mode(g_mac0, MSS_MAC_HASH_MULTICAST);
        MSS_MAC_set_hash(g_mac0, current_hash);
    }
    @endcode
 */
uint64_t
MSS_MAC_get_hash
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_hash_mode()_ function sets the operational mode for the hash
  based address matching.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param mode
    This parameter of type _mss_mac_hash_mode_t_, selects the mode of operation
    of the hash based address filter. The options allow for disabling hash based
    filtering, matching unicast packets only, matching multicast packets only
    and matching both packet types.

  @return
    This function does not return a value.

  Example:
  This example sets the hash bit for the multicast MAC address 01:80:C2:00:00:0E
  and also configures the Ethernet MAC to only use hashing to match multicast
  addresses. The _calc_gem_hash_index()_ function calculates the bit index in 
  the 64 bit hash value associated with a given MAC address.
  @code
    uint32_t calc_gem_hash_index(uint8_t *p_address)
    {
        uint64_t address;
        uint32_t index;
        uint32_t count;

        p_address += 5;
        address = 0;
        for(count = 0; count != 6; count++)
        {
            address <<= 8;
            address |= (uint64_t)*p_address;
            p_address--;
        }

        index = (int32_t)address & 0x0000003Fl;
        for(count = 0; count != 7; count++)
        {
            address >>= 6;
            index ^= (uint32_t)(address & 0x0000003Fl);
        }

        return(index);
    }

    void set_gem_hash(void)
    {
        uint8_t mac_address[6] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e};
        int32_t index;
        uint64_t temp_hash;
        uint64_t current_hash;

        index = calc_gem_hash_index(mac_address);
        temp_hash = (uint64_t)(1ll << index);
        current_hash = MSS_MAC_get_hash(g_mac0);
        current_hash = current_hash | temp_hash;
        MSS_MAC_set_hash_mode(g_mac0, MSS_MAC_HASH_MULTICAST);
        MSS_MAC_set_hash(g_mac0, current_hash);
    }
    @endcode
 */
void
MSS_MAC_set_hash_mode
(
    const mss_mac_instance_t *this_mac,
    mss_mac_hash_mode_t mode
);

/***************************************************************************//**
  The _MSS_MAC_get_hash_mode()_ function retrieves the current operational mode
  for the hash based address matching.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns a value of type _mss_mac_hash_mode_t_, which indicates
    the mode of operation of the hash based address filter.
 */
mss_mac_hash_mode_t
MSS_MAC_get_hash_mode
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_type_filter()_ function is used to configure the specific 
  type filters in the Ethernet MAC. These filters allow selective reception of
  packets based on matching the type ID/length field in the packet.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter
    This parameter of type _uint16_t_, selects which filter to configure. The
    valid range is 1 to 4 to match the numbering in the GEM documentation.

  @param value
    This parameter of type _uint16_t_, is the value to match against. Typical
    values for matching purposes will be greater than 0x600 (1536) as the field
    is used for packet length in some circumstances and as the Ethertype value
    in others. 
    
    Setting _value_ to 0 indicates disabling the filter.

  @return
    This function does not return a value.

  Example:
    This example enables Specific Type filters 1 and 2 for IPv4 and IPv6
    packets respectively.

  @code
    #include "mss_ethernet_mac.h"

    void init_st_filters(void)
    {
        MSS_MAC_set_type_filter(&g_mac0, 1, 0x0800);
        MSS_MAC_set_type_filter(&g_mac0, 2, 0x86DD);
        MSS_MAC_set_type_filter(&g_mac0, 3, 0x0000);
        MSS_MAC_set_type_filter(&g_mac0, 4, 0x0000);
    }
  @endcode
 */
void
MSS_MAC_set_type_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter,
    uint16_t value
);

/***************************************************************************//**
  The _MSS_MAC_get_type_filter()_ function is used to retrieve the current
  settings of the specific type filters in the Ethernet MAC. These filters allow
  selective reception of packets based on matching the type ID/length field in
  the packet.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter
    This parameter of type _uint16_t_, selects which filter to read. The valid
    range is 1 to 4 to match the numbering in the GEM documentation.

  @return
    This function returns a value of type _uint16_t_, which is the current
    filter match value. 
    
    A value of 0 indicates the filter is disabled.
 */
uint16_t
MSS_MAC_get_type_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter
);

/***************************************************************************//**
  The _MSS_MAC_set_sa_filter()_ function is used to configure the specific
  address filters in the Ethernet MAC. These filters allow selective reception
  of packets based on matching the destination or source MAC address field in
  the packet.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter
    This parameter of type _uint16_t_, selects which filter to configure. The
    valid range is 2 to 4 to match the numbering in the GEM documentation.
    
    __Note:__ Filter 1 is reserved for the MAC address of the device and cannot
    be modified using this function.

  @param control
    This parameter of type _uint16_t_, is used to select between source and
    destination matching and selective masking of individual bytes, see the
    _MSS_MAC_SA_FILTER_*_ macros for allowed values. In addition, the
    _MSS_MAC_SA_FILTER_DISABLE_ value is used to disable a filter.

  @param mac_addr
    This parameter of type pointer to _uint8_t_, points to the 6 byte MAC
    address to be used for this filter operation.

  @return
    This function does not return a value.

  Example:
    This example assumes MAC0 is connected on the same network as MAC1 and
    configures source address filters to detect traffic from MAC0 and MAC1 on
    the opposing MAC. This uses filter number two on each Ethernet MAC.

  @code
    #include "mss_ethernet_mac.h"

    void init_sa_filters(void)
    {
        MSS_MAC_set_sa_filter(&g_mac0, 2, MSS_MAC_SA_FILTER_SOURCE,
                              g_mac1.mac_addr);
        MSS_MAC_set_sa_filter(&g_mac1, 2, MSS_MAC_SA_FILTER_SOURCE,
                              g_mac0.mac_addr);
    }
  @endcode
 */
void
MSS_MAC_set_sa_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter,
    uint16_t control,
    const uint8_t *mac_addr
);

/***************************************************************************//**
  The _MSS_MAC_get_sa_filter()_ function retrieves the current specific address
  filter settings from the Ethernet MAC. These filters allow selective reception
  of packets based on matching the destination or source MAC address field in
  the packet.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter
    This parameter of type _uint16_t_, selects which filter to retrieve from.
    The valid range is 2 to 4 to match the numbering in the GEM documentation.
    
    __Note:__ Filter 1 is reserved for the MAC address of the device and cannot
    be read using this function.

  @param mac_addr
    This parameter of type pointer to _uint8_t_, points to a storage location
    where the 6 byte MAC address from the filter will be stored.

  @return
    This function returns a value of type _uint16_t_, which has the same format
    as the control parameter for the _MSS_MAC_set_sa_filter()_ function.


  Example:
    This example reads the filter settings for filter 2 on the pMAC of GEM 0.

  @code
    #include "mss_ethernet_mac.h"

    void read_sa_filter_2(void)
    {
        uint16_t control;
        uint8_t  mac_addr[6];

        control = MSS_MAC_get_sa_filter(&g_mac0, 2, mac_addr);
    }
  @endcode
 */
uint16_t
MSS_MAC_get_sa_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter,
    uint8_t *mac_addr
);

/***************************************************************************//**
  The _MSS_MAC_set_type_1_filter()_ function is used to configure the Type 1
  filters in the Ethernet MAC. These filters allow selective routing of packets
  to specific queues based on combinations of UDP port number and/or
  differentiated service/traffic class values. Packets can also be flagged for
  dropping based on the same criteria.
  
  Packets that do not match a Type 1 or Type 2 filter are automatically routed
  to queue 0.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter_no
    This parameter of type _uint32_t_, selects which filter to configure. The
    valid range is 0 to 3.

  @param filter
    This parameter of type pointer to _mss_mac_type_1_filter_t_, points to the
    filter configuration structure which defines the filter setup required.

  @return
    This function does not return a value.

  Example:
    This example configures 3 type 1 filters which route UDP port 320 packets to
    queue 1, packets with a DS or TC value of 15 to queue 2 and drops packets
    with a UDP port of 1534. The example also checks to see if filter 3 has
    already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t1_filters(void)
    {
        mss_mac_type_1_filter_t filter1;

        MSS_MAC_get_type_1_filter(&g_mac0, 3, &filter1);
        if(0 == filter1.udp_port)
        {
            memset(&filter1, 0, sizeof(filter1));
            filter1.udp_port = 320;
            filter1.udp_port_enable = 1;
            filter1.queue_no = 1;

            MSS_MAC_set_type_1_filter(&g_mac0, 3, &filter1);

            memset(&filter1, 0, sizeof(filter1));
            filter1.dstc = 0x0F;
            filter1.dstc_enable = 1;
            filter1.queue_no = 2;

            MSS_MAC_set_type_1_filter(&g_mac0, 2, &filter1);

            memset(&filter1, 0, sizeof(filter1));
            filter1.udp_port = 1534;
            filter1.udp_port_enable = 1;
            filter1.drop_on_match = 1;
            filter1.queue_no = 3;

            MSS_MAC_set_type_1_filter(&g_mac0, 1, &filter1);
        }
    }
  @endcode
 */
void
MSS_MAC_set_type_1_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter_no,
    const mss_mac_type_1_filter_t *filter
);

/***************************************************************************//**
  The _MSS_MAC_get_type_1_filter()_ function is used to retrieve the current
  Type 1 filter configurations from the Ethernet MAC. These filters allow
  selective routing of packets to specific queues based on combinations of UDP
  port number and/or differentiated service/traffic class values. Packets can
  also be flagged for dropping based on the same criteria.

  Packets that do not match a Type 1 or Type 2 filter are automatically routed
  to queue 0.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter_no
    This parameter of type _uint32_t_, selects which filter to retrieve. The
    valid range is 0 to 3.

  @param filter
    This parameter of type pointer to _mss_mac_type_1_filter_t_, points to the
    filter configuration structure which receives the setup information for
    the selected filter.

  @return
    This function does not return a value.

  Example:
    This example configures 3 type 1 filters which route UDP port 320 packets to
    queue 1, packets with a DS or TC value of 15 to queue 2 and drops packets
    with a UDP port of 1534. The example also checks to see if filter 3 has
    already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t1_filters(void)
    {
        mss_mac_type_1_filter_t filter1;

        MSS_MAC_get_type_1_filter(&g_mac0, 3, &filter1);
        if(0 == filter1.udp_port)
        {
            memset(&filter1, 0, sizeof(filter1));
            filter1.udp_port = 320;
            filter1.udp_port_enable = 1;
            filter1.queue_no = 1;

            MSS_MAC_set_type_1_filter(&g_mac0, 3, &filter1);

            memset(&filter1, 0, sizeof(filter1));
            filter1.dstc = 0x0F;
            filter1.dstc_enable = 1;
            filter1.queue_no = 2;

            MSS_MAC_set_type_1_filter(&g_mac0, 2, &filter1);

            memset(&filter1, 0, sizeof(filter1));
            filter1.udp_port = 1534;
            filter1.udp_port_enable = 1;
            filter1.drop_on_match = 1;
            filter1.queue_no = 3;

            MSS_MAC_set_type_1_filter(&g_mac0, 1, &filter1);
        }
    }
  @endcode
 */
void
MSS_MAC_get_type_1_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter_no,
    mss_mac_type_1_filter_t *filter
);

/***************************************************************************//**
  The _MSS_MAC_set_type_2_filter()_ function is used to configure the Type 2
  filters in the Ethernet MAC. These filters allow selective routing of packets
  to specific queues based on combinations of Ethertype field values, general
  data patterns and VLAN priority values. Packets can also be flagged for
  dropping based on the same criteria.

  For consistent operation, the application should always use the
  _MSS_MAC_set_type_2_compare()_ and _MSS_MAC_set_type_2_ethertype()_ functions
  to set the filter match values before calling _MSS_MAC_set_type_2_filter()_.

  Packets that do not match a Type 1 or Type 2 filter are automatically routed
  to queue 0.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter_no
    This parameter of type _uint32_t_, selects which filter to configure. The
    valid range is 0 to 3 for pMACs and 0 to 1 for eMACs.

  @param filter
    This parameter of type pointer to _mss_mac_type_2_filter_t_, points to the
    filter configuration structure which defines the filter setup required.

  @return
    This function does not return a value.

  Example:
    This example configures 2 filters which route IP packets with 0x9E, 0x48,
    0x05, 0x0F at offset 0x48 to queue 3 and ARP packets with 12 x 0xFF at
    offset 0x30 to queue 2. The example also checks to see if Ethertype block 0
    has already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t2_filters(void)
    {
        mss_mac_type_2_filter_t filter;
        mss_mac_type_2_compare_t compare;

        if(0 == MSS_MAC_get_type_2_ethertype(&g_mac0, 0))
        {
            MSS_MAC_set_type_2_ethertype(&g_mac0, 0, 0x0800);

            compare.disable_mask = 1;
            compare.data = 0x0F05489EUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x48;
            MSS_MAC_set_type_2_compare(&g_mac0, 0, &compare);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 0;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 0;
            filter.queue_no         = 3;
            MSS_MAC_set_type_2_filter(&g_mac0, 0, &filter);

            compare.disable_mask = 1;
            compare.data = 0xFFFFFFFFUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x30;
            MSS_MAC_set_type_2_compare(&g_mac0, 1, &compare);

            compare.offset_value = 0x34;
            MSS_MAC_set_type_2_compare(&g_mac0, 2, &compare);

            compare.offset_value = 0x38;
            MSS_MAC_set_type_2_compare(&g_mac0, 3, &compare);

            MSS_MAC_set_type_2_ethertype(&g_mac0, 1, 0x0806);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 1;
            filter.compare_b_enable = 1;
            filter.compare_b_index  = 2;
            filter.compare_c_enable = 1;
            filter.compare_c_index  = 3;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 1;
            filter.queue_no         = 2;
            MSS_MAC_set_type_2_filter(&g_mac0, 1, &filter);
        }
    }
  @endcode
 */
void
MSS_MAC_set_type_2_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter_no,
    const mss_mac_type_2_filter_t *filter
);

/***************************************************************************//**
  The _MSS_MAC_get_type_2_filter()_ function is used to retrieve the current
  Type 2 filter configuration from the Ethernet MAC. These filters allow
  selective routing of packets to specific queues based on combinations of
  Ethertype field values, general data patterns and VLAN priority values.
  Packets can also be flagged for dropping based on the same criteria.

  Packets that do not match a Type 1 or Type 2 filter are automatically routed
  to queue 0.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param filter_no
    This parameter of type _uint32_t_, selects which filter to retrieve. The
    valid range is 0 to 3 for pMACs and 0 to 1 for eMACs.

  @param filter
    This parameter of type pointer to _mss_mac_type_2_filter_t_, points to the
    filter configuration structure which will receive a copy of the current
    filter setup.

  @return
    This function does not return a value.

  Example:
    This example configures 2 filters which route IP packets with 0x9E, 0x48,
    0x05, 0x0F at offset 0x48 to queue 3 and ARP packets with 12 x 0xFF at
    offset 0x30 to queue 2. The example also checks to see if Ethertype block 0
    has already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t2_filters(void)
    {
        mss_mac_type_2_filter_t filter;
        mss_mac_type_2_compare_t compare;

        if(0 == MSS_MAC_get_type_2_ethertype(&g_mac0, 0))
        {
            MSS_MAC_set_type_2_ethertype(&g_mac0, 0, 0x0800);

            compare.disable_mask = 1;
            compare.data = 0x0F05489EUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x48;
            MSS_MAC_set_type_2_compare(&g_mac0, 0, &compare);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 0;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 0;
            filter.queue_no         = 3;
            MSS_MAC_set_type_2_filter(&g_mac0, 0, &filter);

            compare.disable_mask = 1;
            compare.data = 0xFFFFFFFFUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x30;
            MSS_MAC_set_type_2_compare(&g_mac0, 1, &compare);

            compare.offset_value = 0x34;
            MSS_MAC_set_type_2_compare(&g_mac0, 2, &compare);

            compare.offset_value = 0x38;
            MSS_MAC_set_type_2_compare(&g_mac0, 3, &compare);

            MSS_MAC_set_type_2_ethertype(&g_mac0, 1, 0x0806);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 1;
            filter.compare_b_enable = 1;
            filter.compare_b_index  = 2;
            filter.compare_c_enable = 1;
            filter.compare_c_index  = 3;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 1;
            filter.queue_no         = 2;
            MSS_MAC_set_type_2_filter(&g_mac0, 1, &filter);
        }
    }
  @endcode
 */
void
MSS_MAC_get_type_2_filter
(
    const mss_mac_instance_t *this_mac,
    uint32_t filter_no,
    mss_mac_type_2_filter_t *filter
 );

/***************************************************************************//**
  The _MSS_MAC_set_type_2_ethertype()_ function is used to configure the Type 2
  Ethertype matching blocks in the Ethernet MAC. These are used by Type 2
  filters to allow selection of packet types.
  
  For consistent operation, the application should always use the
  _MSS_MAC_set_type_2_compare()_ and _MSS_MAC_set_type_2_ethertype()_ functions
  to set the filter match values before calling _MSS_MAC_set_type_2_filter()_.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param ethertype_no
    This parameter of type _uint32_t_, selects which Ethertype block to
    configure. The valid range is 0 to 3 for pMACs, eMACs do not support
    Ethertype filters.

  @param ethertype
    This parameter of type _uint16_t_, is the Ethertype value to match against.
    A value of 0 indicates the Ethertype matching block is not configured.

  @return
    This function does not return a value.

  Example:
    This example configures 2 filters which route IP packets with 0x9E, 0x48,
    0x05, 0x0F at offset 0x48 to queue 3 and ARP packets with 12 x 0xFF at
    offset 0x30 to queue 2. The example also checks to see if Ethertype block 0
    has already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t2_filters(void)
    {
        mss_mac_type_2_filter_t filter;
        mss_mac_type_2_compare_t compare;

        if(0 == MSS_MAC_get_type_2_ethertype(&g_mac0, 0))
        {
            MSS_MAC_set_type_2_ethertype(&g_mac0, 0, 0x0800);

            compare.disable_mask = 1;
            compare.data = 0x0F05489EUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x48;
            MSS_MAC_set_type_2_compare(&g_mac0, 0, &compare);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 0;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 0;
            filter.queue_no         = 3;
            MSS_MAC_set_type_2_filter(&g_mac0, 0, &filter);

            compare.disable_mask = 1;
            compare.data = 0xFFFFFFFFUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x30;
            MSS_MAC_set_type_2_compare(&g_mac0, 1, &compare);

            compare.offset_value = 0x34;
            MSS_MAC_set_type_2_compare(&g_mac0, 2, &compare);

            compare.offset_value = 0x38;
            MSS_MAC_set_type_2_compare(&g_mac0, 3, &compare);

            MSS_MAC_set_type_2_ethertype(&g_mac0, 1, 0x0806);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 1;
            filter.compare_b_enable = 1;
            filter.compare_b_index  = 2;
            filter.compare_c_enable = 1;
            filter.compare_c_index  = 3;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 1;
            filter.queue_no         = 2;
            MSS_MAC_set_type_2_filter(&g_mac0, 1, &filter);
        }
    }
  @endcode
 */
void
MSS_MAC_set_type_2_ethertype
(
    const mss_mac_instance_t *this_mac,
    uint32_t ethertype_no,
    uint16_t ethertype
);

/***************************************************************************//**
  The _MSS_MAC_get_type_2_ethertype()_ function is used to retrieve the current
  settings for Type 2 Ethertype matching blocks in the Ethernet MAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param ethertype_no
    This parameter of type _uint32_t_, selects the Ethertype block to retrieve.
    The valid range is 0 to 3 for pMACs, eMACs do not support Ethertype filters.

  @return
    This function returns a value of type _uint16_t_ which is the Ethertype
    value to match against. A value of 0 indicates the Ethertype matching block
    is not configured.

  Example:
    This example configures 2 filters which route IP packets with 0x9E, 0x48,
    0x05, 0x0F at offset 0x48 to queue 3 and ARP packets with 12 x 0xFF at
    offset 0x30 to queue 2. The example also checks to see if Ethertype block 0
    has already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t2_filters(void)
    {
        mss_mac_type_2_filter_t filter;
        mss_mac_type_2_compare_t compare;

        if(0 == MSS_MAC_get_type_2_ethertype(&g_mac0, 0))
        {
            MSS_MAC_set_type_2_ethertype(&g_mac0, 0, 0x0800);

            compare.disable_mask = 1;
            compare.data = 0x0F05489EUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x48;
            MSS_MAC_set_type_2_compare(&g_mac0, 0, &compare);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 0;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 0;
            filter.queue_no         = 3;
            MSS_MAC_set_type_2_filter(&g_mac0, 0, &filter);

            compare.disable_mask = 1;
            compare.data = 0xFFFFFFFFUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x30;
            MSS_MAC_set_type_2_compare(&g_mac0, 1, &compare);

            compare.offset_value = 0x34;
            MSS_MAC_set_type_2_compare(&g_mac0, 2, &compare);

            compare.offset_value = 0x38;
            MSS_MAC_set_type_2_compare(&g_mac0, 3, &compare);

            MSS_MAC_set_type_2_ethertype(&g_mac0, 1, 0x0806);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 1;
            filter.compare_b_enable = 1;
            filter.compare_b_index  = 2;
            filter.compare_c_enable = 1;
            filter.compare_c_index  = 3;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 1;
            filter.queue_no         = 2;
            MSS_MAC_set_type_2_filter(&g_mac0, 1, &filter);
        }
    }
  @endcode
 */
uint16_t
MSS_MAC_get_type_2_ethertype
(
    const mss_mac_instance_t *this_mac,
    uint32_t ethertype_no
);

/***************************************************************************//**
  The _MSS_MAC_set_type_2_compare()_ function is used to configure the Type 2
  data matching blocks in the Ethernet MAC. These are used by Type 2 filters to
  allow comparing up to 4 bytes of data at different locations in the packet.
  They also allow for matching against VLAN IDs. The data matching can be
  performed against 4 consecutive bytes or against up to 2 consecutive bytes
  with an associated bit mask.
  
  For consistent operation, the application should always use the
  _MSS_MAC_set_type_2_compare()_ and _MSS_MAC_set_type_2_ethertype()_ functions
  to set the filter match values before calling _MSS_MAC_set_type_2_filter()_.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param comparer_no_in
    This parameter of type _uint32_t_, selects which compare block to configure.
    The valid range is 0 to 11 for pMACs and 0 to 5 for eMACs.

  @param comparer
    This parameter of type pointer to _mss_mac_type_2_compare_t_, is the address
    of the compare block structure to use for the configuration.

  @return
    This function does not return a value.

  Example:
    This example configures 2 filters which route IP packets with 0x9E, 0x48,
    0x05, 0x0F at offset 0x48 to queue 3 and ARP packets with 12 x 0xFF at
    offset 0x30 to queue 2. The example also checks to see if Ethertype block 0
    has already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t2_filters(void)
    {
        mss_mac_type_2_filter_t filter;
        mss_mac_type_2_compare_t compare;

        if(0 == MSS_MAC_get_type_2_ethertype(&g_mac0, 0))
        {
            MSS_MAC_set_type_2_ethertype(&g_mac0, 0, 0x0800);

            compare.disable_mask = 1;
            compare.data = 0x0F05489EUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x48;
            MSS_MAC_set_type_2_compare(&g_mac0, 0, &compare);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 0;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 0;
            filter.queue_no         = 3;
            MSS_MAC_set_type_2_filter(&g_mac0, 0, &filter);

            compare.disable_mask = 1;
            compare.data = 0xFFFFFFFFUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x30;
            MSS_MAC_set_type_2_compare(&g_mac0, 1, &compare);

            compare.offset_value = 0x34;
            MSS_MAC_set_type_2_compare(&g_mac0, 2, &compare);

            compare.offset_value = 0x38;
            MSS_MAC_set_type_2_compare(&g_mac0, 3, &compare);

            MSS_MAC_set_type_2_ethertype(&g_mac0, 1, 0x0806);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 1;
            filter.compare_b_enable = 1;
            filter.compare_b_index  = 2;
            filter.compare_c_enable = 1;
            filter.compare_c_index  = 3;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 1;
            filter.queue_no         = 2;
            MSS_MAC_set_type_2_filter(&g_mac0, 1, &filter);
        }
    }
  @endcode
 */
void
MSS_MAC_set_type_2_compare
(
    const mss_mac_instance_t *this_mac,
    uint32_t comparer_no_in,
    const mss_mac_type_2_compare_t *comparer
);


/***************************************************************************//**
  The _MSS_MAC_get_type_2_compare()_ function is used to retrieve the current
  Type 2 data matching blocks configuration from the Ethernet MAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param comparer_no_in
    This parameter of type _uint32_t_, selects which compare block to retrieve.
    The valid range is 0 to 11 for pMACs and 0 to 5 for eMACs.

  @param comparer
    This parameter of type pointer to _mss_mac_type_2_compare_t_, is the address
    of the configuration structure which will receive a copy of the current
    comparer block setup.

  @return
    This function does not return a value.

  Example:
    This example configures 2 filters which route IP packets with 0x9E, 0x48,
    0x05, 0x0F at offset 0x48 to queue 3 and ARP packets with 12 x 0xFF at
    offset 0x30 to queue 2. The example also checks to see if Ethertype block 0
    has already been configured and only proceeds if it is not configured.

  @code
    #include "mss_ethernet_mac.h"

    void init_t2_filters(void)
    {
        mss_mac_type_2_filter_t filter;
        mss_mac_type_2_compare_t compare;

        if(0 == MSS_MAC_get_type_2_ethertype(&g_mac0, 0))
        {
            MSS_MAC_set_type_2_ethertype(&g_mac0, 0, 0x0800);

            compare.disable_mask = 1;
            compare.data = 0x0F05489EUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x48;
            MSS_MAC_set_type_2_compare(&g_mac0, 0, &compare);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 0;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 0;
            filter.queue_no         = 3;
            MSS_MAC_set_type_2_filter(&g_mac0, 0, &filter);

            compare.disable_mask = 1;
            compare.data = 0xFFFFFFFFUL;
            compare.compare_offset = MSS_MAC_T2_OFFSET_FRAME;
            compare.offset_value = 0x30;
            MSS_MAC_set_type_2_compare(&g_mac0, 1, &compare);

            compare.offset_value = 0x34;
            MSS_MAC_set_type_2_compare(&g_mac0, 2, &compare);

            compare.offset_value = 0x38;
            MSS_MAC_set_type_2_compare(&g_mac0, 3, &compare);

            MSS_MAC_set_type_2_ethertype(&g_mac0, 1, 0x0806);

            memset(&filter, 0, sizeof(filter));
            filter.compare_a_enable = 1;
            filter.compare_a_index  = 1;
            filter.compare_b_enable = 1;
            filter.compare_b_index  = 2;
            filter.compare_c_enable = 1;
            filter.compare_c_index  = 3;
            filter.ethertype_enable = 1;
            filter.ethertype_index  = 1;
            filter.queue_no         = 2;
            MSS_MAC_set_type_2_filter(&g_mac0, 1, &filter);
        }
    }
  @endcode
 */
void
MSS_MAC_get_type_2_compare
(
    const mss_mac_instance_t *this_mac,
    uint32_t comparer_no_in,
    mss_mac_type_2_compare_t *comparer
);

/***************************************************************************//**
  The _MSS_MAC_set_mmsl_mode()_ function is used to configure the operation of
  the MAC Merge Sublayer component of the GEM. This function is used to enable
  and disable preemption and to determine which MAC (eMAC or pMAC) is used for
  reception in non preemption modes. The function also allows control of the
  preemption verification functionality.
  
  The function only operates on the pMAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param mmsl_cfg
    This parameter of type pointer to _mss_mac_mmsl_config_t_, points to the
    configuration structure used to set the MMSL operation mode.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_mmsl_mode
(
    const mss_mac_instance_t *this_mac,
    const mss_mac_mmsl_config_t *mmsl_cfg
);

/***************************************************************************//**
  The _MSS_MAC_get_mmsl_mode()_ function is used to retrieve the current
  configuration of the MAC Merge Sublayer component of the GEM.
  
  The function only operates on the pMAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param mmsl_cfg
    This parameter of type pointer to _mss_mac_mmsl_config_t_, points to the
    configuration structure which will receive a copy of the current MMSL
    configuration.

  @return
    This function does not return a value.
 */
void
MSS_MAC_get_mmsl_mode
(
    const mss_mac_instance_t *this_mac,
    mss_mac_mmsl_config_t *mmsl_cfg
);

/***************************************************************************//**
  The _MSS_MAC_start_preemption_verify()_ function is used to initiate a link
  preemption verification operation by the MAC Merge Sublayer component of the
  GEM. The progress and completion status of the operation can be monitored by
  calling the _MSS_MAC_get_mmsl_status()_ function until the operation
  completes.
  
  The function only operates on the pMAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function does not return a value.
 */
void
MSS_MAC_start_preemption_verify
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_get_mmsl_status()_ function is used to monitor the current state
  of the MAC Merge Sublayer component of the GEM. The function returns the raw 
  32 bit value from the MMSL Status register and the definitions in 
  _mss_ethernet_mac_regs.h_ can be used to examine the status.

  The function only operates on the pMAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns a 32 bit value which is a copy of the MMSL Status
    register. The register bit defines for the MMSL Status register can be used
    to examine the return value including the _GEM_VERIFY_*_ defines which
    enumerate the verification state machine.
 */
uint32_t
MSS_MAC_get_mmsl_status
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_get_mmsl_stats()_ function is used to retrieve the statistics
  counts from the MAC Merge Sublayer component of the GEM.
  
  The function only operates on the pMAC.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param stats
    This parameter is a pointer to an _mss_mac_mmsl_stats_t_ structure which
    will be populated with the current MMSL statistics counts.

  @return
    This function does not return a value.
 */
void
MSS_MAC_get_mmsl_stats
(
    const mss_mac_instance_t *this_mac,
    mss_mac_mmsl_stats_t *stats
);

/***************************************************************************//**
  The _MSS_MAC_set_tx_cutthru()_ function is used to set the transmit cutthru
  level for the GEM DMA engine. The useful ranges are different for the eMAC and
  pMAC and will also differ based on the allocation of DMA buffer space to
  active queues.

  This setting should be used with caution and the correct values
  will depend on the overall bus speed amongst other things.
  
  __Note:__ This value may need to be configured to allow sending Jumbo frames
  of more than approximately 3,880 bytes depending on system configuration.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param level
    This 32 bit parameter indicates the minimum data to be read into the DMA
    buffer before cutthru operation begins.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_tx_cutthru
(
    const mss_mac_instance_t *this_mac, uint32_t level
);

/***************************************************************************//**
  The _MSS_MAC_set_rx_cutthru()_ function is used to set the receive cutthru
  level for the GEM DMA engine. The useful ranges are different for the eMAC
  and pMAC.
  
  This setting should be used with caution and the correct values will depend on
  the overall bus speed amongst other things.
  
  __Note:__ This value may need to be configured to allow receiving Jumbo frames
  of more than approximately 3,880 bytes depending on system configuration.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param level
    This 32 bit parameter indicates the minimum data to be written from the FIFO
    into the DMA buffer before cutthru operation begins.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_rx_cutthru
(
    const mss_mac_instance_t *this_mac,
    uint32_t level
);

/***************************************************************************//**
  The _MSS_MAC_get_tx_cutthru()_ function is used to retrieve the current
  transmit cutthru level for the GEM DMA engine.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns an unsigned 32 bit value representing the current
    minimum amount of data to be read into the DMA buffer before cutthru
    operation begins.
 */
uint32_t
MSS_MAC_get_tx_cutthru
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_get_rx_cutthru()_ function is used to retrieve the current
  receive cutthru level for the GEM DMA engine.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns an unsigned 32 bit value representing the current
    minimum amount of data to be read to be written from the FIFO into the DMA
    buffer before cutthru operation begins.
 */
uint32_t
MSS_MAC_get_rx_cutthru
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_tx_enable()_ function is used to start or restart transmit
  operations. It can be used as part of recovery from errors which may have
  resulted in transmission being stopped.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function does not return a value.
 */
void
MSS_MAC_tx_enable
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_jumbo_frames_mode()_ function is used to enable and disable
  reception of Jumbo frames. For this function to work correctly, the
  _jumbo_frame_enable_ value in the _mss_mac_cfg_t_ structure used to configure
  the Ethernet MAC will have to have been set to true.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

@param state
    This boolean parameter is set to true to enable Jumbo frame support and
    false to disable Jumbo frame support.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_jumbo_frames_mode
(
    const mss_mac_instance_t *this_mac,
    bool state
);

/***************************************************************************//**
  The _MSS_MAC_get_jumbo_frames_mode()_ function is used to determine if Jumbo
  frame support is enabled or not.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns a boolean value which is true if Jumbo frame support
    is enabled and false otherwise.
 */
bool
MSS_MAC_get_jumbo_frames_mode
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_jumbo_frame_length()_ function is used to set the maximum
  length of Jumbo frames that can be received. The maximum allowed is
  _MSS_MAC_JUMBO_MAX_. For this function to work correctly, the
  _jumbo_frame_enable_ value in the _mss_mac_cfg_t_ structure used to configure
  the Ethernet MAC will have to have been set to true.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param length
    This unsigned 32 bit int parameter sets the maximum Jumbo frame length that
    the MAC will accept. The upper limit is defined as _MSS_MAC_JUMBO_MAX_.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_jumbo_frame_length
(
    const mss_mac_instance_t *this_mac,
    uint32_t length
);

/***************************************************************************//**
  The _MSS_MAC_get_jumbo_frame_length()_ function is used to retrieve the
  current maximum length of Jumbo frames that can be received. For this function
  to work correctly, the _jumbo_frame_enable_ value in _the mss_mac_cfg_t_
  structure used to configure the Ethernet MAC will have to have been set to
  true.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns an unsigned 32 bit int indicating the current maximum
    Jumbo frame length that the MAC will accept.
 */
uint32_t
MSS_MAC_get_jumbo_frame_length
(
    const mss_mac_instance_t *this_mac
);

/***************************************************************************//**
  The _MSS_MAC_set_pause_frame_copy_to_mem()_ function is used to control the
  writing of pause frames into memory. 
  
  The default operation is not to copy pause frames into memory.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @param state
    This boolean value is true to enable copying of pause frames to memory and
    false to disable.

  @return
    This function does not return a value.
 */
void
MSS_MAC_set_pause_frame_copy_to_mem
(
    const mss_mac_instance_t *this_mac,
    bool state
);

/***************************************************************************//**
  The _MSS_MAC_get_pause_frame_copy_to_mem()_ function is used to determine
  whether the GEM is currently configured to copy pause frames to memory.

  @param this_mac
    This parameter is a pointer to one of the global _mss_mac_instance_t_
    structures which identifies the MAC that the function is to operate on.
    There are between 1 and 4 such structures identifying pMAC0, eMAC0, pMAC1
    and eMAC1.

  @return
    This function returns a boolean value which is true if copying of pause
    frames to memory is enabled and false otherwise.

 */
bool
MSS_MAC_get_pause_frame_copy_to_mem
(
    const mss_mac_instance_t *this_mac
);


#ifdef __cplusplus
}
#endif

#endif /* MSS_ETHERNET_MAC_H_ */
