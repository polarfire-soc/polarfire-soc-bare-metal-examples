/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_qspi.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreQSPI prototypes
 *
 */

/**
  @mainpage CoreQSPI Bare Metal Driver

  @section intro Introduction
  This CoreQSPI provides AHB system Interface and SPI interface to connect with
  the SPI memory devices. The control register is used to configure the IP in
  different modes and the FIFO is used to buffer the data across clock domains.

  This driver provides a set of functions for configuring and controlling
  CoreQSPI as part of a bare metal system, where no operating system is
  available. This driver can be adapted as part of an operating system, but the
  implementation of the adaptation layer between the driver and the operating
  system's driver model is outside the scope of this user guide.

  This driver is not compatible with the CoreQSPI versions below v2.1.

  @subsection features Features
    - Master operation
    - Motorola SPI supported
    - Slave select operation in idle cycles configurable
    - Supports Extended SPI operation (1, 2, and 4-bit)
    - Supports QSPI operation (4-bit operation)
    - Supports BSPI operation (2-bit operation)
    - Support XIP (execute in place)
    - Supports three or four-byte SPI address

  @section theory_op Theory of Operation
  The CoreQSPI driver functions are grouped into the following categories.
    - Initialization and configuration functions
    - Polled transmit and receive functions
    - Interrupt-driven transmit and receive functions

@section driver_initialization_configuration Driver Initialization and Configuration

  The CoreQSPI supports the following operations:
    - Normal SPI operations (1-bit)
    - Dual SPI operations   (2-bit)
    - Quad SPI operations   (4-bit)

  The CoreQSPI driver provides the QSPI_init() function to initialize the
  CoreQSPI hardware block. This initialization function must be called before
  calling any other CoreQSPI driver functions.

  The CoreQSPI driver provides the QSPI_config() function to configure
  QSPI with the desired configuration values. It also provides the
  QSPI_get_config() function to read back the current configuration of QSPI
  use QSPI_get_config() function to retrieve the current configurations and
  then overwrite them with application-specific values, such as SPI mode,
  SPI clock rate, SDI sampling, QSPI operation, XIP mode, and XIP address bits.
  All these configuration options are explained in detail in the API function
  description of the respective function.

@section block_transfers  SPI Master Block Transfer Control

  Once the driver is initialized and configured, data transmission and reception
  are achieved by configuring it to the desired value. CoreQSPI is designed to
  specifically work with SPI flash memories. It supports a single, active-low
  slave-select output. Block transfers are accomplished in the following ways:
    - Polled block transfer
    - Interrupt-driven block transfer

@subsection polled_block_transfers  Polled Block Transfer
  The QSPI_polled_transfer_block() function accomplishes data
  transfers where no interrupt is used. The QSPI_polled_transfer_block()
  function polls the status register to know the current status of the on-going
  transfer. This is a blocking function. A CoreQSPI block transfer always has
  some amount of data to be transmitted (at least one command byte), but
  receiving useful data from the target memory device is optional. So, if the
  scheduled block transfer is only transferring data and not receiving any data,
  then QSPI_polled_transfer_block() function exits after transmitting the
  required bytes. If data needs to be received from the target memory device
  during a particular transfer, the QSPI_polled_transfer_block() function
  terminates once the expected data is received from the target memory device.

@subsection interrupt_block_transfers  Interrupt Driven Block Transfer

  This block transfer is accomplished using interrupts instead of polling the
  status register. The following functions are provided to support
  interrupt-driven block transfers:
            - QSPI_irq_transfer_block()
            - QSPI_set_status_handler()

  The QSPI_set_status_handler() function must be used to set a status
  handler call-back function with the driver. The QSPI_set_status_handler()
  function is called back by the driver at two different stages of the transfer.
  At the first stage, it is called when the required number of bytes are
  transmitted. At the second stage, if there is data to be received from the
  target memory device, it is invoked again once the desired data is received.
  An appropriate status value is passed by the driver as a parameter of this
  call-back function so that the application infers that an event has occurred.

@section qspi_status  QSPI Status
  The QSPI_read_status() function reads the current status of CoreQSPI. The
  QSPI_read_status() function is typically used to know the status of an
  ongoing transfer. The QSPI_read_status() function returns the status
  register value and is called at any time after CoreQSPI is initialized and
  configured.

@section direct_access Direct Access
  CoreQSPI allows direct access to the QSPI interface pins to support access to
  non-standard SPI devices through direct CPU control.

  This driver provides the following functions to read and write the direct
  access register of CoreQSPI:
                  - QSPI_read_direct_access_reg()
                  - QSPI_write_direct_access_reg()

  Using these functions, you generate any sequence of binary transitions on the
  QSPI pins that might be needed to communicate with the non-standard target
  devices.

*/

#ifndef CORE_QSPI_H_
#define CORE_QSPI_H_

#include "hal/cpu_types.h"
#include "hal/hal.h"
#include "core_qspi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 # SDI Pin Sampling

 The following constants can be used as input parameter value to configure the
 event on which the SDI pin is sampled.

 @code
 qspi_config.sample = QSPI_SAMPLE_POSAGE_SPICLK;
 QSPI_configure(&qspi_config);
 @endcode

| Constant         | Description           |
|------------------|-----------------------|
|QSPI_SAMPLE_POSAGE_SPICLK |The SDI pin is sampled at the rising edge off SPI CLOCK|
|QSPI_SAMPLE_ACTIVE_SPICLK |The SDI pin is sampled on the last falling HCLK edge in the SPI clock period|
|QSPI_SAMPLE_NEGAGE_SPICLK |The SDI pin is sampled at the rising HCLK edge as SPICLK falls|

*/
#define QSPI_SAMPLE_POSAGE_SPICLK       0x00u
#define QSPI_SAMPLE_ACTIVE_SPICLK       0x01u
#define QSPI_SAMPLE_NEGAGE_SPICLK       0x02u


/**
 # Generic constant definitions
 The following constants can be used to configure the CoreQSPI where a zero or
 non-zero value such as enable or disable is to be provided as input parameter.
 
 @code
       qspi_config.xip = QSPI_DISABLE;
       QSPI_configure(&qspi_config);
 @endcode

| Constant         | Description           |
|------------------|-----------------------|
|QSPI_ENABLE  |Enable  |
|QSPI_DISABLE |Disable |

 */
#define QSPI_ENABLE                     0x01u
#define QSPI_DISABLE                    0x00u

/**
The following table shows that these values are used to program the io_format 
parameter of the configuration structure of this driver.

@code
  qspi_config.io_format = QSPI_QUAD_FULL;
  QSPI_configure(&qspi_config);
@endcode

|   Value            |                  Description                            |
|--------------------|---------------------------------------------------------|
|QSPI_NORMAL         | 1-bit Normal SPI operations                             |
|                    | (single DQ0 TX and single  DQ1 RX lines)                |
|                    |                                                         |
|QSPI_DUAL_EX_RO | 2-bit SPI operations                                    |
|                    | (command and address bytes on DQ0 only. data on DQ[1:0])|
|                    |                                                         |
|QSPI_QUAD_EX_RO | 4-bit SPI operations                                    |
|                    | (command and address bytes on DQ0 only. data on DQ[3:0])|
|                    |                                                         |
|QSPI_DUAL_EX_RW | 2-bit SPI operations                                    |
|                    | (command byte on DQ0 only. Address and data on DQ[1:0]) |
|                    |                                                         |
|QSPI_QUAD_EX_RW | 4-bit SPI operations                                    |
|                    | (command byte on DQ0 only. Address and data on DQ[3:0]) |
|                    |                                                         |
|QSPI_DUAL_FULL  | 2-bit SPI operations                                    |
|                    | (command, address, and data on DQ[1:0])                 |
|                    |                                                         |
|QSPI_QUAD_FULL  | 4-bit SPI operations                                    |
|                    | (command, address, and data on DQ[3:0])                 |
|                    |                                                         |

*/
typedef enum qspi_io_format_t
{
    QSPI_NORMAL      = 0u,
    QSPI_DUAL_EX_RO  = 2u,
    QSPI_QUAD_EX_RO  = 3u,
    QSPI_DUAL_EX_RW  = 4u,
    QSPI_QUAD_EX_RW  = 5u,
    QSPI_DUAL_FULL   = 6u,
    QSPI_QUAD_FULL   = 7u
} qspi_io_format;

/**
These values are used to program the spi_mode parameter of the configuration
structure of this driver. For example:
  @code
    qspi_config.spi_mode = QSPI_MODE0;
    QSPI_configure(&qspi_config);
  @endcode


|   Value           |                   Description                            |
|-------------------|----------------------------------------------------------|
|   QSPI_MODE0      | Set clock IDLE to low   (0)                              |
|   QSPI_MODE3      | Set clock IDLE to high  (1)                              |

*/
typedef enum qspi_protocol_mode_t
{
    QSPI_MODE0    = 0x0u,
    QSPI_MODE3    = 0x1u

} qspi_protocol_mode;

/***************************************************************************//**
These values are used to program the spi_mode parameter of the configuration
structure of this driver. For example:
  @code
    qspi_config.clk_div =  QSPI_CLK_DIV_2;
    QSPI_configure(&qspi_config);
  @endcode

|        Value          |        Description        |
|-----------------------|---------------------------|
|   QSPI_CLK_DIV_2  |  Set SPI clock to HCLK/2  |
|   QSPI_CLK_DIV_4  |  Set SPI clock to HCLK/4  |
|   QSPI_CLK_DIV_6  |  Set SPI clock to HCLK/6  |
|   QSPI_CLK_DIV_8  |  Set SPI clock to HCLK/8  |
|   QSPI_CLK_DIV_10 |  Set SPI clock to HCLK/10 |
|   QSPI_CLK_DIV_12 |  Set SPI clock to HCLK/12 |
|   QSPI_CLK_DIV_14 |  Set SPI clock to HCLK/14 |
|   QSPI_CLK_DIV_16 |  Set SPI clock to HCLK/16 |
|   QSPI_CLK_DIV_18 |  Set SPI clock to HCLK/18 |
|   QSPI_CLK_DIV_20 |  Set SPI clock to HCLK/20 |
|   QSPI_CLK_DIV_22 |  Set SPI clock to HCLK/22 |
|   QSPI_CLK_DIV_24 |  Set SPI clock to HCLK/24 |
|   QSPI_CLK_DIV_26 |  Set SPI clock to HCLK/26 |
|   QSPI_CLK_DIV_28 |  Set SPI clock to HCLK/28 |
|   QSPI_CLK_DIV_30 |  Set SPI clock to HCLK/30 |

*/
typedef enum qspi_clk_div_t
{
    QSPI_CLK_DIV_2     = 0x1u,
    QSPI_CLK_DIV_4     = 0x2u,
    QSPI_CLK_DIV_6     = 0x3u,
    QSPI_CLK_DIV_8     = 0x4u,
    QSPI_CLK_DIV_10    = 0x5u,
    QSPI_CLK_DIV_12    = 0x6u,
    QSPI_CLK_DIV_14    = 0x7u,
    QSPI_CLK_DIV_16    = 0x8u,
    QSPI_CLK_DIV_18    = 0x9u,
    QSPI_CLK_DIV_20    = 0xAu,
    QSPI_CLK_DIV_22    = 0xBu,
    QSPI_CLK_DIV_24    = 0xCu,
    QSPI_CLK_DIV_26    = 0xDu,
    QSPI_CLK_DIV_28    = 0xEu,
    QSPI_CLK_DIV_30    = 0xFu
} qspi_clk_div;


/**
 This prototype defines the function prototype that must be followed by CoreQSPI
 status handler functions. This function is registered with the CoreQSPI driver
 by calling the QSPI_set_status_handler() function.

 Declaring and Implementing Status Handler Function:
 Slave frame receive handler functions should follow the following prototype:
        void transfer_status_handler(uint32_t status);

 The actual name of the status handler not important. You can use any name
 of your choice. The status parameter contains a value indicating which
 of the TX-DONE, or RX-DONE event has caused the interrupt.
*/
typedef void (*qspi_status_handler_t)(uint32_t status);

/***************************************************************************//**
  This is the structure definition for the CoreQSPI configuration instance. It
  defines the configuration data that the application exchanges with the driver.

  The following parameters are the configuration options for CoreQSPI.

|  Parameter  |                         Description                            |
|-------------|----------------------------------------------------------------|
|   xip       |  Enable or disable XIP mode                                    |
|   xip_addr  |  Number of address bytes used in XIP mode                      |
|   spi_mode  |  Select either Motorola mode0 or mode3                         |
|   clk_div   |  HCLK Clock divider for generating SPI clock                   |
|   io_format |  QSPI transfer format, extended, dual, quad and so on.         |
|   sample    |  Select the event on which the QSPI samples the incoming data  |

*/
typedef struct qspi_config{
    uint8_t                 xip;
    uint8_t                 xip_addr;
    qspi_protocol_mode      spi_mode;   /*clkidl mode0 or mode3*/
    qspi_clk_div            clk_div;
    qspi_io_format          io_format;
    uint8_t                 sample;
}qspi_config_t;

/**
 * There should be one instance of this structure for each instance of CoreQSPI
 * in your system. The QSPI_init() function initializes this structure. It is
 * used to identify the various CoreQSPI hardware instances in your system.
 * An initialized timer instance structure should be passed as first parameter to
 * CoreQSPI driver functions to identify which CoreQSPI instance should perform
 * the requested operation.
 * Software using this driver should only need to create one single instance of
 * this data structure for each hardware QSPI instance in the system.
 */
typedef struct __qspi_instance_t
{
    addr_t base_address;
} qspi_instance_t;


/**
  The QSPI_init() function initializes and enables the CoreQSPI.
  It enables the QSPI hardware block, and configures it with the default
  values.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @return
    This function does not return a value.

  @example
  This example shows how to initialize the CoreQSPI driver.

  @code
      QSPI_init(QSPI_INSTANCE, base_addr);
  @endcode

 */
void QSPI_init
(
    qspi_instance_t *this_qspi,
    addr_t addr
);

/***************************************************************************//**
  The QSPI_disable() function disables the CoreQSPI hardware block.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @return
    This function does not return a value.

  @example
  @code
      QSPI_disable(QSPI_INSTANCE);
  @endcode
 */
static inline void QSPI_disable
(
    qspi_instance_t *this_qspi
)
{
    HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
            ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, CONTROL))
              & ~(uint32_t)(CTRL_ENABLE_MASK)));

}

/***************************************************************************//**
  The QSPI_configure() function configures the CoreQSPI to the desired
  configuration values.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @param config
    The config parameter is a pointer to the qspi_config_t structure, which
    provides new configuration values. See the qspi_config_t section for
    details.

  @return
    This function does not return a value.

  @example
  @code

    qspi_config.clk_div =  QSPI_CLK_DIV_8;
    qspi_config.sample = QSPI_SAMPLE_POSAGE_SPICLK;
    qspi_config.spi_mode = QSPI_MODE3;
    qspi_config.xip = QSPI_DISABLE;
    qspi_config.io_format = t_io_format;

    QSPI_configure(QSPI_INSTANCE, &qspi_config);

  @endcode
 */
void QSPI_configure
(
    qspi_instance_t *this_qspi,
    const qspi_config_t* config
);

/***************************************************************************//**
  The QSPI_get_config() function reads-back the current configurations of
  the CoreQSPI. This function can be used when you want to read the current
  configurations, modify the configuration values of your choice and reconfigure
  the CoreQSPI hardware, using QSPI_configure() function.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @param config
    The config parameter is a pointer to an qspi_config_t structure, where
    the current configuration values of the CoreQSPI are returned.

    Please see description of qspi_config_t for more details.

  @return
    This function does not return a value.

  @example
  @code
       qspi_config_t beforexip_qspi_config = { 0 };
       QSPI_get_config(QSPI_INSTANCE, &beforexip_qspi_config);
  @endcode
 */
void QSPI_get_config
(
    qspi_instance_t *this_qspi,
    qspi_config_t* config
);

/***************************************************************************//**
  The QSPI_polled_transfer_block() function carries out a QSPI transfer with the
  target memory device using polling method of data transfer. The QSPI transfer
  characteristics are configured every time a new transfer is initiated.
  This is a blocking function.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @param num_addr_bytes
    The num_addr_bytes parameter indicates the number of address bytes to be
    used while transacting with the target memory device. Depending on the
    target memory device, the address within the target memory device can be
    either 3 or 4 bytes long. You must make sure that you provide the exact same
    number with which the target memory device is configured.

    Note: Few command opcodes do not require specified addresses. For example
    READ_ID. For such commands the num_addr_bytes parameter must be set to 0x0.

  @param target_mem_addr
    The target_mem_addr parameter is the memory address in the target memory
    device on which the read/write operation is to be carried out.

  @param tx_buffer
    The tx_buffer parameter is the pointer to the buffer from which the data
    needs to transmitted to the target memory device.

  @param tx_byte_size
    The tx_byte_size parameter is the exact number of bytes that needs to be
    transmitted to the target memory device.

    Note: This parameter must not consider the command opcode and address bytes
    as part of the data that needs to be transmitted.

  @param rd_buffer
    The rd_buffer parameter is the pointer to the buffer where the data returned
    by the target memory device is stored.

  @param rd_byte_size
    The rd_byte_size parameter is the exact number of bytes that needs to be
    received from the target memory device.

  @param num_idle_cycles
    The num_idle_cycles parameter indicates the number of Idle cycles/dummy clock
    edges that must be generated after the address bytes are transmitted and
    before target memory device starts sending data. This must be correctly set
    based on the target memory device and the SPI command being used. This may
    also vary based on SPI clock and the way the target memory device is
    configured.

  @return
    This function does not return a value.

  @example
  @code
      QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
  @endcode
 */
void QSPI_polled_transfer_block
(
    qspi_instance_t *this_qspi,
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
);

/***************************************************************************//**
  The QSPI_irq_transfer_block() function is used to carry out a QSPI transfer
  with the target memory device using interrupt method of data transfers.
  The QSPI transfer characteristics are configured every time a new transfer is
  initiated. This is non-blocking function. You must configure the interrupt
  handler function, before calling this function. It will enable the interrupts
  and start transmitting as many bytes as requested. You will get an indication
  when the actual SPI transmit operation is complete. When The transmit-done
  interrupt event occurs and this driver calls-back the interrupt handler
  function that you previously provided. If the transfer includes receiving
  data from the target memory device, then the receive-available and receive-done
  interrupts are also enabled by this function. The data will be received in the
  interrupt routine. The interrupt handler provided by you will be called-back
  again when the receive-done interrupt event occurs.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @param num_addr_bytes
    The num_addr_bytes parameter indicates the number of address bytes to be
    used while transacting with the target memory device. Depending on the the
    target memory device, the address within the target memory device can be
    either 3 or 4 bytes long.  You must make sure that you provide the exact
    same number with which the target memory device is configured.

    Note: There will be some command opcodes for which no address needs to be
    specified. e.g. READ_ID. For such commands the num_addr_bytes parameter
    must be set to 0x0.

  @param target_mem_addr
    The target_mem_addr parameter is the memory address in the target memory
    device on which the read/write operation is to be carried out.

  @param tx_buffer
    The tx_buffer parameter is the pointer to the buffer from which the data
    needs to transmitted to the target QSPI memory.

  @param tx_byte_size
    The tx_byte_size parameter is the exact number of bytes that needs to be
    transmitted to target memory device.

    Note: This parameter must not consider the command opcode and address bytes
    as part of the data that needs to be transmitted.

  @param rd_buffer
    The rd_buffer parameter is a pointer to the buffer where the data returned
    by the target memory device is to be stored.

  @param rd_byte_size
    The rd_byte_size parameter is the exact number of bytes that needs to be
    received from the target memory device.

  @param num_idle_cycles
    The num_idle_cycles parameter indicates the number of IDLE/dummy clock
    edges that must be generated after the address bytes are transmitted and
    before target memory device starts sending data. This must be correctly set
    based on the target memory device and the SPI command being used. This may
    also vary based on SPI clock and the way the target memory device is
    configured.

  @return
    This function returns a non-zero value if the CoreQSPI is busy completing the
    previous transfer and can not accept a new transfer. A zero return value
    indicates successful execution of this function.

  @example
  @code
      QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
  @endcode
 */
uint8_t QSPI_irq_transfer_block
(
    qspi_instance_t *this_qspi,
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
);

/***************************************************************************//**
  The QSPI_set_status_handler() function registers an interrupt handler
  function with this driver which is used to indicate the interrupt status back
  to the application. This status handler function is called by this driver in
  two events. First, when the transmission of required bytes is completed
  (Transmit-Done). Second, if data is to be received from the target memory
  device, then this function is called again when required data is received
  (Receive-Done).

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @param handler
    The handler parameter is the interrupt handler function of the type
    qspi_status_handler_t, which needs to be registered.

  @return
    This function does not return a value.

  @example
  @code
      QSPI_set_status_handler(QSPI_INSTANCE, transfer_status_handler);
  @endcode

 */
void QSPI_set_status_handler
(
   qspi_status_handler_t handler
);

/***************************************************************************//**
  The QSPI_read_direct_access_reg() reads the current value of the direct
  access register(DAR) of the CoreQSPI. DAR allows direct access to the QSPI
  interface pins to support access to non-standard SPI devices through direct
  CPU control.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @param
    This function takes no parameters.

  @return
    This function returns the current value of the DAR of the CoreQSPI.

 */
static inline uint32_t QSPI_read_direct_access_reg
(
    qspi_instance_t *this_qspi
)
{
    return (HAL_get_32bit_reg(this_qspi->base_address, DIRECT_ACCESS));
}

/***************************************************************************//**
  The QSPI_write_direct_access_reg() writes value of the DAR of the CoreQSPI.
  DAR allows direct access to the QSPI interface pins, to support access
  to the non-standard SPI devices through direct CPU control.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @param value
    The value parameter is the value that needs to be set into the direct access
    register of the CoreQSPI.

  @return
    This function does not return a value.

 */
static inline void QSPI_write_direct_access_reg
(
    qspi_instance_t *this_qspi,
    uint32_t value
)
{
    HAL_set_32bit_reg(this_qspi->base_address, DIRECT_ACCESS, value);
}

/***************************************************************************//**
  The QSPI_read_status() function is used to read the status of the CoreQSPI.
  This function returns the status register value and can be called any time
  after the CoreQSPI is initialized and configured.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @return
    This function returns the the current value of the status register of the
    CoreQSPI.

 */
static inline uint32_t QSPI_read_status
(
    qspi_instance_t *this_qspi
)
{
    return (HAL_get_32bit_reg(this_qspi->base_address, STATUS));

}

/**************************************************************************//**
 * Interrupt service routine to handle the QSPI interrupt events.
 * The CoreQSPI interrupt will appear as an external interrupt on the processor.
 * This function must be called by the application from the external interrupt
 * handler which corresponds to the CoreQSPI interrupt.

  @param this_qspi
    It is a pointer to the qspi_instance_t data structure that holds all the data
    related to the CoreQSPI instance being initialized. A pointer to this data
    structure is used in all subsequent calls to the CoreQSPI driver functions
    that operate on this CoreQSPI instance.

  @return
   This function does not return a value.
 */
void qspi_isr
(
    qspi_instance_t *this_qspi
);
#ifdef __cplusplus
}
#endif

#endif /* CORE_QSPI_H_*/

