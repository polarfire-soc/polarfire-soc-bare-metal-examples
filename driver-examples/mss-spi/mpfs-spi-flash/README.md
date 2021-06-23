# PolarFire SoC SPI Flash example

This example project demonstrates the use of the PolarFire MSS SPI hardware
block. It reads and writes the content of an external SPI flash device. The 
PolarFire SoC is configured as a master and SPI Flash acts as a slave for this 
particular example project.

# How to use this example

**NOTE:** This project can not be directly used on the Icicle kit since the flash memory device is not accessible via SPI.This example project is provided as reference to demonstrate how to use the MSS SPI driver for accessing SPI Flash. 

The example project is targeted to PolarFire SoC hardware platform. The SPI0 is 
configured in master mode whereas SPI flash is the slave. The data is then 
transferred between SPI0 and SPI Flash.
Run the example project using a debugger. Place watches on buffers
g_flash_wr_buf and g_flash_rd_buf. You will then be able to observe the content
of g_flash_wr_buf being written into external flash and read back into the
g_flash_rd_buf buffer.

The tcl scripts for generating libero design is available under /src/boards/icicle-kit-es/fpga_design/liber_tcl/ folder.

This project provides build configurations and debug launchers as explained [here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)
