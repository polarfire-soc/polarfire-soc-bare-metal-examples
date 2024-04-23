# CoreQSPI Flash Example Project

This example project demonstrates the use of the CoreQSPI Soft IP in PolarFire
SOC. It performs write and read operations on an external QSPI flash memory
**(Micron MT25QL512)** device.

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces
connected. To use this project configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display a greeting message over the UART terminal. It
configures the QSPI flash controller and the QSPI flash memory in the normal SPI
mode and does the write, read operations on it in that sequence. It then
cross-checks the content read from the memory with the contents that were
written to it. A pass or fail message is displayed as per the results.

This program then does the same operations in dual and quad modes of operations.
The CoreQSPI driver can carry out the transfers using polling method or it
can do the same operations using interrupt. Whether to use interrupt or not is
decided by the application. In this example polling mode is used by default.
To enable interrupt mode, define a constant USE_QSPI_INTERRUPT in
src/platform/drivers/off_chip/micron_mt25/micron_mt25.c.

This project provides build configurations and debug launchers as explained
[here](https://mi-v-ecosystem.github.io/redirects/polarfire-soc-documentation-master_README)

This example is tested on the PolarFire SoC Icicle kit. The reference-design for
this example is not available in the following repository: [PolarFire® SoC Icicle Kit Reference Design](https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design).
