const uint8_t msg_turn_on_park_hart_ram[] =
"\r\n"
"1  How to turn on Parked Hart RAM at bootup:\r\n"
"Go to the application source code: [u51_1, u51_2, u51_3, u51_4] \r\n"
"and COMMENT OUT the line: #ifdef TURN_OFF_POWER_TO_PARKED_HARTS\r\n"
"to turn on RAM to Parked Hart at bootup.\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_turn_off_park_hart_ram[] =
"\r\n"
"2  How to turn off Parked Hart RAM at bootup:\r\n"
"Go to the application source code: [u51_1, u51_2, u51_3, u51_4] \r\n"
"and UNCOMMENT the line: #ifdef TURN_OFF_POWER_TO_PARKED_HARTS\r\n"
"to turn off RAM to Parked Hart at bootup.\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_turn_on_fpu[] =
"\r\n"
"3  How to turn on U54 Floating Point Units(FPU) at bootup:\r\n"
"Go to the mss_sw_config.h in the boards/ folder of this project\r\n"
"and COMMENT OUT the line: #define TURN_ALL_FPU_OFF\r\n"
"to turn on u54 Floating Point Units (FPU).\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_turn_off_fpu[] =
"\r\n"
"4  How to turn off U54 Floating Point Units(FPU) at bootup:\r\n"
"Go to the mss_sw_config.h in the boards/ folder of this project\r\n"
"and COMMENT OUT the line: #define TURN_ALL_FPU_ON\r\n"
"to turn off u54 Floating Point Units (FPU).\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_turn_on_unused_perif_ram[] =
"\r\n"
"5  How to turn on RAM of Unused Peripherals at bootup:\r\n"
"Go to the bottom of the e51 application file of this project\r\n"
"and UNCOMMENT the line: #define MEASURED_UNUSED_PERIPHERAL_RAM\r\n"
"to turn on RAM Unused Peripherals RAM at bootup.\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t msg_turn_off_unused_perif_ram[] =
"\r\n"
"6  How to turn off RAM of Unused Peripherals at bootup:\r\n"
"Go to the bottom of the e51 application file of this project\r\n"
"and COMMENT OUT the line: #define MEASURED_UNUSED_PERIPHERAL_RAM\r\n"
"to turn off RAM Unused Peripherals RAM at bootup.\r\n"
"Save changes, build, and run project to measure the power.\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t menu_power_saving[] =
"\r\n\r\n\r\n"
"This program is run from E51\r\n"
"\r\n"
"MPFS HAL Power Saving Options:\r\n"
"1  How to turn on Parked Hart RAM at bootup\r\n"
"2  How to turn off Parked Hart RAM at bootup\r\n"
"3  How to turn on U54 Floating Point Units(FPU) at bootup\r\n"
"4  How to turn off U54 Floating Point Units(FPU) at bootup\r\n"
"5  How to turn on RAM of Unused Peripherals at bootup\r\n"
"6  How to turn off RAM of Unused Peripherals at bootup\r\n"
"7  Display DDR self refresh menu\r\n"
"8  Display clock scaling menu\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t display_menu_self_refresh[] =
"\r\n\r\n\r\n"
"Select the DDR self refresh test:\r\n"
"\r\n"
"Make sure that u54_1 hart is turned on before selecting an option:\r\n"
"1  Clear pattern in memory\r\n"
"2  Place pattern in memory\r\n"
"3  Verify if pattern is in memory\r\n"
"4  Turn on ddr self refresh\r\n"
"5  Turn off ddr self refresh\r\n"
"6  Check ddr self refresh status\r\n"
"7  Go back to main menu\r\n"
"WARNING: DDR is not accessible when in self-refresh mode\r\n"
"\r\n"
"Type 0 to show the menu again\r\n";

const uint8_t display_menu_clock_scaling[] =
"\r\n\r\n\r\n"
"Select a clock frequency option:\r\n"
"\r\n"
"Make sure that u54_1 hart is turned on before selecting an option:\r\n"
"1  Change CPU clock frequency to 300MHz (half)\r\n"
"2  Change CPU clock frequency to 600MHz (default)\r\n"
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