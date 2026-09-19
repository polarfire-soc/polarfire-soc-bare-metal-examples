/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file pac1934.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief pac1934 source file
 *
 */

#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_i2c/mss_i2c.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "pac1934.h"
#include "pac1934_regs.h"

extern mss_uart_instance_t *g_uart;
#define gp_usr_uart g_uart

#define PAC1934_NUM_CHANNELS                4u
#define PAC1934_REG_ADDR_INDEX              0u
#define PAC1934_REG_ADDR_BYTES              1u
#define PAC1934_VPOWER_ACC_BYTES            6u
#define PAC1934_WORD_BYTES                  2u
#define PAC1934_DWORD_BYTES                 4u
#define PAC1934_ID_BYTES                    1u
#define PAC1934_BYTE_SHIFT                  8u
#define PAC1934_REFRESH_DELAY_CYCLES        7250000u
#define PAC1934_ADC_FULL_SCALE_COUNTS       65535.0
#define PAC1934_VSENSE_FULL_SCALE_MA        10000.0
#define PAC1934_VBUS_FULL_SCALE_V           32.0
#define PAC1934_PLIC_PRIORITY               2u
#define PAC1934_SYSTICK_INTERVAL_MS         10u
#define PAC1934_SENSOR_PROBE_SUCCESS        0
#define PAC1934_SENSOR_PROBE_FAILURE        -1
#define PAC1934_CHANNEL_1                   0u
#define PAC1934_CHANNEL_2                   1u
#define PAC1934_CHANNEL_3                   2u
#define PAC1934_CHANNEL_4                   3u
#define PAC1934_PID_INDEX                   0u
#define PAC1934_MID_INDEX                   1u
#define PAC1934_REV_INDEX                   2u

static uint64_t g_vpower_acc[PAC1934_NUM_CHANNELS] = {0};
static uint64_t g_vbus[PAC1934_NUM_CHANNELS] = {0};
static uint64_t g_vsense[PAC1934_NUM_CHANNELS] = {0};

static void i2c1_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status);

static mss_i2c_status_t do_write_transaction
(
    uint8_t serial_addr,
    uint8_t * tx_buffer,
    uint8_t write_length
);

/*------------------------------------------------------------------------------
 * MSS I2C instance
 */
#define I2C_MASTER &g_mss_i2c1_lo

/*------------------------------------------------------------------------------
 * I2C master serial address.
 */
#define MASTER_SER_ADDR         0x21

/*-----------------------------------------------------------------------------
 * PAC1934 current sensor serial address.
 */
#define CURRENT_SENSOR_ADDR     0x10

#define BUFFER_SIZE    32u
static uint8_t g_master_rx_buf[BUFFER_SIZE];
static uint8_t g_master_tx_buf[BUFFER_SIZE];
static uint8_t g_slave_tx_buffer[BUFFER_SIZE];
static uint8_t g_pac1934_sensor_available = 0U;
static uint8_t g_pac1934_error_reported = 0U;

/*-----------------------------------------------------------------------------
 * I2C operation timeout value in mS. Define as MSS_I2C_NO_TIMEOUT to disable
 * the timeout functionality.
 */
#define DEMO_I2C_TIMEOUT    3000u
#define PAC1934_INVALID_READING 0xFFFFFFFFUL

/*-----------------------------------------------------------------------------
 * UI Buffer
 */
#define UI_BUF_SIZE             100u
#define LINE_SYMBOL             "+"
static uint8_t g_ui_buf[UI_BUF_SIZE] = {0};

static void pac1934_enable_access_path(void)
{
    (void)mss_config_clk_rst(MSS_PERIPH_FIC3,
                             (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
    mss_enable_fabric();
}

static uint8_t pac1934_reg_in_range(uint32_t reg, uint32_t first_reg, uint32_t last_reg)
{
    return ((reg >= first_reg) && (reg <= last_reg));
}

static uint8_t pac1934_reg_is_refresh(uint32_t reg)
{
    return ((REFRESH_REG == reg) || (REFRESH_G_REG == reg) || (REFRESH_V_REG == reg));
}

static uint64_t pac1934_read_big_endian_value(uint8_t const * rx_buffer, uint8_t byte_count)
{
    uint64_t value = 0u;

    for (uint8_t byte_idx = 0u; byte_idx < byte_count; byte_idx++)
    {
        value = value << PAC1934_BYTE_SHIFT;
        value = value + rx_buffer[byte_idx];
    }

    return value;
}

static double pac1934_vsense_to_ma(uint64_t raw_value)
{
    return (raw_value * PAC1934_VSENSE_FULL_SCALE_MA) / PAC1934_ADC_FULL_SCALE_COUNTS;
}

static double pac1934_vbus_to_v(uint64_t raw_value)
{
    return (raw_value * PAC1934_VBUS_FULL_SCALE_V) / PAC1934_ADC_FULL_SCALE_COUNTS;
}

/*------------------------------------------------------------------------------
 * Perform write-read transaction with parameters gathered from the command
 * line interface.
 */
static mss_i2c_status_t do_write_read_transaction
(
    uint8_t serial_addr,
    uint8_t * tx_buffer,
    uint8_t write_length,
    uint8_t * rx_buffer,
    uint8_t read_length
)
{
    mss_i2c_status_t status;

    MSS_I2C_write_read(I2C_MASTER,
                       serial_addr,
                       tx_buffer,
                       write_length,
                       rx_buffer,
                       read_length,
                       MSS_I2C_RELEASE_BUS);

    status = MSS_I2C_wait_complete(I2C_MASTER, DEMO_I2C_TIMEOUT);

    return status;
}

/* Check PAC1934 sensor register value */
static uint64_t pac1934_read_register(uint32_t reg)
{
    uint64_t value = 0;
    mss_i2c_status_t instance = MSS_I2C_FAILED;
    uint8_t num_receive_bytes = 0;

    g_master_tx_buf[PAC1934_REG_ADDR_INDEX] = reg;

    if (pac1934_reg_is_refresh(reg))   /* refresh */
    {
        instance = do_write_transaction(CURRENT_SENSOR_ADDR,
                                        g_master_tx_buf,
                                        PAC1934_REG_ADDR_BYTES);
        for (uint32_t cnt = 0u; cnt < PAC1934_REFRESH_DELAY_CYCLES; cnt++);  /* min 2ms TODO: improve this*/
    }
    else if (pac1934_reg_in_range(reg, VPOWER1_ACC_REG, VPOWER4_ACC_REG))          /* Vpower_acc */
    {
        num_receive_bytes = PAC1934_VPOWER_ACC_BYTES;
        instance = do_write_read_transaction(CURRENT_SENSOR_ADDR,
                                             g_master_tx_buf,
                                             PAC1934_REG_ADDR_BYTES,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else if (pac1934_reg_in_range(reg, VBUS1_REG, VSENSE4_AVG_REG)) /* vbus, Vsense, Vsense_avg*/
    {
        num_receive_bytes = PAC1934_WORD_BYTES;
        instance = do_write_read_transaction(CURRENT_SENSOR_ADDR,
                                             g_master_tx_buf,
                                             PAC1934_REG_ADDR_BYTES,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else if (pac1934_reg_in_range(reg, VPOWER1_REG, VPOWER4_REG)) /* Vpower */
    {
        num_receive_bytes = PAC1934_DWORD_BYTES;
        instance = do_write_read_transaction(CURRENT_SENSOR_ADDR,
                                             g_master_tx_buf,
                                             PAC1934_REG_ADDR_BYTES,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else if (pac1934_reg_in_range(reg, PID_REG, REV_REG))    /* IDs */
    {
        num_receive_bytes = PAC1934_ID_BYTES;
        instance = do_write_read_transaction(CURRENT_SENSOR_ADDR,
                                             g_master_tx_buf,
                                             PAC1934_REG_ADDR_BYTES,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else
    {
        ASSERT(0U);
    }

    if (MSS_I2C_SUCCESS == instance)
    {
        if (0u < num_receive_bytes)
        {
            value = pac1934_read_big_endian_value(g_master_rx_buf, num_receive_bytes);
        }

        return value;
    }
    else
    {
        return PAC1934_INVALID_READING;
    }
}

static uint8_t pac1934_read_failed(uint64_t value)
{
    return (PAC1934_INVALID_READING == value);
}

static void pac1934_print_read_failed(void)
{
    if (0U == g_pac1934_error_reported)
    {
        MSS_UART_polled_tx_string(gp_usr_uart,
                                  (const uint8_t *)"\r\nPAC1934 I2C read failed\r\n");
        g_pac1934_error_reported = 1U;
    }
}

void pac1934_draw_isense(void)
{
    pac1934_enable_access_path();

    if (0U == g_pac1934_sensor_available)
    {
        pac1934_print_read_failed();
        return;
    }

    for (uint16_t cnt = 0u; cnt < UI_BUF_SIZE; cnt++)
    {
        g_ui_buf[cnt] = 0u;
    }

    (void)pac1934_read_register(REFRESH_REG);
    g_vsense[PAC1934_CHANNEL_1] = pac1934_read_register(VSENSE1_AVG_REG);
    g_vsense[PAC1934_CHANNEL_2] = pac1934_read_register(VSENSE2_AVG_REG);
    g_vsense[PAC1934_CHANNEL_3] = pac1934_read_register(VSENSE3_AVG_REG);
    g_vsense[PAC1934_CHANNEL_4] = pac1934_read_register(VSENSE4_AVG_REG);

    if (pac1934_read_failed(g_vsense[PAC1934_CHANNEL_1]) ||
        pac1934_read_failed(g_vsense[PAC1934_CHANNEL_2]) ||
        pac1934_read_failed(g_vsense[PAC1934_CHANNEL_3]) ||
        pac1934_read_failed(g_vsense[PAC1934_CHANNEL_4]))
    {
        g_pac1934_sensor_available = 0U;
        pac1934_print_read_failed();
        return;
    }

    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\rCurrent values");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"    IVDD_REG       IVDDA25        IVDD25         IVDDA_REG     ");
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, UI_BUF_SIZE);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    sprintf((char*)&g_ui_buf, "    %08.3fma     %08.3fma     %08.3fma     %08.3fma    ",
                            pac1934_vsense_to_ma(g_vsense[PAC1934_CHANNEL_1]),
                            pac1934_vsense_to_ma(g_vsense[PAC1934_CHANNEL_2]),
                            pac1934_vsense_to_ma(g_vsense[PAC1934_CHANNEL_3]),
                            pac1934_vsense_to_ma(g_vsense[PAC1934_CHANNEL_4]));
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, UI_BUF_SIZE);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r\n\r\n\r\n\r");
}

void pac1934_draw_vb(void)
{
    pac1934_enable_access_path();

    if (0U == g_pac1934_sensor_available)
    {
        pac1934_print_read_failed();
        return;
    }

    for (uint16_t cnt = 0u; cnt < UI_BUF_SIZE; cnt++)
    {
        g_ui_buf[cnt] = 0u;
    }
    (void)pac1934_read_register(REFRESH_REG);
    g_vbus[PAC1934_CHANNEL_1] = pac1934_read_register(VBUS1_AVG_REG);
    g_vbus[PAC1934_CHANNEL_2] = pac1934_read_register(VBUS2_AVG_REG);
    g_vbus[PAC1934_CHANNEL_3] = pac1934_read_register(VBUS3_AVG_REG);
    g_vbus[PAC1934_CHANNEL_4] = pac1934_read_register(VBUS4_AVG_REG);

    if (pac1934_read_failed(g_vbus[PAC1934_CHANNEL_1]) ||
        pac1934_read_failed(g_vbus[PAC1934_CHANNEL_2]) ||
        pac1934_read_failed(g_vbus[PAC1934_CHANNEL_3]) ||
        pac1934_read_failed(g_vbus[PAC1934_CHANNEL_4]))
    {
        g_pac1934_sensor_available = 0U;
        pac1934_print_read_failed();
        return;
    }

    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r\n\rVoltage values");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"    VDD_REG        VDDA25         VDD25          VDDA_REG      ");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    sprintf((char*)&g_ui_buf, "     %04.2fv         %04.2fv          %04.2fv         %04.2fv          ",
                            pac1934_vbus_to_v(g_vbus[PAC1934_CHANNEL_1]),
                            pac1934_vbus_to_v(g_vbus[PAC1934_CHANNEL_2]),
                            pac1934_vbus_to_v(g_vbus[PAC1934_CHANNEL_3]),
                            pac1934_vbus_to_v(g_vbus[PAC1934_CHANNEL_4]));
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, UI_BUF_SIZE);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
}

/**************************************************************************//**
 * MSS I2C accessing current sensor PAC1934
 */
int32_t pac1934_sensor_probe(void)
{
    int32_t i2c_test_result = PAC1934_SENSOR_PROBE_FAILURE;

    pac1934_enable_access_path();

    PLIC_init();
    PLIC_SetPriority(I2C1_MAIN_PLIC, PAC1934_PLIC_PRIORITY);
    PLIC_SetPriority(I2C1_ALERT_PLIC, PAC1934_PLIC_PRIORITY);
    PLIC_SetPriority(I2C1_SUS_PLIC, PAC1934_PLIC_PRIORITY);

    __enable_irq();

    PLIC_EnableIRQ(I2C1_MAIN_PLIC);
    PLIC_EnableIRQ(I2C1_ALERT_PLIC);
    PLIC_EnableIRQ(I2C1_SUS_PLIC);

    /*-------------------------------------------------------------------------
     * Initialize the MSS I2C master Driver
     */
    MSS_I2C_init(I2C_MASTER, MASTER_SER_ADDR, MSS_I2C_PCLK_DIV_192);
    MSS_I2C_register_transfer_completion_handler(I2C_MASTER, i2c1_completion_handler);


    g_vpower_acc[PAC1934_PID_INDEX] = pac1934_read_register(PID_REG);
    g_vpower_acc[PAC1934_MID_INDEX] = pac1934_read_register(MID_REG);
    g_vpower_acc[PAC1934_REV_INDEX] = pac1934_read_register(REV_REG);

    if ((PID_PAC_1934 == g_vpower_acc[PAC1934_PID_INDEX]) &&
        (MID_MICROCHIP == g_vpower_acc[PAC1934_MID_INDEX]) &&
        (REV_DIE == g_vpower_acc[PAC1934_REV_INDEX]))
    {
        i2c_test_result = PAC1934_SENSOR_PROBE_SUCCESS;
        g_pac1934_sensor_available = 1U;
        g_pac1934_error_reported = 0U;
    }
    else
    {
        i2c_test_result = PAC1934_SENSOR_PROBE_FAILURE;
        g_pac1934_sensor_available = 0U;
    }

    return i2c_test_result;
}

mss_i2c_slave_handler_ret_t slave_write_handler
(
    mss_i2c_instance_t * this_i2c,
    uint8_t * p_rx_data,
    uint16_t rx_size
)
{
    uint8_t loop_count;

    (void)this_i2c;

    if (rx_size > BUFFER_SIZE) /* Safety check and limit the data length */
    {
        rx_size = BUFFER_SIZE;
    }

    /* Copy only the data we have received */
    for (loop_count = 0; loop_count < rx_size; loop_count++)
    {
        g_slave_tx_buffer[loop_count] = p_rx_data[loop_count];
    }

    return MSS_I2C_REENABLE_SLAVE_RX;
}

/*------------------------------------------------------------------------------
  I2C-1 completion handler
 */
static void i2c1_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status)
{
    (void)instance;

    if (status != MSS_I2C_SUCCESS)
    {
        ASSERT(0);
    }
}

/*------------------------------------------------------------------------------
 * Perform write transaction with parameters gathered from the command line
 * interface. This function is called as a result of the user's input in the
 * command line interface.
 */
static mss_i2c_status_t do_write_transaction
(
    uint8_t serial_addr,
    uint8_t * tx_buffer,
    uint8_t write_length
)
{
    mss_i2c_status_t status;

    MSS_I2C_write(I2C_MASTER, serial_addr, tx_buffer, write_length,
                  MSS_I2C_RELEASE_BUS);

    status = MSS_I2C_wait_complete(I2C_MASTER, DEMO_I2C_TIMEOUT);

    return status;
}

/*------------------------------------------------------------------------------
 * Service the I2C timeout functionality (SysTick_Handler is called every 10mS).
 */
void SysTick_Handler_h1_IRQHandler(void)
{
    MSS_I2C_system_tick(&g_mss_i2c1_lo, PAC1934_SYSTICK_INTERVAL_MS);
}

void SysTick_Handler_h0_IRQHandler(void)
{
    MSS_I2C_system_tick(&g_mss_i2c0_lo, PAC1934_SYSTICK_INTERVAL_MS);
}
