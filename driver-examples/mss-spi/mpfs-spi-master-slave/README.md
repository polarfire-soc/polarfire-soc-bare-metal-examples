# PolarFire SoC SPI Master Slave example

This example project demonstrates the use of the PolarFire SoC MSS SPI peripheral
device. It loops back communications internally between MSS SPI0 and MSS SPI1. 
In this example project SPI0 is configured in master mode while SPI1 is 
configured in slave mode. The data is then sent from master to slave and slave 
to master to demonstrate the functionality.

# How to use this example

The example project is targeted to PolarFire SoC hardware platform. The SPI0 is 
configured in master mode whereas SPI1 is the slave. The data is then 
transferred between SPI0 and SPI1 using internal loopback mechanism.
Run the example project using a debugger. Place watches on the following buffers
buffers and observe the contents of the master and slave buffers being
exchanged.

g_master_tx_buffer

g_master_rx_buffer

g_slave_tx_buffer

g_slave_rx_buffer

**NOTE**: This project can not be used with the presently released version of the standard reference Libero design.
Please make sure that you use correct Libero design and MSS configurations XML file. The TCL script for the same is available under ./src/boards/icicle-kit-es/design_description/libero_tcl/
folder. Refer  [this]( https://github.com/polarfire-soc/icicle-kit-reference-design#emmc-sd ) to get more information on how to generate a .job file from a libero_tcl file.
and program the board. Use SPI_LOOPBACK as the parameter to generate the appropriate design to test this project.
*SPI_LOOPBACK* design will allow you to loopback on SPI with SPI0 routed to j26/(Rpi connector) and SPI1 routed to j44/j8( MikroBUS connector ).


| **SPI0 signal**  | **j26(Rpi connector) pin-no** | **SPI1 signal** | **j44(MikroBUS connector) pin-no**  |
|---------         |----------                     |------------     |  ---------                          |
|   CE             |  24                           |   CE            |   3                                 |
|   MOSI           |  19                           |   MOSI          |   6                                 |
|   MISO           |  21                           |   MISO          |   5                                 |
|   SCLK           |  23                           |   SCLK          |   4                                 | 

**NOTE**: In Release mode, Debugging level is set to default(-g) so that user 
can put the breakpoint and check the buffer.

This project provides build configurations and debug launchers as explained [here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)
