================================================================================
                    PolarFire SoC MSS QSPI Flash example
================================================================================
This example project demonstrates the use of the PolarFire MSS QSPI hardware
block. It reads and writes the content of an external QSPI flash memory device.
All modes of operations including XIP (Execute In Place) are demonstrated.
--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
To use this project you will need a UART terminal configured as below:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control

This is a self contained example project. A greeting message and is displayed
over the UART terminal. It configures the QSPI flash controller and the
QSPI flash memory in the normal SPI mode and does the write, read operations
on it in that sequence. It then cross-checks the content read from the memory 
with the contents that were written to it. A pass or fail message is displayed 
as per the results.

This program then does the same operations in dual and quad modes of operations.
The MSS QSPI driver can carry out the transfers using polling method or it 
can do the same operations using interrupt. Whether to use interrupt or not is 
decided by the application. In this example polling mode is used by default. 
To enable interrupt mode, define a constant USE_QSPI_INTERRUPT in 
src/platform/drivers/micron_mt25/micron_mt25.c.

At the end this program will configure the the XIP mode and display the data. 
It will then exit the XIP mode and show the normal register access data.

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

## Executing project in the Renode emulation

Further information on working with Renode is available from the "Renode 
emulation platform" section of the SoftConsole release notes. The SoftConsole
release notes document can be found at: <SoftConsole-install-dir>/documentation

## Executing project on the PolarFire SoC hardware

This application can be used on PolarFire hardware platform as well e.g. Icicle 
Kit. In this case, the MMUART0 must be connected to a host PC. The host PC must 
connect to the serial port using a terminal emulator such as Tera Term or PuTTY 
or the SoftConsole built-in terminal view.

Build the project and launch the debug configuration named 
 mpfs-qspi-mt25q-flash hw all-harts debug.launch which is configured for 
PolarFire SoC hardware platform.