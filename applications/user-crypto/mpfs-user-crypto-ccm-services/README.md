# PolarFire SoC MSS User Crypto CCM Services Example

This example project demonstrates the use of the following User Crypto Services
for demonstrating CBC Counter Mode (CCM):

 - CALSymEncAuthDMA()
 - CALMAC()
    
CCM is used to provide assurance of the confidentiality and the authenticity 
of computer data by combining the techniques of the Counter (CTR) mode and the 
Cipher Block Chaining-Message Authentication Code (CBC-MAC) algorithm.  

The example project demonstrates CCM functionality using the existing CTR-AES 
and CMAC-AES modes, which are supported by the TeraFire core and CAL software. 
The two supported TeraFire functions, namely CTR-AES and CMAC-AES can be used 
to construct CCM along with some additional minor application code, to perform 
the CCM padding requirements. The example project demonstrates how to pre-process
the message data (e.g., any required padding), call the appropriate TeraFire 
C-API functions, and do any post-processing required to perform NIST standard 
compliant CCM.

## How to use this example

On connecting PolarFire SoC Video kit J12 to the host PC, you should see 4 COM
port interfaces connected. To use this project configure the COM port
**interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

Run the example project using a debugger. A greeting message will appear over the 
UART terminal followed by a menu system and instructions.

This program displays the return data from User Crypto processor for message 
authentication and hash service.

## Data Authenticated Encryption Using AES 128-bit Key

Select option '1' to perform authenticated encryption using Counter with CBC-MAC
(CCM) algorithm. This example project reads the 128 bit key, 16 bytes of Nonce,
64 bytes of plain text(P), and 16 bytes of additional authentication data (AAD) 
from UART terminal. The example project stores the keys in SNVM region using 
system controller SNVM service. After reading all the required data from UART 
terminal, the example project call **MACCcmEnrypt()** function to perform CCM 
encryption.

The CCM algorithm compute the authentication field and encrypt the message data
using Counter (CTR) mode. This CCM algorithm performs authenticated encryption 
with associated data for confidentiality and integrity. The plain text is both 
encrypted and used in computation of message authentication code according to 
the SATSYMTYPE_AES128 encryption algorithm and CCM mode. Both the encrypted data
and tag are displayed on UART terminal.

## Data Authenticated Decryption Using AES 128-bit Key

Select option '2' to perform authenticated decryption using Counter with CBC-MAC
(CCM) algorithm. This example project reads the 128 bit key, 16 bytes of Nonce,
64 bytes of cipher text(P), and 16 bytes of additional authentication data (AAD) 
from UART terminal. The example project stores the keys in SNVM region using 
system controller SNVM service. After reading all the required data from UART 
terminal, the example project call **MACCcmDecrypt()** function to perform CCM 
decryption. The CCM algorithm decrypt the encrypted message data using Counter 
(CTR) mode. The decrypted data contain the plain text message and the message 
authentication code and are displayed on UART terminal.

**NOTE:**
You can calculate the MAC using CBC MAC function based on the decrypted plain 
text message and compare the decrypted MAC and compute MAC value are equal or
not in order to authenticate the data.

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
ccm_msg_auth.ttl Tera Term Macro script present in project directory for testing
Crypto CCM Services example project.

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