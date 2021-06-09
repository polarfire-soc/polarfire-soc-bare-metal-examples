================================================================================
                 PolarFire SoC MSS I2C sensor interface example
================================================================================

This example project demonstrates reading voltage and current data from PAC1934
sensor on the Icicle kit using I2C interface.

--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
To use this project you will nee a UART terminal configured as below:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control

This project interfaces with the PAC1934 sensor on the Icicle kit over I2C and 
displays the voltage and current values.

--------------------------------------------------------------------------------
                                Target hardware
--------------------------------------------------------------------------------
This example project is targeted for the PolarFire SoC Icicle kit. 

There are configurations that need to be set for this example project. The
configurations are categorized into hardware and software configurations.
The hardware configurations are located in ./src/boards/<target_board> folder.
The default software configurations are stored under 
.src/platform/platform_config_reference folder.

The include files in the "./src/boards/<target_board>/fpga_config" folder define 
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
dependency on the hardware configurations in "fpga_config" folder. Note that 
changing these software configurations may require a change in your application 
code.

## Executing project in the Renode emulation

This project is currently not supported on the Renode model 

## Executing project on the PolarFire SoC hardware (Icicle Kit)

In this case, the MMUART0 must be connected to a host PC. The host PC must 
connect to the serial port using a terminal emulator such as Tera Term or PuTTY
or the SoftConsole built-in terminal view.

Build the project and launch the debug configuration named 
mpfs-i2c-pac1934-sensor hw all-harts debug.launch which is configured for 
PolarFire SoC hardware platform.
