/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
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
 * PolarFire SoC Microprocessor Subsystem MMUART bare metal software driver
 * public API.
 *
 */
/*=========================================================================*//**
  @mainpage PolarFire SoC MSS UART Bare Metal Driver.
  
  ==============================================================================
  Introduction
  ==============================================================================
  The PolarFire SoC Microprocessor subsystem (MSS) includes five multi-mode UART
  (MMUART) peripherals for serial communication. This driver provides a set of
  functions for controlling the MSS MMUARTs as part of a bare metal system
  where no operating system is available. These drivers can be adapted for use
  as part of an operating system, but the implementation of the adaptation layer
  between this driver and the operating system's driver model is outside the
  scope of this driver.
  Note: MSS UART is synonymous with MSS MMUART in this document.
  
  ==============================================================================
  Hardware Flow Dependencies
  ==============================================================================
  The configuration of all features of the MSS MMUART peripherals is covered by
  this driver with the exception of the PolarFire SoC IOMUX configuration.
  PolarFire SoC allows multiple non-concurrent uses of some external pins 
  through IOMUX configuration. This feature allows optimization of external pin 
  usage by assigning external pins for use by either the microprocessor
  subsystem or the FPGA fabric. The MSS MMUART serial signals are routed through
  IOMUXs to the PolarFire SoC device external pins. The MSS MMUART serial 
  signals may also be routed through IOMUXs to the PolarFire SoC FPGA fabric. 
  For more information on IOMUX, refer to the I/O Configuration section of the
  PolarFire SoC Microprocessor Subsystem (MSS) User's Guide.

  The IOMUXs are configured using the PolarFire SoC MSS configurator tool. You
  must ensure that the MSS MMUART peripherals are enabled and configured in the
  PolarFire SoC MSS configurator if you wish to use them. For more information 
  on IOMUXs, refer to the IOMUX section of the PolarFire SoC microprocessor
  Subsystem (MSS) User's Guide.

  On PolarFire SoC an AXI switch forms a bus matrix interconnect among multiple 
  masters and multiple slaves. Five RISC-V CPUs connect to the Master ports 
  M10 to M14 of the AXI switch. By default, all the APB peripherals are 
  accessible on AXI-Slave 5 of the AXI switch via the AXI to AHB and AHB to APB
  bridges (referred as main APB bus). However, to support logical separation in 
  the Asymmetric Multi-Processing (AMP) mode of operation, the APB peripherals 
  can alternatively be accessed on the AXI-Slave 6 via the AXI to AHB and AHB to 
  APB bridges (referred as the AMP APB bus).
  
  Application must make sure that the desired MMUART instance is appropriately 
  configured on one of the APB bus described above by configuring the PolarFire 
  SoC system registers (SYSREG) as per the application need and that the 
  appropriate data structures are provided to this driver as parameter to the 
  functions provided by this driver.

  The base address and register addresses are defined in this driver as 
  constants. The interrupt number assignment for the MSS MMUART peripherals are 
  defined as constants in the MPFS HAL. You must ensure that the latest MPFS HAL 
  is included in the project settings of the SoftConsole tool chain and that it 
  is generated into your project.
  
  ==============================================================================
  Theory of Operation
  ==============================================================================
  The MSS MMUART driver functions are grouped into the following categories:
    - Initialization and configuration functions
    - Polled transmit and receive functions
    - Interrupt driven transmit and receive functions
    
  --------------------------------
  Initialization and Configuration
  --------------------------------
  The MSS MMUART supports the following four broad modes of operation:
    - UART or USART mode
    - LIN mode
    - IrDA mode
    - Smartcard or ISO 7816 mode
    
  The MSS MMUART driver provides the MSS_UART_init(), MSS_UART_lin_init(),
  MSS_UART_irda_init() and MSS_UART_smartcard_init() functions to initialize the
  MSS MMUARTs for operation in one of these modes. One of these initialization
  functions must be called before any other MSS MMUART driver functions can be
  called. The MSS MMUART operating modes are mutually exclusive; therefore only
  one of the initialization functions must be called. The first parameter of the
  initialization functions is a pointer to one of ten global data structures
  used to store state information for each MSS MMUART. A pointer to these data
  structures is also used as the first parameter to many of the driver functions
  to identify which MSS MMUART will be used by the called function. The names of
  these data structures are:
    - g_mss_uart0_lo
    - g_mss_uart1_lo
    - g_mss_uart2_lo
    - g_mss_uart3_lo
    - g_mss_uart4_lo
    - g_mss_uart0_hi
    - g_mss_uart1_hi
    - g_mss_uart2_hi
    - g_mss_uart3_hi
    - g_mss_uart4_hi
    
  Therefore, any call to an MSS MMUART function should be of the form 
  MSS_UART_function_name( &g_mss_uart0_lo, ... ) or
  MSS_UART_function_name( &g_mss_uart1_hi, ... ).
  
  UART or USART Mode
  For the UART or USART modes of operation, the MSS MMUART driver is initialized
  through a call to the MSS_UART_init() function. This function takes the UART's
  configuration as its parameters. The MSS_UART_init() function must be called
  before any other MSS MMUART driver functions can be called.
  The MSS_UART_init() function configures the baud rate based on the input baud
  rate parameter and if possible uses a fractional baud rate for greater
  precision. This function disables the LIN, IrDA and SmartCard modes.
  
  LIN mode
  For the LIN mode of operation, the MSS MMUART driver is initialized through a
  call to the MSS_UART_lin_init() function. This function takes the LIN node's
  configuration as its parameters. The MSS_UART_lin_init() function must be
  called before any other MSS MMUART driver functions can be called. The
  MSS_UART_lin_init() function configures the baud rate based on the input baud
  rate parameter and if possible uses a fractional baud rate for greater
  precision. This function disables the IrDA and SmartCard modes.
  The driver also provides the following LIN mode configuration functions:
    - MSS_UART_set_break()
    - MSS_UART_clear_break()
    - MSS_UART_set_pidpei_handler()
    - MSS_UART_set_linbreak_handler()
    - MSS_UART_set_linsync_handler()
    
  Note: These LIN mode configuration functions can only be called after the
  MSS_UART_lin_init() function is called.
  
  IrDA mode
  For the IrDA mode of operation, the driver is initialized through a call to
  the MSS_UART_irda_init() function. This function takes the IrDA node's
  configuration as its parameters. The MSS_UART_irda_init() function must be
  called before any other MSS MMUART driver functions can be called. The
  MSS_UART_irda_init() function configures the baud rate based on the input baud
  rate parameter and if possible uses a fractional baud rate for greater
  precision. This function disables the LIN and SmartCard modes.
  
  Smartcard or ISO 7816 mode
  For the Smartcard or ISO 7816 mode of operation, the driver is initialized
  through a call to the MSS_UART_smartcard_init() function. This function takes
  the smartcard configuration as its parameters. The MSS_UART_smartcard_init()
  function must be called before any other MSS MMUART driver functions can be
  called. The MSS_UART_smartcard_init() function configures the baud rate based
  on the input baud rate parameter and if possible uses a fractional baud rate
  for greater precision. This function disables the LIN and IrDA modes.
  The driver also provides the following Smartcard mode configuration functions:
    - MSS_UART_enable_halfduplex()
    - MSS_UART_disable_halfduplex()
    - MSS_UART_set_nack_handler()
    
  Note: These Smartcard mode configuration functions can only be called after
  the MSS_UART_smartcard_init() function is called.
  
  Common Configuration Functions
  The driver also provides the configuration functions that can be used with all
  MSS MMUART operating modes. These common configuration functions are as
  follows:
    - MSS_UART_set_rx_endian()
    - MSS_UART_set_tx_endian()
    - MSS_UART_enable_afclear()
    - MSS_UART_disable_afclear()
    - MSS_UART_enable_rx_timeout()
    - MSS_UART_disable_rx_timeout()
    - MSS_UART_enable_tx_time_guard()
    - MSS_UART_disable_tx_time_guard()
    - MSS_UART_set_address()
    - MSS_UART_set_ready_mode()
    - MSS_UART_set_usart_mode()
    - MSS_UART_set_filter_length()
    - MSS_UART_enable_afm()
    - MSS_UART_disable_afm()
    
  Note: These configuration functions can only be called after one of the
        MSS_UART_init(), MSS_UART_lin_init(), MSS_UART_irda_init() or
        MSS_UART_smartcard_init() functions is called.
  
  --------------------------------------  
  Polled Transmit and Receive Operations
  --------------------------------------
  The driver can be used to transmit and receive data once initialized. 
  Data is transmitted using the MSS_UART_polled_tx() function. This function is
  blocking, meaning that it will only return once the data passed to the
  function has been sent to the MSS MMUART hardware transmitter. Data received
  by the MSS MMUART hardware receiver can be read by the MSS_UART_get_rx()
  function.
  The MSS_UART_polled_tx_string() function is provided to transmit a NUL ('\0')
  terminated string in polled mode. This function is blocking, meaning that it
  will only return once the data passed to the function has been sent to the MSS
  MMUART hardware transmitter.
  The MSS_UART_fill_tx_fifo() function fills the MSS MMUART hardware transmit
  FIFO with data from a buffer passed as a parameter and returns the number of
  bytes transferred to the FIFO. If the transmit FIFO is not empty when the
  MSS_UART_fill_tx_fifo() function is called it returns immediately without
  transferring any data to the FIFO.
  
  ---------------------------
  Interrupt Driven Operations
  ---------------------------
  The driver can also transmit or receive data under interrupt control, freeing
  your application to perform other tasks until an interrupt occurs indicating
  that the driver's attention is required.
  
  Local or PLIC interrupt:
  PolarFire SoC architecture provides flexibility in terms of how the MMUART 
  interrupt is seen by the PolarFire SoC Core Complex. Each of the 5 MMUART
  instance interrupt line is connected to the PolarFire SoC Core Complex PLIC. 
  The MMUART0 instance interrupt line is also available as local interrupt on
  E51 processor. The MMUART1 instance interrupt onwards are available as local
  interrupt on the U54_1 processor onwards. e.g. MMUART2 interrupt is available 
  as local interrupt on U54_2. 

  Interrupt Handlers
  The MSS MMUART driver supports all types of interrupt triggered by the MSS
  MMUART. The driver's internal top level interrupt handler identifies the
  source of the MSS MMUART interrupt and calls the corresponding lower level
  handler function that you previously registered with the driver through calls
  to the MSS_UART_set_rx_handler(), MSS_UART_set_tx_handler(),
  MSS_UART_set_rxstatus_handler(), and MSS_UART_set_modemstatus_handler()
  functions. You are responsible for creating these lower level interrupt
  handlers as part of your application program and registering them with the
  driver.
  Note: The PolarFire SoC HAL defines the interrupt handler functions for all 
        5 MMUART instances(with weak linkage) and assigns them as the interrupt 
        service routines (ISR) for the MSS MMUART interrupt inputs to the 
        PolarFire SoC Core Complex PLIC or the Local interrupts on each of the 
        respective CPUs. The MSS MMUART driver provides the implementation 
        functions all these ISRs from which it calls its own internal top level,
        interrupt handler function.
        
  The MSS_UART_enable_irq() and MSS_UART_disable_irq() functions are used to
  enable or disable the received line status, received data available/character
  time-out, transmit holding register empty and modem status interrupts at the
  MSS MMUART level. The MSS_UART_enable_irq() function also enables the MSS
  MMUART instance interrupt at the PolarFire SoC Core Complex level.
  
  Note that the MSS_UART_enable_irq() and MSS_UART_disable_irq() and all the
  calls to set the handler functions assume that the MMUART interrupt is
  connected to the PolarFire SoC Core Complex PLIC. If you want the MMUART
  interrupt to appear as a local interrupt to the corresponding HART then you
  must explicitly call the MSS_UART_enable_local_irq() function. This function
  will disable the PLIC interrupt (if it was previously enable) and enable the
  local interrupt on the HART on which this function is being executed.

  Transmitting Data
  Interrupt-driven transmit is initiated by a call to MSS_UART_irq_tx(),
  specifying the block of data to transmit. Your application is then free to
  perform other tasks and inquire later whether transmit has completed by
  calling the MSS_UART_tx_complete() function. The MSS_UART_irq_tx() function
  enables the UART's transmit holding register empty (THRE) interrupt and then,
  when the interrupt goes active, the driver's default THRE interrupt handler
  transfers the data block to the UART until the entire block is transmitted.
  Note: You can use the MSS_UART_set_tx_handler() function to assign an
        alternative handler to the THRE interrupt. In this case, you must not
        use the MSS_UART_irq_tx() function to initiate the transmit, as this
        will re-assign the driver's default THRE interrupt handler to the THRE
        interrupt. Instead, your alternative THRE interrupt handler must include
        a call to the MSS_UART_fill_tx_fifo() function to transfer the data to
        the UART.
  
  Receiving Data
  Interrupt-driven receive is performed by first calling
  MSS_UART_set_rx_handler() to register a receive handler function that will be
  called by the driver whenever receive data is available. You must provide this
  receive handler function which must include a call to the MSS_UART_get_rx()
  function to actually read the received data.
  
  -----------
  UART Status
  -----------
  The function MSS_UART_get_rx_status() is used to read the receiver error
  status. This function returns the overrun, parity, framing, break, and FIFO
  error status of the receiver.
  The function MSS_UART_get_tx_status() is used to read the transmitter status.
  This function returns the transmit empty (TEMT) and transmit holding register
  empty (THRE) status of the transmitter.
  The function MSS_UART_get_modem_status() is used to read the modem status
  flags. This function returns the current value of the modem status register.
  
  --------
  Loopback
  --------
  The MSS_UART_set_loopback() function can be used to locally loopback the Tx
  and Rx lines of a UART. This is not to be confused with the loopback of UART0
  to UART1, which can be achieved through the microprocessor subsystem's system
  registers.
  
 *//*=========================================================================*/
#ifndef __MSS_CORE_IHC_H_
#define __MSS_CORE_IHC_H_ 1

#include <stddef.h>
#include <stdint.h>
#include "mss_core_ihc_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t (*QUEUE_IHC_INCOMING)(uint32_t , uint32_t, uint8_t * );

extern IHC_TypeDef * IHC[];

/***************************************************************************//**
  The MSS_UART_init() function initializes and configures one of the PolarFire SoC
  MSS UARTs with the configuration passed as a parameter. The configuration
  parameters are the baud_rate which is used to generate the baud value and the
  line_config which is used to specify the line configuration (bit length,
  stop bits and parity).

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are ten such data structures,
    g_mss_uart0_lo to g_mss_uart4_lo, associated with MSS UART0 to MSS UART4
    when they are connected on the AXI switch slave 5 (main APB bus) and
    g_mss_uart0_hi to g_mss_uart4_hi, associated with MSS UART0 to MSS UART4
    when they are connected on the AXI switch slave 6 (AMP APB bus).
    This parameter must point to one of these ten global data structure defined
    within the UART driver.

    Note that if you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param baud_rate
    The baud_rate parameter specifies the baud rate. It can be specified for
    common baud rates using the following defines:
    - MSS_UART_110_BAUD
    - MSS_UART_300_BAUD
    - MSS_UART_600_BAUD
    - MSS_UART_1200_BAUD
    - MSS_UART_2400_BAUD
    - MSS_UART_4800_BAUD
    - MSS_UART_9600_BAUD
    - MSS_UART_19200_BAUD
    - MSS_UART_38400_BAUD
    - MSS_UART_57600_BAUD
    - MSS_UART_115200_BAUD
    - MSS_UART_230400_BAUD
    - MSS_UART_460800_BAUD
    - MSS_UART_921600_BAUD
    
    Alternatively, any nonstandard baud rate can be specified by simply passing
    the actual required baud rate as the value for this parameter.

  @param line_config
    The line_config parameter is the line configuration specifying the bit length,
    number of stop bits and parity settings.
    
    This is a bitwise OR of one value from each of the following groups of
    allowed values:
    
    One of the following to specify the transmit/receive data bit length:
     - MSS_UART_DATA_5_BITS
     - MSS_UART_DATA_6_BITS,
     - MSS_UART_DATA_7_BITS
     - MSS_UART_DATA_8_BITS
     
    One of the following to specify the parity setting:
     - MSS_UART_NO_PARITY
     - MSS_UART_EVEN_PARITY
     - MSS_UART_ODD_PARITY
     - MSS_UART_STICK_PARITY_0
     - MSS_UART_STICK_PARITY_1
        
    One of the following to specify the number of stop bits:
     - MSS_UART_ONE_STOP_BIT
     - MSS_UART_ONEHALF_STOP_BIT
     - MSS_UART_TWO_STOP_BITS

  @return
    This function does not return a value.

  Example:
  @code
  #include "mss_uart.h"

  int main(void)
  {
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
                   
     return(0);
  }
  @endcode
 */
uint32_t
queue_incoming
(
    uint32_t remote_hart_id,
    uint32_t my_hart_id,
    uint8_t * message
);

uint32_t tx_message(uint32_t remote_hart_id, uint32_t my_hart_id, uint8_t *message);
uint32_t rx_message(uint32_t remote_hart_id, uint32_t my_hart_id, QUEUE_IHC_INCOMING handle_incoming);

/**
 * Temp during dev
 * @return
 */
uint32_t print_mem_map_core_inc(void);

#ifdef DEBUG_IHC
void test_state_machine(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* __MSS_CORE_IHC_H_ */
