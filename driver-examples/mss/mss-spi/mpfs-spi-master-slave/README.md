# PolarFire® SoC SPI Master Slave example

This example project demonstrates the use of the PolarFire SoC MSS SPI
peripheral device. It loops back communications internally between MSS SPI0 and
MSS SPI1. In this example project SPI1 is configured in master mode while SPI0
is configured in slave mode. The data is then sent from master to slave and
slave to master to demonstrate the functionality.
# How to use this example

The example project is targeted to PolarFire SoC hardware platform. The SPI1 is
configured in master mode whereas SPI0 is the slave. The data is then
transferred between SPI0 and SPI1 using internal loopback mechanism. Run the
example project using a debugger.

On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
On connecting Discovery kit J4 to the host PC, you should see 3 COM port interfaces.
To use this project, configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control


The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal.

**NOTE**: For the Icicle Kit, This project should be used with the *SPI_LOOPBACK* variant of the
[Icicle kit reference design](https://mi-v-ecosystem.github.io/redirects/releases-icicle-kit-reference-design). The *SPI_LOOPBACK* design will allow you
to loopback on SPI with SPI0 routed to j26/(Rpi connector) and SPI1 routed to
j44/j8(MikroBUS connector).
For the Discovery Kit, This project should be used with the *Default* variant of the [Discovery kit reference design](https://mi-v-ecosystem.github.io/redirects/repo-discovery-kit-reference-design). This will allow you
to loopback on SPI with SPI0 routed to J5/J6(MikroBUS connector) and SPI1 routed to
J10(Rpi connector).

For **Icicle Kit**, The J26(Rpi connector) and J44/J8(MikroBUS connector) should be connected to
each other according to the following table:

| **SPI0 signal**  | **J26(Rpi connector) pin-no** | **SPI1 signal** | **J44(MikroBUS connector) pin-no**  |
|---------         |----------                     |------------     |  ---------                          |
|   CE             |  24                           |   CE            |   3                                 |
|   SCLK           |  23                           |   SCLK          |   4                                 | 
|   DI             |  21                           |   DI            |   5                                 |
|   DO             |  19                           |   DO            |   6                                 |

For **Discovery Kit**, The J10(Rpi connector) and J5/J6(MikroBUS connector) should be connected to
each other according to the following table:

| **SPI0 signal**  | **J10(Rpi connector) pin-no** | **SPI1 signal** | **J5(MikroBUS connector) pin-no**   |
|---------         |----------                     |------------     |  ---------                          |
|   CE             |  24                           |   CE            |   3                                 |
|   SCLK           |  23                           |   SCLK          |   4                                 | 
|   DI             |  21                           |   DO            |   6                                 |
|   DO             |  19                           |   DI            |   5                                 |

**NOTE**: In Release mode, Debugging level is set to default(-g) so that user
can put the breakpoint and check the buffer.

This project provides build configurations and debug launchers as explained [here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples).