/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core1588.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core1588 header file
 *
 */

/*=========================================================================*//**
  @mainpage Core1588 Bare Metal Driver.

==============================================================================
  Introduction
==============================================================================

  Core1588 is an IP component designed to provide hardware timestamping for
  supporting the IEEE® 1588 v2.0 Precision Time Protocol (PTP). The core
  parses PTP frames and records their timestamp for use in protocol
  calculations. This core also provides Real Time Counter (RTC) functionality,
  including latches and triggers.

  This driver provides a set of functions for controlling Core1588 as part of
  the bare metal system where no operating system is available. This driver can
  be adapted to be used as a part of an operating system, but the
  implementation of the adaptation layer between the driver and the operating
  systems driver model is outside the scope of this driver.

  ==============================================================================
  Driver Configuration
  ==============================================================================
  Your application software must configure the Core1588 driver by calling the
  core1588_init() and core1588_configure() functions for each Core1588 instance in
  the hardware design. core1588_init() function configures the Core1588 hardware
  instance base address and initialises all buffers. The core1588_init() function
  must be called before any other Core1588 driver functions can be called. The
  Core1588 should then be configured using the core1588_configure() API. An input
  parameter of c1588_cfg_t type structure provides the complete configuration
  information of the Core1588. This structure should first be initialised with
  the core1588_cfg_struct_def_init() API to avoid any unintended configuration
  changes. In this structure users can customize the configuration register
  mask, initial RTC time and RTC increment amount. The increment amount can be
  specified in two ways, either by  providing the frequency of the PTP clock,
  or by providing the desired step size in nanosenconds and subnanoseconds. If
  both options are provided in the configuration structure, the frequency value
  will be used to set the increment amount.

  ==============================================================================
  Theory of Operation
  ==============================================================================
  The Core1588 software driver is designed to control the multiple instances of
  Core1588 IP. Each instance of Core1588 in the hardware design is associated
  with a single instance of the c1588_instance_t structure in the software. The
  contents of these data structures are initialized by calling the core1588_init()
  and core1588_configure() functions. A pointer to the structure is passed to the
  subsequent driver functions to identify the Core1588 hardware instance on
  which you wish to perform the requested operation.

  Note 1: Do not attempt to directly manipulate the content of c1588_instance_t
          structures. This structure is only intended to be modified by the
          driver functions.

  Core1588 can be divided into two main areas of functionality:
    - Real Time Counter (RTC)
    - PTP timestamping

  ------------------------
  Real Time Counter (RTC)
  ------------------------
  The RTC is responsible for providing the clock for the Core1588. If
  configured, it continues to increment on every clock tick by a fixed
  amount. The current time stored in the RTC is available to read at any time.

  To setup and enable the RTC, use the core1588_configure() function. An initial
  time and increment amount can be provided to this function in the passed
  configuration structure to configure the counter. If no increment amount is
  provided, it will remain at the initially set time. The RTC time can be set
  or received at any time using the core1588_rtc_set_time() or
  core1588_rtc_get_time() functions, respectively. Enable the C1588_RTCSEC_IRQ
  interrupt to generate an interrupt each time the RTC increments the seconds
  counter.

  The current RTC increment amount is stored in a c1588_instance_t type
  structure. The increment amount can be modified in two ways. Assign the RTC
  with an entirely new increment value using core1588_rtc_set_increment(),
  which updates the RTC registers and the instance structure with the provided
  new increment value. Alternatively, the increment amount can be adjusted
  using core1588_rtc_adjfreq(), which is passed a parts-per-billion (ppb) value
  to adjust the current increment values stored in the RTC registers and
  configuration structure.

  As well as constant increments per tick, the RTC time can be modified by
  one-off adjustments. This is done using the core1588_rtc_adjtime() function.
  This function takes a signed value to adjust the current time in the RTC.

  Latch and trigger functionality is also provided by the Core1588 RTC. The
  core contains three latches (0, 1 and 2) and three triggers (0, 1 and 2).
  These features must be configured in the Libero® design before getting used
  by the driver. Latches are enabled using core1588_latch_enable() and capture
  the current RTC time on receipt of a high signal from its corresponding
  input pin. The captured time can be retrieved with
  core1588_latch_get_timestamp(). core1588_latch_default_handler() in the
  Core1588 interrupt handler manages the latched timestamps automatically.
  The timestamps are stored in a ring buffer with the ID of the source latch.
  Call core1588_latch_get_from_buffer() to retrieve the oldest latch
  information from the buffer. Similarly, triggers are enabled using
  core1588_trigger_enable(). A trigger raises an interrupt once the RTC
  reaches a time programmed into the triggers registers. This trigger time
  can be set using core1588_trigger_set_timestamp().
  core1588_trigger_default_handler() in the Core1588 interrupt handler
  manages the triggered timestamps automatically. The timestamps are stored in
  a ring buffer with the ID of the source trigger. Call
  core1588_latch_get_from_buffer() function to retrieve the oldest trigger
  information from the buffer.

  -----------------
  PTP Timestamping
  -----------------
  Timestamping of PTP packets is enabled within the core during configuration.
  On receipt or transmission of specific PTP packets the core records the
  current RTC time in a set of registers for the driver to receive and place
  in a buffer. Enabling/disabling interrupt options filters PTP message based
  on the PTP message types of interest. No function calls are necessary to
  take a snapshot of the Rx/Tx timestamp, only to read the timestamp from a
  buffer once it has already been saved.

  core1588_isr() handles all interrupts related to the Core1588. This interrupt
  handler should be called by the appropriate interrupt handler provided by the
  processor HAL layer. For example, interrupt handlers provided by MIV_RV32 HAL,
  which will vary depending on how the Core1588 interrupts are hooked to the
  processor in your design. The interrupt types handled by the interrupt
  handler will depend on the interrupts enabled by the user. Depending on the
  interrupt that has been detected, core1588_isr() may call a different interrupt
  handler. There should be no need to call these individual interrupt handlers
  as a user, this API should handle it instead. For example,
  core1588_ptp_rx_default_handler() does not need to be called in the
  application code to handle a received message. core1588_isr() should be
  triggered instead, and will in turn call the Rx message handler itself.

  If a user wishes to include their own interrupt handling APIs, use the
  core1588_interrupt.c file. This file contains APIs for Tx and Rx messages,
  as well as for select RTC functionality (such as latches, triggers and second
  counters). Users can use these APIs to implement their own handlers, which
  will be called after the associated default handler has returned. To call a
  user handler, it must be enabled in core1588_user_config.h file. If a user
  handler is enabled but not implemented, the application will halt upon
  calling the user handler.

  Received SYNC, DELAY_REQUEST, PEERDELAY_REQUEST and PEERDELAY_RESPONSE
  messages trigger the Core1588 to save the RTC time at the time of receipt.
  The source port ID and sequence ID contained within these packets are also
  saved. The interrupt handler calls the core1588_ptp_rx_default_handler() to
  read and package this data and store it in a ring buffer for use by the user
  application. To retrieve information from the Rx ring buffer, call
  core1588_ptp_rx_get_from_buffer() and pass the sequence ID associated with
  the message and timestamp of interest. Rx packet information gets stored only
  for a given packet type if its corresponding interrupt is enabled. For
  example, Rx SYNC messages will only be timestamped if C1588_RXSYNC_IRQ is
  enabled.

  Transmitted SYNC, DELAY_REQUEST, PEERDELAY_REQUEST and PEERDELAY_RESPONSE
  messages trigger the Core1588 to save the RTC time at the time of
  transmission. The sequence ID contained within these packets is also saved.
  The interrupt handler calls the core1588_ptp_tx_default_handler() to read and
  package this data and store it in a ring buffer for use by the user
  application. To retrieve information from the Tx ring buffer, call
  core1588_ptp_tx_get_from_buffer() and pass the sequence ID associated with
  the message and timestamp of interest. Tx packet information gets stored only
  for a given packet type if its corresponding interrupt is enabled. For
  example, Tx SYNC messages will only be timestamped if C1588_TXSYNC_IRQ is
  enabled.

  If C1588_ONE_STEP_SYNC_MODE is enabled during Core1588 configuration, the
  core will insert the current RTC timestamp into any outgoing SYNC messages,
  replacing the timestamp inserted in software. The Tx timestamp is also stored
  in the Tx registers as normal, and gets placed in a buffer for use in
  applications. If C1588_ONE_STEP_SYNC_MODE is not enabled, the outgoing SYNC
  messages will retain the original timestamps placed in them by the software.

  Further filtering of incoming PTP messages is achieved by using the unicast
  address filtering functionality of the Core1588. The core will ignore any
  incoming or outgoing PTP messages that do not match the unicast addresses
  that the core has been programmed with. To filter the messages, Rx and Tx
  unicast addresses are set using core1588_ptp_set_rx_unicast_addr() and
  core1588_ptp_set_rx_unicast_addr(), respectively.

*//*=========================================================================*/

#ifndef __CORE1588_H
#define __CORE1588_H 1

#include "core1588_user_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#include "hal_assert.h"
#endif

/*-------------------------------------------------------------------------*/ /**
  Interrupt Identifiers
  =======================================
  The following constants specify the interrupt identifier number, which is
  specifically used by the driver API. Each define corresponds to a different
  interrupt available in the core. IRQ_TYPE_MASK and IRQ_MASK constants are
  masks designed to filter Rx and Tx IRQ messages in handler APIs.

  | Constant               | Description                                      |
  |------------------------|--------------------------------------------------|
  | C1588_TTS_IRQ          | 1588 frame transmitted                           |
  | C1588_RTS_IRQ          | 1588 frame received                              |
  | C1588_TT0_IRQ          | Trigger 0 time reached                           |
  | C1588_TT1_IRQ          | Trigger 1 time reached                           |
  | C1588_TT2_IRQ          | Trigger 2 time reached                           |
  | C1588_LT0_IRQ          | Rising edge detected on Latch 0                  |
  | C1588_LT1_IRQ          | Rising edge detected on Latch 1                  |
  | C1588_LT2_IRQ          | Rising edge detected on Latch 2                  |
  | C1588_RTCSEC_IRQ       | RTC second counter incremented                   |
  | C1588_TXSYNC_IRQ       | PTP SYNC frame transmitted at GMII Tx            |
  | C1588_TXDELAYREQ_IRQ   | PTP DELAY REQUEST frame transmitted at GMII Tx   |
  | C1588_TXPDELAYREQ_IRQ  | PTP PDELAY REQUEST frame transmitted at GMII Tx  |
  | C1588_TXPDELAYRESP_IRQ | PTP PDELAY RESPONSE frame transmitted at GMII Tx |
  | C1588_RXSYNC_IRQ       | PTP SYNC frame received at GMII Rx               |
  | C1588_RXDELAYREQ_IRQ   | PTP DELAY REQUEST frame received at GMII Rx      |
  | C1588_RXPDELAYREQ_IRQ  | PTP PDELAY REQUEST frame received at GMII Rx     |
  | C1588_RXPDELAYRESP_IRQ | PTP PDELAY RESPONSE frame received at GMII Rx    |
  | C1588_TTSID_IRQ        | PTP sequenceID transmitted at GMII Tx            |
  | C1588_RTSID_IRQ        | PTP sequenceID received at GMII Rx               |
  | C1588_PEERTTSID_IRQ    | PTP peer sequenceID transmitted at GMII Tx       |
  | C1588_PEERRTSID_IRQ    | PTP peer sequenceID received at GMII Rx          |
  | C1588_RX_IRQ_TYPE_MASK | Mask for all Rx message type identifiers         |
  | C1588_RX_IRQ_MASK      | Mask for all Rx message identifiers              |
  | C1588_TX_IRQ_TYPE_MASK | Mask for all Tx message type identifiers         |
  | C1588_TX_IRQ_MASK      | Mask for all Tx message identifiers              |
  */
#define C1588_TTS_IRQ 0x00000001UL
#define C1588_RTS_IRQ 0x00000002UL
#define C1588_TT0_IRQ 0x00000004UL
#define C1588_TT1_IRQ 0x00000008UL
#define C1588_TT2_IRQ 0x00000010UL
#define C1588_LT0_IRQ 0x00000020UL
#define C1588_LT1_IRQ 0x00000040UL
#define C1588_LT2_IRQ 0x00000080UL
#define C1588_RTCSEC_IRQ 0x00000100UL
#define C1588_TXSYNC_IRQ 0x00000200UL
#define C1588_TXDELAYREQ_IRQ 0x00000400UL
#define C1588_TXPDELAYREQ_IRQ 0x00001000UL
#define C1588_TXPDELAYRESP_IRQ 0x00002000UL
#define C1588_RXSYNC_IRQ 0x00004000UL
#define C1588_RXDELAYREQ_IRQ 0x00008000UL
#define C1588_RXPDELAYREQ_IRQ 0x00020000UL
#define C1588_RXPDELAYRESP_IRQ 0x00040000UL
#define C1588_TTSID_IRQ 0x00080000UL
#define C1588_RTSID_IRQ 0x00100000UL
#define C1588_PEERTTSID_IRQ 0x00200000UL
#define C1588_PEERRTSID_IRQ 0x00400000UL
#define C1588_RX_IRQ_TYPE_MASK 0x0007c000UL
#define C1588_RX_IRQ_MASK 0x0057c002UL
#define C1588_TX_IRQ_TYPE_MASK 0x00003e00UL
#define C1588_TX_IRQ_MASK 0x00283e01UL
#define C1588_MASK_ALL_IRQ 0x007FFFFFUL

/*-------------------------------------------------------------------------*/ /**
  Message Identifiers
  =======================================
  The following constants specify the values used to identify each of the PTP
  message types in the driver. The Rx constants are used as identifiers in the
  driver for the various Rx message types. The Tx constants provide similar
  functionality for the Tx message types. The define values correspond to
  those seen in the IRQ identifiers to simplify driver functionality.

  | Constant           | Description                               |
  |--------------------|-------------------------------------------|
  | C1588_RXSYNC       | Rx Sync message identifier                |
  | C1588_RXDELAYREQ   | Rx Delay Request message identifier       |
  | C1588_RXPDELAYREQ  | Rx Peer Delay Request message identifier  |
  | C1588_RXPDELAYRESP | Rx Peer Delay Response message identifier |
  |                    |                                           |
  | C1588_TXSYNC       | Tx Sync message identifier                |
  | C1588_TXDELAYREQ   | Tx Delay Request message identifier       |
  | C1588_TXPDELAYREQ  | Tx Peer Delay Request message identifier  |
  | C1588_TXPDELAYRESP | Tx Peer Delay Response message identifier |
  */
#define C1588_RXSYNC 0x00004000UL
#define C1588_RXDELAYREQ 0x00008000UL
#define C1588_RXPDELAYREQ 0x00020000UL
#define C1588_RXPDELAYRESP 0x00040000UL

#define C1588_TXSYNC 0x00000200UL
#define C1588_TXDELAYREQ 0x00000400UL
#define C1588_TXPDELAYREQ 0x00001000UL
#define C1588_TXPDELAYRESP 0x00002000UL

/*-------------------------------------------------------------------------*/ /**
  Latch Identifiers
  ==================
  The following constants specify latch register address offsets and modifiers
  specific to each latch. C1588_LATCH_0, C1588_LATCH_1 and C1588_LATCH_2 are
  used to identify the three latches throughout the driver. Their defined
  values are aligned with their corresponding offsets in the interrupt
  register. LATCH_MASK_ALL is a mask for selecting only latch related bits from
  a register. LATCH_CFG_SHIFT is used with the configuration registers to shift
  the latch offsets.

  | Constant              | Description                     |
  |-----------------------|---------------------------------|
  | C1588_LATCH_0         | Latch 0 mask                    |
  | C1588_LATCH_1         | Latch 1 mask                    |
  | C1588_LATCH_2         | Latch 2 mask                    |
  | C1588_LATCH_MASK_ALL  | Latch 0, 1, 2 mask              |
  | C1588_LATCH_CFG_SHIFT | Shift for latch config register |
  */
#define C1588_LATCH_0 0x00000020UL
#define C1588_LATCH_1 0x00000040UL
#define C1588_LATCH_2 0x00000080UL
#define C1588_LATCH_MASK_ALL 0x000000E0UL
#define C1588_LATCH_CFG_SHIFT 0x00000008UL

/*-------------------------------------------------------------------------*/ /**
  Trigger Identifiers
  ====================
  The following constants specify trigger register address offsets and
  modifiers specific to each trigger. C1588_TRIGGER_0, C1588_TRIGGER_1 and
  C1588_TRIGGER_2 are used to identify the three triggers throughout the
  driver. Their defined values are aligned with their corresponding offsets in
  the interrupt register. TRIGGER_MASK_ALL is a mask for selecting only trigger
  related bits from a register. TRIGGER_CFG_SHIFT is used with the
  configuration registers to shift the trigger offsets.

  | Constant                | Description                       |
  |-------------------------|-----------------------------------|
  | C1588_TRIGGER_0         | Trigger 0 mask                    |
  | C1588_TRIGGER_1         | Trigger 1 mask                    |
  | C1588_TRIGGER_2         | Trigger 2 mask                    |
  | C1588_TRIGGER_MASK_ALL  | Trigger 0, 1, 2 mask              |
  | C1588_TRIGGER_CFG_SHIFT | Shift for trigger config register |
  */
#define C1588_TRIGGER_0 0x00000004UL
#define C1588_TRIGGER_1 0x00000008UL
#define C1588_TRIGGER_2 0x00000010UL
#define C1588_TRIGGER_MASK_ALL 0x0000001CUL
#define C1588_TRIGGER_CFG_SHIFT 0x00000008UL

/*-------------------------------------------------------------------------*/ /**
   Core1588 Configuration
   =======================
   The following constants specify the configuration options for the Core1588
   available in the general configuration register. The configurations should
   be provided as a logical OR to the configuration API. C1588_CORE_ENABLE
   enables the Core1588. C1588_ONE_STEP_SYNC_MODE enables one-step sync mode.
   The absence of this constant in a logical OR will disable one-step sync
   mode. C1588_RESPONDER_MODE and C1588_REQUESTOR_MODE place the Core1588 into
   responder mode or requestor mode, respectively. Only one of these should be
   provided to the configuration API at a time. C1588_PTP_UNICAST_ENABLE
   enables the PTP unicast address filtering in the Core1588.

   | Constant                 | Description                    |
   |--------------------------|--------------------------------|
   | C1588_CORE_ENABLE        | Enable Core1588 core           |
   | C1588_ONE_STEP_SYNC_MODE | Enable one-step sync mode      |
   | C1588_RESPONDER_MODE     | Put core in responder mode     |
   | C1588_REQUESTOR_MODE     | Put core in requestor more     |
   | C1588_PTP_UNICAST_ENABLE | Enable PTP unicast addressing  |
  */
#define C1588_CORE_ENABLE 0x00000001UL
#define C1588_ONE_STEP_SYNC_MODE 0x00000002UL
#define C1588_RESPONDER_MODE 0x00000000UL
#define C1588_REQUESTOR_MODE 0x00000008UL
#define C1588_PTP_UNICAST_ENABLE 0x00000010UL

/*-------------------------------------------------------------------------*/ /**
   Core1588 Control Parameters
   ============================
   The following constants specify the parameters used as inputs for the
   control APIs available in the Core1588 driver. These APIs include
   core1588_ptp_control(), core1588_one_step_sync_control(),
   core1588_irq_control(), core1588_latch_control() and
   core1588_trigger_control(). C1588_ENABLE is used to enable options within an
   API. C1588_DISABLE is used to disable options within an API.

   | Constant                 | Description                    |
   |--------------------------|--------------------------------|
   | C1588_ENABLE             | Core1588 Control Enable        |
   | C1588_DISABLE            | Core1588 Control Disable       |
  */
#define C1588_ENABLE 0x00000001UL
#define C1588_DISABLE 0x00000000UL

/*-------------------------------------------------------------------------*/ /**
   RTC Increment Macros
   =======================
   The following constants are used as helper macros to set the RTC increment
   amount relative to the designs clock. The standard API for setting the
   increment size takes nsecs and subnsecs as inputs to set the RTC period.
   These macros can be used with an alternate version of the API to calculate
   the RTC period from the RTC frequency provided.
   */
#define NS_IN_A_SEC (1000000000UL)
#define SUB_NS_IN_A_NS (16777216ULL)
#define SUB_NS_IN_A_SEC (SUB_NS_IN_A_NS * NS_IN_A_SEC)

#define N_SEC_SYS_CLK_PERIOD_FROM_FREQ_HZ(sys_clk_freq)                        \
  (NS_IN_A_SEC / sys_clk_freq)

#define SUB_NS_FROM_SYS_CLK_FREQ_HZ(sys_clk_freq)                              \
  ((SUB_NS_IN_A_SEC / sys_clk_freq) -                                          \
   (N_SEC_SYS_CLK_PERIOD_FROM_FREQ_HZ(sys_clk_freq) * SUB_NS_IN_A_NS))

/**
 * The c1588_status enum type is used to return the status of Core1588
 * functionality.
 */
typedef enum c1588_status { C1588_SUCCESS = 0u, C1588_FAILURE } c1588_status_t;

/**
 * The c1588_timestamp structure type is used to store timestamps within the
 * Core1588 driver. This includes RTC times or those captured on receipt/
 * transmission of PTP packets. The core stores timestamps with a resolution
 * of 48-bits for seconds, 32-bits for nanoseconds and 16-bits for
 * sub-nanoseconds.
 */
typedef struct c1588_timestamp {
  uint64_t secs;
  uint32_t nsecs;
  uint32_t subnsecs;
} c1588_timestamp_t;

/**
 * The c1588_rtc_event_timestamp structure type stores the timestamps read from
 * latches or triggers upon interrupt. It contains a c1588_timestamp to store
 * the event time and also stores the ID of the latch or trigger in the id
 * variable.
 */
typedef struct c1588_rtc_event_timestamp {
  c1588_timestamp_t ts;
  uint32_t id;
} c1588_rtc_event_timestamp_t;

/**
 * The c1588_ptp_packet_info structure type combines several PTP packet
 * information structs into one to store all required information from a PTP
 * event.
 *
 *  - type: Indicates the kind of PTP packet the information belongs to as well
 * as the direction of travel (Tx/Rx)
 *  - ts: Stores the packet timestamp
 *  - seq_id: Contains the packet sequence ID
 *  - src_port_id: Contains the source port ID of the PTP message if the
 * packet is an Rx packet, otherwise it will be empty
 */
typedef struct c1588_ptp_packet_info {
  uint32_t type;
  c1588_timestamp_t ts;
  uint16_t seq_id;
  uint8_t src_port_id[10];
} c1588_ptp_packet_info_t;

/**
 * The c1588_time_incr structure type stores the RTC timer increment values.
 * This is used for reading, setting or modifying the RTC increment amount
 * using step-size/period.
 *
 *  - incr_ns: An 8-bit value for the nanosecond increment size.
 *  - incr_subns: A 24-bit value for the subnanosecond increment size.
 */
typedef struct c1588_time_incr {
  uint32_t incr_subns;
  uint8_t incr_ns;
} c1588_time_incr_t;

/**
 * The c1588_cfg structure type contains configuration information for an
 * instance of the Core1588. It contains the initial configuration settings for
 * the Core1588. The configuration settings for the device should be described
 * here at the beginning of an application and must be set using the
 * core1588_configure API.
 *
 *  - config_mask: Mask of configuration options used with the global
 * configuration register
 *  - initial_time: Initial time to which the RTC is set
 *  - initial_rtc_incr: Initial RTC increment amount specified in step size
 *  - rtc_freq: Clock frequency of the PTP_CLK clock connected to the Core1588
 * provided in Hz
 *
 * If rtc_freq is a non-zero value, the RTC clock increment size will be
 * calculated from the provided clock frequency. Otherwise, the increment size
 * will be set from the initial_rtc_incr values.
 */
typedef struct c1588_cfg {
  uint32_t config_mask;
  c1588_timestamp_t initial_time;
  c1588_time_incr_t initial_rtc_incr;
  uint32_t rtc_freq;
} c1588_cfg_t;

/**
 * The c1588_instance structure type identifies individual Core1588 hardware
 * instances within your system. The core1588_init() function initializes this
 * structure. A pointer to an initialized instance of the structure should be
 * provided as the first parameter to all Core1588 driver functions to identify
 * which Core1588 should perform the requested operation.
 *
 *  - base_address: Hardware address of the Core1588 instance
 *  - rtc_incr: Current RTC increment values
 *
 * The instance structure tracks all buffers in the driver. There are buffers
 * for Rx messages, Tx messages, trigger events and latch events. The Rx and Tx
 * buffers store c1588_ptp_packet_info structs to track PTP message
 * information. The trigger and latch buffers store c1588_rtc_event_timestamp
 * structs. The buffers follow a x_buf[] naming structure. Each buffer has an
 * associated x_write_idx and x_read_idx element to track the current
 * status of the buffer. These indices are updated by buffer read and write
 * APIs. The Tx and Rx buffers also have an associated x_buf_read[] array
 * used to track the array elements tha have already been read.
 */
typedef struct c1588_instance {
  addr_t base_address;
  c1588_time_incr_t rtc_incr;
  c1588_ptp_packet_info_t rx_buf[C1588_RX_RING_SIZE];
  uint8_t rx_buf_read[C1588_RX_RING_SIZE];
  c1588_ptp_packet_info_t tx_buf[C1588_TX_RING_SIZE];
  uint8_t tx_buf_read[C1588_TX_RING_SIZE];
  c1588_rtc_event_timestamp_t latch_buf[C1588_LATCH_RING_SIZE];
  c1588_rtc_event_timestamp_t trigger_buf[C1588_TRIGGER_RING_SIZE];
  uint16_t rx_write_idx;
  uint16_t rx_read_idx;
  uint16_t tx_write_idx;
  uint16_t tx_read_idx;
  uint16_t latch_write_idx;
  uint16_t latch_read_idx;
  uint16_t trigger_write_idx;
  uint16_t trigger_read_idx;
} c1588_instance_t;

/**
  `core1588_cfg_struct_def_init()` initializes a c1588_cfg_t configuration data
  structure to default values. A configuration structure should be initialized
  before it is used to avoid any unintended behaviour.

  @param cfg
  The cfg parameter is a pointer to the c1588_cfg_t data structure used as a
  parameter for the `core1588_configure()` function.

  @return
  This function does not return a value.

  @example
  @code
    core1588_cfg_struct_def_init(&c1588_cfg_t);
  @endcode
 */
void core1588_cfg_struct_def_init(c1588_cfg_t *cfg);

/**
  `core1588_init()` initializes the driver. This function sets the base address
  for the Core1588 instance and prepare all buffers in the driver for use. This
  function must be called before calling any other driver functions.

  @param this_c1588
  The this_c1588 parameter is a pointer to the c1588_instance_t structure that
  holds all the data related to the Core1588 instance being initialized. A
  pointer to this data structure is used in all subsequent calls to the
  Core1588 driver functions that operate on this Core1588 instance.

  @param base_addr
  addr_t base address for this_c1588.

  @return
  This function does not return a value.

  @example
  @code
    core1588_init(this_c1588, base_addr);
  @endcode
 */
void core1588_init(c1588_instance_t *this_c1588, addr_t base_addr);

/**
  The core1588_configure() function configures the driver. This function sets
  the Core1588 configurations in the general configuration register, RTC
  initial time registers and RTC increment amount registers. The configuration
  options should be provided as a mask created using a logical OR of the
  Core1588 configuration constants. Initial time should be provided as a
  c1588_timestamp_t structure. Increment amount can be provided as a
  c1588_time_incr structure or as a uint32_t rtc_freq value provided in Hz.
  If rtc_freq is non-zero, the driver will use this to set the RTC increment
  size. Otherwise, it will use rtc_incr for setting the step size. All
  configuration parameters will be provided through the c1588_cfg_t
  structure.

  @param this_c1588
  The this_c1588 parameter is a pointer to the c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param cfg
  c1588_cfg_t pointer to a data structure that contains a mask of configuration
  options, an initial time for the RTC and timer increment amount (through
  step-size or frequency). The possible configs for the configuration mask are:
    - C1588_CORE_ENABLE
    - C1588_ONE_STEP_SYNC_MODE
    - C1588_RESPONDER_MODE
    - C1588_REQUESTOR_MODE
    - C1588_PTP_UNICAST_ENABLE

  @return
  This function does not return a value.

  @example
  @code
    c1588_cfg_t cfg;
    core1588_cfg_struct_def_init(&cfg);
    cfg.config_mask = C1588_CORE_ENABLE;
    cfg.initial_time.secs = (uint64_t)0x0u;
    cfg.initial_time.nsecs = 0x0u;
    cfg.rtc_freq = 83330000u;
    core1588_configure(this_c1588, &cfg);
  @endcode
 */
void core1588_configure(c1588_instance_t *this_c1588, c1588_cfg_t *cfg);

/**
  core1588_ptp_control() controls the Core1588 and its PTP timestamping
  functionality. It is an alternative way to control the initial configuration.
  Passing C1588_ENABLE as a parameter will enable this functionality, while
  C1588_DISABLE will disable this functionality.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param control
  uint32_t parameter which selects whether the API enables or disables its
  respective functionality.

  @return
  This function does not return a value.

  @example
  @code
    core1588_ptp_control(this_c1588, C1588_ENABLE);
  @endcode
 */
void core1588_ptp_control(c1588_instance_t *this_c1588, uint32_t control);

/**
  core1588_one_step_sync_control() controls the Core1588 one-step sync message
  mode. It is an alternative way to control the initial configuration. Passing
  C1588_ENABLE as a parameter will enable this functionality, while
  C1588_DISABLE will disable this functionality.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param control
  uint32_t parameter which selects whether the API enables or disables its
  respective functionality.

  @return
  This function does not return a value.

  @example
  @code
    core1588_one_step_sync_control(this_c1588, C1588_ENABLE);
  @endcode
 */
void core1588_one_step_sync_control(c1588_instance_t *this_c1588,
                                    uint32_t control);

/**
  core1588_get_enabled_irq() returns a mask of the currently enabled IRQ in
  the Core1588 Interrupt Enable Register.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @return
  32-bit mask of enabled interrupts.

  @example
  @code
    uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
  @endcode
 */
uint32_t core1588_get_enabled_irq(c1588_instance_t *this_c1588);

/**
  core1588_irq_control() controls interrupts. The irq_mask input is a 32-bit
  mask that selects which interrupts to control. Passing C1588_ENABLE as a
  parameter will enable any interrupts included in irq_mask, while passing
  C1588_DISABLE will disable any interrupts included in irq_mask.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param irq_mask
  uint32_t mask of interrupts to control. The possible interrupts are:
    - C1588_TTS_IRQ
    - C1588_RTS_IRQ
    - C1588_TT0_IRQ
    - C1588_TT1_IRQ
    - C1588_TT2_IRQ
    - C1588_LT0_IRQ
    - C1588_LT1_IRQ
    - C1588_LT2_IRQ
    - C1588_RTCSEC_IRQ
    - C1588_TXSYNC_IRQ
    - C1588_TXDELAYREQ_IRQ
    - C1588_TXPDELAYREQ_IRQ
    - C1588_TXPDELAYRESP_IRQ
    - C1588_RXSYNC_IRQ
    - C1588_RXDELAYREQ_IRQ
    - C1588_RXPDELAYREQ_IRQ
    - C1588_RXPDELAYRESP_IRQ
    - C1588_TTSID_IRQ
    - C1588_RTSID_IRQ
    - C1588_PEERTTSID_IRQ
    - C1588_PEERRTSID_IRQ

  @param control
  uint32_t parameter which selects whether the API enables or disables the IRQs
  provided in irq_mask.

  @param control
  uint32_t parameter which selects whether the API enables or disables the
  provided interrupts.

  @return
  This function does not return a value.

  @example
  @code
    uint32_t irq_mask = C1588_RTS_IRQ | C1588_RXSYNC_IRQ;
    core1588_irq_control(this_c1588, irq_mask, C1588_ENABLE);
  @endcode
  */
void core1588_irq_control(c1588_instance_t *this_c1588, uint32_t irq_mask,
                          uint32_t control);

/**
  core1588_get_irq_src() returns the value of the masked interrupt register.
  Any triggered interrupts will have their corresponding bits set to 1.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @return
  32-bit mask of triggered interrupts.

  @example
  @code
    uint32_t irq_mask = core1588_get_irq_src(this_c1588);
  @endcode
 */
uint32_t core1588_get_irq_src(c1588_instance_t *this_c1588);

/**
  core1588_clear_irq_src() clears masked interrupt register bits. The
  irq_mask input is a 32-bit mask of interrupts to clear.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param irq_mask
  uint32_t mask of interrupts to clear.

  @return
  This function does not return a value.

  @example
  @code
    core1588_clear_irq_src(this_c1588, irq_clear_mask);
  @endcode
 */
void core1588_clear_irq_src(c1588_instance_t *this_c1588, uint32_t irq_mask);

/**
  core1588_isr() handles Core1588 interrupts. Individual interrupt handlers are
  called depending on the type of interrupt that has triggered. Default
  handlers are always called where applicable. User handlers can be called if
  they are enabled in core2588_user_config.h. Once enabled the user handler
  must be implemented, otherwise the application will halt upon calling the
  handler.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @return
  This function does not return a value.
 */
void core1588_isr(c1588_instance_t *this_c1588);

/**
  core1588_rtc_set_time() sets the current time of the RTC. The time to write
  to the RTC is provided as a c1588_timestamp_t structure through the ts API
  parameter.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param ts
  The ts parameter is a pointer to a c1588_timestamp_t structure containing
  the new RTC timestamp value to set.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_timestamp_t timestamp;
    timestamp.secs = (uint64_t)0x1800000u;
    timestamp.nsecs = 0x3200u;
    core1588_rtc_set_time(this_c1588, &timestamp);
  @endcode
 */
c1588_status_t core1588_rtc_set_time(c1588_instance_t *this_c1588,
                                     c1588_timestamp_t *ts);

/**
  core1588_rtc_get_time() reads the current time of the RTC and stores the
  value as a c1588_timestamp_t at the address provided by the ts API
  parameter.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param ts
  The ts parameter is a pointer to a c1588_timestamp_t structure, which stores
  the RTC timestamp.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_timestamp_t timestamp;
    core1588_rtc_get_time(this_c1588, &timestamp);
  @endcode
 */
c1588_status_t core1588_rtc_get_time(c1588_instance_t *this_c1588,
                                     c1588_timestamp_t *ts);

/**
  core1588_rtc_set_increment() sets the RTC step-size values. Increment
  values are provided as a c1588_time_incr structure through the incr API
  parameter. This API parameter allows the increment to be set by specifying the
  period. The latest increment sizes are stored in the c1588_instance_t
  structure.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param incr
  c1588_time_incr specifies the increment amount.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_time_incr_t incr;
    incr.incr_ns = 0x100u;
    incr.incr_subns = 0x800u;
    core1588_rtc_set_increment(this_c1588, &incr);
  @endcode
 */
c1588_status_t core1588_rtc_set_increment(c1588_instance_t *this_c1588,
                                          c1588_time_incr_t *incr);

/**
  core1588_rtc_set_increment_freq() sets the RTC step-size values. The value
  is calculated from the frequency value freq passed to the API. The
  frequency should be provided in Hz and should match the frequency of the
  PTP_CLK clock input connected to the Core1588 hardware block. The latest
  increment sizes are stored in the c1588_instance_t structure after they
  are calculated from the provided frequency.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param freq
  uint32_t specifies the frequency in Hz.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    uint32_t freq = 125000000u;
    core1588_rtc_set_increment_freq(this_c1588, freq);
  @endcode
 */
c1588_status_t core1588_rtc_set_increment_freq(c1588_instance_t *this_c1588,
                                               uint32_t freq);

/**
  core1588_rtc_get_increment() gets the current increment value of the RTC.
  The RTC increment value is returned as a c1588_time_incr_t structure and is
  written to the address given by the incr parameter.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param incr
  c1588_time_incr specifies the increment amount.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_time_incr_t incr;
    core1588_rtc_get_increment(this_c1588, &incr);
  @endcode
 */
c1588_status_t core1588_rtc_get_increment(c1588_instance_t *this_c1588,
                                          c1588_time_incr_t *incr);

/**
  core1588_rtc_adjfreq() adjusts the increment values of the RTC. freq_adj
  contains the amount to adjust the increment value by in parts per billion
  (ppb).

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param freq_adj
  int32_t specifies the frequency adjustment in ppb.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    int32_t freq_adj = -80;
    core1588_rtc_adjfreq(this_c1588, freq_adj);
  @endcode
 */
c1588_status_t core1588_rtc_adjfreq(c1588_instance_t *this_c1588,
                                    int32_t freq_adj);

/**
  core1588_rtc_adjtime() adjusts the current time of the RTC. adj_val is
  the one-off adjustment amount needed to change the RTC.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param adj_val
  30-bit signed adjustment value.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    int32_t adj_val = 123;
    core1588_rtc_adjtime(this_c1588, adj_val);
  @endcode
 */
c1588_status_t core1588_rtc_adjtime(c1588_instance_t *this_c1588,
                                    int32_t adj_val);

/**
  core1588_ptp_get_rxstamp() reads the sampled Rx RTC timestamp from the Rx
  timestamp registers. The RTC is sampled in response to the appearance of an
  Rx PTP packet and stored in the appropriate registers. The API reads the
  timestamp as a c1588_timestamp_t structure and stores the value at the
  address provided by the ts API parameter. rx_type denotes the type of
  message received by the Core1588 and is used by the API to determine the
  register from which the timestamp is read.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param ts
  The ts parameter is a pointer to a c1588_timestamp_t structure where the
  sampled Rx RTC timestamp is stored.

  @param rx_type
  uint32_t mask of interrupt bits that denotes the Rx package type.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_timestamp_t timestamp;
    uint32_t rx_type = C1588_RXSYNC;
    core1588_ptp_get_rxstamp(this_c1588, &timestamp, rx_type);
  @endcode
 */
c1588_status_t core1588_ptp_get_rxstamp(c1588_instance_t *this_c1588,
                                        c1588_timestamp_t *ts,
                                        uint32_t rx_type);

/**
  core1588_ptp_get_rx_seq_id() gets the Rx timestamp sequence ID. The value
  is stored in the c1588_ptp_packet_info_t structure at the address provided
  by the pkt_info API parameter. rx_type denotes the type of message
  received by the Core1588 and is used by the API to determine which register to
  read the sequence ID from.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param pkt_info
  c1588_ptp_packet_info_t is a pointer to the information structure which stores
  the sequence ID.

  @param rx_type
  uint32_t mask of interrupt bits that denotes the Rx package type.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_ptp_packet_info_t pkt_info;
    uint32_t rx_type = C1588_RXSYNC;
    core1588_ptp_get_rx_seq_id(this_c1588, &pkt_info, rx_type);
  @endcode
 */
c1588_status_t core1588_ptp_get_rx_seq_id(c1588_instance_t *this_c1588,
                                          c1588_ptp_packet_info_t *pkt_info,
                                          uint32_t rx_type);

/**
  core1588_ptp_get_rx_src_port_id() gets the Rx timestamp source port ID
  (80-bit wide value). The value is written to the id array passed to the
  API. The array should be a uint8_t array of length 10. rx_type denotes the
  type of message received by the Core1588 and is used by the API to determine
  which register set to read the source port ID from.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param id
  uint8_t specifies an array of length 10 to write the source port ID to.

  @param rx_type
  uint32_t mask of interrupt bits that denotes the Rx package type.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    uint8_t src_port[10];
    uint32_t rx_type = C1588_RXSYNC;
    core1588_ptp_get_rx_src_port_id(this_c1588, src_port, rx_type);
  @endcode
 */
c1588_status_t core1588_ptp_get_rx_src_port_id(c1588_instance_t *this_c1588,
                                               uint8_t id[10],
                                               uint32_t rx_type);

/**
  core1588_ptp_rx_default_handler() handles the gathering of Rx packet
  information on receiving a valid PTP packet. Timestamp, sequence ID, source
  port ID and message type are all recorded, and interrupt flags are cleared.
  The packet information is stored in a c1588_ptp_packet_info_t structure and is
  placed in the c1588_instance_t rx_buf ring buffer. rx_irq_mask should contain
  a mask of all of the Rx related interrupts in the masked interrupt register.
  This API is the default Rx interrupt handler and is called by core1588_isr().

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param rx_irq_mask
  uint32_t mask of interrupt bits that denotes the Rx IRQ type.

  @return
  This function does not return a value.
 */
void core1588_ptp_rx_default_handler(c1588_instance_t *this_c1588,
                                     uint32_t rx_irq_mask);

/**
  The core1588_ptp_rx_add_to_buffer() adds a received PTP messages information
  to the PTP Rx message ring buffer. The buffer stores C1588_RX_RING_SIZE - 1
  items, where C1588_RX_RING_SIZE is defined in core1588_user_config.h. PTP
  information is stored in the buffer as c1588_ptp_packet_info_t structs and
  passed to the API through the pkt_info API parameter. The API stores messages
  using FIFO and handles the management of ring buffer indices. If the buffer is
  full, the oldest buffer contents will be overwritten. This API will be called
  by the core1588_ptp_rx_default_handler() API.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param pkt_info
  The pkt_info paramater contains the Rx packet information to store in the
  Rx ring buffer.

  @return
  This function does not return a value.

  @example
  @code
    core1588_ptp_get_rx_src_port_id(this_c1588, pkt_info);
  @endcode
 */
void core1588_ptp_rx_add_to_buffer(c1588_instance_t *this_c1588,
                                   c1588_ptp_packet_info_t pkt_info);

/**
  core1588_ptp_rx_get_from_buffer() retrieves a received PTP messages
  information from the PTP Rx message ring buffer. The API copies the message
  contents to the address provided by the pkt_info parameter. The user must
  provide the sequence ID of the desired PTP message using the seq_id_rx
  parameter. The API then looks for a timestamp with a matching sequence ID in
  the buffer. If no timestamp can be found, the provided pointer will be
  assigned C1588_NULL_PACKET_INFO. The API will retrieve messages on a FIFO
  basis and handle the management of ring indices.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param pkt_info
  The pkt_info paramater is the pointer to which the desired packet information
  will be written.

  @param seq_id_rx
  The seq_id_rx parameter contains the sequence ID of the desired PTP message.

  @return
  c1588_status_t declares the success or failure.
 */
c1588_status_t
core1588_ptp_rx_get_from_buffer(c1588_instance_t *this_c1588,
                                c1588_ptp_packet_info_t *pkt_info,
                                uint16_t seq_id_rx);

/**
  core1588_ptp_set_rx_unicast_addr() sets the Rx unicast IP address. The
  address is provided through the addr parameter as a uint8_t array of length
  16.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param addr
  uint8_t is an array of length 16 containing the Rx unicast address to set.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    uint8_t ucast[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab,
                         0xcd, 0xef, 0xa1, 0xb2, 0xc3, 0xd4,
                         0xee, 0xff, 0x77, 0x88};
    core1588_ptp_get_rx_src_port_id(this_c1588, ucast);
  @endcode
 */
c1588_status_t core1588_ptp_set_rx_unicast_addr(c1588_instance_t *this_c1588,
                                                uint8_t addr[16]);

/**
  core1588_ptp_get_txstamp() reads the sampled Tx RTC timestamp from the Tx
  timestamp registers. The RTC is sampled in response to the appearance of a
  Tx PTP packet and is stored in the appropriate registers. The API reads the
  timestamp as a c1588_timestamp_t structure and stores the value at the
  address provided by the ts API parameter. tx_type denotes the type of
  message transmitted by the Core1588 and is used by the API to determine the
  register set from which the timestamp is read.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param ts
  The ts parameter is a pointer to a c1588_timestamp_t structure where the
  sampled Tx RTC timestamp is stored.

  @param tx_type
  uint32_t mask of interrupt bits to denote Tx package type.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_timestamp_t timestamp;
    uint32_t Tx_type = C1588_TXSYNC;
    core1588_ptp_get_txstamp(this_c1588, &timestamp, tx_type);
  @endcode
 */
c1588_status_t core1588_ptp_get_txstamp(c1588_instance_t *this_c1588,
                                        c1588_timestamp_t *ts,
                                        uint32_t tx_type);

/**
  core1588_ptp_get_tx_seq_id() gets the Tx timestamp sequence ID. The value
  is stored in the c1588_ptp_packet_info_t structure at the address provided
  by the pkt_info API parameter. tx_type denotes the type of message
  transmitted by the Core1588 and is used by the API to determine which
  register to read the sequence ID from.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param pkt_info
  c1588_ptp_packet_info_t is a pointer to the information structure that stores
  the sequence ID.

  @param tx_type
  uint32_t mask of interrupt bits to denote Tx package type.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_ptp_packet_info_t pkt_info;
    uint32_t tx_type = C1588_TXSYNC;
    core1588_ptp_get_tx_seq_id(this_c1588, &pkt_info, tx_type);
  @endcode
 */
c1588_status_t core1588_ptp_get_tx_seq_id(c1588_instance_t *this_c1588,
                                          c1588_ptp_packet_info_t *pkt_info,
                                          uint32_t tx_type);

/**
  core1588_ptp_tx_default_handler() handles the gathering of Tx packet
  information on transmission of a valid PTP packet. Timestamp, sequence ID,
  source port ID and message type are recorded and interrupt flags are cleared.
  The packet information is stored in a c1588_ptp_packet_info_t structure and is
  placed in the c1588_instance_t ring buffer tx_buf. tx_irq_mask should contain
  a mask of all of the Tx related interrupts in the masked interrupt register.
  This API is the default Tx interrupt handler and is called by core1588_isr().

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param tx_irq_mask
  uint32_t mask of interrupt bits to denote Tx IRQ type.

  @return
  This function does not return a value.
 */
void core1588_ptp_tx_default_handler(c1588_instance_t *this_c1588,
                                     uint32_t tx_irq_mask);

/**
  core1588_ptp_tx_add_to_buffer() adds a transmitted PTP messages information to
  the PTP Tx message ring buffer. The buffer can store C1588_TX_RING_SIZE - 1
  items, where C1588_TX_RING_SIZE is defined in core1588_user_config.h. PTP
  information is stored in the buffer as c1588_ptp_packet_info_t structs and
  passed to the API through the pkt_info API parameter. The API stores messages
  using FIFO and handles the management of ring buffer indices. If the buffer is
  full, the oldest buffer contents will be overwritten. This API will be called
  by the core1588_ptp_tx_default_handler() API.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param pkt_info
  The pkt_info paramater contains the Tx packet information to store in the
  Tx ring buffer.

  @return
  This function does not return a value.

  @example
  @code
    core1588_ptp_get_tx_src_port_id(this_c1588, pkt_info);
  @endcode
 */
void core1588_ptp_tx_add_to_buffer(c1588_instance_t *this_c1588,
                                   c1588_ptp_packet_info_t pkt_info);

/**
  core1588_ptp_tx_get_from_buffer() retrieves a transmitted PTP messages
  information from the PTP Tx message ring buffer. The API copies the message
  contents to the address provided by the pkt_info parameter. User must
  provide the sequence ID of the desired PTP message through the seq_id_tx
  paramter. The API then looks for a timestamp with a matching sequence ID
  in the buffer. If no timestamp is found, the provided pointer will be
  assigned C1588_NULL_PACKET_INFO. The API retrieves messages on a FIFO
  basis and handle the management of ring indices.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param pkt_info
  The pkt_info paramater is the pointer where the desired packet information
  is written.

  @param seq_id_tx
  The seq_id_tx parameter contains the sequence ID of the desired PTP message.

  @return
  c1588_status_t declares the success or failure.
 */
c1588_status_t
core1588_ptp_tx_get_from_buffer(c1588_instance_t *this_c1588,
                                c1588_ptp_packet_info_t *pkt_info,
                                uint16_t seq_id_tx);

/**
  core1588_ptp_set_tx_unicast_addr() sets the Tx unicast IP address. The
  address is provided through the addr parameter as a uint8_t array of length
  16.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param addr
  uint8_t array of length 16 containing the Tx unicast address to set.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    uint8_t ucast[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab,
                         0xcd, 0xef, 0xa1, 0xb2, 0xc3, 0xd4,
                         0xee, 0xff, 0x77, 0x88};
    core1588_ptp_get_tx_src_port_id(this_c1588, ucast);
  @endcode
 */
c1588_status_t core1588_ptp_set_tx_unicast_addr(c1588_instance_t *this_c1588,
                                                uint8_t addr[16]);

/**
  core1588_latch_check_enabled() checks whether the latch associated with
  the provided ID has been enabled or not in the configuration registers.
  latch_mask is a mask of latch IDs to check.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param latch_mask
  uint32_t mask of interrupt bits to denote which latch ID to check. The
  possible options are:
    - C1588_LATCH_0
    - C1588_LATCH_1
    - C1588_LATCH_2
    - C1588_LATCH_MASK_ALL

  @return
    - 1: if latch is enabled
    - 0: if latch is disabled

  @example
  @code
    uint32_t latch_mask = C1588_LATCH_0 | C1588_LATCH_1;
    if (core1588_latch_check_enabled(this_c1588, latch_mask))
    {
      .......
    }
  @endcode
 */
uint8_t core1588_latch_check_enabled(c1588_instance_t *this_c1588,
                                     uint32_t latch_mask);

/**
  core1588_latch_control() controls RTC latches and their registers. latch_mask
  is a mask of latch IDs to control. Passing C1588_ENABLE as a parameter will
  enable any latches included in latch_mask, while passing C1588_DISABLE will
  disable any latches included in latch_mask.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param latch_mask
  uint32_t mask of interrupt bits to denote which latch ID to control. The
  possible options are:
    - C1588_LATCH_0
    - C1588_LATCH_1
    - C1588_LATCH_2
    - C1588_LATCH_MASK_ALL

  @param control
  uint32_t parameter which selects whether the API enables or disables its
  respective functionality.

  @return
  This function does not return a value.

  @example
  @code
    uint32_t latch_mask = C1588_LATCH_0 | C1588_LATCH_1;
    core1588_latch_control(this_c1588, latch_mask, C1588_ENABLE);
  @endcode
 */
void core1588_latch_control(c1588_instance_t *this_c1588, uint32_t latch_mask,
                            uint32_t control);

/**
  core1588_latch_get_timestamp() reads the sampled RTC timestamp from the
  selected set of latch registers. The timestamp is read as a c1588_timestamp_t
  structure and stores the value at the address provided by the ts API
  parameter. latch_mask parameter is used to select the latch from which to
  read the timestamp (0, 1, 2). Only one latch may be included in the mask
  while calling the API, otherwise the API will return a failure status.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param ts
  The ts parameter is a pointer to a c1588_timestamp_t structure that stores
  the sampled latch RTC timestamp.

  @param latch_mask
  uint32_t mask of interrupt bits to denote which latch ID to read timestamp
  from. The possible options are:
    - C1588_LATCH_0
    - C1588_LATCH_1
    - C1588_LATCH_2
  NOTE: Only one of these latches should be selected at a time.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_timestamp_t timestamp;
    uint32_t latch_mask = C1588_LATCH_0;
    core1588_latch_get_timestamp(this_c1588, &timestamp, latch_mask);
  @endcode
 */
c1588_status_t core1588_latch_get_timestamp(c1588_instance_t *this_c1588,
                                            c1588_timestamp_t *ts,
                                            uint32_t latch_mask);

/**
  core1588_latch_default_handler() handles the gathering of latch timestamp
  information in the event of an RTC latch event. Timestamp and latch ID are
  recorded and interrupt flags are cleared. The event information is stored in a
  c1588_rtc_event_timestamp_t structure and is placed in the c1588_instance_t
  ring buffer latch_buf. latch_mask should contain a mask of all of the
  latch related interrupts in the masked interrupt register. This API is the
  default latch interrupt handler and is called by core1588_isr().

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param latch_mask
  uint32_t mask of interrupt bits to denote which latch ID to handle. The
  possible options are:
    - C1588_LATCH_0
    - C1588_LATCH_1
    - C1588_LATCH_2
    - C1588_LATCH_MASK_ALL

  @return
  This function does not return a value.
 */
void core1588_latch_default_handler(c1588_instance_t *this_c1588,
                                    uint32_t latch_mask);

/**
  core1588_latch_add_to_buffer() adds an RTC latch event's information to the
  latch event ring buffer. The buffer can store C1588_LATCH_RING_SIZE - 1 items,
  where C1588_LATCH_RING_SIZE is defined in core1588_user_config.h. Latch
  event information is stored in the buffer as c1588_rtc_event_timestamp_t
  structs and passed to the API through the latch_ts API parameter. The API
  stores messages using FIFO and handles the management of ring buffer indices.
  If the buffer is full, the oldest buffer contents will be overwritten. This
  API will be called by the core1588_latch_default_handler() API.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param latch_ts
  The latch_ts parameter contains the latch timestamp information to store in
  the latch ring buffer.

  @return
  This function does not return a value.
 */
void core1588_latch_add_to_buffer(c1588_instance_t *this_c1588,
                                  c1588_rtc_event_timestamp_t latch_ts);

/**
  core1588_latch_get_from_buffer() pops the oldest latch timestamping
  information from the latch information ring buffer. The function copies the
  message contents to the address of the pointer provided by the latch_ts
  parameter. The API will retrieve messages on a FIFO basis and handle the
  management of ring indices.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param latch_ts
  The latch_ts parameter is the pointer where the desired latch timestamp
  will be written.

  @return
  c1588_status_t declares the success or failure.
 */
c1588_status_t
core1588_latch_get_from_buffer(c1588_instance_t *this_c1588,
                               c1588_rtc_event_timestamp_t *latch_ts);

/**
  core1588_trigger_check_enabled() checks whether the trigger associated with
  the provided ID has been enabled or not in the configuration registers.
  trigger_mask is a mask of trigger IDs to check.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param trigger_mask
  uint32_t mask of interrupt bits to denote which trigger ID to check. The
  possible options are:
    - C1588_TRIGGER_0
    - C1588_TRIGGER_1
    - C1588_TRIGGER_2
    - C1588_TRIGGER_MASK_ALL

  @return
    - 1: if latch is enabled
    - 0: if latch is disabled

  @example
  @code
    uint32_t trigger_mask = C1588_TRIGGER_0 | C1588_TRIGGER_1;
    if (core1588_trigger_check_enabled(this_c1588, trigger_mask))
    {
      .......
    }
  @endcode
 */
uint8_t core1588_trigger_check_enabled(c1588_instance_t *this_c1588,
                                       uint32_t trigger_mask);

/**
  core1588_trigger_control() control RTC triggers and their registers.
  trigger_mask is a mask of trigger IDs to control. Passing C1588_ENABLE as a
  parameter will enable any triggers included in trigger_mask, while passing
  C1588_DISABLE will disable any triggers included in trigger_mask.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param trigger_mask
  uint32_t mask of interrupt bits to denote which trigger ID to enable. The
  possible options are:
    - C1588_TRIGGER_0
    - C1588_TRIGGER_1
    - C1588_TRIGGER_2
    - C1588_TRIGGER_MASK_ALL

  @param control
  uint32_t parameter which selects whether the API enables or disables its
  respective functionality.

  @return
  This function does not return a value.

  @example
  @code
    uint32_t trigger_mask = C1588_TRIGGER_0 | C1588_TRIGGER_1;
    core1588_trigger_control(this_c1588, trigger_mask, C1588_ENABLE);
  @endcode
 */
void core1588_trigger_control(c1588_instance_t *this_c1588,
                              uint32_t trigger_mask, uint32_t control);

/**
  core1588_trigger_set_timestamp() writes an RTC timestamp to the selected
  set trigger registers to configure the time at which an ETC trigger event
  will occur. The timestamp is passed as a c1588_timestamp_t structure and is
  provided by  the ts API parameter. trigger_mask parameter is used to
  select the trigger, where the timestamp will be written (0, 1, 2). Only one
  trigger may be included in the mask while calling to the API, otherwise the
  API will return a failure status.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param ts
  The ts parameter is a pointer to a c1588_timestamp_t structure that stores
  the RTC trigger timestamp to set.

  @param trigger_mask
  uint32_t mask of interrupt bits to denote which trigger ID to write to. The
  possible options are:
    - C1588_TRIGGER_0
    - C1588_TRIGGER_1
    - C1588_TRIGGER_2
  NOTE: C1588_TRIGGER_MASK_ALL should not be used in this API, triggers must
  be set one at a time.

  @return
  c1588_status_t declares the success or failure.
 */
c1588_status_t core1588_trigger_set_timestamp(c1588_instance_t *this_c1588,
                                              c1588_timestamp_t *ts,
                                              uint32_t trigger_mask);

/**
  core1588_trigger_get_timestamp() reads the sampled RTC timestamp from the
  selected set of trigger registers. The timestamp is read as a
  c1588_timestamp_t structure and stores the value at the address provided by
  the ts API parameter. trigger_mask parameter is used to select the
  trigger from which to read the timestamp (0, 1, 2). Only one trigger may be
  included in the mask while calling the API, otherwise the API will return
  a failure status.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param ts
  The ts parameter is a pointer to a c1588_timestamp_t structure that stores
  the sampled trigger RTC timestamp value.

  @param trigger_mask
  uint32_t mask of interrupt bits to denote which trigger ID to read timestamp
  from. The possible options are:
    - C1588_TRIGGER_0
    - C1588_TRIGGER_1
    - C1588_TRIGGER_2
  NOTE: Only one of these triggers should be selected at a time.

  @return
  c1588_status_t declares the success or failure.

  @example
  @code
    c1588_timestamp_t timestamp;
    uint32_t trigger_mask = C1588_TRIGGER_0;
    core1588_trigger_get_timestamp(this_c1588, &timestamp, trigger_mask);
  @endcode
 */
c1588_status_t core1588_trigger_get_timestamp(c1588_instance_t *this_c1588,
                                              c1588_timestamp_t *ts,
                                              uint32_t trigger_id);

/**
  core1588_trigger_default_handler() handles the gathering of trigger
  timestamp information in the event of an RTC trigger event. Timestamp and
  trigger ID are recorded and interrupt flags are cleared. The event information
  is stored in a c1588_rtc_event_timestamp_t structure and is placed in the
  c1588_instance_t ring buffer trigger_buf. trigger_mask should contain a mask
  of all of the trigger related interrupts in the masked interrupt register.
  This API is the default trigger interrupt handler and is called by
  core1588_isr().

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param trigger_mask
  uint32_t mask of interrupt bits to denote which trigger ID to handle. The
  possible options are:
    - C1588_TRIGGER_0
    - C1588_TRIGGER_1
    - C1588_TRIGGER_2
    - C1588_TRIGGER_MASK_ALL

  @return
  This function does not return a value.
 */
void core1588_trigger_default_handler(c1588_instance_t *this_c1588,
                                      uint32_t trigger_mask);

/**
  core1588_trigger_add_to_buffer() adds an RTC trigger events information to the
  trigger event ring buffer. The buffer can store C1588_TRIGGER_RING_SIZE - 1
  items, where C1588_TRIGGER_RING_SIZE is defined in core1588_user_config.h.
  Trigger event information is stored in the buffer as
  c1588_rtc_event_timestamp_t structs and passed to the API through the
  trigger_ts API parameter. The API stores messages on a FIFO basis and handles
  the management of ring buffer indices. If the buffer is full, the oldest
  buffer contents will be overwritten. This API will be called by the
  core1588_trigger_default_handler() API.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param trigger_ts
  the trigger_ts parameter contains the trigger timestamp information to store
  in the trigger ring buffer.

  @return
  This function does not return a value.
 */
void core1588_trigger_add_to_buffer(c1588_instance_t *this_c1588,
                                    c1588_rtc_event_timestamp_t trigger_ts);

/**
  core1588_trigger_get_from_buffer() retrieves the oldest trigger timestamping
  information from the trigger information ring buffer, and also removes it
  from the buffer. The function will copy the message contents to the address
  of the pointer provided by the trigger_ts parameter. The API will retrieve
  messages on a FIFO basis and handle the management of ring indices.

  @param this_c1588
  The this_c1588 parameter is a pointer to a c1588_instance_t structure that
  holds all the data related to a Core1588 instance.

  @param trigger_ts
  The trigger_ts parameter is the pointer where the desired trigger timestamp
  will be written.

  @return
  c1588_status_t declares the success or failure.
 */
c1588_status_t
core1588_trigger_get_from_buffer(c1588_instance_t *this_c1588,
                                 c1588_rtc_event_timestamp_t *trigger_ts);

#ifdef __cplusplus
}
#endif

#endif /* __CORE1588_H */
