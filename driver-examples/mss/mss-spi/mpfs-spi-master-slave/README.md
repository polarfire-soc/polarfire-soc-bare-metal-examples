# PolarFire SoC SPI Master Slave example

This example project demonstrates the use of the PolarFire SoC MSS SPI peripheral
device. It loops back communications internally between MSS SPI0 and MSS SPI1. 
In this example project SPI1 is configured in master mode while SPI0 is 
configured in slave mode. The data is then sent from master to slave and slave 
to master to demonstrate the functionality.

# How to use this example

The example project is targeted to PolarFire SoC hardware platform. The SPI1 is 
configured in master mode whereas SPI0 is the slave. The data is then 
transferred between SPI0 and SPI1 using internal loopback mechanism.
Run the example project using a debugger. 

On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces
connected. To use this project configure the COM port **interface1** as below:

 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal. Follow the

**NOTE**: This project can be used with the *SPI_LOOPBACK* varient of the [PolarFire 
SoC reference design](https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design)
*SPI_LOOPBACK* design will allow you to loopback on SPI with SPI0 routed to j26/(Rpi connector) 
and SPI1 routed to j44/j8( MikroBUS connector ).


| **SPI0 signal**  | **j26(Rpi connector) pin-no** | **SPI1 signal** | **j44(MikroBUS connector) pin-no**  |
|---------         |----------                     |------------     |  ---------                          |
|   CE             |  24                           |   CE            |   3                                 |
|   SCLK           |  23                           |   SCLK          |   4                                 | 
|   DI             |  21                           |   DI            |   5                                 |
|   DO             |  19                           |   DO            |   6                                 |


**NOTE**: In Release mode, Debugging level is set to default(-g) so that user 
can put the breakpoint and check the buffer.

This project provides build configurations and debug launchers as explained
[here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples).