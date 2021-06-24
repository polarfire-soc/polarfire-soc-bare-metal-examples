# PolarFire SoC MSS Basic CAN example

This example project demonstrates using the MSS CAN peripheral to perform CAN
message transmission and reception. The MSS CAN driver has APIs for BasicCAN and
FullCAN Configurations. This project is configured for BasicCAN communication.

The operation of the MSS CAN is controlled via a serial console.

## How to use this example

On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces. 
To use this project, configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control
    
This is a self contained example project. A greeting message is displayed
over the UART terminal. On startup, the example project requests the user to 
enter the data to be send via the CAN Bus. You can enter up to 32 pairs of hex 
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
                                
Note: Because the BasicCAN example program provides an ISR for the CAN_IRQ, the
      following macros need to be defined in the project settings to ensure
      interrupts are fully enabled and the default ISR in mss_can.c is disabled:
      
          MSS_CAN_ENABLE_INTERRUPTS
          MSS_CAN_USER_ISR

Jumper settings:
Connect CAN-0 and PCAN as mentioned below:
  
  |   CAN-0 J27   |   PCAN   | Description  |
  |---------------|--------- |--------------|
  |   1           |   7      |  CAN_H       |
  |   2           |   2      |  CAN_L       |
  |   3           |   6      |  GND         |
  
## Test CAN Message Transmission
    1. Enter the data on UART terminal, which will be received through MSSUART1.
    2. Based on received data bytes, segregate as CAN messages of maximum 8 
       bytes length.
    3. Send the received data in terms of CAN messages.
    4. Observe the CAN messages on CAN Analyzer with message identifier as 0x78.
    5. Compare the data received on CAN Analyzer with the data sent from the 
       UART terminal data should be same.

## Test CAN Message Reception
    1. Send the 8 bytes of CAN message from CAN Analyzer with message identifier
        as 0x200.
    2. Read the data using CAN APIs and store it in to RAM buffer.
    3. Transmit the data using MSSUART1 on to UART terminal.
    4. Observe the data received on UART terminal.
    5. Compare the data sent from CAN Analyzer with the data received on 
       UART terminal data should be same.
  
This project provides build configurations and debug launchers as exaplained 
[here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)

The design description file with this clock setting is available at ./src/boards/icicle-kit-es/fpga_design/design_description/ICICLE_MSS_CAN_8MHz.xml
This project can be tested with standard Libero reference design. However, please make sure that the input clock to MSS CAN block is set to 8MHz in xml.
