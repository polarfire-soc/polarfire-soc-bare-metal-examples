/**
 * Copyright 2014 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * CoreTSE 10/100/1000 Mbps Ethernet MAC bare metal software driver public API.
 *
 * @file core_tse.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreTSE prototypes
 *
 */

/**
  @mainpage CoreTSE Bare Metal Driver

  @section intro Introduction

  The Core Triple Speed Ethernet (CoreTSE) is a hardware soft IP
  core that implements 10/100/1000 Mbps Ethernet Media Access Control (MAC).
  The CoreTSE supports MII/GMII/TBI interfaces to the physical
  layer devices (PHY).

  The only difference between CoreTSE_AHB and CoreTSE soft IP is that the CoreTSE_AHB
  has DMA with an AHB interface for transmitting and receiving data packets whereas
  the CoreTSE does not. For CoreTSE, no firmware intervention is needed for packet
  transmit/receive DMA operations. All the other operations are the same for
  CoreTSE_AHB and CoreTSE soft IP and this driver can be used to execute them.

  Note:
  This document covers the driver functionality for the CoreTSE_AHB and CoreTSE.
  For brevity, we use the term CoreTSE to describe both in this document, unless
  we are describing functionality specific to the CoreTSE_AHB.

  This software driver provides a set of functions for controlling the
  CoreTSE as part of a bare metal system where no operating system
  is available. This driver can be adapted for use as part of an operating
  system, but the implementation of the adaptation layer between the driver
  and the operating system's driver model is outside the scope of the driver.

  @section hw_flow_dependencies Hardware Flow Dependencies

  The configuration of all features of the CoreTSE_AHB or CoreTSE soft IP is
  covered by this driver except for the selection of the Ethernet
  PHY, the PHY interface, the size of transmit and receive rings, and the MDIO
  address of the TBI/1000BaseX module within CoreTSE.

  The PHY interface type is selected using the Firmware Catalogue configuration
  dialogue window.
  This driver supports MII, GMII and TBI interfaces.
  See the "PHY Interface Types" sub-section for an explanation of the PHY interface.

  The size of the transmit and receive rings defines the maximum number of transmit
  and receive packets that can be queued.

  The MDIO address of the TBI module within CoreTSE is also selected using the Firmware
  Catalogue configuration dialogue window.

  Note:
  This selection is applicable only when CoreTSE is configured to operate in TBI
  mode and is ignored when CoreTSE is configured to operate in G/MII mode.

  The system may use the CoreTSE in GMII mode and use an external
  CoreRGMII soft IP to interface with a PHY using RGMII interface type. If the
  CoreRGMII is being used in the system, then the MDIO address of the CoreRGMII
  also needs to be configured here for this driver.

  Your application software should configure the CoreTSE driver by calling the
  `TSE_init()` function for each CoreTSE instance in the hardware design.

  @subsection phy_if_types PHY Interface Types

  - MII: CoreTSE operates in MII mode and interfaces with MII PHY directly.

  - GMII: CoreTSE operates in GMII mode and interfaces with GMII PHY directly.

  - TBI: CoreTSE operates in TBI mode (Internal TBI/1000BaseX module is
  enabled).

  @section theory_op Theory of Operation

  The CoreTSE software driver is designed to allow the control of multiple instances
  of CoreTSE. Each instance of CoreTSE in the hardware design is associated with
  a single instance of the tse_instance_t structure in the software.
  You need to allocate memory for one unique tse_instance_t structure instance for
  each CoreTSE hardware instance. The contents of these data structures are initialized
  by calling `TSE_init()`.
  A pointer to this structure is passed to subsequent driver functions to identify
  the CoreTSE hardware instance you wish to perform the requested operation on.

  Note:
  Do not attempt to directly manipulate the content of tse_instance_t
  structures. This structure is only intended to be modified by the driver
  function.

  The CoreTSE driver functions are grouped into the following categories:
      - Initialization and configuration
      - Transmit operations
      - Receive operations
      - Reading link status and statistics
      - Address-based frame filtering
      - Wake up on LAN (WoL) with unicast match and AMD magic packet detection
      - Jumbo Frames

  @subsection Configuration

  The CoreTSE driver is initialized and configured by calling the `TSE_init()`
  function. The `TSE_init()` function takes a pointer to a configuration data
  structure as a parameter. This data structure contains all the configuration
  information required to initialize and configure the CoreTSE.


  @subsection Initialization

  The CoreTSE driver provides the `TSE_cfg_struct_def_init()` function to
  initialize the configuration data structure to the default value. It is
  recommended using this function to retrieve the default configuration and then
  overwrite the defaults with the application-specific settings such as PHY
  address, allowed link speeds, link duplex mode, and MAC address.

  The `TSE_init()` function must be called before any other CoreTSE driver
  functions. The `TSE_cfg_struct_def_init()` is the only function which can be
  called before calling the `TSE_init()` function.

  The following functions are used as part of the initialization and configuration
  process:
      - `TSE_cfg_struct_def_init()`
      - `TSE_init()`

  @subsection tx_op Transmit Operations

  The CoreTSE driver transmit operations are interrupt driven. The application
  must register a transmit call-back function with the driver using the
  `TSE_set_tx_callback()` function.
  The CoreTSE driver calls this call-back function every time a packet is sent.

  The application must call the `TSE_send_pkt()` function every time the user wants
  to transmit a packet. The application passes a pointer to the buffer
  containing the packet to send. It is the user's responsibility to
  manage the memory allocated to store the transmit packets. The CoreTSE
  driver only requires a pointer to the buffer containing the packet and the
  packet size. The CoreTSE driver calls the transmit call-back function
  registered using the `TSE_set_tx_callback()` function once a packet is sent.
  The transmit call-back function is supplied by the application and can be
  used to release the memory used to store the packet that was sent.

  The following functions are used as part of transmit and receive operations:
      - `TSE_send_pkt()`
      - `TSE_set_tx_callback()`

  Note:
  This operation is applicable only for CoreTSE_AHB and this operation is not supported
  on CoreTSE soft IP.

  @subsection rx_op Receive Operations

  The CoreTSE driver receive operations are interrupt driven. The application
  must first register a receive call-back function using the
  `TSE_set_rx_callback()` function. The application can then allocate receive
  buffers to the CoreTSE driver by calling the `TSE_receive_pkt()` function.
  This function can be called multiple times to allocate more than one receive
  buffer. The CoreTSE driver calls the receive call-back function whenever a packet
  is received into one of the receive buffer.
  The driver hands back the receive buffer to the application for packet processing.
  The CoreTSE driver won't reuse the receive buffer unless a call to `TSE_receive_pkt()`
  re-allocates it to the driver.

  The following functions are used as part of transmit and receive operations:
      - `TSE_receive_pkt()`
      - `TSE_set_rx_callback()`

  Note:
  This operation is applicable only for CoreTSE_AHB and this operation is not supported
  on CoreTSE soft IP.

  @subsection status Statistics and Link Status

  The CoreTSE driver provides the following functions to retrieve the current
  link status and statistics.
      - `TSE_get_link_status()`
      - `TSE_read_stat()`

  @subsection addr_filtering Address-based Frame Filtering

  The CoreTSE performs frame filtering based on the destination MAC
  address of the received frame. The `TSE_init()` function initializes the
  CoreTSE hardware to a default filtering mode where only broadcast
  frames and frames with a destination address equal to the local base station
  MAC address are passed to the MAC. The Broadcast frames are usually required
  by an application to obtain an IP address. This default configuration does not
  need the frame filtering hash table to be filled. This default configuration
  is returned with the `TSE_cfg_struct_def_init()` function. You may change the
  configuration before passing it to the `TSE_init()` function.

  The application can use the `TSE_set_address_filter()` function to overwrite
  the frame filter choice, which was selected at initialization time. The
  application must provide the list of MAC addresses from which it
  wants to receive frames (allowed MAC addresses list) to this driver using
  the `TSE_set_address_filter()` function. If a received frame contains one of
  the MAC addresses contained in the allowed MAC addresses list then that
  frame is passed.

  The setting of the broadcast, hash-unicast/multicast configuration in Frame Pass
  Control register is inferred from the values of the MAC addresses passed to the
  driver in the allowed MAC addresses list.

  - If all MAC addresses included in the allowed MAC addresses list are unicast
  then hash-unicast filtering is selected. This results in more unwanted frames
  being rejected.
  - If all MAC addresses included in the allowed MAC addresses list are multicast
  then hash-multicast filtering is selected.
  - If the allowed MAC addresses list contains a mix of unicast and multicast MAC
  addresses, then hash-unicast and hash-multicast filtering is selected.
  - Broadcast filtering is selected if the broadcast MAC address `(FF:FF:FF:FF:FF:FF)`
  is included in the allowed MAC addresses list.

  If the allowed MAC addresses list contains a mix of broadcast, unicast, and multicast
  MAC addresses then broadcast, hash-unicast and hash-multicast filtering is selected.
  The local base station MAC address must be included in the allowed MAC addresses
  list if the application wants to receive frames addressed to it after calling the
  `TSE_set_address_filter()` function.


  The filtering is not perfect since the filtering hardware uses a hash table.
  Therefore, some frames with addresses are not included in the allowed MAC
  addresses list are still passed because the hash value for their MAC address
  is identical to the hash value of an allowed MAC address. The hash
  unicast/multicast setting allows further reduction of the number of unwanted
  frames passed by both checking the MAC address against the hash table and
  checking the received MAC address unicast/multicast bit against the hash
  unicast/multicast setting of the filtering IP.

  The following functions is used as part of the frame filtering
      - `TSE_set_address_filter()`

  @subsection frame_drop Frame Drop Criterion

  Apart from the address-based frame filtering, the CoreTSE
  provides the facility to drop frames based on certain criteria.
  The criteria for the frame drop logic can be configured using the framedrop_mask
  element of the tse_cfg_t structure. The value of this element is set to
  TSE_DEFVAL_FRAMEDROP_MASK by the `TSE_cfg_struct_def_init()` function.
  This configuration value enables CoreTSE to drop frames which
  have CRC errors, code errors, and frames with unsupported op-codes. You may
  change the configuration before passing it to the `TSE_init()` function.

  @subsection wol Wake on LAN (WoL)
  The CoreTSE supports the Wake on LAN feature with unicast match
  frames and AMD magic packets. The WoL interrupt is generated when this feature
  is enabled and one of the two frames mentioned above is received.

  `TSE_cfg_struct_def_init()` disables WoL. You may change the configuration
  before passing it to the `TSE_init()` function. You can enable this feature using
  TSE_WOL_UNICAST_FRAME_DETECT_EN or TSE_WOL_MAGIC_FRAME_DETECT_EN or both the
  constants with a bitwise OR operation.

  The callback handler function must be provided to this driver using
  `TSE_set_wol_callback()` function. A function of type tse_wol_callback_t must
  be provided as a parameter to this function.
  The driver calls this function when the WoL event happens.

  The following function is used as part of WoL:
      - `TSE_set_wol_callback()`

  @subsection jumbo_frames Jumbo Frames

  The CoreTSE_AHB supports jumbo frames that exceed the 1500 byte max of the standard
  Ethernet frame, up to 4000 bytes long.
  This driver provides TSE_JUMBO_PACKET_SIZE constant which can be used to define
  the Tx-Rx buffer size where jumbo frame support is required.

*/
#ifndef CORE_TSE_H_
#define CORE_TSE_H_

#include "coretse_types.h"
#include "coretse_user_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  # State definitions

  | Constant    | Description         |
  | ----------- | ------------------- |
  | TSE_DISABLE | Disable the CoreTSE |
  | TSE_ENABLE  | Enable the CoreTSE  |
 */
#define TSE_DISABLE                      ((uint8_t)0)
#define TSE_ENABLE                       ((uint8_t)1)

/**
  # Packet Transmission

  The following definitions are used with functions `TSE_send_pkt()` and
  `TSE_receive_pkt()` to report success or failure in assigning the packet memory
  buffer to a transmit/receive descriptor.

  | Constant    | Description |
  | ----------- | --------------------------------------------------------------------------------- |
  | TSE_SUCCESS | The memory buffer was successfully assigned to a transmit/receive descriptor.     |
  | TSE_FAILED  | The memory buffer was not successfully assigned to a transmit/receive descriptor. |

*/
#define TSE_SUCCESS                      TSE_ENABLE
#define TSE_FAILED                       TSE_DISABLE

/**
  # Link Status

  The following definitions are used with `TSE_get_link_status()` to report the link status.

  | Constant        | Description                                                            |
  | --------------- | -----------------------------------------------------------------------|
  | TSE_LINK_DOWN   | Ethernet link is down. There is no connection to a remote device.      |
  | TSE_LINK_UP     | Ethernet link is up. A connection is established with a remote device. |
  | TSE_HALF_DUPLEX | Connection is half-duplex.                                             |
  | TSE_FULL_DUPLEX | Connection is full-duplex.                                             |

*/

#define TSE_LINK_DOWN                    0u
#define TSE_LINK_UP                      1u

#define TSE_HALF_DUPLEX                  0u
#define TSE_FULL_DUPLEX                  1u

/**
  # Maximum MAC frame size (packet size)

  | Constant              | Description                                       |
  | --------------------- | ------------------------------------------------- |
  | TSE_MAX_PACKET_SIZE   | Maximum packet size of a standard Ethernet frame. |
  | TSE_JUMBO_PACKET_SIZE | Maximum size of Jumbo frame packet.               |
 */
#define TSE_MAX_PACKET_SIZE              1518u
#define TSE_JUMBO_PACKET_SIZE            9000u

/**
  # Transmit and receive packet buffer sizes

  | Constant            | Description                   |
  | ------------------- | ----------------------------- |
  | TSE_MAX_TX_BUF_SIZE | Maximum transmit buffer size. |
  | TSE_MAX_RX_BUF_SIZE | Maximum receive buffer size.  |
 */
#define TSE_MAX_TX_BUF_SIZE              TSE_MAX_PACKET_SIZE
#define TSE_MAX_RX_BUF_SIZE              TSE_MAX_PACKET_SIZE

/**
  # Configuration Parameter Definitions

 */

/**
  ## FIFO error detection & correction

  | Constant                 | Description                                  |
  | ------------------------ | -------------------------------------------- |
  | TSE_ERR_DET_CORR_ENABLE  | Enable FIFO error detection and correction.  |
  | TSE_ERR_DET_CORR_DISABLE | Disable FIFO error detection and correction. |
 */
#define TSE_ERR_DET_CORR_ENABLE          TSE_ENABLE
#define TSE_ERR_DET_CORR_DISABLE         TSE_DISABLE

/**
  ## Huge frame support

  | Constant               | Description                 |
  | ---------------------- | --------------------------- |
  | TSE_HUGE_FRAME_ENABLE  | Enable Huge frame support.  |
  | TSE_HUGE_FRAME_DISABLE | Disable Huge frame support. |
 */
#define TSE_HUGE_FRAME_ENABLE            TSE_ENABLE
#define TSE_HUGE_FRAME_DISABLE           TSE_DISABLE

/**
  ## Length field checking

  | Constant                       | Description                    |
  | ------------------------------ | ------------------------------ |
  | TSE_LENGTH_FIELD_CHECK_ENABLE  | Enable length field checking.  |
  | TSE_LENGTH_FIELD_CHECK_DISABLE | Disable length field checking. |
 */
#define TSE_LENGTH_FIELD_CHECK_ENABLE    TSE_ENABLE
#define TSE_LENGTH_FIELD_CHECK_DISABLE   TSE_DISABLE

/**
  ## Padding and CRC

  | Constant               | Description              |
  | ---------------------- | ------------------------ |
  | TSE_PAD_N_CRC_ENABLE   | Enable padding and CRC.  |
  | TSE_PAD_N_CRC_DISABLE  | Disable padding and CRC. |
*/
#define TSE_PAD_N_CRC_ENABLE             TSE_ENABLE
#define TSE_PAD_N_CRC_DISABLE            TSE_DISABLE

/**
  ## Append CRC

  | Constant        | Description            |
  | --------------- | ---------------------- |
  | TSE_CRC_ENABLE  | Enable CRC appending.  |
  | TSE_CRC_DISABLE | Disable CRC appending. |
 */
#define TSE_CRC_ENABLE                   TSE_ENABLE
#define TSE_CRC_DISABLE                  TSE_DISABLE

/**
  ## Full-duplex mode

  | Constant               | Description              |
  | ---------------------- | ------------------------ |
  | TSE_FULLDUPLEX_ENABLE  | Enable full-duplex mode.  |
  | TSE_FULLDUPLEX_DISABLE | Disable full-duplex mode. |
 */
#define TSE_FULLDUPLEX_ENABLE            TSE_ENABLE
#define TSE_FULLDUPLEX_DISABLE           TSE_DISABLE

/**
  ## Loopback mode

  | Constant             | Description            |
  | -------------------- | ---------------------- |
  | TSE_LOOPBACK_ENABLE  | Enable loopback mode.  |
  | TSE_LOOPBACK_DISABLE | Disable loopback mode. |
 */
#define TSE_LOOPBACK_ENABLE              TSE_ENABLE
#define TSE_LOOPBACK_DISABLE             TSE_DISABLE

/**
  ## Receiver flow control

  | Constant                 | Description                    |
  | ------------------------ | ------------------------------ |
  | TSE_RX_FLOW_CTRL_ENABLE  | Enable receiver flow control.  |
  | TSE_RX_FLOW_CTRL_DISABLE | Disable receiver flow control. |
 */
#define TSE_RX_FLOW_CTRL_ENABLE          TSE_ENABLE
#define TSE_RX_FLOW_CTRL_DISABLE         TSE_DISABLE

/**
  ## Transmission flow control

  | Constant                 | Description                        |
  | ------------------------ | ---------------------------------- |
  | TSE_TX_FLOW_CTRL_ENABLE  | Enable Transmission flow control.  |
  | TSE_TX_FLOW_CTRL_DISABLE | Disable Transmission flow control. |
 */
#define TSE_TX_FLOW_CTRL_ENABLE          TSE_ENABLE
#define TSE_TX_FLOW_CTRL_DISABLE         TSE_DISABLE

/**
  ## Excessive defer

  | Constant               | Description              |
  | ---------------------- | ------------------------ |
  | TSE_EXSS_DEFER_ENABLE  | Enable excessive defer.  |
  | TSE_EXSS_DEFER_DISABLE | Disable excessive defer. |

 */
#define TSE_EXSS_DEFER_ENABLE            TSE_ENABLE
#define TSE_EXSS_DEFER_DISABLE           TSE_DISABLE

/**
  ## No-backoff

  | Constant               | Description         |
  | ---------------------- | ------------------- |
  | TSE_NO_BACKOFF_ENABLE  | Enable no-backoff.  |
  | TSE_NO_BACKOFF_DISABLE | Disable no-backoff. |
 */
#define TSE_NO_BACKOFF_ENABLE            TSE_ENABLE
#define TSE_NO_BACKOFF_DISABLE           TSE_DISABLE

/**
  ## Backpressure no-backoff

  | Constant                         | Description                      |
  | -------------------------------- | -------------------------------- |
  | TSE_BACKPRESS_NO_BACKOFF_ENABLE  | Enable backpressure no-backoff.  |
  | TSE_BACKPRESS_NO_BACKOFF_DISABLE | Disable backpressure no-backoff. |
 */
#define TSE_BACKPRESS_NO_BACKOFF_ENABLE  TSE_ENABLE
#define TSE_BACKPRESS_NO_BACKOFF_DISABLE TSE_DISABLE

/**
  ## Alternative binary exponential backoff

  | Constant         | Description                                     |
  | ---------------- | ----------------------------------------------- |
  | TSE_ABEB_ENABLE  | Enable alternative binary exponential backoff.  |
  | TSE_ABEB_DISABLE | Disable alternative binary exponential backoff. |
 */
#define TSE_ABEB_ENABLE                  TSE_ENABLE
#define TSE_ABEB_DISABLE                 TSE_DISABLE

/**
  ## Suppress preamble

  | Constant                      | Description                   |
  | ----------------------------- | ----------------------------- |
  | TSE_SUPPRESS_PREAMBLE_ENABLE  | Enable preamble suppression.  |
  | TSE_SUPPRESS_PREAMBLE_DISABLE | Disable preamble suppression. |
 */
#define TSE_SUPPRESS_PREAMBLE_ENABLE     TSE_ENABLE
#define TSE_SUPPRESS_PREAMBLE_DISABLE    TSE_DISABLE

/**
  ## Auto-scan PHYs

  | Constant                 | Description             |
  | ------------------------ | ----------------------- |
  | TSE_PHY_AUTOSCAN_ENABLE  | Enable PHYs auto-scan.  |
  | TSE_PHY_AUTOSCAN_DISABLE | Disable PHYs auto-scan. |
 */
#define TSE_PHY_AUTOSCAN_ENABLE          TSE_ENABLE
#define TSE_PHY_AUTOSCAN_DISABLE         TSE_DISABLE

/**
  ## WoL event detection masks

  | Constant                        | Description                             |
  | ------------------------------- | --------------------------------------- |
  | TSE_WOL_UNICAST_FRAME_DETECT_EN | WoL with unicast match frame detection. |
  | TSE_WOL_MAGIC_FRAME_DETECT_EN   | WoL with AMD magic packet detection.    |
  | TSE_WOL_DETECT_DISABLE          | WoL detection disabled.                 |
 */
#define TSE_WOL_UNICAST_FRAME_DETECT_EN  (1u << 0)
#define TSE_WOL_MAGIC_FRAME_DETECT_EN    (1u << 1)
#define TSE_WOL_DETECT_DISABLE           TSE_DISABLE

/**
  ## Preamble length default value and maximum value

  | Constant            | Description              |
  | ------------------- | ------------------------ |
  | TSE_PREAMLEN_DEFVAL | Default preamble length. |
  | TSE_PREAMLEN_MAXVAL | Maximum preamble length. |
 */
#define TSE_PREAMLEN_DEFVAL              ((uint8_t)(0x07))
#define TSE_PREAMLEN_MAXVAL              ((uint8_t)(0x0F))

/**
  ## Byte/Nibble mode

  | Constant        | Description  |
  | --------------- | ------------ |
  | TSE_NIBBLE_MODE | Nibble mode. |
  | TSE_BYTE_MODE   | Byte mode.   |
 */
#define TSE_NIBBLE_MODE                  ((uint8_t)(0x00))
#define TSE_BYTE_MODE                    ((uint8_t)(0x01))

/**
  # IPG/IFG values

  | Constant             | Description                                      |
  | -------------------- | -------------------------------------------------|
  | TSE_MINIFG_MAXVAL    | Minimum interframe gap - maximum value.          |
  | TSE_MINIFG_DEFVAL    | Minimum interframe gap - default value.          |
  | TSE_BTBIFG_MAXVAL    | Back-to-back interframe gap - maximum value      |
  | TSE_BTBIFG_DEFVAL    | Back-to-back interframe gap - default value.     |
  | TSE_NONBTBIFG_DEFVAL | Non-back-to-back interframe gap - default value. |
  | TSE_NONBTBIFG_MAXVAL | Non-back-to-back interframe gap - maximum value. |
 */
#define TSE_MINIFG_MAXVAL                ((uint8_t)(0xFF))
#define TSE_MINIFG_DEFVAL                ((uint8_t)(0x50))
#define TSE_BTBIFG_MAXVAL                ((uint8_t)(0x7F))
#define TSE_BTBIFG_DEFVAL                ((uint8_t)(0x60))
#define TSE_NONBTBIFG_DEFVAL             ((uint16_t)((0x4000) | 0x0060))
#define TSE_NONBTBIFG_MAXVAL             ((uint16_t)(0x3FFF))

/**
  # Number of maximum retransmission tries

  | Constant             | Description                                                      |
  | -------------------- | ---------------------------------------------------------------- |
  | TSE_MAXRETX_MAXVAL   | Maximum number of retransmission attempts - maximum value.       |
  | TSE_MAXRETX_DEFVAL   | Maximum number of retransmission attempts - default value.       |
  | TSE_ABEBTRUNC_MAXVAL | Alternate Binary Exponential Backoff Truncation - maximum value. |
  | TSE_ABEBTRUNC_DEFVAL | Alternate Binary Exponential Backoff Truncation - default value. |
 */
#define TSE_MAXRETX_MAXVAL               ((uint8_t)(0x1F))
#define TSE_MAXRETX_DEFVAL               ((uint8_t)(0x0F))
#define TSE_ABEBTRUNC_MAXVAL             ((uint8_t)(0x0F))
#define TSE_ABEBTRUNC_DEFVAL             ((uint8_t)(0x0A))

/**
  # PHY clock divider values

  | Constant         | Description                 |
  | ---------------- | --------------------------- |
  | TSE_DEF_PHY_CLK  | Default source clock.       |
  | TSE_BY4_PHY_CLK  | Source clock divided by 4.  |
  | TSE_BY6_PHY_CLK  | Source clock divided by 6.  |
  | TSE_BY8_PHY_CLK  | Source clock divided by 8.  |
  | TSE_BY10_PHY_CLK | Source clock divided by 10. |
  | TSE_BY14_PHY_CLK | Source clock divided by 14. |
  | TSE_BY20_PHY_CLK | Source clock divided by 20. |
  | TSE_BY28_PHY_CLK | Source clock divided by 28. |
 */
#define TSE_DEF_PHY_CLK                  ((uint8_t)(0x07))
#define TSE_BY4_PHY_CLK                  ((uint8_t)(0x01))
#define TSE_BY6_PHY_CLK                  ((uint8_t)(0x02))
#define TSE_BY8_PHY_CLK                  ((uint8_t)(0x03))
#define TSE_BY10_PHY_CLK                 ((uint8_t)(0x04))
#define TSE_BY14_PHY_CLK                 ((uint8_t)(0x05))
#define TSE_BY20_PHY_CLK                 ((uint8_t)(0x06))
#define TSE_BY28_PHY_CLK                 ((uint8_t)(0x07))

/**
  # PHY addresses

  | Constant              | Description                           |
  | --------------------- | ------------------------------------- |
  | TSE_DEFAULT_PHY       | Default PHY address.                  |
  | TSE_PHYADDR_MAXVAL    | PHY address - maximum value.          |
  | TSE_PHYREGADDR_MAXVAL | PHY register address - maximum value. |
  | TSE_PHY_ADDR_DEFVAL   | PHY address - default value.          |
  | TSE_PHYREGADDR_DEFVAL | PHY register address - default value. |
 */
#define TSE_DEFAULT_PHY                  (DP83848)
#define TSE_PHYADDR_MAXVAL               ((uint8_t)(0x1F))
#define TSE_PHYREGADDR_MAXVAL            ((uint8_t)(0x1F))
#define TSE_PHY_ADDR_DEFVAL              ((uint8_t)(0x00))
#define TSE_PHYREGADDR_DEFVAL            ((uint8_t)(0x00))

/**
  # Maximum frame length

  | Constant               | Description                           |
  | ---------------------- | ------------------------------------- |
  | TSE_MAXFRAMELEN_DEFVAL | Maximum frame length - default value. |
  | TSE_MAXFRAMELEN_MAXVAL | Maximum frame length - maximum value. |
 */
#define TSE_MAXFRAMELEN_DEFVAL           ((uint16_t)(0x0600))
#define TSE_MAXFRAMELEN_MAXVAL           ((uint16_t)(0x0600))

/**
  # Slot Time (Collision window)

  | Constant            | Description                                 |
  | ------------------- | ------------------------------------------- |
  | TSE_SLOTTIME_DEFVAL | Slot time (Collision window) default value. |
  | TSE_SLOTTIME_MAXVAL | Slot time (Collision window) maximum value. |
 */
#define TSE_SLOTTIME_DEFVAL              ((uint16_t)(0x0037))
#define TSE_SLOTTIME_MAXVAL              ((uint16_t)(0x03FF))

/**
  # Auto-negotiation options

  | Constant         | Description               |
  | ---------------- | ------------------------- |
  | TSE_ANEG_ENABLE  | Enable auto-negotiation.  |
  | TSE_ANEG_DISABLE | Disable auto-negotiation. |
 */
#define TSE_ANEG_ENABLE                  TSE_ENABLE
#define TSE_ANEG_DISABLE                 TSE_DISABLE

/**
  # Speed configuration options

| Constant            | Description                              |
| ------------------- | ---------------------------------------- |
| TSE_ANEG_10M_FD     | Auto-negotiate at 10 Mbps full-duplex.   |
| TSE_ANEG_10M_HD     | Auto-negotiate at 10 Mbps half-duplex.   |
| TSE_ANEG_100M_FD    | Auto-negotiate at 100 Mbps full-duplex.  |
| TSE_ANEG_100M_HD    | Auto-negotiate at 100 Mbps half-duplex.  |
| TSE_ANEG_1000M_FD   | Auto-negotiate at 1000 Mbps full-duplex. |
| TSE_ANEG_ALL_SPEEDS | Auto-negotiate at all speeds.            |

 */
#define TSE_ANEG_10M_FD                  0x00000001u
#define TSE_ANEG_10M_HD                  0x00000002u
#define TSE_ANEG_100M_FD                 0x00000004u
#define TSE_ANEG_100M_HD                 0x00000008u
#define TSE_ANEG_1000M_FD                0x00000010u
#define TSE_ANEG_1000M_HD                0x00000020u
#define TSE_ANEG_ALL_SPEEDS \
    (TSE_ANEG_10M_FD | TSE_ANEG_10M_HD | TSE_ANEG_100M_FD | TSE_ANEG_100M_HD | TSE_ANEG_1000M_FD | \
     TSE_ANEG_1000M_HD)

/**
  # Frame Filter configuration options

| Constant                         | Description |
| -------------------------------- |----------------------------------------------------------------------------------- | 
| TSE_FC_PASS_BROADCAST_MASK       | Pass all broadcast frames.                                                         |
| TSE_FC_PASS_MULTICAST_MASK       | Pass all broadcast frames.                                                         |
| TSE_FC_PASS_UNICAST_MASK         | Pass the frame if its Unicast-DA matches the configured destination address.       |
| TSE_FC_PROMISCOUS_MODE_MASK      | Promiscuous mode, allow all the frames to pass.                                    |
| TSE_FC_PASS_UNICAST_HASHT_MASK   | Pass the frame if the hash table entry matches for Unicast destination addresses.  |
| TSE_FC_PASS_MULTICAST_HASHT_MASK | Pass the frame if the hash table entry matches for Multicast destination address.  |
| TSE_FC_DEFAULT_MASK              | Default frame filtering mask.                                                      |
 */
#define TSE_FC_PASS_BROADCAST_MASK       (1u << 0)
#define TSE_FC_PASS_MULTICAST_MASK       (1u << 1)
#define TSE_FC_PASS_UNICAST_MASK         (1u << 2)
#define TSE_FC_PROMISCOUS_MODE_MASK      (1u << 3)
#define TSE_FC_PASS_UNICAST_HASHT_MASK   (1u << 4)
#define TSE_FC_PASS_MULTICAST_HASHT_MASK (1u << 5)
#define TSE_FC_DEFAULT_MASK              (TSE_FC_PASS_BROADCAST_MASK | TSE_FC_PASS_UNICAST_MASK)

/**
  # Default framedrop mask

  | Constant                  | Description                                            |
  | ------------------------- | ------------------------------------------------------ |
  | TSE_DEFVAL_FRAMEDROP_MASK | Generic default value applicable to most applications. |
 */
#define TSE_DEFVAL_FRAMEDROP_MASK \
    (TSE_CRC_ERR_FRAMEDROP_MASK | TSE_CODE_ERR_FRAMEDROP_MASK | \
     TSE_UNSUPPORTED_OPCODE_FRAMEDROP_MASK)

/**
  ## Framedrop if IFG is small

  | Constant                     | Description                      |
  | ---------------------------- | -------------------------------- |
  | TSE_SMALL_AFG_FRAMEDROP_MASK | Drop the packet if IFG is small. |
 */
#define TSE_SMALL_AFG_FRAMEDROP_MASK          (1u << 0)

/**
  ## Framedrop if short RXDV Event

  | Constant                      | Description                                        |
  | ----------------------------- | -------------------------------------------------- |
  | TSE_EXDV_EVENT_FRAMEDROP_MASK | Drop the packet if a short RXDV event is received. |

  Short RXDV event: Indicates that the last received event seen was not long enough to be a valid
  packet.
 */
#define TSE_EXDV_EVENT_FRAMEDROP_MASK         (1u << 1)

/**
  ## Framedrop if False Carrier

  | Constant                         | Description                                   |
  | -------------------------------- | --------------------------------------------- |
  | TSE_FALSE_CARRIER_FRAMEDROP_MASK | Drop the packet if a False Carrier is received. |

  ### False Carrier
  Indicates that at some time since the last receive statistics vector, a false
  carrier was detected, noted, and reported with the next receive statistics.
  The false carrier is not associated with this packet.
  False carrier is an activity on the receive channel that does not result in a
  packet receive attempt being made.

  Defined to be RXER = 1, RXDV = 0, RXD[3:0] = 0xE, RXD[7:0] = 0x0E.
 */
#define TSE_FALSE_CARRIER_FRAMEDROP_MASK      (1u << 2)

/**
  ## Framedrop if Code error

  | Constant                    | Description                                  |
  | --------------------------- | -------------------------------------------- |
  | TSE_CODE_ERR_FRAMEDROP_MASK | Drop the packet if a code error is received. |

  Code Error: One or more nibbles were signalled as errors during the reception of the packet.
 */
#define TSE_CODE_ERR_FRAMEDROP_MASK           (1u << 3)

/**
  ## Framedrop if CRC Error

  | Constant                   | Description                                 |
  | -------------------------- | ------------------------------------------- |
  | TSE_CRC_ERR_FRAMEDROP_MASK | Drop the packet if a CRC Error is received. |

  CRC Error: The packet's CRC did not match the internally generated CRC.
 */
#define TSE_CRC_ERR_FRAMEDROP_MASK            (1u << 4)

/**
  ## Framedrop if Length check error

  | Constant                      | Description                                         |
  | ----------------------------- | --------------------------------------------------- |
  | TSE_LEN_CHKERR_FRAMEDROP_MASK | Drop the frame if a length check error is received. |

  ### Length Check Error
  Indicates that the frame length field value in the packet does not match the actual
  data byte length and is not a type field.
 */
#define TSE_LEN_CHKERR_FRAMEDROP_MASK         (1u << 5)

/**
  ## Framedrop if Length Out of Range

  | Constant                          | Description                                                       |
  | --------------------------------- | ----------------------------------------------------------------- | 
  |  TSE_OUTOFRANGE_LEN_FRAMEDROP_MASK | Drop the frame if a frame with Length Out of Range is  received. |

  ### Length Out of Range
  Indicates that the frames length was larger than 1518 bytes but smaller than
  the hosts maximum frame length value (type field).
 */
#define TSE_OUTOFRANGE_LEN_FRAMEDROP_MASK     (1u << 6)

/**
  ## Framedrop if an OK frame

  | Constant                    | Description                                 |
  | --------------------------- | ------------------------------------------- |
  | TSE_OK_FRAME_FRAMEDROP_MASK | Drop the frame if an OK frame is received.  |

  ### OK frame
  The frame contained a valid CRC and did not have a code error.
 */
#define TSE_OK_FRAME_FRAMEDROP_MASK           (1u << 7)

/**
  ## Framedrop if Multicast frame

  | Constant                     | Description                                      |
  | ---------------------------- | ------------------------------------------------ |
  | TSE_MULTICAST_FRAMEDROP_MASK | Drop the frame if a Multicast frame is received. |

  ### Multicast frame
  The packet's destination address contained a multicast address.
 */
#define TSE_MULTICAST_FRAMEDROP_MASK          (1u << 8)

/**
  ## Framedrop if Broadcast frame

  | Constant                     | Description                                      |
  | ---------------------------- | ------------------------------------------------ |
  | TSE_BROADCAST_FRAMEDROP_MASK | Drop the frame if a Broadcast frame is received. |

  ### Broadcast frame
  The packet's destination address contained the broadcast address.
 */
#define TSE_BROADCAST_FRAMEDROP_MASK          (1u << 9)

/**
  ## Framedrop if Dribble Nibble

  | Constant                          | Description                                     |
  | --------------------------------- | ----------------------------------------------- |
  | TSE_DRIBBLE_NIBBLE_FRAMEDROP_MASK | Drop the frame if a Dribble Nibble is received. |

  ### Dribble Nibble
  Indicates that after the end of the packet, an additional 1 to 7 bits were received.
  A single nibble, called the dribble nibble, is formed but not sent to the system
  (10/100 Mbps only).
 */
#define TSE_DRIBBLE_NIBBLE_FRAMEDROP_MASK     (1u << 10)

/**
  ## Framedrop if Control Frame

  | Constant                         | Description                                    |
  | -------------------------------- | ---------------------------------------------- |
  | TSE_CONTROL_FRAME_FRAMEDROP_MASK | Drop the frame if a Control Frame is received. |

  ### Control Frame
  The current frame was recognized as a Control frame for having a valid
  Type-Length designation.
 */
#define TSE_CONTROL_FRAME_FRAMEDROP_MASK      (1u << 11)

/**
  ## Framedrop if PAUSE Control Frame

  | Constant                       | Description                                          |
  | ------------------------------ | ---------------------------------------------------- |
  | TSE_PAUSE_FRAME_FRAMEDROP_MASK | Drop the frame if a PAUSE Control Frame is received. |

  ### PAUSE Control frame
  Current frame was recognized as a Control frame containing a valid PAUSE Frame
  Op-code and a valid address.
 */
#define TSE_PAUSE_FRAME_FRAMEDROP_MASK        (1u << 12)

/**
  ## Framedrop if Unsupported Op-code

  | Constant                              | Description                                           |
  | ------------------------------------- | ----------------------------------------------------- |
  | TSE_UNSUPPORTED_OPCODE_FRAMEDROP_MASK | Drop the frame if an Unsupported Op-code is received. |

  ### Unsupported Op-code
  The current frame was recognized as a Control frame by the PEMCS, but it contains an unknown
  Op-code.
 */
#define TSE_UNSUPPORTED_OPCODE_FRAMEDROP_MASK (1u << 13)

/**
  ## Framedrop if VLAN Tag

  | Constant                    | Description                             |
  | --------------------------- | --------------------------------------- |
  | TSE_VLAN_TAG_FRAMEDROP_MASK | Drop the frame if VLAN Tag is detected. |

  ### VLAN Tag: Received frames length/type field that contains 0x8100, which is the VLAN Protocol
  Identifier.
 */
#define TSE_VLAN_TAG_FRAMEDROP_MASK           (1u << 14)

/**
  ## Framedrop if a Long Event

  | Constant                      | Description                                 |
  | ----------------------------- | ------------------------------------------- |
  | TSE_LONG_EVENT_FRAMEDROP_MASK | Drop the frame if a Long Event is received. |
 */
#define TSE_LONG_EVENT_FRAMEDROP_MASK         (1u << 15)

/**
  ## Framedrop if a Truncated frame

  | Constant                      | Description                                 |
  | ----------------------------- | ------------------------------------------- |
  | TSE_LONG_EVENT_FRAMEDROP_MASK | Drop the frame if a Long Event is received. |
 */
#define TSE_TRUNCKATED_FRAME_FRAMEDROP_MASK   (1u << 16)

/**
  ## Framedrop if Unicast frame detected

  | Constant                              | Description                                                                                  |
  | ------------------------------------- | -------------------------------------------------------------------------------------------- |
  | TSE_UNICAST_NO_SAMATCH_FRAMEDROP_MASK | Drop the frame if a Unicast frame is detected but did not  match configured station address. |
 */
#define TSE_UNICAST_NO_SAMATCH_FRAMEDROP_MASK (1u << 17)

/**
  ## Framedrop if a short frame received

  | Constant                       | Description                                                               |
  | ------------------------------ | ------------------------------------------------------------------------- |
  | TSE_SHORT_FRAME_FRAMEDROP_MASK | Drop the frame if a short frame less than 64 bytes in length is received. |
 */
#define TSE_SHORT_FRAME_FRAMEDROP_MASK        (1u << 18)

/**
  # PHY Address

  The application can use this constant to indicate to the driver to probe the PHY
  and auto-detect the PHY address that it is configured with. If you already
  know the PHY address configured in your hardware system, you can provide that
  address to the driver instead of this constant. That way the `TSE_init()` function
  would be faster because the auto-detection process of the PHY address is now
  avoided.

  Note:
  To auto-detect the PHY address, the driver scans the valid MDIO addresses starting
  from -0- for valid data.
  If CoreRGMII and PHY are connected to the MAC's management interface,
  then you must make sure that the PHY device MDIO address is less than the
  CoreRGMII MDIO address.

  | Constant                    | Value |
  | --------------------------- | ----- |
  | TSE_PHY_ADDRESS_AUTO_DETECT | 255   |

 */
#define TSE_PHY_ADDRESS_AUTO_DETECT           255

/**
  # Per Packet Override Masks

  A 5-bit field containing the per-packet override flags signalled to the FIFO during
  packet transmission.
  The bits are encoded as follows:

  | Constant                      | Value                                     |
  | ----------------------------- | ----------------------------------------- |
  | TSE_FIFO_TX_CTRL_FRAME        | FIFO Transmit Control Frame               |
  | TSE_FIFO_TX_NO_CTRL_FRAME     | Not  FIFO Transmit Control Frame          |
  | TSE_FIFO_TX_PERPKT_PAD_FCS    | FIFO Transmit Per-Packet PAD Mode         |
  | TSE_FIFO_TX_PERPKT_NO_PAD_FCS | Not  FIFO Transmit Per-Packet PAD Mode    |
  | TSE_FIFO_TX_PERPKT_FCS        | FIFO Transmit Per-Packet Generate FCS     |
  | TSE_FIFO_TX_PERPKT_NO_FCS     | Not FIFO Transmit Per-Packet Generate FCS |
  | TSE_FIFO_TX_PERPKT_ENABLE     | FIFO Transmit Per-Packet Generate FCS     |
  | TSE_FIFO_TX_PERPKT_DISABLE    | Not FIFO Transmit Per-Packet Generate FCS |
 */

#define TSE_FIFO_TX_CTRL_FRAME                ((uint32_t)0x00100000)
#define TSE_FIFO_TX_NO_CTRL_FRAME             (~TSE_FIFO_TX_CTRL_FRAME)
#define TSE_FIFO_TX_PERPKT_PAD_FCS            ((uint32_t)0x00040000)
#define TSE_FIFO_TX_PERPKT_NO_PAD_FCS         (~TSE_FIFO_TX_PERPKT_PAD_FCS)
#define TSE_FIFO_TX_PERPKT_FCS                ((uint32_t)0x00020000)
#define TSE_FIFO_TX_PERPKT_NO_FCS             (~TSE_FIFO_TX_PERPKT_FCS)
#define TSE_FIFO_TX_PERPKT_ENABLE             ((uint32_t)0x00010000)
#define TSE_FIFO_TX_PERPKT_DISABLE            (~TSE_FIFO_TX_PERPKT_ENABLE)

/*------------------------Public Function-------------------------------------*/

/**

    `TSE_cfg_struct_def_init()` initializes a tse_cfg_t configuration
    data structure to default values. The default configuration uses the MII
    interface connected to a PHY at address 0x00 which is set to auto-negotiate
    at all available speeds up to 1000Mbps. This default configuration can then
    be used as a parameter to `TSE_init()`. Typically the default configuration
    would be modified to suit the application before being passed to `TSE_init()`.

  @param cfg
    The cfg parameter is a pointer to a tse_cfg_t data structure used as a parameter
    to function `TSE_init()`.

  @return
  This function does not return a value.

  @example
  The example below demonstrates the use of the `TSE_cfg_struct_def_init()`
  function. It retrieves the default MAC configuration and modifies it to
  connect through an MII Ethernet PHY at MII management interface address 0x01.

  This example also demonstrates how to assign the device's MAC address and
  force a 100Mbps full duplex link.

  @code
    #define TSEMAC_BASE  0x30000000
    tse_cfg_t g_tse_config;
    tse_instance_t g_tse;

    TSE_cfg_struct_def_init(&g_tse_config);

    g_tse_config.phy_addr = TSE_PHY_ADDRESS_AUTO_DETECT;
    g_tse_config.speed_duplex_select = TSE_ANEG_100M_FD;
    g_tse_config.mac_addr[0] = 0xC0;
    g_tse_config.mac_addr[1] = 0xB1;
    g_tse_config.mac_addr[2] = 0x3C;
    g_tse_config.mac_addr[3] = 0x60;
    g_tse_config.mac_addr[4] = 0x60;
    g_tse_config.mac_addr[5] = 0x60;

    TSE_init(&g_tse, TSEMAC_BASE, &g_tse_config);

  @endcode
 */

void TSE_cfg_struct_def_init(tse_cfg_t *cfg);

/**
    `TSE_init()` initializes the CoreTSE hardware and
    driver internal data structures.
    This configuration data structure contains all the information required to
    configure the CoreTSE. The `TSE_init()` function initializes the
    descriptor rings and their pointers to initial values. `TSE_init()` enables DMA
    Rx packet received and Tx packet sent interrupts.

    The configuration passed to the `TSE_init()` function specifies the type of
    interface used to connect the CoreTSE and Ethernet PHY as well
    as the PHY MII management interface address. The application may pass the
    TSE_PHY_ADDRESS_AUTO_DETECT constant as a configuration parameter to indicate
    to the driver to probe for the PHY address. If you know the hardware
    configuration PHY address of the onboard PHY in your system, you may choose
    to provide that instead.
    When the user supplies a PHY address value other than TSE_PHY_ADDRESS_AUTO_DETECT,
    the driver will use the supplied value directly, provided it is within the valid
    PHY address range (0u - 31u).

    The `TSE_init()` function also specifies the allowed link speeds and duplex
    modes. It is at this point that the application chooses to auto-negotiate the
    link speeds and duplex modes with the link partner or force a specific speed and
    duplex mode.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t data structure
    instance holding all data regarding the CoreTSE hardware instance
    being initialized. A pointer to the same data structure must be provided in
    subsequent calls to the various CoreTSE driver functions to identify the CoreTSE
    instance that should perform the operation
    implemented by the called driver function.

  @param base_addr
    The base_addr parameter is the base address in the processor's memory map
    for the registers of the CoreTSE instance being initialized.

  @param cfg
    The cfg parameter is a pointer to a data structure of type tse_cfg_t
    containing the CoreTSE's requested configuration. You must
    initialize this data structure by first calling the `TSE_cfg_struct_def_init()`
    function to fill the configuration data structure with default values. You
    can then overwrite some of the default settings with the ones specific to your
    application before passing this data structure as a parameter to the `TSE_init()`
    function.

    Note:
    At a minimum the `mac_addr[5]` array of the configuration data structure must
    be  overwritten so a unique value can be used as the device's MAC address.

  @return
    This function does not return a value.

  @example

    This example demonstrates the use of the `TSE_init()` function to configure
    the CoreTSE with the default configuration.

    Note:
    A unique MAC address must always be assigned through the configuration data
    passed as a parameter to the `TSE_init()` function.

  @code
    #define TSEMAC_BASE  0x30000000
    tse_cfg_t g_tse_config;
    tse_instance_t g_tse;

    TSE_cfg_struct_def_init(&g_tse_config);

    g_tse_config.phy_addr = TSE_PHY_ADDRESS_AUTO_DETECT;
    g_tse_config.speed_duplex_select = TSE_ANEG_100M_FD;
    g_tse_config.mac_addr[0] = 0xC0;
    g_tse_config.mac_addr[1] = 0xB1;
    g_tse_config.mac_addr[2] = 0x3C;
    g_tse_config.mac_addr[3] = 0x60;
    g_tse_config.mac_addr[4] = 0x60;
    g_tse_config.mac_addr[5] = 0x60;

    TSE_init(&g_tse, TSEMAC_BASE, &g_tse_config);

  @endcode

 */

void TSE_init(tse_instance_t *this_tse, uint32_t base_addr, tse_cfg_t *cfg);

/**
    `TSE_set_tx_callback()` registers the function that the CoreTSE driver
    calls when a packet is sent.

    Note:
    This function is applicable only for CoreTSE_AHB.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param tx_complete_handler
    The tx_complete_handler parameter is a pointer to the call-back function
    that is called when the CoreTSE sends a packet.

  @return
    This function does not return a value.

  @example
    This example demonstrates the use of the `TSE_set_tx_callback()` function.
    The application registers the `tx_complete_callback()` transmit completion
    callback function with the CoreTSE driver by a call to `TSE_set_tx_callback()`.
    The application dynamically allocates memory for an application-defined packet_t
    data structure, builds a packet, and calls send_packet().
    `send_packet()` extracts the pointer to the buffer containing the
    data to transmit and its length from the tx_packet data structure and passes
    these to `TSE_send_pkt()`. It also passes the pointer to tx_packet as the
    p_user_data parameter. The CoreTSE driver calls `tx_complete_callback()` once
    the packet is sent. The `tx_complete_callback()` function uses the p_user_data,
    which points to tx_packet, to release memory allocated by the application
    to store the transmit packet.

  @code
    void tx_complete_callback(void * p_user_data);
    tse_instance_t g_tse;

    void init(void)
    {
        TSE_set_tx_callback((&g_tse, tx_complete_callback);
    }

    void tx_complete_callback (void * p_user_data)
    {
        release_packet_memory(p_user_data);
    }

    void send_packet(packet_t * tx_packet)
    {
        TSE_send_pkt(&g_tse, tx_packet->buffer, tx_packet->length, tx_packet);
    }
  @endcode
 */

void TSE_set_tx_callback(tse_instance_t *this_tse, tse_transmit_callback_t tx_complete_handler);

/**

    `TSE_set_rx_callback()` registers the function the CoreTSE driver calls when a
    packet is received.

    Note:
    This function is applicable only for CoreTSE_AHB.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param rx_callback
    The rx_callback parameter is a pointer to the function that is called
    when the packet is received by CoreTSE.

  @example
    The example below demonstrates the use of the `TSE_set_rx_callback()` function.
    The `init()` function calls the `TSE_set_rx_callback()` function to register
    the `rx_callback()` receive callback function with the CoreTSE driver. The
    `TSE_receive_pkt()` function is then called to assign the rx_buffer to an
    CoreTSE_AHB descriptor for packet reception.
    Once a packet is received into the rx_buffer, the CoreTSE driver calls
    the rx_callback function.
    The `rx_callback()` function calls the `process_rx_packet()` application
    function to process the received packet then calls `TSE_receive_pkt()` to
    reallocate rx_buffer to receive another packet.
    Every time a packet is received the `rx_callback()` function is called to process
    the received packet and reallocate rx_buffer for packet reception.

  @code
    uint8_t rx_buffer[TSE_MAX_RX_BUF_SIZE];

    void rx_callback
    (
        uint8_t * p_rx_packet,
        uint32_t pckt_length,
        void * p_user_data
    )
    {
        process_rx_packet(p_rx_packet, pckt_length);
        TSE_receive_pkt(rx_buffer, (void *)0);
    }

    void init(void)
    {
        TSE_set_rx_callback(rx_callback);
        TSE_receive_pkt(rx_buffer, (void *)0);
    }
  @endcode
 */

void TSE_set_rx_callback(tse_instance_t *this_tse, tse_receive_callback_t rx_callback);

/**

    The `TSE_set_wol_callback()` function registers the function that is
    called by the CoreTSE driver when Wake on LAN (WoL) feature is enabled and
    WoL event is detected. The WoL event happens when either a Unicast match frame
    or AMD magic packet frame is detected by CoreTSE. The wol_enable
    parameter in tse_cfg_t structure decides which type of packets can cause the
    WoL event.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param wol_callback
    The wol_callback parameter is a pointer to the function that is called
    when Wake on LAN (WoL) feature is enabled and WoL event happens.

  @return
    This function does not return a value.

  @example
    The example below demonstrates the use of the `TSE_set_wol_callback()` function.
    The `init()` function calls the `TSE_set_wol_callback()` function to register the
    wol_callback() receive callback function with the CoreTSE driver. The wol_callback
    function is called by the CoreTSE driver once a WoL event is detected.
    `wol_callback()` can implement the application-specific action on
    detecting the WoL event.

  @code
    tse_instance_t g_tse;

    void wol_callback
    (
        void * p_user_data
    )
    {
        //Process WoL interrupt here.
    }
    void init(void)
    {
        TSE_set_wol_callback(&g_tse, wol_callback);
    }
  @endcode
    */
void TSE_set_wol_callback(tse_instance_t *this_tse, tse_wol_callback_t wol_callback);

/**

    The `TSE_send_pkt()` function initiates the transmission of a packet. It places
    the buffer containing the packet to send into one of the CoreTSE_AHB's transmit
    descriptors. This function is non-blocking. It returns immediately
    without waiting for the packet to be sent. The CoreTSE driver indicates that
    the packet is sent by calling the transmit completion handler registered by
    a call to `TSE_set_tx_callback()`.

    Note:
    This function is applicable only for CoreTSE_AHB.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE_AHB instance controlled through this function
    call.

  @param tx_buffer
    The tx_buffer parameter is a pointer to the buffer containing the packet to
    send.

  @param tx_length
    The tx_length parameter specifies the length in bytes of the packet to send.

  @param p_user_data
    This parameter is a pointer to an optional, application-defined data
    structure. Its usage is left up to the application. It is intended to help the
    application manage memory allocated to store packets. The CoreTSE driver
    does not make use of this pointer. The CoreTSE driver passes back this
    pointer to the application as part of the call to the transmit completion
    handler registered by the application.

  @return
    This function returns 1 on successfully assigning the packet to a transmit
    descriptor. It returns 0 otherwise.

  @example
    This example demonstrates the use of the `TSE_send_pkt()` function.
    The application registers the `tx_complete_callback()` transmit completion
    callback function with the CoreTSE driver by a call to `TSE_set_tx_callback()`.
    The application dynamically allocates memory for an application-defined
    packet_t data structure builds a packet and calls send_packet().
    `send_packet()` extracts the pointer to the buffer containing the
    data to transmit and its length from the tx_packet data structure and passes
    these to `TSE_send_pkt()`.
    It also passes the pointer to tx_packet as the p_user_data parameter.
    The CoreTSE driver call `tx_complete_callback()` once the packet is sent.
    The `tx_complete_callback()` function uses the p_user_data, which points to
    tx_packet, to release memory allocated by the application to store the transmit
    packet.

  @code
    void tx_complete_callback(void * p_user_data);
    tse_instance_t g_tse;

    void init(void)
    {
        TSE_set_tx_callback((&g_tse, tx_complete_callback);
    }

    void tx_complete_callback (void * p_user_data)
    {
        release_packet_memory(p_user_data);
    }

    void send_packet(packet_t * tx_packet)
    {
        TSE_send_pkt(&g_tse, tx_packet->buffer, tx_packet->length, tx_packet);
    }
  @endcode

 */
uint8_t TSE_send_pkt(tse_instance_t *this_tse,
                     uint8_t const *tx_buffer,
                     uint32_t tx_length,
                     void *p_user_data);

/**

    `TSE_receive_pkt()` assigns a buffer to one of the CoreTSE_AHB
    receive descriptors. The receive buffer specified as a parameter receives one
    single packet. The receive buffer is handed back to the
    application via a call to the receive callback function, assigned through a
    call to `TSE_set_rx_callback()`. `TSE_receive_pkt()` needs to
    be called again pointing to the same buffer if more packets are to be
    received into this same buffer after the packet is processed by the
    application.

    `TSE_receive_pkt()` is non-blocking. It returns immediately
    and does not wait for a packet to be received. The application needs to
    implement a receive callback function to be notified that a packet is
    received.

    The p_user_data parameter can optionally be used to point to a memory
    management data structure managed by the application.

    Note:
    This function is applicable only for CoreTSE_AHB.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE_AHB instance controlled through this function
    call.

  @param rx_pkt_buffer
    The rx_pkt_buffer parameter is a pointer to a memory buffer. It points to
    the memory that is assigned to one of the CoreTSE_AHB's receive
    descriptors. It must point to a buffer large enough to contain the largest
    possible packet.

  @param p_user_data
    The p_user_data parameter is intended to help the application manage memory.
    Its usage is left up to the application. The CoreTSE driver does not make use
    of this pointer. The CoreTSE driver passes this pointer back to the
    application as part of the call to the application receive callback
    function to help the application associate the received packet with the
    memory it allocated before the call to `TSE_receive_pkt()`.

  @return
    This function returns 1 on successfully assigning the buffer to a receive
    descriptor. It returns 0 otherwise.

  @example
    The example below demonstrates the use of `TSE_receive_pkt()` to
    handle packet reception using two receive buffers. The `init()` function calls
    `TSE_set_rx_callback()` to register the `rx_callback()` receive
    callback function with the CoreTSE driver. `TSE_receive_pkt()`
    is then called twice to assign rx_buffer_1 and rx_buffer_2 to CoreTSE
    descriptors for packet reception. The rx_callback function is called
    by the CoreTSE driver once a packet is received into one of the
    receive buffers. The `rx_callback()` function calls the `process_rx_packet()`
    application function to process the received packet then calls
    `TSE_receive_pkt()` to reallocate the receive buffer to receive another packet.
    The `rx_callback()` function is called every time a packet is
    received to process the received packet and reallocate rx_buffer for packet
    reception.

    Note:
    The use of the p_user_data parameter to handle the buffer reassignment to the
    CoreTSE_AHB as part of the `rx_callback()` function.
    This is a simplistic use of p_user_data.
    It is more likely that p_user_data would be useful to keep track of a
    pointer to a TCP/IP stack packet container data structure dynamically allocated.
    In this more complex use case, the first parameter of `TSE_receive_pkt()` would
    point to the actual receive buffer and the second parameter would point to a
    data structure used to free the receive buffer memory once the packet has
    been consumed by the TCP/IP stack.

  @code
    uint8_t rx_buffer_1[TSE_MAX_RX_BUF_SIZE];
    uint8_t rx_buffer_2[TSE_MAX_RX_BUF_SIZE];
    tse_instance_t g_tse;

    void rx_callback
    (
        uint8_t * p_rx_packet,
        uint32_t pckt_length,
        void * p_user_data
    )
    {
        process_rx_packet(p_rx_packet, pckt_length);
        TSE_receive_pkt((&g_tse, (uint8_t *)p_user_data, p_user_data);
    }

    void init(void)
    {
        TSE_set_rx_callback((&g_tse, rx_callback);
        TSE_receive_pkt(rx_buffer_1, (void *)rx_buffer_1);
        TSE_receive_pkt(rx_buffer_2, (void *)rx_buffer_2);
    }
  @endcode

 */
uint8_t TSE_receive_pkt(tse_instance_t *this_tse, uint8_t *rx_pkt_buffer, void *p_user_data);

/**

    `TSE_get_link_status()` retrieves the status of the link from
    the Ethernet PHY. It returns the current state of the Ethernet link.
    The speed and duplex mode of the link are also returned via the two pointers
    passed as parameters - if the link is up.

    This function also adjusts the CoreTSE's internal configuration
    if some of the link characteristics have changed since the previous call to
    this function.
    As such, it is recommended to call this funciton periodically, so that the driver
    can automatically adapt to changes in the network link status.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param speed
    The speed parameter is a pointer to a variable of type tse_speed_t where the
    current link speed is stored if the link is up. This variable is not
    updated if the link is down. This parameter can be set to zero if the caller
    does not need to find out the link speed.

  @param fullduplex
    The full-duplex parameter is a pointer to an unsigned character where the
    current link duplex mode is stored if the link is up. This variable is
    not updated if the link is down.

  @return
    This function returns 1 if the link is up. It returns 0 if the link is down.

  @example
  @code
    uint8_t link_up;
    tse_speed_t speed;
    uint8_t full_duplex
    tse_instance_t g_tse;

    link_up = TSE_get_link_status(&g_tse, &speed, &full_duplex);
  @endcode

 */
uint8_t TSE_get_link_status(tse_instance_t *this_tse, tse_speed_t *speed, uint8_t *fullduplex);

/**

   `TSE_read_stat()` reads the transmit and receive statistics of
    the CoreTSE. This function can be used to read one of seventeen receiver
    statistics, twenty transmitter statistics and seven frame-type statistics as defined
    in the tse_stat_t enumeration.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param stat
    This parameter of type tse_stat_t identifies the statistic that is read.
    Refer to the tse_stat_t type definition to see the allowed values.

  @return
    This function returns the value of the requested statistic.

  @example
  @code
    uint32_t tx_pkts_cnt = 0;
    tse_instance_t g_tse;

    tx_pkts_cnt = TSE_read_stat(&g_tse, TSE_TX_PKT_CNT);

  @endcode
  */
uint32_t TSE_read_stat(tse_instance_t *this_tse, tse_stat_t stat);

/**

    `TSE_clear_statistics()` clears all forty-four statistics counter
    registers.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @return
    This function does not return a value.

  @example
  @code
    uint32_t tx_pkts_cnt = 0;

    tx_pkts_cnt = TSE_clear_statistics(&g_tse);

  @endcode
 */
void TSE_clear_statistics(tse_instance_t *instance);

/**
    `TSE_read_phy_reg()` reads the Ethernet PHY register specified as
    a parameter. It uses the MII management interface to communicate with the
    Ethernet PHY. This function is part of the Ethernet PHY drivers provided
    alongside the CoreTSE driver. You only need to use this function if writing
    your own Ethernet PHY driver.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param phyaddr
    The phyaddr parameter is the 5-bit address of the Ethernet PHY on the MII
    management interface. This address is typically defined through Ethernet PHY
    hardware configuration signals. Please refer to the Ethernet PHY's datasheet
    for details of how this address is assigned.

  @param regaddr
    The regaddr parameter is the address of the Ethernet register that is
    read. This address is the offset of the register within the Ethernet PHY's
    register map.

  @return
    This function returns the 16-bit value of the requested register.

  @example
  @code
    #include -phy.h-
    tse_instance_t g_tse;

    uint16_t read_phy_status(uint8_t phy_addr)
    {
        uint16_t phy_status = TSE_read_phy_reg(&g_tse, phy_addr , MII_BMSR);
        return phy_status;
    }
  @endcode

 */
uint16_t TSE_read_phy_reg(tse_instance_t *this_tse, uint8_t phyaddr, uint8_t regaddr);

/**

    `TSE_write_phy_reg()` writes a 16-bit value to the specified
    Ethernet PHY register. It uses the MII management interface to communicate
    with the Ethernet PHY. This function is part of the Ethernet PHY drivers
    provided alongside the CoreTSE driver. You only need to use this function if
    writing your own Ethernet PHY driver.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param phyaddr
    The phyaddr parameter is the 5-bit address of the Ethernet PHY on the MII
    management interface. This address is typically defined through Ethernet PHY
    hardware configuration signals. Please refer to the Ethernet PHY's datasheet
    for details of how this address is assigned.

  @param regaddr
    The regaddr parameter is the address of the Ethernet register that is
    written. This address is the offset of the register within the Ethernet
    PHY's register map.

  @param regval
    The regval parameter is the 16-bit value that is written into the
    specified PHY register.

  @return
    This function does not return a value.

  @example
  @code
    #include "phy.h"
    tse_instance_t g_tse;

    void rest_sgmii_phy(void)
    {
        TSE_write_phy_reg(&g_tse, SGMII_PHY_ADDR, MII_BMCR, 0x8000);
    }
  @endcode

 */
void TSE_write_phy_reg(tse_instance_t *this_tse, uint8_t phyaddr, uint8_t regaddr, uint16_t regval);

/**

    `TSE_isr()` is the top-level interrupt handler function for the
    CoreTSE driver. You must call `TSE_isr()` from the system-level
    (CoreInterrupt and NVIC level) interrupt handler assigned to the interrupt
    triggered by the CoreTSE INTR signal. Your system-level interrupt
    handler must also clear the system-level interrupt triggered by the
    CoreTSE INTR signal before returning, to prevent a re-assertion
    of the same interrupt.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @return
  This function does not return a value.

  @example
  @code

    tse_instance_t g_tse;

    void FabricIrq0_IRQHandler
    (
        void
    )
    {
        TSE_isr(&g_tse);
    }
  @endcode
 */
void TSE_isr(tse_instance_t *this_tse);

/**

    `TSE_set_address_filter()` implements the frame filtering
    functionality of the driver. This function specifies the list of
    destination MAC addresses of received frames that are passed to the MAC.
    This function takes an array of MAC addresses as a parameter and generates the
    correct hash table for that list of addresses. It also
    manages the setting of the Broadcast-DA, Multicast-DA/Unicast-DA hardware IP
    configuration bits based on the value of the MAC addresses passed as a
    parameter.
    For example, if the list contains one or more unicast addresses then
    unicast hash match filtering is enabled. Likewise, multicast hash match
    filtering is enabled if the list contains one or more multicast addresses.
    It enables broadcast filtering if the broadcast address is included in the
    allowed MAC addresses list passed as a parameter. It also enables passing
    frames addressed to the local MAC address (perfect unicast match) if the
    local MAC address is included in the allowed MAC addresses list.

    Note:
    The address filtering choice can also be selected using
    `TSE_cfg_struct_def_init()` function. The configuration value returned by this
    function can be modified and then passed on to the `TSE_init()` function. If
    The `TSE_set_address_filter()` function is used, the original filter setting
    chosen at the initialization time gets overwritten.

  @param this_tse
    The this_tse parameter is a pointer to the tse_instance_t structure holding
    all data regarding the CoreTSE instance controlled through this
    function call.

  @param mac_addresses
    The mac_addresses parameter is a pointer to the buffer containing the MAC
    addresses that are used to generate the MAC address hash table.

  @param nb_addresses
    The nb_addresses parameter specifies the number of MAC addresses being
    passed in the buffer pointed by the mac_addresses buffer pointer.

    Note:
    Each MAC address consists of six octets. Each MAC address must be placed in
    the buffer starting with the first octet of the MAC address.

    Note:
    Each MAC address consists of six octets and must be placed in the
    buffer starting with the first (most significant) octet of the MAC address.

  @return
    This function does not return a value.

  @example

    This example demonstrates the use of the `TSE_set_address_filter()` function to
    handle frame filtering. The list of MAC addresses passed to
    `TSE_set_address_filter()` function includes unicast, multicast, local base
    station and broadcast MAC addresses. The `TSE_set_address_filter()` function
    sets up the hash-unicast, hash-multicast, broadcast and perfect unicast match
    (local base station address) filtering to pass frames with these destination
    addresses to the MAC.

  @code
    #define TSEMAC_BASE  0x30000000
    tse_instance_t g_tse;
    tse_cfg_t g_tse_config;

    uint8_t mac_data[4][6] = {{0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
                            {0x43, 0x40, 0x40, 0x40, 0x40, 0x43},
                            {0xC0, 0xB1, 0x3C, 0x60, 0x60, 0x60},
                            {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

    g_tse_config.phy_addr = TSE_PHY_ADDRESS_AUTO_DETECT;
    g_tse_config.speed_duplex_select = TSE_ANEG_100M_FD;
    g_tse_config.mac_addr[0] = 0xC0;
    g_tse_config.mac_addr[1] = 0xB1;
    g_tse_config.mac_addr[2] = 0x3C;
    g_tse_config.mac_addr[3] = 0x60;
    g_tse_config.mac_addr[4] = 0x60;
    g_tse_config.mac_addr[5] = 0x60;

    TSE_init(&g_tse, TSEMAC_BASE, &g_tse_config);

    TSE_set_address_filter(&g_tse, mac_data[0], 4);
  @endcode
 */
void TSE_set_address_filter(tse_instance_t *this_tse,
                            const uint8_t *mac_addresses,
                            uint32_t nb_addresses);

#ifdef __cplusplus
}
#endif

#endif /* CORE_TSE_H_ */
