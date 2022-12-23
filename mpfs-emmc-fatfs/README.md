# mpfs-emmc-fatfs

This example project demonstrates the use of the PolarFire SoC MSS MMC block. 
The MSS MMC supports eMMC, SD, and SDIO devices. This example project demonstrates
the use of FATFS APIs for file write and read on the eMMC device.

## How to use this example

On connecting Icicle kit J11 to the host PC, you should see four COM port interfaces 
connected. To use this project, configure the COM port **interfaces** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity

A greeting message and menu instructions are displayed over the UART terminal. 
Follow the instruction and use different menu options provided by the example project.

This example is tested on the PolarFire SoC Icicle kit with the v2022.08
[reference design](https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design).
Common design for eMMC and SD card.

Before running the example project, please make [Default Jumper Settings](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/boards/mpfs-icicle-kit-es/icicle-kit-user-guide/icicle-kit-user-guide.md#jumpers).
