# PolarFire SoC MSS User Crypto RSA Encyption/Deryption Services Example

This example project demonstrates the use of the PolarFire RSA service to 
encrypt and decrypt the message. The following User Athena service are used:

  - RSA Encryption  
  - RSA Decryption

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
the particular service, user has to enter the required information as shown over
the serial port. This program also displays the return data from User Crypto
processor.

RSA is an algorithm used by modern computers to encrypt and decrypt messages. 
It is an asymmetric cryptographic algorithm. Asymmetric means that there are two
different keys. This is also called public key cryptography, because one of them
can be given to everyone. The other key must be kept private. It is based on the
fact that finding the factors of an integer is hard (the factoring problem). 
RSA involves a public key and private key. The public key can be known to 
everyone, it is used to encrypt messages. Messages encrypted using the public 
key can only be decrypted with the private key. 

### Data Encryption

Option '1' to encrypt the message using RSA.
This service encrypt the message based on public key. This example project will
read the public key(n & e) and message from UART terminal and computes the
cipher text corresponding to c = m^e mod n. The cipher text is displayed on 
UART terminal.

### Data Decryption

Option '2' to decrypt the message using RSA.
This service decrypt the message based on private key. This service performs
an RSA private key decryption operation with CRT and SCA countermeasures on the 
data buffer. This example project will read the cipher text, private key (n & d), 
public key(n & e), private prime modulus i.e. P modulus and q modulus from UART
terminal and computes the plain text corresponding to m = c^d mod n = m^(de) mod n.
The computed plain text is displayed on UART terminal.

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

## Macro Script

A macro script is provided with this example which automatically enters the NIST
vectors and associated data to verify the functionality. You can use
RSA_Cryptography.ttl Tera Term Macro script present in project directory for
testing RSA Cryptography example project.

**NOTE:**
1. Tera Term Macros don’t work with Windows 10 build 14393.0. You should update
   to Windows 10 build 14393.0.105 or [later.](https://osdn.net/ticket/browse.php?group_id=1412&tid=36526) 
2. Before running Tera Term Macro script, set language as English 
   (Setup->General->Language). Also setup transmit delay in (Setup->Serial port)
   to 5msec/char and 5msec/line.
3. By default, Tera Term log will be stored in Tera Term installation Directory.

## Silicon Revision Dependencies

This example is tested on PolarFire SoC Video kit (MPFS250TS-1FCG1152I).

## CAL Library Src

The CAL source code is bound by license agreement. If you need access to the CAL
source code, please contact FPGA_marketing@microchip.com for further details on
NDA requirements.