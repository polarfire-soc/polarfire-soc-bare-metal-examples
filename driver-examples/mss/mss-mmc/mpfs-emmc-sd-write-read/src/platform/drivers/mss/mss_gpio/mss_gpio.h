/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_gpio.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) GPIO bare metal software
 * driver public API.
 *
 */

/*=========================================================================*//**
  @mainpage PolarFire® SoC MSS GPIO Bare Metal Driver

  ==============================================================================
  Introduction
  ==============================================================================
  The PolarFire® SoC Microprocessor Subsystem (MSS) includes three blocks of
  general purpose input/outputs (GPIO). The GPIO0, GPIO1, and GPIO2 blocks have
  14, 24, and 32 GPIO ports respectively. This software driver provides a set of
  functions for controlling the MSS GPIO blocks as part of a bare metal system
  where no operating system is available. This driver can be adapted for use as
  part of an operating system, but the implementation of the adaptation layer
  between this driver and the operating system's driver model is outside the
  scope of this driver.

  ==============================================================================
  Hardware Flow Dependencies
  ==============================================================================
  The configuration of all the features of the MSS GPIO peripherals is covered
  by this driver with the exception of the PolarFire SoC IOMUX configuration.
  PolarFire SoC allows multiple non-concurrent uses of some external pins
  through the Pin Multiplexing (IOMUX) configuration. This feature allows
  optimization of external pin usage by assigning external pins for use by
  either the Microprocessor Subsystem or the FPGA fabric. The MSS GPIO signals
  are routed through IOMUXs to the PolarFire SoC device external pins. The MSS
  GPIO serial signals are also routed through IOMUXs to the PolarFire SoC FPGA
  fabric. For more information on IOMUX, see the IOMUX section of the PolarFire
  SoC Microprocessor Subsystem (MSS) User's Guide.

  The IOMUXs are configured using the PolarFire SoC MSS configurator tool. You
  must ensure that the MSS GPIO peripherals are enabled and configured in the
  PolarFire SoC MSS configurator if you wish to use them. For more information
  on IOMUXs, see the IOMUX section of the PolarFire SoC Microprocessor Subsystem
  (MSS) User's Guide.

  On PolarFire SoC, an AXI switch forms a bus matrix interconnect among multiple
  masters and multiple slaves. Five RISC-V CPUs connect to the Master ports, M10
  to M14, of the AXI switch. By default, all the APB peripherals are accessible
  on AXI-Slave 5 of the AXI switch through the AXI to AHB and AHB to APB bridges
  (referred to as the Main APB bus). However, to support logical separation in
  the Asymmetric Multi-Processing (AMP) mode of operation, the APB peripherals
  can alternatively be accessed on the AXI-Slave 6 through the AXI to AHB and
  AHB to APB bridges (referred the as the AMP APB bus).

  Application must make sure that the desired GPIO instance is appropriately
  configured on one of the APB buses described above by configuring the
  PolarFire SoC system registers (SYSREG) as per the application need, and that
  the appropriate data structures are passed as a parameter to the functions
  provided by this driver.

  The base address and register addresses are defined in this driver as
  constants. The interrupt number assignment for the MSS GPIO peripherals are
  defined as constants in the MPFS HAL. You must ensure that the latest MPFS HAL
  is included in the project settings of the SoftConsole tool chain and that it
  is generated into your project.

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
  The MSS GPIO driver is initialized by calling the MSS_GPIO_init() function.
  The MSS_GPIO_init() function must be called before calling any other MSS GPIO.

  --------------------------------
  Configuration
  --------------------------------
  Each GPIO port is individually configured by calling the MSS_GPIO_config()
  function. Configuration includes deciding if a GPIO port will be used as an
  input, or an output, or both. GPIO ports configured as inputs can be further
  configured to generate interrupts based on the input's state. Interrupts can
  be level or edge sensitive. Level interrupts will clear once the interrupt
  source is removed. Edge sensitive interrupts hold their value until it is
  cleared by MSS_GPIO_clear_irq(). The MSS_GPIO_config_byte() function can be
  used to configure eight consecutive GPIO ports identically. The
  MSS_GPIO_config_byte() function can be used to configure all available GPIO
  ports identically.

  --------------------------------
  Reading and Setting GPIO State
  --------------------------------
  The state of the GPIO ports can be read and set using the following functions:
    - MSS_GPIO_get_inputs()
    - MSS_GPIO_get_outputs()
    - MSS_GPIO_set_outputs()
    - MSS_GPIO_set_output()
    - MSS_GPIO_drive_inout()

  --------------------------------
  Interrupt Control
  --------------------------------
  Interrupts generated by GPIO ports configured as inputs are controlled using
  the following functions:
    - MSS_GPIO_enable_irq()
    - MSS_GPIO_disable_irq()
    - MSS_GPIO_clear_irq()
    - MSS_GPIO_get_irq()
    - MSS_GPIO_enable_nondirect_irq()
    - MSS_GPIO_disable_nondirect_irq()

  The GPIO interrupts are multiplexed. Total GPIO interrupt inputs on PLIC are
  41.

  41 = (14 from GPIO0 + 24 from GPIO1 + 3 non direct interrupts) GPIO2
  interrupts are not available by default. Setting the corresponding bit in
  GPIO_INTERRUPT_FAB_CR(31:0) system register enables GPIO2(31:0) corresponding
  interrupts on PLIC. For example, if GPIO_INTERRUPT_FAB_CR bit0 is set then
  GPIO2 bit0 interrupt is available on the direct input pin on the PLIC. In this
  case, the GPIO0 bit 0 interrupt is not available on the direct input pin on
  the PLIC. The GPIO0 non-direct input is asserted as an OR of all the GPIO0
  interrupts, which does not have a direct interrupt input on PLIC. These are
  connected to the corresponding non-direct input pin. The following table
  explains all the GPIO direct and non-direct interrupt connectivity options.

  | PLIC | GPIO_INTERRUPT_FAB_CR = 0 | GPIO_INTERRUPT_FAB_CR = 1 |
  |------|---------------------------|---------------------------|
  |   0  |      GPIO0 bit 0          |        GPIO2 bit 0        |
  |   1  |      GPIO0 bit 1          |        GPIO2 bit 1        |
  |  ... |         ...               |        ...                |
  |  12  |      GPIO0 bit 12         |        GPIO2 bit 12       |
  |  13  |      GPIO0 bit 13         |        GPIO2 bit 13       |
  |  14  |      GPIO1 bit 0          |        GPIO2 bit 14       |
  |  15  |      GPIO1 bit 1          |        GPIO2 bit 15       |
  |  ... |          ...              |             ...           |
  |  30  |      GPIO1  bit 16        |        GPIO2 bit 30       |
  |  31  |      GPIO1  bit 17        |        GPIO2 bit 31       |


  | PLIC |                           Interrupt source                           |
  |------|----------------------------------------------------------------------|
  |  32  |                             GPIO1 bit 18                             |
  |  33  |                             GPIO1 bit 19                             |
  |  34  |                             GPIO1 bit 20                             |
  |  35  |                             GPIO1 bit 21                             |
  |  36  |                             GPIO1 bit 22                             |
  |  37  |                             GPIO1 bit 23                             |
  |  38  | OR of all GPIO0 interrupts who don't have a direct connection enabled|
  |  39  | OR of all GPIO1 interrupts who don't have a direct connection enabled|
  |  40  | OR of all GPIO2 interrupts who don't have a direct connection enabled|


    Note:
    GPIO_INTERRUPT_FAB_CR controls the multiplexing in preceding table. It is
    your responsibility to set up the GPIO_INTERRUPT_FAB_CR bits in application
    code. You must make sure that you are using the valid combination of
    GPIO0/1/2 interrupt per preceding table.

 *//*=========================================================================*/
#ifndef MSS_GPIO_H_
#define MSS_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*-------------------------------------------------------------------------*//**
  The mss_gpio_id_t enumeration identifies individual GPIO ports as an
  argument to the following functions:
    - MSS_GPIO_config()
    - MSS_GPIO_set_output() and MSS_GPIO_drive_inout()
    - MSS_GPIO_enable_irq(), MSS_GPIO_disable_irq(), and MSS_GPIO_clear_irq()

  Note:
  GPIO0, GPIO1, and GPIO2 blocks have 14, 24, and 32 GPIO ports respectively.
 */
typedef enum mss_gpio_id
{
    MSS_GPIO_0 = 0,
    MSS_GPIO_1 = 1,
    MSS_GPIO_2 = 2,
    MSS_GPIO_3 = 3,
    MSS_GPIO_4 = 4,
    MSS_GPIO_5 = 5,
    MSS_GPIO_6 = 6,
    MSS_GPIO_7 = 7,
    MSS_GPIO_8 = 8,
    MSS_GPIO_9 = 9,
    MSS_GPIO_10 = 10,
    MSS_GPIO_11 = 11,
    MSS_GPIO_12 = 12,
    MSS_GPIO_13 = 13,
    MSS_GPIO_14 = 14,
    MSS_GPIO_15 = 15,
    MSS_GPIO_16 = 16,
    MSS_GPIO_17 = 17,
    MSS_GPIO_18 = 18,
    MSS_GPIO_19 = 19,
    MSS_GPIO_20 = 20,
    MSS_GPIO_21 = 21,
    MSS_GPIO_22 = 22,
    MSS_GPIO_23 = 23,
    MSS_GPIO_24 = 24,
    MSS_GPIO_25 = 25,
    MSS_GPIO_26 = 26,
    MSS_GPIO_27 = 27,
    MSS_GPIO_28 = 28,
    MSS_GPIO_29 = 29,
    MSS_GPIO_30 = 30,
    MSS_GPIO_31 = 31
} mss_gpio_id_t;

/*-------------------------------------------------------------------------*//**
  The mss_gpio_inout_state_t enumeration specifies the output state of an INOUT
  GPIO. It is passed as an argument to the MSS_GPIO_drive_inout() function.
 */
typedef enum mss_gpio_inout_state
{
    MSS_GPIO_DRIVE_LOW = 0,
    MSS_GPIO_DRIVE_HIGH,
    MSS_GPIO_HIGH_Z
} mss_gpio_inout_state_t;

/*-------------------------------------------------------------------------*//**
  The mss_gpio_byte_num_t enumeration specifies the set of the eight consecutive
  GPIO ports that are to be configured. It is passed as an argument to the
  MSS_GPIO_config_byte() function.
 */
typedef enum mss_gpio_byte_num
{
    MSS_GPIO_BYTE_0 = 0,
    MSS_GPIO_BYTE_1,
    MSS_GPIO_BYTE_2,
    MSS_GPIO_BYTE_3,
    MSS_GPIO_BYTE_INVALID,
} mss_gpio_byte_num_t;

/*-------------------------------------------------------------------------*//**
  GPIO Instance Identification
  ============================
  These constants are provided for the application use. These constants must be
  passed as a first parameter of all the APIs provided by this driver. The
  GPIO0_LO, GPIO1_LO, and GPIO2_LO represent the GPIO0, GPIO1, and GPIO2
  hardware blocks when they are connected on the main APB bus. The GPIO0_HI,
  GPIO1_HI, and GPIO2_HI represent the GPIO0, GPIO1, and GPIO2 hardware blocks
  when they are connected on the AMP APB bus.

  | Constant | Description                           |
  |----------|---------------------------------------|
  | GPIO0_LO | GPIO0 block connected on main APB bus |
  | GPIO1_LO | GPIO1 block connected on main APB bus |
  | GPIO2_LO | GPIO2 block connected on main APB bus |
  | GPIO0_HI | GPIO0 block connected on AMP APB bus  |
  | GPIO1_HI | GPIO1 block connected on AMP APB bus  |
  | GPIO2_HI | GPIO2 block connected on AMP APB bus  |

 */
#define GPIO0_LO                        ((GPIO_TypeDef*)0x20120000UL)
#define GPIO1_LO                        ((GPIO_TypeDef*)0x20121000UL)
#define GPIO2_LO                        ((GPIO_TypeDef*)0x20122000UL)
#define GPIO0_HI                        ((GPIO_TypeDef*)0x28120000UL)
#define GPIO1_HI                        ((GPIO_TypeDef*)0x28121000UL)
#define GPIO2_HI                        ((GPIO_TypeDef*)0x28122000UL)

/*-------------------------------------------------------------------------*//**
  GPIO Port Masks
  ===============
  These constant definitions are used as an argument to the
  MSS_GPIO_set_outputs() function to identify GPIO ports. A bitwise OR of these
  constants can be used to specify multiple GPIO ports. These definitions can
  also be used to identify GPIO ports through logical operations on the return
  value of the MSS_GPIO_get_inputs() function.

  | Constant         | Description           |
  |------------------|-----------------------|
  | MSS_GPIO_0_MASK  | GPIO port 0-bit mask  |
  | MSS_GPIO_1_MASK  | GPIO port 1-bit mask  |
  | MSS_GPIO_2_MASK  | GPIO port 2-bit mask  |
  | MSS_GPIO_3_MASK  | GPIO port 3-bit mask  |
  | MSS_GPIO_4_MASK  | GPIO port 4-bit mask  |
  | MSS_GPIO_5_MASK  | GPIO port 5-bit mask  |
  | MSS_GPIO_6_MASK  | GPIO port 6-bit mask  |
  | MSS_GPIO_7_MASK  | GPIO port 7-bit mask  |
  | MSS_GPIO_8_MASK  | GPIO port 8-bit mask  |
  | MSS_GPIO_9_MASK  | GPIO port 9-bit mask  |
  | MSS_GPIO_10_MASK | GPIO port 10-bit mask |
  | MSS_GPIO_11_MASK | GPIO port 11-bit mask |
  | MSS_GPIO_12_MASK | GPIO port 12-bit mask |
  | MSS_GPIO_13_MASK | GPIO port 13-bit mask |
  | MSS_GPIO_14_MASK | GPIO port 14-bit mask |
  | MSS_GPIO_15_MASK | GPIO port 15-bit mask |
  | MSS_GPIO_16_MASK | GPIO port 16-bit mask |
  | MSS_GPIO_17_MASK | GPIO port 17-bit mask |
  | MSS_GPIO_18_MASK | GPIO port 18-bit mask |
  | MSS_GPIO_19_MASK | GPIO port 19-bit mask |
  | MSS_GPIO_20_MASK | GPIO port 20-bit mask |
  | MSS_GPIO_21_MASK | GPIO port 21-bit mask |
  | MSS_GPIO_22_MASK | GPIO port 22-bit mask |
  | MSS_GPIO_23_MASK | GPIO port 23-bit mask |
  | MSS_GPIO_24_MASK | GPIO port 24-bit mask |
  | MSS_GPIO_25_MASK | GPIO port 25-bit mask |
  | MSS_GPIO_26_MASK | GPIO port 26-bit mask |
  | MSS_GPIO_27_MASK | GPIO port 27-bit mask |
  | MSS_GPIO_28_MASK | GPIO port 28-bit mask |
  | MSS_GPIO_29_MASK | GPIO port 29-bit mask |
  | MSS_GPIO_30_MASK | GPIO port 30-bit mask |
  | MSS_GPIO_31_MASK | GPIO port 31-bit mask |

 */
#define MSS_GPIO_0_MASK         0x00000001UL
#define MSS_GPIO_1_MASK         0x00000002UL
#define MSS_GPIO_2_MASK         0x00000004UL
#define MSS_GPIO_3_MASK         0x00000008UL
#define MSS_GPIO_4_MASK         0x00000010UL
#define MSS_GPIO_5_MASK         0x00000020UL
#define MSS_GPIO_6_MASK         0x00000040UL
#define MSS_GPIO_7_MASK         0x00000080UL
#define MSS_GPIO_8_MASK         0x00000100UL
#define MSS_GPIO_9_MASK         0x00000200UL
#define MSS_GPIO_10_MASK        0x00000400UL
#define MSS_GPIO_11_MASK        0x00000800UL
#define MSS_GPIO_12_MASK        0x00001000UL
#define MSS_GPIO_13_MASK        0x00002000UL
#define MSS_GPIO_14_MASK        0x00004000UL
#define MSS_GPIO_15_MASK        0x00008000UL
#define MSS_GPIO_16_MASK        0x00010000UL
#define MSS_GPIO_17_MASK        0x00020000UL
#define MSS_GPIO_18_MASK        0x00040000UL
#define MSS_GPIO_19_MASK        0x00080000UL
#define MSS_GPIO_20_MASK        0x00100000UL
#define MSS_GPIO_21_MASK        0x00200000UL
#define MSS_GPIO_22_MASK        0x00400000UL
#define MSS_GPIO_23_MASK        0x00800000UL
#define MSS_GPIO_24_MASK        0x01000000UL
#define MSS_GPIO_25_MASK        0x02000000UL
#define MSS_GPIO_26_MASK        0x04000000UL
#define MSS_GPIO_27_MASK        0x08000000UL
#define MSS_GPIO_28_MASK        0x10000000UL
#define MSS_GPIO_29_MASK        0x20000000UL
#define MSS_GPIO_30_MASK        0x40000000UL
#define MSS_GPIO_31_MASK        0x80000000UL

/*-------------------------------------------------------------------------*//**
  GPIO Port I/O Mode
  ==================
  These constant definitions are used as an argument to the MSS_GPIO_config()
  function to specify the I/O mode of each GPIO port.

  | Constant             | Description                |
  |----------------------|----------------------------|
  | MSS_GPIO_INPUT_MODE  | Input port only            |
  | MSS_GPIO_OUTPUT_MODE | Output port only           |
  | MSS_GPIO_INOUT_MODE  | Both input and output port |

 */
#define MSS_GPIO_INPUT_MODE              0x0000000002UL
#define MSS_GPIO_OUTPUT_MODE             0x0000000005UL
#define MSS_GPIO_INOUT_MODE              0x0000000003UL

/*-------------------------------------------------------------------------*//**
  GPIO Interrupt Mode
  ===================
  These constant definitions are used as an argument to the MSS_GPIO_config()
  function to specify the interrupt mode of each GPIO port.

  | Constant                   | Description                                         |
  |----------------------------|-----------------------------------------------------|
  | MSS_GPIO_IRQ_LEVEL_HIGH    | Interrupt on GPIO input level High                  |
  | MSS_GPIO_IRQ_LEVEL_LOW     | Interrupt on GPIO input level Low                   |
  | MSS_GPIO_IRQ_EDGE_POSITIVE | Interrupt on GPIO input positive edge               |
  | MSS_GPIO_IRQ_EDGE_NEGATIVE | Interrupt on GPIO input negative edge               |
  | MSS_GPIO_IRQ_EDGE_BOTH     | Interrupt on GPIO input positive and negative edges |

 */
#define MSS_GPIO_IRQ_LEVEL_HIGH           0x0000000000UL
#define MSS_GPIO_IRQ_LEVEL_LOW            0x0000000020UL
#define MSS_GPIO_IRQ_EDGE_POSITIVE        0x0000000040UL
#define MSS_GPIO_IRQ_EDGE_NEGATIVE        0x0000000060UL
#define MSS_GPIO_IRQ_EDGE_BOTH            0x0000000080UL

/*------------------------Private data structures-----------------------------*/
/*----------------------------------- GPIO -----------------------------------*/
/*----------------------------------------------------------------------------*/
typedef struct
{
    volatile uint32_t GPIO_CFG[32];
    volatile uint32_t GPIO_IRQ;
    volatile const uint32_t GPIO_IN;
    volatile uint32_t GPIO_OUT;
    volatile uint32_t GPIO_CFG_ALL;
    volatile uint32_t GPIO_CFG_BYTE[4];
    volatile uint32_t GPIO_CLR_BITS;
    volatile uint32_t GPIO_SET_BITS;

} GPIO_TypeDef;

/*--------------------------------Public APIs---------------------------------*/

/*-------------------------------------------------------------------------*//**
  The MSS_GPIO_init() function initializes the PolarFire SoC MSS GPIO block. It
  resets the MSS GPIO hardware block and clears any pending MSS GPIO interrupts
  in the interrupt controller. When the function exits, it takes the MSS GPIO
  block out of reset.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @return
    This function does not return a value.

  @example
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config(GPIO0_LO, MSS_GPIO_4, MSS_GPIO_INPUT_MODE |
                        MSS_GPIO_IRQ_EDGE_POSITIVE);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_init
(
    GPIO_TypeDef * gpio
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_config() configures an individual GPIO port.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param port_id
    The port_id parameter identifies the GPIO port to be configured. An
    enumeration item of the form MSS_GPIO_n, where n is the number of the GPIO
    port, identifies the GPIO port. For example, MSS_GPIO_0 identifies the first
    GPIO port and MSS_GPIO_31 identifies the last GPIO port.

  @param config
    The config parameter specifies the configuration to be applied to the GPIO
    port identified by the port_id parameter. It is a bitwise OR of the required
    I/O mode and the required interrupt mode. The interrupt mode is not relevant
    if the GPIO is configured as an output only.
    - These I/O mode constants are allowed:
      - MSS_GPIO_INPUT_MODE
      - MSS_GPIO_OUTPUT_MODE
      - MSS_GPIO_INOUT_MODE
    - These interrupt mode constants are allowed:
      - MSS_GPIO_IRQ_LEVEL_HIGH
      - MSS_GPIO_IRQ_LEVEL_LOW
      - MSS_GPIO_IRQ_EDGE_POSITIVE
      - MSS_GPIO_IRQ_EDGE_NEGATIVE
      - MSS_GPIO_IRQ_EDGE_BOTH

  @return
    This function does not return any value.

  @example
    The following call configures GPIO 4 on GPIO0 hardware block on main APB bus
    as an input generated interrupt on the Low to High transition of the input:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config(GPIO0_LO, MSS_GPIO_4, MSS_GPIO_INPUT_MODE |
                        MSS_GPIO_IRQ_EDGE_POSITIVE);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_config
(
    GPIO_TypeDef * gpio,
    mss_gpio_id_t port_id,
    uint32_t config
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_set_outputs() sets the state of all the GPIO ports configured as
  outputs.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param value
    The value parameter specifies the state of the GPIO ports configured as
    outputs. It is a bit mask of the form (MSS_GPIO_n_MASK | MSS_GPIO_m_MASK)
    where n and m are numbers identifying GPIOs. For example, (MSS_GPIO_0_MASK |
    MSS_GPIO_1_MASK | MSS_GPIO_2_MASK) specifies that the first, second, and
    third GPIO outputs must be set High and all other GPIO outputs set Low.
    The driver provides 32 mask constants for this purpose.

  @return
    This function does not return any value.

  @example
    Set GPIOs outputs 0 and 8 High and all other GPIO outputs Low:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config(GPIO0_LO, MSS_GPIO_4, MSS_GPIO_INPUT_MODE |
                        MSS_GPIO_IRQ_EDGE_POSITIVE);
        MSS_GPIO_set_outputs(GPIO0_LO, MSS_GPIO_0_MASK | MSS_GPIO_8_MASK);
        return (0u);
    }
  @endcode

  @example
    Set GPIOs outputs 2 and 4 Low without affecting other GPIO outputs:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        uint32_t gpio_outputs;
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config(GPIO0_LO, MSS_GPIO_4, MSS_GPIO_INPUT_MODE |
                        MSS_GPIO_IRQ_EDGE_POSITIVE);

        gpio_outputs = MSS_GPIO_get_outputs();
        gpio_outputs &= ~(MSS_GPIO_2_MASK | MSS_GPIO_4_MASK);
        MSS_GPIO_set_outputs(GPIO0_LO, gpio_outputs);

        return (0u);
    }
  @endcode
  @see MSS_GPIO_get_outputs()
 */
static inline void MSS_GPIO_set_outputs
(
    GPIO_TypeDef * gpio,
    uint32_t value
)
{
    gpio->GPIO_OUT = value;
}

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_config_all() configures all the ports of the GPIO block. This
  function applies the same configuration values to all GPIO ports.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param config
    The config parameter specifies the configuration applied to all the GPIO
    ports. It is a bitwise OR of the required I/O mode and the required
    interrupt mode. The interrupt mode is not relevant if the GPIO is configured
    as an output only.
    - These I/O mode constants are allowed:
      - MSS_GPIO_INPUT_MODE
      - MSS_GPIO_OUTPUT_MODE
      - MSS_GPIO_INOUT_MODE
    - These interrupt mode constants are allowed:
      - MSS_GPIO_IRQ_LEVEL_HIGH
      - MSS_GPIO_IRQ_LEVEL_LOW
      - MSS_GPIO_IRQ_EDGE_POSITIVE
      - MSS_GPIO_IRQ_EDGE_NEGATIVE
      - MSS_GPIO_IRQ_EDGE_BOTH

  @return
    This function does not return any value.

  @example
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_all(GPIO0_LO, MSS_GPIO_INPUT_MODE |
                            MSS_GPIO_IRQ_EDGE_POSITIVE);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_config_all
(
    GPIO_TypeDef * gpio,
    uint32_t config
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_config_byte() configures the GPIO ports byte-wise (consecutive eight
  ports).

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param byte_num
    The byte_num parameter specifies the byte (consecutive eight ports) which
    needs to be configured. The value 0 indicates the ports from gpio port0 to
    gpio port7. Value of 3 indicates the ports from gpio port25 to gpio port31.
    When you use this function, you must make sure that the gpio ports that you
    are configuring do exist for that particular GPIO hardware block. GPIO0 has
    14 ports. GPIO1 has 24 ports. GPIO3 has 32 ports.

  @param config
    The config parameter specifies the configuration to be applied to the GPIO
    byte identified by the byte_num parameter. It is a bitwise OR of the
    required I/O mode and the required interrupt mode. The interrupt mode is not
    relevant if the GPIO is configured as an output only.
    - These I/O mode constants are allowed:
      - MSS_GPIO_INPUT_MODE
      - MSS_GPIO_OUTPUT_MODE
      - MSS_GPIO_INOUT_MODE
    - These interrupt mode constants are allowed:
      - MSS_GPIO_IRQ_LEVEL_HIGH
      - MSS_GPIO_IRQ_LEVEL_LOW
      - MSS_GPIO_IRQ_EDGE_POSITIVE
      - MSS_GPIO_IRQ_EDGE_NEGATIVE
      - MSS_GPIO_IRQ_EDGE_BOTH

  @return
    This function does not return any value.

  @example
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_byte(GPIO0_LO, MSS_GPIO_BYTE_1, MSS_GPIO_INPUT_MODE |
                            MSS_GPIO_IRQ_EDGE_POSITIVE);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_config_byte
(
    GPIO_TypeDef * gpio,
    mss_gpio_byte_num_t byte_num,
    uint32_t config
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_set_output() sets the state of a single GPIO port configured as an
  output.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param port_id
    The port_id parameter identifies the GPIO port that is to have its output
    set. An enumeration item of the form MSS_GPIO_n, where n is the number of
    the GPIO port, identifies the GPIO port. For example, MSS_GPIO_0 identifies
    the first GPIO port and MSS_GPIO_31 identifies the last GPIO port.

  @param value
    The value parameter specifies the desired state for the GPIO output. A value
    of 0 sets the output Low and a value of 1 sets the output High.

  @return
    This function does not return a value.

  @example
    The following call sets GPIO output 12 High, leaving all other GPIO outputs
    unaffected:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_all(GPIO0_LO, MSS_GPIO_INOUT_MODE);
        MSS_GPIO_set_output(GPIO0_LO, MSS_GPIO_13, 1);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_set_output
(
    GPIO_TypeDef *      gpio,
    mss_gpio_id_t       port_id,
    uint8_t             value
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_get_inputs() reads the current state of all GPIO ports configured as
  inputs.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @return
    This function returns a 32-bit unsigned integer where each bit represents
    the state of a GPIO input. The least significant bit represents the state of
    GPIO input 0 and the most significant bit the state of GPIO input 31.

  @example
    Read and assign the current state of the GPIO outputs to a variable:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        uint32_t gpio_inputs;
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_all(GPIO0_LO, MSS_GPIO_INOUT_MODE);
        gpio_inputs = MSS_GPIO_get_inputs(GPIO0_LO);
        return (0u);
    }
  @endcode
 */
static inline uint32_t
MSS_GPIO_get_inputs(GPIO_TypeDef const * gpio)
{
    return gpio->GPIO_IN;
}

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_enable_nondirect_irq() enables the non-direct interrupt input at the
  PLIC.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @return
    This function does not return any value.

  @example
    Read and assign the current state of the GPIO outputs to a variable:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        uint32_t gpio_inputs;
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_all(GPIO0_LO, MSS_GPIO_INOUT_MODE |
                                        MSS_GPIO_IRQ_EDGE_POSITIVE);
        MSS_GPIO_enable_nondirect_irq(GPIO1_LO);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_enable_nondirect_irq
(
    GPIO_TypeDef const * gpio
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_disable_nondirect_irq() disables the non-direct interrupt input at
  the PLIC.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @return
    This function does not return any value.

  @example
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        uint32_t gpio_inputs;
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_all(GPIO0_LO, MSS_GPIO_INOUT_MODE |
                                        MSS_GPIO_IRQ_EDGE_POSITIVE);
        MSS_GPIO_disable_nondirect_irq(GPIO1_LO);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_disable_nondirect_irq
(
    GPIO_TypeDef const * gpio
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_get_outputs() reads the current state of all GPIO ports configured as
  outputs.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @return
    This function returns a 32-bit unsigned integer where each bit represents
    the state of a GPIO output. The least significant bit represents the state
    of GPIO output 0, and the most significant bit represents the state of GPIO
    output 31.

  @example
    Read and assign the current state of the GPIO outputs to a variable:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        uint32_t gpio_outputs;
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config(GPIO0_LO, MSS_GPIO_4, MSS_GPIO_INPUT_MODE |
                        MSS_GPIO_IRQ_EDGE_POSITIVE);

        gpio_outputs = MSS_GPIO_get_outputs();
        gpio_outputs &= ~(MSS_GPIO_2_MASK | MSS_GPIO_4_MASK);
        MSS_GPIO_set_outputs(gpio_outputs);

        return (0u);
    }
  @endcode
 */
static inline uint32_t MSS_GPIO_get_outputs(GPIO_TypeDef const * gpio)
{
    return gpio->GPIO_OUT;
}

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_get_irq() reads the current value of the IRQ register. The GPIO
  interrupts are multiplexed. The GPIO interrupts, which are not available on
  the direct GPIO interrupt line on the PLIC, are ORed and routed to the
  non-direct interrupt line on the PLIC for the corresponding GPIO hardware
  block. When the non-direct interrupt is asserted, this function determines
  which exact GPIO bit(s) caused the interrupt.

  Note:
  Non-direct level-sensitive interrupts may already be cleared before this
  calling this function.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @return
    This function returns a 32-bit unsigned integer value of the IRQ register.

  @example
    In the non-direct interrupt ISR, read the IRQ register to know which GPIO
    port causes the interrupt:
  @code
    uint8_t gpio2_non_direct_plic_IRQHandler(void)
    {
        uint32_t intr_num = 0;
        intr_num = MSS_GPIO_get_irq(GPIO2_LO);

        for(int cnt=0; cnt<32; cnt++)
        {
            if (1u == (intr_num & 0x00000001U))
            {
                MSS_GPIO_clear_irq(GPIO0_LO, (mss_gpio_id_t)cnt);
            }

            intr_num >>= 1u;
        }

        return EXT_IRQ_KEEP_ENABLED;
    }
  @endcode
 */
static inline uint32_t
MSS_GPIO_get_irq(GPIO_TypeDef const * gpio)
{
    return gpio->GPIO_IRQ;
}

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_drive_inout() sets the output state of a single GPIO port configured
  as an INOUT.

  An INOUT GPIO can be in one of three states:
    - High
    - Low
    - High impedance

  An INOUT output would typically be used where several devices can drive the
  state of a shared signal line. The High and Low states are equivalent to the
  High and Low states of a GPIO configured as an output. The High impedance
  state prevents the GPIO from driving its output state onto the signal line,
  while at the same time allowing the input state of the GPIO to be read.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param port_id
    The port_id parameter identifies the GPIO port for which you want to change
    the output state. An enumeration item of the form MSS_GPIO_n, where n is the
    number of the GPIO port, identifies the GPIO port. For example, MSS_GPIO_0
    identifies the first GPIO port and MSS_GPIO_31 identifies the last GPIO
    port.

  @param inout_state
    The inout_state parameter specifies the state of the GPIO port identified by
    the port_id parameter.
    - Following are the allowed values of type mss_gpio_inout_state_t:
      - MSS_GPIO_DRIVE_HIGH
      - MSS_GPIO_DRIVE_LOW
      - MSS_GPIO_HIGH_Z (High impedance)

  @return
    This function does not return a value.

  @example
    The call to MSS_GPIO_drive_inout() in the following example sets the GPIO 7
    output to the high impedance state:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        uint32_t gpio_inputs;
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_all(GPIO0_LO, MSS_GPIO_INOUT_MODE |
                                        MSS_GPIO_IRQ_EDGE_POSITIVE);
        MSS_GPIO_drive_inout(GPIO0_LO, MSS_GPIO_7, MSS_GPIO_HIGH_Z);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_drive_inout
(
    GPIO_TypeDef *          gpio,
    mss_gpio_id_t           port_id,
    mss_gpio_inout_state_t  inout_state
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_enable_irq() enables interrupt generation for the specified GPIO
  input. Interrupts are generated based on the state of the GPIO input and the
  interrupt mode configured for it by MSS_GPIO_config(). This function enables
  the corresponding GPIO direct interrupt on the PLIC as well.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param port_id
    The port_id parameter identifies the GPIO port for which you want to enable
    interrupt generation. An enumeration item of the form MSS_GPIO_n, where n is
    the number of the GPIO port, identifies the GPIO port. For example,
    MSS_GPIO_0 identifies the first GPIO port and MSS_GPIO_31 identifies the
    last GPIO port.
    
  @return
    This function does not return a value.

  @example
    The call to MSS_GPIO_enable_irq() in the following example allows GPIO 8 to
    generate interrupts:
  @code
    #include "mss_gpio.h"
    int main(void)
    {
        uint32_t gpio_inputs;
        MSS_GPIO_init(GPIO0_LO);
        MSS_GPIO_config_all(GPIO0_LO, MSS_GPIO_INOUT_MODE |
                                        MSS_GPIO_IRQ_EDGE_POSITIVE);
        MSS_GPIO_enable_irq(GPIO0_LO, MSS_GPIO_8);
        return (0u);
    }
  @endcode
 */
void MSS_GPIO_enable_irq
(
    GPIO_TypeDef * gpio,
    mss_gpio_id_t port_id
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_disable_irq() disables interrupt generation for the specified GPIO
  input. This function also disables the corresponding GPIO direct interrupt on
  the PLIC.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param port_id
    The port_id parameter identifies the GPIO port for which you want to disable
    interrupt generation. An enumeration item of the form MSS_GPIO_n, where n is
    the number of the GPIO port, identifies the GPIO port. For example,
    MSS_GPIO_0 identifies the first GPIO port and MSS_GPIO_31 identifies the
    last GPIO port.

  @return
    This function does not return a value.

  @example
    In the following example, calling the MSS_GPIO_disable_irq() prevents GPIO 8
    from generating interrupts:
  @code
    MSS_GPIO_disable_irq(MSS_GPIO_8);
  @endcode
 */
void MSS_GPIO_disable_irq
(
    GPIO_TypeDef * gpio,
    mss_gpio_id_t port_id
);

/*-------------------------------------------------------------------------*//**
  MSS_GPIO_clear_irq() clears a pending interrupt from the specified GPIO input.

  Note:
  The MSS_GPIO_clear_irq() function must be called as part of any GPIO
  interrupt service routine (ISR) in order to prevent the same interrupt event
  retriggering a call to the GPIO ISR.

  @param gpio
    The gpio parameter specifies the GPIO block that needs to be configured.

  @param port_id
    The port_id parameter identifies the GPIO port for which you want to clear
    the interrupt. An enumeration item of the form MSS_GPIO_n, where n is the
    number of the GPIO port, identifies the GPIO port. For example, MSS_GPIO_0
    identifies the first GPIO port and MSS_GPIO_31 identifies the last GPIO
    port.

  @return
    This function does not return a value.

  @example
    The following example demonstrates the use of the MSS_GPIO_clear_irq()
    function as part of the GPIO 9 interrupt service routine:
  @code
    uint8_t gpio2_non_direct_plic_IRQHandler(void)
    {
        uint32_t intr_num = 0;
        intr_num = MSS_GPIO_get_irq(GPIO2_LO);

        for(int cnt=0; cnt<32; cnt++)
        {
            if (1u == (intr_num & 0x00000001U))
            {
                MSS_GPIO_clear_irq(GPIO0_LO, (mss_gpio_id_t)cnt);
            }

            intr_num >>= 1u;
        }

        return EXT_IRQ_KEEP_ENABLED;
    }
  @endcode
 */
void MSS_GPIO_clear_irq
(
    GPIO_TypeDef * gpio,
    mss_gpio_id_t port_id
);

#ifdef __cplusplus
}
#endif

#endif /* MSS_GPIO_H_ */
