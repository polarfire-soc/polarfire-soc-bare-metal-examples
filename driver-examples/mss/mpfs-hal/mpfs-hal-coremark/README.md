
#            PolarFire SoC MSS Coremark Example Program


This example project runs the Cormark performace test.


## How to use this example

### 1. Coremark Source Files:


To use this example you will need to obtain the coremark source files from the
Embedded Microprocessor Benchmark Consortium (EEMBC). These are placed in the
coremark directory. This will be done automatically using git submodule, once 
you clone the repo using git and then run the following git submodule command 
in 'polarfire-soc-bare-metal-examples' directory.  

	git submodule update --init --recursive

### 2. UART Terminal Configuration:  
Configure a UART terminal with the following settings:  
- 115200 baud  
- 8 data bits  
- 1 stop bit  
- no parity  
- no flow control  

The example project will display the coremark results over MSS UART0 and MSS 
UART1. The project should be programmed to eNVM and run from there.

## CoreMark Modifications for PolarFire SoC

The following files in the CoreMark (release v1.01) repository on the EEMBC 
GitHub page are required to be modified for the PolarFire SoC. However, these 
modifications have already been made in the example project, located at 
*src\middleware\config\coremark\port*. Users do not need to make these changes 
again. Additionally, there are other files in the CoreMark repository that do 
not require any modifications.

|  Coremark file                              | Detail                         |
| :-----------------------------------------: | :----------------------------: |
|  port/core_portme.c                         | file modified               |
|  port/core_portme.h                         | file modified               |
|  port/core_eeprintf.c                       | file modified               |

**Note:** The changes listed below are for reference only, as they have already 
been made in the example project.

### port/core_portme.h 

 - Modify the following defines:

		#define HAS_FLOAT 1
		#define HAS_TIME_H 0
		#define USE_CLOCK 0
		#define HAS_STDIO 0
		#define HAS_PRINTF 0
		#define COMPILER_FLAGS FLAGS_STR
		#define MEM_LOCATION "STACK"

 - Change the type define ee_u32 to  

		typedef signed int ee_u32;

 - Edit the define CLOCKS_PER_SEC to match the MSS clock (LIBERO_SETTING_MSS_SYSTEM_CLK)
found in soc_config/clocks/hw_mss_clks.hal-ddr-demo

		#define CLOCKS_PER_SEC  600000000

### port/core_portme.c  

 - Add the following at the top of the file

		#include "mpfs_hal/mss_hal.h"
		#include "drivers/mss_mmuart/mss_uart.h"

		mss_uart_instance_t *gp_my_uart;


 - Modify barebones_clock() function

		CORETIMETYPE barebones_clock() {
			CORETIMETYPE mcycles;
			mcycles = readmcycle();
			return mcycles;
		}

 - Edit portable_init() function:

		void portable_init(core_portable *p, int *argc, char *argv[])
		{
			SYSREG->SOFT_RESET_CR &= ~( (1u << 0u) | (1u << 4u) | (1u << 5u) |
                                    (1u << 19u) | (1u << 23u) | (1u << 28u));

			SYSREG->SUBBLK_CLOCK_CR = 0xffffffff;

			gp_my_uart = &g_mss_uart0_lo;

			MSS_UART_init( &g_mss_uart0_lo,
                   MSS_UART_115200_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

			if (sizeof(ee_ptr_int) != sizeof(ee_u8 *)) {
				ee_printf("ERROR! Please define ee_ptr_int to a type that holds a pointer!\n");
			}
			if (sizeof(ee_u32) != 4) {
				ee_printf("ERROR! Please define ee_u32 to a 32b unsigned type!\n");
			}
			p->portable_id=1;
		}

### port/core_eeprintf.c  

 - Add the following declaration:

		extern mss_uart_instance_t *gp_my_uart;

 - Implement uart_send_char:

		void uart_send_char(char c) {
			/* #error "You must implement the method uart_send_char to use this file!\n"; */

			MSS_UART_polled_tx(gp_my_uart, &c, 1);

			/*	Output of a char to a UART usually follows the following model:
			Wait until UART is ready
			Write char to UART
			Wait until UART is done

			Or in code:
			while (*UART_CONTROL_ADDRESS != UART_READY);
			*UART_DATA_ADDRESS = c;
			while (*UART_CONTROL_ADDRESS != UART_READY);

			Check the UART sample code on your platform or the board documentation.
			*/
		}

 - Modify ee_printf:

		int ee_printf(const char *fmt, ...)
		{
			char buf[256],*p;
			va_list args;
			int n=0;

			va_start(args, fmt);
			ee_vsprintf(buf, fmt, args);
			va_end(args);
			p=buf;

			while (*p) {
				uart_send_char(*p);
				n++;
				p++;
			}
			uart_send_char('\r');

			return n;
		}


## Target configuration

The project currently supports the Icicle kit platform. The target 
configurations can be found in:

./src/boards/<icicle_kit_es>/..

If you need to add support for your own board, create a directory under 
./src/boards/<your_own_board>/...

#### Hardware Target configuration

The configurations are categorized into hardware and software configurations.
The hardware configurations are located in ./src/boards/<target_board> folder.
The default software configurations are stored under
.src/platform/platform_config_reference folder.

The include files in the "./src/boards/<target_board>/soc_config" folder define
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
dependency on the hardware configurations in "soc_config" folder. Note that
changing these software configurations may require a change in your application
code.

## Project settings

### Target Processor Configuration

| Item                     | setting 				  |
| :----------------------: | :----------------------: |
| Architecture             | RV64G                    |
| compressed               | off                      |
| Integer ABI              | Default                  |
| Tuning                   | Default                  |
| Code model               | medany                   |
| Small data limit         | 8                        |
| Align                    | mtune                    |

### Optimization Flags used in the project

Refer to the **Project -> Properties -> C/C++ Build -> Settings ->Tool Settings -> GNU RISCV C Cross Compiler -> Optimization** 
for detailed settings.

| Flags                    |
| :----------------------: |
| -O2                      |
| -Wno-maybe-uninitialized |
| -fno-common              |
| -funroll-loops           |
| -finline-functions       |
| -falign-functions=16     |
| -falign-jumps=4          |
| -falign-loops=4          |
| -finline-limit=1000      |
| -fno-if-conversion2      |
| -fselective-scheduling   |
| -fno-tree-dominator-opts |

## Executing project on the PolarFire SoC hardware

There is one target platform supplied with this project: the Icicle kit. This 
application can be used on the PolarFire hardware platform, such as the Icicle 
Kit. The host PC must connect to the serial port using a terminal emulator such 
as Tera Term, PuTTY, or the SoftConsole built-in terminal view.

There are following four different ways to execute this example project. User 
can use any one of them as per requirement.  

### 1. Using Debug configurations (lim and scratchpad)

Build the project using one of the hardware configurations and launch the
matching debug configuration.
e.g.

|  Project Build Configuration                   | Matching Debug configuration         | UART Port/Interface Number for Results |
| :--------------------------------------------: | :----------------------------------: | :------------------------------------: |
|  Icicle-kit-coremark-stack-in-lim-debug        | mpfs-hal-coremark hw all-harts debug | interface 0                            |
|  Icicle-kit-coremark-stack-in-scratchpad-debug | mpfs-hal-coremark hw all-harts debug | interface 0                            |

### 2. Using bootloader (mpfs-hal-ddr-demo)

To run CoreMark on the selected U54 core, follow the steps below:

You can choose any one of the following build configurations to execute and run 
CoreMark on the corresponding U54 core:

|  Project Build Configuration | UART Port/Interface Number for Results |
| :--------------------------: | :------------------------------------: |
|  Icicle-kit-payload-u54-1    | interface 0                            |
|  Icicle-kit-payload-u54-2    | interface 0 and interface 1            |
|  Icicle-kit-payload-u54-3    | interface 0 and interface 2            |
|  Icicle-kit-payload-u54-4    | interface 0 and interface 3            |

#### Steps:

1. **Program the Icicle kit in Boot Mode 1**  
   Use the mpfs-hal-ddr-demo project image to program the Icicle Kit in Boot Mode 1.
	- Select the envm-scratchpad (Icicle-Kit-eNVM-Scratchpad-Release) build 
	configuration from the mpfs-hal-ddr-demo project.  
	- Use the following link for mpfs-hal-ddr-demo project:-  
    [mpfs-hal-ddr-demo](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/tree/main/driver-examples/mss/mpfs-hal/mpfs-hal-ddr-demo) 
2. **Open the Corresponding UART Interface**  
	Open the UART interface corresponding to the selected U54 core. Ensure the 
	baud rate and other settings are correctly configured as per the details above.
3. **Build the CoreMark Example Project**  
	Build the CoreMark example project using the appropriate build image, as 
	listed in the table.
4. **Restart the board.**  
	After building the project, restart the Icicle Kit.
5. **Load the Image to DDR**  
	From the menu displayed on UART Interface 0, select option 6: "Load image to 
	DDR using YMODEM." This will allow you to flash the .bin file of the 
	selected build from the CoreMark example project to DDR using YMODEM.
6. **Start the U54 Core Application**  
	Press the key 'a' to start all U54 cores from DDR. This will initiate the 
	respective U54 core application.

**NOTE** :- The HSS can also be used. Please find the details here: 
	[HSS payloads](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/hss-and-u-boot/hss-payloads.md). 
	The entry points in the YAML file need to match the entry points used in 
	each ELF. These are:

 - u54_1: '0x80000000'  
 - u54_2: '0x800A0000'  
 - u54_3: '0x80140000'  
 - u54_4: '0x801E0000'  


### 3. Programming the eNVM

To program the eNVM, follow these steps:

1. **Build the CoreMark Example Project**  
	Use the **Icicle-kit-run-from-scratchpad-envm** build configuration to build 
	the CoreMark example project.

2. **Program the eNVM in Boot Mode 1**  
	Once the project is built, program the eNVM by using an external tool in 
	Boot Mode 1.  
		- Use SoftConsole to Load the Program  
			In SoftConsole, open the <Run/External Tools> menu and select the 
			non-secure boot mode 1 program.

**NOTE** :- Ensure the working directory is selected to the location of the .elf 
file you wish to download while programming eNVM in boot mode 1.

### 4. Renode Emulation
Renode is an emulation platform used to model the PolarFire SoC hardware 
functionality. Renode is integrated with the SoftConsole tool. To execute an 
example firmware project on Renode, you must first launch the PolarFire SoC 
model from SoftConsole and then run the executable on it.  
This can be done in two ways:

1. **Run Renode as external tool**
	* To launch the PolarFire SoC board emulation model on Renode from 
	SoftConsole, launch the preconfigured external tool from  
	**Run -> External Tools -> "PolarFire-SoC-Icicle-Renode-emulation-platform"**.  

	* Create and launch a debug configuration to download the executable and 
	connect to one of the harts for running and step debugging the code.  
	**Run -> Debug Configurations -> mpfs-hal-coremark Renode all-harts debug**.

2. **Use launch group**
	* Create a **Launch Group** configuration. This configuration will combine 
	the two steps i.e. launching of the Renode tool and launching of the debug 
	config to download and debug the code into one launch group configuration.  
	**Run -> Debug Configurations -> Launch group -> mpfs-hal-coremark Renode all-harts Start-platform-and-debug** .

#### DDR Training and Renode
When DDR training is enabled in the firmware, the application startup in Renode 
will take significantly longer. However, training doesn't have any practical 
impact in this environment since the emulated DDR memory is already reliable.  

To control DDR training, simply remove the `#define DDR_SUPPORT` line from the 
mss_sw_config.h file. This file can be found in 
`src\boards\[BOARD]\platform_config\mpfs_hal_config\`.

If your project uses the default configuration file in 
`src\platform\platform_config_reference\` to enable DDR training, it's 
recommended to copy this file to the boards directory and disable `DDR_SUPPORT` 
in the copied version.

More information about using Renode is available on 
[Renode](https://renode.readthedocs.io).  

## Key Factors Affecting Expected Results

1. **Compile options**

	The compile options you choose play a significant role in performance.

2. **Memory location considerations**

	The CoreMark program can be configured to use either heap or stack memory 
	during its tests. The provided example uses the stack for this purpose. The 
	type of memory used—whether stack or heap—has a significant impact on 
	performance. However, the location of other program memory, including code 
	location, has a negligible influence on performance.

3. **Choosing Test options**

	The project comes with several configurations.

	**Steps to Choose the Active Configuration in the SoftConsole Project:**

	- Select the configuration that best suits your needs.  
	- This will automatically choose:  
		- The correct linker script.  
		- The target Core.  
		- The appropriate memory option.  

For more information, go to 
**Project -> Properties -> C/C++ Build -> Settings ->Tool Settings -> GNU RISCV C Cross Compiler -> Preprocessor** 
to view the list of defines used in the project.

## Expected Results using single hart, other harts parked

Memory configuration is the primary factor influencing CoreMark performance.

The code model used for testing is **medany**. Both LIM and Scratchpad code 
models were tested, but they showed no noticeable impact on the results.

Important Note: The only memory that affects the CoreMark score is the program's 
stack memory.


| Ref  | Memory Configuration             | Hart tested  | Iter/Sec    | Coremark  |
| :--: | :------------------------------: | :----------: | :---------: | :-------: |
| R1   | LIM as stack                     | U54_1        |  573        |    0.95   |
| R2   | LIM as stack                     | U54_2        |  573        |    0.95   |
| R3   | LIM as stack                     | U54_3        |  573        |    0.95   |  
| R4   | LIM as stack                     | U54_4        |  573        |    0.95   |  
| R5   | DTIM as stack                    | U54_1        |  676        |    1.13   |
| R6   | DTIM as stack                    | U54_2        |  676        |    1.13   |
| R7   | DTIM as stack                    | U54_3        |  676        |    1.13   |
| R8   | DTIM as stack                    | U54_4        |  676        |    1.13   |
| R9   | scratch as stack                 | U54_1        |  1874       |    3.12   |  
| R10  | scratch as stack                 | U54_2        |  1874       |    3.12   |   
| R11  | scratch as stack                 | U54_3        |  1874       |    3.12   |
| R12  | scratch as stack                 | U54_4        |  1874       |    3.12   |
| R13  | ITIM as stack                    | U54_4        |  609        |    1.01   |
| R14  | App running from DDR             | U54_1        |  1875       |    3.12   |




### Some examples of verbose results shown on the terminal

Example 1

	Core info        : Core under test:U54_4
	Memory info      : stack:SCRATCHPAD
	2K performance run parameters for coremark.
	CoreMark Size    : 666
	Total ticks      : 32015414487
	Total time (secs): 53.359024
	Iterations/Sec   : 1874.097242
	Iterations       : 100000
	Compiler version : GCC8.3.0
	Compiler flags   : -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -finline-limit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts
	Memory location  : STACK
	seedcrc          : 0xe9f5
	[0]crclist       : 0xe714
	[0]crcmatrix     : 0x1fd7
	[0]crcstate      : 0x8e3a
	[0]crcfinal      : 0xd340
	Correct operation validated. See README.md for run and reporting rules.
	CoreMark 1.0 : 1874.097242 / GCC8.3.0 -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 - / STACKlimit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts
                                                                   nline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -finline-limit=1000 -fno                                                                       / STACKersion2 -fselective-scheduling -fno-tree-dominator-opts
Example 2

	Core info        : Core under test:U54_4
	Memory info      : stack:LIM
	2K performance run parameters for coremark.
	CoreMark Size    : 666
	Total ticks      : 104688882607
	Total time (secs): 174.481471
	Iterations/Sec   : 573.126759
	Iterations       : 100000
	Compiler version : GCC8.3.0
	Compiler flags   : -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -finline-limit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts
	Memory location  : STACK
	seedcrc          : 0xe9f5
	[0]crclist       : 0xe714
	[0]crcmatrix     : 0x1fd7
	[0]crcstate      : 0x8e3a
	[0]crcfinal      : 0xd340
	Correct operation validated. See README.md for run and reporting rules.
	CoreMark 1.0 : 573.126759 / GCC8.3.0 -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -f / STACKimit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts

Example 3

	Core info        : Core under test:U54_4
	Memory info      : stack:ITIM
	2K performance run parameters for coremark.
	CoreMark Size    : 666
	Total ticks      : 98386813418
	Total time (secs): 163.978022
	Iterations/Sec   : 609.837822
	Iterations       : 100000
	Compiler version : GCC8.3.0
	Compiler flags   : -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -finline-limit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts
	Memory location  : STACK
	seedcrc          : 0xe9f5
	[0]crclist       : 0xe714
	[0]crcmatrix     : 0x1fd7
	[0]crcstate      : 0x8e3a
	[0]crcfinal      : 0xd340
	Correct operation validated. See README.md for run and reporting rules.
	CoreMark 1.0 : 609.837822 / GCC8.3.0 -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -f / STACKimit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts

Example 4

	Core info        : Core under test:U54_4
	Memory info      : stack:DTIM
	2K performance run parameters for coremark.
	CoreMark Size    : 666
	Total ticks      : 87700859284
	Total time (secs): 146.168099
	Iterations/Sec   : 684.143810
	Iterations       : 100000
	Compiler version : GCC8.3.0
	Compiler flags   : -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -finline-limit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts
	Memory location  : STACK
	seedcrc          : 0xe9f5
	[0]crclist       : 0xe714
	[0]crcmatrix     : 0x1fd7
	[0]crcstate      : 0x8e3a
	[0]crcfinal      : 0xd340
	Correct operation validated. See README.md for run and reporting rules.
	CoreMark 1.0 : 684.143810 / GCC8.3.0 -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -f / STACKimit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts

Example 5

	Core info        : Core under test:U54_1
	Memory info      : stack:DDR
	2K performance run parameters for coremark.
	CoreMark Size    : 666
	Total ticks      : 31994603893
	Total time (secs): 53.324340
	Iterations/Sec   : 1875.316231
	Iterations       : 100000
	Compiler version : GCC8.3.0
	Compiler flags   : -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 -finline-limit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts
	Memory location  : STACK
	seedcrc          : 0xe9f5
	[0]crclist       : 0xe714
	[0]crcmatrix     : 0x1fd7
	[0]crcstate      : 0x8e3a
	[0]crcfinal      : 0xd340
	Correct operation validated. See README.md for run and reporting rules.
	CoreMark 1.0 : 1875.316231 / GCC8.3.0 -Wno-maybe-uninitialized -fno-common -funroll-loops -finline-functions -falign-functions=16 -falign-jumps=4 -falign-loops=4 - / STACKlimit=1000 -fno-if-conversion2 -fselective-scheduling -fno-tree-dominator-opts

