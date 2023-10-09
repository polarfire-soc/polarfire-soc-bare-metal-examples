# PolarFire SoC MSS Timer example

This example project demonstrates the use of the PolarFire SoC MSS timer.
The Timer is configured in either 32 bit or 64 bit mode. In 32 bit mode there are 
two configurations as follows.

1. **Periodic IRQ** -: When the timer counter reaches zero an interrupt will be generated, then 
the counter will be loaded again with a value which is defined through **MSS_TIM1_load_immediate()** function. 
The time interval between two interrupts will be equal hence periodic. 


2. **Single-Shot IRQ** -: The Timer counter is loaded with a value which is defined through **MSS_TIM1_load_immediate()**
function . An interrupt will be generated when this value runs down to 0 and the Timer counter is not loaded again unlike
*Periodic IRQ* mode.

*Note* -: Timer1 has been used to demonstrate 32 bit timer configurations in this example project. 

In a 64 bit configurations, Timer has been configured to generate a periodic 64 bit timer interrupt. 
In this mode, the two 32-bit timers are used to realize a 64-bit timer. 

In the 64 bit mode, updating the various “counter-value” registers requires two 32-bit 
writes. The upper 32-bit word must be written first. The writes only take effect when 
the second write (to the lower 32 bits) is complete.

*Note* -: We have used 64 bit periodic mode to demonstrate 64 bit timer functions. It can also be configured in single-shot mode. 

## How to use this example

To use this project you will need a UART1(interface1) and UART2(interface2) configured as below:

- 115200 baud
- 8 data bits
- 1 stop bit
- no parity
- no flow control

Following options will be displayed on UART1 terminal. 

## Options

1. Timer1 as 32 bit in periodic mode (an interrupt will be raised periodically after the timer value reaches 0)
2. Timer1 as 32 bit in timer in one-shot mode (an interrupt will be raised only once after the timer value reaches 0)
3. Timer64 periodic mode (an interrupt will be raised when a 64 bit time value reaches 0)
4. Configure Timer1 to generate interrupts at non uniform interval using background load API (Timer1 will be configured in periodic mode but the time between two interrupts will be decided by the value passed by *MSS_TIM1_background_load()* function) 


Upon choosing an option you will see messages appearing on the UART2 terminal when timer interrupt occurs .
In the case of background load the messages will appear at different preprogrammed time intervals.  

This project provides build configurations and debug launchers as explained [here](https://mi-v-ecosystem.github.io/redirects/repo-polarfire-soc-bare-metal-examples)

# Renode Emulation
This example can be run on the Renode emulation platform. Build the application as normal, and then launch it using `mpfs-timer-example renode all-harts start-platform-and-debug.launch`

UART console display and logging options can be found under the Startup tab in `Debug Configurations` > `mpfs-timer-example renode all-harts debug`. More information about using Renode is available on https://renode.readthedocs.io

### DDR training and Renode
If the firmware has DDR training enabled, then the application will take significantly longer to start up in Renode. Training has no practical impact in this environment as the emulated DDR memory is already reliable.

Training can be controlled by removing `#define DDR_SUPPORT` in your `mss_sw_config.h` file. This change should be made in `src\boards\[BOARD]\platform_config\mpfs_hal_config\`

If your project uses the default configuration file in `src\platform\platform_config_reference\` to enable DDR training, it is recommended to create a copy under the boards directory and disable `DDR_SUPPORT` there.
