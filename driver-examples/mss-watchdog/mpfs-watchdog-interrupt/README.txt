================================================================================
                    PolarFire SoC MSS Watchdog interrupt example
================================================================================

This example project demonstrates the use of the PolarFire SoC MSS Watchdog. It 
demonstrates the watchdog configurations, the time-out interrupt and the Maximum 
Value up to which Refresh is Permitted (MVRP) interrupt and their handling.

--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
To use this project you will nee a UART terminal configured as below:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control

The example project will display instructions over the serial port. A greeting
message is displayed on the UART terminal. Watchdog0, 1 and 2 are configured to 
generate MVRP and time-out interrupt at ~13sec and ~25sec after system reset 
respectively. Messages are displayed on the UART terminal as and when the events
happen. Finally the system will reset when the watchdog0 down counter reaches
zero value.

--------------------------------------------------------------------------------
                                Target hardware
--------------------------------------------------------------------------------
This example project can be used on the PolarFire SoC model on PolarFire SoC 
FPGA family hardware platforms. 

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
mpfs-watchdog-interrupt hw all-harts debug.launch which is configured for 
PolarFire SoC hardware platform.
