=================================================================
 PolarFire SoC MSS Ethernet MAC Test program - Simple web server 
=================================================================

This program implements a simple web server example using FreeRTOS and the
LwIP TCP/IP stack with the PolarFire SoC MSS MAC Ethernet driver. This can be
used to exercise the driver on the G5 SoC Emulation Platform with the Peripheral
Daughter Board or the Icicle Kit.

Please note that the sample application assigns a MAC address for the GEM 0 of
00:FC:00:12:34:56 and 00:FC:00:12:34:57 for GEM 1. If you are using multiple 
instances of this software on the same network, you will need to modify 
low_level_init() in e51.c to ensure each GEM device has a unique MAC address.

The software has a fixed IP address of 10.2.2.20/255.255.255.0 If you point
a browser at that address you should see a simple web page display some
information about the system and a count that updates each time the page
auto refreshes.

Note: When testing this functionality, the recommended method is to use a
separate ethernet port on the PC with a fixed IP address in the 10.2.2.x range.
If your PC does not have multiple Ethernet ports, you can modify the IP address
settings of the Firmware to match your network address range or use a USB to
Ethernet adapter to provide a dedicated link for testing - we would recommend
using the USB to Ethernet adapter as this provides isolation from your normal
network.

The IP address for the web server can be changed by editing the
prvEthernetConfigureInterface() function in e51.c.

The following project defines are used to configure the system:

Define these to enable support for FreeRTOS and LwIP in the driver.

USING_FREERTOS
USING_LWIP

LwIP specific defines

LWIP_COMPAT_MUTEX
LWIP_SKIP_PACKING_CHECK
LWIP_PROVIDE_ERRNO

G5_SOC_EMU_USE_GEM0 - Define this to test GEM 0
G5_SOC_EMU_USE_GEM1 - Define this to test GEM 1

Configurations for VTSS API for VSC8575

VTSS_CHIP_CU_PHY
VTSS_FEATURE_SYNCE
VTSS_FEATURE_PHY_TS_ONE_STEP_TXFIFO_OPTION
VTSS_FEATURE_SERDES_MACRO_SETTINGS
VTSS_OPT_PORT_COUNT=4
VTSS_OPT_VCORE_III=0
VTSS_PRODUCT_CHIP="PHY"
VTSS_PHY_API_ONLY
VTSS_OPT_TRACE=0
VTSS_OS_BARE_METAL_RV

CMSIS_PROT
TARGET_G5_SOC   - Define when building for G5 SoC Emulation Platform
PSE=1           - Define as 1 when building for G5 SoC Emulation Platform, 0
                  otherwise.
                  
MSS_MAC_64_BIT_ADDRESS_MODE - Sets address bus width for DMA
MSS_MAC_SIMPLE_TX_QUEUE     - Set this, multipacket TX queue is not implemented
CALCONFIGH=\"config_user.h\"
TEST_H2F_CONTROLLER=0
_ZL303XX_MIV

The MSS_MAC_HW_PLATFORM macro should be defined in mss_ethernet_mac_user_config 
with a value that identifies the hardware platform and interface modes to use.

Valid values are:

    MSS_MAC_DESIGN_EMUL_GMII         - G5 Emulation Platform VSC8575 designs
                                       with GMII to SGMII bridge on GEM0
    MSS_MAC_DESIGN_EMUL_TBI          - G5 Emulation Platform VSC8575 designs
                                       with TBI to SGMII bridge on GEM0
    MSS_MAC_DESIGN_EMUL_TI_GMII      - G5 Emulation Platform DP83867 design with
                                       GMII to SGMII bridge
    MSS_MAC_DESIGN_EMUL_DUAL_EX_TI   - G5 Emulation Platform Dual GEM design 
                                     - with external TI PHY on GEM1 (GMII)
    MSS_MAC_DESIGN_EMUL_DUAL_EX_VTS  - G5 Emulation Platform Dual GEM design
                                       with external Vitess PHY on GEM0 (GMII)
    MSS_MAC_DESIGN_EMUL_GMII_GEM1    - G5 Emulation Platform VSC8575 designs 
                                       with GMII to SGMII bridge on GEM 1
    MSS_MAC_DESIGN_EMUL_TBI_GEM1     - G5 Emulation Platform VSC8575 designs
                                       with TBI to SGMII bridge GEM1
    MSS_MAC_DESIGN_EMUL_TBI_GEM1_TI  - G5 Emulation Platform DP83867 designs 
                                       with TBI to SGMII bridge GEM0
    MSS_MAC_DESIGN_SVG_GMII_GEM0     - SVG Board with GEM0 GMII
    MSS_MAC_DESIGN_SVG_GMII_GEM1     - SVG Board with GEM1 GMII
    MSS_MAC_DESIGN_SVG_SGMII_GEM0    - SVG Board with GEM0 SGMII
    MSS_MAC_DESIGN_SVG_SGMII_GEM1    - SVG Board with GEM1 SGMII
    MSS_MAC_DESIGN_ICICLE_SGMII_GEM0 - Icicle board with GEM0 SGMII.
    MSS_MAC_DESIGN_ICICLE_SGMII_GEM1 - Icicle board with GEM1 SGMII.
    MSS_MAC_DESIGN_ICICLE_STD_GEM0   - Icicle board GEM0 Standard Reference
                                       Design
    MSS_MAC_DESIGN_ICICLE_STD_GEM1   - Icicle board GEM1 Standard Reference
                                       Design

The serial port baud rate is 115200.

To use this project you will need a UART terminal configured as below:

    - 115200 baud
    - 8 data bits
    - 1 stop bit
    - no parity
    - no flow control

The serial interface can be used to confirm the application is running as it
will echo any typed characters.

Icicle Standard design has the following setup for the VSC8662:

  NRESET is connected to an MCP121T active low open drain power on reset.
  chip. The signal is also connected to pin C12 of the G5SoC - GPIO0NB1.

  NSRESET is connected to pin D16 of the G5SoC - GPIO10NB1.

  CMODE 0 to 2 are connected to 0R resistors to ground or VCC. They also
  connect to the G5SoC but the use of 0R resistors means they cannot
  be changed via the I/O. The current default strapping is:

    CMODE 0 - 0 (C16)
    CMODE 1 - 1 (C17)
    CMODE 2 - 0 (B19)

    This selects a PHY address range of 8 to 11 for the device i.e. port 0
    is at address 8, port 1 is at address 9 and addresses 10 and 11 are
    reserved.
    These pins on the G5SoC should be configured as inputs to avoid issues.

  CMODE 3 to 7 are currently floating and connect to G5SoC as follows and should
  be driven to 0 for consistent operation:

    CMODE 3 - D18
    CMODE 4 - A18
    CMODE 5 - B18
    CMODE 6 - A12
    CMODE 7 - B12

  GPIO  0/Sigdet 0 is connected to A10 - GPIO177PB1
  GPIO  1/Sigdet 1 is connected to A11 - GPIO177PB1
  GPIO  2 is not connected
  GPIO  3 is not connected
  GPIO  4 is not connected
  GPIO  5/I2C_SCL1 is not connected
  GPIO  6 is not connected
  GPIO  7 is not connected
  GPIO  8 is not connected
  GPIO  9/FASTLINKFAIL is connected to C11 - GPIO178PB1
  GPIO 10 is not connected
  GPIO 11 is not connected
  GPIO 12 is not connected
  GPIO 13 is not connected
  GPIO 14 is not connected
  GPIO 15 is not connected

  RXD0P is connected to L5
  RXD0N is connected to L6
  TXD0P is connected to N6
  TXD0N is connected to N7
  RCVRD_CLK1 is connected to F17
  RXD1P is connected to K6
  RXD1N is connected to K7
  TXD1P is connected to M7
  TXD1N is connected to N8
  RCVRD_CLK2 is connected to E15

  MDINT0 is connected to E16
  MDINT1 is connected to B10
  MDC is connected to D3 - MSSIO28B2
  MDIO is connected to C2 - MSSIO29B2

  OSCEN is connected to E18 however there is a 0R resistor R344 marked as NL...
  CLKOUT is connected to A13 - GPIO1PB1/CLKIN_S_5
  PLLMODE is connected to D12

  For this design, the jumper settings must be set as follows:
    Jumper Setting
    J15    Open
    J17    Open
    J24    Open
    J28    Closed
    J31    Open
    J34    1 & 2
    J35    1 & 2
    J43    1 & 2
    J45    1 & 2
    J46    Closed

Icicle board test design has the following setup for the VSC8662:

  NRESET is connected to an MCP121T active low open drain power on reset
  chip. The signal is also connected to pin C12 of the G5SoC - GPIO0NB1.
  This connects to MSS GPIO 2 M2F 2

  NSRESET is connected to pin D16 of the G5SoC - GPIO10NB1.
  This connects to MSS GPIO 2 M2F 3

  CMODE 0 to 2 are connected to 0R resistors to ground or VCC. They also
  connect to the G5SoC but the use of 0R resistors means they cannot
  be changed via the I/O. The current default strapping is:

    CMODE 0 - 0 (C16 - GPIO12PB1)
    CMODE 1 - 1 (C17 - GPIO15NB1)
    CMODE 2 - 0 (B19 - GPIO19NB9)

    This selects a PHY address range of 8 to 11 for the device i.e. port 0
    is at address 8, port 1 is at address 9 and addresses 10 and 11 are
    reserved.
    These pins on the G5SoC should be configured as inputs to avoid issues.

  CMODE 3 to 7 are currently floating and connect to G5SoC as follows and should
  be driven to 0 for consistent operation:

    CMODE 3 - D18 - GPIO16PB1
    CMODE 4 - A18 - GPIO17PB1
    CMODE 5 - B18 - GPIO17NB1
    CMODE 6 - A12 - GPIO1NB1
    CMODE 7 - B12 - GPIO0PB1

  GPIO  0/Sigdet 0 is connected to A10 - GPIO177PB1
  GPIO  1/Sigdet 1 is connected to A11 - GPIO177PB1
  GPIO  2 is not connected
  GPIO  3 is not connected
  GPIO  4 is not connected
  GPIO  5/I2C_SCL1 is not connected
  GPIO  6 is not connected
  GPIO  7 is not connected
  GPIO  8 is not connected
  GPIO  9/FASTLINKFAIL is connected to C11 - GPIO178PB1
  GPIO 10 is not connected
  GPIO 11 is not connected
  GPIO 12 is not connected
  GPIO 13 is not connected
  GPIO 14 is not connected
  GPIO 15 is not connected

  RXD0P is connected to L5
  RXD0N is connected to L6
  TXD0P is connected to N6
  TXD0N is connected to N7
  RCVRD_CLK1 is connected to F17
  RXD1P is connected to K6
  RXD1N is connected to K7
  TXD1P is connected to M7
  TXD1N is connected to N8
  RCVRD_CLK2 is connected to E15

  MDINT0 is connected to E16
  MDINT1 is connected to B10
  MDC is connected to D3 - MSSIO28B2
  MDIO is connected to C2 - MSSIO29B2

  OSCEN is connected to E18 however there is a 0R resistor R344 marked as NL...
  CLKOUT is connected to A13 - GPIO1PB1/CLKIN_S_5
  PLLMODE is connected to D12

SVB has the following setup for the VSC8662:

  NRESET is connected to an MCP121T active low open drain power on reset
  chip. The signal is also connected to pin D12 on the FMC connector J1D.
  On the peripheral board FMC1 D12 connects to HPC1_LA05_N_B7 which is pin M7
  on the G5SoC - GPIO149NB7.
  On the peripheral board FMC2 D12 connects to HPC2_LA05_N_B1 which is pin
  E12 on the G5SoC - GPIO7NB1GPIO7NB1.

  NSRESET is connected to pin G31 on the FMC connector J1G.
  On the peripheral board FMC1 G31 connects to HPC1_LA29_N_B7 which is pin G5
  on the G5SoC - GPIO155NB7.
  On the peripheral board FMC2 G31 connects to HPC2_LA29_N_B1 which is pin
  A5 on the G5SoC - GPIO184NB1.

  CMODE 0 to 2 are connected to 0R resistors to ground or VCC. They also
  connect to the FMC connector but the use of 0R resistors means they cannot
  be changed via the FMC connector. The current default strapping is:

    CMODE 0 - 0 (J1G G13 - N8)
    CMODE 1 - 1 (J1H H7  - U2)
    CMODE 2 - 0 (J1H H8  - U1)

    This selects a PHY address range of 8 to 11 for the device i.e. port 0
    is at address 8, port 1 is at address 9 and addresses 10 and 11 are
    reserved.
    These pins on the G5SoC should be configured as inputs to avoid issues.

  CMODE 3 to 7 are currently floating and connect to the FMC connector as
  follows and should be driven to 0 for consistent operation:

    CMODE 3 - J1C C26 (FMC1 HPC1_LA27_P_B7 - T4, FMC2 HPC2_LA27_P_B1 - C7)
    CMODE 4 - J1C C27 (FMC1 HPC1_LA27_N_B7 - U4, FMC2 HPC2_LA27_N_B1 - B7)
    CMODE 5 - J1H H10 (FMC1 HPC1_LA04_P_B7 - P1, FMC2 HPC2_LA04_P_B1 - H12)
    CMODE 6 - J1H H11 (FMC1 HPC1_LA04_N_B7 - R1, FMC2 HPC2_LA04_N_B1 - G12)
    CMODE 7 - J1D D11 (FMC1 HPC1_LA05_P_B7 - L7, FMC2 HPC2_LA05_P_B1 - F12)

  GPIO  0/Sigdet 0 is connected to pin E24 on FMC connector J1E     (FMC1 HPC1_HB05_P_B0      - AP24, FMC2 HPC2_HB05_P_B9    - F24)
  GPIO  1/Sigdet 1 is connected to pin E25 on FMC connector J1E     (FMC1 HPC1_HB05_N_B0      - AN24, FMC2 HPC2_HB05_N_B9    - G24)
  GPIO  2 is connected to pin J27 on FMC connector J1J              (FMC1 HPC1_HB07_P_B0      - AD21, FMC2 HPC2_HB07_P_B9    - E23)
  GPIO  3 is connected to pin J28 on FMC connector J1J              (FMC1 HPC1_HB07_N_B0      - AE21, FMC2 HPC2_HB07_N_B9    - D23)
  GPIO  4/I2C_SCL0 is connected to pin E21 on FMC connector J1E     (FMC1 HPC1_HB03_P_B0      - AJ21, FMC2 HPC2_HB03_P_B9    - D24)
  GPIO  5/I2C_SCL1 is connected to pin J30 on FMC connector J1J     (FMC1 HPC1_HB11_P_B0      - AG22, FMC2 HPC2_HB11_P_B9    - C23)
  GPIO  6 is connected to pin J31 on FMC connector J1J              (FMC1 HPC1_HB11_N_B0      - AG21, FMC2 HPC2_HB11_N_B9    - C24)
  GPIO  7 is connected to pin J33 on FMC connector J1J              (FMC1 HPC1_HB15_P_B0      - AK22, FMC2 HPC2_HB15_P_B9    - A28)
  GPIO  8/I2C_SDA is connected to pin E22 on FMC connector J1E      (FMC1 HPC1_HB03_N_B0      - AK21, FMC2 HPC2_HB03_N_B9    - D25)
  GPIO  9/FASTLINKFAIL is connected to pin J34 on FMC connector J1J (FMC1 HPC1_HB15_N_B0      - AK23, FMC2 HPC2_HB15_N_B9    - B28)
  GPIO 10 is connected to pin J36 on FMC connector J1J              (FMC1 HPC1_HB18_P_B0      - AH19, FMC2 HPC2_HB18_P_B9    - D26)
  GPIO 11 is connected to pin J37 on FMC connector J1J              (FMC1 HPC1_HB18_N_B0      - AJ19, FMC2 HPC2_HB18_N_B9    - C27)
  GPIO 12 is connected to pin K34 on FMC connector J1K              (FMC1 HPC1_HB14_P_B0      - AH22, FMC2 HPC2_HB14_P_B9    - B26)
  GPIO 13 is connected to pin K35 on FMC connector J1K              (FMC1 HPC1_HB14_N_B0      - AH21, FMC2 HPC2_HB14_N_B9    - C26)
  GPIO 14 is connected to pin K31 on FMC connector J1K              (FMC1 HPC1_HB10_P_B0      - AK20, FMC2 HPC2_HB10_P_B9    - A22)
  GPIO 15 is connected to pin K32 on FMC connector J1K              (FMC1 HPC1_HB10_N_B0      - AJ20, FMC2 HPC2_HB10_N_B9    - A23)

  RXD0P is connected to pin J21 on FMC connector J1J                (FMC1 HPC1_SGMII_RXP0     - W1,   FMC2 HPC2_HA22_P_B9    - D19)
  RXD0N is connected to pin J22 on FMC connector J1J                (FMC1 HPC1_SGMII_RXN0     - Y1,   FMC2 HPC2_HA22_N_B9    - C18)
  TXD0P is connected to pin F19 on FMC connector J1F                (FMC1 HPC1_SGMII_TXP0     - Y5,   FMC2 HPC2_HA19_P_B9    - A18)
  TXD0N is connected to pin F20 on FMC connector J1F                (FMC1 HPC1_SGMII_TXN0     - AA5,  FMC2 HPC2_HA19_N_B9    - A19)
  RCVRD_CLK1 is connected to pin G6 on FMC connector J1G            (FMC1 HPC1_LA00_CC_P_B7   - R2,   FMC2 HPC2_LA00_CC_P_B1 - L15)
  RXD1P is connected to pin K7 on FMC connector J1K                 (FMC1 HPC1_SGMII_RXP0     - Y2,   FMC2 HPC2_HA02_P_B9    - H18)
  RXD1N is connected to pin K8 on FMC connector J1K                 (FMC1 HPC1_SGMII_RXN0     - Y3,   FMC2 HPC2_HA02_N_B9    - J18)
  TXD1P is connected to pin E18 on FMC connector J1E                (FMC1 HPC1_SGMII_TXP0     - W4,   FMC2 HPC2_HA20_P_B9    - E20)
  TXD1N is connected to pin E19 on FMC connector J1E                (FMC1 HPC1_SGMII_TXN0     - W5,   FMC2 HPC2_HA20_N_B9    - D20)
  RCVRD_CLK2 is connected to pin H23 on FMC connector J1H           (FMC1 HPC1_LA19_K10_B7    - K10,  FMC2 HPC2_LA19_N_B1    - F5)

  MDINT0 is connected to pin H14 on FMC connector J1H               (FMC1 HPC1_LA07_N_B7      - N6,   FMC2 HPC2_LA07_N_B1    - G14)
  MDC1_B7 is connected to pin G12 on FMC connector J1G              (FMC1 HPC1_LA08_P_B7      - N7,   FMC2 HPC2_LA08_P_B1    - F10)
  MDC1_B2 is connected to pin K17 on FMC connector J1K              (FMC1 HPC1_HA17_U11_B2    - U11,  FMC2 HPC2_HA17_CC_P_B1 - E11)
  MDIO1_B2 is connected to pin J19 on FMC connector J1J             (FMC1 HPC1_HA18_U10_B2    - U10,  FMC2 HPC2_HA18_N_B9    - F19)
  MDIO1_B7 is connected to pin H13 on FMC connector J1H             (FMC1 HPC1_LA07_P_B7      - M6,   FMC2 HPC2_LA07_P_B1    - F13)
  MDINT1 is connected to pin G7 on FMC connector J1G                (FMC1 HPC1_LA00_CC_N_B7   - R2,   FMC2 HPC2_LA00_CC_N_B1 - L14)

  OSCEN is connected to pin C15 on FMC connector J1C                (FMC1 HPC1_LA10_N_B7      - M9,   FMC2 HPC2_LA10_N_B1    - D10)
  CLKOUT is connected to pin C18 on FMC connector J1C               (FMC1 HPC1_LA14_P_B7      - J5,   FMC2 HPC2_LA14_P_B1    - E3)
  PLLMODE is connected to pin D20 on FMC connector J1D              (FMC1 HPC1_LA17_CC_L12_B7 - L12,  FMC2 HPC2_LA17_CC_P_B1 - J15)

SVB has the following setup for the VSC8541:

  The hardware bootstrap signals are configured as follows:
  
    RX_CLK       - 0 - Select managed mode
    TX_CLK       - 1 - MII/GMII interface
    RX_ER        - 0 - Don't force 1000BT mode
    RXD0         - 0 -\
    RXD1         - 1  |
    RXD2         - 0  | PHY address = 10
    RXD3         - 1  |
    RX_DV/RX_CTL - 0 -/
    RXD4         - 1 - Don't care in managed mode
    RXD5         - 1 - Don't care in managed mode
    RXD6         - 1 \ 50 MHz CLKOUT
    RXD7         - 0 /
    COL          - 1 - Don't care in managed mode
    CRS          - 0 - Don't care in managed mode
    CLKOUT       - 1 - CLKOUT enabled
    
  The following connections are made:
  
    NRESET is connected to pin K29 on FMC connector J1K         (FMC1 HPC1_HB06_CC_P_B0 - AM25, FMC2 HPC2_HB06_CC_N_B1 - B11)

    COL is connected to pin H38 on FMC connector J1H            (FMC1 HPC1_LA32_N_B7    - N12,  FMC2 HPC2_LA32_N_B9    - E15)
    CRS is connected to pin F34 on FMC connector J1F            (FMC1 HPC1_HB16_P_B0    - AH24, FMC2 HPC2_HB16_P_B9    - A27)
    RX_CLK is connected to pin F5 on FMC connector J1F          (FMC1 HPC1_HA00_CC_N_B7 - M10,  FMC2 HPC2_HA00_CC_P_B9 - J27)
    RX_ER is connected to pin D24 on FMC connector J1D          (FMC1 HPC1_LA23_P_B7    - T3,   FMC2 HPC2_LA23_N_B1    - D5)
    RX_DV is connected to pin F26 on FMC connector J1F          (FMC1 HPC1_HB04_N_B0    - AN27, FMC2 HPC2_HB04_N_B9    - E22)
    RXD0 is connected to pin F10 on FMC connector J1F           (FMC1 HPC1_HA08_P_B7    - F3,   FMC2 HPC2_HA08_P_B9    - A12)
    RXD1 is connected to pin F11 on FMC connector J1F           (FMC1 HPC1_HA08_N_B7    - G2,   FMC2 HPC2_HA08_N_B9    - A13)
    RXD2 is connected to pin C10 on FMC connector J1C           (FMC1 HPC1_LA06_P_B7    - F2,   FMC2 HPC2_LA06_P_B1    - G11)
    RXD3 is connected to pin C11 on FMC connector J1C           (FMC1 HPC1_LA06_N_B7    - E1,   FMC2 HPC2_LA06_N_B1    - G10)
    RXD4 is connected to pin G27 on FMC connector J1G           (FMC1 HPC1_LA25_P_B7    - K7,   FMC2 HPC2_LA25_P_B1    - F7)
    RXD5 is connected to pin G28 on FMC connector J1G           (FMC1 HPC1_LA25_N_B7    - J8,   FMC2 HPC2_LA25_N_B1    - E7)
    RXD6 is connected to pin H28 on FMC connector J1H           (FMC1 HPC1_LA24_P_B7    - J1,   FMC2 HPC2_LA24_P_B1    - D6)
    RXD7 is connected to pin H29 on FMC connector J1H           (FMC1 HPC1_LA24_N_B7    - K1,   FMC2 HPC2_LA24_N_B1    - C6)

    TX_CLK is connected to pin F4 on FMC connector J1F          (FMC1 HPC1_HA00_CC_P_B7 - L9,   FMC2 HPC2_HA00_CC_P_B9 - J27)
    MII_TX_CLK is connected to pin G30 on FMC connector J1G     (FMC1 HPC1_LA29_P_B7    - H6,   FMC2 HPC2_LA29_P_B1    - B6)
    TX_ER is connected to pin D14 on FMC connector J1D          (FMC1 HPC1_LA09_P_B7    - L5,   FMC2 HPC2_LA09_P_B1    - B9)
    TX_EN is connected to pin F25 on FMC connector J1F          (FMC1 HPC1_HB04_P_B0    - AN26, FMC2 HPC2_HB04_P_B9    - F22)
    TXD0 is connected to pin G33 on FMC connector J1G           (FMC1 HPC1_LA31_P_B7    - J3,   FMC2 HPC2_LA31_P_B9    - H16)
    TXD1 is connected to pin G34 on FMC connector J1G           (FMC1 HPC1_LA31_N_B7    - K2,   FMC2 HPC2_LA31_N_B9    - G16)
    TXD2 is connected to pin D8 on FMC connector J1D            (FMC1 HPC1_LA01_CC_P_B7 - M14,  FMC2 HPC2_LA01_CC_P_B1 - J14)
    TXD3 is connected to pin D9 on FMC connector J1D            (FMC1 HPC1_LA01_CC_N_B7 - N14,  FMC2 HPC2_LA01_CC_N_B1 - H14)
    TXD4 is connected to pin D26 on FMC connector J1D           (FMC1 HPC1_LA26_P_B7    - V2,   FMC2 HPC2_LA26_P_B1    - D8)
    TXD5 is connected to pin D27 on FMC connector J1D           (FMC1 HPC1_LA26_N_B7    - V1,   FMC2 HPC2_LA26_N_B1    - C8)
    TXD6 is connected to pin G36 on FMC connector J1G           (FMC1 HPC1_LA33_P_B7    - M11,  FMC2 HPC2_LA33_P_B9    - G15)
    TXD7 is connected to pin G37 on FMC connector J1G           (FMC1 HPC1_LA33_N_B7    - N11,  FMC2 HPC2_LA33_N_B9    - F15)

    MDINT is connected to pin D18 on FMC connector J1D          (FMC1 HPC1_LA13_N_B7    - K3,   FMC2 HPC2_LA13_N_B1    - C3)
    MDIO_B7 is connected to pin G19 on FMC connector J1G        (FMC1 HPC1_LA16_L10_B7  - L10,  FMC2 HPC2_LA16_N_B1    - A4)
    MDIO_B2 is connected to pin K20 on FMC connector J1H        (FMC1 HPC1_HA21_U14_B2  - U14,  FMC2 HPC2_HA21_N_B9    - B19)
    MDC_B7 is connected to pin G15 on FMC connector J1G         (FMC1 HPC1_LA12_M12_B7  - M12,  FMC2 HPC2_LA12_P_B1    - C1)
    MDC_B2 is connected to pin J18 on FMC connector J1J         (FMC1 HPC1_HA18_U10_B2  - U10,  FMC2 HPC2_HA18_P_B9    - F20)
    
    FASTLINK_FAIL is connected to pin K28 on FMC connector J1K  (FMC1 HPC1_HB06_CC_P_B0 - AL25, FMC2 HPC2_HB06_CC_P_B1 - C11)
    RECRVD_CLK  is connected to pin D23 on FMC connector J1D    (FMC1 HPC1_LA23_P_B7    - T3,   FMC2 HPC2_LA23_P_B1    - E6)
    CLKOUT is connected to pin H37 on FMC connector J1H         (FMC1 HPC1_LA32_P_B7    - N13,  FMC2 HPC2_LA32_P_B9    - E16)
    CLK_SQUELCH_IN is connected to pin F29 on FMC connector J1F (FMC1 HPC1_HB08_N_B0    - AJ27, FMC2 HPC2_HB08_N_B9    - B22)

--------------------------------------------------------------------------------
Target hardware
--------------------------------------------------------------------------------
This example project can be used on PolarFire SoC FPGA family hardware 
platforms.
There are configurations that need to be set for this example project. The
configurations are categorized into hardware and software configurations.
The hardware configurations are located in ./src/boards/<target_board> folder.
The default software configurations are stored under 
.src/platform/platform_config_reference folder.
The include files in the "./src/boards/<target_board>/soc_config" folder define 
the hardware configurations such as clocks. You must make sure that the 
configurations in this example project match the actual configurations of your 
target Libero design that you are using to test this example project.
If you need to change the software configurations, you are advised to create a 
new folder to replicate this folder under the ./src/boards directory and do the 
modifications there. It would look like 
./src/boards/<target_board>/platform_config
The include files in the "platform_config" folder define the software 
configurations such as how many harts are being used in the software, what is 
the tick rate of the internal timer of each hart. These configurations have no 
dependency on the hardware configurations in "soc_config" folder. Note that 
changing these software configurations may require a change in your application 
code.
## Executing project on the PolarFire SoC hardware
This application can be used on PolarFire hardware platform as well e.g. Icicle 
Kit. In this case, the MMUART0 must be connected to a host PC. The host PC must 
connect to the serial port using a terminal emulator such as Tera Term or PuTTY 
or the SoftConsole built-in terminal view.
Build the project and launch the debug configuration named 
"mpfs-uart-mac-freertos_lwip hw all-harts debug" which is configured for 
PolarFire SoC hardware platform.
    