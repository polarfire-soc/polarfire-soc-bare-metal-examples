# PolarFire SoC Blank Baremetal Project

This project serves as a fresh template for customer development. It includes only
the essential initialization and startup code for all harts (E51 and U54 cores),
and demonstrates basic inter-hart software interrupt handling. No application
logic or peripheral drivers are enabled by default, allowing you to begin your
own development from a blank state, tailored to your specific requirements.

## How to use this example

When connecting the **Icicle Kit** J11 to the host PC, you should see 4 COM port interfaces connected.
When connecting the **Discovery Kit** J4 to the host PC, you should see 3 COM port interfaces connected.
When connecting the **PFSoC Video Kit** J12 to the host PC, you should see 4 COM port interfaces connected.

To use this project, configure the COM port **interface 0** as below:

 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

This project will print "Hello World" over UART interface 0. The output can be viewed on a
serial terminal connected to interface 0.

## Build and Debug Configurations

This project includes reference build, debug, and external tools configurations for all supported kits, located in the `launch_configs` folder. These files are automatically detected by SoftConsole and can be selected as run or debug options. Use these as a starting point or reference when setting up your own project configurations for different kits.

The provided build configurations and debug launchers are explained
[here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples).

## Renode Emulation
This example can be run on the Renode emulation platform. Build the application as normal, and then launch it using `mpfs-blank-baremetal renode all-harts start-platform-and-debug.launch`

UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-blank-baremetal renode all-harts debug`. More information about using Renode is available at https://renode.readthedocs.io

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\[BUILD-CONFIGURATION]\mpfs_hal_config\`

If your project uses the default configuration file in `src\platform\platform_config_reference\` to enable DDR training, it is recommended to create a copy under the boards directory and disable `DDR_SUPPORT` there.
