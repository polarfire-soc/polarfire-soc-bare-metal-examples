# PolarFire SoC Hardware Abstraction Layer v1.7

This document provides a brief overview of the PolarFire SoC hardware features related to memory hierarchy and suggested uses of these features.

Please refer to the [PolarFire SoC Microprocessor Subsystem (MSS) User Guide](https://www.microsemi.com/document-portal/doc_download/1244570-ug0880-polarfire-soc-fpga-microprocessor-subsystem-mss-user-guide) for the detailed description of PolarFire SoC.

## Revision History

|  Revision  | Description                                                              	   |
| ---------- | ------------------------------------------------------------------------------- |
| 0.1        | Initial draft -copy from word format. Todo next: add pictures from word document
   

## Contents

- Introduction
- MPFS HAL usage
- MPFS HAL features
- Files provided
- MPFS memory map
- MPFS HAL deplyment
- MPFS HAL configuration
- Use with SoftConsole
- Useful tips


## Introduction

PolarFire SoC HAL is the hardware abstraction layer for Microchip’s PolarFire SoC (MPFS) MSS. The MPFS HAL provides the boot code, interrupt handling, and hardware access methods for the MPFS MSS. The terms PolarFire SoC HAL and MPFS HAL are used interchangeably in this document.
The MPFS HAL package is a combination of C and assembly source code.
Its supports bare metal program development for Microchips PolarFire SoC development platform (Renode development platform before release) and SiFive’s HiFive Unleashed platform.
Some example programs are bundled with the HAL.

### Features

The MPFS HAL provides the following features:
-	Support for the SoftConsole tool chain
-	Configuration of the MSS on startup
-	Platform-Level Interrupt Controller (PLIC) access functions
-	Power, Reset, Clock, and Interrupt (PRCI) access functions
-	Hardware abstraction layer for Microchip’s DirectCore IP drivers
-	Systick functionality
-	Project directory structure layout guidance

### Supported IP

The MPFS HAL can be used with the PolarFire SoC FPGA Family.

## MPFS HAL USAGE

The MPFS HAL is designed to be used as part of a bare metal OS.
The MPFS HAL provides infrastructure code for handling the boot sequence and system configuration of the MPFS processor. MPFS HAL relies on the hardware configuration data generated during the Libero design flow. 
The MPFS HAL consumes hardware configuration data in the form of C header files which contain defines and data structures. Once the Libero design flow is completed, the following information must be extracted from Libero into the firmware project:

-	The Memory map
-	The Interrupt mapping from MSS Cores 
-	The Interrupt mapping from DirectCores
-	DDR controller configuration
-	Clock rate defines which are used
    -	To set the tick rate (if used)
    -	Clock setup for certain cores and the corresponding core drivers

The linker script and <hw_platform.h> files contain hardware related information and must be edited with hardware information from the Libero design.

The HAL is intended to be used in one of the following use cases:
-	Use case one – Sole control of Coreplex
-	Use case two – Bare metal program on one U54

### Use case one- Boot from reset

A program of this type will initialize and run in the MSS from reset. It will be located in eNVM.
An example of the tasks which will may be performed are:

-	Switch on Clocks
-	I/O config
-	MPU config
-	DDR config
-	Cache config
-	Ethernet MAC config
-	POST of peripherals
-	Run user required functionality
-	Can run code on multiple harts
-	Can be used as the foundation of a bootloader

#### Figure 1 Simplified view of multi-hart start-up (to go here)


![Hierarchy](https://bitbucket.microchip.com/projects/FPGA_PFSOC_ES/repos/polarfire-soc-documentation/raw/images/memory-hierarchy/Cache-LIM-Scratchpad-hierarchy.png?at=refs%2Fheads%2Ftemporary-images)


### Use case two - Single U54

This program will run on one of the U54 harts. It will be started by a boot loader ( most likley by the Hart Software Services ) generally running from the E51 hart.  
In this mode HW config will be limited to the peripherals under its control. 
It will run in Supervisor Mode. The bootlocader will have taken care of setting up the most of the hardware including the PMP's.

##### Expected from bootloader:

-   memory space initialised
-   PMP setup so only access allowed as required ( its own program and peripherals )
-   Interrupts have been delegated to supervisor mode
-   handlers to allow turn on/off of reset/clk on perippherals. Current version enables U54 associated peipherals on startup by default.
-   pointer to shared hart memory space passed ( See details in OpenSBI section ) This shared memory space will be used for future added functionality where inter hart communication is required.

##### Code functionality:

-	Initialize program memory on start-up
-   Contains driver code for hardware under its control 
-   Systick Interrupt setup and handler
-   Driver associated PLIC interrupts and handlers
-   local interrupt setup and handlers
-	Application functionality



## MPFS HAL Features

This section describes the features of MPFS HAL.
We use the term main hart to reference the hart designated to initialize the program memory and bring the other harts out of wfi.


### Start-up code

The MPFS HAL provides the startup code out of reset. 
The main features are:
#### SET/CLEAR Registers
Includes setting the stack pointer for each hart.

#### Synchronization of hart startup
There is one hart which is software defined as the main hart. It will bring up other harts as required. 
There are flags which are used in the startup code to make sure other harts are in correct state when being brought out of wfi. 

#### Initialize memory
Clear and set memory as required. This is done by the main hart.

#### Initialize systick timer
There is a function available to do this.
<uint32_t systick_config()>

#### Bus Error Unit
Configures how BEU operates.

#### DDR setup
Configures DDR, based on hardware configuration. Detail to be added in later rev. of this UG.

#### MMSIO
Configures IO, based on hardware configuration. Detail to be added in later rev. of this UG.

#### SGMII
Configures SGMII, based on hardware configuration. Detail to be added in later rev. of this UG.

### Interrupt servicing structure

Structures and routines are provided to manage interrupts and exceptions.

#### External PLIC interrupts
Code is provided to managed PLIC interrupts
Structures and routines are provided to manage interrupts and exceptions.

#### Internal interrupts
Code is provided to manage internal interrupts

#### Exceptions
Code is provided to handle exceptions.

#### 4.3	Atomic Instructions
RISC-V provides a number of atomic instructions.
Inline defines for these are provided in:
<platform/mpfs_hal/atomic.h>

#### 4.4	Mutex support functions
In the file <mss_mutex.S>, there is mutex support.
This is only supported by the E51 hart, as the Atomic extension is required.
Note 1: An extension is required for this to work, E51 hart and U54 hart include A extension.
Note 2: E51 hart extension is: RV64IMAC, U54 hart is extension is RV64GC (G includes A)

#### 4.5	System Timer - Systick
The MPFS HAL supports the setup and maintenance of a systick timer.
The related functions can be found in <mtrap.c>.
The following functions are provided.
void SysTick_Config(void);
uint64_t SysTick(void);
uint64_t SysTick_Handler(void);
The SysTick_Config() function uses a value in milliseconds defined in <mss_sw_config.h> and sets up the system tick to increment at that frequency. 


The function SysTick_Handler() increments systick value
The function SysTick() returns the current systick value

systick rollover will occur when the MTIME register reaches 0xFFFF_FFFF_FFFF_FFFF
For SysTick(), this equates to 
SysTick() interval = (0xFFFF_FFFF_FFFF_FFFF /TICK_RATE_IN_SYS_FREQ)

Where 
TICK_RATE_IN_SYS_FREQ = (MSS_COREPLEX_CPU_CLK /CURRENT_HART_TICK_RATE_MS)


## 5	Files provided

The following resources are provided as part of MPFS HAL:
-	Documentation
-	Core source code (including example linker scripts)
-	Example projects
-	Debug configuration files

The MPFS HAL is distributed through the Firmware Catalog of Microchip SoC Products Group. The Firmware Catalog:
-	Provides access to the documentation of MPFS HAL
-	Generates the MPFS HAL source files into an application project
-	Generates debug configuration files for the SoftConsole tool chain.
-	Generates example projects
The Firmware Catalog is available at www.microsemi.com/soc/products/software/firmwarecat/default.aspx.

### 5.1	Documentation
The Firmware Catalog provides access to the following documents:
-	User guide (this document)
-	Release notes

### 5.2	MPFS HAL Source Code

The Firmware Catalog generates the PolarFire SoC (MPFS) HAL source code into the <hal>, <mpfs_hal>         and <config> sub directories of the selected <platform> software project directory. In the example project below, the directories in the orange box contain the MPFS HAL and drivers plus configuration for the <hal> and <drivers>.

### Figure 2 Example directory structure ( to go here)


### 5.2.1	Files located in the <mpfs_hal> directory
The <mpfs_hal> directory contains files which support the OS and bare metal drivers.
Please note these files should not be edited. Where configuration is required, it is provided using header files located in a separate configuration directory detailed in a separate section.
#### atomic.h
Provides inline access functions for RISC-V atomic instructions
#### bits.h
Some helpful defines
#### encoding.h
RISC-V register bit definitions

#### entry.S
Startup code and interrupt vector table
#### mcall.h
interrupt defines
#### mss_clint.h
Clint access functions and definitions
#### mss_coreplex.h
Some Coreplex definitions
#### mss_h2f.c
host to fabric functions
#### mss_h2f.h
host to fabric definitions
#### mss_hal.h
single reference to header files
#### mss_hart_ints.h
contains local hart interrupt definitions
#### mss_ints.h
interrupt definitions
#### mss_mpu.c
memory protect unit support functions
#### mss_mpu.h
memory protect unit support definitions
#### mss_mutex.S
mutex implementation functions
#### mss_peripheral_base_add.h
mss peripheral base addresses definitions

#### mss_plic.h
plic definitions

#### mss_prci.h
power,reset,clock definitions
#### mss_seg.h
segment definitions
#### mss_stubs.c
weak handler stub definitions
#### mss_sysreg.h
mss system register definitions
#### mss_util.c
some utilities
#### mss_util.h
utility header file
#### mtrap.c
interrupt handling functions
#### mtrap.h
definitions related to interrupt handling
#### newlib_stubs.c
newlib library
#### system_startup.c
First C code called on startup. Will jump to user code.
#### system_startup.h
definitions associated with system startup.


### 5.2.2	Files located in the <hal> directory

The <hal> directory contains files which support drivers related to soft fabric components (known as DirectCores). As a general rule, the bare metal OS or MSS drivers do not use the files located here.
cpu_types.h
types related to cpu use.
#### hal.h
obsolete method of access registers. Present to support older drivers.
#### hal_assert.h
assert support for older drivers.
#### hal_irq.h
enable/disable interrupts support functions
#### hw_macros.h
Obsolete. Present to support older drivers.
#### hw_reg_access.h
Obsolete. Present to support older drivers.
#### hw_reg_access.S
Obsolete. Present to support older drivers.


### 5.2.3	MPFS Hal Configuration files
The MPFS HAL must reference configuration files related to the MPFS HAL and the bare metal driver configuration.
These are located in the <platform/config> directory
Please see section on MPFS HAL configuration for further details. 

### Figure 3 Figure showing where example linker scripts are located ( to go here)

### 5.2.4	Bare metal drivers
The bare metal drivers are located in the <platform/drivers> folder.
Each driver has its own folder in this directory. They should be added to a project as required.

### Figure 4 figure showing drivers folder location (todo: to go here)


## 6	MPFS memory map
This section contains details on the PolarFire SoC MSS memory map.

### 6.1.1	Core Complex address map
Table below give core complex address map.

#### Table 1 Core Complex address space

| Base          | Top           | Attribute | Description |
| ----------    | ------------------------------------------------------------------------------|
| 0x0100_0000	|0x0100_1FFF	| RWXA		|E51 DTIM (8KB)
| 0x100_2000	| 0x16F_FFFF	|	|Reserved	
| 0x170_0000	| 0x170_0FFF	| RW		|Bus Error Unit for E51 Hart0
| 0x170_1000	| 0x170_1FFF	| RW		|Bus Error Unit for U54 Hart 1
| 0x170_2000	| 0x170_2FFF	| RW		|Bus Error Unit for U54 Hart 2
| 0x170_3000	| 0x170_3FFF	| RW		|Bus Error Unit for U54 Hart 3
| 0x170_4000	| 0x170_4FFF	| RW		|Bus Error Unit for U54 Hart 4
| 0x170_4000	|				|			|Reserved	
| 0x180_0000	| 0x180_1FFF	| RWXA		|E51 Hart 0 ITIM
| 0x180_2000	| 0x180_7FFF	|			|Reserved
| 0x180_8000	| 0x180_EFFF	| RWXA		|U54 Hart 1 ITIM
| 0x180_F000	| 0x180_FFFF	|			|Reserved
| 0x181_0000	| 0x181_6FFF	| RWXA		|U54 Hart 2 ITIM
| 0x181_7000	| 0x181_7FFF	|			|Reserved
| 0x181_8000	| 0x181_EFFF	| RWXA		|U54 Hart 3 ITIM
| 0x181_F000	| 0x181_FFFF	|			|Reserved
| 0x182_0000	| 0x182_6FFF	| RWXA		|U54 Hart 4 ITIM
| 0x182_7000	| 0x182_7000	|			|Reserved
| 0x20_0000		| 0x280_FFFF	| RW		|CLINT
| 0x201_0000	| 0x201_0FFF	| RW		|Cache controller
| 0x202_1000	| 0x202_FFFF	|			|Reserved
| 0x202_0000	| 0x202_0FFF	| RW	WCB |
| 0x202_1000	| 0x202_1FFF	|			|Reserved
| 0x300_0000	| 0x30F_FFFF	| RW		|DMA Controller
| 0x310_0000	| 0x7FF_FFFF	|			|Reserved
| 0x800_0000	| 0x9FF_FFFF	| RWX		|L2-LIM
| 0xA00_0000	| 0xBFF_FFFF	| RWXC		|L2 Zero Device
| 0xC00_0000	| 0xFFF_FFFF	| RW		|PLIC
| 0x1000_0000	| 0x1FFF_FFFF	|			|Reserved

### 6.2	Code and data Memory map
The key memory area’s associated with program and data storage is detailed below.
There is also data and instruction caches associated with each U54 hart but they are not detailed here.

### 6.2.1	eNVM

All five E51/U54 hart’s reset vectors point to the eNVM at address 0x20222000.

			
| Instance          | Base address           | range 	| comment 									|
| ----------    	| ------------------------------------------------------------------------------|
|ENVM				|0x2022_2000			|56KB		|Used to store initial boot program

There is a driver associated with read/write operations to the eNVM. 
This is available in the library on github.

### 6.2.2	Data Tightly Integrated Memory - DTIM 

Physically DTIM is close (access time) to the E51 but can be accessed/used by all the harts.

| Instance	| Base address	| range	| comment
| ----------    	| ------------------------------------------------------------------------------|
| DTIM	| 0x0100_0000	| 8KB	| Use as memory for low latency code

Recommended uses:
-	DTIM can be used as memory for a small boot program where 8KB is adequate.
-	It can also be used for program sections where performance is required.
-	If DMA data section can be a good use.
Also a section of a program could be run from here if you want to reprogram the eNVM. 

--- 6.2.3	Loosely Integrated memory – LIM

There is a 2MB cache memory in front of the DDR. On hardware reset, this is configured as LIM memory apart from one cache way at the top of the 2 MB memory range. It can be used for program/data storage if not required as cache. Cache is configured using the cache controller register set.

|Instance	|Base address	|range	|comment
| ----------    	| ------------------------------------------------------------------------------|
|LIM	|0x0100_0000	|8KB	|

--- 6.2.4	DDR

There are a number of addresses that point to the DDR memory space, depending on how you want to access it.
The SEG0 and SEG1 are used to configure an offset into the DDR as required.
There is also blocker IP located in SEG0 which prevents access to unconfigured DDR.
The blocker must be unset by software once the DDR is configured. 

|SEG Reg		|SEG0	|SEG1
| ----------    	| ------------------------------------------------------------------------------|
|0	|Cached access at 0x00_8000_0000	|yes	|no
|1	|Cached access at 0x10_0000_0000	|yes	|no
|2	|Non-Cached access at 0x00_c000_0000	|no	|yes
|3	|Non-Cached access at 0x14_0000_000	|no	|yes
|4	|Non-Cached WCB access at 0x00_d000_0000	|no	|yes
|5	|Non-Cached WCB access at 0x18_0000_000	|no	|yes
|6	|Trace access	|No	|Yes
|7	|DDRC blocker Control 	|Yes	|No

The <mpfs_hal/mss_seg.h> contains a structure used to access the SEG registers, used when configuring the SEG register.
More details and an example project will be included in this document in a later revision.

###--- 6.2.5	L2 Cache
There is a 2MB cache memory in front of the DDR. On hardware reset, this is configured as LIM memory apart from one cache way at the top of the 2 MB memory range. It can be used for program/data storage if not required as cache. Cache is configured using the cache controller register set.


## 6.3	MSS peripheral memory map
Each MSS peripheral has a fixed location in memory (although some have a second alternate address to allow
 bandwidth management on the internal AXI bus)
 
| Instance Name 	| Base Address 	| Range
| ----------|--------|------------------------------------------------------------------------------|
|MMUART0_LO |0x2000 0000 	|4 KBytes 
|WDOG0_LO| 0x2000 1000 	| 4 KBytes
|g5_mss_top_sysreg | 0x2000 2000 	| 1 KBytes
|SYSREGSCB | 0x2000 3000 	| 1 KBytes
|AXISW | 0x2000 4000 	| 4 KBytes
|MPUCFG | 0x2000 5000 	| 4 KBytes
|FMETER | 0x2000 6000 	| 4 KBytes
|CFG_DDR_SGMII_PHY | 0x2000 7000 	| 4 KBytes
|EMMC_SD | 0x2000 8000 	| 4 KBytes
|DDRCFG | 0x2008 0000 	| 256 KBytes
|MMUART1_LO | 0x2010 0000 	| 4 KBytes
|WDOG1_LO | 0x2010 1000 	| 4 KBytes
|MMUART2_LO | 0x2010 2000 	| 4 KBytes
|WDOG2_LO | 0x2010 3000 	| 4 KBytes
|MMUART3_LO | 0x2010 4000 	| 4 KBytes
|WDOG3_LO | 0x2010 5000 	| 4 KBytes
|MMUART4_LO | 0x2010 6000 	| 4 KBytes
|WDOG4_LO | 0x2010 7000 	| 4 KBytes
|SPI_A_LO | 0x2010 8000 	| 4 KBytes
|SPI_B_LO | 0x2010 9000 	| 4 KBytes
|I2C_A_LO | 0x2010 A000 	| 4 KBytes
|I2C_B_LO | 0x2010 B000 	| 4 KBytes
|CAN_A_LO | 0x2010 C000 	| 4 KBytes
|CAN_B_LO | 0x2010 D000 	| 4 KBytes
|GEM_A_LO | 0x2011 0000 	| 8 KBytes
|GEM_A_HI | 0x2011 2000 	| 8 KBytes
|GPIO_IOBANK0_LO | 0x2012 0000 	| 4 KBytes
|GPIO_IOBANK1_LO | 0x2012 1000 	| 4 KBytes
|GPIO_FAB_LO | 0x2012 2000 	| 4 KBytes
|MSRTC_LO | 0x2012 4000 	| 4 KBytes
|MSTIMER_LO | 0x2012 5000 	| 4 KBytes
|H2FINT_LO | 0x2012 6000 	| 4 KBytes
|CRYPTO | 0x2012 7000 	| 64 KBytes
|ENVMCFG | 0x2020 0000 	| 4 KBytes
|USB | 0x2020 1000 	| 4 KBytes
|QSPIXIP | 0x2100 0000 	| 4 KBytes
|ATHENA | 0x2200 0000 	| 64 KBytes
|TRACE | 0x2300 0000 	| 192 KBytes
|MMUART0_HI | 0x2800 0000 	| 4 KBytes
|WDOG0_HI | 0x2800 1000 	| 4 KBytes
|MMUART1_HI | 0x2810 0000 	| 4 KBytes
|WDOG1_HI | 0x2810 1000 	| 4 KBytes
|MMUART2_HI | 0x2810 2000 	| 4 KBytes
|WDOG2_HI | 0x2810 3000 	| 4 KBytes
|MMUART3_HI | 0x2810 4000 	| 4 KBytes
|WDOG3_HI | 0x2810 5000 	| 4 KBytes
|MMUART4_HI | 0x2810 6000 	| 4 KBytes
|WDOG4_HI | 0x2810 7000 	| 4 KBytes
|SPI_A_HI | 0x2810 8000 	| 4 KBytes
|SPI_B_HI | 0x2810 9000 	| 4 KBytes
|I2C_A_HI | 0x2810 A000 	| 4 KBytes
|I2C_B_HI | 0x2810 B000 	| 4 KBytes
|CAN_A_HI | 0x2810 C000 	| 4 KBytes
|CAN_B_HI | 0x2810 D000 	| 4 KBytes
|GEM_B_LO | 0x2811 0000 	| 8 KBytes
|GEM_B_HI | 0x2811 2000 	| 8 KBytes
|GPIO_IOBANK0_HI | 0x2812 0000 	| 4 KBytes
|GPIO_IOBANK1_HI | 0x2812 1000 	| 4 KBytes
|GPIO_FAB_HI | 0x2812 2000 	| 4 KBytes
|MSRTC_HI | 0x2812 4000 	| 4 KBytes
|MSTIMER_HI | 0x2812 5000 	| 4 KBytes
|H2FINT_HI | 0x2812 6000 	| 4 KBytes
|IOSCBCFG | 0x3708 0000 	| 3KBytes

#### Figure 5 Peripheral memory map

The above memory map is detailed in <mss_peripheral_base_add.h>


## 6.4	Compiler code model options

There are two memory models supported by the compiler, medlow and medany.
Both are limited to a max 2GB range.
medlow is more efficient as it utilizes the RISC-V gp (global pointer) register to use less instructions.

There are plans to support a larger memory model than 3GB but as I write (August 2019) this is not supported.



Table 2 Memory model options

| Compiler Memory model	| Address range	| Comment
|---------------------------------------------------|
|medlow (medium-low)	| Linked around zero -2 GiB and +2 GiB |i f code is running from DDR, the code must be compiled using the medany memory model
|medany (medium-any)	|any single 2 GiB address range	|

If code is running from DDR, the code must be compiled using the 
medany memory model, as code will be in an unsupported memory range. 

The newlib-nano lib was compiled using the medlow option.
This means it cannot be used when compiling for DDR, use newlib instead.
[This may have changed. Compile with newlib-nano and see if you get an error]

For more information
See https://www.sifive.com/blog/all-aboard-part-4-risc-v-code-models#what-is-a-code-model
See also the SoftConsole release notes.


|memory	|Address range	|Comment
|-----------------------------------------|
|DDR	|0x80000000	|If code running from DDR, the code must be compiled using the medany memory model
|LIM	|0x08000000-0x09FFFFFF	|Use medany or medlow as in the 2G range from zer0
|DTIM	|0x01000000-0x01002000	|Use medany or medlow as in the 2G range from zer0
|ITIM	|See core complex address map above |Normally used as L1 cache, put time critical code can be loaded here

Workaround to access memory outside address range, pointers can be used:
-	uint32_t *pointer;
-	pointer = ADDRESS_OUSIDE_RANGE;
-	uint32_t value = *pointer;

# 7	MPFS HAL Deployment
The MPFS HAL is deployed from the Firmware Catalog into a software project by generating the MPFS HAL source files into the project’s <platform> directory.

## TODO: aDD - Figure 6 platform directory location within a project

### 7.1	Recommended program structure
Figure 4 above is the recommended program structure. The <platform> directory must be present in your program as show. The inclusion of a root <src> folders and an <application> and <module> directory is recommended.

### 7.1.1	Application code
It is recommended the user application code is placed in the <application> directory.

### 7.1.2	Modules
It is recommended the third-party code is placed in the <modules> directory, each in its own sub folder. It is also recommended configuration files associated with the third-party software is placed in the <modules/config> folder.

# 8	MPFS HAL Configuration

The MPFS HAL provides infrastructure code for handling the boot sequence and system configuration of the MPFS processor. The MPFS HAL relies on the hardware configuration data generated during the Libero design flow. The MPFS HAL must not be configured manually.
The MPFS HAL consumes hardware configuration data in the form of C header files and data structures. Once the Libero design flow is completed, the following information must be extracted from Libero into the firmware project:
-	The memory map
-	The interrupt mapping from MSS peripherals
-	The interrupt mapping from DirectCores
-	Clock dependencies
-	To set the systick rate (if used)
-	Clock setup for certain cores and the corresponding core drivers

The linker script and <hw_platform.h   >   files contain hardware related information extracted from the Libero design.

## TODO: ADD- <Table 3 Config folder in the platform directory>

## 8.1	Hardware configuration

The variable hardware elements associated with the MPFS HAL are fed to the MPFS HAL using header files.  
These header files will be generated from the hardware flow but at time of writing are hand crafted. The starting point for editing these files should be the files bundled with the example programs. 
The header files are located in the following directory:
<src/platform/config/hardware>

The <src/platform/config/hardware> directory contains subdirectories related to the hardware elements shown below.

## TODO: ADD < picture of directory here from UG>

These subdirectory header files are referenced in the following file:
	 
	<src/platform/config/hardware/hw_platform.h>

### todo: <Table 4 Example <hw_platform.h> where only clock configuration is required>

The file <src/platform/config/hardware/hw_platform.h> is referenced by the MPFS HAL code.  
Hence clock speed, memory location and size etc are accessed in a systematic way without the 
need to edit MPFS HAL code directly.

### Clock hardware Configuration
The clock configuration file must be edited to match the hardware design. Please examine comments at the start of file in one of the example projects for details.
The configuration file should be located in the following directory

	<src/platform/config/hardware/clocks/hw_cfg_clocks.h>

### DDR hardware Configuration
The DDR configuration should be located here.

	<src/platform/config/hardware/ddr/hw_cfg_ddr.h>

### MSS IO hardware Configuration
The MSS IO configuration should be located here.

	<src/platform/config/hardware/mssio/hw_cfg_mssio.h>
	
### Memory Map
The memory map output from the hardware flow will be contained in the directory shown below.

	<src/platform/config/hardware/memory_map/readme.txt>

Note: This will be generated during the hardware design flow. Currently it is a placeholder.

## 8.2	Software configuration
Software configuration related to the MPFS HAL and associated drivers is located in the following directory

	<src/platform/config/software/>
	
### 8.2.1	MPFS HAL software configuration
The following directory contains the software configuration:

	<src/platform/config/software/mpfs_hal/mss_sw_config.h>

Elements of the MPFS HAL software configuration are detailed under the following headings:

### Elements of MPFS HAL software configuration
Below is some detail on software configuration elements.
#### NUMBER OF HARTS
Define the starting hart and number of harts to take out of wfi
#### TICK RATE
Define required tick rate for each hart
#### BUS ERROR UNIT
Define required setup of the BEU

### 8.2.2	Driver software configuration

Software configuration for drivers where required should be located in a subdirectory of the following directory:

	<src/platform/config/software/drivers/>
	         
The subdirectory name should be the same name as the driver directory name. 


## 8.3	Linker script
The linker script file for SoftConsole must be updated with the settings required for the project. Copy an example linker script file from the example project provided with MPFS HAL to:

	<src/platform/config/linker/> and edit it as required.
This will be the linker file used in the application project.
todo: add <Table 5 Copy the closest example from examples to /linker directory and edit as required>

### 8.3.1	Critical Elements of the linker script

Below is some detail on important elements of the linker script. Please note there are example linker scripts which should be used as a starting reference
#### MEMORY
This section defines the various physical memory areas to be used in the project in terms of start address and size.
#### SECTIONS
This section defines the various program elements and which physical memory they will be located in. 
#### ENTRY(_start)
This gives the reset address and is used in the program startup. 
#### PROVIDE(__stack_bottom_h0$ = .)
This gives the stack size (one for each hart) and is used in the program startup. 

# 9	Using the PolarFire SoC HAL with Softconsole
Please see the example programs bundled with the MPFH HAL for correct configurations settings.
Please note Softconsole v6.1 or later must be used.
Detail on using SoftConsole v6.1 can be found in the SoftConsole release notes.
Two example projects come bundled with the MPFS HAL
-	mpfs-mss-uart-example
-	unleashed-uart-example

## 9.1	Creating a workspace
To create a new workspace you must make a copy of the <workspace.empty> example workspace in SoftConsole v6.1. This is because the <workspace.empty> workspace has some required settings for Renode as well as some useful settings – including an integrated “develop and debug” perspective preconfigured to make development and debug easier than with a completely new/blank workspace.

## 9.2	Import example projects

From the firmware catalog, generate the MPFS HAL example projects into your workspace.

## 9.3	Open SoftConsole v6.1 and import project
Follow these steps
-	Open SoftConsole
-	Switch workspace to your workspace
	- File->switch workspace
	- Import the projects
-	File->import->existing projects into workspace
	- In the dialog untick “copy projects into workspace”
	- Select the root directory, which is the workspace
	- The example projects you generated earlier should be displayed
-	Click OK
	- The project should be displayed in the project explorer window

## 9.4	Compile the project

-	Open one of the projects by double clicking on the project in the Project Explorer window
-	Click the hammer icon to compile the project
	- The project should compile successfully

## 9.5	Use the debugger with Renode
There is a debugger setup bundled with the mpfs-mss-uart-example project
-	Click on the down option beside the bug icon to open debugger options
-	Select debug configurations
-	Under Launch group choose “mpfs-mss-uart-example Start-Renode-emulator-and-attach”
The emulator should start and the debugger window will open. You are now in a debug session.

Details on configuring and using Renode can be found in the file <Working_with_Renode.md> in each example project root directory and in the SoftConsole v6.1 release notes.

## 9.6	Use the debugger with the SiFive HiFive unleashed board
-	Connect power to board
-	Connect USB cable between PC and board
-	Click on the down option beside the bug icon to open debugger options
-	Select debug configurations
-	Under Launch group choose “unleashed-uart-example-debug”

# 10	Useful tips
This section details some common issues you may encounter and how to work around them.

When attempting to connect the debugger to the SiFive HiFive Unleashed board, you may get the following error.

~~~
Info : auto-selecting first available session transport "jtag". To override use 'transport select <transport>'.
Error: libusb_open() failed with LIBUSB_ERROR_NOT_SUPPORTED
Error: libusb_open() failed with LIBUSB_ERROR_NOT_FOUND
Error: no device found
Error: unable to open ftdi device with vid 0403, pid 6010, description 'Dual RS232-HS', serial '*' at bus location '*'
~~~


Please use the open source program Zadig to associate required driver with Interface 0 of the Dual RS232-HS.

todo: <add pic from UG here>

## 10.2	Renode emulator will not start

You must use the empty project directory that comes bundled with SoftConsole. See section on creating a workspace















