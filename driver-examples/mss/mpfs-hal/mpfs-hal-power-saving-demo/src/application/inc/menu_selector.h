extern mss_uart_instance_t *g_uart;
uint32_t ddr_sr_test;
static void display_clocks(void);

static void select_ddr_self_refresh_option(void)
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
            switch(rx_buff[0])
            {
                default:
                    MSS_UART_polled_tx_string(g_uart, display_menu_self_refresh);
                    break;
                case '0':
                    MSS_UART_polled_tx_string(g_uart, display_menu_self_refresh);
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
                    /* 7  Print main menu for power saving and leave function */
                    MSS_UART_polled_tx_string(g_uart, menu_power_saving);
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

static void select_clock_scaling_option(void)
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
                    MSS_UART_polled_tx_string(g_uart, display_menu_clock_scaling);
                    break;
                case '0':
                    MSS_UART_polled_tx_string(g_uart, display_menu_clock_scaling);
                    break;
                case '1':
                    /* 1  Change CPU clock frequency to 300MHz (half) */
                    mss_freq_scaling(MSS_CLK_SCALING_MEDIUM);
                    MSS_UART_polled_tx_string(g_uart, msg_medium_frequency_enabled);
                    display_clocks();
                    MSS_UART_polled_tx_string(g_uart, msg_show_menu_again_prompt);
                    break;
                case '2':
                    /* 2  Change CPU clock frequency to 600MHz (default) */
                    mss_freq_scaling(MSS_CLK_SCALING_NORMAL);
                    MSS_UART_polled_tx_string(g_uart, msg_normal_frequency_enabled);
                    display_clocks();
                    MSS_UART_polled_tx_string(g_uart, msg_show_menu_again_prompt);
                    break;
                case '3':
                    /* 3  Display clock status */
                    display_clocks();
                    MSS_UART_polled_tx_string(g_uart, msg_show_menu_again_prompt);
                    break;
                case '7':
                    /* 7  Print main menu for power saving and leave function */
                    MSS_UART_polled_tx_string(g_uart, menu_power_saving);
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

static void display_clocks(void)
{
    char info_string_clks[200];
    uint32_t count0;
    cfmChannelMode chMode;

    memset(&chMode, 0, sizeof(chMode));

    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)
                              "\n\rSystem Clock Frequencies:\n\r");
    /* group 1, ref clk is clk in rerf here it is 125 Mhz */
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

    /* group 2, ref clkis clk in rerf here it is 125 Mhz */
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

    /* group 4, ref clkis clk in rerf here it is 125 Mhz */
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

    /* group 5, ref clkis clk in rerf here it is 125 Mhz */
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