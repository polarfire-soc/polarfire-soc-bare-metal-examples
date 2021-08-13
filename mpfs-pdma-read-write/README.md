# PolarFire SoC MSS PDMA Driver example

This example project demonstrates the use of the PolarFire SoC MSS Platform
DMA driver to configure the DMA channel and initiate the transaction between
source and destination memory locations.
Success and error status in the transactions are reported by respective
interrupts.

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
To use this project, configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

This is a self contained example project. A greeting message is displayed
over the MMUART terminal indicating the  DMA channels for transaction.
User need to select the appropriate DMA channel(0 - 3) and the driver will
configure the selected channel. Once the channel is successfully configured,
transaction will initiate. Error or Success status of the transaction is
displayed over the UART terminal.
User can repeat the process to verify the transactions on different DMA channel.

This project provides build configurations and debug launchers as explained
[here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)
