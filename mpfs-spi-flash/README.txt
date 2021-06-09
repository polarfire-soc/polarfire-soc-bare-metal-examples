================================================================================
                    PolarFire SoC SPI Flash example
================================================================================
This example project demonstrates the use of the PolarFire MSS SPI hardware
block. It reads and writes the content of an external SPI flash device. The 
PolarFire SoC is configured as a master and SPI Flash acts as a slave for this 
particular example project.
--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
The example project is targeted to PolarFire SoC hardware platform. The SPI0 is 
configured in master mode whereas SPI flash is the slave. The data is then 
transferred between SPI0 and SPI Flash.
Run the example project using a debugger. Place watches on buffers
g_flash_wr_buf and g_flash_rd_buf. You will then be able to observe the content
of g_flash_wr_buf being written into external flash and read back into the
g_flash_rd_buf buffer.

NOTE: In Release mode, Debugging level is set to default(-g) so that user 
can put the breakpoint and check the buffer.
--------------------------------------------------------------------------------
                                Target hardware
--------------------------------------------------------------------------------
This example project can be used on the PolarFire SoC FPGA family hardware 
platforms. 

There are configurations that need to be set for this example project. The
configurations are categorized into hardware and software configurations.
The hardware configurations are located in ./src/boards/<target_board> folder.
The default software configurations are stored under 
.src/platform/platform_config_reference folder.

The include files in the "./src/boards/<target_board>/soc_config" folder define 
the hardware configurations such as clocks. You must make sure that the 
configurations in this example project match the actual configurations of your 
target Libero design that you are using to test this example project.

If you need to change the software configurations, you are advised to create a 
new folder to replicate this folder under the ./src/boards directory and do the 
modifications there. It would look like 
./src/boards/<target_board>/platform_config

The include files in the "platform_config" folder define the software 
configurations such as how many harts are being used in the software, what is 
the tick rate of the internal timer of each hart. These configurations have no 
dependency on the hardware configurations in "soc_config" folder. Note that 
changing these software configurations may require a change in your application 
code.


## Executing project on the PolarFire SoC hardware

Build the project and launch the debug configuration named 
mpfs-spi-flash hw all-harts debug.launch which is configured for the 
PolarFire SoC hardware platform.
