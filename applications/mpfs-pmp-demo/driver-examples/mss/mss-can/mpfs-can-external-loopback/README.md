# PolarFire SoC MSS Full CAN external loop back example

This example project demonstrates using the MSS CAN peripheral to perform CAN
message transmission and reception using external loop back. The user need to 
connect the CAN_H and CAN_L as mentioned below in jumper setting section before 
running the example project.  

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
received into the rx buffers is displayed on the console.

The following macros modify the behaviour of the program:

    CAN_TX_EXTENDED_ID        - Defining this macro causes CAN messages with
                                with extended 29 bit IDs to be sent instead of
                                the standard 11 bit IDs.

Jumper settings:
Connect CAN-0 and CAN-1 as mentioned below:
  
  |   J25   |   J27   | Description  |
  |---------|---------|--------------|
  |   1     |   1     |  CAN_H       |
  |   2     |   2     |  CAN_L       |
  |   3     |   3     |  GND         |
      
## Test CAN Message Transmission
    1. Enter the data on hyperterminal, which will be received through MSSUART1.
    2. Based on received data bytes, segregate as CAN messages of maximum 8 
       bytes length.
    3. Send the received data in terms of CAN messages from CAN-0 to CAN-1
    4. Observe the received CAN messages on CAN-1
    5. Compare the data received on CAN-1 with the data sent from the 
       hyperterminal data should be same.
       
This project provides build configurations and debug launchers as exaplained 
[here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)

The design description file with this clock setting is available at ./src/boards/icicle-kit-es/fpga_design/design_description/ICICLE_MSS_CAN_8MHz.xml
This project can be tested with standard Libero reference design. However, please make sure that the input clock to MSS CAN block is set to 8MHz in xml.
