# Bare Metal Examples
This repository provides bare metal application example projects for SoftConsole toolchain.
These examples serve as an easy starting point for the users to start exploring PolarFire&reg; SoC.
Each project provides ready to use build configurations and debug launch configurations.

## Build configurations
The build configurations configure the projects to work in a specific way. E.g. Optimization level -Os, linking to DDR memory address space etc..
Below is the list of all the build configurations provided with each project. You may use the existing build configurations or create your own for your project.

|Configuration              | Description                                                                                                |
|------------------------- | ---------------------------------------------------------------------------------------------------------- |
|LIM-Debug                  | Download to and debug from LIM memory. Not-optimized (-O0). (Could be used with boot mode0)                 |
|LIM-Release                | Download to and debug from LIM memory. Optimized (-Os). (Could be used with boot mode 2)                   |
|DDR-Release                | Execute from cached DDR memory – typically via a bootloader. Optimized (-Os).                              |
|eNVM-Scratchpad-Release    | Booting from eNVM, program relocates itself to scratchpad memory and continues execution. Optimized (-Os). (Could be used with boot mode 1)|

The build configurations is a particular combination of settings done at different places in the SoftConsole project.
Following sections describe various types of configurations accomplished by the default build configurations.

### SoftConsole project settings
Some of the important project settings are explained in following sections.

#### Debug build configurations
The Build configurations post-fixed with '-Debug' are intended for the early development and debug phase of the project. By Conventions, the *-Debug configurations uses optimization level -O0 and generate maximum symbol information.

#### Release build configurations
The Build configurations post-fixed with '-Release' are intended for the final production release where executable stored in non-volatile memory is running after power-on-reset or the executable is launched by a previous stage bootloader. By Conventions, the *-Release configurations uses optimization level (-Os) and does not generate symbol information. It also defines a NDEBUG macro which is used to avoid any debug
code to be excluded from the build.

**Linker scripts:** Each build configuration needs a linker script. The linker script describes the memory layout of the executable. Each build configuration selects an appropriate linker scripts via project settings. E.g. eNVM-Scratchpad-Release uses mpfs-envm-lma-scratchpad-vma.ld.

There are several other settings that are required for a project. For complete project settings go to \<proect-name>->Properties->settings->Tool settings.

### Settings via header files
Apart from the SoftConsole project settings, each project needs few more configurations. These configurations are categorized into hardware and software configurations.

#### Hardware configurations
The hardware configurations are located in \<project-root>/src/boards/\<target-board> folder. The include files in the \<project-root>/src/boards/\<target-board>/fpga_design_config folder define the hardware configurations such as clocks. You must make sure that the configurations in this example project match the actual configurations of your target Libero&reg; design that you are using to test this example project. The design configuration data generated from your libero design must be placed under \<project-root>/src/boards/\<target-board>/fpga_design/design_description folder.

#### Software configurations
The default software configurations are stored under \<project-root>/platform/platform_config_reference folder. If you need to change the default software configurations, you are advised to create a new folder to replicate this folder under the \<project-root>/src/boards/ directory and do the modifications there. It would look like \<project-root>/src/boards/\<target-board>/platform_config

The include files in the "platform_config" folder define the software configurations such as how many harts are being used in the software, what is the tick rate of the internal timer of each hart etc.. These configurations have no dependency on the hardware configurations in "fpga_design_config" folder. Note that changing these software configurations may require a change in your application code.

*IMAGE_LOADED_BY_BOOTLOADER:*

One of the important software configurations is to configure IMAGE_LOADED_BY_BOOTLOADER in the mss_sw_config.h. We set IMAGE_LOADED_BY_BOOTLOADER = 0 when no previous stage bootloader is used. e.g. application stored in non-volatile memory starts running after reset. The default software configuration uses this setting.

Set IMAGE_LOADED_BY_BOOTLOADER = 1 when you want the application's executable image to be loaded by a previous stage bootloader. The DDR-Release is one such configuration which uses this setting. The modified mss_sw_config.h can be found  under the \<project-root>/src/boards/\<icicle-kit-es>/platform_config folder.

## Debug launchers
There are following two preconfigured debug launchers provided with each project.

|Configuration              | Description                                                                                                |
|---------------------------|------------------------------------------------------------------------------------------------------------|
|_\<project name> hw all-harts debug.launch_ | Intended to be used with xxx-Debug configurations. Resets harts.<br> Downloads the executable and the symbols to the memory. Sets up PC to start location. |
|_\<project name> hw all-harts attach.launch_ | Intended to be used with xxx-Release configurations. Does not reset harts. <br> Downloads only the symbol information and attaches to harts. Typically used to check the current state of harts. If the project is linked to RAM memory such as DDR, you will be able to put breakpoints and step debug further.                   |

Both the launchers are configured to use currently _active_ build configuration, hence the same launcher can be used with any of the build configuration. Make sure that an appropriate build configuration is set as _active_ to avoid issues. Trying to use _all-harts attach.launch_ while a xxx-Debug build is active may not work.

You may change the existing debug launchers or create your own for your project.

## Target hardware
All the projects are tested on the PolarFire SoC Icicle kit. Most of the projects are tested with the latest available PolarFire SoC Reference Libero design or one of it's variants. Please refer **README.md in the root directory of each project** to know more.

### Further reading
For the latest releases of the MPFS HAL, peripheral device drivers and default reference platform configurations refer [PolarFire SoC platform](https://github.com/polarfire-soc/platform).

For more details on the PolarFire SoC Hardware abstraction layer (MPFS HAL) refer [MPFS HAL documentation](https://github.com/polarfire-soc/polarfire-soc-documentation/bare-metal-documentation).

For more details on the SoftConsole Bare metal software project folder structure refer [Bare Metal Software Projects Structure](https://github.com/polarfire-soc/polarfire-soc-documentation/bare-metal-documentation).

For more information on boot modes [PolarFire SoC Boot Modes](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/fundamentals/boot-modes/boot-modes-fundamentals.md).

___
Libero&reg; and PolarFire&reg; are the property of Microchip Technology Inc. (MCHP).
