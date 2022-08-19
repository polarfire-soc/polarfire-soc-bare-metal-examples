# PolarFire SoC MSS MMUART transmit/receive example

This example project demonstrates the use of the PolarFire SoC MSS MMUART to 
transmit and receive data using interrupt and polling method of operations.

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces
connected. To use this project configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal. Follow the
instruction to use different menu options provided by the example project.

This project provides build configurations and debug launchers as explained [here](https://mi-v-ecosystem.github.io/redirects/polarfire-soc-documentation-master_README)
