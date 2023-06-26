# PolarFire SoC MSS USB Mass storage Class Host Controller

This project demonstrates the use of the PolarFire SoC MSS USB driver stack in 
USB Host mode.

This example demonstrates the USB Mass storage class host functionality. MSS USB 
will act as a MSC class host which can read/write files from the Mass storage 
device (e.g., a pen drive) connected to the PolarFire SoC device through USB.

# File System support
This example uses the FatFs file system library. The supported features are 
configured for this project are listed below.

|                      |                                         |
|:---------------------|:----------------------------------------|
|'File systems'        | <ul><li>  FAT12, FAT16, FAT32.      </li>|
|'Sector size'         | <ul><li>  512                       </li>|
|'Logical Units'       | <ul><li>  1                         </li>|
|'File Names'          | <ul><li>  Short File Names          </li>|
|'Character code set'  | <ul><li>  ANSI/OEM                  </li>|
|'Partition'           | <ul><li>  1                         </li>|

To know more about the library configurations in this example, 
refer to .\FatFs\ffconf.h

To read more about the FatFs, refer - 
[Generic FAT Filesystem Module](http://elm-chan.org/fsw/ff/00index_e.html)

# How to use this project
To use this project, you will need a UART terminal configured as follows:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

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

This application uses the FatFs file system library to access the files on the 
targeted Flash drive. The application only needs to use the APIs provided by 
FatFS to access the flash drive. To achieve this, the discio.c (required by 
FatFs.) is implemented by this application. The discio.c internally uses the MSS 
USB Host Driver, which in turn accesses the connected flash drive.

This example initialises the MSS USB host driver in USB host mode. Initializes 
the USB MSC host class driver and registers it with the MSS USB host driver. As 
the FatFs application interface APIs are used, the corresponding MSS USB MSC 
class driver APIs will be invoked through discio.c  (the disc I/O interface).

Note:  It is preferred to connect a pre-formatted Flash drive to PolarFire SoC. 
If unformatted Flash drive is connected, f_mount API can be used to clean the 
Flash Drive. But this takes long time to complete(depending on the size of the 
Flash drive.). This long time might give a false appearance that the PolarFire 
SoC system is not work.

The UI appears on UART. The UI is self explanatory. Follow the steps as per the 
messages appearing on the console.

# File Read/Write performance
The speed of the file read/write operation depends on the application, FatFs and 
the MSS USB driver. For maximum file read/write performance the application 
should configure the FatFs to extract its maximum performance. To know more 
about the efficient configuration of FatFs, 
please refer: 
[FatFs configuration](http://elm-chan.org/fsw/ff/en/appnote.html#limits).

To achieve maximum performance, the application should also make sure that the 
data transfers should be sector aligned. One way of achieving this is by 
selecting the data buffer sizes and transfer sizes which are integer multiple of 
sector size i.e. 512 bytes.

The MSS USB driver supports multi packet Bulk transfers. The application should 
make sure that the transfer size is selected as the maximum transfer size 
supported by FatFs, to make sure that the maximum possible bandwidth with multi 
packet Bulk transfer is utilized fully.

The MSS USB driver supports Bulk transfers using the DMA inside MSS USB or 
without using the internal DMA. It is advised that the MSS USB driver is 
configured to use the internal DMA in order to free up the application from 
transferring data to/from MSS USB hardware block.
   
# Target hardware
This example project can be used on the PolarFire SoC model on PolarFire SoC 
FPGA family hardware platforms. 

## Executing project on PolarFire SoC hardware

The same application can be used on PolarFire SoC hardware platform. The MMUART1 
must be connected to host PC. The host PC must connect to the serial port using 
a terminal emulator such as Tera Term or PuTTY.

Build the project and launch the debug configuration named **mpfs-usb-host-msc 
hw all-harts debug.launch** which is configured for PolarFire SoC hardware 
platform.

Follow the instructions on MMUART1. Once correct device is connected to usb 
port, the Polar Fire Soc will recognise it and print the subsequent messages and 
following menu on the serial port.

Select Menu

   1. List Root-Directory elements

   2. Copy file

   3. Read file (First 512 bytes)

   4. Get Device Descriptor

   5. Suspend Host

Depending on the options chosen on the serial port The results will be as 
follows.

Option **1** will print all root directory files on the terminal if it is used.

If option **2** is chosen, the user can copy the file and paste it with a new 
name.

If option **3** is used, all root files will be listed, and after choosing a 
file number, the contents of that file will be printed on the terminal.

Option **4** will pri1nt the device descriptor on the terminal if it is chosen.

If option **5** is chosen, the host will remain suspended until "Enter" is 
pressed. Connect a pen drive with a led power indicator to check and verify this 
scenario. This led on the pendrive will turn off when the host is suspended 
until the terminal is restarted by pressing "Enter."

**NOTE** :- This application tested with using following pendrives:-

1. SanDisk 4.0 GB
2. Kingston 8.0 GB 
3. SandDisk 8.0 GB