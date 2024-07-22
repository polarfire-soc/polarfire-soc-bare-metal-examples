const uint8_t msg_toggle_park_hart_ram[] =
"\r\n"
"How to toggle ON/OFF Parked Hart RAM(s) at bootup:\r\n"
"\r\n"
"Open any or all of the application source code files of this project:\r\n"
"application/hart1/u51_1.c\r\n"
"application/hart2/u51_2.c\r\n"
"application/hart3/u51_3.c\r\n"
"application/hart4/u51_4.c\r\n"
"\r\n"
"To turn ON RAM to Parked Hart at bootup:\r\n"
"COMMENT the line: #ifdef TURN_OFF_POWER_TO_PARKED_HARTS\r\n"
"\r\n"
"To turn OFF RAM to Parked Hart at bootup:\r\n"
"UNCOMMENT the line: #ifdef TURN_OFF_POWER_TO_PARKED_HARTS\r\n"
"\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_toggle_fpu[] =
"\r\n"
"How to toggle ON/OFF U54 Floating Point Units(FPU) at bootup:\r\n"
"\r\n"
"Open boards/{BOARD}/platform_config/mpfs_hal_config/mss_sw_config.h:\r\n"
"\r\n"
"To turn ON U54 Floating Point Units (FPU) at bootup:\r\n"
"UNCOMMENT the line: #define TURN_ALL_FPU_ON\r\n"
"COMMENT the line: #define TURN_ALL_FPU_OFF\r\n"
"\r\n"
"To turn OFF U54 Floating Point Units (FPU) at bootup:\r\n"
"COMMENT the line: #define TURN_ALL_FPU_ON\r\n"
"UNCOMMENT the line: #define TURN_ALL_FPU_OFF\r\n"
"\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_toggle_unused_perif_ram[] =
"\r\n"
"How to toggle ON/OFF RAM of Unused Peripherals at bootup:\r\n"
"\r\n"
"Open application/hart0/e51.c and go to the bottom of the file\r\n"
"\r\n"
"To turn ON RAM of Unused Peripherals at bootup:\r\n"
"UNCOMMENT the line: #define MEASURED_UNUSED_PERIPHERAL_RAM\r\n"
"\r\n"
"To turn OFF RAM of Unused Peripherals at bootup:\r\n"
"COMMENT the line: #define MEASURED_UNUSED_PERIPHERAL_RAM\r\n"
"\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t menu_power_saving[] =
"\r\n\r\n\r\n"
"This program is run from E51\r\n"
"\r\n"
"MPFS HAL Power Saving Options:\r\n"
"1  How to toggle ON/OFF Parked Hart RAM at bootup\r\n"
"2  How to toggle ON/OFF U54 Floating Point Units(FPU) at bootup\r\n"
"3  How to toggle ON/OFF RAM of Unused Peripherals at bootup\r\n"
"4  Display DDR menu\r\n"
"5  Display clock scaling menu\r\n"
"6  Display maximum power-saving menu\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t display_menu_ddr[] =
"\r\n\r\n\r\n"
"Select a DDR option:\r\n"
"\r\n"
"Make sure that u54_1 hart is turned on before selecting an option:\r\n"
"1  Clear pattern in memory block (<1 minute)\r\n"
"2  Place pattern in memory block (<1 minute)\r\n"
"3  Verify if pattern is in memory (<2 minutes)\r\n"
"4  Turn on ddr self refresh\r\n"
"5  Turn off ddr self refresh\r\n"
"6  Check ddr self refresh status\r\n"
"7  Turn off ddr pll\r\n"
"8  Turn on ddr pll\r\n"
"9  Go back to main menu\r\n"
"WARNING: DDR is not accessible when in self-refresh mode, or PLL is disabled\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t display_menu_clock_scaling[] =
"\r\n\r\n\r\n"
"Select a clock scaling option:\r\n"
"\r\n"
"Make sure that u54_1 hart is turned on before selecting an option:\r\n"
"1  Change CPU clock frequency to 300MHz (half)\r\n"
"2  Change CPU clock frequency to 600MHz (default)\r\n"
"3  Display clock status\r\n"
"7  Go back to main menu\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t display_menu_max[] =
"\r\n\r\n\r\n"
"Select a max power-saving option:\r\n"
"\r\n"
"Make sure that u54_1 hart is turned on before selecting an option:\r\n"
"1  Toggle maximum power-saving mode with clock scaling\r\n"
"2  Toggle maximum power-saving mode without clock scaling\r\n"
"3  Display clock status\r\n"
"7  Go back to main menu\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_medium_frequency_enabled[] =
"\r\nCPU operating in half speed of default frequency:\r\n";

const uint8_t msg_normal_frequency_enabled[] =
"\r\nCPU operating in default frequency:\r\n";

const uint8_t msg_show_menu_again_prompt[] =
"\r\nType 0 to show the menu again\r\n";
