/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_uart.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) MMUART bare metal
 * software driver public API.
 *
 */

/*=========================================================================*//**
  @mainpage PolarFire® SoC MSS UART Bare Metal Driver.

  ==============================================================================
  Introduction
  ==============================================================================
  The PolarFire® SoC Microprocessor Subsystem (MSS) includes five multi-mode 
  UART(MMUART) peripherals for serial communication. This driver provides a set
  of functions to control the MSS MMUARTs as part of a bare metal system
  where no operating system is available. These drivers can be adapted for use
  as part of an operating system, but the implementation of the adaptation layer
  between this driver and the operating system's driver model is outside the
  scope of this driver.
  Note: MSS UART and MSS MMUART are synonymous in this document.

  ==============================================================================
  Hardware Flow Dependencies
  ==============================================================================
  The configuration of all features of the MSS MMUART peripherals is covered by
  this driver with the exception of the PolarFire® SoC IOMUX configuration.
  PolarFire® SoC allows multiple non-concurrent uses of external pins
  through IOMUX configuration. This feature allows optimization of external pin
  usage by assigning external pins for use by either the microprocessor
  subsystem or the FPGA fabric. The MSS MMUART serial signals are routed through
  IOMUXs to the PolarFire® SoC device external pins. The MSS MMUART serial
  signals may also be routed through IOMUXs to the PolarFire® SoC FPGA fabric.
  For more information on IOMUX, see the I/O Configuration section of the
  PolarFire® SoC Microprocessor Subsystem (MSS) User's Guide.

  The IOMUXs are configured using the PolarFire® SoC MSS configurator tool. You
  must ensure that the MSS MMUART peripherals are enabled and configured in the
  PolarFire® SoC MSS configurator if you wish to use them. For more information
  on IOMUXs, see the IOMUX section of the PolarFire® SoC Microprocessor
  Subsystem (MSS) User's Guide.

  On PolarFire® SoC, an AXI switch forms a bus matrix interconnect among 
  multiple masters and multiple slaves. Five RISC-V CPUs connect to the Master 
  ports M10 to M14 of the AXI switch. By default, all the APB peripherals are
  accessible on AXI-Slave 5 of the AXI switch using the AXI to AHB and AHB to APB
  bridges (referred as main APB bus). However, to support logical separation in
  the Asymmetric Multi-Processing (AMP) mode of operation, the APB peripherals
  can alternatively be accessed on the AXI-Slave 6 using the AXI to AHB and AHB to
  APB bridges (referred as the AMP APB bus).

  Application must make sure that the desired MMUART instance is appropriately
  configured on one of the APB bus as described above by configuring the 
  PolarFire® SoC system registers (SYSREG) as per the application need and that 
  the appropriate data structures are provided to this driver as parameter to 
  the functions provided by this driver.

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
  MSS_UART_irda_init(), and MSS_UART_smartcard_init() functions to initialize the
  MSS MMUARTs for operation in one of these modes. One of these initialization
  functions must be called before calling any other MSS MMUART driver functions.
  The MSS MMUART operating modes are mutually exclusive; therefore, 
  only one of the initialization functions can be called. The first parameter of 
  the initialization functions is a pointer to one of ten global data structures
  used to store state information for each MSS MMUART. A pointer to these data
  structures is also used as the first parameter to many of the driver functions
  to identify which MSS MMUART is used by the called function. The names of
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
  For the UART or USART modes of operation, call the MSS_UART_init() function to
  initialize the MSS MMUART instance. This function takes the UART's
  configuration as its parameters. The MSS_UART_init() function must be called
  before calling any other MSS MMUART driver functions.
  The MSS_UART_init() function configures the baud rate based on the input baud
  rate parameter and, if possible, uses a fractional baud rate for greater
  precision. This function disables the LIN, IrDA, and SmartCard modes.

  LIN Mode    
  For the LIN mode of operation, call the MSS_UART_lin_init() function to
  initialize the MSS MMUART instance. This function takes the LIN node's
  configuration as its parameters. The MSS_UART_lin_init() function must be
  called before calling any other MSS MMUART driver functions. The
  MSS_UART_lin_init() function configures the baud rate based on the input baud
  rate parameter and, if possible, uses a fractional baud rate for greater
  precision. This function disables the IrDA and SmartCard modes.
  The driver also provides the following LIN mode configuration functions:
    - MSS_UART_set_break()
    - MSS_UART_clear_break()
    - MSS_UART_set_pidpei_handler()
    - MSS_UART_set_linbreak_handler()
    - MSS_UART_set_linsync_handler()

  Note: These LIN mode configuration functions can only be called after calling 
  the MSS_UART_lin_init() function.

  IrDA Mode    
  For the IrDA mode of operation, call the MSS_UART_irda_init() function to
  initialize the MSS MMUART instance. This function takes the IrDA node's
  configuration as its parameters. The MSS_UART_irda_init() function must be
  called before calling any other MSS MMUART driver functions. The
  MSS_UART_irda_init() function configures the baud rate based on the input baud
  rate parameter and, if possible, uses a fractional baud rate for greater
  precision. This function disables the LIN and SmartCard modes.

  Smartcard or ISO 7816 Mode    
  For the Smartcard or ISO 7816 mode of operation, call the
  MSS_UART_smartcard_init() function to initialize the MSS MMUART instance. This
  function takes the smartcard configuration as its parameters. The
  MSS_UART_smartcard_init() function must be called before calling any other
  MSS MMUART driver functions. The MSS_UART_smartcard_init() function configures
  the baud rate based on the input baud rate parameter and, if possible, uses a
  fractional baud rate for greater precision. This function disables the LIN and
  IrDA modes. The driver also provides the following Smartcard mode
  configuration functions:
    - MSS_UART_enable_halfduplex()
    - MSS_UART_disable_halfduplex()
    - MSS_UART_set_nack_handler()

  Note: These Smartcard mode configuration functions can only be called after
  calling the MSS_UART_smartcard_init() function.

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

  Note: These configuration functions can only be called after calling one of
        the MSS_UART_init(), MSS_UART_lin_init(), MSS_UART_irda_init(), or
        MSS_UART_smartcard_init() function.

  --------------------------------------
  Polled Transmit and Receive Operations
  --------------------------------------
  The driver can be used to transmit and receive data once initialized.    
  Data is transmitted using the MSS_UART_polled_tx() function. This function is
  blocking, which means that it will only return once the data passed to the
  function has been sent to the MSS MMUART hardware transmitter. Data received
  by the MSS MMUART hardware receiver can be read by the MSS_UART_get_rx()
  function.    
  The MSS_UART_polled_tx_string() function is provided to transmit a NUL ('\0')
  terminated string in polled mode. This function is blocking, which means that
  it will only return once the data passed to the function has been sent to the
  MSS MMUART hardware transmitter.    
  The MSS_UART_fill_tx_fifo() function fills the MSS MMUART hardware transmit
  FIFO with data from a buffer passed as a parameter and returns the number of
  bytes transferred to the FIFO. If the transmit FIFO is not empty when the
  MSS_UART_fill_tx_fifo() function is called, it returns immediately without
  transferring any data to the FIFO.

  ---------------------------
  Interrupt Driven Operations
  ---------------------------
  The driver can also transmit or receive data under interrupt control, which
  frees your application to perform other tasks until an interrupt occurs,
  which indicates that the driver's attention is required.

  Local or PLIC Interrupt    
  PolarFire® SoC architecture provides flexibility in terms of how the MMUART
  interrupt is seen by the PolarFire® SoC Core Complex. Each of the five MMUART
  instance interrupt lines is connected to the PolarFire® SoC Core Complex PLIC.
  The MMUART0 instance interrupt line is also available as local interrupt on
  E51 processor. The MMUART1 instance interrupt onwards are available as local
  interrupt on the U54_1 processor onwards. For example MMUART2 interrupt is
  available as local interrupt on U54_2 and so on.

  Interrupt Handlers    
  The MSS MMUART driver supports all types of interrupt triggered by the MSS
  MMUART. The driver's internal top level interrupt handler identifies the
  source of the MSS MMUART interrupt and calls the corresponding lower level
  handler function that you previously registered with the driver through calls
  to the MSS_UART_set_rx_handler(), MSS_UART_set_tx_handler(),
  MSS_UART_set_rxstatus_handler(), and MSS_UART_set_modemstatus_handler()
  functions. You are responsible for creating these lower level interrupt
  handlers as part of your application and registering them with the
  driver.    
  Note: The PolarFire® SoC HAL defines the interrupt handler functions for all
        five MMUART instances (with weak linkage) and assigns them as the
        interrupt service routines (ISR) for the MSS MMUART interrupt inputs to
        the PolarFire® SoC Core Complex PLIC or the Local interrupts on each of
        the respective CPUs. The MSS MMUART driver provides the implementation
        functions to all these ISRs from which it calls its own internal top
        level, interrupt handler function.

  The MSS_UART_enable_irq() and MSS_UART_disable_irq() functions are used to
  enable or disable the received line status, received data available/character
  time-out, transmit holding register empty, and modem status interrupts at the
  MSS MMUART level.

  Note: MSS_UART_enable_irq(), MSS_UART_disable_irq(), and all the
  calls to set the handler functions assume that the MMUART interrupt is
  connected to the PolarFire® SoC Core Complex PLIC. If you want the MMUART
  interrupt to appear as a local interrupt to the corresponding hart then you
  must explicitly call the MSS_UART_enable_local_irq() function. This function
  disables the PLIC interrupt (if it was previously enable) and enable the
  local interrupt on the hart on which this function is being executed.

  Transmitting Data    
  Interrupt-driven transmit is initiated by calling MSS_UART_irq_tx(),
  that specifies the block of data to transmit. Your application is then free to
  perform other tasks and inquire later whether transmit is complete by
  calling the MSS_UART_tx_complete() function. The MSS_UART_irq_tx() function
  enables the UART's transmit holding register empty (THRE) interrupt and then,
  when the interrupt goes active, the driver's THRE interrupt handler
  transfers the data block to the UART until the entire block is transmitted.    
  Note: You can use the MSS_UART_set_tx_handler() function to assign an
        alternative handler to the THRE interrupt. In this case, your 
        alternative THRE interrupt handler must call MSS_UART_fill_tx_fifo()
        to transfer the data to the UART.

  Receiving Data    
  Interrupt-driven receive is performed by first calling
  MSS_UART_set_rx_handler() to register a receive handler function that is
  called by the driver whenever receive data is available. You must provide this
  receive handler function which must call the MSS_UART_get_rx()
  function to actually read the received data.

  -----------
  UART Status
  -----------
  The function MSS_UART_get_rx_status() reads the receiver error
  status. This function returns the overrun, parity, framing, break, and FIFO
  error status of the receiver.    
  The function MSS_UART_get_tx_status() reads the transmitter status.
  This function returns the transmit empty (TEMT) and transmit holding register
  empty (THRE) status of the transmitter.    
  The function MSS_UART_get_modem_status() reads the modem status
  flags. This function returns the current value of the modem status register.

  --------
  Loopback
  --------
  The MSS_UART_set_loopback() function is used to locally loopback the Tx
  and Rx lines of a UART. This is not to be confused with the loopback of UART0
  to UART1, which can be achieved through the microprocessor subsystem's system
  registers.

 *//*=========================================================================*/
#ifndef __MSS_UART_H_
#define __MSS_UART_H_ 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************//**
  Baud rates
  ==========
  The following definitions are used to specify standard baud rates as a
  parameter to the MSS_UART_init(), MSS_UART_lin_init(), MSS_UART_irda_init(),
  and MSS_UART_smartcard_init() functions.

  | Constant             | Description      |
  |----------------------|------------------|
  | MSS_UART_110_BAUD    |    110 baud rate |
  | MSS_UART_300_BAUD    |    300 baud rate |
  | MSS_UART_600_BAUD    |    600 baud rate |
  | MSS_UART_1200_BAUD   |   1200 baud rate |
  | MSS_UART_2400_BAUD   |   2400 baud rate |
  | MSS_UART_4800_BAUD   |   4800 baud rate |
  | MSS_UART_9600_BAUD   |   9600 baud rate |
  | MSS_UART_19200_BAUD  |  19200 baud rate |
  | MSS_UART_38400_BAUD  |  38400 baud rate |
  | MSS_UART_57600_BAUD  |  57600 baud rate |
  | MSS_UART_115200_BAUD | 115200 baud rate |
  | MSS_UART_230400_BAUD | 230400 baud rate |
  | MSS_UART_460800_BAUD | 460800 baud rate |
  | MSS_UART_921600_BAUD | 921600 baud rate |

 */
#define MSS_UART_110_BAUD       110U
#define MSS_UART_300_BAUD       300U
#define MSS_UART_600_BAUD       600U
#define MSS_UART_1200_BAUD      1200U
#define MSS_UART_2400_BAUD      2400U
#define MSS_UART_4800_BAUD      4800U
#define MSS_UART_9600_BAUD      9600U
#define MSS_UART_19200_BAUD     19200U
#define MSS_UART_38400_BAUD     38400U
#define MSS_UART_57600_BAUD     57600U
#define MSS_UART_115200_BAUD    115200U
#define MSS_UART_230400_BAUD    230400U
#define MSS_UART_460800_BAUD    460800U
#define MSS_UART_921600_BAUD    921600U

/***************************************************************************//**
  Data Bits Length
  ================
  The following defines are used to build the value of the MSS_UART_init(),
  MSS_UART_lin_init(), MSS_UART_irda_init(), and MSS_UART_smartcard_init()
  functions line_config parameter.

  | Constant             | Description                |
  |----------------------|----------------------------|
  | MSS_UART_DATA_5_BITS | 5 bits of data transmitted |
  | MSS_UART_DATA_6_BITS | 6 bits of data transmitted |
  | MSS_UART_DATA_7_BITS | 7 bits of data transmitted |
  | MSS_UART_DATA_8_BITS | 8 bits of data transmitted |

 */
#define MSS_UART_DATA_5_BITS     ((uint8_t) 0x00)
#define MSS_UART_DATA_6_BITS     ((uint8_t) 0x01)
#define MSS_UART_DATA_7_BITS     ((uint8_t) 0x02)
#define MSS_UART_DATA_8_BITS     ((uint8_t) 0x03)

/***************************************************************************//**
  Parity
  ======
  The following defines are used to build the value of the MSS_UART_init(),
  MSS_UART_lin_init(), MSS_UART_irda_init(), and MSS_UART_smartcard_init()
  functions line_config parameter.

  | Constant                | Description              |
  |-------------------------|--------------------------|
  | MSS_UART_NO_PARITY      | No parity                |
  | MSS_UART_ODD_PARITY     | Odd Parity               |
  | MSS_UART_EVEN_PARITY    | Even parity              |
  | MSS_UART_STICK_PARITY_0 | Stick parity bit to zero |
  | MSS_UART_STICK_PARITY_1 | Stick parity bit to one  |

 */
#define MSS_UART_NO_PARITY           ((uint8_t) 0x00)
#define MSS_UART_ODD_PARITY          ((uint8_t) 0x08)
#define MSS_UART_EVEN_PARITY         ((uint8_t) 0x18)
#define MSS_UART_STICK_PARITY_0      ((uint8_t) 0x38)
#define MSS_UART_STICK_PARITY_1      ((uint8_t) 0x28)

/***************************************************************************//**
  Number of Stop Bits
  ===================
  The following defines are used to build the value of the MSS_UART_init(),
  MSS_UART_lin_init(), MSS_UART_irda_init(), and MSS_UART_smartcard_init()
  functions line_config parameter.

  | Constant                  | Description              |
  |---------------------------|--------------------------|
  | MSS_UART_ONE_STOP_BIT     | One Stop bit             |
  | MSS_UART_ONEHALF_STOP_BIT | One and a half Stop bits |
  | MSS_UART_TWO_STOP_BITS    | Two Stop bits            |

 */
#define MSS_UART_ONE_STOP_BIT        ((uint8_t) 0x00)
#define MSS_UART_ONEHALF_STOP_BIT    ((uint8_t) 0x04)
#define MSS_UART_TWO_STOP_BITS       ((uint8_t) 0x04)

/***************************************************************************//**
  Receiver Error Status
  =====================
  The following defines are used to determine the UART receiver error type.
  These bit mask constants are used with the return value of the
  MSS_UART_get_rx_status() function to find out if any errors occurred while
  receiving data.


  | Constant               | Description                                |
  |------------------------|--------------------------------------------|
  | MSS_UART_NO_ERROR      | No error bit mask (0x00)                   |
  | MSS_UART_OVERUN_ERROR  | Overrun error bit mask (0x02)              |
  | MSS_UART_PARITY_ERROR  | Parity error bit mask (0x04)               |
  | MSS_UART_FRAMING_ERROR | Framing error bit mask (0x08)              |
  | MSS_UART_BREAK_ERROR   | Break error bit mask (0x10)                |
  | MSS_UART_FIFO_ERROR    | FIFO error bit mask (0x80)                 |
  | MSS_UART_INVALID_PARAM | Invalid function parameter bit mask (0xFF) |

 */
#define MSS_UART_INVALID_PARAM    ((uint8_t)0xFF)
#define MSS_UART_NO_ERROR         ((uint8_t)0x00 )
#define MSS_UART_OVERUN_ERROR     ((uint8_t)0x02)
#define MSS_UART_PARITY_ERROR     ((uint8_t)0x04)
#define MSS_UART_FRAMING_ERROR    ((uint8_t)0x08)
#define MSS_UART_BREAK_ERROR      ((uint8_t)0x10)
#define MSS_UART_FIFO_ERROR       ((uint8_t)0x80)

/***************************************************************************//**
  Transmitter Status
  ==================
  The following definitions are used to determine the UART transmitter status.
  These bit mask constants are used with the return value of the
  MSS_UART_get_tx_status() function to find out the status of the transmitter.

  | Constant         | Description                                        |
  |------------------|----------------------------------------------------|
  | MSS_UART_TX_BUSY | Transmitter busy (0x00)                            |
  | MSS_UART_THRE    | Transmitter holding register empty bit mask (0x20) |
  | MSS_UART_TEMT    | Transmitter empty bit mask (0x40)                  |

 */
#define MSS_UART_TX_BUSY          ((uint8_t) 0x00)
#define MSS_UART_THRE             ((uint8_t) 0x20)
#define MSS_UART_TEMT             ((uint8_t) 0x40)

/***************************************************************************//**
  Modem Status
  ============
  The following defines are used to determine the modem status. These bit
  mask constants are used with the return value of the
  MSS_UART_get_modem_status() function to find out the modem status of
  the UART.

  | Constant      | Description                                     |
  |---------------|-------------------------------------------------|
  | MSS_UART_DCTS | Delta clear to send bit mask (0x01)             |
  | MSS_UART_DDSR | Delta data set ready bit mask (0x02)            |
  | MSS_UART_TERI | Trailing edge of ring indicator bit mask (0x04) |
  | MSS_UART_DDCD | Delta data carrier detect bit mask (0x08)       |
  | MSS_UART_CTS  | Clear to send bit mask (0x10)                   |
  | MSS_UART_DSR  | Data set ready bit mask (0x20)                  |
  | MSS_UART_RI   | Ring indicator bit mask (0x40)                  |
  | MSS_UART_DCD  | Data carrier detect bit mask (0x80)             |

 */
#define MSS_UART_DCTS             ((uint8_t) 0x01)
#define MSS_UART_DDSR             ((uint8_t) 0x02)
#define MSS_UART_TERI             ((uint8_t) 0x04)
#define MSS_UART_DDCD             ((uint8_t) 0x08)
#define MSS_UART_CTS              ((uint8_t) 0x10)
#define MSS_UART_DSR              ((uint8_t) 0x20)
#define MSS_UART_RI               ((uint8_t) 0x40)
#define MSS_UART_DCD              ((uint8_t) 0x80)

/***************************************************************************//**
  This typedef specifies the irq_mask parameter for the MSS_UART_enable_irq()
  and MSS_UART_disable_irq() functions. The driver defines a set of bit masks
  that are used to build the value of the irq_mask parameter. A bitwise OR of
  these bit masks is used to enable or disable multiple MSS MMUART interrupts.
 */
typedef uint16_t mss_uart_irq_t;

/***************************************************************************//**
  MSS MMUART Interrupts
  =====================
  The following defines specify the interrupt masks to enable and disable MSS
  MMUART interrupts. They are used to build the value of the irq_mask parameter
  for the MSS_UART_enable_irq() and MSS_UART_disable_irq() functions. A bitwise
  OR of these constants is used to enable or disable multiple interrupts.


  | Constant           | Description                                                   |
  |--------------------|---------------------------------------------------------------|
  | MSS_UART_RBF_IRQ   | Receive Data Available Interrupt bit mask (0x001)             |
  | MSS_UART_TBE_IRQ   | Transmitter Holding Register Empty interrupt bit mask (0x002) |
  | MSS_UART_LS_IRQ    | Receiver Line Status interrupt bit mask (0x004)               |
  | MSS_UART_MS_IRQ    | Modem Status interrupt bit mask (0x008)                       |
  | MSS_UART_RTO_IRQ   | Receiver time-out interrupt bit mask (0x010)                  |
  | MSS_UART_NACK_IRQ  | NACK / ERR signal interrupt bit mask (0x020)                  |
  | MSS_UART_PIDPE_IRQ | PID parity error interrupt bit mask (0x040)                   |
  | MSS_UART_LINB_IRQ  | LIN break detection interrupt bit mask (0x080)                |
  | MSS_UART_LINS_IRQ  | LIN Sync detection interrupt bit mask (0x100)                 |

 */
#define MSS_UART_RBF_IRQ        0x001
#define MSS_UART_TBE_IRQ        0x002
#define MSS_UART_LS_IRQ         0x004
#define MSS_UART_MS_IRQ         0x008
#define MSS_UART_RTO_IRQ        0x010
#define MSS_UART_NACK_IRQ       0x020
#define MSS_UART_PIDPE_IRQ      0x040
#define MSS_UART_LINB_IRQ       0x080
#define MSS_UART_LINS_IRQ       0x100
#define MSS_UART_INVALID_IRQ    UINT16_MAX

/***************************************************************************//**
  This enumeration specifies the receiver FIFO trigger level. This is the number
  of bytes that must be received before the UART generates a receive data
  available interrupt. It provides the allowed values for the
  MSS_UART_set_rx_handler() function trigger_level parameter.
 */
typedef enum {
    MSS_UART_FIFO_SINGLE_BYTE    = 0x00,
    MSS_UART_FIFO_FOUR_BYTES     = 0x40,
    MSS_UART_FIFO_EIGHT_BYTES    = 0x80,
    MSS_UART_FIFO_FOURTEEN_BYTES = 0xC0,
    MSS_UART_FIFO_INVALID_TRIG_LEVEL

} mss_uart_rx_trig_level_t;

/***************************************************************************//**
  This enumeration specifies the loopback configuration of the UART. It provides
  the allowed values for the MSS_UART_set_loopback() function's loopback
  parameter. Use MSS_UART_LOCAL_LOOPBACK_ON to set up the UART to locally
  loopback its Tx and Rx lines. Use MSS_UART_REMOTE_LOOPBACK_ON to set up the
  UART in remote loopback mode.
 */
typedef enum {
    MSS_UART_LOCAL_LOOPBACK_OFF,
    MSS_UART_LOCAL_LOOPBACK_ON,
    MSS_UART_REMOTE_LOOPBACK_OFF,
    MSS_UART_REMOTE_LOOPBACK_ON,
    MSS_UART_AUTO_ECHO_OFF,
    MSS_UART_AUTO_ECHO_ON,
    MSS_UART_INVALID_LOOPBACK

} mss_uart_loopback_t;

/***************************************************************************//**
  IrDA input / output polarity.
  This enumeration specifies the RZI modem polarity for input and output signals.
  This is passed as parameters in MSS_UART_irda_init() function.
 */
typedef enum {
    MSS_UART_ACTIVE_LOW = 0u,
    MSS_UART_ACTIVE_HIGH = 1u,
    MSS_UART_INVALID_POLARITY

} mss_uart_rzi_polarity_t;

/***************************************************************************//**
  IrDA input / output pulse width.
  This enumeration specifies the RZI modem pulse width for input and output
  signals. This is passed as parameters in MSS_UART_irda_init() function.
 */
typedef enum {
    MSS_UART_3_BY_16 = 0u,
    MSS_UART_1_BY_4 = 1u,
    MSS_UART_INVALID_PW

} mss_uart_rzi_pulsewidth_t;

/***************************************************************************//**
  Tx / Rx endianess.
  This enumeration specifies the MSB first or LSB first for MSS UART transmitter
  and receiver. The parameter of this type shall be passed in
  MSS_UART_set_rx_endian()and MSS_UART_set_tx_endian() functions.
 */
typedef enum {
    MSS_UART_LITTLEEND,
    MSS_UART_BIGEND,
    MSS_UART_INVALID_ENDIAN

} mss_uart_endian_t;

/***************************************************************************//**
  Glitch filter length.
  This enumeration specifies the glitch filter length. The function
  MSS_UART_set_filter_length() accepts the parameter of this type.
 */
typedef enum {
    MSS_UART_LEN0 = 0,
    MSS_UART_LEN1 = 1,
    MSS_UART_LEN2 = 2,
    MSS_UART_LEN3 = 3,
    MSS_UART_LEN4 = 4,
    MSS_UART_LEN5 = 5,
    MSS_UART_LEN6 = 6,
    MSS_UART_LEN7 = 7,
    MSS_UART_INVALID_FILTER_LENGTH = 8

} mss_uart_filter_length_t;

/***************************************************************************//**
  TXRDY and RXRDY mode.
  This enumeration specifies the TXRDY and RXRDY signal modes. The function
  MSS_UART_set_ready_mode() accepts the parameter of this type.
 */
typedef enum {
    MSS_UART_READY_MODE0,
    MSS_UART_READY_MODE1,
    MSS_UART_INVALID_READY_MODE

} mss_uart_ready_mode_t;

/***************************************************************************//**
  USART mode of operation.
  This enumeration specifies the mode of operation of MSS UART when operating
  as USART. The function MSS_UART_set_usart_mode() accepts the parameter of this
  type.
 */
typedef enum {
    MSS_UART_ASYNC_MODE               = 0,
    MSS_UART_SYNC_SLAVE_POS_EDGE_CLK  = 1,
    MSS_UART_SYNC_SLAVE_NEG_EDGE_CLK  = 2,
    MSS_UART_SYNC_MASTER_POS_EDGE_CLK = 3,
    MSS_UART_SYNC_MASTER_NEG_EDGE_CLK = 4,
    MSS_UART_INVALID_SYNC_MODE        = 5

} mss_uart_usart_mode_t;


typedef enum {
    MSS_UART0_LO    = 0,
    MSS_UART1_LO    = 1,
    MSS_UART2_LO    = 2,
    MSS_UART3_LO    = 3,
    MSS_UART4_LO    = 4,
    MSS_UART0_HI    = 5,
    MSS_UART1_HI    = 6,
    MSS_UART2_HI    = 7,
    MSS_UART3_HI    = 8,
    MSS_UAR4_HI     = 9,

} mss_uart_num_t;

/***************************************************************************//**
  This is the type definition for MSS UART instance. You need to create and
  maintain a record of this type. This holds all data regarding the MSS UART
  instance.
 */
typedef struct  mss_uart_instance mss_uart_instance_t;

/***************************************************************************//**
  This typedef specifies the function prototype for MSS UART interrupt handlers.
  All interrupt handlers registered with the MSS UART driver must be of this type.
  The interrupt handlers are registered with the driver through the
  MSS_UART_set_rx_handler(), MSS_UART_set_tx_handler(),
  MSS_UART_set_rxstatus_handler(), and MSS_UART_set_modemstatus_handler()
  functions.    
  The this_uart parameter is a pointer to the MMUART instance to
  associate with the handler function.
 */
typedef void (*mss_uart_irq_handler_t)( mss_uart_instance_t * this_uart );

/*----------------------------------------------------------------------------*/
/*----------------------------------- UART -----------------------------------*/
/*----------------------------------------------------------------------------*/

typedef struct
{
    union
    {
        volatile const uint8_t    RBR;
        volatile uint8_t    THR;
        volatile uint8_t    DLR;
        uint32_t RESERVED0;
    };

    union
    {
        volatile uint8_t  DMR;
        volatile uint8_t  IER;
        uint32_t RESERVED1;
    };

    union
    {
        volatile uint8_t  IIR;
        volatile uint8_t  FCR;
        uint32_t RESERVED2;
    };

    volatile uint8_t  LCR;
    uint8_t  RESERVED3[3];

    volatile uint8_t  MCR;
    uint8_t  RESERVED4[3];

    volatile const  uint8_t  LSR;
    uint8_t  RESERVED5[3];

    volatile const  uint8_t  MSR;
    uint8_t  RESERVED6[3];

    volatile uint8_t  SR;
    uint8_t  RESERVED7[7];

    volatile uint8_t  IEM;
    uint8_t  RESERVED8[3];

    volatile uint8_t  IIM;
    uint8_t  RESERVED9[7];

    volatile uint8_t  MM0;
    uint8_t  RESERVED10[3];

    volatile uint8_t  MM1;
    uint8_t  RESERVED11[3];

    volatile uint8_t  MM2;
    uint8_t  RESERVED12[3];

    volatile uint8_t  DFR;
    uint8_t  RESERVED13[7];

    volatile uint8_t GFR;
    uint8_t  RESERVED14[3];

    volatile uint8_t TTG;
    uint8_t  RESERVED15[3];

    volatile uint8_t RTO;
    uint8_t  RESERVED16[3];

    volatile uint8_t ADR;
    uint8_t  RESERVED17[3];

} MSS_UART_TypeDef;


/***************************************************************************//**
  mss_uart_instance.
  There is one instance of this structure for each instance of the
  microprocessor subsystem's UARTs. Instances of this structure are used to
  identify a specific UART. A pointer to an initialized instance of the
  mss_uart_instance_t structure is passed as the first parameter to
  MSS UART driver functions to identify which UART should perform the
  requested operation.
 */
struct mss_uart_instance{
    /* CMSIS related defines identifying the UART hardware. */
    MSS_UART_TypeDef *      hw_reg;     /*!< Pointer to UART registers. */
    uint32_t                baudrate;   /*!< Operating baud rate. */
    uint8_t                 lineconfig; /*!< Line configuration parameters. */
    uint8_t                 status;     /*!< Sticky line status. */

    /* Padding bytes for data alignment. */
    uint8_t padding0[2];                /*!< Padding bytes for data alignment. */

    /* transmit related info (used with interrupt driven transmit): */
    const uint8_t * tx_buffer;          /*!< Pointer to transmit buffer. */
    uint32_t        tx_buff_size;       /*!< Transmit buffer size. */
    uint32_t        tx_idx;             /*!< Index within transmit buffer of next byte to transmit.*/

    /* line status interrupt handler:*/
    mss_uart_irq_handler_t linests_handler;   /*!< Pointer to user registered line status handler. */
    /* receive interrupt handler:*/
    mss_uart_irq_handler_t rx_handler;        /*!< Pointer to user registered receiver handler. */
    /* transmit interrupt handler:*/
    mss_uart_irq_handler_t tx_handler;        /*!< Pointer to user registered transmit handler. */
    /* modem status interrupt handler:*/
    mss_uart_irq_handler_t modemsts_handler;  /*!< Pointer to user registered modem status handler. */
    /* receiver timeout interrupt handler */
    mss_uart_irq_handler_t rto_handler;       /*!< Pointer to user registered receiver timeout handler. */
    /* NACK interrupt handler */
    mss_uart_irq_handler_t nack_handler;      /*!< Pointer to user registered NACK handler. */
    /* PID parity perror interrupt handler */
    mss_uart_irq_handler_t pid_pei_handler;   /*!< Pointer to user registered PID parity error handler. */
    /* LIN break interrupt handler */
    mss_uart_irq_handler_t break_handler;     /*!< Pointer to user registered LIN break handler. */
    /* LIN sync detection interrupt handler */
    mss_uart_irq_handler_t sync_handler;      /*!< Pointer to user registered LIN sync detection handler. */
    uint8_t                local_irq_enabled;  /*!< Check if local interrupt were enabled on this instance. */
    uint8_t padding1[7];                      /*!< Padding bytes for data alignment. */
    void* user_data;                          /*!< Pointer to user provided pointer for user specific use. */

};

/***************************************************************************//**
  This instance of mss_uart_instance_t holds all data related to the operations
  performed by the MMUART. The function MSS_UART_init() initializes this structure.
  A pointer to g_mss_uart0_lo is passed as the first parameter to MSS UART driver
  functions to indicate that MMUART0 should perform the requested operation.
 */

extern mss_uart_instance_t g_mss_uart0_lo;
extern mss_uart_instance_t g_mss_uart1_lo;
extern mss_uart_instance_t g_mss_uart2_lo;
extern mss_uart_instance_t g_mss_uart3_lo;
extern mss_uart_instance_t g_mss_uart4_lo;

extern mss_uart_instance_t g_mss_uart0_hi;
extern mss_uart_instance_t g_mss_uart1_hi;
extern mss_uart_instance_t g_mss_uart2_hi;
extern mss_uart_instance_t g_mss_uart3_hi;
extern mss_uart_instance_t g_mss_uart4_hi;


/***************************************************************************//**
  The MSS_UART_init() function initializes and configures one of the PolarFire®
  SoC MSS UARTs with the configuration passed as parameters. The configuration
  parameters are the baud_rate, which generates the baud value, and 
  the line_config, which specifies the line configuration (bit length,
  Stop bits, and parity).
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
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
    The line_config parameter is the line configuration that specifies the bit
    length, number of Stop bits, and parity settings.
    This is a bitwise OR of one value from each of the following groups of
    allowed values:
    - One of the following to specify the transmit/receive data bit length:
      - MSS_UART_DATA_5_BITS
      - MSS_UART_DATA_6_BITS
      - MSS_UART_DATA_7_BITS
      - MSS_UART_DATA_8_BITS
    - One of the following to specify the parity setting:
      - MSS_UART_EVEN_PARITY
      - MSS_UART_NO_PARITY
      - MSS_UART_ODD_PARITY
      - MSS_UART_STICK_PARITY_0
      - MSS_UART_STICK_PARITY_1
    - One of the following to specify the number of Stop bits:
      - MSS_UART_ONE_STOP_BIT
      - MSS_UART_ONEHALF_STOP_BIT
      - MSS_UART_TWO_STOP_BITS

  @return
    This function does not return a value.

  @example
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
void
MSS_UART_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
);

/***************************************************************************//**
 The MSS_UART_lin_init() function initializes the MSS UART for
 LIN mode of operation. The configuration parameters are the baud_rate, which is
 used to generate the baud value, and the line_config, which specifies
 the line configuration (bit length, Stop bits and parity).
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
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
    The line_config parameter is the line configuration that specifies the bit
    length, number of Stop bits, and parity settings.
    This is a bitwise OR of one value from each of the following groups of
    allowed values:
    - One of the following to specify the transmit/receive data bit length:
      - MSS_UART_DATA_5_BITS
      - MSS_UART_DATA_6_BITS
      - MSS_UART_DATA_7_BITS
      - MSS_UART_DATA_8_BITS
    - One of the following to specify the parity setting:
      - MSS_UART_EVEN_PARITY
      - MSS_UART_NO_PARITY
      - MSS_UART_ODD_PARITY
      - MSS_UART_STICK_PARITY_0
      - MSS_UART_STICK_PARITY_1
    - One of the following to specify the number of Stop bits:
      - MSS_UART_ONE_STOP_BIT
      - MSS_UART_ONEHALF_STOP_BIT
      - MSS_UART_TWO_STOP_BITS

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     MSS_UART_lin_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     return(0);
  }
  @endcode
 */
void
MSS_UART_lin_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
);

/***************************************************************************//**
  The MSS_UART_irda_init() function initializes the MSS UART instance
  referenced by the this_uart parameter  for IrDA mode of operation. This
  function must be called before calling any other IrDA specific functions.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
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
    The line_config parameter is the line configuration that specifies the bit
    length, number of Stop bits, and parity settings.
    This is a bitwise OR of one value from each of the following groups of
    allowed values:
    - One of the following to specify the transmit/receive data bit length:
      - MSS_UART_DATA_5_BITS
      - MSS_UART_DATA_6_BITS
      - MSS_UART_DATA_7_BITS
      - MSS_UART_DATA_8_BITS
    - One of the following to specify the parity setting:
      - MSS_UART_EVEN_PARITY
      - MSS_UART_NO_PARITY
      - MSS_UART_ODD_PARITY
      - MSS_UART_STICK_PARITY_0
      - MSS_UART_STICK_PARITY_1
    - One of the following to specify the number of Stop bits:
      - MSS_UART_ONE_STOP_BIT
      - MSS_UART_ONEHALF_STOP_BIT
      - MSS_UART_TWO_STOP_BITS

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_irda_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT,
              MSS_UART_ACTIVE_LOW,
              MSS_UART_ACTIVE_LOW,
              MSS_UART_3_BY_16);
  @endcode
 */
void
MSS_UART_irda_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config,
    mss_uart_rzi_polarity_t rxpol,
    mss_uart_rzi_polarity_t txpol,
    mss_uart_rzi_pulsewidth_t pw
);

/***************************************************************************//**
  The MSS_UART_smartcard_init() function initializes the MSS UART
  for ISO 7816 (smartcard) mode of operation. The configuration parameters are
  the baud_rate, which generates the baud value, and the line_config,
  which specifies the line configuration (bit length, Stop bits and
  parity). This function disables all other modes of the MSS UART instance
  pointed by the this_uart parameter.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
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
    The line_config parameter is the line configuration that specifies the bit
    length, number of Stop bits, and parity settings.
    This is a bitwise OR of one value from each of the following groups of
    allowed values:
    - One of the following to specify the transmit/receive data bit length:
      - MSS_UART_DATA_5_BITS
      - MSS_UART_DATA_6_BITS
      - MSS_UART_DATA_7_BITS
      - MSS_UART_DATA_8_BITS
    - One of the following to specify the parity setting:
      - MSS_UART_EVEN_PARITY
      - MSS_UART_NO_PARITY
      - MSS_UART_ODD_PARITY
      - MSS_UART_STICK_PARITY_0
      - MSS_UART_STICK_PARITY_1
    - One of the following to specify the number of Stop bits:
      - MSS_UART_ONE_STOP_BIT
      - MSS_UART_ONEHALF_STOP_BIT
      - MSS_UART_TWO_STOP_BITS

  @return
    This function does not return a value.

  @example
  @code
    #include "mss_uart.h"

      int main(void)
      {
         MSS_UART_smartcard_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

         return(0);
      }
  @endcode
 */
void
MSS_UART_smartcard_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
);

/***************************************************************************//**
  The function MSS_UART_polled_tx() transmits data. It transfers the
  content of the transmitter data buffer, passed as a function parameter, into
  the UART's hardware transmitter FIFO. It returns when the full content of the
  transmit data buffer is transferred to the UART's transmit FIFO. It is
  safe to release or reuse the memory used as the transmitter data buffer once
  this function returns.

  Note:     This function reads the UART's line status register (LSR) to poll
  for the active state of the transmitter holding register empty (THRE) bit
  before transferring data from the data buffer to the transmitter FIFO. It
  transfers data to the transmitter FIFO in blocks of 16 bytes or less and
  allows the FIFO to empty before transferring the next block of data.

  Note:     The actual transmission over the serial connection is still
  in progress when this function returns. Use the MSS_UART_get_tx_status()
  function if you need to know when the transmitter is empty.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param pbuff
    The pbuff parameter is a pointer to a buffer containing the data to
    transmit.

  @param tx_size
    The tx_size parameter specifies the size, in bytes, of the data to
    transmit.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t message[12] = "Hello World";

     MSS_UART_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              SS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_polled_tx(&g_mss_uart0_lo, message, sizeof(message));

     return(0);
  }
  @endcode
 */
void
MSS_UART_polled_tx
(
    mss_uart_instance_t * this_uart,
    const uint8_t * pbuff,
    uint32_t tx_size
);

/***************************************************************************//**
  The function MSS_UART_polled_tx_string() transmits a NULL ('\0')
  terminated string. It transfers the text string, located at
  the address pointed to by p_sz_string, to the UART's hardware transmitter
  FIFO. It returns when the complete string is transferred to the UART's
  transmit FIFO. It is safe to release or reuse the memory used as the string
  buffer once this function returns.

  Note:     This function reads the UART's line status register (LSR) to poll
  for the active state of the transmitter holding register empty (THRE) bit
  before transferring data from the data buffer to the transmitter FIFO. It
  transfers data to the transmitter FIFO in blocks of 16 bytes or less and
  allows the FIFO to empty before transferring the next block of data.

  Note:     The actual transmission over the serial connection is still
  in progress when this function returns. Use the MSS_UART_get_tx_status()
  function if you need to know when the transmitter is empty.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param p_sz_string
    The p_sz_string parameter is a pointer to a buffer containing the NULL 
    ('\0') terminated string to transmit.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t message[12] = "Hello World";

     MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_polled_tx_string(&g_mss_uart0_lo, message);

     return(0);
  }
  @endcode

 */
void
MSS_UART_polled_tx_string
(
    mss_uart_instance_t * this_uart,
    const uint8_t * p_sz_string
);

/***************************************************************************//**
  The function MSS_UART_irq_tx() initiates an interrupt-driven
  transmit. It returns immediately after passing the transmit buffer location
  to the MMUART instance and enabling transmit interrupts both at the UART and 
  the PolarFire® SoC Core Complex PLIC level. This function takes a pointer, 
  the pbuff parameter, to a memory buffer containing the data to transmit. The 
  memory buffer specified through this pointer must remain allocated and contain
  the data to transmit until the transmit completion is detected through calling
  the MSS_UART_tx_complete() function. The actual transmission over the serial
  connection is still in progress until calls to the MSS_UART_tx_complete()
  function indicate transmit completion.

  Note:     The MSS_UART_irq_tx() function enables both the transmit holding
  register empty (THRE) interrupt in the UART and the MSS UART instance
  interrupt in the PolarFire® SoC Core Complex PLIC as part of its implementation.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param pbuff
    The pbuff parameter is a pointer to a buffer containing the data
    to transmit.

  @param tx_size
    The tx_size parameter specifies the size, in bytes, of the data
    to transmit.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";

     MSS_UART_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_irq_tx(&g_mss_uart0_lo, tx_buff, sizeof(tx_buff));

     while(0 == MSS_UART_tx_complete(&g_mss_uart0_lo))
     {
         ;
     }
     return(0);
  }
  @endcode
 */
void
MSS_UART_irq_tx
(
    mss_uart_instance_t * this_uart,
    const uint8_t * pbuff,
    uint32_t tx_size
);

/***************************************************************************//**
  The MSS_UART_tx_complete() function is used to find out if the
  interrupt-driven transmit previously initiated through a call to
  MSS_UART_irq_tx() is complete. This is typically used to find out when it is
  safe to reuse or release the memory buffer holding transmit data.

  Note:     The transfer of all of the data from the memory buffer to the UART's
  transmit FIFO and the actual transmission over the serial connection are both
  complete when a call to the MSS_UART_tx_complete() function indicates transmit
  completion.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function return a non-zero value if transmit is complete, it returns
    zero otherwise.

  @example
    See the MSS_UART_irq_tx() function for an example that uses the
    MSS_UART_tx_complete() function.

 */
int8_t
MSS_UART_tx_complete
(
   mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_get_rx() function reads the content of the UART receiver's FIFO
  and stores it in the receive buffer that is passed using the rx_buff
  parameter. It copies either the full content of the FIFO into the receive
  buffer, or just enough data from the FIFO to fill the receive buffer,
  depending on the size of the receive buffer passed by the buff_size
  parameter. The MSS_UART_get_rx() function returns the number of bytes copied
  into the receive buffer. This function is non-blocking and returns 0
  immediately if no data is received.

  Note:     The MSS_UART_get_rx() function reads and accumulates the receiver
  status of the MSS UART instance before reading each byte from the receiver's
  data register/FIFO. This allows the driver to maintain a sticky record of any
  receiver errors that occur as the UART receives each data byte; receiver
  errors would otherwise be lost after each read from the receiver's data
  register. Calling the MSS_UART_get_rx_status() function returns any receiver
  errors accumulated during the execution of the MSS_UART_get_rx() function.

  Note:     If you need to read the error status for each byte received, set
  the buff_size to 1 and read the receive line error status for each byte
  using the MSS_UART_get_rx_status() function.

  The MSS_UART_get_rx() function can be used in polled mode, where it is called
  at regular intervals to find out if any data is received, or in
  interrupt driven-mode, where it is called as part of a receive handler that is
  called by the driver as a result of data being received.

  Note:     In interrupt driven mode, call the MSS_UART_get_rx()
  function as part of the receive handler function that you register with
  the MSS UART driver through calling MSS_UART_set_rx_handler().
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.
  @param rx_buff
    The rx_buff parameter is a pointer to a buffer where the received
    data is copied.

  @param buff_size
    The buff_size parameter specifies the size of the receive buffer in bytes.

  @return
    This function returns the number of bytes that were copied into the
    rx_buff buffer. It returns 0 if no data is received.

  Polled mode example:
  @example
  @code
   int main( void )
   {
      uint8_t rx_buff[RX_BUFF_SIZE];
      uint32_t rx_idx  = 0;

      MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

      while(1)
      {
          rx_size = MSS_UART_get_rx(&g_mss_uart0_lo, rx_buff, sizeof(rx_buff));
          if(rx_size > 0)
          {
              process_rx_data(rx_buff, rx_size);
          }
          task_a();
          task_b();
      }
      return 0;
   }
  @endcode

  Interrupt driven example:
  @example
  @code
   int main( void )
   {
      MSS_UART_init(&g_mss_uart1,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

      MSS_UART_set_rx_handler(&g_mss_uart1,
                              uart1_rx_handler,
                              MSS_UART_FIFO_SINGLE_BYTE);

      while(1)
      {
          task_a();
          task_b();
      }
      return 0;
   }

   void uart1_rx_handler(mss_uart_instance_t * this_uart)
   {
      uint8_t rx_buff[RX_BUFF_SIZE];
      uint32_t rx_idx  = 0;
      rx_size = MSS_UART_get_rx(this_uart, rx_buff, sizeof(rx_buff));
      process_rx_data(rx_buff, rx_size);
   }
  @endcode
 */
size_t
MSS_UART_get_rx
(
   mss_uart_instance_t * this_uart,
   uint8_t * rx_buff,
   size_t buff_size
);

/***************************************************************************//**
  The MSS_UART_set_rx_handler() function registers a receive handler
  function that is called by the driver when a UART receive data available (RDA)
  interrupt occurs. You must create and register the receive handler function
  to suit your application and it must call the MSS_UART_get_rx()
  function to actually read the received data.

  Note:     The MSS_UART_set_rx_handler() function enables both the RDA
  interrupt in the MSS UART instance and in the corresponding
  MSS UART instance interrupt in the PolarFire® SoC Core Complex PLIC  as part
  of its implementation.

  Note:     You can disable the RDA interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and is dependent upon
  your application.

  Note:     The trigger level is actually applied only if the this_uart is set
  to ready mode 1. For more information, see the MSS_UART_set_ready_mode()
  function.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param handler
    The handler parameter is a pointer to a receive interrupt handler function
    provided by your application that is called when a UART RDA interrupt
    is triggered. This handler function must be of type mss_uart_irq_handler_t.

  @param trigger_level
    The trigger_level parameter is the receive FIFO trigger level. This
    specifies the number of bytes that must be received before the UART
    triggers an RDA interrupt.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  #define RX_BUFF_SIZE    64

  uint8_t g_rx_buff[RX_BUFF_SIZE];

  void uart0_rx_handler(mss_uart_instance_t * this_uart)
  {
      MSS_UART_get_rx(this_uart, &g_rx_buff[g_rx_idx], sizeof(g_rx_buff));
  }

  int main(void)
  {
      MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

      MSS_UART_set_rx_handler(&g_mss_uart0_lo,
                              uart0_rx_handler,
                              MSS_UART_FIFO_SINGLE_BYTE);

      while(1)
      {
         ;
      }
      return(0);
   }
  @endcode
 */
void
MSS_UART_set_rx_handler
(
    mss_uart_instance_t *       this_uart,
    mss_uart_irq_handler_t          handler,
    mss_uart_rx_trig_level_t    trigger_level
);

/***************************************************************************//**
  The MSS_UART_set_loopback() function is used to locally loop-back the Tx and
  Rx lines of a UART. This is not to be confused with the loop-back of UART0
  to UART1, which can be achieved through the microprocessor subsystem's
  system registers.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param loopback
    The loopback parameter indicates whether or not the UART's transmit and
    receive lines should be looped back. Following are the allowed values:
      - MSS_UART_LOCAL_LOOPBACK_ON
      - MSS_UART_LOCAL_LOOPBACK_OFF
      - MSS_UART_REMOTE_LOOPBACK_ON
      - MSS_UART_REMOTE_LOOPBACK_OFF
      - MSS_UART_AUTO_ECHO_ON
      - MSS_UART_AUTO_ECHO_OFF

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

      MSS_UART_set_loopback(&g_mss_uart0_lo, MSS_UART_LOCAL_LOOPBACK_OFF);
  @endcode
 */
void
MSS_UART_set_loopback
(
    mss_uart_instance_t *   this_uart,
    mss_uart_loopback_t     loopback
);

/***************************************************************************//**
  The MSS_UART_enable_irq() function enables the MSS UART interrupts specified
  by the irq_mask parameter. The irq_mask parameter identifies the MSS UART
  interrupts by bit position, as defined in the interrupt enable register (IER)
  of MSS UART. The MSS UART interrupts and their identifying irq_mask bit
  positions are as follows: when an irq_mask bit position is set to 1, this
  function enables the corresponding MSS UART interrupt in the IER register.

  Note: the Transmit Buffer Empty interrupt is not enabled in this API. Indeed,
  enabling it here leads to an interrupt occuring before any data is passed to
  the UART, causing a crash. The TBE bit in the IER register is set
  in the MSS_UART_irq_tx() function, that actually starts the transmission.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param irq_mask
    The irq_mask parameter is used to select which of the MSS UART's interrupts
    you want to enable. The allowed value for the irq_mask parameter is one of
    the following constants or a bitwise OR of more than one:
      - MSS_UART_RBF_IRQ        (bit mask = 0x001)
      - MSS_UART_TBE_IRQ        (bit mask = 0x002)
      - MSS_UART_LS_IRQ         (bit mask = 0x004)
      - MSS_UART_MS_IRQ         (bit mask = 0x008)
      - MSS_UART_RTO_IRQ        (bit mask = 0x010)
      - MSS_UART_NACK_IRQ       (bit mask = 0x020)
      - MSS_UART_PIDPE_IRQ      (bit mask = 0x040)
      - MSS_UART_LINB_IRQ       (bit mask = 0x080)
      - MSS_UART_LINS_IRQ       (bit mask = 0x100)


  @return
     This function does not return a value.

  @example
  @code
      #include "mss_uart.h"

      int main(void)
      {
         uint8_t tx_buff[10] = "abcdefghi";
         uint32_t interrupt_priority = 4;

         enable_interrupts();
         (void) mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) 1, PERIPHERAL_ON);
         MSS_UART_init(&g_mss_uart0_lo,
                   MSS_UART_57600_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
         PLIC_init();
         MSS_UART_enable_irq(&g_mss_uart0_lo, (MSS_UART_RBF_IRQ | MSS_UART_TBE_IRQ));
         PLIC_SetPriority(MMUART0_PLIC_77, interrupt_priority);
         PLIC_SetPriority_Threshold(0);

         return(0);
      }

  @endcode
 */
void
MSS_UART_enable_irq
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_t irq_mask
);

/***************************************************************************//**
  The MSS_UART_disable_irq() function disables the MSS UART interrupts specified
  by the irq_mask parameter. The irq_mask parameter identifies the MSS UART
  interrupts by bit position, as defined in the interrupt enable register (IER)
  of MSS UART. The MSS UART interrupts and their identifying bit positions are
  as follows: when an irq_mask bit position is set to 1, this function disables
  the corresponding MSS UART interrupt in the IER register. When an irq_mask bit
  position is set to 0, the state of the corresponding interrupt remains
  unchanged in the IER register.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param irq_mask
    The irq_mask parameter is used to select which of the MSS UART's interrupts
    you want to disable. The allowed value for the irq_mask parameter is one of
    the following constants or a bitwise OR of more than one:
      - MSS_UART_RBF_IRQ    (bit mask = 0x001)
      - MSS_UART_TBE_IRQ    (bit mask = 0x002)
      - MSS_UART_LS_IRQ     (bit mask = 0x004)
      - MSS_UART_MS_IRQ     (bit mask = 0x008)
      - MSS_UART_RTO_IRQ    (bit mask = 0x010)
      - MSS_UART_NACK_IRQ   (bit mask = 0x020)
      - MSS_UART_PIDPE_IRQ  (bit mask = 0x040)
      - MSS_UART_LINB_IRQ   (bit mask = 0x080)
      - MSS_UART_LINS_IRQ   (bit mask = 0x100)

  @return
     This function does not return a value.

  @example
  @code
      #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";

     MSS_UART_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_disable_irq(&g_mss_uart0_lo,(MSS_UART_RBF_IRQ | MSS_UART_TBE_IRQ));

     return(0);
  }

  @endcode
 */
void
MSS_UART_disable_irq
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_t irq_mask
);

/***************************************************************************//**
  The MSS_UART_set_pidpei_handler() function assigns a custom 
  interrupt handler for the PIDPEI (PID parity error interrupt) when the MSS 
  UART is operating in LIN mode.

  Note:     The MSS_UART_set_pidpei_handler() function enables both the PIDPEI
  interrupt in the MSS UART instance and the corresponding
  MSS UART instance interrupt in the PolarFire® SoC Core Complex PLIC as part of
  its implementation.

  Note:     You can disable the PIDPEI interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and it depends upon
  your application.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param handler
    The handler parameter is the pointer to the custom handler function.
    This parameter is of mss_uart_irq_handler_t type.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";

     MSS_UART_init(&g_mss_uart0_lo,
            MSS_UART_57600_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_set_pidpei_handler(&g_mss_uart0_lo, my_pidpei_handler);

     return(0);
  }
  @endcode
 */
void
MSS_UART_set_pidpei_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_linbreak_handler () function assigns a custom
  interrupt handler for the LIN Break detection interrupt when the MSS UART
  is operating in LIN mode.

  Note:     The MSS_UART_set_linbreak_handler() function enables both the LIN
  BREAK interrupt in the MSS UART instance and the corresponding
  MSS UART instance interrupt in the PolarFire® SoC Core Complex PLIC as part of
  its implementation.

  Note:     You can disable the LIN BREAK interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and it depends upon
  your application.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param handler
    The handler parameter is the pointer to the custom handler function.
    This parameter is of mss_uart_irq_handler_t type.

  @return
    This function does not return a value.
  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";

     MSS_UART_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_set_linbreak_handler(&g_mss_uart0_lo, my_break_handler);

     return(0);
  }

  @endcode
 */
void
MSS_UART_set_linbreak_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_linsync_handler() function assigns a custom 
  interrupt handler for the LIN Sync character detection interrupt when the 
  MSS UART is operating in LIN mode.

  Note:     The MSS_UART_set_linsync_handler() function enables both the LIN
  SYNC interrupt in the MSS UART instance and the corresponding
  MSS UART instance interrupt in the PolarFire® SoC Core Complex PLIC as part of
  its implementation.

  Note:     You can disable the LIN SYNC interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and it depends upon
  your application.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param handler
    The handler parameter is the pointer to the custom handler function.
    This parameter is of mss_uart_irq_handler_t type.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";

     MSS_UART_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_set_linsync_handler(&g_mss_uart0_lo, my_linsync_handler);

     return(0);
  }

  @endcode
 */
void
MSS_UART_set_linsync_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_nack_handler() function assigns a custom interrupt
  handler for the NACK character detection interrupt when the MSS UART
  is operating in Smartcard mode.

  Note:     The MSS_UART_set_nack_handler() function enables both the NACK
  interrupt in the MSS UART instance and the corresponding
  MSS UART instance interrupt in the PolarFire® SoC Core Complex PLIC as part of
  its implementation.

  Note:     You can disable the NACK interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and it depends upon
  your application.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param handler
    The handler parameter is the pointer to the custom handler function.
    This parameter is of mss_uart_irq_handler_t type.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";

     MSS_UART_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_set_nack_handler(&g_mss_uart0_lo, my_nack_handler);

     return(0);
  }

  @endcode
 */
void
MSS_UART_set_nack_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_rx_timeout_handler() function assigns a custom
  interrupt handler for the receiver timeout interrupt when the MSS UART is
  operating in IrDA mode of operation.

  Note:     The MSS_UART_set_rx_timeout_handler() function enables both the
  time-out interrupt in the MSS UART instance and the corresponding
  MSS UART instance interrupt in the PolarFire® SoC Core Complex PLIC as part of
  its implementation.

  Note:     You can disable the RX time-out interrupt when required by calling
  the MSS_UART_disable_irq() function. This is your choice and it depends upon
  your application.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param handler
    The handler parameter is the pointer to the custom handler function.
    This parameter is of type mss_uart_irq_handler_t.

  @return
    This function does not return a value.

  @example
  @code
    #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";

     MSS_UART_init(&g_mss_uart0_lo,
              MSS_UART_57600_BAUD,
              MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

     MSS_UART_set_rx_timeout_handler(&g_mss_uart0_lo, my_rxtimeout_handler);

     return(0);
  }

  @endcode
 */
void
MSS_UART_set_rx_timeout_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_rxstatus_handler() function registers a receiver
  status handler function that is called by the driver when a UART receiver
  line status (RLS) interrupt occurs. You must create and register the handler
  function to suit your application.

  Note:     The MSS_UART_set_rxstatus_handler() function enables both the RLS
  interrupt in the MSS UART instance and the corresponding MSS UART
  instance interrupt in the PolarFire® SoC Core Complex PLIC as part of its
  implementation.

  Note:     You can disable the RLS interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and it depends upon
  your application.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param handler
    The handler parameter is a pointer to a receiver line status interrupt
    handler function, which is provided by your application and is called when a
    UART RLS interrupt is triggered. This handler function must be of
    mss_uart_irq_handler_t type.

  @return
    This function does not return a value.

  @example
  @code
  #include "mss_uart.h"

  void uart_rxsts_handler(mss_uart_instance_t * this_uart)
  {
    uint8_t status;
    status = MSS_UART_get_rx_status(this_uart);
    if(status & MSS_UART_OVERUN_ERROR)
    {
        discard_rx_data();
    }
  }

  int main(void)
  {
    MSS_UART_init( &g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_rxstatus_handler(&g_mss_uart0_lo, uart_rxsts_handler);

    while(1)
    {
        ;
    }
    return(0);
  }
  @endcode
 */
void
MSS_UART_set_rxstatus_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_tx_handler() function registers a transmit handler
  function that is called by the driver when a UART transmit holding register
  empty (THRE) interrupt occurs. You must create and register the transmit
  handler function to suit your application. You can then use MSS_UART_irq_tx()
  to trigger the interrupt and pass the data you want to transmit.

  Note:     If handler is NULL_HANDLER (or ((mss_uart_irq_handler_t)0)), the Tx 
  interrupt handler is the default_tx_handler().

  Note:     Your alternative THRE interrupt handler must include a call to the 
  MSS_UART_fill_tx_fifo() function to transfer the data to the UART.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

   @param handler
    The handler parameter is a pointer to a transmit interrupt handler
    function provided by your application that is called when a UART THRE
    interrupt is triggered. This handler function must be of
    mss_uart_irq_handler_t type. If this parameter is NULL_HANDLER (or 
    ((mss_uart_irq_handler_t)0)), the default_tx_handler() is assigned as
    the Tx interrupt handler.

   @return
    This function does not return a value.

   @example
   @code
    #include "mss_uart.h"

    uint8_t * g_tx_buffer = "Message from Tx irq handler";
    size_t g_tx_size = 27;

    void uart_tx_handler(mss_uart_instance_t * this_uart)
    {
        size_t size_in_fifo;
        const uint8_t * sub_buffer = &this_uart->tx_buffer[this_uart->tx_idx];
        uint32_t sub_buffer_size = this_uart->tx_buff_size - this_uart->tx_idx;

        ASSERT(( (uint8_t*)0 ) != this_uart->tx_buffer);
        ASSERT(0u < this_uart->tx_buff_size);

        size_in_fifo = MSS_UART_fill_tx_fifo(this_uart, 
                                             sub_buffer,  
                                             sub_buffer_size);
        this_uart->tx_idx += size_in_fifo;

        // Additional user code can be added here
    }

    int main(void)
    {
        uint8_t message[12] = "Hello world";

        MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

        g_tx_buffer = message;
        g_tx_size = sizeof(message);

        MSS_UART_set_tx_handler(&g_mss_uart0_lo, uart_tx_handler);
        MSS_UART_irq_tx(&g_mss_uart0_lo, g_tx_buffer, g_tx_size);

        while(1)
        {
            ;
        }
        return(0);
    }
   @endcode
 */
void
MSS_UART_set_tx_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_modemstatus_handler() function registers a modem
  status handler function that is called by the driver when a UART modem status
  (MS) interrupt occurs. You must create and register the handler function to
  suit your application.

  Note:     The MSS_UART_set_modemstatus_handler() function enables both the MS
  interrupt in the MSS UART instance and the corresponding MSS UART
  instance interrupt in the PolarFire® SoC Core Complex PLIC as part of its
  implementation.

  Note:     You can disable the MS interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and it depends
  upon your application.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

   @param handler
    The handler parameter is a pointer to a modem status interrupt handler
    function provided by your application that is called when a UART MS 
    interrupt is triggered. This handler function must be of type
    mss_uart_irq_handler_t.

   @return
    This function does not return a value.

   @example
   @code
   #include "mss_uart.h"

   void uart_modem_handler(mss_uart_instance_t * this_uart)
   {
     uint8_t status;
     status = MSS_UART_get_modem_status(this_uart);
     if(status & MSS_UART_CTS)
     {
        uart_cts_handler();
     }
   }

   int main(void)
   {
      MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

      MSS_UART_set_modemstatus_handler(&g_mss_uart0_lo, uart_modem_handler);

      while(1)
      {
        ;
      }
      return(0);
   }
   @endcode
 */

void
MSS_UART_set_modemstatus_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_fill_tx_fifo() function fills the UART's hardware transmitter
  FIFO with the data found in the transmitter buffer that is passed using the
  tx_buffer function parameter. If the transmitter FIFO is not empty when
  the function is called, the function returns immediately without transferring
  any data to the FIFO; otherwise, the function transfers data from the
  transmitter buffer to the FIFO until it is filled or until the full
  content of the transmitter buffer is copied into the FIFO. The
  function returns the number of bytes copied into the UART's transmitter FIFO.

  Note:     This function reads the UART's line status register (LSR) to check
  for the active state of the transmitter holding register empty (THRE) bit
  before transferring data from the data buffer to the transmitter FIFO. If
  THRE is 0, the function returns immediately, without transferring any data
  to the FIFO. If THRE is 1, the function transfers up to 16 bytes of data
  to the FIFO and then returns.

  Note:     The actual transmission over the serial connection will still be
  in progress when this function returns. Use the MSS_UART_get_tx_status()
  function if you need to know when the transmitter is empty.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param tx_buffer
    The tx_buffer parameter is a pointer to a buffer containing the data
    to transmit.

  @param tx_size
    The tx_size parameter is the size in bytes, of the data to be transmitted.

  @return
    This function returns the number of bytes copied into the UART's
    transmitter FIFO.

  @example
  @code
     void send_using_interrupt(uint8_t * pbuff, size_t tx_size)
     {
        size_t size_in_fifo;
        size_in_fifo = MSS_UART_fill_tx_fifo(&g_mss_uart0_lo, pbuff, tx_size);
     }
  @endcode
 */
size_t
MSS_UART_fill_tx_fifo
(
    mss_uart_instance_t * this_uart,
    const uint8_t * tx_buffer,
    size_t tx_size
);

/***************************************************************************//**
  The MSS_UART_get_rx_status() function returns the receiver error status of the
  MSS UART instance. It reads both the current error status of the receiver from
  the UART's line status register (LSR) and the accumulated error status from
  preceding calls to the MSS_UART_get_rx() function, and it combines them using
  a bitwise OR. It returns the cumulative overrun, parity, framing, break and
  FIFO error status of the receiver, since the previous call to
  MSS_UART_get_rx_status(), as an 8-bit encoded value.

  Note:     The MSS_UART_get_rx() function reads and accumulates the receiver
  status of the MSS UART instance before reading each byte from the receiver's
  data register/FIFO. The driver maintains a sticky record of the cumulative
  receiver error status, which persists after the MSS_UART_get_rx() function
  returns. The MSS_UART_get_rx_status() function clears the driver's sticky
  receiver error record before returning.

  Note:     The driver's transmit functions also read the line status
  register (LSR) as part of their implementation. When the driver reads the
  LSR, the UART clears any active receiver error bits in the LSR. This could
  result in the driver losing receiver errors. To avoid any loss of receiver
  errors, the transmit functions also update the driver's sticky record of the
  cumulative receiver error status whenever they read the LSR.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function returns the UART's receiver error status as an 8-bit unsigned
    integer. The returned value is 0, if no receiver errors occurred. The driver
    provides a set of bit mask constants that should be compared with and/or
    used to mask the returned value to determine the receiver error status.
    When the return value is compared to the following bit masks, a non-zero
    result indicates that the corresponding error occurred:
        - MSS_UART_OVERRUN_ERROR      (bit mask = 0x02)
        - MSS_UART_PARITY_ERROR       (bit mask = 0x04)
        - MSS_UART_FRAMING_ERROR      (bit mask = 0x08)
        - MSS_UART_BREAK_ERROR        (bit mask = 0x10)
        - MSS_UART_FIFO_ERROR         (bit mask = 0x80)    

    When the return value is compared to the following bit mask, a non-zero
    result indicates that no error occurred:
        - MSS_UART_NO_ERROR       (bit mask = 0x00)    

    Upon unsuccessful execution, this function returns:
        - MSS_UART_INVALID_PARAM      (bit mask = 0xFF)

  @example
  @code
    uint8_t rx_data[MAX_RX_DATA_SIZE];
    uint8_t err_status;
    err_status = MSS_UART_get_rx_status(&g_mss_uart0);

    if(MSS_UART_NO_ERROR == err_status)
    {
       rx_size = MSS_UART_get_rx(&g_mss_uart0_lo, rx_data, MAX_RX_DATA_SIZE);
    }
  @endcode
 */
uint8_t
MSS_UART_get_rx_status
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_get_modem_status() function returns the modem status of the
  MSS UART instance. It reads the modem status register (MSR) and returns
  the 8-bit value. The bit encoding of the returned value is exactly the
  same as the definition of the bits in the MSR.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function returns current state of the UART's MSR as an 8-bit
    unsigned integer. The driver provides the following set of bit mask
    constants that should be compared with and/or used to mask the
    returned value to determine the modem status:
        - MSS_UART_DCTS   (bit mask = 0x01)
        - MSS_UART_DDSR   (bit mask = 0x02)
        - MSS_UART_TERI   (bit mask = 0x04)
        - MSS_UART_DDCD   (bit mask = 0x08)
        - MSS_UART_CTS    (bit mask = 0x10)
        - MSS_UART_DSR    (bit mask = 0x20)
        - MSS_UART_RI     (bit mask = 0x40)
        - MSS_UART_DCD    (bit mask = 0x80)

  @example
  @code
    void uart_modem_status_isr(mss_uart_instance_t * this_uart)
    {
        uint8_t status;
        status = MSS_UART_get_modem_status(this_uart);
        if( status & MSS_UART_DCTS )
        {
            uart_dcts_handler();
        }
        if( status & MSS_UART_CTS )
        {
            uart_cts_handler();
        }
    }
  @endcode
 */
uint8_t
MSS_UART_get_modem_status
(
    const mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_get_tx_status() function returns the transmitter status of the
  MSS UART instance. It reads both the UART's line status register (LSR) and
  returns the status of the transmit holding register empty (THRE) and
  transmitter empty (TEMT) bits.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function returns the UART's transmitter status as an 8-bit unsigned
    integer. The returned value is 0, if the transmitter status bits are not
    set or the function execution failed. The driver provides a set of bit
    mask constants that should be compared with and/or used to mask the
    returned value to determine the transmitter status.
    When the return value is compared to the following bit mask, a non-zero
    result indicates that the corresponding transmitter status bit is set:
      - MSS_UART_THRE       (bit mask = 0x20)
      - MSS_UART_TEMT       (bit mask = 0x40)    

    When the return value is compared to the following bit mask, a non-zero
    result indicates that the transmitter is busy or the function execution
    failed:
      - MSS_UART_TX_BUSY        (bit mask = 0x00)

  @example
  @code
    uint8_t tx_buff[10] = "abcdefghi";
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx(&g_mss_uart0_lo, tx_buff, sizeof(tx_buff));

    while(!(MSS_UART_TEMT & MSS_UART_get_tx_status(&g_mss_uart0)))
    {
        ;
    }
  @endcode
 */
uint8_t
MSS_UART_get_tx_status
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_set_break() function sends the break
  (9 zeros after Stop bit) signal on the Tx line. This function can be used
  only when the MSS UART is initialized in LIN mode by using MSS_UART_lin_init().
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_lin_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_break(&g_mss_uart0);
  @endcode
 */
void
MSS_UART_set_break
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_clear_break() function removes the break signal on the
  Tx line. This function can be used only when the MSS UART is initialized in
  LIN mode by using MSS_UART_lin_init().
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_lin_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_clear_break(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_clear_break
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_enable_half_duplex() function enables the half-duplex
  (single wire) mode for the MSS UART. Though it finds application in Smartcard
  mode, half-duplex mode can be used in other modes as well.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_enable_half_duplex(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_enable_half_duplex
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_disable_half_duplex() function disables the half-duplex
  (single wire) mode for the MSS UART. Though it finds application in Smartcard
  mode, half-duplex mode can be used in other modes as well.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_disable_half_duplex(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_disable_half_duplex
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_set_rx_endian() function configures the LSB first or
  MSB first setting for MSS UART receiver.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param endian
    The endian parameter tells if the LSB or MSB is received first.
    This parameter is of type mss_uart_endian_t. The allowed values are:
      - MSS_UART_LITTLEEND
      - MSS_UART_BIGEND

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_rx_endian(&g_mss_uart0_lo, MSS_UART_LITTLEEND);
  @endcode
 */
void
MSS_UART_set_rx_endian
(
    mss_uart_instance_t * this_uart,
    mss_uart_endian_t endian
);

/***************************************************************************//**
  The MSS_UART_set_tx_endian() function configures the LSB first or
  MSB first setting for MSS UART transmitter.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param endian
    The endian parameter tells if the LSB or MSB is transmitted first.
    This parameter is of type mss_uart_endian_t. The allowed values are:
      - MSS_UART_LITTLEEND
      - MSS_UART_BIGEND

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_tx_endian(&g_mss_uart0_lo, MSS_UART_LITTLEEND);
  @endcode
 */
void
MSS_UART_set_tx_endian
(
    mss_uart_instance_t * this_uart,
    mss_uart_endian_t endian
);

/***************************************************************************//**
  The MSS_UART_set_filter_length() function configures the glitch
  filter length of the MSS UART. This should be configured in accordance with
  the chosen baud rate.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param length
    The length parameter is of mss_uart_filter_length_t type and it determines
    the length of the glitch filter.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_filter_length(&g_mss_uart0_lo, MSS_UART_LEN2);
  @endcode
 */
void
MSS_UART_set_filter_length
(
    mss_uart_instance_t * this_uart,
    mss_uart_filter_length_t length
);

/***************************************************************************//**
  The MSS_UART_enable_afm() function enables address flag detection
  mode of the MSS UART.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_enable_afm(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_enable_afm
(
     mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_disable_afm() function disables address flag detection
  mode of the MSS UART.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_disable_afm(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_disable_afm
(
     mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_enable_afclear() function enables address flag clear
  of the MSS UART. This should be used in conjunction with address flag
  detection mode (AFM).
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_enable_afclear(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_enable_afclear
(
     mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_disable_afclear() function disables address flag
  clear of the MSS UART. This should be used in conjunction with address flag
  detection mode (AFM).
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_disable_afclear(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_disable_afclear
(
     mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_enable_rx_timeout() function enables and configures
  the receiver timeout functionality of MSS UART. This function accepts the
  timeout parameter and applies the timeout based up on the baud rate as per
  the formula 4 x timeout x bit time.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param timeout
    The timeout parameter specifies the receiver timeout multiple.
    It should be configured according to the baud rate in use.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_enable_rx_timeout(&g_mss_uart0_lo, 24);
  @endcode
 */
void
MSS_UART_enable_rx_timeout
(
    mss_uart_instance_t * this_uart,
    uint8_t timeout
);

/***************************************************************************//**
  The MSS_UART_disable_rx_timeout() function disables the receiver
  timeout functionality of MSS UART.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_disable_rx_timeout(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_disable_rx_timeout
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_enable_tx_time_guard() function enables and configures
  the transmitter timeguard functionality of MSS UART. This function accepts
  the timeguard parameter and applies the timeguard based on the baud rate
  as per the formula timeguard x bit time.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param timeguard
    The timeguard parameter specifies the transmitter timeguard multiple.
    It should be configured according to the baud rate in use.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_enable_tx_time_guard(&g_mss_uart0_lo, 24);
  @endcode
 */
void
MSS_UART_enable_tx_time_guard
(
    mss_uart_instance_t * this_uart,
    uint8_t timeguard
);

/***************************************************************************//**
  The MSS_UART_disable_tx_time_guard() function disables the
  transmitter timeguard functionality of MSS UART.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_disable_tx_time_guard(&g_mss_uart0_lo);
  @endcode
 */
void
MSS_UART_disable_tx_time_guard
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_set_address() function sets the 8-bit address for
  the MSS UART, which is referenced by this_uart parameter.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param address
    The address parameter is the 8-bit address allocated
    to the MSS UART, which is referenced by this_uart parameter.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_address(&g_mss_uart0_lo, 0xAA);
  @endcode
 */
void
MSS_UART_set_address
(
    mss_uart_instance_t * this_uart,
    uint8_t address
);

/***************************************************************************//**
  The MSS_UART_set_ready_mode() function configures the MODE0 or MODE1
  to the TXRDY and RXRDY signals of the MSS UART, which is referenced by
  this_uart parameter. The mode parameter is used to provide the mode to be
  configured. See the following details for the MODE0 and MODE1 description.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param mode
    The mode parameter is the mss_uart_ready_mode_t type which is used to
    configure the TXRDY and RXRDY signal modes.    
    MODE0: RXRDY goes high (active) when there is at least one character
    in the RX FIFO (that is, the RDA is triggered when there is at least one
    character in the RX FIFO). TXRDY goes inactive after the first character is
    loaded in the Tx FIFO.    
    MODE1: RXRDY goes high (active) when the trigger level or the timeout is
    reached. TXRDY goes inactive when the Tx FIFO is completely full.

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_ready_mode(&g_mss_uart0_lo, MSS_UART_READY_MODE0);
  @endcode
 */
void
MSS_UART_set_ready_mode
(
    mss_uart_instance_t * this_uart,
    mss_uart_ready_mode_t mode
);

/***************************************************************************//**
  The MSS_UART_set_usart_mode() function configures the MSS UART, which is
  referenced by the this_uart parameter in USART mode. Various USART modes
  are supported which can be configured by the parameter mode of
  mss_uart_usart_mode_t type.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @param mode
    The mode parameter is the USART mode to be configured.
    This parameter is of mss_uart_usart_mode_t type. The allowed values are:
      - MSS_UART_ASYNC_MODE
      - MSS_UART_SYNC_SLAVE_POS_EDGE_CLK
      - MSS_UART_SYNC_SLAVE_NEG_EDGE_CLK
      - MSS_UART_SYNC_MASTER_POS_EDGE_CLK
      - MSS_UART_SYNC_MASTER_NEG_EDGE_CLK

  @return
    This function does not return a value.

  @example
  @code
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_usart_mode(&g_mss_uart0_lo, MSS_UART_SYNC_MASTER_POS_EDGE_CLK);
  @endcode
 */
void
MSS_UART_set_usart_mode
(
    mss_uart_instance_t * this_uart,
    mss_uart_usart_mode_t mode
);

/***************************************************************************//**
  The MSS_UART_enable_local_irq() function enables the MMUART
  interrupt as a local interrupt to the hart rather than as a PLIC interrupt.
  MMUART interrupt can be configured to trigger a PLIC interrupt or it
  can be configured to trigger a local interrupt. The arrangement is such that
  the UART0 interrupt can appear as local interrupt on E51. The UART1 to UART4
  interrupts can appear as local interrupt to U51_1 to U54_4, respectively.
  The UART0 to UART4 can appear as PLIC interrupt. Multiple harts can enable
  and receive the PLIC interrupt, the hart that claims the interrupt processes
  it. For rest of the harts, the IRQ gets silently skipped as the interrupt
  claim has already been taken.    
  To enable the local interrupt, application must explicitly call
  MSS_UART_enable_local_irq() function. Note that this function disables the
  interrupt over PLIC if it was previously enabled.    
  This function must be called after the MMUART is initialized, the required
  interrupt handler functions are set and before initiating any data transfers.
  If you want to register multiple interrupt handlers such as Tx handler, rx
  handler and so on, then this function must be called after all such handlers
  are set.    
  Call to this function is treated as one time activity. The driver gives no
  option to disable the local interrupt and enable the PLIC interrupt again at
  runtime.
  
  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure that identifies the MSS UART hardware block performing
    the requested function. There are ten such data structures. The data
    structures from g_mss_uart0_lo to g_mss_uart4_lo are associated with 
    MSS UART0 to MSS UART4 when they are connected on the AXI switch slave 5
    (main APB bus). The data structures g_mss_uart0_hi to g_mss_uart4_hi are
    associated with MSS UART0 to MSS UART4 when they are connected on the AXI
    switch slave 6 (AMP APB bus). This parameter must point to one of these ten
    global data structure defined within the UART driver.    
    Note: If you are using the UART on the AMP APB bus, the hardware
    configuration to connect UART on AMP APB bus must already be done by the
    application using SYSREG registers before initializing the UART instance
    structure.

  @return
    This function does not return a value.

  @example
  @code


    __enable_irq();
    MSS_UART_init(&g_mss_uart0_lo,
             MSS_UART_57600_BAUD,
             MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_set_rx_handler(&g_mss_uart0_lo,
                            uart0_rx_handler,
                            MSS_UART_FIFO_SINGLE_BYTE);

    MSS_UART_enable_local_irq(&g_mss_uart0_lo);

  @endcode
 */
void
MSS_UART_enable_local_irq
(
    mss_uart_instance_t * this_uart
);

#ifdef __cplusplus
}
#endif

#endif /* __MSS_UART_H_ */
