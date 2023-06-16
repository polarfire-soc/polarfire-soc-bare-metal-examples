# PolarFire SoC DMA Benchmarking Application

## Table of Contents

- [PolarFire SoC DMA Benchmarking Application](#polarfire-soc-dma-benchmarking-application)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Requirements](#requirements)
    - [Tested on](#tested-on)
  - [Prerequisites](#prerequisites)
    - [Icicle Kit Reference Design](#icicle-kit-reference-design)
    - [Connecting to the Icicle Kit via UART](#connecting-to-the-icicle-kit-via-uart)
    - [Configuring the L2 Cache](#configuring-the-l2-cache)
    - [Configuring the DDR Demo project to load to L2-LIM](#configuring-the-ddr-demo-project-to-load-to-l2-lim)
  - [DMA Benchmarking Application](#dma-benchmarking-application)
    - [Structure](#structure)
    - [Configuration](#configuration)
    - [Running from: L2-LIM](#running-from-l2-lim)
      - [MPFS HAL DDR Demo Project](#mpfs-hal-ddr-demo-project)
      - [MPFS DMA Benchmarking Project](#mpfs-dma-benchmarking-project)
    - [Running from: Scratchpad Memory](#running-from-scratchpad-memory)
      - [MPFS DMA Benchmarking Project](#mpfs-dma-benchmarking-project-1)
    - [Running from: Cached DDR](#running-from-cached-ddr)
      - [MPFS HAL DDR Demo Project](#mpfs-hal-ddr-demo-project-1)
      - [MPFS DMA Benchmarking Project](#mpfs-dma-benchmarking-project-2)

## Introduction

This document describes how to use the DMA benchmarking application to benchmark the performance of
the Direct Memory Access (DMA) controllers on PolarFire SoC.
There are two types of DMA controllers on PolarFire Soc :

- The Platform DMA controller (P-DMA) which is located inside the CPU core complex.
- The CoreAXI4DMAController (F-DMA) is a DMA controller located in the FPGA fabric.

This document :

- Outlines the configuration options of the DMA benchmarking application.
- Describes how to set up the application to benchmark the performance of each DMA controller when executing
  from each of the memory locations the application is capable of running from.

The associated [DMA benchmarking results][1] document contains a full set of
results, in addition to a discussion of the performance of each DMA controller.

[1]: https://mi-v-ecosystem.github.io/redirects/polarfire-soc/benchmarks/Readme

## Requirements

- [PolarFire SoC Icicle Kit][2] (MPFS250T_ES-FCVG484E)

- The `AXI4_STREAM_DEMO` build configuration of the PolarFire SoC [Icicle Kit Reference Design][3].

- [SoftConsole v2022.2][4]  or later.

- A terminal emulator program, such as [Tera Term][5].

- The [mpfs-dma-benchmarking][6] bare metal project.

- The [mpfs-hal-ddr-demo][7] bare metal example project.

[2]: https://www.microchip.com/en-us/development-tool/MPFS-ICICLE-KIT-ES
[3]: https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design
[4]: https://www.microchip.com/en-us/products/fpgas-and-plds/fpga-and-soc-design-tools/soc-fpga/softconsole
[5]: https://ttssh2.osdn.jp/index.html.en
[6]: https://mi-v-ecosystem.github.io/redirects/demo-mpfs-dma-benchmarking
[7]: https://mi-v-ecosystem.github.io/redirects/polarfire-soc-bare-metal-examples-driver-examples-mss-mpfs-hal-ddr-demo-readme

### Tested on

- PolarFire SoC Icicle Kit (MPFS250T_ES-FCVG484E)

## Prerequisites

### Icicle Kit Reference Design

The `AXI4_STREAM_DEMO` configuration of the
[Icicle Kit reference design][3]
must be built and programmed onto your PolarFire SoC Icicle Kit to enable benchmarking of the the F-DMA
and concurrent benchmarking of both DMA controllers.

### Connecting to the Icicle Kit via UART

The project will display results via UART 1, to observe these messages:

1. Connect to the Icicle Kit via the USB - UART Terminal, J11.
2. Open up a serial connection to the board.
3. Choose the COM port corresponding to UART interface 1.
4. Set the baud rate to “115200”, set data bits to 8, and set flow control to none.

### Configuring the L2 Cache

To use this project with any of the `LIM-Release` or `DDR-Release` build configuration, the MPFS HAL
DDR demo project must be rebuilt using a new XML configuration file.

This XML file has been generated from the MSS configuration specific to this project.

The MPFS HAL DDR demo application requires a new MSS configuration with an increased Scratchpad memory
size of 1024KB, compared to the default MSS configuration which uses a Scratchpad memory size of 512KB.

For reference the new MSS configuration is also included with the project, it is located in the
`src/board/icicle-kit-es/mss_configuration` folder.

1. The new XML file can be found in the `src/boards/icicle-kit-es/fpga_design/design_description`
   folder of the `mpfs-dma-benchmarking` bare metal project.

2. Open the [mpfs-hal-ddr-demo][7] bare metal project.

3. Copy the new XML file (`ICICLE_MSS_mss_cfg.xml`) from the `mpfs-dma-benchmarking`  project into the
   `boards/icicle-kit-es/fpga_design/design_description` folder of `mpfs-hal-ddr-demo` project,
   replacing the existing XML file there.

### Configuring the DDR Demo project to load to L2-LIM

To run this project from L2-LIM the MPFS HAL DDR demo project must be modified slightly:

1. In `src/application/hart0/e51.c`, change the definition of `DDR_BASE` from:

   ```c
   #define DDR_BASE            0x80000000u
   ```

   to:

   ```c
   #define DDR_BASE            0x08020000u
   ```

2. In `src/application/hart1/u54_1.c`, change the 3rd argument of the `jump_to_application` function from:

   ```c
   jump_to_application(hls, M_MODE, (uint64_t)0x80000000);
   ```

   to:

   ```c
   jump_to_application(hls, M_MODE, (uint64_t)0x08020000);
   ```

## DMA Benchmarking Application

### Structure

The application contains 3 'application' folders that are used depending on which set of benchmarking
tests will be run.
They are:

- `application_pdma`: for benchmarking the performance of the P-DMA.
- `application_fdma`: for benchmarking the performance of the F-DMA.
- `application_concurrent`: for benchmarking the performance of both DMA controllers concurrently.

These 3 folders each have 4 associated build configurations, for executing the application from
Cached DDR memory, LIM memory or Scratchpad memory, and for debugging from LIM memory.

### Configuration

The application will display menus over `UART1` allowing a benchmarking test to be selected.

P-DMA transfer ordering can be turned on by defining the `FORCE_ORDER` macro in the header file located
in the same directory as `u54_1.c` file, in the `hart1/` directory.
Turning on transfer ordering will reduce P-DMA performance, for further information see the
[DMA benchmarking results][1] document.

### Running from: L2-LIM

To run the application from L2-LIM:

#### MPFS HAL DDR Demo Project

1. Complete the prerequisite: [configuring the L2 Cache](#configuring-the-l2-cache),
and the prerequisite: [configuring the DDR demo project to load to L2-LIM](#configuring-the-ddr-demo-project-to-load-to-l2-lim),
on how to configure the MPFS HAL DDR Demo project.
2. Connect to the Icicle Kit via the USB Embedded Programming connector, J33, and ensure that the J9
   is closed. Power on the board.
3. Set the active build configuration as `Icicle-Kit-eNVM-Scratchpad-Release` and build the project.
4. Program the eNVM of the Icicle Kit by running the "PolarFire SoC non-secure boot mode 1" external
   tool from the external tools menu.

#### MPFS DMA Benchmarking Project

1. Import the `mpfs-dma-benchmarking` project into SoftConsole.
2. Put the project in focus, set the active build configuration as `LIM-Release` and build the project.
3. Complete the prerequisite: [connecting to Icicle Kit via UART](#connecting-to-the-icicle-kit-via-uart),
   open a serial terminal connection to UART1.

4. Use Tera Term to open a serial terminal connection to UART0.
   - Change the baud rate to “115200”, set data bits to 8, and set flow control to none.

5. Power cycle your board, and observe UART0, wait for DDR testing to complete.

6. Once presented with the menu, select bootloader option 6 to "Load an image to DDR using YMODEM".

7. Load the `mpfs-dma-benchmarking.bin` binary file onto DDR of your Icicle Kit using YMODEM.
   1. In Tera Term select, `File->Transfer->YMODEM->Send`; a file explorer window will open.
   2. Navigate to the `LIM-Release` folder of the `mpfs-dma-benchmarking` project and select the `mpfs-dma-benchmarking.bin`
      binary file.

8. Select Bootloader option 7 to "Start the U54_1 from DDR @0x80000000".
    - Note: since the MPFS HAL DDR Demo project was modified in step 1, the u54_1 is actually starting
      from L2-LIM at address `0x08020000`, and not DDR address `0x80000000`.

9. Complete the prerequisite: [connecting to Icicle Kit via UART](#connecting-to-the-icicle-kit-via-uart),
observe UART1 and press `SPACE` when prompted to run the P-DMA benchmarks.

### Running from: Scratchpad Memory

#### MPFS DMA Benchmarking Project

To run the application from Scratchpad Memory:

1. Import the mpfs-dma-benchmarking project into SoftConsole.

2. Put the project in focus, set the active build configuration as
   `eNVM-Scratchpad-Release` and build the project.

3. Connect to the Icicle Kit via the USB Embedded Programming connector, J33,
   and ensure that the J9 is closed. Power on the board.

4. Run the project by selecting the
   `PolarFire SoC program non-secure boot mode 1` from the external tools menu.

5. Complete the prerequisite: [connecting to Icicle Kit via UART](#connecting-to-the-icicle-kit-via-uart),
observe UART1 and press `SPACE` when prompted to run the P-DMA benchmarks.

### Running from: Cached DDR

To run the application from Cached DDR memory:

#### MPFS HAL DDR Demo Project

1. Note: if the mpfs-hal-ddr-demo project was changed to load to L2-LIM, as described
   in the [fourth prerequisite](#configuring-the-ddr-demo-project-to-load-to-l2-lim),
   revert these changes.
2. Follow the prerequisite: [configuring the L2 Cache](#configuring-the-l2-cache)
on how to configure the MPFS HAL DDR Demo project.
3. Connect to the Icicle Kit via the USB Embedded Programming connector, J33, and ensure that the J9
   is closed. Power on the board.
4. Set the active build configuration as `Icicle-Kit-eNVM-Scratchpad-Release` and build the project.
5. Program the eNVM of the Icicle Kit by running the "PolarFire SoC non-secure boot mode 1" external
   tool from the external tools menu.

#### MPFS DMA Benchmarking Project

1. Import the `mpfs-dma-benchmarking` project into SoftConsole.

2. Put the project in focus, set the active build configuration as `DDR-Release`
   and build the project.

3. Complete the prerequisite: [connecting to Icicle Kit via UART](#connecting-to-the-icicle-kit-via-uart).

4. Use Tera Term to open a serial terminal connection to UART0.
   - Change the baud rate to “115200”, set data bits to 8, and set flow control to none.

5. Power cycle your board, and observe UART0, wait for DDR training to complete.

6. Once presented with the menu, select bootloader option 6 to "Load an image to DDR using YMODEM".

7. Load the `mpfs-dma-benchmarking.bin` binary file onto DDR of your Icicle Kit using YMODEM.
   1. In Tera Term select, `File->Transfer->YMODEM->Send`; a file explorer window will open.
   2. Navigate to the `DDR-Release` folder of the `mpfs-dma-benchmarking` project and select the
      `mpfs-dma-benchmarking.bin` binary file.

8. Select Bootloader option 7 to "Start the U54_1 from DDR @0x80000000".

9. Complete the prerequisite: [connecting to Icicle Kit via UART](#connecting-to-the-icicle-kit-via-uart),
   observe UART1 and press `SPACE` when prompted to run the P-DMA benchmarks.
