# PolarFire SoC MSS System Services example
This example project demonstrates the use of the PolarFire SoC MSS System 
service driver to execute the services supported by system controller.  

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces. 
To use this project, configure the COM port interface1 as below:
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
default is polling mode. 
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

For SPI copy service, an external flash memory is required in the hardware and 
the design must have the SPI connections to the external flash device.  

Match OTP service requires a proper validator input for it to execute 
successfully.

This project provides build configurations and debug launchers as exaplained 
[here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples)