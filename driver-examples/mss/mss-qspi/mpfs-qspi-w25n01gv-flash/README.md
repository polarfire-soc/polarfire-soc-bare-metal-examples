# PolarFire SoC MSS QSPI Flash example
This example project demonstrates the use of the PolarFire MSS QSPI hardware
block. It performs write and read operations on an external QSPI flash memory
**(Winbond w25n01gv)** device.
The example is tested with normal mode, dual full and quad full mode and also
extended ro and extended rw modes.

**NOTE**: To test this example, the Mikroe Flash 5 click board must be attached
          to the PolarFire SoC Icicle Kit's RPi header using Pi 3 click daughter board.
          out of 2 Mikro bus slots on the Pi 3 click board, the Flash 5 click is
          connected to slot 1.

## How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
To use this project, configure the COM port interface1 as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

A list of menu options are provided over the UART terminal. Choose the options to perform particular actions. The options are self explainatory. 
This project provides build configurations and debug launchers as exaplained
[here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples)


### Testing done:
 - Both Polling mode and Interrupt mode are supported.
 - All operating modes(Normal, Dual and Quad) are supported.
 - Complete memory range of 128MBytes is accessed.
 - BUF=1 and ECC is enabled
 - Bad block management APIs are implemented. When using a unused device, makes sure that you scan and record the bad block before erasing or programming the device
 - 2022.08 reference desig is used for testing
 - Make sure that J47 is closed to provide power supply to the PMOD adaptor card. DO NOT CONNECT external power supply to ADAPTRO card.
 
 ## Limitations:
 - MSS QSPI controller provides 3 byte addressing in the XIP mode. The memory map of MSS QSPI also shows max 24bit address space.
   MSS QSPI provides ADDRUP register to store the bit[31:24] of the address, but they dont get transmitted on the QSPI bus. 
   And if we have to update the ADDRUP register while executing in XIP mode, then the code needs to be aware about this address change requirement.
   In XIP mode the MSS QSPI always transfers 3 address bytes, 3 bytes idle and reads 4 data bytes.
   Effectively, only first 16MBytes of the Flash can be used for XIP.
    
 - The MSS QSPI DOES NOT directly support extended RO operation for SPI write commands where the address is 
   transmitted serially and data is transmitted by the core in BI/QUAD mode to the SPI flash memory. This is not an big issue for flash memories though since quad full(fastest) mode works.
