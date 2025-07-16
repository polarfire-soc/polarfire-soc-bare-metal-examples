/*******************************************************************************
 * (c) Copyright 2025 Microchip FPGA Embedded Systems Solutions.
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
 * @file core_axi4protoconv.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief This file contains the application programming interface for the
 * CoreAXI4ProtoConv IP bare metal driver.
 *
 */
/*=========================================================================*//**
  @mainpage CoreAXI4ProtoConv Bare Metal Driver.

==============================================================================
  Introduction
==============================================================================

  CoreAXI4ProtoConv is an IP component and is designed for high throughput data
  transfer between the AXI4 memory-mapped and AXI4-stream interfaces.
  CoreAXI4ProtoConv supports the Memory Mapped to Stream (MM2S) and Stream to
  Memory Mapped (S2MM) conversion independently in a full-duplex like method.
  AXI4-Lite target interface is used for register configuration.

  This driver provides a set of functions for controlling CoreAXI4ProtoConv as
  part of the bare metal system where no operating system is available. This
  driver can be adapted to be used as a part of an operating system, but the
  implementation of the adaptation layer between the driver and the operating
  system's driver model is outside the scope of this driver.

  Note: PCDMA is used as the short form for CoreAXI4ProtoConv throughout the
        driver or document.

  ==============================================================================
  Driver Configuration
  ==============================================================================
  Your application software must configure the PCDMA driver by calling the
  PCDMA_init() function for each CoreAXI4ProtoConv instance in the hardware
  design. PCDMA_init() function configures CoreAXI4ProtoConv hardware instance
  base address. The PCDMA_init() function must be called before any other PCDMA
  driver functions can be called.

  ==============================================================================
  Theory of Operation
  ==============================================================================
  The PCDMA software driver is designed to control the multiple instances of
  CoreAXI4ProtoConv IP. Each instance of CoreAXI4ProtoConv in the hardware
  design is associated with a single instance of the PCDMA_instance_t structure
  in the software. The contents of these data structures are initialized by
  calling the PCDMA_init() function. A pointer to the structure is passed to
  subsequent driver functions in order to identify the CoreAXI4ProtoConv
  hardware instance you wish to perform the requested operation on.

  Note 1: Do not attempt to directly manipulate the content of PCDMA_instance_t
          structures. This structure is only intended to be modified by the
          driver function.

  Note 2: PCDMA_init() function initializes both the Stream to Memory Map (S2MM)
          and Memory Map to Stream (MM2S) blocks.

  CoreAXI4ProtoConv has two main blocks:
    - Stream to Memory Map (S2MM)
    - Memory Map to Stream (MM2S)

  ----------------------------
  Stream to Memory Map (S2MM)
  ----------------------------
  The S2MM block is responsible for transmitting transactions between the AXI4-
  Stream Interface and AXI4-Memory Mapped Interface. It has its dedicated sets
  of registers to load command and read the status of the corresponding commands.
  S2MM block supports queuing both commands and status. Registers can be
  accessed through AXI4-Lite interface.

  Based upon the command inputs and input data from the AXI4-Stream interface,
  the S2MM block issues a write request on the AXI4-Memory Mapped interface.
  Input stream data can be optionally stored inside a data FIFO. Data FIFO
  supports optional store and forward mode or packet mode in which data is not
  read from the FIFO until complete packet is received from the AXI4-Stream
  interface.

  The following functions are used to perform S2MM data transfer:
    - PCDMA_S2MM_configure()
    - PCDMA_S2MM_start_transfer()

  To initiate an S2MM (Stream to Memory Map) data transfer, first call the
  PCDMA_S2MM_configure() function to configure transfer size (indicates the
  total number of bytes to be transferred), 64-bit address, command ID, and
  burst type. After configuration, the PCDMA_S2MM_start_transfer() function
  begins the data transfer from the AXI4-Stream interface to the AXI4-Memory
  Mapped interface.

  When S2MM Undefined Burst Length Enable feature is enabled in the
  CoreAXI4ProtoConv IP configurator, see the PCDMA_S2MM_get_len() function
  for additional details.

  PCDMA_S2MM_get_status() function is used to get the current status of the S2MM
  data transfer.

  The following functions are used to enable, disable and clear specific S2MM
  interrupt by using the irq_type parameter respectively.

    - PCDMA_S2MM_enable_irq()
    - PCDMA_S2MM_disable_irq()
    - PCDMA_S2MM_clr_int_src()

  Additionally, the PCDMA_S2MM_get_int_src() function is used to get the current
  status of the S2MM data transfer when the interrupt method is used.

  Note: When using the interrupt method, either the S2MM interrupt source
        register or the S2MM status register should be read, but not both
        simultaneously.
  ----------------------------
  Memory Map to Stream (MM2S)
  ----------------------------
  The MM2S block is responsible for transactions from the AXI4-Memory Mapped
  Interface to AXI4-Stream interface. It has its dedicated sets of registers
  to command and read the status of the corresponding commands. MM2S block supports
  queuing both commands and status. Registers can be accessed through AXI4-Lite
  interface.

  Based on command inputs, the MM2S block issues a read request on the AXI4-
  Memory Mapped interface. Read data can be optionally stored inside the data
  FIFO. Data FIFO supports optional store and forward mode or packet mode in
  which data is not read from the FIFO until complete packet is received from
  the AXI4-Memory Mapped interface that is, number of bytes configured in MM2S
  Length Register are received.

  The following functions are used to perform the MM2S data transfer:
    - PCDMA_MM2S_configure()
    - PCDMA_MM2S_start_transfer()

  To initiate an MM2S (Memory Map to Stream) data transfer, first call the
  PCDMA_MM2S_configure() function to configure transfer size (indicates the
  total number of bytes to be transferred), 64-bit address, command ID, and
  burst type. After the configuration, the PCDMA_MM2S_start_transfer() function
  begins the data transfer from the AXI4-Memory Mapped interface and AXI4-Stream
  interface.

  PCDMA_MM2S_get_status() function is used to get the current status of the MM2S
  data transfer.

  The following functions are used to enable, disable and clear the specific MM2S
  interrupt by using the irq_type parameter respectively.

    - PCDMA_MM2S_enable_irq()
    - PCDMA_MM2S_disable_irq()
    - PCDMA_MM2S_clr_int_src()

  Additionally, the PCDMA_MM2S_get_int_src() function used to get the current
  status of the MM2S data transfer when the interrupt method is used.

  Note: When using the interrupt method, either the MM2S interrupt source
        register or the MM2S status register should be read, but not both
		simultaneously.

*//*=========================================================================*/

#ifndef __CORE_AXI4PROTOCONV_H
#define __CORE_AXI4PROTOCONV_H    1

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#include "hal_assert.h"
#endif

/*-------------------------------------------------------------------------*//**
  S2MM Interrupt Identifier
  ============================
  The following constants specify the interrupt identifier number, which is
  specifically used by the driver API. The S2MM_IE_DONE_IRQ,
  S2MM_IE_AXI4_ERR_IRQ, S2MM_IE_PKT_DROP_OVF_IRQ and S2MM_IE_PKT_DROP_ERR_IRQ
  are used by S2MM enable interrupt, S2MM disable interrupt and S2MM clear
  interrupt functions.

  | Constant                 | Description                                      |
  |--------------------------|--------------------------------------------------|
  | S2MM_IE_DONE_IRQ         | S2MM done interrupt                              |
  | S2MM_IE_AXI4_ERR_IRQ     | S2MM AXI4 bus error is detected                  |
  | S2MM_IE_PKT_DROP_OVF_IRQ | S2MM packet is dropped due to FIFO overflow      |
  | S2MM_IE_PKT_DROP_ERR_IRQ | S2MM packet is dropped due to error detection in |
  |                          | the received packet.                             |

 */
#define S2MM_IE_DONE_IRQ             0x01u
#define S2MM_IE_AXI4_ERR_IRQ         0x04u
#define S2MM_IE_PKT_DROP_OVF_IRQ     0x08u
#define S2MM_IE_PKT_DROP_ERR_IRQ     0x10u

/*-------------------------------------------------------------------------*//**
  MM2S Interrupt Identifier
  =======================================
  The following constants specify the interrupt identifier number, which is
  specifically used by the driver API. The MM2S_IE_DONE_IRQ and
  MM2S_IE_AXI4_ERR_IRQ are used by MM2S enable interrupt, MM2S disable interrupt
  and MM2S clear interrupt functions.

  | Constant                 | Description                                      |
  |--------------------------|--------------------------------------------------|
  | MM2S_IE_DONE_IRQ         | MM2S done interrupt                              |
  | MM2S_IE_AXI4_ERR_IRQ     | MM2S AXI4 bus error is detected                  |

 */
#define MM2S_IE_DONE_IRQ             0x01u
#define MM2S_IE_AXI4_ERR_IRQ         0x04u

/***************************************************************************//**
 * This structure instance identifies various CoreAXI4ProtoConv hardware
 * instances within your system. The PCDMA_init() function initializes this
 * structure. A pointer to an initialized instance of the structure should be
 * provided as the first parameter to the PCDMA driver functions to
 * identify which CoreAXI4ProtoConv should perform the requested operation.
 */

typedef struct PCDMA_instance
{
    addr_t    base_address;
}PCDMA_instance_t;

/***************************************************************************//**
 * This enumeration is used to program the burst type, which can be either
 * fixed burst or increment burst. It is used as a parameter to the
 * PCDMA_S2MM_configure() function.

  @example
  @code
  PCDMA_S2MM_configure(&g_pcdma, 0x20, 0x80000000, 0xa1, PCDMA_BURST_TYPE_FIXED);
  @endcode
 */

typedef enum PCDMA_burst_type
{
    PCDMA_BURST_TYPE_FIXED = 0u,
    PCDMA_BURST_TYPE_INCR  = 1u
}PCDMA_burst_type_t;

/*------------------------Public Function-------------------------------------*/

/***************************************************************************//**
  The PCDMA_init() function initializes the driver. It sets the base address for
  the CoreAXI4ProtoConv instance and initializes the PCDMA_instance_t data
  structure. This function must be called before any other CoreAXI4ProtoConv
  driver functions are called.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param base_addr
  The base_addr parameter is the base address in the processor's memory map for
  the registers of the CoreAXI4ProtoConv instance being initialized.

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR            0x60030000u

    PCDMA_instance_t  g_pcdma;

    void system_init( void )
    {
       PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    }
  @endcode
 */
void
PCDMA_init
(
    PCDMA_instance_t  * this_pcdma,
    addr_t base_addr
);

/***************************************************************************//**
  The PCDMA_S2MM_configure() function is used to configure the transfer size
  (indicates the total number of bytes to be transferred), 64-bit address,
  command ID, and burst type.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param xfr_size
  The xfr_size parameter specifies the total number of bytes to be transferred
  from AXI4-Stream interface to the AXI4-Memory Mapped interface (S2MM).  
  Note 1: xfr_size parameter must not program zero value.  
  Note 2: When S2MM Undefined Burst Length Enable feature is enabled within the
         CoreAXI4ProtoConv IP configurator, configuring the xfr_size will have
         no effect. PCDMA_S2MM_get_len() provides the number of bytes received
         in an undefined burst transaction.

  @param src_add
  The src_add parameter specifies the 64-bit address of the AXI4-Memory Mapped
  interface.

  @param cmd_id
  The cmd_id parameter allows you to assign an arbitrary value. After the S2MM
  transfer completes, the CoreAXI4ProtoConv IP inserts cmd_id value into the
  S2MM status register or interrupt source register when interrupt method is
  used.

  @param burst_type
  The burst_type parameter allows you to specify the burst type, which can be
  either fixed burst or increment burst.  
  Note 1: Fixed burst transfer should be used to access peripheral device which
  supports fixed address like FIFO.  
  Note 2: Increment burst transfer should be used to access peripheral device,
  which supports incremental address like SRAM.

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                               S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
  @endcode
 */
void PCDMA_S2MM_configure
(
    PCDMA_instance_t  * this_pcdma,
    uint32_t xfr_size,
    uint64_t src_add,
    uint16_t cmd_id,
    uint8_t burst_type
);

/***************************************************************************//**
  The PCDMA_S2MM_start_transfer() function is used to initiate the S2MM data
  transfer from the AXI4-Stream interface to the AXI4-Memory Mapped interface.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                       S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_S2MM_start_transfer(&g_pcdma);
  @endcode
 */
void PCDMA_S2MM_start_transfer
(
    PCDMA_instance_t  * this_pcdma
);

/***************************************************************************//**
  The PCDMA_S2MM_get_status() function is used to know the current status of
  the data transfer.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @return
  This function returns the current value of the S2MM status register.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                       S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_S2MM_start_transfer(&g_pcdma);
    uint32_t reg_value = PCDMA_S2MM_get_status(&g_pcdma);
  @endcode
 */
uint32_t PCDMA_S2MM_get_status
(
    PCDMA_instance_t  * this_pcdma
);

/***************************************************************************//**
  The PCDMA_S2MM_enable_irq() function enables the interrupts specified by the
  irq_type parameter.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param irq_type
  The irq_type parameter specify the type of interrupt(s). The possible
  interrupts are:
      - S2MM_IE_DONE_IRQ
      - S2MM_IE_AXI4_ERR_IRQ
      - S2MM_IE_PKT_DROP_OVF_IRQ
      - S2MM_IE_PKT_DROP_ERR_IRQ  
  To enable multiple interrupts in single call, use
      S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ.

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_enable_irq(&g_pcdma, S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                       S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_S2MM_start_transfer(&g_pcdma);
  @endcode
 */
void PCDMA_S2MM_enable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
);

/***************************************************************************//**
  The PCDMA_S2MM_disable_irq() function disables the interrupts specified by the
  irq_type parameter.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param irq_type
  The irq_type parameter specify the type of interrupt(s). The possible
  interrupts are:
      - S2MM_IE_DONE_IRQ
      - S2MM_IE_AXI4_ERR_IRQ
      - S2MM_IE_PKT_DROP_OVF_IRQ
      - S2MM_IE_PKT_DROP_ERR_IRQ  
  To disable multiple interrupts in single call, use
      S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ.

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_enable_irq(&g_pcdma, S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                       S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_S2MM_start_transfer(&g_pcdma);
    PCDMA_S2MM_disable_irq(&g_pcdma, S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ);
  @endcode
 */
void PCDMA_S2MM_disable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
);

/***************************************************************************//**
  The PCDMA_S2MM_get_int_src() function is used to know the current status of
  the data transfer when interrupt method is used.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @return
  This function returns the current value of the S2MM interrupt source register.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_enable_irq(&g_pcdma, S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                       S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_S2MM_start_transfer(&g_pcdma);
    uint32_t reg_value = PCDMA_S2MM_get_int_src(&g_pcdma);
  @endcode
 */
uint32_t PCDMA_S2MM_get_int_src
(
   PCDMA_instance_t  * this_pcdma
);

/***************************************************************************//**
  The PCDMA_S2MM_clr_int_src() function clears the interrupts specified by the
  irq_type parameter.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param irq_type
  The irq_type parameter specify the type of interrupt(s). The possible
  interrupts are:
      - S2MM_IE_DONE_IRQ
      - S2MM_IE_AXI4_ERR_IRQ
      - S2MM_IE_PKT_DROP_OVF_IRQ
      - S2MM_IE_PKT_DROP_ERR_IRQ  
  To clear multiple interrupts in single call, use
      S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_enable_irq(&g_pcdma, S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                       S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_S2MM_start_transfer(&g_pcdma);
    PCDMA_S2MM_clr_int_src(&g_pcdma, S2MM_IE_DONE_IRQ | S2MM_IE_AXI4_ERR_IRQ);
  @endcode
 */
void PCDMA_S2MM_clr_int_src
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
);
#ifdef IP_Config_S2MM_UNDEFINED_BURST_LENGTH_ENABLE

/***************************************************************************//**
  The PCDMA_S2MM_get_len() function used to know the number of received bytes
  when undefined burst transaction enabled in the CoreAXI4ProtoConv IP
  configurator.

  Note: This API must be used when undefined burst transaction is enabled in the
        CoreAXI4ProtoConv IP configurator.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @return
  This function returns value representing number of bytes received in the
  undefined burst transaction.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define S2MM_CMD_ID                                 0xb6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_S2MM_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                       S2MM_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_S2MM_start_transfer(&g_pcdma);
    uint32_t reg_value = PCDMA_S2MM_get_len(&g_pcdma);
  @endcode
 */
uint32_t PCDMA_S2MM_get_len
(
   PCDMA_instance_t  * this_pcdma
);
#endif

/***************************************************************************//**
 The PCDMA_MM2S_configure() function is used to configure the transfer size
 (indicates the total number of bytes to be transferred), 64-bit address,
 command ID, and burst type.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param xfr_size
  The xfr_size parameter specifies the total number of bytes to be transferred
  from AXI4-Memory Mapped interface to the AXI4-Stream interface (MM2S).  
  Note: xfr_size parameter must not program zero value.

  @param src_add
  The src_add parameter specifies the 64-bit address of the AXI4-Memory Mapped
  interface.

  @param cmd_id
  The cmd_id parameter allows you to assign an arbitrary value. After the MM2S
  transfer completes, the CoreAXI4ProtoConv IP inserts cmd_id value into the
  MM2S status register or interrupt source register when interrupt method is
  used.

  @param burst_type
  The burst_type parameter allows you to specify the burst type, which can be
  either fixed burst or increment burst.  
  Note 1: Fixed burst transfer should be used to access peripheral device which
  supports fixed address like FIFO.  
  Note 2: Increment burst transfer should be used to access peripheral device,
  which supports incremental address like SRAM.

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define MM2S_CMD_ID                                 0xa6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_MM2S_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                          MM2S_CMD_ID, PCDMA_BURST_TYPE_INCR);
  @endcode
 */
void PCDMA_MM2S_configure
(
    PCDMA_instance_t  * this_pcdma,
    uint32_t xfr_size,
    uint64_t src_add,
    uint16_t cmd_id,
    uint8_t burst_type
);

/***************************************************************************//**
  The PCDMA_MM2S_start_transfer() function is used to initiate the S2MM data
  transfer from the AXI4-Memory Mapped interface to the AXI4-Stream interface.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define MM2S_CMD_ID                                 0xa6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_MM2S_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                          MM2S_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_MM2S_start_transfer(&g_pcdma);
  @endcode
 */
void PCDMA_MM2S_start_transfer
(
    PCDMA_instance_t  * this_pcdma
);

/***************************************************************************//**
  The PCDMA_MM2S_get_status() function is used to know the current status of
  the data transfer.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @return
  This function returns the current value of the MM2S status register.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define MM2S_CMD_ID                                 0xa6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_MM2S_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                          MM2S_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_MM2S_start_transfer(&g_pcdma);
    uint32_t reg_value = PCDMA_MM2S_get_status(&g_pcdma);
  @endcode
 */
uint32_t PCDMA_MM2S_get_status
(
    PCDMA_instance_t  * this_pcdma
);

/***************************************************************************//**
  The PCDMA_MM2S_enable_irq() function enables the interrupts specified by the
  irq_type parameter.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this
  CoreAXI4ProtoConv instance.

  @param irq_type
  The irq_type parameter specify the type of interrupt(s). The possible
  interrupts are:
      - MM2S_IE_DONE_IRQ
      - MM2S_IE_AXI4_ERR_IRQ  
  To enable both interrupts in single call, use
      MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define MM2S_CMD_ID                                 0xa6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_MM2S_enable_irq(&g_pcdma, MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ);
    PCDMA_MM2S_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                          MM2S_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_MM2S_start_transfer(&g_pcdma);
  @endcode
 */
void PCDMA_MM2S_enable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
);

/***************************************************************************//**
  The PCDMA_MM2S_disable_irq() function disables the interrupts specified by the
  irq_type parameter.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param irq_type
  The irq_type parameter specify the type of interrupt(s). The possible
  interrupts are:
      - MM2S_IE_DONE_IRQ
      - MM2S_IE_AXI4_ERR_IRQ  
  To disable both interrupts in single call, use
      MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define MM2S_CMD_ID                                 0xa6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_MM2S_enable_irq(&g_pcdma, MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ);
    PCDMA_MM2S_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                          MM2S_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_MM2S_start_transfer(&g_pcdma);
    PCDMA_MM2S_disable_irq(&g_pcdma, MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ);
  @endcode
 */
void PCDMA_MM2S_disable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
);

/***************************************************************************//**
  The PCDMA_MM2S_get_int_src() function is used to know the current status of
  the data transfer when interrupt method is used.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @return
  This function returns the current value of the MM2S interrupt source register.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define MM2S_CMD_ID                                 0xa6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_MM2S_enable_irq(&g_pcdma, MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ);
    PCDMA_MM2S_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                          MM2S_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_MM2S_start_transfer(&g_pcdma);
    uint32_t reg_value = PCDMA_MM2S_get_int_src(&g_pcdma);
  @endcode
 */


uint32_t PCDMA_MM2S_get_int_src
(
   PCDMA_instance_t  * this_pcdma
);

/***************************************************************************//**
  The PCDMA_MM2S_clr_int_src() function clears the interrupts specified by the
  irq_type parameter.

  @param this_pcdma
  The this_pcdma parameter is a pointer to a PCDMA_instance_t structure that
  holds all the data related to the CoreAXI4ProtoConv instance being
  initialized. A pointer to this data structure is used in all subsequent calls
  to the PCDMA driver functions that operate on this CoreAXI4ProtoConv instance.

  @param irq_type
  The irq_type parameter specify the type of interrupt(s). The possible
  interrupts are:
      - MM2S_IE_DONE_IRQ
      - MM2S_IE_AXI4_ERR_IRQ  
  To disable both interrupts in single call, use
      MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ

  @return
  This function does not return any value.

  @example
  @code
    #define COREAXI4PROTOCONV_BASE_ADDR                 0x60030000u
    #define DDR_BASE_ADDR                               0x80010000u
    #define MM2S_CMD_ID                                 0xa6
    #define TRANSFER_SIZE_BYTES                         1000

    PCDMA_instance_t  g_pcdma;

    PCDMA_init(&g_pcdma, COREAXI4PROTOCONV_BASE_ADDR);
    PCDMA_MM2S_enable_irq(&g_pcdma, MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ);
    PCDMA_MM2S_configure(&g_pcdma, TRANSFER_SIZE_BYTES, DDR_BASE_ADDR, \
                                          MM2S_CMD_ID, PCDMA_BURST_TYPE_INCR);
    PCDMA_MM2S_start_transfer(&g_pcdma);
    PCDMA_MM2S_clr_int_src(&g_pcdma, MM2S_IE_DONE_IRQ | MM2S_IE_AXI4_ERR_IRQ);
  @endcode
 */
void PCDMA_MM2S_clr_int_src
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
);

#ifdef __cplusplus
}
#endif

#endif /* __CORE_AXI4PROTOCONV_H */
