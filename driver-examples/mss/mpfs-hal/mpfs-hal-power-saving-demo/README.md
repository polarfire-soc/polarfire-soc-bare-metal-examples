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
clocks, use floating point units (FPU), turn off peripheral RAMs, and scale the
CPU frequency to half its default state.

See the following tables to see the power comparisons with these features.

|| 1P1V power rail (mW) | 1P1V saved power percentage (%)|
|:-|:-|:-|
| Self-Refresh disabled | 43.77  |        |
| Self-Refresh enabled  | 34.42  |  ~27%  |

|| VDDI1 power rail (mW) | VDDI1 saved power percentage (%) |
|:-|:-|:-|
| FPU disabled      | 13.71    |        |
| FPU enabled       | 0.61     |  ~95%  |

|| VDD power rail (mW) | VDD saved power percentage (%) |
|:-|:-|:-|
| Clock scaling default (600MHz)      | 838.22    |        |
| Clock scaling half (300MHz)         | 659.42    |  ~20%  |

For peripherals that are enabled in the mss configurator:

|| 3P3V power rail (mW) | 5P0V power rail (mW) | 1P2V power rail (mW) | Total power (mW) | Total saved power percentage (%) |
|:-|:-|:-|:-|:-|:-|
| Peripheral clocks enabled  |  4572.48  |  1256.76  |  744.79  |  7872.39  |        |
| Peripheral clocks disabled |  4567.36  |  1250.51  |  742.83  |  7860.52  | ~0.15% |

|| VDD power rail (mW) | VDD25 power rail (mW) | 3P3V power rail (mW) | 5P0V power rail (mW) | 1P2V power rail (mW) | Total power (mW) | Total saved power percentage (%) |
|:-|:-|:-|:-|:-|:-|:-|:-|
| All peripheral RAMs | 691.11 | 38.98 | 4606.02 | 1253.51 | 742.38 | 7954.69 |     |
| No peripheral RAMs  | 643.96 | 30.52 | 4472.40 | 1225.36 | 731.29 | 7725.84 | ~3% |

If all power saving options are implemented:

|| Self-refresh enabled (mW) | Peripheral clocks disabled (mW) | FPU Enabled (mW) | Peripheral RAMs disabled (mW) | Clock scaling half (mW) | Total power (mW) |
|:-|:-|:-|:-|:-|:-|:-|
| Total power average (TPA) |       |       |      |        |        | 7917.80 |
| Power saved (PS)          | 10.63 | 11.87 | 8.27 | 222.85 | 178.80 | 432.42  |
| TPA - PS                  |       |       |      |        |        | 7485.38 (~5% power saved in total) |


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
  8  Display clock scaling menu

  Type 0 to show the menu again
  ```

2. When the DDR self refresh menu is displayed:

  ```
  Select the DDR self refresh test:

  Make sure that u54_1 hart is turned on before selecting an option:
  1  Clear pattern in memory
  2  Place pattern in memory
  3  Verify if pattern is in memory
  4  Turn on ddr self refresh
  5  Turn off ddr self refresh
  6  Check ddr self refresh status
  7  Go back to main menu
  WARNING: DDR is not accessible when in self-refresh mode

  Type 0 to show the menu again
  ```

3. When the clock scaling menu is displayed:

  ```
  Select a clock frequency option:

  Make sure that u54_1 hart is turned on before selecting an option:
  1  Change CPU clock frequency to 300MHz (half)
  2  Change CPU clock frequency to 600MHz (default)
  3  Display clock status
  7  Go back to main menu

  Type 0 to show the menu again;
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