#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "mpfs_hal/mpfs_hal_version.h"
#include "menu_selector.h"
#include "drivers/off-chip/pac1934/pac1934.h"

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
"Save changes, build, and run project to measure the power.\r\n";

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
"Save changes, build, and run project to measure the power.\r\n";

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
"Save changes, build, and run project to measure the power.\r\n";

const uint8_t menu_power_saving[] =
"\r\n"
"This program is run from E51\r\n"
"\r\n"
"MPFS HAL Power Saving Options:\r\n"
"1  How to toggle ON/OFF Parked Hart RAM at bootup\r\n"
"2  How to toggle ON/OFF U54 Floating Point Units(FPU) at bootup\r\n"
"3  How to toggle ON/OFF RAM of Unused Peripherals at bootup\r\n"
"4  Display DDR menu\r\n"
"5  Display clock scaling menu\r\n"
"6  Display maximum power-saving menu\r\n"
"7  Toggle periodic low power mode\r\n"
"c  Display PAC1934 current monitor values\r\n";

const uint8_t display_menu_ddr[] =
"\r\n"
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
"c  Display PAC1934 current monitor values\r\n"
"m  Go back to main menu\r\n"
"WARNING: DDR is not accessible when in self-refresh mode, or PLL is disabled\r\n";

const uint8_t display_menu_clock_scaling[] =
"\r\n"
"Select a clock scaling option:\r\n"
"\r\n"
"Make sure that u54_1 hart is turned on before selecting an option:\r\n"
"1  Change CPU clock frequency to 300MHz (half)\r\n"
"2  Change CPU clock frequency to 600MHz (default)\r\n"
"3  Display clock status\r\n"
"c  Display PAC1934 current monitor values\r\n"
"m  Go back to main menu\r\n";

const uint8_t display_menu_max[] =
"\r\n"
"Select a max power-saving option:\r\n"
"\r\n"
"Make sure that u54_1 hart is turned on before selecting an option:\r\n"
"1  Toggle maximum power-saving mode with clock scaling\r\n"
"2  Toggle maximum power-saving mode without clock scaling\r\n"
"3  Reset to default settings\r\n"
"4  Display clock status\r\n"
"c  Display PAC1934 current monitor values\r\n"
"m  Go back to main menu\r\n";

const uint8_t msg_medium_frequency_enabled[] =
"\r\nCPU operating in half speed of default frequency:\r\n";

const uint8_t msg_normal_frequency_enabled[] =
"\r\nCPU operating in default frequency:\r\n";

const uint8_t msg_self_refresh_status_on[] =
"\r\nSelf refresh status: ON\r\n";

const uint8_t msg_self_refresh_status_off[] =
"\r\nSelf refresh status: OFF\r\n";

const uint8_t msg_ddr_pll_status_on[] =
"DDR PLL status: ON\r\n";

const uint8_t msg_ddr_pll_status_off[] =
"DDR PLL status: OFF\r\n";

const uint8_t msg_show_menu_again_prompt[] =
"\r\n"
"Type 0 to show the menu again\r\n"
"-------------------------------------------------------------------------\r\n";

const uint8_t msg_page_break[] =
"-------------------------------------------------------------------------\r\n";

const uint8_t msg_periodic_lp_mode_intro[] =
"Welcome to periodic low-power mode!\r\n"
"\r\n"
"The device will be in low power mode for a few seconds\r\n"
"before waking up in high power mode for a short period.\r\n"
"This cycle will repeat indefintely.\r\n"
"\r\n"
"To leave periodic low-power mode, hit any key\r\n"
"and wait until the next high-power interrupt\r\n"
"\r\n";

const uint8_t msg_periodic_mode_status_lp[] =
"Status: Low power mode...\r\n";

const uint8_t msg_periodic_mode_status_hp[] =
"Status: High power mode...\r\n";


void select_ddr_option(uint8_t config_option)
{
    uint8_t rx_buff[1];
    uint8_t get_uart_rx = 0;
    uint8_t leave_function = 0;

    if (config_option == MAX_POWER_SAVING)
    {
        /* Turn self-refresh on and turn DDR PLL off */
        asm("fence.i");
        flush_l2_cache((uint32_t)1U);
        mpfs_hal_turn_ddr_selfrefresh_on();
        mpfs_hal_ddr_logic_power_state(DDR_LOW_POWER, DDR_TOGGLE_OUTPUTS);

    }
    else if (config_option == RESET_TO_DEFAULT)
    {
        /* Turn self-refresh off and turn DDR PLL on */
        asm("fence.i");
        flush_l2_cache((uint32_t)1U);
        mpfs_hal_turn_ddr_selfrefresh_off();
        mpfs_hal_ddr_logic_power_state(DDR_NORMAL_POWER, DDR_TOGGLE_OUTPUTS);
    }
    else
    {
        while (1)
        {
            get_uart_rx = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff,
                                                (uint32_t)sizeof(rx_buff));

            if (get_uart_rx++)
            {
                switch(rx_buff[0])
                {
                    default:
                        MSS_UART_polled_tx_string(g_uart, msg_page_break);
                        MSS_UART_polled_tx_string(g_uart, display_menu_ddr);
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        break;
                    case '0':
                        MSS_UART_polled_tx_string(g_uart, msg_page_break);
                        MSS_UART_polled_tx_string(g_uart, display_menu_ddr);
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        break;
                    case '1':
                        /* 1  Clear pattern in memory */
                        ddr_sr_test = 1;
                        break;
                    case '2':
                        /* 2  Place pattern in memory */
                        ddr_sr_test = 2;
                        break;
                    case '3':
                        /* 3  Verify data in memory */
                        ddr_sr_test = 3;
                        break;
                    case '4':
                        /* 4  Turn on ddr self refresh */
                        ddr_sr_test = 4;
                        break;
                    case '5':
                        /* 5  Turn off ddr self refresh */
                        ddr_sr_test = 5;
                        break;
                    case '6':
                        /* 6  Check ddr self refresh status */
                        ddr_sr_test = 6;
                        break;
                    case '7':
                        /* 7  Turn off ddr pll */
                        ddr_sr_test = 7;
                        break;
                    case '8':
                        /* 8  Turn on ddr pll */
                        ddr_sr_test = 8;
                        break;
                    case 'c':
                        /* c  c  Display PAC1934 current monitor values */
                        monitor_current_flag = 1;
                        break;
                    case 'm':
                        /* m  Print main menu for power saving and leave function */
                        MSS_UART_polled_tx_string(g_uart, msg_page_break);
                        MSS_UART_polled_tx_string(g_uart, menu_power_saving);
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        leave_function = 1;
                        break;
                } /* End of switch statement */
            } /* End of receive buffer check */

            /* Leave this fuction if flag is raised */
            if (leave_function == 1)
            {
                break; /* Break from while(1) loop */
            }

         } /* End of while loop */
    }
}

void select_clock_scaling_option(uint8_t config_option)
{
    uint8_t rx_buff[1];
    uint8_t get_uart_rx = 0;
    uint8_t leave_function = 0;

    if (config_option == MAX_POWER_SAVING)
    {
        /* Change CPU clock frequency to 300MHz (half) */
        mss_freq_scaling(MSS_CLK_SCALING_MEDIUM);
    }
    else if(config_option == DEFAULT_CLOCK_SCALE)
    {
        /* Change CPU clock frequency to 600MHz */
        mss_freq_scaling(MSS_CLK_SCALING_NORMAL);

    }
    else
    {
        while (1)
        {
            get_uart_rx = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff,
                                                (uint32_t)sizeof(rx_buff));

            if (get_uart_rx++)
            {
                switch (rx_buff[0])
                {
                    default:
                        MSS_UART_polled_tx_string(g_uart, msg_page_break);
                        MSS_UART_polled_tx_string(g_uart, display_menu_clock_scaling);
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        break;
                    case '0':
                        MSS_UART_polled_tx_string(g_uart, msg_page_break);
                        MSS_UART_polled_tx_string(g_uart, display_menu_clock_scaling);
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        break;
                    case '1':
                        /* 1  Change CPU clock frequency to 300MHz (half) */
                        mss_freq_scaling(MSS_CLK_SCALING_MEDIUM);
                        MSS_UART_polled_tx_string(g_uart, msg_medium_frequency_enabled);
                        display_clocks();
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        break;
                    case '2':
                        /* 2  Change CPU clock frequency to 600MHz (default) */
                        mss_freq_scaling(MSS_CLK_SCALING_NORMAL);
                        MSS_UART_polled_tx_string(g_uart, msg_normal_frequency_enabled);
                        display_clocks();
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        break;
                    case '3':
                        /* 3  Display clock status */
                        display_clocks();
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        break;
                    case 'c':
                        /* c  Display PAC1934 current monitor values */
                        monitor_current_flag = 1;
                        break;
                    case 'm':
                        /* m  Print main menu for power saving and leave function */
                        MSS_UART_polled_tx_string(g_uart, msg_page_break);
                        MSS_UART_polled_tx_string(g_uart, menu_power_saving);
                        MSS_UART_polled_tx_string(g_uart,
                                                    msg_show_menu_again_prompt);
                        leave_function = 1;
                        break;
                } /* End of switch statement */
            } /* End of receive buffer check */

            /* Leave this fuction if flag is raised */
            if (leave_function == 1)
            {
                break; /* Break from while(1) loop */
            }

        } /* End of while loop */
    }
}

void select_max_option(uint8_t config_option)
{
    uint8_t rx_buff[1];
    uint8_t get_uart_rx = 0;
    uint8_t leave_function = 0;

    while (1)
    {
        get_uart_rx = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff,
                                            (uint32_t)sizeof(rx_buff));

        if (get_uart_rx++)
        {
            switch (rx_buff[0])
            {
                default:
                    MSS_UART_polled_tx_string(g_uart, msg_page_break);
                    MSS_UART_polled_tx_string(g_uart, display_menu_max);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_show_menu_again_prompt);
                    break;
                case '0':
                    MSS_UART_polled_tx_string(g_uart, msg_page_break);
                    MSS_UART_polled_tx_string(g_uart, display_menu_max);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_show_menu_again_prompt);
                    break;
                case '1':
                    /* 1  Toggle maximum power-saving mode with clock scaling */
                    select_ddr_option(MAX_POWER_SAVING);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_self_refresh_status_on);
                    MSS_UART_polled_tx_string(g_uart, msg_ddr_pll_status_off);
                    select_clock_scaling_option(MAX_POWER_SAVING);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_medium_frequency_enabled);
                    display_clocks();
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_show_menu_again_prompt);
                    break;
                case '2':
                    /* 2  Toggle maximum power-saving mode without clock scaling */
                    select_ddr_option(MAX_POWER_SAVING);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_self_refresh_status_on);
                    MSS_UART_polled_tx_string(g_uart, msg_ddr_pll_status_off);
                    select_clock_scaling_option(DEFAULT_CLOCK_SCALE);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_normal_frequency_enabled);
                    display_clocks();
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_show_menu_again_prompt);
                    break;
                case '3':
                    /* 3  Reset to default settings */
                    select_ddr_option(RESET_TO_DEFAULT);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_self_refresh_status_off);
                    MSS_UART_polled_tx_string(g_uart, msg_ddr_pll_status_on);
                    select_clock_scaling_option(DEFAULT_CLOCK_SCALE);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_normal_frequency_enabled);
                    display_clocks();
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_show_menu_again_prompt);
                    break;
                case '4':
                    /* 4  Display clock status */
                    display_clocks();
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_show_menu_again_prompt);
                    break;
                case 'c':
                    /* c  Display PAC1934 current monitor values */
                    monitor_current_flag = 1;
                    break;
                case 'm':
                    /* m  Print main menu for power saving and leave function */
                    MSS_UART_polled_tx_string(g_uart, msg_page_break);
                    MSS_UART_polled_tx_string(g_uart, menu_power_saving);
                    MSS_UART_polled_tx_string(g_uart,
                                                msg_show_menu_again_prompt);
                    leave_function = 1;
                    break;
            } /* End of switch statement */
        } /* End of receive buffer check */

        /* Leave this fuction if flag is raised */
        if (leave_function == 1)
        {
            break; /* Break from while(1) loop */
        }
    } /* End of while loop */
}

void display_clocks(void)
{
    char info_string_clks[200];
    uint32_t count0;
    cfmChannelMode chMode;

    memset(&chMode, 0, sizeof(chMode));

    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)
                              "\n\rSystem Clock Frequencies:\n\r");
    /* GROUP 1, default reference clock frequency is 125 Mhz */
    MSS_CLF_clk_configuration(0x1, 1, 0, 0x0, 0x1);
    CFM_REG->clkselReg &= ~(0x1U << CFM_CLK_MONSEL_SHIFT);
    MSS_CFM_runtime_register(0x2710);
    chMode.channel0 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;

    sprintf(info_string_clks, "CPU clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    MSS_CFM_runtime_register(10000U);
    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string_clks, "AXI clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks, "AHB clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks, "Reference clock   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string_clks, "DFI clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    /* GROUP 2, default reference clock frequency is 125 Mhz */
    MSS_CLF_clk_configuration(0x2, 1, 0, 0x0, 0x1);
    chMode.channel0 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;
    sprintf(info_string_clks, "MAC TX clock      = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string_clks, "MAC TSU clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks, "MAC0 RX clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks, "MAC1 RX clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string_clks, "SGMII CLK C OUT   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    /* GROUP 4, default reference clock frequency is 125 Mhz */
    MSS_CLF_clk_configuration(0x4, 1, 0, 0x0, 0x1);
    chMode.channel0 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;
    sprintf(info_string_clks, "SGMII PLL clock 0 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string_clks, "SGMII PLL clock 1 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks, "SGMII PLL clock 2 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks, "SGMII PLL clock 3 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string_clks, "SGMII DLL clock   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    /* GROUP 5, default reference clock frequency is 125 Mhz */
    MSS_CLF_clk_configuration(0x5, 1, 0, 0x0, 0x1);
    chMode.channel2 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks, "FAB MAC0 TSU clk  = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while (ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks, "FAB MAC1 TSU clk  = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_clks);
}

void periodic_lp_mode(void)
{
    uint8_t rx_buff[1];
    uint8_t get_uart_rx = 0;
    uint8_t leave_function = 0;
    MSS_UART_polled_tx_string(g_uart, msg_periodic_lp_mode_intro);

    while (1)
    {

        get_uart_rx = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff,
                                            (uint32_t)sizeof(rx_buff));
        if(get_uart_rx++)
        {
            MSS_UART_polled_tx_string(g_uart, menu_power_saving);
            MSS_UART_polled_tx_string(g_uart,
                                        msg_show_menu_again_prompt);
            break;
        }
        else
        {
            /* low power mode */
            select_ddr_option(MAX_POWER_SAVING);
            select_clock_scaling_option(MAX_POWER_SAVING);
            MSS_TIM1_init(TIMER_LO, MSS_TIMER_ONE_SHOT_MODE);
            MSS_TIM1_load_immediate(TIMER_LO, LOAD_VALUE_1500M);
            MSS_TIM1_start(TIMER_LO);
            MSS_UART_polled_tx_string(g_uart, msg_periodic_mode_status_lp);
            while (*TIMER_1_READ_VALUE > 0)
            {
                ;
            }
            MSS_TIM1_stop(TIMER_LO);
            
            /* high power mode */
            select_ddr_option(RESET_TO_DEFAULT);
            select_clock_scaling_option(DEFAULT_CLOCK_SCALE);
            MSS_TIM1_load_immediate(TIMER_LO, LOAD_VALUE_20M);
            MSS_TIM1_start(TIMER_LO);
            MSS_UART_polled_tx_string(g_uart, msg_periodic_mode_status_hp);
            while (*TIMER_1_READ_VALUE > 0)
            {
                ;
            }
            MSS_TIM1_stop(TIMER_LO);
        }
    }
}
