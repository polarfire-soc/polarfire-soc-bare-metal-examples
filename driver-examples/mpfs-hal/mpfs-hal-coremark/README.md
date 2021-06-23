
#            PolarFire SoC MSS Coremark Example Program


This example project runs the Cormark performace test. 


## How to use this example

To use this example you will need to obtain the coremark source files from the
Embedded Microprocessor Benchmark Consortium (EEMBC). These are placed in the 
coremark directory

To use this project you will need a UART terminal configured as below:
    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control

The example project will display the coremark results over MSS UART0.
The project should be programmed to eNVM and run from there.

## Coremark from the Embedded Microprocessor Benchmark Consortium (EEMBC)

The following files are present in Coremark 2018 from EEMBC.
Place the filesin the source/application/hart1/coremark folder.
Some configuration is required.

|  Coremark file                              | Detail                                       | 
| :-----------------------------------------: | :------------------------------------------: | 
|  coremark.h                                 | no edit required                             |
|  core_util.c                                | no edit required                             |
|  core_state.h                               | no edit required                             |
|  core_matrix.h                              | no edit required                             |
|  core_main.h                                | no edit required                             |
|  core_list_join.h                           | no edit required                             |
|  port/core_portme.c                         | edits required, see table below              | 
|  port/core_portme.h                         | edits required, see table below              | 
|  port/core_eeprintf.c                       | edits required, see table below              | 

### port/core_portme.h  required edits for the Icicle kit

The following are defines as

	#define HAS_FLOAT 1
	#define HAS_TIME_H 0
	#define USE_CLOCK 0
	#define HAS_STDIO 0
	#define HAS_PRINTF 0
	#define COMPILER_FLAGS FLAGS_STR
	#define MEM_LOCATION "STACK"

Change the type define ee_u32 to
	typedef signed int ee_u32;  

Edit the define CLOCKS_PER_SEC to match the MSS clock (LIBERO_SETTING_MSS_SYSTEM_CLK) 
found in soc_config/clocks/hw_mss_clks.hal-ddr-demo

	#define CLOCKS_PER_SEC  600000000
	
### port/core_portme.c  required edits for the Icicle kit

Add the following at the top of the file

	#include "mpfs_hal/mss_hal.h"
	#include "drivers/mss_mmuart/mss_uart.h"

	mss_uart_instance_t *gp_my_uart;
	

edit the barebones_clock() funtion

	CORETIMETYPE barebones_clock() {
		CORETIMETYPE mcycles;
		mcycles = readmcycle();
		return mcycles;
	}	

Edit portable_init()

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

### port/core_eeprintf.c  required edits for the Icicle kit

add the following

	extern mss_uart_instance_t *gp_my_uart;

	void uart_send_char(char c) {
	//<CJ>#error "You must implement the method uart_send_char to use this file!\n";
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
	
and edit ee_printf(const char *fmt, ...)
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


### Target configuration

There are two target platforms supplied with this project, the Icicle kit and 
the PolarFire SoC peripheral Base board. 

The settings associated with each taget are found in

"./src/boards/<icicle_kit_es>/.."

and 

"./src/boards/<mss_validation_board>/.."

If adding support for your own board, add a directory here

"./src/boards/<your_own_board>/.."




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


### Executing project on the PolarFire SoC hardware

There are two target platforms supplied with this project, the Icicle kit and 
the PolarFire SoC peripheral Base board. 
This application can be used on PolarFire hardware platform as well e.g. Icicle 
Kit. In this case, the MSS MMUART0 must be connected to a host PC. The host PC 
must connect to the serial port using a terminal emulator such as Tera Term or 
PuTTY or the SoftConsole built-in terminal view.

Build the project using one of the hardware configurations and launch the 
matching debug configuration.
e.g.

|  project configuration               | Matching Debug configuration                         | 
| :----------------------------------: | :--------------------------------------------------: | 
|  Icicle-kit-run-from-scratchpad-envm | mpfs-hal-coremark hw Run-from-scratchpad-envm.launch |                             
|  Icicle-kit-debug                    | mpfs-hal-coremark hw Run-from-scratchpad.launch      | 

### Programing the eNVM

The eNVM can be programmed using SoftConsole to call an external program.
In the <run/external tools> menu, call the non-secure boot mode 1 progam.
Please note the working directory must be pointing to the directory with the
.elf file you wish to download.

### Options affecting expected results

#### Compile options

The compile options chosen have a significant affect on performance

#### Memory location considerations

The coremark program can be compiler to use heap or stack in its tests.
The example program is configured to use stack.
The type of memory used for stack(or heap if Coremark using heap) in a program has 
a significant affect on performance.
Location of other program memory including code location has negligible influence.


### Choosing Test options

There are several configurations bundled with the project



 - Choose the active configuration in the SoftConsole project
    - This selects the linker script
    - The target Core 
    - The memory option
	
See project->tool settings->cross c compiler->Preprocessor
for a list of defines used in the project.
 
|  project configurations              | Detail                                       | 
| :----------------------------------: | :------------------------------------------: | 
|  Icicle-kit-run-from-scratchpad-envm | Load to eNVM on Icicle                       |                             
|  Icicle-kit-payload-u54-1*           | generates bin file, load using boot-loader   | 
|  Peripheral-base-board-envm          | Load to eNVM on PF base board                | 
|  Icicle-kit-debug                    | Use when debugging progam, loads to LIM      | 
 

*Note : In example Icicle-kit-payload-u54-1, a binary file is produced and loaded to DDR 
using a second program.You can use the mpfs-hal-ddr-demo program to do this. There is a 
section below describing this process.

### Expected Results using single hart, other harts parked

The memory configuration is the main impact on Coremark performace

The code model used is medany.
Both code models where tried from LIM and Scratchpad.
It made no difference to result.

Again, please note, only the program stack memory has an affect on the Coremark score.


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


## Project settings

### Target processor

| Item                     | setting 				  |
| :----------------------: | :----------------------: |
| Architecture             | RV64G                    |
| compressed               | off                      |
| Integer ABI              | Default                  |
| Tuning                   | Default                  |
| Case model               | medany                   |
| Small data limit         | 8                        |
| Align                    | mtune                    |


### Optimization flags used in the project

See project->GNU RISCV c cross compiler->optimization

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

##### Some examples of verbose results shown on the terminal

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
	
	
	
## Loading the Payload binary
	
When the project configuration <Icicle-kit-payload-u54-1> is selected, the resulting binary requires loading using a boot-loader.
The MPFS HAL DDR DEMO program bundled with this example can be used. Load the MPFS HAL DDR DEMO binary to envm and use the CLI menu to load the binary.
	