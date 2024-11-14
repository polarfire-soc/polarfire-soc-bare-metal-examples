# Power Saving Demo on PolarFire SoC

This example project demonstrates power saving options available with the MPFS
SoC. API functions have been added to the MPFS HAL to help utilize these power
saving features. The project uses the MSS DDR Controller, and its documentation
can be found by referring to
[PolarFire® FPGA and PolarFire SoC FPGA Memory Controller](https://ww1.microchip.com/downloads/aemDocuments/documents/FPGA/ProductDocuments/UserGuides/PolarFire_FPGA_PolarFire_SoC_FPGA_Memory_Controller_User_Guide_VB.pdf)

The following power saving features are demonstrated in this project. You can
also find measured power figures for each of the features further on in this
document:
- DDR Self-Refresh
- DDR Controller and PHY Clock Controller (ON/OFF)
- MSS System Clock Scaling
- Turning off Peripheral RAM
- Turning off RAM of unused HARTS
- Periodic low power mode
- Enabling low power mode via state machine handshake

The application uses a serial terminal for user interaction. It prints
informative messages on the serial port at the start-up providing details of
the DDR initialization. Once DDR is trained, the E51 application will present a
menu to select further test options.

One of the key options in the menu available to the user is enabling DDR
self-refresh. DDR self-refresh is a power-saving mode used in dynamic
random-access memory (DRAM) devices. It allows the DRAM to retain data without
external clocking, by performing its own auto-refresh cycles. This leads to a
significant reduction in power consumption by the memory chip. There are also
features that enable a user to disable peripheral clocks, disable DDR PLL,
disable peripheral RAMs, and scale the CPU frequency to half its default state.
The user is also able to check the current monitor status within the menus.

The following table shows to where the power rails supply.

| Power rail | Description |
|:-|:-|
| VDD             | Device core digital supply          |
| 1P1V power rail | Supply for MSS DDR (DDR power rail) |

These tests were conducted on a PolarFire SoC Video Kit using the standard
reference design and MSS configuration. Results will vary depending on several
factors, including the FPGA design used and MSS configuration (such as the base
clock speed and peripherals enabled). In this example project, there is a soft
reset applied to the FPGA fabric upon startup. For accurate results, this demo
should be run using an intended target configuration. See the following tables
to see the power comparisons with these features.

|| 1P1V power rail (mW) | 1P1V saved power percentage (%) |
|:-|:-|:-|
| DDR Self-Refresh disabled | 43.77  |        |
| DDR Self-Refresh enabled  | 34.42  |  ~27%  |

|| VDD power rail (mW) | VDD saved power percentage (%) |
|:-|:-|:-|
| MSS DDR Controller PLL outputs enabled      | 950.18    |        |
| MSS DDR Controller PLL outputs disabled     | 690.20    |  ~27%  |

|| VDD power rail (mW) | Total saved power percentage (%) |
|:-|:-|:-|
| All MSS Peripheral RAMs enabled  | 950.63 |     |
| All MSS Peripheral RAMs disabled | 927.74 | ~2% |

The following table shows the clock scaling numbers.

| Unscaled Clock Values | Scaled Clock Values |
|:-|:-|
| PLL output frequency - 600MHz      | PLL output frequency - 600MHz         |
| CPU clock divider 1 (x/1) - 600MHz | CPU clock divider 2 (x/2) - 300MHz    |
| AXI clock divider 2 (x/2) - 300MHz | AXI clock divider 4 (x/4) - 150MHz    |
| APB clock divider 4 (x/4) - 150MHz | APB clock divider 4 (x/4) - 150MHz    |

|| VDD power rail (mW) | VDD saved power percentage (%) |
|:-|:-|:-|
| MSS PLL clock at 600MHz      | 952.32    |        |
| MSS PLL clock at 300MHz      | 777.60    |  ~22%  |

If all MSS power saving options are implemented:

| Power Saving Feature | VDD Power (mW) | VDD saved power percentage (%) |
|:-|:-|:-|
| Full power mode                         | 951.40    |        |
| MSS PLL clock scaled to 300MHz (mW)     | - 174.72  |        |
| MSS DDR Controller PLL disabled (mW)    | - 261.24  |        |
| All MSS Peripheral RAMs disabled        | - 22.89   |        |
| Lowest power mode                       | 492.55    |  ~52%  |

## Target boards:

This example project is targeted at a number of different boards
 - MPFS Icicle Kit
 - MPFS Video Kit

| Board | MSS DDR memory Type | Data width | Speed |
|:-|:-|:-|:-|
| MPFS Icicle Kit | LPDDR4              | x32        | 1600MHz     |
| MPFS Video Kit  | LPDDR4              | x32        | 1600MHz     |

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
  This program is run from E51

  MPFS HAL Power Saving Options:
  1  Display bootup option menu
  2  Display DDR menu
  3  Display clock scaling menu
  4  Display maximum power-saving menu
  5  Display state machine menu
  6  Toggle periodic low power mode
  c  Display PAC1934 current monitor values

  Type 0 to show the menu again
  ```

2. When the bootup menu is displayed:

  ```
  Select a bootup option:

  MPFS HAL Power Saving Options:
  1  How to toggle ON/OFF Parked Hart RAM at bootup
  2  How to toggle ON/OFF RAM of Unused Peripherals at bootup
  c  Display PAC1934 current monitor values
  m  Go back to main menu

  Type 0 to show the menu again
  ```

3. When the DDR menu is displayed:

  ```
  Select a DDR option:

  Make sure that u54_1 hart is turned on before selecting an option:
  1  Clear pattern in memory block (<1 minute)
  2  Place pattern in memory block (<1 minute)
  3  Verify if pattern is in memory (<2 minutes)
  4  Turn on ddr self refresh
  5  Turn off ddr self refresh
  6  Check ddr self refresh status
  7  Turn off ddr pll outputs
  8  Turn on ddr pll outputs
  c  Display PAC1934 current monitor values
  m  Go back to main menu
  WARNING: DDR is not accessible when in self-refresh mode, or PLL is disabled

  Type 0 to show the menu again
  ```

4. When the clock scaling menu is displayed:

  ```
  Select a clock scaling option:

  Make sure that u54_1 hart is turned on before selecting an option:
  1  Change CPU clock frequency to 300MHz (half)
  2  Change CPU clock frequency to 600MHz (default)
  3  Display clock status
  c  Display PAC1934 current monitor values
  m  Go back to main menu

  Type 0 to show the menu again
  ```

5. When the max power-saving menu is displayed:

  ```
  Select a max power-saving option:

  Make sure that u54_1 hart is turned on before selecting an option:
  1  Toggle maximum power-saving mode with clock scaling
  2  Toggle maximum power-saving mode without clock scaling
  3  Reset to default settings
  4  Display clock status
  c  Display PAC1934 current monitor values
  m  Go back to main menu

  Type 0 to show the menu again
  ```

6. When the state machine menu is displayed:

  ```
  Select a handshake option:

  1  Start app by sending request to u54_1 core
  2  Get state machine status
  c  Display PAC1934 current monitor values
  m  Go back to main menu

  Type 0 to show the menu again
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