# PolarFire SoC MSS Watchdog interrupt example
This example project demonstrates the use of the PolarFire SoC MSS Watchdog. It 
demonstrates the watchdog configurations, the time-out interrupt and the Maximum 
Value up to which Refresh is Permitted (MVRP) interrupt and their handling.

## How to use this example
On connecting Icicle kit J11 connector to the host PC, you should see 4 serial COM port interfaces. 
To use this project, configure the COM port **interface0** as below:
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

# Build configurations
This project provides only one build configuration - eNVM-Scratchpad-Release.

The watchdog downcounters will not run when a debugger is attached, hence \*-Debug type
build configurations will not allow the functioality to complete.

This project needs to use the E51 monitor core to configure the watchdog0.
Hence this project can not be run when a Hart System Services (HSS) or similar
bootloader which are executing on E51 and keep running even after the U54 applications are launched. Note that the HSS provides watchdog services and the applications launched by HSS bootloader
can avail them. For more information refer [HSS watchdog services](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/hart-software-services/watchdog-service/watchdog-service.md).

For latest version of the HSS refer [HSS release page](https://github.com/polarfire-soc/hart-software-services/releases/tag/2021.04).