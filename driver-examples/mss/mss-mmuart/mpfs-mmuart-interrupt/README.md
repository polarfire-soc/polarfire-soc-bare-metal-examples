# PolarFire SoC MSS MMUART transmit/receive example

This example project demonstrates the use of the PolarFire SoC MSS MMUART to 
transmit and receive data using interrupt and polling method of operations.

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces
connected. 
On connecting Discovery Kit J4 to the host PC, you should see 3 COM port interfaces
connected.

To use this project configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

The example project will display instructions over the serial port. A greeting
message and menu instructions are displayed over the UART terminal. Follow the
instruction to use different menu options provided by the example project.

This project provides build configurations and debug launchers as explained [here](https://mi-v-ecosystem.github.io/redirects/polarfire-soc-documentation-master_README)

# Renode Emulation
This example can be run on the Renode emulation platform. Build the application as normal, and then launch it using `mpfs-mmuart-interrupt renode all-harts start-platform-and-debug.launch`

UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-mmuart-interrupt renode all-harts debug`. More information about using Renode is available on https://renode.readthedocs.io

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\[BUILD_CONFIGURATION]\mpfs_hal_config\`

The ICICLE Kit Libero design used in this project can be found [here](https://github.com/polarfire-soc/icicle-kit-reference-design).

The Discovery Kit Libero design used in this project can be found [here](https://github.com/polarfire-soc/polarfire-soc-discovery-kit-reference-design)

This project provides build configurations and debug launchers, as explained [here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)
