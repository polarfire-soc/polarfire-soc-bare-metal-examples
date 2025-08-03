
# Bare metal single hart(U54_1) payload example

This example project creates an executable which can be loaded by a boot-loader.
The program runs in m-mode.

The program runs on U54_1 only and prints messages on UART1.

See the file mss_sw_config.h for configuration details located in the following directory.

~~~
src\boards\icicle-kit\platform_config\mpfs_hal_config
~~~

The program can be loaded and [run using the Hart Software Systems (HSS)](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/driver-examples/mss/mpfs-hal/README.md).
Alternatively you can use the *mpfs-hal-ddr-demo* example program as a simple bootloader.

# Renode Emulation
This example can be run on the Renode emulation platform. Build the application as normal, and then launch it using `mpfs-hal-run-from-ddr-u54-1 renode all-harts start-platform-and-debug.launch`

UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-hal-run-from-ddr-u54-1 renode all-harts debug`. More information about using Renode is available on https://renode.readthedocs.io

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\mpfs_hal_config\`

If your project uses the default configuration file in `src\platform\platform_config_reference\` to enable DDR training, it is recommended to create a copy under the boards directory and disable `DDR_SUPPORT` there.
