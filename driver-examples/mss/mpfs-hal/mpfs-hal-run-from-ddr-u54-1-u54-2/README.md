
# Bare metal SMP payload example


This example project creates an executable which can be loaded by a boot-loader.
The program runs in m-mode. 

The program runs on U54_1 and U54_2 only and prints messages on UART1 and UART2 respectively.

See the file mss_sw_config.h for configuration details located in the following directory.

~~~
src\boards\icicle-kit-es\platform_config\mpfs_hal_config
~~~

The program can be loaded and [run using the Hart Software Systems (HSS)](https://github.com/polarfire-soc/polarfire-soc-bare-metal-examples/blob/main/driver-examples/mss/mpfs-hal/README.md).
Alternatively you can use the *mpfs-hal-ddr-demo* example program as a simple bootloader.

