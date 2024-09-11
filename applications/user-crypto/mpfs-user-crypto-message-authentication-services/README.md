# PolarFire SoC MSS User Crypto - Message Authentication, Verification and Hash
Service Example

This example project demonstrates the use of the following PolarFire User Crypto
Services functions:

   - CALSymEncAuth()
   - CALSymEncAuthDMA()
   - CALSymDecVerify()
   - CALSymDecVerifyDMA()
   - CALMAC()
   - CALMACDMA()
   - CALHash()
   - CALHashDMA()
    
The User Crypto message authentication service provides a way to ensure the 
message integrity and to confirm that the message came from the stated sender
(its authenticity). This example project demonstrate the use of MAC services 
to generate message authenticate code, authenticated encryption (the input 
message), and hash service to covert data of arbitrary length to a fixed length.

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
the serial port. This example project will also displays the return data from 
User Crypto processor for message authentication and hash service.

### Data Authenticated Encryption & Decryption Using AES 256-bit key

Select option '1' to perform authenticated encryption & decryption using Galois/
counter mode (GCM) algorithm. This example project reads the 256 bit key, IV,
16 bytes plain text, and additional authentication data (AAD) from UART terminal
and calls the **CALSymEncAuth()** or **CALSymEncAuthDMA()** function. This function
performs authenticated encryption with associated data for confidentiality and
integrity. The plain text is both encrypted and used in computation of message 
authentication code according to the **SATSYMTYPE_AES256** encryption algorithm
and GCM mode. Both the encrypted data and tag are displayed on UART terminal.
For authenticated decryption, it calls the **CALSymDecVerify()** or
**CALSymDecVerifyDMA()** function to perform authenticated decryption with
associated data for confidentiality and integrity. Both the decrypted data and
tag are also displayed on UART terminal.

**NOTE**:
   In Case of GCM, in place of IV paramter user has to pass JO value which is calculated 
   according to the algorithm mentioned in the following [document.](http://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf)

### Generate MAC Using **HMACSHA-256** Algorithms

Select option '2' to calculate message authentication code using HMAC SHA-256
algorithms. This example project reads the key and message from UART terminal
and calls the **CALMAC()** or **CALMACDMA()** function. The **CALMAC()** and
**CALMACDMA()** functions calculate the MAC for the message based on SHA256
algorithm and the secret key. The calculated MAC is also displayed on UART
terminal.

### Generate MAC Using AES-CMAC-256 Algorithms

Select option '3' to calculated message authentication code using AES-CMAC-256
algorithms. This example project reads the key and message from UART terminal
and calls the CALMAC function. The **CALMAC()** functions calculates the MAC for
the message based on AES CMAC 256 algorithm and the secret key. The calculated
MAC is also displayed on UART terminal.

**NOTE**:
    MAC computation using DMA (i.e. **CALMACDMA** function) doesn’t support 
    **AES-CMAC-256** Algorithm.

### Hashing

Select option '4' to perform hashing. This service reads 64 bytes of message
value and performs the hash operation using SHA-256 hash algorithm on the stored
data. The hash results are stored at user memory location and also displayed on
UART terminal.

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
vectors and associated data to verify the functionality.

The following Tera Term macro scripts are present in "script" folder. 
You can use these script for testing Message Authentication, Verfication and
Hashing services.
1. MAC-gcm_msg_auth_ver.ttl - Data Authenticated Encryption
2. MAC-hmac_aes_cmac_256.ttl - Generate MAC Using AES-CMAC-256 Algorithms
3. MAC-hmac_sha_256.ttl - Generate MAC Using HMAC SHA-256 Algorithms
4. SHA-256.ttl - Hashing

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