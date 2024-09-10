#ifndef MENU_SELECTOR_
#define MENU_SELECTOR_

#include <stdio.h>
#include <string.h>

#define CUSTOM_CONFIG                   0
#define MAX_POWER_SAVING                1
#define RESET_TO_DEFAULT                2
#define DEFAULT_CLOCK_SCALE             2

#define DDR_ALL_OPTIONS_CHOSEN          0x7FU
#define DDR_NO_OPTIONS_CHOSEN           0x0U
#define DDR_TOGGLE_OUTPUTS              0x1U

extern const uint8_t msg_toggle_park_hart_ram[];
extern const uint8_t msg_toggle_fpu[];
extern const uint8_t msg_toggle_unused_perif_ram[];
extern const uint8_t menu_power_saving[];
extern const uint8_t display_menu_ddr[];
extern const uint8_t display_menu_clock_scaling[];
extern const uint8_t display_menu_max[];
extern const uint8_t msg_medium_frequency_enabled[];
extern const uint8_t msg_normal_frequency_enabled[];
extern const uint8_t msg_show_menu_again_prompt[];

void select_ddr_option(uint8_t config_option);
void select_clock_scaling_option(uint8_t config_option);
void select_max_option(uint8_t config_option);
void display_clocks(void);

extern mss_uart_instance_t *g_uart;
volatile uint32_t ddr_sr_test;
volatile uint32_t monitor_current_flag;

#endif

