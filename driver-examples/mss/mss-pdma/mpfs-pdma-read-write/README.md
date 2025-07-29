# PolarFire SoC MSS PDMA Driver example

This example project demonstrates the use of the PolarFire SoC MSS Platform
DMA driver to configure the DMA channel and initiate the transaction between
source and destination memory locations for polling mode and interrupt mode of
operation.
Success and error status in the transactions are reported by respective
interrupts.

# How to use this example
On connecting Icicle kit J11 to the host PC, you should see 4 COM port interfaces.
On connecting Discovery kit J4 to the host PC, you should see 3 COM port interfaces.
To use this project, configure the COM port **interface1** as below:
 - 115200 baud
 - 8 data bits
 - 1 stop bit
 - no parity
 - no flow control

This is a self contained example project. A greeting message is displayed
over the MMUART terminal indicating the  DMA channels for transaction.
User need to select the appropriate DMA channel(0 - 3) and the driver will
configure the selected channel. Once the channel is successfully configured,
transaction will initiate. Error or Success status of the transaction is
displayed over the UART terminal.

The application can be used to demonstrate the PDMA channel 0-3 in polling or
interrupt mode. User will have to enable the MSS_PDMA_INTERRUPT_MODE macro from
application and configure the interrupt bits in channel configuration properly
to use it in interrupt mode.

User can repeat the process to verify the transactions on different DMA channel.

This project provides build configurations and debug launchers as explained
[here](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/README.md)

# Renode Emulation
This example can be run on the Renode emulation platform. UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-pdma-read-write renode all-harts debug`. More information about using Renode is available on https://renode.readthedocs.io


The following configuration can be launched using `mpfs-pdma-read-write renode all-harts start-platform-and-debug.launch`:

 - LIM-Debug
 - LIM-Release
 - eNVM-Scratchpad-Release
 
The following configuration can be launched using `mpfs-pdma-read-write renode bootload-platform-and-debug.launch`:

 - DDR-Release

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\mpfs_hal_config\`

If your project uses the default configuration file in `src\platform\platform_config_reference\` to enable DDR training, it is recommended to create a copy under the boards directory and disable `DDR_SUPPORT` there.
