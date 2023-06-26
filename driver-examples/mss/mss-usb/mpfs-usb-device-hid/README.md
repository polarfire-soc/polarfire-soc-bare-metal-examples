# PolarFire SoC MSS USB Human Interface Device example 

This project demonstrates the use of MSS USB driver stack to easily implement a 
human interface device. When connected to a USB host, This feature automatically 
moves the mouse pointer horizontally(x-direction) on the desktop to which this 
USB port is connected.

This example project and the involved drivers passes the USB-IF USB2.0 
compliance test.

## IMPORTANT
Note that the mouse pointer will keep moving in horizontal direction back and 
forth till USB port is connected to host device.

# Device Description:



|                            |                                      |
| :------------------------- | :----------------------------------- |
| `HID Class`                | <ul><li> Has one HID class interface at its default      configuration. </li> |
| `HS`                       | <ul><li> operates at high speed with high-speed capable host. When connected to 1.x host or connected through 1.x hub operates at FS automatically. </li> |
| `VID`                      | <ul><li> 0x1514 (Actel Corp.)                          </li> |
| `PID`                      | <ul><li> 0x0001 (Fake)                                 </li> |
| `bcdUSB`                   | <ul><li> 0x0002 (represents USB2.0)                    </li> |
| `bcdDevice`                | <ul><li> 0x0030 (Fake)                                 </li> |
| `iserialNumber`            | <ul><li> HID1234 (Fake)                                </li> |
| `Remote wakeup`            | <ul><li> not supported                                 </li> |
| `Power`                    | <ul><li> Self powered (Does not use USB bus power)     </li> |
| `Device class Code`        | <ul><li> 0x03 (HID class)                              </li> |
| `Device SubClass Code`     | <ul><li> 0x00                                          </li> |
| `Device Protocol Code`     | <ul><li> 0x02                                          </li> |
| `Peripheral Device type`   | <ul><li> 0x00 (PDT)                                    </li> |


# Target hardware

This example project can be used on the PolarFire SoC model on PolarFire SoC 
FPGA family hardware platforms.

Executing project on PolarFire SoC hardware.

## How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port 
interfaces connected. To use this project configure the COM port **interface1** 
as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal.

Before compiling, please ensure that you follow the steps provided below (which 
are already done in this example project) :-  
(1) Copy the "mss_usb_sw_config.h" file from the [platform repository](https://github.com/polarfire-soc/platform/tree/main/platform_config_reference/drivers_config/mss/mss_usb) 
and paste it into the board repository 
(*boards\icicle-kit-es\platform_config\drivers_config\mss\mss_usb* and 
*boards\icicle-kit-es\platform_config_release\drivers_config\mss\mss_usb*). 
Then, enable the corresponding USB mode (ex. host and device mode).  
(2) In the USB example projects, please add the include path 
**#include "drivers_config/mss/mss_usb/mss_usb_sw_config.h"** in the 
mss_sw_config file under the boards section (*boards\icicle-kit-es\platform_config\mpfs_hal_config* and *boards\icicle-kit-es\platform_config_release\mpfs_hal_config*).

Build the project and launch the debug configuration named mpfs-usb-device-hid
hw all-harts debug.launch which is configured for PolarFire SoC hardware 
platform.

