/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_can.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreCAN header file
 *
 */

/*=========================================================================*//**
  @mainpage CoreCAN Bare Metal Driver.

==============================================================================
  Introduction
==============================================================================

  CoreCAN is an IP component that supports CoreCAN v2.0 with ISO 11898-1. CAN is
  a robust serial communication protocol. A Controller Area Network (CAN) is a
  serial half-duplex and differential two-wired asynchronous communication
  protocol. Any node can initiate communication when the bus is free. Data is
  transmitted in frames with specific identifiers, not to specific nodes.
  Messages are broadcast to all nodes, which then decide whether to accept or
  ignore them.

  This driver provides a set of functions for controlling CoreCAN as part of the
  bare metal system where no operating system is available. This driver can be
  adapted to be used as a part of an operating system, but the implementation
  of the adaptation layer between the driver and the operating system's driver
  model is outside the scope of this driver.

  --------------------------------
  Features
  --------------------------------
  The CoreCAN driver provides support for the following features:
    - Basic CAN APIs (if application needs support for basic CAN operation)
    - Full CAN APIs (each message has its own message filter)
    - Support for 11-bit and 29-bit message identifiers
    - Support for data frame and remote frames
    - Error detection mechanism

  ==============================================================================
  Driver Configuration
  ==============================================================================
  Your application software must configure the CoreCAN driver by calling the
  CAN_init() function for each CoreCAN instance in the hardware design.
  CAN_init() function configures CoreCAN hardware instance base address.
  You must call the CAN_init() function before calling any other CoreCAN driver
  functions.

  ==============================================================================
  Theory of Operation
  ==============================================================================
  The CoreCAN software driver is designed to control the multiple instances of
  CoreCAN IP. Each instance of CoreCAN in the hardware design is associated
  with a single instance of the can_instance_t structure in the software. The
  contents of these data structures are initialized by calling the CAN_init()
  function. A pointer to the structure is passed to the subsequent driver
  functions to identify the CoreCAN hardware instance on which the operation
  should be performed.

  Note: Do not attempt to directly manipulate the content of can_instance_t
        structure. This structure must be modified by the driver function.

  The CoreCAN driver operations can be divided in following sub-sections:
    - CAN Controller Configuration
    - Operation Status
    - Interrupt Support
    - Helper Functions
    - Basic CAN Message Handling
    - Full CAN Message Handling

  --------------------------------
  Configuration
  --------------------------------
  The CoreCAN driver must be initialized first and the mode of operation must
  be selected before performing data transfers on CAN Bus. The CAN_init()
  function initializes the CAN controller and driver. Set CAN controller
  operation mode as normal operation using CAN_set_mode() function. The
  operating mode of operation is selected using CAN_set_mode() function. The
  data transfer including the transmission or reception of data gets started by
  using the CAN_start() function. CAN_stop() function is used to stop the CAN
  controller. Once initialized, during the normal mode of operation, the
  CoreCAN driver configuration can be changed using CAN_set_config_reg()
  function.

  --------------------------------
  Operation Status
  --------------------------------
  CAN_get_err_sts() function  returns the current CAN error state (error active,
  error passive and bus off). The CAN_get_rx_err_cnt() and CAN_get_tx_err_cnt()
  functions return the actual receive and transmit error counter values while
  CAN_get_rx_gte96() function and CAN_get_tx_gte96() function show if the error
  counters are greater or equal to 96, which indicates a heavily disturbed bus.

  --------------------------------
  Interrupt Support
  --------------------------------
  The interrupt service routines are not part of the CAN driver. But access
  functions for the interrupt registers are provided. The individual
  interrupt enable bits are set using CAN_enable_irq() function and individual
  interrupt enable bits are disabled using CAN_disable_irq() while
  CAN_get_int_en() function returns their actual state.

  CAN_get_global_int_en() function indicates if interrupt generation is enabled
  at all. CAN_get_int_src() function shows the current state of the different
  interrupt status bits. Each interrupt status bit is cleared individually
  using CAN_clear_int_src() function.

  --------------------------------
  Helper Functions
  --------------------------------
  CAN_get_amcr_mask_f() function returns the message filter settings of the
  selected received message. CAN_set_amcr_mask_f() configures the message filter
  settings for the selected receive message.

  --------------------------------
  Basic CAN Message Handling
  --------------------------------
  A basic CAN type controller contains one or more message filter and one
  common message buffer or FIFO. The CAN driver contains some functions to
  emulate basic CAN operation by linking several buffers together to form a
  buffer array that shares one message filter. Since this buffer array is not
  a real FIFO, message inversion might happen (for example, a newer message
  might be pulled from the receive buffer prior to an older message).

  Before using the basic CAN API, configure the CAN controller first by calling
  the CAN_config_buffer() function. This sets up the message array and
  configures the message filter. CAN_send_msg() function and CAN_get_msg()
  function  are used to send and receive a message from transmit or receive
  buffers. CAN_tx_msg_avail() function indicates if a new message can be sent.
  CAN_rx_msg_avail() function shows if a new message is available.

  --------------------------------
  Full CAN Message Handling
  --------------------------------
  In Full CAN operation, each message has its own message filter. This reduces
  the number of receive interrupts as the host CPU only gets an interrupt when
  a message of interest has arrived. Further, software based message filtering
  overhead is reduced and there is less message to be checked. Before using a
  buffer for Full CAN operation, it must be configured using
  CAN_config_buffer_f() function. An error is generated if this buffer is
  already reserved for basic CAN operation. CAN_tx_msg_avail_f() function
  indicates the availability of the Tx message buffer specified by the message
  index to send a new message.The CAN_get_rx_buff_sts() and CAN_get_tx_buff_sts()
  functions indicate the current state of the receive and transmit buffers,
  respectively. The CAN_send_msg_f() function sends a message using the buffer.
  The CAN_abort_tx_msg_f() function aborts a pending message transfer, and the
  CAN_get_msg_f() function reads a message. CAN_rx_msg_avail_f() function shows
  that the Rx buffer specified by the message index contains a new message.
  If a buffer is set for automatic RTR reply, CAN_set_rtr_msg_f() function sets
  the CAN message that is returned upon reception of the RTR message.
  CAN_get_rtr_msg_abort_f() function aborts the transmit request of the RTR
  message.

  NOTE:
  1. To abort a pending RTR auto-reply, the RTR message filter should be
     configured to match the CAN_get_rtr_msg_abort_f() function.
  2. An error is generated if buffer is already reserved for basic CAN
     operation and is using the same buffer for Full CAN functionality.
  3. Special case of Full CAN where several messages are linked together to
     create FIFOs that share an identical message filter configuration, can
     be built upon the available Full CAN functions.

*//*=========================================================================*/

#ifndef __CORE_CAN_H
#define __CORE_CAN_H    1

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#include "hal_assert.h"
#endif

/* Configuration and Speed definitions */
#define CAN_SET_SJW(_sjw)                (_sjw<<2u)
#define CAN_SET_TSEG2(_tseg2)            (_tseg2<<5u)
#define CAN_SET_TSEG1(_tseg1)            (_tseg1<<8u)
#define CAN_SET_BITRATE(_bitrate)        (_bitrate<<16u)

//Auto restart
#define CAN_AUTO_RESTART_DISABLE         0u
#define CAN_AUTO_RESTART_ENABLE          (1u << 4u)

//Transmit buffer arbitration
#define CAN_ROUND_ROBIN_ARB              0u
#define CAN_FIXED_PRIORITY_ARB           (1u << 12u)

// Byte order
#define CAN_BIG_ENDIAN                   0u
#define CAN_LITTLE_ENDIAN                (1u << 13u)

#define CAN_MB_MAX                       32u
#define CAN_MB_MIN                       1u

#define NULL_INSTANCE                    ((can_instance_t* )0u)
#define ENABLE                           1u
#define DISABLE                          0u

/* Manual setting with specified fields  */
#define CAN_SPEED_MANUAL                 0u //remove this macro if we are not providing manual clk configurations
#define CAN_TSEG1_MIN                    (2u)
#define CORECAN_MSG_STD_ID_SHIFT         (21u)

/***************************************************************************//**
  The following constants are used in the CoreCAN driver for bitrate definitions:

  | Constants          |  Description                                        |
  |--------------------|-----------------------------------------------------|
  | CAN_SPEED_40M_1M   | Indicates CAN controller shall be configured with   |
  |                    | 1Mbps baud rate if the input clock is 40MHz.        |
  | CAN_SPEED_40M_500K | Indicates CAN controller shall be configured with   |
  |                    | 500Kbps baud rate if the input clock is 40MHz.      |
  | CAN_SPEED_40M_250K | Indicates CAN controller shall be configured with   |
  |                    | 250Kbps baud rate if the input clock is 40MHz.      |
  | CAN_SPEED_40M_125K | Indicates CAN controller shall be configured with   |
  |                    | 125Kbps baud rate if the input clock is 40MHz.      |
*/

#define CAN_SPEED_40M_1M    CAN_SET_BITRATE(3)|CAN_SET_TSEG1(4)|CAN_SET_TSEG2(3)
#define CAN_SPEED_40M_500K  CAN_SET_BITRATE(4)|CAN_SET_TSEG1(9)|CAN_SET_TSEG2(4)
#define CAN_SPEED_40M_250K  CAN_SET_BITRATE(9)|CAN_SET_TSEG1(9)|CAN_SET_TSEG2(4)
#define CAN_SPEED_40M_125K  CAN_SET_BITRATE(19)|CAN_SET_TSEG1(9)|CAN_SET_TSEG2(4)

/***************************************************************************//**
  The following constants are used for error codes:

  |  Constants            |  Description                                |
  |-----------------------|---------------------------------------------|
  | CAN_OK                | Indicates there is no error                 |
  | CAN_ERR               | Indicates error condition                   |
  | CAN_TSEG1_TOO_SMALL   | Value provided to configure TSEG1 is too    |
  |                       | small                                       |
  | CAN_TSEG2_TOO_SMALL   | Value provided to configure TSEG2 is too    |
  |                       | small                                       |
  | CAN_SJW_TOO_BIG       | Value provided to configure synchronous jump|
  |                       | width (SJW) is too big.                     |
  | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
  |                       | Basic CAN operation                         |
  | CAN_NO_RTR_MAILBOX    | Indicates that there is no mailbox for      |
  |                       | remote transmit request (RTR) frame         |
  | CAN_INVALID_MAILBOX   | Indicates invalid mailbox number            |
 */
#define CAN_OK                           0u
#define CAN_ERR                          1u
#define CAN_TSEG1_TOO_SMALL              2u
#define CAN_TSEG2_TOO_SMALL              3u
#define CAN_SJW_TOO_BIG                  4u
#define CAN_BASIC_CAN_MAILBOX            5u
#define CAN_NO_RTR_MAILBOX               6u
#define CAN_INVALID_MAILBOX              7u

/*  Flag bits */
#define CAN_NO_MSG                       0x0u
#define CAN_VALID_MSG                    0x1u

/***************************************************************************//**
  The following constants are used in the CoreCAN driver for Interrupt Bit
  Definitions

  |  Constants               |  Description                                   |
  |--------------------------|------------------------------------------------|
  | CAN_INT_GLOBAL           | Indicates to enable global interrupt           |
  | CAN_INT_ARB_LOSS         | Indicates arbitration loss interrupt           |
  | CAN_INT_OVR_LOAD         | Indicates overload message detected interrupt  |
  | CAN_INT_BIT_ERR          | Indicates bit error interrupt                  |
  | CAN_INT_STUFF_ERR        | Indicates bit stuffing error interrupt         |
  | CAN_INT_ACK_ERR          | Indicates acknowledge error interrupt          |
  | CAN_INT_FORM_ERR         | Indicates format error interrupt               |
  | CAN_INT_CRC_ERR          | Indicates CRC error interrupt                  |
  | CAN_INT_BUS_OFF          | Indicates bus off interrupt                    |
  | CAN_INT_RX_MSG_LOST      | Indicates received message lost interrupt      |
  | CAN_INT_TX_MSG           | Indicates message transmit interrupt           |
  | CAN_INT_RX_MSG           | Indicates receive message available interrupt  |
  | CAN_INT_RTR_MSG          | Indicates RTR auto-reply message sent interrupt|
  | CAN_INT_STUCK_AT_0       | Indicates stuck at dominant error interrupt    |
  | CAN_INT_OST_FAILURE      | Indicates one shot transmission failure        |
  |                          | interrupt                                      |
 */
#define CAN_INT_GLOBAL        (1<<0)    /* Global interrupt  */
#define CAN_INT_ARB_LOSS      (1<<2)    /* Arbitration loss interrupt  */
#define CAN_INT_OVR_LOAD      (1<<3)    /*Overload interrupt  */
#define CAN_INT_BIT_ERR       (1<<4)    /* Bit error interrupt  */
#define CAN_INT_STUFF_ERR     (1<<5)    /* Bit stuffing error interrupt  */
#define CAN_INT_ACK_ERR       (1<<6)    /* Acknowledgement error interrupt  */
#define CAN_INT_FORM_ERR      (1<<7)    /* Format error interrupt  */
#define CAN_INT_CRC_ERR       (1<<8)    /* CRC error interrupt  */
#define CAN_INT_BUS_OFF       (1<<9)    /* Bus-off interrupt  */
#define CAN_INT_RX_MSG_LOST   (1<<10)   /* Rx message lost interrupt  */
#define CAN_INT_TX_MSG        (1<<11)   /* Tx message interupt  */
#define CAN_INT_RX_MSG        (1<<12)   /* Rx message interrupt  */
#define CAN_INT_RTR_MSG       (1<<13)   /* RTR message interrupt  */
#define CAN_INT_STUCK_AT_0    (1<<14)   /* Stuck-at-0 error interrupt  */
#define CAN_INT_OST_FAILURE   (1<<15)   /* One-shot transmission error interrupt*/

/*-------------------------------------------------------------------------*//**
  The mss_can_mode_t enumeration specifies the possible operating modes of CAN
  controller. The following table provide details of the constants.

  |  Modes                     |  Description                             |
  |----------------------------|------------------------------------------|
  | CANOP_MODE_NORMAL          | Indicates CAN controller is in normal    |
  |                            | operational mode.                        |
  | CANOP_MODE_LISTEN_ONLY     | Indicates CAN controller is in listen    |
  |                            | only mode.                               |
  | CANOP_MODE_EXT_LOOPBACK    | Indicates CAN controller is in external  |
  |                            | loop back mode.                          |
  | CANOP_MODE_INT_LOOPBACK    | Indicates CAN controller is in internal  |
  |                            | loop back mode.                          |
 */
typedef enum can_mode
{
    CANOP_MODE_NORMAL       = 0x00u,
    CANOP_MODE_LISTEN_ONLY  = 0x01u,
    CANOP_MODE_EXT_LOOPBACK = 0x02u,
    CANOP_MODE_INT_LOOPBACK = 0x03u,
}can_mode_t;

/***************************************************************************//**
 * The can_mb_t enumeration defines the 32 mailboxes (CAN_MB_0 to CAN_MB_31)
 * used by the CAN driver for message transmission (Tx) and reception (Rx).
 * These mailboxes are used as parameters in functions to send or receive data
 * using Tx/Rx registers.
 *
 */

typedef enum can_mb_t {
    CAN_MB_0 = 0,
    CAN_MB_1,
    CAN_MB_2,
    CAN_MB_3,
    CAN_MB_4,
    CAN_MB_5,
    CAN_MB_6,
    CAN_MB_7,
    CAN_MB_8,
    CAN_MB_9,
    CAN_MB_10,
    CAN_MB_11,
    CAN_MB_12,
    CAN_MB_13,
    CAN_MB_14,
    CAN_MB_15,
    CAN_MB_16,
    CAN_MB_17,
    CAN_MB_18,
    CAN_MB_19,
    CAN_MB_20,
    CAN_MB_21,
    CAN_MB_22,
    CAN_MB_23,
    CAN_MB_24,
    CAN_MB_25,
    CAN_MB_26,
    CAN_MB_27,
    CAN_MB_28,
    CAN_MB_29,
    CAN_MB_30,
    CAN_MB_31
}can_mb_t;

/***************************************************************************//**
 * This structure identifies various CoreCAN hardware instances within your
 * system. The CAN_init() function initializes this structure. A pointer to
 * an initialized instance of the structure should be provided as the first
 * parameter to the CAN driver functions to identify which CoreCAN should
 * perform the requested operation.
 */

typedef struct CAN_instance
{
    addr_t    base_address;
    /* Local data (eg: pointer to local FIFO) */
    uint8_t   basic_can_rx_mb; /* number of rx mailboxes */
    uint8_t   basic_can_tx_mb; /* number of tx mailboxes */
}can_instance_t;

/***************************************************************************//**
 * The can_txmsgobject_t structure is used to configure and transmit CAN
 * messages. It contains the message identifier (ID), data (Word0 and Word1)
 * and control bits to configure the transmission of CAN messages. This
 * structure is passed to the CAN driver functions to set up the message for
 * transmission that includes defining the message ID, data content and
 * additional control parameters such as Remote Transmission Request (RTR),
 * Identifier Extension (IDE) and Data Length Code (DLC).
 *
 */

typedef struct can_txmsgobject
{
    uint32_t ID;
    uint32_t word1;
    uint32_t word0;
    uint8_t RTR;
    uint8_t IDE;
    uint8_t DLC;
}can_txmsgobject_t;

/***************************************************************************//**
 * The can_rxmsgobject_t structure filters and receive CAN messages. To
 * configure the filtering, use the Acceptance Mask Register (AMR) and
 * Acceptance Code Register (ACR), along with their corresponding data fields
 * (AMR Data and ACR Data). These fields define which CAN messages should be
 * accepted based on their ID, IDE, RTR and data. Any unused fields in the
 * structure should be set to 0 or ignored. You can use this structure to get
 * message data, including the message ID, data words (Word0, Word1), and
 * control information such as Remote Transmission Request (RTR), Identifier
 * Extension (IDE), and Data Length Code (DLC).
 *
 */

typedef struct can_rxmsgobject
{
    uint32_t ID;
    uint32_t word1;
    uint32_t word0;
    uint32_t AMR;
    uint32_t ACR;
    uint16_t AMR_D;
    uint16_t ACR_D;
    uint8_t RTR;
    uint8_t IDE;
    uint8_t DLC;
}can_rxmsgobject_t;


/*------------------------Public Function-------------------------------------*/

/***************************************************************************//**
  The CAN_init() function initializes the driver. It sets the base address for
  the CoreCAN instance and initializes the can_instance_t data structure. This
  function must be called before calling any other CoreCAN driver functions.
  The basic_can_rx_mb and basic_can_tx_mb configures the number of receive and
  transmit mailboxes in basic CAN operation. This function configures the CAN
  channel speed as per the “bitrate” parameter. It initializes all receive
  messages and makes it ready for configuration.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param base_addr
    The base_addr parameter is the base address in the processor's memory map
	for the registers of the CoreCAN hardware instance being initialized.

  @param bitrate
    The bitrate parameter is used to configure CAN speed. The following standard
    preset bitrate constants are defined for systems with a PCLK of 40 MHz:

        - CAN_SPEED_40M_1M   : 1 Mbps CAN speed
        - CAN_SPEED_40M_500K : 500 Kbps CAN speed
        - CAN_SPEED_40M_250K : 250 Kbps CAN speed
        - CAN_SPEED_40M_125K : 125 Kbps CAN speed

    For custom settings, use CAN_SPEED_MANUAL and configure the settings using
    can_config parameter.
    The default configurations gets altered by the addition of 0 or more of
    the following:
        - CAN_AUTO_RESTART_DISABLE
        - CAN_AUTO_RESTART_ENABLE
        - CAN_ROUND_ROBIN_ARB
        - CAN_FIXED_PRIORITY_ARB
        - CAN_LITTLE_ENDIAN
        - CAN_BIG_ENDIAN

    Note: Currently, only the predefined bitrate constants for a PCLK of 40 MHz
          are fully supported and recommended for use. Custom bitrate
          configurations (via CAN_SPEED_MANUAL) and other clock frequencies are
          not yet fully supported and may not function as expected. Support for
          custom bit timing configurations and other clock frequencies will be
          provided in a future release.

  @param can_config
    The can_config parameter is used only when bitrate is configured as
    CAN_SPEED_MANUAL. The following details should be noted:

      1. CFG_BITRATE defines the length of a CAN time quantum in terms of PCLK1
         with 0 = 1 PCLK1, 1 = 2 PCLK1s and so on.
      2. A CAN bit time is made of between 8 and 25 time quanta and the bitrate
         is PCLK1 / ((CFG_BITRATE + 1) * number of time quanta per bit).
      3. There is a fixed overhead of 1 time quantum for synchronization at the
         start of every CAN bit and the remaining time quanta in the bit are
         allocated with CFG_TSEG1 and CFG_TSEG2.
      4. CFG_TSEG1 can have a value between 2 and 15, which represents between 3
         and 16 time quanta.
      5. If SAMPLING_MODE is 0, CFG_TSEG2 can have a value between 1 and 7 that
         represents between 2 and 8 time quanta and if SAMPLING_MODE is 1,
         CFG_TSEG2 can have a value between 2 and 7 that represents between 3
         and 8 time quanta.
      6. Receive sampling takes place at the end of the segment defined by
         CFG_TSEG1.
    For example, if CFG_TSEG1 = 3 and CFG_TSEG2 = 2, we get:

          |<------------ 1 CAN bit time (8 time quanta)------------>|
           /------+------+------+------+------+------+------+------\
         -+ Synch |        CFG_TSEG1 + 1      | CFG_TSEG2 + 1       +-
           \------+------+------+------+------+------+------+------/
                                              |
                Receiver samples date here -->|

  @param basic_can_rx_mb
    The can_rx_mb parameter lists the number of receive mailboxes used in
    basic CAN mode.

  @param basic_can_tx_mb
    The can_tx_mb parameter lists the number of transmit mailboxes used in
    basic CAN mode.

  @return
    This function returns CAN_OK on successful execution, otherwise, it will
    return the following error codes:
    |  Constants            |  Description                                  |
    |-----------------------|-----------------------------------------------|
    | CAN_TSEG1_TOO_SMALL   | Value provided to configure TSEG1 is too small|
    | CAN_TSEG2_TOO_SMALL   | Value provided to configure TSEG2 is too small|
    | CAN_SJW_TOO_BIG       | Value provided to configure synchronous jump  |
    |                       | width (SJW) is too big.                       |

  @example
  Using a default set for bitrate, tseg1, tseg2, sjw and additional
  configuration parameters.
  @code
      can_instance_t g_can;
      int main(void)
      {
          CAN_init(&g_can, CAN_SPEED_40M_1M, 0, 16u, 7u);

          return(0);
      }
  @endcode

  @example
  Using custom settings for bitrate, tseg1, tseg2 and sjw.
  @code
      can_instance_t g_can;

      #define SYSTEM_CLOCK    8000000
      #define BITS_PER_SECOND 10000
      #define CFG_BITRATE ((SYSTEM_CLOCK / (BITS_PER_SECOND * 8)) - 1)
      #define CAN_CONFIG  CAN_SET_SJW(0) | CAN_SET_TSEG2(1) | CAN_SET_TSEG1(4) \
        | CAN_FIXED_PRIORITY_ARB | CAN_LITTLE_ENDIAN | CAN_SET_BITRATE(CFG_BITRATE)

      int main(void)
      {
          can_config_reg canreg;
          CAN_init(&g_can_0_lo,CAN_SPEED_MANUAL, CAN_CONFIG, 8, 4);

          return(0);
      }
  @endcode
 */
uint32_t
CAN_init
(
    can_instance_t * this_can,
    addr_t base_addr,
    uint32_t bitrate,
    uint32_t can_config,
    uint8_t basic_can_rx_mb,
    uint8_t basic_can_tx_mb
);

/***************************************************************************//**
  The CAN_set_config_reg() function  sets the configuration register and starts
  the CAN controller for normal mode operation. This function changes the
  configuration settings while the CAN controller is already initialized using
  CAN_init() function  and is also running. This function should not be used
  until the CAN controller is initialized.

  It performs the following tasks:
      - Clears all pending interrupts
      - Stops CAN controller
      - Disable interrupts
      - Sets new configuration
      - Starts CAN controller

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param cfg
    The cfg parameter is a 4 bytes variable used to set the configuration
    settings.

  @return
    This function does not return a value.

  @example
  @code
      can_instance_t g_can;

      #define CAN_CONFIG CAN_SET_SJW(0) | CAN_SET_TSEG2(1) | CAN_SET_TSEG1(4) \
                                | CAN_FIXED_PRIORITY_ARB | CAN_LITTLE_ENDIAN
      int main(void)
      {
          Return_status = CAN_init(&g_can, CAN_SPEED_40M_1M, 0, 16u, 7u);
          ....

          CAN_set_config_reg(&g_can, (CAN_SPEED_40M_1M | CAN_CONFIG));

          ....
          return(0);
      }
  @endcode
 */
void
CAN_set_config_reg
(
    can_instance_t * this_can,
    uint32_t cfg
);

/***************************************************************************//**
  The CAN_set_mode() function sets the CAN controller operating mode based on
  the mode parameter. After this, the CAN controller is not operational,
  to do that invoke CAN_start() function.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mode
    The mode parameter describes the desired operating mode of a CAN controller.
    Following are the possible operating modes:
    |  Modes                     |  Description                             |
    |----------------------------|------------------------------------------|
    | CANOP_MODE_NORMAL          | Indicates CAN controller is in normal    |
    |                            | operational mode.                        |
    | CANOP_MODE_LISTEN_ONLY     | Indicates CAN controller is in listen    |
    |                            | only mode.                               |
    | CANOP_MODE_EXT_LOOPBACK    | Indicates CAN controller is in external  |
    |                            | loop back mode.                          |
    | CANOP_MODE_INT_LOOPBACK    | Indicates CAN controller is in internal  |
    |                            | loop back mode.                          |

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          CAN_init(&g_can, CAN_SPEED_40M_1M, 0, 16u, 7u);
          CAN_set_mode(&g_can, CANOP_MODE_INT_LOOPBACK);
          ....

          return(0);
      }
  @endcode
 */
void
CAN_set_mode
(
    can_instance_t * this_can,
    can_mode_t mode
);

/***************************************************************************//**
  The CAN_start() function clears all pending interrupts and enable CAN
  controller to perform normal operation. It also enables receive interrupts.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          CAN_init(&g_can, CAN_SPEED_40M_1M, 0, 16u, 7u);
          CAN_set_mode(&g_can, CANOP_MODE_INT_LOOPBACK);
          CAN_start(&g_can);

          ....

          return(0);
      }
  @endcode
 */
void
CAN_start
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_stop() function is used to stop the CAN controller.

  NOTE: Interrupt flags status remain unaffected.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          CAN_init(&g_can, CAN_SPEED_40M_1M, 0, 16u, 7u);
          CAN_set_mode(&g_can, CANOP_MODE_INT_LOOPBACK);
          CAN_start(&g_can);

          ....
          ....

          CAN_stop(&g_can);

          return(0);
      }
  @endcode
 */
void
CAN_stop
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_enable_irq() function enable specific interrupts based on the irq_flag
  parameter.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param irq_flag
    The irq_flag parameter is a 4 byte variable that indicates the interrupt type.
    Following are the details of interrupt values:
    |  Constants             |  Description                                   |
    |------------------------|------------------------------------------------|
    | CAN_INT_GLOBAL         | Indicates to enable global interrupt           |
    | CAN_INT_ARB_LOSS       | Indicates arbitration loss interrupt           |
    | CAN_INT_OVR_LOAD       | Indicates overload message detected interrupt  |
    | CAN_INT_BIT_ERR        | Indicates bit error interrupt                  |
    | CAN_INT_STUFF_ERR      | Indicates bit stuffing error interrupt         |
    | CAN_INT_ACK_ERR        | Indicates acknowledge error interrupt          |
    | CAN_INT_FORM_ERR       | Indicates format error interrupt               |
    | CAN_INT_CRC_ERR        | Indicates CRC error interrupt                  |
    | CAN_INT_BUS_OFF        | Indicates bus off interrupt                    |
    | CAN_INT_RX_MSG_LOST    | Indicates received message lost interrupt      |
    | CAN_INT_TX_MSG         | Indicates message transmit interrupt           |
    | CAN_INT_RX_MSG         | Indicates receive message available interrupt  |
    | CAN_INT_RTR_MSG        | Indicates RTR auto-reply message sent interrupt|
    | CAN_INT_STUCK_AT_0     | Indicates stuck at dominant error interrupt    |
    | CAN_INT_OST_FAILURE    | Indicates one shot transmission failure        |
    |                        | interrupt                                      |

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          ....

          CAN_enable_irq(&g_can, CAN_INT_TX_MSG | CAN_INT_RX_MSG);

          ....
          return(0);
      }
  @endcode
 */
void
CAN_enable_irq
(
    can_instance_t * this_can,
    uint32_t irq_flag
);

/***************************************************************************//**
  The CAN_disable_irq() function disable specific interrupt based on irq_flag
  parameter.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param irq_flag
    The irq_flag parameter is a 4 byte variable that indicates the interrupt type.
    Following are the details of interrupt values:
    |  Constants             |  Description                                   |
    |------------------------|------------------------------------------------|
    | CAN_INT_GLOBAL         | Indicates to enable global interrupt           |
    | CAN_INT_ARB_LOSS       | Indicates arbitration loss interrupt           |
    | CAN_INT_OVR_LOAD       | Indicates overload message detected interrupt  |
    | CAN_INT_BIT_ERR        | Indicates bit error interrupt                  |
    | CAN_INT_STUFF_ERR      | Indicates bit stuffing error interrupt         |
    | CAN_INT_ACK_ERR        | Indicates acknowledge error interrupt          |
    | CAN_INT_FORM_ERR       | Indicates format error interrupt               |
    | CAN_INT_CRC_ERR        | Indicates CRC error interrupt                  |
    | CAN_INT_BUS_OFF        | Indicates bus off interrupt                    |
    | CAN_INT_RX_MSG_LOST    | Indicates received message lost interrupt      |
    | CAN_INT_TX_MSG         | Indicates message transmit interrupt           |
    | CAN_INT_RX_MSG         | Indicates receive message available interrupt  |
    | CAN_INT_RTR_MSG        | Indicates RTR auto-reply message sent interrupt|
    | CAN_INT_STUCK_AT_0     | Indicates stuck at dominant error interrupt    |
    | CAN_INT_OST_FAILURE    | Indicates one shot transmission failure        |
    |                        | interrupt                                      |

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          ....

          CAN_disable_irq(&g_can, CAN_INT_TX_MSG | CAN_INT_RX_MSG);

          ....
          return(0);
      }
  @endcode
 */
void
CAN_disable_irq
(
    can_instance_t * this_can,
    uint32_t irq_flag
);

/***************************************************************************//**
  The CAN_get_global_int_en() function returns the status of global interrupt
  enable flag.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns global interrupt enable flag status.

  @example
  @code
      int main(void)
      {
          ....

          CAN_get_global_int_en(&g_can);
          ....
          return(0);
      }
  @endcode
 */
uint32_t
CAN_get_global_int_en
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_int_en() function returns the status of interrupt enable flags.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns interrupt enable flag status.

  @example
  @code
      int main(void)
      {
          ....

          CAN_get_int_en(&g_can);
          ....
          return(0);
      }
  @endcode
 */
uint32_t
CAN_get_int_en
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_int_src() function returns current status of interrupts.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns current status of interrupts.

  @example
  @code
      int main(void)
      {
          ....

          CAN_get_int_src(&g_can);
          ....
          return(0);
      }
  @endcode
 */
uint32_t
CAN_get_int_src
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_clear_int_src() function clears the selected interrupt flags.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param irq_flag
    The irq_flag parameter is a 4 byte variable that indicates the interrupt type.
    Following are the details of interrupt values:
    |  Constants             |  Description                                   |
    |------------------------|------------------------------------------------|
    | CAN_INT_ARB_LOSS       | Indicates arbitration loss interrupt           |
    | CAN_INT_OVR_LOAD       | Indicates overload message detected interrupt  |
    | CAN_INT_BIT_ERR        | Indicates bit error interrupt                  |
    | CAN_INT_STUFF_ERR      | Indicates bit stuffing error interrupt         |
    | CAN_INT_ACK_ERR        | Indicates acknowledge error interrupt          |
    | CAN_INT_FORM_ERR       | Indicates format error interrupt               |
    | CAN_INT_CRC_ERR        | Indicates CRC error interrupt                  |
    | CAN_INT_BUS_OFF        | Indicates bus off interrupt                    |
    | CAN_INT_RX_MSG_LOST    | Indicates received message lost interrupt      |
    | CAN_INT_TX_MSG         | Indicates message transmit interrupt           |
    | CAN_INT_RX_MSG         | Indicates receive message available interrupt  |
    | CAN_INT_RTR_MSG        | Indicates RTR auto-reply message sent interrupt|
    | CAN_INT_STUCK_AT_0     | Indicates stuck at dominant error interrupt    |
    | CAN_INT_OST_FAILURE    | Indicates one shot transmission failure        |
    |                        | interrupt                                      |

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          ....
          CAN_clear_int_src(&g_can, CAN_INT_RX_MSG | CAN_INT_TX_MSG);
          ....
          return(0);
      }
  @endcode
 */
void
CAN_clear_int_src
(
    can_instance_t * this_can,
    uint32_t irq_flag
);

/***************************************************************************//**
  The CAN_set_rtr_message_f() function  loads mailbox with the given CAN
  message. This message will be sent out in response to an incoming RTR message
  request. It verifies that whether the given mailbox is configured for Full
  CAN or not and also checks whether RTR auto-reply is enabled or not.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @param pmsg
    The pmsg parameter is a pointer to the receiver message object. This
    structure is used either to configure the receive mailbox with filter and
    mask settings, or to hold data of a received CAN message or to provide
    the identifier and data that will be sent in response to an RTR (Remote
    Transmission Request), depending on the API usage.

  @return
    This function returns CAN_OK on successful execution, otherwise, it will
    return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
    |                       | basic CAN operation                         |
    | CAN_NO_RTR_MAILBOX    | Indicates that there is no mailbox for      |
    |                       | remote transmit request (RTR) frame         |

  @example
  @code
      int main(void)
      {
          ...

          CAN_set_rtr_message_f(&g_can, CAN_MB_5, &pmsg);

          ...
      }
  @endcode
 */
uint32_t
CAN_set_rtr_message_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_get_rtr_msg_abort_f() function aborts the transmit request of an RTR
  message for the specified mailbox number (mb_num) and checks whether the
  message abort was successful.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @return
    This function returns CAN_OK on successful execution, otherwise, it will
    return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_ERR               | Indicates error condition                   |
    | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
    |                       | basic CAN operation                         |

  @example
  @code
      int main(void)
      {
          ...

          ret_status = CAN_get_rtr_msg_abort_f(&g_can, CAN_MB_5);

          ...
      }
  @endcode
*/
uint32_t
CAN_get_rtr_msg_abort_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
);

/***************************************************************************//**
  The CAN_config_buffer() function configures the RX mailboxes initialized
  for basic CAN operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param pmsg
    The pmsg parameter is a pointer to the receiver message object. This
    structure is used either to configure the receive mailbox with filter and
    mask settings, or to hold data of a received CAN message, depending on
    the API usage.

  @return
    This function returns CAN_OK on successful execution, otherwise, it will
    return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_NO_MSG            | Indicates that there is no message received |

  @example
  @code
      int main(void)
      {

          ret_status = CAN_config_buffer(&g_can, &rx_msg);
          ...
      }
  @endcode
 */
uint32_t
CAN_config_buffer
(
    can_instance_t * this_can,
    can_rxmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_config_buffer_f() function  configures the Rx mailbox specified by the
  mailbox number(mb_num). The function checks that the mailbox is set for Full
  CAN operation. If not, then it returns with an error code.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @param pmsg
    The pmsg parameter is a pointer to the receiver message object. This
    structure is used either to configure the receive mailbox with filter and
    mask settings, or to hold data of a received CAN message, depending on
    the API usage.

  @return
    This function returns CAN_OK on successful execution, otherwise, it will
    return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
    |                       | basic CAN operation                         |

  @example
  @code
      int main(void)
      {
          ...

          ret_status = CAN_config_buffer_f(&g_can, CAN_MB_3, &rx_msg);
          ...
      }
  @endcode
*/
uint32_t
CAN_config_buffer_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_get_msg() function read message from the first mailbox set for
  basic CAN  operation that contains a message. Once the message has been read
  from the mailbox, the message receipt is acknowledged.
  Note: Since neither a hardware nor a software FIFO exists, message inversion
        might happen (for example, a newer message might be read from the
        receive buffer prior to an older message).

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param pmsg
    The pmsg parameter is a pointer to the receiver message object. This
    structure is used either to configure the receive mailbox with filter and
    mask settings, or to hold data of a received CAN message, depending on
    the API usage.

  @return
    This function returns CAN_VALID_MSG on successful execution, otherwise, it
    will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_NO_MSG            | Indicates that there is no message received |

  @example
  @code
      int main(void)
      {
          ...
          ...
          CAN_get_msg(&g_can, &rx_buf);
          ...
      }
  @endcode
 */

uint32_t
CAN_get_msg
(
    can_instance_t * this_can,
    can_rxmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_get_msg_f() function read message from the Rx mailbox specified in
  mailbox number (mb_num) parameter and returns the status of operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @param pmsg
    The pmsg parameter is a pointer to the receiver message object. This
    structure is used either to configure the receive mailbox with filter and
    mask settings, or to hold data of a received CAN message, depending on
    the API usage.

  @return
    This function returns CAN_VALID_MSG on successful execution, otherwise, it
    will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_NO_MSG            | Indicates that there is no message received |
    | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
    |                       | basic CAN operation                         |

  @example
  @code
      int main(void)
      {
          ...

          ret_status = CAN_get_msg_f(&g_can, CAN_MB_3, &rxmsg);
          ...
      }
  @endcode
 */

uint32_t
CAN_get_msg_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_rx_msg_avail() function indicates if the Rx buffer contains a new
  message in the basic CAN operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns CAN_VALID_MSG on successful execution, otherwise, it
    will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_NO_MSG            | Indicates that there is no message received |

  @example
  @code
      int main(void)
      {
          ...
          ...
          if(CAN_VALID_MSG == CAN_rx_msg_avail(&g_can))
          {
             CAN_get_msg(&g_can, &rx_buf);
          }
          ...
      }
  @endcode
 */
uint32_t
CAN_rx_msg_avail
(
    can_instance_t* this_can
);

/***************************************************************************//**
  The CAN_rx_msg_avail_f() function indicates if the Rx buffer, specified by
  the mailbox number (mb_num), contains a new message in Full CAN operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @return
    This function returns CAN_VALID_MSG on successful execution, otherwise, it
    will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_NO_MSG            | Indicates that there is no message received |
    | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
    |                       | basic CAN operation                         |

  @example
  @code
      int main(void)
      {
          ...
          ...
          if(CAN_VALID_MSG == CAN_rx_msg_avail_f(&g_can, CAN_MB_3))
          {
             CAN_get_msg_f(&g_can, CAN_MB_3, &rx_buf);
          }
          ...
      }
  @endcode
 */
uint32_t
CAN_rx_msg_avail_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
);

/***************************************************************************//**
  The CAN_get_amcr_mask() function returns the message filter settings of the
  selected Rx mailbox specified by the mailbox number (mb_num). The function is
  valid for Full CAN operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @param pmsg
    The pmsg parameter is a pointer to the receiver message object. This
    structure is used either to configure the receive mailbox with filter and
    mask settings, or to hold data of a received CAN message, depending on
    the API usage.

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          ...
          ...
          CAN_get_amcr_mask_f(&g_can, CAN_MB_3, &rx_buf);
          ...
      }
  @endcode
*/
void
CAN_get_amcr_mask_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_set_amcr_mask_f() function configures the message filter settings of
  the Rx mailbox specified by the mailbox number (mb_num). The function is
  valid for Full CAN operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @param pmsg
    The pmsg parameter is a pointer to the receiver message object. This
    structure is used either to configure the receive mailbox with filter and
    mask settings, or to hold data of a received CAN message, depending on
    the API usage.

  @return
    This function does not return a value.

  @example
  @code
      int main(void)
      {
          ...
          ...
          CAN_set_amcr_mask_f(&g_can, CAN_MB_3, &rx_buf);
          ...
      }
  @endcode
*/
void
CAN_set_amcr_mask_f
(
    can_instance_t* this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_send_msg() function will copy the data to the first available Tx
  mailbox set for basic CAN operation and send data on to the bus.
  Note: Since neither a hardware nor a software FIFO exists, message inversion
  might happen (for example, a newer message might be sent from the transmit
  buffer prior to an older message).

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param pmsg
    The pmsg parameter is a pointer to the transmit message object that holds
    the CAN message.

  @return
    This function returns CAN_VALID_MSG on successful execution, otherwise, it
    will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_NO_MSG            | Indicates that there is no message received |

  @example
  @code
      int main(void)
      {
          ...
          ...

          CAN_send_msg(&g_can, &tx_msg);
          ...
      }
  @endcode
 */
uint32_t
CAN_send_msg
(
    can_instance_t * this_can,
    can_txmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_send_msg_f() function sends a message using the Tx mailbox specified
  by the mailbox number (mb_num). The function verifies that this message is
  configured for Full CAN operation and is empty.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @param pmsg
    The pmsg parameter is a pointer to the transmit message object that holds
    the CAN message.

  @return
    This function returns CAN_VALID_MSG on successful execution, otherwise, it
    will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_NO_MSG            | Indicates that there is no message received |
    | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
    |                       | Basic CAN operation                         |

  @example
  @code
      int main(void)
      {
          ...
          ...

          CAN_send_msg_f(&g_can, CAN_MB_0, &txmsg);
          ...
      }
  @endcode
 */
uint32_t
CAN_send_msg_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_txmsgobject_t *pmsg
);

/***************************************************************************//**
  The CAN_abort_tx_msg_f() function aborts the message transmit request for the
  specified mailbox number (mb_num) and checks the status of the message abort.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @param mb_num
    The mb_num parameter specifies the mailbox number used for message
    operations. It must be a value from the can_mb_t enumeration (CAN_MB_0 to
    CAN_MB_31), which represents one of the 32 available mailboxes in the CAN
    peripheral.

  @return
    This function returns CAN_OK on successful execution, otherwise, it
    will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_ERR               | Indicates error condition                   |
    | CAN_BASIC_CAN_MAILBOX | Indicates that mailbox is configured for    |
    |                       | basic CAN operation                         |

  @example
  @code
      int main(void)
      {
          ...
          ...
          if (CAN_OK != CAN_abort_tx_msg_f(&g_can, CAN_MB_6))
          {
             ...
          }
          ...
      }
  @endcode
 */
uint32_t
CAN_abort_tx_msg_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
);

/***************************************************************************//**
  The CAN_tx_msg_avail() function identifies the availability of a Tx mailbox
  to fill with a new message in Basic CAN operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns CAN_OK on successful identification of free mailbox,
    otherwise, it will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_ERR               | Indicates error condition                   |

  @example
  @code
      int main(void)
      {
          ...
          ...

          if(CAN_OK == CAN_tx_msg_avail(&g_can))
          {
              CAN_send_msg(&g_can, &txmsg);
          }
          ...
      }
  @endcode
 */
uint32_t
CAN_tx_msg_avail
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_tx_msg_avail_f() function identifies the availability of the Tx
  mailbox specified by the mailbox number (mb_num) to fill the new message in
  the Full CAN operation.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns CAN_OK on successful identification of free mailbox,
    otherwise, it will return the following error codes:
    |  Constants            |  Description                                |
    |-----------------------|---------------------------------------------|
    | CAN_ERR               | Indicates error condition                   |

  @example
  @code
      int main(void)
      {
          ...
          ...

          if(CAN_OK == CAN_tx_msg_avail_f(&g_can, CAN_MB_6))
          {
              CAN_send_msg_f(&g_can, CAN_MB_6, &txmsg);
          }
          ...
      }
  @endcode
 */
uint32_t
CAN_tx_msg_avail_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
);

/***************************************************************************//**
  The CAN_get_rx_buff_sts() function returns the status of the buffer for all
  32 Rx messages.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns status of the Rx buffers (32 buffers).

  @example
  @code
      int main(void)
      {
          ...
          uint32_t return_status=0;
          ...
          return_status = CAN_get_rx_buff_sts(&g_can);
          ...
      }
  @endcode
 */
uint32_t
CAN_get_rx_buff_sts
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_tx_buff_sts() function returns the status of the buffer for all
  32 Tx messages.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns the status of the Tx buffers (32 buffers).

  @example
  @code
      int main(void)
      {
          ...
          uint32_t return_status = 0;
          ...
          return_status = CAN_get_tx_buff_sts(&g_can);
          ...
      }
  @endcode
*/
uint32_t
CAN_get_tx_buff_sts
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_rx_err_cnt() function returns the current receive error counter
  value. Counter value ranges from 0x00 to 0xFF.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns the receive error counter value.

  @example
  @code
      int main(void)
      {
          ...
          uint32_t return_status = 0;
          ...
          return_status = CAN_get_rx_err_cnt(&g_can);
          ...
      }
  @endcode
 */
uint32_t
CAN_get_rx_err_cnt
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_tx_err_cnt() function returns the current transmit error counter
  value. Counter value ranges from 0x00 to 0xFF.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns the transmit error counter value.

  @example
  @code
      int main(void)
      {
          ...
          uint32_t return_status = 0;
          ...
          return_status = CAN_get_tx_err_cnt(&g_can_0_lo);
          ...
      }
  @endcode
 */
uint32_t
CAN_get_tx_err_cnt
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_err_sts() function returns the present error state of the CAN
  controller. Error state might be active error or passive error or bus-off
  state.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    The function returns the following codes:
    | Codes  |  Descriptions                 |
    |--------|-------------------------------|
    |  0     | Active error state            |
    |  1     | Passive error state           |
    |  2     | Bus-off state                 |

  @example
  @code
      int main(void)
      {
          ...
          uint8_t return_status = 0;
          ...
          return_status = CAN_get_err_sts(&g_can);
          ...
      }
  @endcode
 */
uint32_t
CAN_get_err_sts
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_rx_gte96() function provides information about the receive error
  count. It identifies that the receive error count is greater than or equal to
  96 and reports 1 if the count exceeds 96.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns the following values:
    | Value |  Description                                            |
    |-------|---------------------------------------------------------|
    |  0    | If receive error count is less than 96.                 |
    |  1    | If receive error count is greater than or equals to 96. |

  @example
  @code
      int main(void)
      {
          ...
          uint32_t return_status = 0;
          ...
          return_status = CAN_get_rx_gte96(&g_can);
          ...
      }
  @endcode
 */
uint32_t
CAN_get_rx_gte96
(
    can_instance_t * this_can
);

/***************************************************************************//**
  The CAN_get_tx_gte96() function provides information about transmit error
  count. It identifies that transmit error count is greater than or equals to
  96, and reports 1 if count exceeds 96.

  @param this_can
    The this_can parameter is a pointer to a can_instance_t structure that
    holds all data related to the CoreCAN instance being initialized. A
    pointer to this data structure is used in all subsequent calls to the CAN
    driver functions that operate on this CoreCAN instance.

  @return
    This function returns the following values:
    | Value |  Description                                             |
    |-------|----------------------------------------------------------|
    |  0    | If transmit error count is less than 96.                 |
    |  1    | If transmit error count is greater than or equals to 96. |

  @example
  @code
      int main(void)
      {
          ...
          uint32_t return_status = 0;
          ...
          return_status = CAN_get_tx_gte96(&g_can);
          ...
      }
  @endcode
 */
uint32_t
CAN_get_tx_gte96
(
    can_instance_t * this_can
);

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CAN_H */
