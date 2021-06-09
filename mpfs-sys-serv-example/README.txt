================================================================================
                    PolarFire SoC MSS System Services example
================================================================================
This example project demonstrates the use of the PolarFire SoC MSS System 
service driver to execute the services supported by system controller.  

--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
To use this project you will need a UART terminal configured as below:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control

The PolarFire SoC MSS system service driver can be configured to execute the 
service in interrupt mode or polling mode. Enable the macro 
MSS_SYS_INTERRUPT_MODE in e51.c to execute the service in interrupt mode. 
User need to select the mode of operation by configuring the driver with 
appropriate service mode macros. If application does not select any mode, 
dafault is polling mode. 
The application will display the list of the services implemented in this
example. User can select the desired service for execution from the list 
displayed on UART terminal.
Every service will respond with SUCCESS or ERROR message, which can be observed 
on the UART terminal. 

Note: 
This application demonstrates the execution of some of the system services. 
Most of the services require some infrastructure in the design for successful
execution. If the required infrastructure is not available, service will not
execute or return error code.

For example:
To execute usercode service, the usercode must be programmed in the design. 

To execute device certificate service, the certificate has to be stored
in device pNVM.

For SPI copy service, an external flash memory is required in the hardware and 
the design must have the SPI connections to the external flash device.  

Match OTP service requires a proper validator input for it to execute 
successfully.
--------------------------------------------------------------------------------
                                Target hardware
--------------------------------------------------------------------------------
This example project is targeted PolarFire SoC FPGA family hardware platforms
with MSS MMUART enabled .

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

This application can be used on PolarFire hardware platform as well e.g. Icicle 
Kit. In this case, the MMUART0 must be connected to a host PC. The host PC must 
connect to the serial port using a terminal emulator such as Tera Term or PuTTY 
or the SoftConsole built-in terminal view.

Build the project and launch the debug configuration named 
mpfs-sys-serv-example hw all-harts debug.launch which is configured for the
PolarFire SoC hardware platform.
