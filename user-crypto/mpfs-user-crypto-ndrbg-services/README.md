 # PolarFire SoC MSS User Crypto DRBG Services Example

This example project demonstrates the use of PolarFire User Crypto 
deterministic random bit generator using functions:

   - CALDRBGInstantiate()
   - CALDRBGGenerate()
   - CALDRBGUninstantiate()

## How to use this example

On connecting PolarFire SoC Video kit J12 to the host PC, you should see 4 COM
port interfaces connected. To use this project configure the COM port
**interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display instructions over the serial port. To execute
the particular service, user has to select the options as shown over the serial
port.

The example project displays instructions over the serial port. 
Press the "i" to instantiate random numbers. Press the "g" to generate a random
number. Press "d" to un-instantiate the currently instantiated random numbers.

**NOTE:** 
Please note that you will not be able to generate random numbers once the DRBG 
instance has been un-instantiates DRBG service.

## Target Hardware

This example project is targeted at PolarFire SoC Video kit (MPFS250TS-1FCG1152I).
The reference design for the PolarFire SoC video kit is available in the
following GitHub repository: [PolarFire® SoC Video Kit Reference Design.](https://github.com/polarfire-soc/polarfire-soc-video-kit-reference-design)

The steps mentioned [here](https://github.com/polarfire-soc/polarfire-soc-video-kit-reference-design)
can be used to generate a *FlashPro Express* job file from above mentioned tcl
script.

This project provides build configurations and debug launchers as explained [here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples)

## Configurations

- **PolarFire User Crypto Driver Configuration**
   - **config_user.h** is a custom configuration file for PolarFire SoC CAL
     library. Following are the recommended configuration for config_user.h file.
      
      - Define **g_user_crypto_base_address** global variable as shown below.
        This will used to configure the **PKX0_BASE** (defined in config_user.h)
        and to connect to the User Crypto Co-processor in the Libero design.

     `uint32_t g_user_crypto_base_addr  0x22000000UL;`

      - **SAT_LITTLE_ENDIAN** - Endianness of the processor executing CAL
        Library customization definitions. These definitions enable the
        respective cryptographic services in the CAL which are supported by the
        User Crypto Processor. It is recommended that these definitions are not
        removed or changed unless required. 

   - A symbol INC_STDINT_H is defined in project preprocessor setting. For more
     detail, please refer to caltypes.h file present in CAL folder.

**NOTE:**
   1. If you try to enter data values other than 0 - 9, a - f, A - F, an error 
      message will be displayed on the serial terminal.
   2. You must enter all input data as whole bytes. If you enter the 128-bit
      key {1230...0} as 0x12 0x3 and press return, this will be treated as
      byte0 = 0x12, byte1 = 0x30, byte2-127 = 0x00.

## Silicon Revision Dependencies

This example is tested on PolarFire SoC Video kit (MPFS250TS-1FCG1152I).

## CAL Library Src

The CAL source code is bound by license agreement. If you need access to the CAL
source code, please contact FPGA_marketing@microchip.com for further details on
NDA requirements.