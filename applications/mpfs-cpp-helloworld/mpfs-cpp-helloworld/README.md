# PolarFire SoC MSS C++ Example
This example project demonstrates the use of the PolarFire SoC MSS having
the mixture of "C" and "C++" code in a project.
This example prints the Hello World from the application code written in "C++"
and "C".

## How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
To use this project, configure the COM port **interface0** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

This is a self contained example project. A greeting message is displayed over
the UART terminal.

This project provides build configurations and debug launchers as explained
[here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples).

# Renode Emulation
This example can be run on the Renode emulation platform. Build the application as normal, and then launch it using `mpfs-cpp-helloworld renode all-harts start-platform-and-debug.launch`

UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-cpp-helloworld renode all-harts debug`. More information about using Renode is available on https://renode.readthedocs.io

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\mpfs_hal_config\`

If your project uses the default configuration file in `src\platform\platform_config_reference\` to enable DDR training, it is recommended to create a copy under the boards directory and disable `DDR_SUPPORT` there.
