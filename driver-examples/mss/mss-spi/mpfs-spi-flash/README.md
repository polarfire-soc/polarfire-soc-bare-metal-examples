# PolarFire SoC SPI Flash example

This example project demonstrates the use of the PolarFire MSS SPI hardware
block. It reads and writes the content of an external SPI flash device. The 
PolarFire SoC MSS SPI1 is configured as a master and SPI Flash acts as a slave.
The **(Micron MT25Q)** device available on the Mikrobus click board "MT25QL01GBBB"
connected on the MikroBus socket on Icicle kit was used to test this example project.

# How to use this example

On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces
connected. To use this project configure the COM port **interface1** as below:

 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project is targeted to PolarFire SoC hardware platform. This is a self
contained example project. A greeting message is displayed over the UART terminal.

It configures the MSS SPI1 as master and the flash memory in the normal SPI mode
and does the write, read operations on it in that sequence. It then cross-checks
the content read from the memory with the contents that were written to it. A pass
or fail message is displayed as per the results.

This project provides build configurations and debug launchers as explained
[here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples)
