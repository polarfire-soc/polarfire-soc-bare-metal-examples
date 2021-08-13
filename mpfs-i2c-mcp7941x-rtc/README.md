# PolarFire SoC MSS I2C example

This example project demonstrates the use of MCP7941x RTC i.e RTC6 click board
over I2C0 which routes to the polarfire SOC fabric interface and icicle kit
mikroBUS connector.
Here we demonstrate how to use the various APIs provided by the MCP7941x
Off-chip driver. For more documentation please refer to MCP7941x.h file

[ click here for MCP7941x RTC6 datasheet ]( http://ww1.microchip.com/downloads/en/devicedoc/20002266h.pdf )

## How to use this example

On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces connected. To use this project configure the COM port interface1 as below:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity

The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal. Follow the
instruction to use different menu options provided by the example project.

**Note** -: Application uses mmuart1 as main UART for user interaction, Open the
serial port corresponding to the mmuart1 i.e(interface1). 

This project provides build configurations and debug launchers as explained [here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)
