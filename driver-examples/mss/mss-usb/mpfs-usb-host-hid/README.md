# PolarFire SoC MSS USB Human Interface Devices Host Controller

This project demonstrates the use of PolarFire SoC MSS USB driver stack in USB 
Host mode.

This example emulates the USB Human Interface Devices class host functionality. 
MSS USB will act as a HID class host which read the HID device report 
periodically to check whether new data is present in the HID device (e.g. mouse)
connected to PolarFire SoC device through USB.

Note: This example project was tested and verified using Renode emulation only. 

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

# Executing project in the Renode emulation

To launch the PolarFire SoC board emulation model on Renode from SoftConsole, 
launch the preconfigured external tool from
   Run -> External Tools -> "mpfs-usb-host-hid renode-emulation-platform". 

This will also launch an UART terminal which works with Renode.

Build the project and launch the debug configuration named 
mpfs-mss-usb-host-hid Renode Debug.launch which is configured for Renode. 

For more information, please refer to the 'Working_with_Renode.md' file located 
in the root folder of this project.
