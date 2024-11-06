#ifndef MENU_SELECTOR_
#define MENU_SELECTOR_

#include <stdio.h>
#include <string.h>
#include "drivers/mss/mss_timer/mss_timer.h"
#include "state_machine.h"

#define TIMER_1_READ_VALUE                  ((const volatile uint32_t*)0x20125000)
#define TIMER_1_LOAD_VALUE                  ((volatile uint32_t*)0x20125004)
#define TIMER_1_BG_LOAD_VALUE               ((volatile uint32_t*)0x20125008)
#define TIMER_1_CONTROL_REG                 ((volatile uint32_t*)0x2012500C)
#define TIMER_1_RAW_INTERRUPT_STATUS        ((volatile uint32_t*)0x20125010)
#define TIMER_1_MASKED_INTERRUPT_STATUS     ((const volatile uint32_t*)0x20125014)

#define LOAD_VALUE_1500M                    ((uint32_t)1500000000U)
#define LOAD_VALUE_20M                      ((uint32_t)20000000U)

#define FINAL_COUNT_VALUE                   ((uint32_t)0U)
#define TIMER_DIFFERENCE_THRESHOLD          ((uint32_t)1000U)
#define EXECUTION_AVG_TIME                  ((uint32_t)40U)

#define EXPECTED_CPU_CLOCK                  LIBERO_SETTING_MSS_COREPLEX_CPU_CLK
#define EXPECTED_RTC_CLOCK                  LIBERO_SETTING_MSS_RTC_TOGGLE_CLK
#define EXPECTED_APB_CLOCK                  LIBERO_SETTING_MSS_APB_AHB_CLK

#define CUSTOM_CONFIG                   0
#define MAX_POWER_SAVING                1
#define RESET_TO_DEFAULT                2
#define DEFAULT_CLOCK_SCALE             2

#define DDR_ALL_OPTIONS_CHOSEN          0x7FU
#define DDR_NO_OPTIONS_CHOSEN           0x0U
#define DDR_TOGGLE_OUTPUTS              0x1U

extern const uint8_t menu_power_saving[];
extern const uint8_t display_menu_ddr[];
extern const uint8_t display_menu_clock_scaling[];
extern const uint8_t display_menu_max[];
extern const uint8_t display_menu_state_machine[];
extern const uint8_t display_menu_bootup_options[];

extern const uint8_t msg_toggle_park_hart_ram[];
extern const uint8_t msg_toggle_fpu[];
extern const uint8_t msg_toggle_unused_perif_ram[];
extern const uint8_t msg_medium_frequency_enabled[];
extern const uint8_t msg_normal_frequency_enabled[];
extern const uint8_t msg_show_menu_again_prompt[];
extern const uint8_t msg_state_machine_status[];
extern const uint8_t msg_self_refresh_status_on[];
extern const uint8_t msg_ddr_pll_output_status_off[];

extern const uint8_t msg_req_tx_to_u54_1[];
extern const uint8_t msg_req_rx_from_e51[];
extern const uint8_t msg_ack_tx_to_e51[];
extern const uint8_t msg_ack_rx_from_u54_1[];
extern const uint8_t msg_page_break[];
extern const uint8_t msg_start_u54_1[];

void select_bootup_option(uint8_t config_option);
void select_ddr_option(uint8_t config_option);
void select_clock_scaling_option(uint8_t config_option);
void select_max_option(uint8_t config_option);
void select_state_machine_option(uint8_t config_option);
void periodic_lp_mode(void);
void display_clocks(void);

extern mss_uart_instance_t *g_uart;
volatile uint32_t ddr_sr_test;
volatile uint32_t monitor_current_flag;
extern uint32_t user_sm_request_h0;
extern uint32_t state_machine_status_request_h0;
extern uint32_t state_machine_status_request_h1;

#endif

