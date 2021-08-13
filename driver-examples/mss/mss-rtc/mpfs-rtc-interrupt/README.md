# PolarFire SoC MSS RTC interrupt example
This example project demonstrates the use of the PolarFire SoC MSS RTC driver to
configure the MSS RTC block to demonstrate RTC interrupt in binary mode. The
messages are displayed over the UART terminal at the occurrence of each interrupt.

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
To use this project, configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

This is a self contained example project. A greeting message is displayed over
the UART terminal. The example project takes care of configuring and initiating
the MSS RTC block. User can observe the UART messages displayed over the terminal
to identify the occurrence of periodic interrupt.

This project provides build configurations and debug launchers as explained
[here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)
