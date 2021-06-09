================================================================================
                    PolarFire SoC MSS I2C Master/Slave example
================================================================================

This example project demonstrates the use of the PolarFire SoC MSS I2C 
peripherals using external Loop back. It demonstrates reading and writing 
data between a pair of MSS I2Cs configured as a master and a slave.

The operation of the MSS I2Cs is controlled via a serial console.

--------------------------------------------------------------------------------
                            How to use this example
--------------------------------------------------------------------------------
Connect the IO pins for the SDA and SCL of the two MSS I2Cs together.

To use this project you will need a UART terminal configured as below:
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
   
--------------------------------------------------------------------------------
                                Target hardware
--------------------------------------------------------------------------------
This example project can be used on the PolarFire SoC model on Renode emulation 
platform as well as PolarFire SoC FPGA family hardware platforms. 

There are configurations that need to be set for this example project. The
configurations are categorized into hardware and software configurations.
The hardware configurations are located in ./src/boards/<target_board> folder.
The default software configurations are stored under 
.src/platform/platform_config_reference folder.

The include files in the "./src/boards/<target_board>/fpga_config" folder define 
the hardware configurations such as clocks. You must make sure that the 
configurations in this example project match the actual configurations of your 
target Libero design that you are using to test this example project.

If you need to change the software configurations, you are advised to create a 
new folder to replicate this folder under the ./src/boards directory and do the 
modifications there. It would look like 
./src/boards/<target_board>/platform_config

The include files in the "platform_config" folder define the software 
configurations such as how many harts are being used in the software, what is 
the tick rate of the internal timer of each hart. These configurations have no 
dependency on the hardware configurations in "fpga_config" folder. Note that 
changing these software configurations may require a change in your application 
code.

## Executing project in the Renode emulation

Further information on working with Renode is available from the "Renode 
emulation platform" section of the SoftConsole release notes. The SoftConsole
release notes document can be found at: <SoftConsole-install-dir>/documentation

## Executing project on the PolarFire SoC hardware

This application can be used on PolarFire hardware platform as well e.g. Icicle 
Kit. In this case, the MMUART0 must be connected to a host PC. The host PC must 
connect to the serial port using a terminal emulator such as Tera Term or PuTTY 
or the SoftConsole built-in terminal view.

Build the project and launch the debug configuration named 
mpfs-i2c-master-slave hw all-harts debug.launch which is configured 
for PolarFire SoC hardware platform.

NOTE:
It is a good idea to set the drive current for the I2C I/O to the maximum
allowed so that the fall time for the I2C pins is minimised. This will help to
improve the reliability of the I2C communications.
 
