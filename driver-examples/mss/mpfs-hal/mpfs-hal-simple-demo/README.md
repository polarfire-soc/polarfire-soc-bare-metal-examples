
#                        mpfs-hal-simple-example

This example project demonstrates the use of the MPFS HAL using the
MSS UART. This project is targeted at the Microchip Icicle kit.
It provides an example of how to share a peripheral, in this case a UART between
code running on separate harts. The example project here runs from hardware
reset and is not loaded by a boot-loader.

## Powerup

On reset, the hard reset vector points to code in the <mpfs_hal\entry.S>. The
reset vector is pointed to by the following label :
~~~
_start
~~~
This address is present in the associated linker scripts, used in each of the
project configurations:
~~~
/src/boards/<target-board>/platform_config/<target-build>/linker/mpfs-lim.ld
/src/boards/<target-board>/platform_config/<target-build>/linker/mpfs-envm.ld
~~~

The MPFS HAL code in <src/platform/mpfs_hal/startup_gcc/mss_entry.S> and
<src/platform/mpfs_hal/startup_gcc/system_startup.c> brings up the
hardware per configuration parameter's from the MSS Configurator and
<mss_sw_config.h> file. The Application software for each hart being used is
then called, located in the following directories:
~~~
<\src\application\hart0\>
<\src\application\hart1\>
<\src\application\hart2\>
<\src\application\hart3\>
<\src\application\hart4\>
~~~
The application code for each hart starts in the following functions:
~~~
e51()
u54_1()
u54_2()
u54_3()
u54_4()
~~~
Note the application harts started are defined by the following defines
~~~
MPFS_HAL_FIRST_HART
MPFS_HAL_LAST_HART
~~~
located in the software board configuration file
~~~
<src/boards/<target-board>/platform_config/<target-build>/mpfs_hal_config/mss_sw_config.h>
~~~

## Description of Application code in this example :
   - In this example, the E51 is the MPFS_HAL_FIRST_HART
   - The MMUART0 is initialized
   - Hart1 is started by sending a software interrupt from the E51 code
   - The example demonstrates using MMUART0 to write to an attached terminal
      using hart0 and hart 1.
   - There is a CLI driven from hart0. This allows the user to wake hart 1,2,3
      and 4.
   - Hart 1,2,3 and 4 write out to the terminal interface demonstrating the use
      of a shared peripheral.
   - The MMUART polled tx is protected using a mutex, to allow each hart write to
      it independently.

## Libero Design:

The Libero design used with this project is the reference design and is
preprogrammed on new Icicle kits and can found at the following link :
~~~
https://github.com/polarfire-soc/icicle-kit-reference-design.git
~~~

## How to use this example

This example project demonstrates sharing a UART across multiple harts.

To use this example:

 - Import and Open the project in SoftConsole
 - Compile the project with the desired configurations
  - LIM-Debug ( To debug from LIM or Renode )
  - eNVM-Release  (When you want to load to eNVM)
 - Run the example software project, using one of the build configurations
   options
  - renode hardware emulation
  - debug on hardware
  - load to eNVM
 - When Debugging on hardware, first put the board in mode 0
 - Then run the debugger script <mpfs-hal-simple-demo hw all-harts debug>

The software will display a menu on the command line.

From the menu you can start the U54 harts and observe output on the terminal
window. This demonstrates that the harts are running independently.

## The UART configuration

On connecting Icicle kit J11 to the host PC, you should see four COM port
interfaces connected. This example project requires MMUART0. To use this
project the host PC must connect to the COM port interface0 using a terminal
emulator such as HyperTerminal or PuTTY configured as follows:

   - 115200 baud
   - 8 data bits
   - 1 stop bit
   - no parity
   - no flow control

# Renode Emulation
This example can be run on the Renode emulation platform. Build the application as normal, and then launch it using `mpfs-hal-simple-demo renode all-harts start-platform-and-debug.launch`

UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-hal-simple-demo renode all-harts debug`. More information about using Renode is available on https://renode.readthedocs.io

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\[BUILD_CONFIGURATION]\mpfs_hal_config\`

If your project uses the default configuration file in `src\platform\platform_config_reference\` to enable DDR training, it is recommended to create a copy under the boards directory and disable `DDR_SUPPORT` there.

This project provides build configurations and debug launchers, as explained [here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)
