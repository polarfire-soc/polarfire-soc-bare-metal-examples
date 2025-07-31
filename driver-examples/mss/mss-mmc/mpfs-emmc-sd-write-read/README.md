# mpfs-emmc-sd-write-read

This example project demonstrates the use of the PolarFire SoC MSS MMC block. 
The MSS MMC supports eMMC, SD, and SDIO devices. This example project demonstrates
the use of the driver APIs for single block and multi-block transfers on the eMMC and SD devices.

## How to use this example

On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
On connecting Discovery kit J4 to the host PC, you should see 3 COM port interfaces connected.
To use this project, configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

A greeting message and menu instructions are displayed over the UART terminal. 
Follow the instruction and use different menu options provided by the example project.

The functionality implemented in the menu options depends on the Libero design that is programmed on the device.
For example, the SD card transactions will not work if the Libero design does not support it.

This example has been tested on the PolarFire SoC Icicle Kit and Discovery Kit using their respective latest released reference designs: [Icicle Kit Reference Design](https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design) and [Discovery Kit Reference Design](https://mi-v-ecosystem.github.io/redirects/repo-discovery-kit-reference-design).<br>For the Icicle Kit, both eMMC and SD card are supported. On the Discovery Kit, as eMMC is not available, only SD card transactions are supported.

This project provides build configurations and debug launchers as explained [here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples). 
Following changes are made to the default build configurations for this project:
 - LIM-Release: Uses mpfs-lim-lma-scratchpad-vma.ld
 - LIM-Debug: Uses mpfs-lim.ld
 - eNVM-Scratchpad-Release: Is not provided because LIM and scratchpad sizes as configured by reference design are not enough for this project.
 - DDR-Release: Uses mpfs-ddr-loaded-by-boot-loader.ld

The standard reference design is used to test this project. However a modified xml under boards/[BOARD]/fpga_design/design_description is used, which disables the scratchpad and enables L2 as maximum size LIM.