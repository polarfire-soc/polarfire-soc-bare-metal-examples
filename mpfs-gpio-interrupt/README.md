# PolarFire SoC MSS GPIO example

This example project demonstrates the use of the PolarFire SoC MSS GPIO block. 
Both the input and output port configurations are demonstrated along with the 
interrupt handling for the input ports.

## How to use this example

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


This project provides build configurations and debug launchers as explained
[here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples).


 
