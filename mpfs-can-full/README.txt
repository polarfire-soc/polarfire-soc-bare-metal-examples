================================================================================
              PolarFire SoC MSS Full CAN example
================================================================================

This example project demonstrates using the MSS CAN peripheral to perform CAN
message transmission and reception. The MSS CAN driver has APIs for BasicCAN and
FullCAN Configurations. This project is configured for FullCAN communication.

The operation of the MSS CAN is controlled via a serial console.

--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
To use this project you will need a UART terminal configured as below:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control
    
This is a self contained example project. A greeting message and is displayed
mover the UART terminal. On startup, the example project requests the user to 
enter in data to send via the CAN Bus. You can enter up to 32 pairs of hex 
digits (no separating spaces) and the data will be sent out in chunks of 8 
bytes at a time in up to 4 CAN packets. You can send less than 32 bytes of 
data by pressing return to terminate the data early.
    
The test program then enters a loop looking for user input to select the next
action to perform. Whilst in this loop, the data portion of any CAN Bus packets
received into the rx buffers is displayed on the console. The following menu
options are available:

    0 - Perform a hardware reset of the MSS CAN peripheral via SYSREG. This 
        shuts down all CAN communications.
    5 - Reinitailze the MSS CAN peripheral then get data from user and send via
        CAN Bus.
    7 - Get data from user and send via CAN Bus.

The following macros modify the behaviour of the program:

    CAN_TX_EXTENDED_ID        - Defining this macro causes CAN messages with
                                with extended 29 bit IDs to be sent instead of
                                the standard 11 bit IDs.
    CAN_TARGET_COCO_PC_ACTIVE - Defining this macro adjusts the baud rate to
                                enable reliable operation with the Kromschroder
                                CoCo PC Active CAN Bus interface.

--------------------------------------------------------------------------------
                        Test CAN Message Transmission
--------------------------------------------------------------------------------

    1. Enter the data on UART terminal, which will be received through MSSUART0.
    2. Based on received data bytes, segregate as CAN messages of maximum 8 
       bytes length.
    3. Send the received data in terms of CAN messages.
    4. Observe the CAN messages on CAN Analyzer with message identifier as 0x78.
    5. Compare the data received on CAN Analyzer with the data sent from the 
       UART terminal data should be same.

--------------------------------------------------------------------------------
                        Test CAN Message Reception
--------------------------------------------------------------------------------

    1. Send the 8 bytes of CAN message from CAN Analyzer with message identifier
        as 0x80.
    2. Read the data using CAN APIs and store it in to RAM buffer.
    3. Transmit the data using MSSUART0 on to UART terminal.
    4. Observe the data received on UART terminal.
    5. Compare the data sent from CAN Analyzer with the data received on 
       UART terminal data should be same.
  
--------------------------------------------------------------------------------
                                Target hardware
--------------------------------------------------------------------------------
This example project can be used on the PolarFire SoC FPGA family hardware 
platforms. 

There are configurations that need to be set for this example project. The
configurations are categorized into hardware and software configurations.
The hardware configurations are located in ./src/board/<target_board> folder.
The default software configurations are stored under 
.src/platform/platform_config_reference folder.
If you need to change these configurations, you are advised to create a new 
folder to replicate this folder under the ./boards directory and do the 
modifications there. It would look like 
./src/boards/<target_board>/platform_config

The include files in the "soc_config" folder define the hardware configurations 
such as clocks. You must make sure that the configurations in this example 
project match the actual configurations of your target design that you are using 
to test this example project.

The include files in the "mpfs_hal_config" folder define the software 
configurations such as how many HARTs are being used in the software, what is 
the tick rate of the internal timer of each HART. These configurations have no 
dependency on the hardware configurations "soc_config". Note that changing these 
software configurations may require a change in your application code.

## Executing project in the Renode emulation

Further information on working with Renode is available from the "Renode 
emulation platform" section of the SoftConsole release notes. The SoftConsole
release notes document can be found at: <SoftConsole-install-dir>/documentation

## Executing project on the PolarFire SoC

This application can be used on PolarFire hardware platform as well e.g. Icicle 
Kit. In this case, the MMUART0 must be connected to the host PC. The host PC 
must connect to the serial port using a terminal emulator such as Tera Term or 
PuTTY or the SoftConsole built-in terminal view.

Build the project and launch the debug configuration named 
mpfs-full-can hw all-harts debug.launch which is configured for 
PolarFire SoC hardware platform.

