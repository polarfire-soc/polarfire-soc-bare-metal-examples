
#                         mpfs-pmp-app-u54-1

This example project creates an appliction which is loaded by hart-software-services(HSS). 
U54_1 executes this application in M-mode.

This appliction provides user options to validate PMP.

#### hss-payload-generator

The payload generator tool is used to create payload.bin from mpfs-pmp-app-u54-1.elf.
HSS ymodem is invoked to copy the payload.bin to SD or eMMC.

**Payload generator** [https://github.com/polarfire-soc/hart-software-services/tree/master/tools/hss-payload-generator](https://github.com/polarfire-soc/hart-software-services/tree/master/tools/hss-payload-generator)

#### Loading application to LPDDR4
On board power-up, the payload.bin will be copied to LPDDR4 from SD or eMMC and executed.

# Renode Emulation
This example can be run on the Renode emulation platform. Build the application as normal, and then launch it using `mpfs-pmp-app-u54-1  renode all-harts start-platform-and-debug.launch`

UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-pmp-app-u54-1 renode all-harts debug`. More information about using Renode is available on https://renode.readthedocs.io

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\mpfs_hal_config\`

If your project uses the default configuration file in `src\platform\platform_config_reference\` to enable DDR training, it is recommended to create a copy under the boards directory and disable `DDR_SUPPORT` there.
