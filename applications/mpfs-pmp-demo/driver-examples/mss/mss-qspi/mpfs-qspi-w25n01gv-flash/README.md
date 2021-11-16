# PolarFire SoC MSS QSPI Flash example
This example project demonstrates the use of the PolarFire MSS QSPI hardware
block. It performs write and read operations on an external QSPI flash memory
**(Winbond w25n01gv)** device.
The example is tested with normal mode, dual full and quad full mode as of now.

**NOTE**: To test this example, the Mikroe Flash 5 click board must be attached
          to the PolarFire SoC Icicle Kit MikroBus port.

## How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
To use this project, configure the COM port interface1 as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

This is a self contained example project. A greeting message is displayed
over the UART terminal. It configures the QSPI flash controller and the
QSPI flash memory in the normal SPI mode and does the write, read operations
on it in that sequence. It then cross-checks the content read from the memory
with the contents that were written to it. A pass or fail message is displayed
as per the results.

This program then does the same operations in dual and quad modes of operations.
This example is configured to use MSS QSPI driver in  Polling mode.

**NOTE:** Interrupt method of data transfers and the XIP modes for the **Winbond w25n01gv** flash are not supported in this release.

This project provides build configurations and debug launchers as explained
[here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md).
