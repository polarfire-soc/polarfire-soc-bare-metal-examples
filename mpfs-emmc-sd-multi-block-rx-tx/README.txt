================================================================================
    PolarFire SoC MSS eMMC SD single/multiple blocks read and write application
================================================================================

This example project demonstrates the use of PolarFire SoC MSS eMMC SD.
It reads and writes single/multiple block of data from/to the eMMC/SD
device based on BUFFER_SIZE defined in e51.c. The BUFFER_SIZE is 512 for single
block read/write and multiple of 512 bytes for multiple blocks read/write.
It is targeted PolarFire SoC FPGA family hardware platform.

The following project macros(defined in project settings) are used to configure
the system:

MMC_CARD - Define this to test eMMC card - default
SD_CARD  - Define this to test SD card
MSS_MMC_ADMA2 - Define this to test data transfer of eMMC/SD with ADMA2,
                if not define, default data transfer of eMMC/SD is SDMA.

--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
Run the example project using a debugger. Place watches on buffers
g_mmc_tx_buff and g_mmc_rx_buff. You will then be able to observe the content
of g_mmc_tx_buff being written into the eMMC/SD device and read back into the
g_mmc_rx_buff buffer.

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

## Executing project on PolarFire SoC hardware

This application can be used on PolarFire hardware platform as well e.g. Icicle 
Kit.

Build the project and launch the debug configuration named 
mpfs-emmc-sd-multi-block-rx-tx hw all-harts debug.launch which is configured for 
PolarFire SoC hardware platform.
