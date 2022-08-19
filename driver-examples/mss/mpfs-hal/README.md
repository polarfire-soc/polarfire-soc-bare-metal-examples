
# Creating Bare Metal Payload For HSS

## Introduction
The Hart Software Systems (HSS)[^1] is the default monitor software and boot loader 
for PolarFire SoC devices which can boot various applications payloads that 
include U-Boot, Linux, RTOS and Bare metal applications.

This document outlines the steps to 
 - Build a bare metal SoftConsole configuration and convert it into a HSS payload
 - Load and run a project using the debugger

Below is the list of SoftConsole projects which provide the build configurations
which can be used for generating HSS payloads:

| Project name	                    | Description |
| --------                          | --------    |
| mpfs-hal-run-from-ddr-u54-1       | Application running on U54_1 only |
| mpfs-hal-run-from-ddr-u54-2       | Application running on U54_2 only |
| mpfs-hal-run-from-ddr-u54-1-u54-2 | Application running on U54_1 and U54_2 |

## Build Configurations

Each of the above listed projects provide two build configurations:
- DDR-Release
- LIM-Release

The DDR-Release configuration is built to run from DDR memory.
The LIM-Release configuration is built to run from LIM memory. 

In the current Libero reference design, the memory is allocated as follows:

| Type       | size (KB) |
| :--------: | :--------:|
| LIM        | 512       |
| Scratchpad | 512       |
| L2 Cache   | 1024      |
| DDR cached | 1GB       |
| DDR Non-cached | 256MB |

## Executing the application using a bootloader
Use the Hart Software Services (HSS) to load the application to target memory (DDR, LIM).

Each of the above listed projects provide a ./resources folder with a sample HSS 
payload generator YAML file to build the payload.

1. Ensure that the HSS is programmed on the eNVM of your PolarFire SoC device.  
Either Download the HSS from the [Hart Software Services][Hart Software Services],
compile and *program it using SoftConsole*[^2]  
OR program the reference design [job file][reference design job file] *using FlashPro Express*[^3].

2. Download the hss-payload-generator from the [Hart Software Services releases][Hart Software Services releases].  
3. Copy the payload generator binaries into the ./resources folder of this project
4. Open a terminal and execute the following commands to *generate the HSS payload*[^4]:

   ```bash
   cd <project-name>/resources
   hss-payload-generator(.exe) -c <provided-yaml-file>.yaml payload.bin
   ```
	\<provided-yaml-file\> is the provided yaml file included with the project, 
	choose the appropriate file for your target memory.

4. *Program the generated payload*[^5] from ./resources directory to eMMC or SD-card 
following the steps shown in the [Updating PolarFire SoC Icicle-Kit FPGA Design 
and Linux Image][udt] document to program using USBImager or, for example, using dd on Linux:

   ```bash
   sudo dd if=payload.bin of=/dev/sdX
   ```

   > Be very careful while picking /dev/sdX device! Look at dmesg, lsblk, GNOME 
   Disks, etc. before and after plugging in your USB flash device/uSD/SD to find 
   a proper device. Double check it to avoid overwriting any of system disks/partitions!
   
[Hart Software Services]: https://mi-v-ecosystem.github.io/redirects/repo-hart-software-services   
[reference design job file]: https://mi-v-ecosystem.github.io/redirects/releases-icicle-kit-reference-design   
[Hart Software Services releases]: https://mi-v-ecosystem.github.io/redirects/releases-hart-software-services
[udt]: https://mi-v-ecosystem.github.io/redirects/updating-icicle-kit_updating-icicle-kit-design-and-linux

## Executing the application using a debugger	

In the early development phase, you may want to first ensure that the application
in itself is working as desired before converting into the HSS payload.

In such cases, use the FlashPro debugger to download the application and step-debug. 

The prerequisite is that the hardware initializations must be complete before we 
can download the executable to the memory. The above listed SoftConsole projects 
are configured with IMAGE_LOADED_BY_BOOTLOADER == 1 which prevents all hardware 
initializations. The HSS as bootloader does the initializations and hence it 
must first be running on your board.

Please follow the steps as below.
1. Ensure that the HSS is programmed on the eNVM of your PolarFire SoC device.[^2][^3]
2. Compile the _DDR-Release_ or _LIM-Release_ build configuration provided with the SoftConsole project. 
3. Connect the debugger to the target device, and power cycle.
4. After power cycle, stop the HSS from booting U54 payloads by 
pressing any key in the UART terminal of the HSS during bootup
5. Use **\<project-name\> Payload-App-Debug.launch** to load your application to target 
memory using the SoftConsole


Note1: The **\<project-name\> hw all-hart attach.launch** can be used in a situation 
where the U54(s) are woken up by HSS and are executing the payloads 
and you want to see the current state of the processors ( e.g. to debug an issue).
Ensure that the .elf used with the launcher matches the payload executing on U54(s).

### Videos
[^1]: [![What is HSS](https://img.youtube.com/vi/AY3qhLb3EME/mqdefault.jpg)](https://www.youtube.com/watch?v=AY3qhLb3EME "What is HSS")  
[^2]: [![Building HSS and Programming the eNVM](https://img.youtube.com/vi/McAt2-6cwd4/mqdefault.jpg)](https://www.youtube.com/watch?v=McAt2-6cwd4 "Building HSS and Programming the eNVM")  
[^3]: [![Program reference design using FPExpress](https://img.youtube.com/vi/Foil5pIRBlM/mqdefault.jpg)](https://www.youtube.com/watch?v=Foil5pIRBlM "Program reference design using FPExpress")  
[^4]: [![Generating HSS Payload](https://img.youtube.com/vi/LseJta8qVbM/mqdefault.jpg)](https://www.youtube.com/watch?v=LseJta8qVbM "Generating HSS Payload")  
[^5]: [![Programming eMMC with HSS payload](https://img.youtube.com/vi/a8_uuhrv1FM/mqdefault.jpg)](https://www.youtube.com/watch?v=a8_uuhrv1FM "Programming eMMC with HSS payload")  
