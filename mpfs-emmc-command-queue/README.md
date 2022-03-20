# mpfs-emmc-command-queue-rx-tx

This example project demonstrates the use of the PolarFire SoC MSS MMC block.
This example project demonstrates the use of the driver APIs for command queue operations.

## How to use this example

On connecting Icicle kit J11 to the host PC, you should see four COM port interfaces
connected. To use this project, configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity

The example project is self contained. It first enables the command queues on the
eMMC controller as well as the target eMMC device and then executes data transfers
on the eMMC device using command queues. Status messages are displayed on the UART terminal.

This example is tested on the PolarFire SoC Icicle kit with the latest released
[reference design](https://github.com/polarfire-soc/icicle-kit-reference-design/releases).

This project provides build configurations and debug launchers as explained [here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md).
Following changes are made to the default build configurations for this project:
 - LIM-Release: Uses mpfs-envm.ld
 - eNVM-Scratchpad-Release: Is not provided because LIM and scratchpad sizes as configured by reference design are not enough for this project.

The standard reference design is used to test this project. However, a modified xml under boards/icicle-kit-es/fpga_design/design_description is used, which disables the scratchpad and enables L2 as maximum size LIM.


