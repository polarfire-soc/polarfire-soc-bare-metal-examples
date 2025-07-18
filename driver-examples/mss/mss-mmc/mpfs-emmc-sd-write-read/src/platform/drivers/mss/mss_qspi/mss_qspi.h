/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_qspi.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) QSPI bare metal software
 * driver public API.
 *
 */

/*=========================================================================*//**
  @mainpage PolarFire® SoC MSS QSPI Bare Metal Driver

  ==============================================================================
  Introduction
  ==============================================================================
  The PolarFire SoC Microprocessor Sub-System (MSS) includes a Quad Serial 
  Peripheral Interface (QSPI) controller for fast SPI transfers. The MSS QSPI is 
  designed as a Serial Peripheral Interface (SPI) master to specifically work
  with the QSPI and SPI memory devices. The PolarFire SoC MSS QSPI driver 
  provides a set of functions for controlling MSS QSPI as part of a bare metal 
  system where no operating system is available. This driver is adapted for use 
  as part of an operating system, but the implementation of the adaptation layer 
  between this driver and the operating system's driver model is outside the 
  scope of this driver.

  --------------------------------
  Features
  --------------------------------
  The PolarFire SoC MSS QSPI driver provides the following features:
       - QSPI Master operations in 1-bit, 2-bit, and 4-bit formats and extended
         operations
       - XIP operations
       - Configurable SPI mode0 and mode3
       - Programmable SPI clock

  ==============================================================================
  Hardware Flow Dependencies
  ==============================================================================
  The configuration of all the features of the MSS QSPI peripheral is covered by 
  this driver, with the exception of the PolarFire SoC IOMUX configuration. The 
  PolarFire SoC allows multiple non-concurrent uses of a few external pins 
  through IOMUX configuration. This feature allows optimization of external pin 
  usage by assigning external pins for use by either the microprocessor 
  sub-system or the FPGA fabric. The MSS QSPI serial signals are routed through 
  IOMUXs to the PolarFire SoC device's external pins. The MSS QSPI serial 
  signals are also routed through IOMUXs to the PolarFire SoC FPGA fabric. 
  For more information on IOMUX, refer to the PolarFire® SoC MSS Technical 
  Reference Manual and PolarFire Device Register Map.

  The IOMUXs are configured using the PolarFire SoC MSS configurator tool. You
  must ensure that the MSS QSPI peripherals are enabled and configured in the
  PolarFire SoC MSS configurator if you wish to use them. For more information
  on IOMUXs, refer to the PolarFire® SoC MSS Technical Reference Manual and 
  PolarFire Device Register Map.

  The base address and the register addresses are defined in this driver as
  constants. The interrupt number assignment for the MSS QSPI peripherals is
  defined as constants in the Microchip PolarFire SoC (MPFS) Hardware 
  Abstraction Layer (HAL). You must ensure that the latest MPFS HAL is included 
  in the project settings of the SoftConsole toolchain and that it is generated 
  into your project.

  ==============================================================================
  Theory of Operation
  ==============================================================================
  The MSS QSPI driver functions are grouped into the following categories.
    - Initialization and configuration functions
    - Polled transmit and receive functions
    - Interrupt-driven transmit and receive functions

  --------------------------------
  Initialization and Configuration
  --------------------------------
  The MSS QSPI supports the following operations:
    - Normal SPI operations (1-bit)
    - Dual SPI operations   (2-bit)
    - Quad SPI operations   (4-bit)

  The MSS QSPI driver provides the MSS_QSPI_init() function to initialize the
  MSS QSPI hardware block. This initialization function must be called before 
  any other MSS QSPI driver functions are called.

  The MSS QSPI driver provides the MSS_QSPI_config() function to configure MSS 
  QSPI with the desired configuration values. It also provides the 
  MSS_QSPI_get_config() function to read back the current configuration of MSS 
  QSPI. You use MSS_QSPI_get_config() function to retrieve the current 
  configurations and then overwrite them with application-specific values, such 
  as SPI mode, SPI clock rate, SDI sampling, QSPI operation, XIP mode, and XIP 
  address bits. All these configuration options are explained in detail in the 
  API function description of the respective function.

  --------------------------------------
  SPI Master Block Transfer Control
  --------------------------------------
  Once the driver is initialized and configured, data transmission and reception 
  are achieved by configuring it to the desired value. MSS QSPI is designed to 
  specifically work with SPI flash memories. It supports a single, active-low 
  slave-select output. Block transfers are accomplished in the following ways:  
    - Polled block transfer
    - Interrupt-driven block transfer

  ---------------------------
  Polled Block Transfer
  ---------------------------
  The MSS_QSPI_polled_transfer_block() function is provided to accomplish data 
  transfers where no interrupt is used. The MSS_QSPI_polled_transfer_block() 
  function polls the status register to know the current status of the on-going 
  transfer. This is a blocking function. A MSS QSPI block transfer always has 
  some amount of data to be transmitted (at least one command byte), but 
  receiving useful data from the target memory device is optional. So, if the 
  scheduled block transfer is only transferring data and not receiving any data, 
  then MSS_QSPI_polled_transfer_block() function exits after transmitting the 
  required bytes. If data needs to be received from the target memory device 
  during a particular transfer, the MSS_QSPI_polled_transfer_block() function 
  terminates once the expected data is received from the target memory device.

  --------------------------------
  Interrupt Driven Block Transfer
  --------------------------------
  This block transfer is accomplished using interrupts instead of polling the
  status register. The following functions are provided to support 
  interrupt-driven block transfers:
            - MSS_QSPI_irq_transfer_block()
            - MSS_QSPI_set_status_handler()

  The MSS_QSPI_set_status_handler() function must be used to set a status 
  handler call-back function with the driver. The MSS_QSPI_set_status_handler() 
  function is called back by the driver at two different stages of the transfer. 
  At the first stage, it is called when the required number of bytes are 
  transmitted. At the second stage, if there is data to be received from the 
  target memory device, it is invoked again once the desired data is received. 
  An appropriate status value is passed by the driver as a parameter of this 
  call-back function so that the application infers that an event occurred.

  -----------
  QSPI Status
  -----------
  The MSS_QSPI_read_status() function reads the current status of MSS QSPI. The 
  MSS_QSPI_read_status() function is typically used to know the status of an 
  ongoing transfer. The MSS_QSPI_read_status() function returns the status 
  register value and is called at any time after MSS QSPI is initialized and 
  configured.

  -------------
  Direct Access
  -------------
  MSS QSPI allows direct access to the QSPI interface pins to support access to
  non-standard SPI devices through direct CPU control.
  
  This driver provides the following functions to read and write the direct 
  access register of MSS QSPI:
                  - MSS_QSPI_read_direct_access_reg()
                  - MSS_QSPI_write_direct_access_reg()
  
  Using these functions, you generate any sequence of binary transitions on the
  QSPI pins that might be needed to communicate with non-standard target 
  devices.

 *//*=========================================================================*/

#ifndef MSS_QSPI_H_
#define MSS_QSPI_H_             1

#include "drivers/mss/mss_qspi/mss_qspi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*-------------------------------------------------------------------------*//**
  MSS QSPI Generic Constants
  ==========================
  The following constants are used as input parameter values to configure the
  event on which the SDI pin is sampled. For example:  
  @code
    qspi_config.sample = MSS_QSPI_SAMPLE_POSAGE_SPICLK;
    MSS_QSPI_configure(&qspi_config);
  @endcode
 * */
/*-------------------------------------------------------------------------*//**
  ## MSS_QSPI_SAMPLE_POSAGE_SPICLK
  The SDI pin is sampled at the rising edge of the SPI clock(SPICLK). 
*/
#define MSS_QSPI_SAMPLE_POSAGE_SPICLK       0x00u

/*-------------------------------------------------------------------------*//**
  ## MSS_QSPI_SAMPLE_ACTIVE_SPICLK
  The SDI pin is sampled on the last falling Hardwired Clock (HCLK) edge in the 
  SPI clock(SPICLK) period. 
*/
#define MSS_QSPI_SAMPLE_ACTIVE_SPICLK       0x01u

/*-------------------------------------------------------------------------*//**
  ## MSS_QSPI_SAMPLE_NEGAGE_SPICLK
The SDI pin is sampled at the rising HCLK edge as SPI clock(SPICLK) falls.
*/
#define MSS_QSPI_SAMPLE_NEGAGE_SPICLK       0x02u

/*-------------------------------------------------------------------------*//**
  Public Constant Definitions
  ===========================
  The following constants are used to configure MSS QSPI where a zero or 
  non-zero value, such as enable or disable, is to be provided as an input 
  parameter. For example:  
  @code
    qspi_config.xip = MSS_QSPI_DISABLE;
    MSS_QSPI_configure(&qspi_config);
  @endcode
 * */
#define MSS_QSPI_ENABLE                     0x01u
#define MSS_QSPI_DISABLE                    0x00u

/***************************************************************************//**
These values are used to program the io_format parameter of the configuration
structure of this driver, as shown below:  
  @code
    qspi_config.io_format = MSS_QSPI_QUAD_FULL;
    MSS_QSPI_configure(&qspi_config);
  @endcode

|   Value            |                  Description                            |
|--------------------|---------------------------------------------------------|
|MSS_QSPI_NORMAL     | 1-bit Normal SPI operations                             |
|                    | (single DQ0 TX and single  DQ1 RX lines)                |
|                    |                                                         |
|MSS_QSPI_DUAL_EX_RO | 2-bit SPI operations                                    |
|                    | (command and address bytes on DQ0 only. data on DQ[1:0])|
|                    |                                                         |
|MSS_QSPI_QUAD_EX_RO | 4-bit SPI operations                                    |
|                    | (command and address bytes on DQ0 only. data on DQ[3:0])|
|                    |                                                         |
|MSS_QSPI_DUAL_EX_RW | 2-bit SPI operations                                    |
|                    | (command byte on DQ0 only. Address and data on DQ[1:0]) |
|                    |                                                         |
|MSS_QSPI_QUAD_EX_RW | 4-bit SPI operations                                    |
|                    | (command byte on DQ0 only. Address and data on DQ[3:0]) |
|                    |                                                         |
|MSS_QSPI_DUAL_FULL  | 2-bit SPI operations                                    |
|                    | (command, address, and data on DQ[1:0])                 |
|                    |                                                         |
|MSS_QSPI_QUAD_FULL  | 4-bit SPI operations                                    |
|                    | (command, address, and data on DQ[3:0])                 |
|                    |                                                         |

*/
typedef enum mss_qspi_io_format_t
{
    MSS_QSPI_NORMAL      = 0u,
    MSS_QSPI_DUAL_EX_RO  = 2u,
    MSS_QSPI_QUAD_EX_RO  = 3u,
    MSS_QSPI_DUAL_EX_RW  = 4u,
    MSS_QSPI_QUAD_EX_RW  = 5u,
    MSS_QSPI_DUAL_FULL   = 6u,
    MSS_QSPI_QUAD_FULL   = 7u

} mss_qspi_io_format;

/***************************************************************************//**
These values are used to program the spi_mode parameter of the configuration
structure of this driver. For example:  
  @code
    qspi_config.spi_mode = MSS_QSPI_MODE0;
    MSS_QSPI_configure(&qspi_config);
  @endcode

|   Value           |                   Description                            |
|-------------------|----------------------------------------------------------|
|   MSS_QSPI_MODE0  | Set clock IDLE to low   (0)                              |
|   MSS_QSPI_MODE0  | Set clock IDLE to high  (1)                              |

*/
typedef enum mss_qspi_protocol_mode_t
{
    MSS_QSPI_MODE0    = 0x0u,
    MSS_QSPI_MODE3    = 0x1u

} mss_qspi_protocol_mode;

/***************************************************************************//**
These values are used to program the spi_mode parameter of the configuration
structure of this driver. For example:  
  @code
    qspi_config.clk_div =  MSS_QSPI_CLK_DIV_2;
    MSS_QSPI_configure(&qspi_config);
  @endcode

|        Value          |        Description        |
|-----------------------|---------------------------|
|   MSS_QSPI_CLK_DIV_2  |  Set SPI clock to HCLK/2  |
|   MSS_QSPI_CLK_DIV_4  |  Set SPI clock to HCLK/4  |
|   MSS_QSPI_CLK_DIV_6  |  Set SPI clock to HCLK/6  |
|   MSS_QSPI_CLK_DIV_8  |  Set SPI clock to HCLK/8  |
|   MSS_QSPI_CLK_DIV_10 |  Set SPI clock to HCLK/10 |
|   MSS_QSPI_CLK_DIV_12 |  Set SPI clock to HCLK/12 |
|   MSS_QSPI_CLK_DIV_14 |  Set SPI clock to HCLK/14 |
|   MSS_QSPI_CLK_DIV_16 |  Set SPI clock to HCLK/16 |
|   MSS_QSPI_CLK_DIV_18 |  Set SPI clock to HCLK/18 |
|   MSS_QSPI_CLK_DIV_20 |  Set SPI clock to HCLK/20 |
|   MSS_QSPI_CLK_DIV_22 |  Set SPI clock to HCLK/22 |
|   MSS_QSPI_CLK_DIV_24 |  Set SPI clock to HCLK/24 |
|   MSS_QSPI_CLK_DIV_26 |  Set SPI clock to HCLK/26 |
|   MSS_QSPI_CLK_DIV_28 |  Set SPI clock to HCLK/28 |
|   MSS_QSPI_CLK_DIV_30 |  Set SPI clock to HCLK/30 |

*/
typedef enum mss_qspi_clk_div_t
{
    MSS_QSPI_CLK_DIV_2     = 0x1u,
    MSS_QSPI_CLK_DIV_4     = 0x2u,
    MSS_QSPI_CLK_DIV_6     = 0x3u,
    MSS_QSPI_CLK_DIV_8     = 0x4u,
    MSS_QSPI_CLK_DIV_10    = 0x5u,
    MSS_QSPI_CLK_DIV_12    = 0x6u,
    MSS_QSPI_CLK_DIV_14    = 0x7u,
    MSS_QSPI_CLK_DIV_16    = 0x8u,
    MSS_QSPI_CLK_DIV_18    = 0x9u,
    MSS_QSPI_CLK_DIV_20    = 0xAu,
    MSS_QSPI_CLK_DIV_22    = 0xBu,
    MSS_QSPI_CLK_DIV_24    = 0xCu,
    MSS_QSPI_CLK_DIV_26    = 0xDu,
    MSS_QSPI_CLK_DIV_28    = 0xEu,
    MSS_QSPI_CLK_DIV_30    = 0xFu

} mss_qspi_clk_div;

/***************************************************************************//**
This prototype defines the function prototype that must be followed by MSS QSPI 
status handler functions. This function is registered with the MSS QSPI driver 
through a call to the MSS_QSPI_set_status_handler() function.  

__Declaring and Implementing the Status Handler Function:__  
The Slave frame receive handler functions must follow the following prototype.  
    void transfer_status_handler(uint32_t status);

The actual name of the status handler is unimportant. Use any name of your
choice. The status parameter contains a value indicating which of the TX-DONE or 
RX-DONE events caused the interrupt.
*/
typedef void (*mss_qspi_status_handler_t)(uint32_t status);


/***************************************************************************//**
  This is the structure definition for the MSS QSPI configuration instance. It 
  defines the configuration data that the application exchanges with the driver.

  The following parameters are the configuration options for MSS QSPI.

|  Parameter  |                         Description                            |
|-------------|----------------------------------------------------------------|
|   xip       |  Enable or disable XIP mode                                    |
|   xip_addr  |  Number of address bytes used in XIP mode                      |
|   spi_mode  |  Select either Motorola mode0 or mode3                         |
|   clk_div   |  HCLK Clock divider for generating SPI clock                   |
|   io_format |  QSPI transfer format, extended, dual, quad and so on.         |
|   sample    |  Select the event on which the QSPI samples the incoming data  |

*/
typedef struct mss_qspi_config{
    uint8_t                 xip;
    uint8_t                 xip_addr;
    mss_qspi_protocol_mode  spi_mode;   /*clkidl mode0 or mode3*/
    mss_qspi_clk_div        clk_div;
    mss_qspi_io_format      io_format;
    uint8_t                 sample;
}mss_qspi_config_t;


/*----------------------------------------------------------------------------*/
/*------------------------ MSS QSPI internal structures  ---------------------*/
/*----------------------------------------------------------------------------*/

/***************************************************************************//**
These are register map of the PolarFire SoC MSS QSPI driver.
*/
typedef struct
{
    volatile uint32_t CONTROL;
    volatile uint32_t FRAMES;
    volatile uint32_t RESERVED1;
    volatile uint32_t INTENABLE;
    volatile uint32_t STATUS;
    volatile uint32_t DIRECT;
    volatile uint8_t ADDRUP;
    volatile uint8_t ADDRUP_R1;
    volatile uint8_t ADDRUP_R2;
    volatile uint8_t ADDRUP_R3;
    volatile uint32_t RESERVED2[9];
    volatile uint8_t RXDATAX1;
    volatile uint8_t RXDATAX1_R1;
    volatile uint8_t RXDATAX1_R2;
    volatile uint8_t RXDATAX1_R3;
    volatile uint8_t TXDATAX1;
    volatile uint8_t TXDATAX1_R1;
    volatile uint8_t TXDATAX1_R2;
    volatile uint8_t TXDATAX1_R3;
    volatile uint32_t RXDATAX4;
    volatile uint32_t TXDATAX4;
    volatile uint32_t FRAMESUP;

} QSPI_TypeDef;

/*-------------------------------------------------------------------------*//**
  QSPI_BASE
  =========
  The QSPI_BASE constant provides the PolarFire SoC MSS QSPI base memory 
  address.
*/
#define QSPI_BASE               0x21000000u

/*-------------------------------------------------------------------------*//**
  QSPI
  ====
  The QSPI constant provides the PolarFire SoC MSS QSPI hardware instance.
*/
#define QSPI                    ((QSPI_TypeDef *) QSPI_BASE)



/*----------------------------------------------------------------------------*/
/*------------------------MSS QSPI Public APIs--------------------------------*/
/*----------------------------------------------------------------------------*/



/***************************************************************************//**
  The MSS_QSPI_init() function initializes and enables PolarFireSoC MSS QSPI.

  It enables the MSS QSPI hardware block and configures it with the default
  values.

  @param 
    The MSS_QSPI_init() function takes no function parameters.

  @return
    The MSS_QSPI_init() function does not return a value.
 */
void MSS_QSPI_init
(
    void
);

/***************************************************************************//**
  The MSS_QSPI_enable() function enables the MSS QSPI hardware block.
  
  @param 
    The MSS_QSPI_enable() function takes no function parameters.
    
  @return
    The MSS_QSPI_enable() function does not return a value.
 */
static inline void MSS_QSPI_enable(void)
{
    QSPI->CONTROL |= CTRL_EN_MASK;
}

/***************************************************************************//**
  The MSS_QSPI_disable() function disables the MSS QSPI hardware block.
  
  @param 
    The MSS_QSPI_disable() function takes no function parameters.
    
  @return
    The MSS_QSPI_disable() function does not return a value.
 */
static inline void MSS_QSPI_disable(void)
{
    QSPI->CONTROL &= ~CTRL_EN_MASK;
}

/***************************************************************************//**
  The MSS_QSPI_configure() function configures MSS QSPI to the desired 
  configuration values.

  @param config
    The config parameter is a pointer to the mss_qspi_config_t structure that 
    provides new configuration values. For more details, see mss_qspi_config_t 
    section.

  @return
    The MSS_QSPI_configure() function does not return a value.
 */
void MSS_QSPI_configure
(
    const mss_qspi_config_t* config
);

/***************************************************************************//**
  The MSS_QSPI_get_config() function reads back the current configurations of 
  MSS QSPI. The MSS_QSPI_get_config() function is used when you want to read 
  the current configurations, modify the configuration values of your choice, 
  and reconfigure the MSS QSPI hardware using the MSS_QSPI_configure() function.

  @param config
    The config parameter is a pointer to the mss_qspi_config_t structure that 
    returns the current configuration values of MSS QSPI. For more details, see 
    mss_qspi_config_t section.

  @return
    The MSS_QSPI_get_config() function does not return a value.
 */
void MSS_QSPI_get_config
(
    mss_qspi_config_t* config
);

/***************************************************************************//**
  The MSS_QSPI_polled_transfer_block() function carries out a QSPI transfer with 
  the target memory device using the polling method of data transfer. The QSPI 
  transfer characteristics are configured every time a new transfer is 
  initiated. This is a blocking function.

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
    The rd_buffer parameter is the pointer to the buffer that stores the data 
    returned by the target memory device.

  @param rd_byte_size
    The rd_byte_size parameter is the exact number of bytes that need to be
    received from the target memory device.

  @param num_idle_cycles
    The num_idle_cycles parameter indicates the number of idle cycles, or dummy 
    clock edges, that must be generated after the address bytes are transmitted 
    and before the target memory device starts sending data. This must be 
    correctly set based on the target memory device and the SPI command being 
    used. This may also vary based on the SPI clock and the way the target 
    memory device is configured.

  @return
    The MSS_QSPI_polled_transfer_block() function does not return a value.
 */
void MSS_QSPI_polled_transfer_block
(
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
);

/***************************************************************************//**
  The MSS_QSPI_irq_transfer_block() function carries out a QSPI transfer with 
  the target memory device using the interrupt method of data transfers. The 
  QSPI transfer characteristics are configured every time a new transfer is 
  initiated. This is a non-blocking function. You must configure the interrupt 
  handler function before calling the MSS_QSPI_irq_transfer_block() function. It 
  enables the interrupts and starts transmitting as many bytes as requested. 
  When the Transmit-Done interrupt event occurs and this driver calls back the 
  interrupt handler function that you previously provided, you get an indication 
  that the actual SPI transmit process is complete. If the transfer includes 
  receiving data from the target memory device, then the receive-available and 
  receive-done interrupts are also enabled by the MSS_QSPI_irq_transfer_block() 
  function. The data is received in the interrupt routine. When the Receive-Done 
  interrupt event occurs, the interrupt handler you provided is called again.

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
  The rd_buffer parameter is the pointer to the buffer that stores the data 
  returned by the target memory device. This must be correctly set based on the 
  transfer. That is, for the status, readid, and other configuration read 
  commands, this must be set, however for the configuration write commands, 
  this must be NULL.

  @param rd_byte_size
    The rd_byte_size parameter indicates the exact number of bytes that need to 
    be received from the target memory device.

  @param num_idle_cycles
    The num_idle_cycles parameter indicates the number of idle cycles, or dummy 
    clock edges, that must be generated after the address bytes are transmitted 
    and before target memory device starts sending data. This must be correctly 
    set based on the target memory device and the SPI command being used, This 
    may also vary based on the SPI clock and the way the target memory device is 
    configured.

  @return
    The MSS_QSPI_irq_transfer_block() function returns a non-zero value if MSS 
    QSPI is busy completing the previous transfer and does not accept a new 
    transfer. A zero return value indicates successful execution of the 
    MSS_QSPI_irq_transfer_block function.
 */
uint8_t MSS_QSPI_irq_transfer_block
(
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
);

/***************************************************************************//**
  The MSS_QSPI_set_status_handler() function registers an interrupt handler 
  function with this driver that returns the interrupt status back to the 
  application. This status handler function is called by this driver on two 
  events. First, when the transmission of the required bytes is completed 
  (Transmit-Done). Second, if data is to be received from the target memory 
  device, the MSS_QSPI_set_status_handler() function is called again when the 
  required data is received (Receive-Done).

  @param handler
  The handler parameter is the interrupt handler function of the 
  mss_qspi_status_handler_t type that must be registered.

  @return
    The MSS_QSPI_set_status_handler() function does not return a value.
 */
void MSS_QSPI_set_status_handler
(
   mss_qspi_status_handler_t handler
);

/***************************************************************************//**
  The MSS_QSPI_read_direct_access_reg() function reads the current value of the 
  Direct Access Register (DAR) of MSS QSPI. DAR allows direct access to the QSPI 
  interface pins to support access to non-standard SPI devices through direct 
  CPU control.
  
  @param 
    The MSS_QSPI_read_direct_access_reg() function takes no function parameters.

  @return
    The MSS_QSPI_read_direct_access_reg() function returns the current value of 
    DAR of MSS QSPI.
 */
static inline uint32_t MSS_QSPI_read_direct_access_reg(void)
{
    return(QSPI->DIRECT);
}

/***************************************************************************//**
  The MSS_QSPI_write_direct_access_reg() function writes a value of DAR of MSS 
  QSPI. DAR allows direct access to the QSPI interface pins to support access to 
  non-standard SPI devices through direct CPU control.

  @param value
  The value parameter is the value that must set into DAR of MSS QSPI.

  @return
    The MSS_QSPI_write_direct_access_reg() function does not return a value.
 */
static inline void MSS_QSPI_write_direct_access_reg(uint32_t value)
{
    QSPI->DIRECT = value;
}

/***************************************************************************//**
  The MSS_QSPI_read_status() function reads the status of MSS QSPI. This 
  function returns the status register value and is called any time after MSS 
  QSPI is initialized and configured.

  @param 
    The MSS_QSPI_read_status() function takes no function parameters.
    
  @return
    The MSS_QSPI_read_status() function returns the current value of the status 
    register of MSS QSPI.
 */
static inline uint32_t MSS_QSPI_read_status(void)
{
    return(QSPI->STATUS);
}

#ifdef __cplusplus
}
#endif

#endif /* MSS_QSPI_H_*/
