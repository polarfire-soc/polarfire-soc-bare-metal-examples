# PolarFire SoC MSS I2C Master/Slave example

This example project demonstrates the use of the PolarFire SoC MSS I2C peripherals using external Loop back. It demonstrates reading and writing data between a pair of MSS I2Cs configured as a master and a slave.

The operation of the MSS I2Cs is controlled via a serial console.

## How to use this example

Connect the IO pins for the SDA and SCL of the two MSS I2Cs together.
       I2C-0                  I2C-1
I2C 0 SCL: J26 Pin 29 -> I2C 1 SCL: J26 Pin 31
I2C 0 SDA: J26 Pin 35 -> I2C 1 SDA: J26 Pin 37

On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces. 
To use this project, configure the COM port interface1 as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal. Follow the
instruction to use different menu options provided by the example project.

Use menu option 1 to write between 0 and 32 bytes of data from the master I2C
device to the slave I2C device. Any data received by the slave is written to
the slave_tx_buffer[] array and overwrites some or all of the default contents 
- "<<-------Slave Tx data ------->>". 0 byte transfers are allowed with this
option but not with option 3 below.

Use menu option 2 to read the 32 bytes of data from the Slave I2C 
slave_tx_buffer[] via I2C and display it on the console.

Use menu option 3 to write between 1 and 32 bytes of data to the slave and read
it back in the same operation (uses repeat start between read and write). 

Use menu option 4 to terminate the demo.

To demonstrate the error detection and time out features of the driver, follow
these steps:

1. Error: Disconnect the SDA line. Attempt to write some data to the slave 
   via menu option 1 and observe the "Data Write Failed!" message.
   
2. Time out: Connect the SDA line to gnd. Attempt to write some data to 
   the slave via menu option 1 and observe the "Data Write Timed Out!" message 
   after 3 seconds.
3. The I2C_LOOPBACK design variant should be used for testing MSS I2C master slave example 
   project. Please refer [here](https://github.com/polarfire-soc/icicle-kit-reference-design) for more details.

This project provides build configurations and debug launchers as exaplained 
[here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)