
#                         mpfs-hal-payload-example


This example project creates a program which is loaded by a boot-loader. 
The program runs in m-mode. See mss-config.h for configuration details.

You can debug the program using Renode.
It will run on hart1 only.

The program can be loaded and run using the mpfs-hal-ddr-demo example program.


#### Loading application to DDR

The application is configured to run from DDR. It must be loaded to DDR using a previous stage
program. The MPFS HAL ddr demo program is useful for this while developing.
The Hart Software Services (HSS) can be used once your program is developed.
The program uses MMUART1 to print out IP settings at startup. The host PC must 
connect to the serial port using a terminal emulator such as Tera Term or PuTTY 
or the SoftConsole built-in terminal view.
Please load the MPFS HAL DDR demo program to eNVM. This boots the Icicle kit
and prints information on UART0. 
From there you can use Ymodem to load this program to DDR ( Use the bin file located in 
the DDR-Release directory ) and start it running
using option 7 "Start U54_1 from DDR @0x80000000"
On UART 1 you should see the IP set-up displayed. 
You can ping this address and display web page once you connect to the board on 
Ethernet. ( Ethernet port on USB serial port side )

## To Debug the application in DDR on the Icicle kit

1. Load MPFS HAL ddr demo project to eNVM as above.
2. Load binary file using oprion 6 as above.
3. Connect using "mpfs-uart-mac-freertos-lwip-DDR-Release" debug configuration
4. Add any break-point you desire. e.g. Add one here "U54_1"
5. Press resume button on the debugger
6. Press option 7 on UART0. 
7. The program should halt at your added break-point.