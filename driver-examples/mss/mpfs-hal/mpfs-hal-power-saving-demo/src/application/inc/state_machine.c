/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software
 *
 */

#include <string.h>
#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "mpfs_hal/mpfs_hal_version.h"

#include "state_machine.h"
#include "menu_selector.h"

/*
 * External data
 */
extern uint32_t user_sm_request_h0;
extern uint32_t user_sm_request_h1;
extern uint32_t state_machine_status_request_h0;
extern uint32_t state_machine_status_request_h1;

/*
 * Local function declarations
 */

const char* get_state_name_string_h0(FS_SM_0 state);
const char* get_state_name_string_h1(FS_SM_1 state);

uint32_t request_from_h0 = 0;
uint32_t ack_from_h1 = 0;
uint32_t make_the_change = 0;

/*
 * Public Functions - API
 */
uint32_t state_machine_h0(void)
{
    uint32_t ret_status = 0U;
    static FS_DATA_0 fs_h0 = {.sm = FS_SM_0_INIT};
    static uint32_t previous_user_request = 0U;
    char info_string[200];
    
    switch (fs_h0.sm)
    {
        default:
        case FS_SM_0_INIT:
            fs_h0.current_state = FS_SM_0_INIT;
            fs_h0.previous_state = FS_SM_0_INIT;
            fs_h0.request_state = FS_SM_0_DEFAULT_STATE;
            fs_h0.sm = FS_SM_0_DEFAULT_STATE;
            break;

        case FS_SM_0_DEFAULT_STATE:
            fs_h0.current_state = FS_SM_0_DEFAULT_STATE;
            if (user_sm_request_h0 == FS_SM_0_START_APP)
            {
                fs_h0.sm = FS_SM_0_START_APP;
                fs_h0.previous_state = FS_SM_0_DEFAULT_STATE;
                user_sm_request_h0 = 0;
            }
            break;

        case FS_SM_0_START_APP:
            fs_h0.current_state = FS_SM_0_START_APP;
            request_from_h0 = 1;
            MSS_UART_polled_tx_string(&g_mss_uart0_lo, msg_req_tx_to_u54_1);
            if (ack_from_h1 == 1)
            {
                ack_from_h1 = 0;
                MSS_UART_polled_tx_string(&g_mss_uart0_lo, msg_ack_rx_from_u54_1);
                MSS_UART_polled_tx_string(&g_mss_uart0_lo, msg_show_menu_again_prompt);
                fs_h0.previous_state = FS_SM_0_START_APP;
                fs_h0.request_state = FS_SM_0_DEFAULT_STATE;
                fs_h0.sm = FS_SM_0_DEFAULT_STATE;

                // NOW SET FLAG TO START u51 APP
            }
            break;
    } /* end of case statement */

    if (state_machine_status_request_h0 == 1)
    {
        sprintf(info_string, msg_state_machine_status,
                get_state_name_string_h0(fs_h0.current_state),
                get_state_name_string_h0(fs_h0.request_state),
                get_state_name_string_h0(fs_h0.previous_state));
        MSS_UART_polled_tx_string(&g_mss_uart0_lo, info_string);
        MSS_UART_polled_tx_string(&g_mss_uart0_lo, msg_show_menu_again_prompt);
        state_machine_status_request_h0 = 0;
    }

    return (ret_status);
}

uint32_t state_machine_h1(void)
{
    uint32_t ret_status = 0U;
    static FS_DATA_0 fs_h1 = {.sm = FS_SM_1_INIT};
    static uint32_t previous_user_request = 0U;
    char info_string[200];

    switch (fs_h1.sm)
    {
        default:
        case FS_SM_1_INIT:
            fs_h1.current_state = FS_SM_1_INIT;
            fs_h1.previous_state = FS_SM_1_INIT;
            fs_h1.request_state = FS_SM_1_WAIT_START;
            fs_h1.sm = FS_SM_1_WAIT_START;
            break;

        case FS_SM_1_WAIT_START:
            fs_h1.current_state = FS_SM_1_WAIT_START;
            if (request_from_h0 == 1)
            {
                MSS_UART_polled_tx_string(&g_mss_uart1_lo, msg_req_rx_from_e51);
                ack_from_h1 = 1;
                MSS_UART_polled_tx_string(&g_mss_uart1_lo, msg_ack_tx_to_e51);
                MSS_UART_polled_tx_string(&g_mss_uart1_lo, msg_show_menu_again_prompt);
                request_from_h0 = 0;
            }
            break;

        case FS_SM_1_LOW_FREQ:
            fs_h1.current_state = FS_SM_1_LOW_FREQ;
            if (user_sm_request_h1 != previous_user_request)
            {
                previous_user_request = user_sm_request_h1;
                fs_h1.previous_state = FS_SM_1_LOW_FREQ;
            }
            break;

        case FS_SM_1_START_APP:
            break;
    } /* end of case statement */

    if (state_machine_status_request_h1 == 1)
    {
        sprintf(info_string, msg_state_machine_status,
                get_state_name_string_h1(fs_h1.current_state),
                get_state_name_string_h1(fs_h1.request_state),
                get_state_name_string_h1(fs_h1.previous_state));
        MSS_UART_polled_tx_string(&g_mss_uart1_lo, info_string);
        MSS_UART_polled_tx_string(&g_mss_uart1_lo, msg_show_menu_again_prompt);
        state_machine_status_request_h1 = 0;
    }

    return (ret_status);
}

const char* get_state_name_string_h0(FS_SM_0 state)
{
    switch (state)
    {
        case FS_SM_0_INIT:
            return "FS_SM_0_INIT";
        case FS_SM_0_DEFAULT_STATE:
            return "FS_SM_0_DEFAULT_STATE";
        case FS_SM_0_START_APP:
            return "FS_SM_0_START_APP";
        case FS_SM_0_LOW_FREQ:
            return "FS_SM_0_LOW_FREQ";
        case FS_SM_0_NORMAL_FREQ:
            return "FS_SM_0_NORMAL_FREQ";
        default:
            return "UNKNOWN";
    }
}

const char* get_state_name_string_h1(FS_SM_1 state)
{
    switch (state)
    {
        case FS_SM_1_INIT:
            return "FS_SM_1_INIT";
        case FS_SM_1_WAIT_START:
            return "FS_SM_1_WAIT_START";
        case FS_SM_1_START_APP:
            return "FS_SM_1_START_APP";
        case FS_SM_1_LOW_FREQ:
            return "FS_SM_1_LOW_FREQ";
        case FS_SM_1_NORMAL_FREQ:
            return "FS_SM_1_NORMAL_FREQ";
        default:
            return "UNKNOWN";
    }
}
