# Power Saving Demo on PolarFire SoC

This example project demonstrates the use of the MPFS HAL for power saving.
The project uses the MSS DDR Controller, and its documentation can be found by
referring to [PolarFire® FPGA and PolarFire SoC FPGA Memory Controller](https://onlinedocs.microchip.com/pr/GUID-FF8061A7-7A15-470F-A6F5-E733C24D85F0-en-US-1/index.html) 
The application uses a serial terminal for user interaction. It prints
informative messages on the serial port at the start-up providing details of
the DDR initialization. Once DDR is trained, the E51 application will present a
menu to select further test options.

The PolarFire SoC MSS includes an embedded DDR controller to address the memory
solution requirements for a wide range of applications with varying power
consumption and efficiency levels. The DDR controller along with other blocks
external to MSS form the MSS DDR subsystem that can be configured to support
DDR3, DDR3L, DDR4, LPDDR3, and LPDDR4 memory devices.d

One of the key options in the menu available to the user is enabling DDR
self-refresh. DDR self-refresh is a power-saving mode used in dynamic
random-access memory (DRAM) devices. It allows the DRAM to retain data without
external clocking, thus performing its own auto-refresh cycles and results in
power saving. There are also features that enable a user to disable peripheral
clocks, use floating point units (FPU) and turn off peripheral RAMs. See the
following table to see the power comparisons with these features.

|| 1P1V Power Rail (mW) | Total Power (mW) | Total saved power percentage (%)|
|:----------------------|:----------|:------------|:------|
| Self-Refresh enabled             | 43.77     | 7886.18     |       |
| Self-Refresh disabled         | ==34.42== | 7875.56     | 0.13% (ref. 1P1V)|
| FPU disabled                      | 7287      | 7957.90     |       |
| FPU enabled                       | 7314      | ==7949.64== | 0.10% |

For peripherals that are enabled in the mss configurator:

|| 1P1V Power Rail (mW) | Total Power (mW) | Total saved power percentage (%)|
|:----------------------|:----------|:------------|:------|
| Peripheral clocks enabled         | 34.48     | 7872.39     |       |
| Peripheral clocks disabled        | 34.33     | ==7860.52== | 0.15% |
| All peripheral RAMs               | 7307      | 7954.69     |       |
| No peripheral RAMs                | 7344      | ==7725.84== | 2.96% |

## Target boards:

This example project is targeted at a number of different boards
 - MPFS Icicle Kit
 - MPFS Video Kit

| Board                     | MSS DDR memory Type| Data width | Speed       |
| :-------------            | :----------  | :----------  |:----------  |
|  MPFS Icicle Kit          | LPDDR4       |  x32         |  1600MHz    |
|  MPFS Video Kit           | LPDDR4       |  x32         |  1600MHz    |

## Libero Designs and MSS configuration files:

The reference design includes the MSS Configurator source file **MPFS_ICICLE_MSS_baremetal.cfg**
and the software configuration output file **MPFS_ICICLE_MSS_baremetal.xml**

The latest reference Libero design for **MPFS Icicle kit** is available at [Icicle Kit Reference Design](https://mi-v-ecosystem.github.io/redirects/repo-icicle-kit-reference-design).
The latest reference Libero design for **MPFS Video kit** is available at [Video Kit Reference Design](https://mi-v-ecosystem.github.io/redirects/repo-sev-kit-reference-design).

For the jumper settings and other board details for the **MPFS Icicle Kit**, refer to the [Icicle Kit user's guide](https://mi-v-ecosystem.github.io/redirects/updating-icicle-kit_updating-icicle-kit-design-and-linux).
For the jumper settings and other board details for the **MPFS Video Kit**, refer to the [Video Kit user's guide](https://mi-v-ecosystem.github.io/redirects/boards-mpfs-sev-kit-sev-kit-user-guide).

For all the boards supported with this project, the MSS Configurator generated
.cfg file and the .xml output generated from it are also part of this
SoftConsole project.

Please see **src/boards/\<my-board>/fpga_design/design_description>** directory
for each board.

#### Compile and debug

1. Select the \<board>-LIM-DEBUG build configuration and compile
2. Connect a serial terminal emulator to the com port associated with UART0
3. Run the "mpfs-hal-power-saving-demo hw all-harts debug.launch" debug launcher
4. Make sure that you see the following expected output on the terminal emulator port

#### Expected terminal emulator output

1. The following menu should appear if the DDR has trained successfully

  ```
  MPFS HAL Power Saving Options:
  1  How to turn on Parked Hart RAM at bootup
  2  How to turn off Parked Hart RAM at bootup
  3  How to turn on U54 Floating Point Units(FPU) at bootup
  4  How to turn off U54 Floating Point Units(FPU) at bootup
  5  How to turn on RAM of Unused Peripherals at bootup
  6  How to turn off RAM of Unused Peripherals at bootup
  7  Display DDR self refresh menu
  ```

2. When the DDR self refresh menu is displayed:

  ```
  DDR options:
  1  Clear pattern in memory
  2  Place pattern in memory
  3  Turn on ddr self refresh
  4  Turn off ddr self refresh
  5  Verify data in memory
  6  Go back to main menu
  ```

## UART configuration

On connecting UART to the host PC (see the individual board user guide), you
should see several COM port interfaces connected. This project requires MMUART0
for communication. Please see the board user guide for your target for
information on how to connect UARTs. Use the following settings in the serial
terminal.

- 115200 baud
- 8 data bits
- 1 stop bit
- no parity
- no flow control

## References

[PolarFire® FPGA and PolarFire SoC FPGA Memory Controller](https://onlinedocs.microchip.com/pr/GUID-FF8061A7-7A15-470F-A6F5-E733C24D85F0-en-US-1/index.html)  
[PolarFire® SoC MSS Technical Reference Manual](https://onlinedocs.microchip.com/pr/GUID-0E320577-28E6-4365-9BB8-9E1416A0A6E4-en-US-3/index.html)
[SoftConsole Releases](https://www.microchip.com/en-us/products/fpgas-and-plds/fpga-and-soc-design-tools/soc-fpga/softconsole#Download%20Software)