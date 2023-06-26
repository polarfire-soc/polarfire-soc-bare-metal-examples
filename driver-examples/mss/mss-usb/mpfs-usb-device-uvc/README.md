# PolarFire SoC MSS USB Video Class example 

This example project demonstrates PolarFire SoC MSS USB as a USB camera device. 
The example project will send the still image from device to Host. Any webcam 
application (ex. vlc player) can be used to view still image.
## IMPORTANT ##
Note that this project currently working with DDR-Release build only. 

________________________________________________________________________________
## Device Description:
________________________________________________________________________________
There are configurations that need to be set for this example project. The
configurations are categorized into hardware and software configurations. The 
hardware configurations are located in ./src/board/<target_board> folder. The 
default software configurations are stored under 
.src/platform/platform_config_reference folder.  
If you need to change these configurations, you are advised to create a new 
folder to replicate this folder under the ./boards directory and do the 
modifications there. It would look like 
./src/boards/<target_board>/platform_config

The include files in the "mpfs_hal_config" folder define the software 
configurations such as how many HARTs are being used in the software, what is 
the tick rate of the internal timer of each HART. Note that changing these 
software configurations may require a change in your application code.

________________________________________________________________________________
## Target hardware
________________________________________________________________________________
This example project can be used on the PolarFire SoC model on PolarFire SoC 
FPGA family hardware platforms.


### UART terminal configuration
On connecting Icicle kit J11 to the host PC, you should see 4 COM port 
interfaces connected. To use this project configure the COM port **interface0** 
and **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal.


________________________________________________________________________________
## How to use this example
________________________________________________________________________________
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

Follow the steps below to run this example project:-
1. Program the Icicle kit in boot mode 1 using the mpfs-hal-ddr-demo project 
image. 
    - Use envm-scratchpad (Icicle-Kit-eNVM-Scratchpad-Release) build 
    configuration. Use the following link for mpfs-hal-ddr-demo project:- 
    [mpfs-hal-ddr-demo](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/tree/main/driver-examples/mss/mpfs-hal/mpfs-hal-ddr-demo)  
    - mpfs-hal-ddr-demo project supports only 256KB(1024 * 128 * 2) for
    YMODEM data transfer. So, if .bin file size is bigger than 256KB, please do
    following modification in mpfs-hal-ddr-demo project. mpfs-usb-device-uvc 
    example project is greater then >256KB, So following changes are required 
    to be made. 
        - Open e51.c file and change g_rx_size variable size more than your .bin 
        file size. Save it and then follow the above mentioned steps. For 
        example, In this UVC example project. Size of .bin file is 621KB. So,
        size of g_rx_size variable changed to 640KB(1024 * 128 * 5).
2. Build DDR-Release for this uvc project.
3. Open UART interface 0 and interface 1, and make sure baudrate and other  
setting are similar as mentioned above.
4. Restart board.
5. Select option 6(Load image to DDR using YMODEM) from menu appear on UART 
interface 0 to flash .bin file of DDR-Release of uvc project using YMODEM. 
6. mss-usb-device-uvc application instructions are visible on UART interface 1.  
7. Now press key 7 to Start U54_1 from DDR, this will start running U54_1 core 
application. If required to capture USB packets using any USB analyzer. Trigger 
should be initiated before this step.
8. Open any webcam application. (For example, the VLC media player as webcam 
application).
9. Go to following path in VLC " Media -> Open capture Device -> Capture Device "
10. In 'Device Selection' section select 'Video device name' as 
"PolarFire Soc - camera".
11. Click on 'play' button in VLC. after 2-3 seconds, Still image can be viewed 
in VLC media player. 
