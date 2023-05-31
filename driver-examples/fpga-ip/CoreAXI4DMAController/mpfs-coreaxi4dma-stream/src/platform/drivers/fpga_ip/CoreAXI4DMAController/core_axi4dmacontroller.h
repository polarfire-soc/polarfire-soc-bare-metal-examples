/*******************************************************************************
 * Copyright 2021 Microchip FPGA Embedded Systems Solutions.
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
 *
 * CoreAXI4DMAController bare metal software driver public API.
 *
 */

/*=========================================================================*//**
  @mainpage CoreAXI4DMAController Bare Metal Driver

  ==============================================================================
  Introduction
  ==============================================================================
  The CoreAXI4DMAController is an AXI4 DMA controller designed to perform memory
  to memory style DMA (Direct Memory Access) transfers in an AXI system. The core
  provides in-built flow control techniques to ensure that the bandwidth of the
  AXI interface is optimally utilized. In addition, the core provides a bridge
  to AXI4 memory-mapped slaves for AXI4-Stream masters. The CoreAXI4DMAController
  bare metal software driver is designed for use in systems with no operating
  system adapted for use as part of an operating system but the implementation
  of the adaptation layer between this driver and the operating system's driver
  model is outside the scope of this driver.

  **NOTE** This is an early version of the driver. Presently, only the streaming
  and interrupt handling APIs are supported. All other APIs are work in progress.
  Please see API descriptions for more information.

  ==============================================================================
  Hardware Flow Dependencies
  ==============================================================================
  There are certain parameters in the CoreAXI4DMAController IP which are user
  configurable at the IP instantiation time. The coreaxi4dmacontroller_user_config.h
  has the equivalent constants for those parameters which are relevant for the
  this driver.

  ==============================================================================
  Theory of Operation
  ==============================================================================
  The MSS GPIO driver functions are grouped into the following categories:
    - Initialization
    - Configuration
    - Reading and setting GPIO state
    - Interrupt control
    
  --------------------------------
  Initialization
  --------------------------------

  --------------------------------
  Configuration
  --------------------------------

  --------------------------------
  Reading and Setting GPIO State
  --------------------------------

  --------------------------------
  Interrupt Control
  --------------------------------
  The number of interrupt outputs and priority levels (when more than one
  interrupt output is selected) are configurable in the IP configurator in the
  Libero design. Interrupt for each descriptor can be associated with any of the
  available interrupt output and this configuration is also accomplished via
  IP configurator. Note also that a multiple descriptors can be associated with
  a single interrupt output.

  When the interrupt is asserted, the AXI4DMA_transfer_status() function can be
  used to find out which event on a descriptor associated with the interrupt
  number has caused this interrupt. The AXI4DMA_transfer_status() can also be
  used with stream transfer events.

 *//*=========================================================================*/
#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#endif

#include "coreaxi4dmacontroller_user_config.h"
#include "coreaxi4dmacontroller_regs.h"

#ifndef CORE_AXI4DMACTRL_H_
#define CORE_AXI4DMACTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
/* Public type definitions                                                     */
/*******************************************************************************/

/***************************************************************************//**
  These constants are used to build the channel_cfg parameter of the
  configuration function.
 */
#define AXI4DMA_DISABLE                                0x0u
#define AXI4DMA_ENABLE                                 0x01u

/*-------------------------------------------------------------------------*//**
  Interrupt identification masks
  ===============================
  These constant definitions are used to create the mask values as parameters for
  interrupt control APIs for the DMA transfers.

  | Constant                        | Description                |
  |---------------------------------|----------------------------|
  | AXI4DMA_OP_COMPLETE_INTR_MASK  | DMA operation complete interrupt |
  | AXI4DMA_WR_ERR_INTR_MASK       | AXI write error detection interrupt|
  | AXI4DMA_RD_ERR_INTR_MASK       | AXI read error detection interrupt  |
  | AXI4DMA_INVALID_DESC_INTR_MASK | Invalid descriptor interrupt |

 */
#define AXI4DMA_OP_COMPLETE_INTR_MASK                  (0x00000001u << 0u)
#define AXI4DMA_WR_ERR_INTR_MASK                       (0x00000001u << 1u)
#define AXI4DMA_RD_ERR_INTR_MASK                       (0x00000001u << 2u)
#define AXI4DMA_INVALID_DESC_INTR_MASK                 (0x00000001u << 3u)

/*-------------------------------------------------------------------------*//**
  Flow control masks
  ===============================
  These constant definitions are used to generate the flow control mask values
  which can be used with AXI4DMA_flow_control() and AXI4DMA_stream_flow_control()
  functions.

  | Constant                  | Description                |
  |---------------------------|----------------------------|
  | AXI4DMA_SRC_ADDR_VALID   | sets the value of source data valid (bit13)|
  | AXI4DMA_DEST_ADDR_VALID  | sets the value of destination data ready (bit14)|
  | AXI4DMA_DESCRIPTOR_VALID | sets the value of descriptor valid (bit15)|

 */
#define AXI4DMA_SRC_ADDR_VALID                         (0x00000001u << 0u)
#define AXI4DMA_DEST_ADDR_VALID                        (0x00000001u << 1u)
#define AXI4DMA_DESCRIPTOR_VALID                       (0x00000001u << 2u)

typedef enum __axi4dma_desc_id
{
    INTRN_DESC_0 = 0,
    INTRN_DESC_1,
    INTRN_DESC_2,
    INTRN_DESC_3,
    INTRN_DESC_4,
    INTRN_DESC_5,
    INTRN_DESC_6,
    INTRN_DESC_7,
    INTRN_DESC_8,
    INTRN_DESC_9,
    INTRN_DESC_10,
    INTRN_DESC_11,
    INTRN_DESC_12,
    INTRN_DESC_13,
    INTRN_DESC_14,
    INTRN_DESC_15,
    INTRN_DESC_16,
    INTRN_DESC_17,
    INTRN_DESC_18,
    INTRN_DESC_19,
    INTRN_DESC_20,
    INTRN_DESC_21,
    INTRN_DESC_22,
    INTRN_DESC_23,
    INTRN_DESC_24,
    INTRN_DESC_25,
    INTRN_DESC_26,
    INTRN_DESC_27,
    INTRN_DESC_28,
    INTRN_DESC_29,
    INTRN_DESC_30,
    INTRN_DESC_31,
    EXT_DESC_32,
    STREAM_DESC_33
} axi4dma_desc_id_t;

typedef enum __axi4dma_irq_num
{
    IRQ_NUM_0 = 0,
    IRQ_NUM_1,
    IRQ_NUM_2,
    IRQ_NUM_3
} axi4dma_irq_num_t;

typedef enum __axi4dma_op_type
{
    OP_NOOP = 0, //used when pointing to external descriptor
    OP_INC_ADDR,
    OP_FIXED_ADDR,
    OP_UNUSED
} axi4dma_op_type_t;

typedef enum __axi4dma_tdest
{
    TDEST_0 = 0, //used when pointing to external descriptor
    TDEST_1,
    TDEST_2,
    TDEST_3
} axi4dma_tdest_signal_t;

typedef struct
{
    axi4dma_op_type_t src_op;
    axi4dma_op_type_t dest_op;
    uint8_t chain;
    uint8_t ext_desc_enable;
    uint8_t enable_intr;
    uint8_t src_data_valid;
    uint8_t dest_data_valid;
    uint8_t descriptor_valid;
} axi4dma_desc_config_t;

typedef struct
{
    axi4dma_desc_id_t desc_id;
    uint32_t desc_cfg;
    uint32_t xfr_count;
    uint32_t src_addr;
    uint32_t dest_addr;
    uint32_t next_desc;
} axi4dma_desc_t;

typedef struct
{
    uint32_t config;
    uint32_t xfr_count;
    uint32_t dest_addr;
} axi4dma_stream_desc_t;

/*******************************************************************************
 */
typedef struct
{
    uint32_t                    base_addr;
    uint32_t                    intr_mask[AXI4DMA_NUM_OF_INTERRUPTS];
} axi4dma_instance_t;

/*-------------------------------------------------------------------------*//**
  The AXI4DMA_init() function initializes the driver. It sets the base address
  for the CoreAXI4DMAController instance and initializes the axi4dma_instance_t
  data structure. This function must be called before any other driver function
  can be called.

  @param this_dmac
    The this_dmac parameter is a pointer to an axi4dma_instance_t type structure
    which holds all data regarding this instance of the CoreAXI4DMAController.

  @param base_addr
    The base_addr parameter is the base address of the CoreAXI4DMAController
    IP instance.

   @return
    This function returns zero value on successful execution. A negative value
    is returned in case of error.

  Example:

  Some text about the example here.

  @code
    Example
  @endcode
 */
int32_t
AXI4DMA_init
(
    axi4dma_instance_t* this_dmac,
    uint32_t base_addr
);

/*-------------------------------------------------------------------------*//**
  The AXI4DMA_configure_stream() function is used to configure a
  stream descriptor in the memory pointed to by the stream_desc_ptr parameter and
  write the address of the stream descriptor into the CoreAXI4DMAController IP
  registers so that it can fetch the descriptor and start the stream transfer
  based on the TDEST signal on the streaming interface.
  Maximum size of a supported speed is 8MB.
  Maximum number of supported stream descriptors are 4. Once the stream transfer
  is setup, the actual transfer will start when first AXI stream master generates
  first AXI transaction.

  @param this_dmac
    The this_dmac parameter is a pointer to a axi4dma_instance_t structure which
    holds all data regarding this instance of the CoreAXI4DMAController.

  @param stream_desc_ptr
    The stream_desc_ptr parameter is the pointer to the memory which
    will be used as the stream descriptor. This function will configure the
    appropriate words and bits in the descriptor as per the other configuration
    parameters provided as input to this function. Ensure that the stream
    descriptor location is accessible to the IP instance and you reserve memory
    enough for the size of the stream descriptor.

  | descriptor pointer  | descriptor fields        | Bits |
  |---------------------|--------------------------|------|
  | stream_desc_ptr ->  | Configuration (offset 0) |[32:0]-unused; 3-Descriptor valid; 2-Destination Data ready; [1:0]-destination operation|
  |                     | Byte Count  (offset 4)   | 32 bit count  |
  |                     | Destination (offset 8)   | 32 bit address |

    The address of the stream descriptor must be written to the appropriate
    Stream Descriptor Address register prior to the AXI4-Stream transaction being
    initiated, along with the prior existence of the valid stream descriptor in
    the AXI4 DMA address space.

  @param tdest_signal_num
    The tdest_signal_num parameter indicates the AXI4 TDEST signal on the stream
    interface to be associated with this stream descriptor. supported TDEST IDs
    are 0-3.

  @param config
    The config parameter is the value that will be stored into the location
    pointed by stream_desc_ptr -> Configuration.
    See the table above for the bit field description.

  @param xfr_count
    The xfr_count parameter indicates the number of bytes constructing the
    stream

  @param dest_addr
    The dest_addr parameter is the destination memory address where the stream
    data will be stored

   @return
    This function returns zero value on successful execution. A negative value
    is returned in case of error.

  Example:

  Some text about the example here.

  @code
    Example
  @endcode
 */
int8_t
AXI4DMA_configure_stream
(
    axi4dma_instance_t* this_dmac,
    axi4dma_stream_desc_t* stream_desc_ptr,
    axi4dma_tdest_signal_t tdest_signal_num,
    uint8_t config,
    uint32_t xfr_count,
    uint32_t dest_addr
);

/*-------------------------------------------------------------------------*//**
  The AXI4DMA_transfer_status() function returns the error or success status of
  the DMA transfers. When interrupts are enabled, the user provided interrupt
  handler function registered with AXI4DMA_set_irq_handler() is called and it
  provides the same information as provided by the AXI4DMA_transfer_status()
  function. When interrupts are disabled, this function can be used to poll on
  the transfer status. This function can be used to know the status of stream
  descriptor transfers as well.

  @param this_dmac
    The this_dmac parameter is a pointer to a axi4dma_instance_t structure which
    holds all data regarding this instance of the CoreAXI4DMAController.

  @param irq_num
    The irq_num is an input parameter indicating the interrupt number whose
    status needs to be returned.

  @param desc_id
    The desc_id is an output parameter. It should point to a location where the
    driver will store the internal descriptor ID associated with the irq_num for
    which the current status is being returned.

     |desc_id| Description                                                     |
     |-------|-----------------------------------------------------------------|
     | 0-31  | Internal descriptor number that the status information is       |
     |       | associated with.                                                |
     |  32   | Refer to the External Descriptor Address register to determine  |
     |       | the address of the external buffer descriptor to which the status|
     |       | information is associated with.                                 |
     |  33   | Stream operation complete. Refer to the External Descriptor     |
     |       | Address register to determine the address of the stream descriptor |
     |       | to which the status information is associated.                  |

  @param ext_desc_addr
    The ext_desc_addr is an output parameter. It should point to a location where
    the driver will store the external descriptor address location for which the
    current status is being returned. This parameter is applicable only for the
    when desc_id parameter value is either 32 or 33. If the status is for
    internal descriptor, a zero will be written in this location.

   @return
    This function returns the interrupt status register value corresponding to
    the interrupt indicated by irq_num parameter.

  Example:

  Some text about the example here.

  @code
    Example
  @endcode
 */
uint32_t
AXI4DMA_transfer_status
(
    axi4dma_instance_t* this_dmac,
    axi4dma_irq_num_t irq_num,
    axi4dma_desc_id_t* desc_id,
    uint32_t* ext_desc_addr
);

/***************************************************************************//**
 * The AXI4DMA_enable_irq() function enables the interrupts selected by the
 * intr_mask for the interrupt number indicated by the irq_num parameter.
 */
static inline void
AXI4DMA_enable_irq
(
    axi4dma_instance_t* this_dmac,
    axi4dma_irq_num_t irq_num,
    uint32_t intr_mask
)
{
    this_dmac->intr_mask[irq_num] = intr_mask;
    HAL_set_32bit_reg(((this_dmac->base_addr) + (irq_num * 0x10u)), I0MASK, intr_mask);
}

/***************************************************************************//**
 * The AXI4DMA_disable_irq() function disables the interrupts selected by the
 * intr_mask for the interrupt number indicated by the irq_num parameter.
 */
static inline void
AXI4DMA_disable_irq
(
    axi4dma_instance_t* this_dmac,
    axi4dma_irq_num_t irq_num,
    uint32_t intr_mask
)
{
    uint32_t tempreg = 0u;

    tempreg = HAL_get_32bit_reg(((this_dmac->base_addr) + (irq_num * 0x4u)), I0MASK);
    tempreg &= ~intr_mask;
    HAL_set_32bit_reg(((this_dmac->base_addr) + (irq_num * 0x10u)), I0MASK, tempreg);
}

/***************************************************************************//**
 * The AXI4DMA_clear_irq() function clears the interrupts selected by the
 * intr_mask for the interrupt number indicated by the irq_num parameter.
 */
void
static inline AXI4DMA_clear_irq
(
    axi4dma_instance_t* this_dmac,
    axi4dma_irq_num_t irq_num,
    uint32_t intr_mask
)
{
    /* writing 1 to corresponding bit in the clear register clears interrupt*/
    HAL_set_32bit_reg(((this_dmac->base_addr) + (irq_num * 0x10u)), I0CLR, intr_mask);
}

/*-------------------------------------------------------------------------*//**
  The AXI4DMA_stream_flow_control() function is used to control the stream.
  There are three bits in the descriptor configuration register which
  are used for flow control - source data valid (bit13), destination data ready
  (bit 14), descriptor valid(bit 15). This functions sets the values of these 3
  bits for a stream descriptor.

  @param stream_desc_ptr
    The stream_desc_ptr parameter is a pointer to a stream descriptor. This can
    exist in any memory space accessible to the CoreAXI4DMAController IP.

  @param flow_control_mask
    The flow_control_mask parameter is the mask value that will be set in the
    flow control bits.

     |flow_control_mask bits| Description                                |
     |----------------------|--------------------------------------------|
     |       1:0            | sets the value of Destination Operation type (bit [1:0])|
     |       1              | sets the value of destination data ready (bit2)|
     |       2              | sets the value of descriptor valid (bit3)|

   @return
    This function does not return any value.

  Example:

  Some text about the example here.

  @code

  @endcode
 */
static inline void
AXI4DMA_stream_flow_control
(
    axi4dma_stream_desc_t* stream_desc_ptr,
    uint8_t flow_control_mask
)
{
    stream_desc_ptr->config = flow_control_mask;
}

/*-------------------------------------------------------------------------*//**
  The AXI4DMA_flow_control() function is used to control the descriptor data
  flow. There are three bits in the descriptor configuration register which
  are used for flow control - source data valid (bit13), destination data ready
  (bit 14), descriptor valid(bit 15). This functions sets the values of these 3
  bits.

  @param this_dmac
    The this_dmac parameter is a pointer to a axi4dma_instance_t structure which
    holds all data regarding this instance of the CoreAXI4DMAController.

  @param desc_id
    The desc_id is an output parameter. It should point to a location where the
    driver will store the internal descriptor ID associated with the irq_num for
    which the current status is being returned.

     |desc_id| Description                                                     |
     |-------|-----------------------------------------------------------------|
     | 0-31  | Internal descriptor number that the status information is       |
     |       | associated with.                                                |
     |  32   | Refer to the External Descriptor Address register to determine  |
     |       | the address of the external buffer descriptor to which the status|
     |       | information is associated with.                                 |
     |  33   | Stream operation complete. Refer to the External Descriptor     |
     |       | Address register to determine the address of the stream descriptor |
     |       | to which the status information is associated.                  |

  @param flow_control_mask
    The flow_control_mask parameter is the mask value that will be set in the
    flow control bits.

     |flow_control_mask bits| Description                                |
     |----------------------|--------------------------------------------|
     |       0              | sets the value of source data valid (bit13)|
     |       1              | sets the value of destination data ready (bit14)|
     |       2              | sets the value of descriptor valid (bit15)|

   @return
    This function does not return any value.

  Example:

  Some text about the example here.

  @code
    // CoreAXI4DMAController IP base address in Libero design = 0x60020000u
    AXI4DMA_init (&g_dmac, 0x60020000u);
    AXI4DMA_flow_control(&g_dmac, INTRN_DESC_31, (AXI4DMA_SRC_ADDR_VALID | AXI4DMA_DEST_ADDR_VALID | AXI4DMA_DESCRIPTOR_VALID)

  @endcode
 */

static inline void
AXI4DMA_flow_control
(
    axi4dma_instance_t* this_dmac,
    axi4dma_desc_id_t desc_id,
    uint32_t flow_control_mask
)
{
    HAL_set_32bit_reg_field((this_dmac->base_addr +
                            (desc_id * 0x20)),
                            ID0CFG_FLCTRL, flow_control_mask);
}

int8_t
AXI4DMA_configure
(
    axi4dma_instance_t* this_dmac,
    axi4dma_desc_id_t desc_id,
    uint8_t src_op,
    uint8_t dest_op,
    uint32_t byte_count,
    uint32_t src_addr,
    uint32_t dest_addr
);

int8_t
AXI4DMA_configure_chain
(
    axi4dma_instance_t* this_dmac,
    axi4dma_desc_t* desc_data,
    uint8_t num_desc
);

/***************************************************************************//**
 * This function starts the DMA transfer. This function does not need to be used
 * with the stream transfers.
 */
void
AXI4DMA_start_transfer
(
    axi4dma_instance_t* this_dmac,
    axi4dma_desc_id_t desc_id
);


#ifdef __cplusplus
}
#endif

#endif /* CORE_AXI4DMACTRL_H_ */
