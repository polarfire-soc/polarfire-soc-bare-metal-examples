/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file pac1934.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief The PAC1934 bare metal software driver implementation.
 *
 */

/*-------------------------------------------------------------------------*//**
 * Include section
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_i2c/mss_i2c.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "pac1934_regs.h"

/*-------------------------------------------------------------------------*//**
  Macros and constants
 */
#define PAC1934_I2C_MASTER (&g_mss_i2c1_lo)
#define PAC1934_MASTER_ADDR (0x21U)
#define PAC1934_SLAVE_ADDR (0x20U)
#define PAC1934_SENSOR_ADDR (0x10U)
#define PAC1934_BUFFER_SIZE (32U)
#define PAC1934_I2C_TIMEOUT (3000U)
#define PAC1934_UI_BUF_SIZE (100U)
#define PAC1934_LINE_SYMBOL "+"
#define PAC1934_VOLTAGE_FULL_SCALE 32.0f
#define PAC1934_ADC_RESOLUTION 65535.0f
#define PAC1934_CURRENT_SCALING_FACTOR 10000.0f
#define ERROR_CODE 0xFFFFFFFFUL
#define WAIT_TIME 7250000U

/*-------------------------------------------------------------------------*//**
  Global/static variables
 */
static mss_uart_instance_t *const gp_usr_uart = &g_mss_uart0_lo;
static uint8_t g_master_tx_buf[PAC1934_BUFFER_SIZE];
static uint8_t g_master_rx_buf[PAC1934_BUFFER_SIZE];
static uint8_t g_slave_rx_buffer[PAC1934_BUFFER_SIZE];
static uint8_t g_slave_tx_buffer[PAC1934_BUFFER_SIZE];
static uint8_t g_ui_buf[PAC1934_UI_BUF_SIZE] = {0};

static uint32_t VPowerACC[4] = {0};
static uint32_t VBus[4] = {0};
static uint32_t VSense[4] = {0};
static uint32_t VBusAvg[4] = {0};
static uint32_t VSenseAvg[4] = {0};
static uint32_t VPower[4] = {0};

const uint8_t msg_pac_show_menu_again_prompt[] =
"\r\n"
"Type 0 to show the menu again\r\n"
"-------------------------------------------------------------------------\r\n";

const uint8_t msg_pac_page_break[] =
"\r\n"
"-------------------------------------------------------------------------\r\n";

const uint8_t msg_pac_reading_current[] =
"\r\n"
"++++++++++++++++++++| Reading Current sensor over I2C |++++++++++++++++++\r\n";


const uint8_t msg_pac_page_break_mid[] =
"\r\n"
"                     ---------------------------------                   \r\n";

const uint8_t msg_pac_page_break_edge[] =
"\r\n"
"+                                                                       +\r\n";

const uint8_t msg_pac_column_title_current_vals[] =
"    IVDD_REG       IVDDA25        IVDD25         IVDDA_REG     ";

const uint8_t msg_pac_column_title_voltage_vals[] =
"    VDD_REG        VDDA25         VDD25          VDDA_REG      ";

const uint8_t msg_pac_voltage_prints[] =
"    %04.2fv          %04.2fv          %04.2fv          %04.2fv         ";

const uint8_t msg_pac_current_prints[] =
"    %08.3fma     %08.3fma     %08.3fma     %08.3fma    ";

const uint8_t msg_pac_column_title_power_vals[] =
"   VPowerACC1    VPowerACC2     VPowerACC3      VPowerACC4     ";

/*-------------------------------------------------------------------------*//**
  Static function prototypes
 */
static mss_i2c_status_t pac1934_do_write(uint8_t serial_addr,
                                         uint8_t *tx_buffer,
                                         uint8_t write_length);
static mss_i2c_status_t pac1934_do_write_read(uint8_t serial_addr,
                                              uint8_t *tx_buffer,
                                              uint8_t write_length,
                                              uint8_t *rx_buffer,
                                              uint8_t read_length);
static uint32_t pac1934_sensor_reg_check(uint32_t reg);

/*-------------------------------------------------------------------------*//**
  I2C completion handler
 */
void i2c1_completion_handler(mss_i2c_instance_t *instance,
                             mss_i2c_status_t status)
{
    if (status != MSS_I2C_SUCCESS)
    {
        ASSERT(0U);
    }
}

/*-------------------------------------------------------------------------*//**
  I2C write transaction
 */
static mss_i2c_status_t pac1934_do_write(uint8_t serial_addr,
                                         uint8_t *tx_buffer,
                                         uint8_t write_length)
{
    MSS_I2C_write(PAC1934_I2C_MASTER, serial_addr, tx_buffer, write_length,
                  MSS_I2C_RELEASE_BUS);
    return MSS_I2C_wait_complete(PAC1934_I2C_MASTER, PAC1934_I2C_TIMEOUT);
}

/*-------------------------------------------------------------------------*//**
  I2C write-read transaction
 */
static mss_i2c_status_t pac1934_do_write_read(uint8_t serial_addr,
                                              uint8_t *tx_buffer,
                                              uint8_t write_length,
                                              uint8_t *rx_buffer,
                                              uint8_t read_length)
{
    MSS_I2C_write_read(PAC1934_I2C_MASTER, serial_addr, tx_buffer, write_length,
                       rx_buffer, read_length, MSS_I2C_RELEASE_BUS);
    return MSS_I2C_wait_complete(PAC1934_I2C_MASTER, MSS_I2C_NO_TIMEOUT);
}

/*-------------------------------------------------------------------------*//**
  Read PAC1934 sensor register value
 */
static uint32_t pac1934_sensor_reg_check(uint32_t reg)
{
    uint32_t value = 0U;
    mss_i2c_status_t status;
    uint8_t num_receive_bytes = 0U;

    g_master_tx_buf[0] = (uint8_t)reg;

    /* refresh */
    if ((reg == REFRESH_REG) || (reg == REFRESH_G_REG) || (reg == REFRESH_V_REG))
    {
        status = pac1934_do_write(PAC1934_SENSOR_ADDR, g_master_tx_buf, 1U);
        for (volatile uint32_t cnt = 0U; cnt < WAIT_TIME; cnt++)
        {
            ;
        }
    }
    /* vpower_acc */
    else if ((reg >= VPOWER1_ACC_REG) && (reg <= VPOWER4_ACC_REG))
    {
        num_receive_bytes = 6U;
        status = pac1934_do_write_read(PAC1934_SENSOR_ADDR, g_master_tx_buf, 1U,
                                       g_master_rx_buf, num_receive_bytes);
    }
    /* vbus, vsense, vsense_avg*/
    else if ((reg >= VBUS1_REG) && (reg <= VSENSE4_AVG_REG))
    {
        num_receive_bytes = 2U;
        status = pac1934_do_write_read(PAC1934_SENSOR_ADDR, g_master_tx_buf, 1U,
                                       g_master_rx_buf, num_receive_bytes);
    }
    /* vpower */
    else if ((reg >= VPOWER1_REG) && (reg <= VPOWER4_REG))
    {
        num_receive_bytes = 4U;
        status = pac1934_do_write_read(PAC1934_SENSOR_ADDR, g_master_tx_buf, 1U,
                                       g_master_rx_buf, num_receive_bytes);
    }
    /* IDs */
    else if ((reg >= PID_REG) && (reg <= REV_REG))
    {
        num_receive_bytes = 1U;
        status = pac1934_do_write_read(PAC1934_SENSOR_ADDR, g_master_tx_buf, 1U,
                                       g_master_rx_buf, num_receive_bytes);
    }
    else
    {
        ASSERT(0U);
        return ERROR_CODE;
    }

    if (status == MSS_I2C_SUCCESS)
    {
        /* vpower_acc */
        if ((reg >= VPOWER1_ACC_REG) && (reg <= VPOWER4_ACC_REG))
        {
            value = ((uint32_t)g_master_rx_buf[0] << 40) |
                    ((uint32_t)g_master_rx_buf[1] << 32) |
                    ((uint32_t)g_master_rx_buf[2] << 24) |
                    ((uint32_t)g_master_rx_buf[3] << 16) |
                    ((uint32_t)g_master_rx_buf[4] << 8) |
                    ((uint32_t)g_master_rx_buf[5]);
        }
        /* vbus, vsense, vsense_avg */
        else if ((reg >= VBUS1_REG) && (reg <= VSENSE4_AVG_REG))
        {
            value = ((uint32_t)g_master_rx_buf[0] << 8) |
                    ((uint32_t)g_master_rx_buf[1]);
        }
        /* vpower */
        else if ((reg >= VPOWER1_REG) && (reg <= VPOWER4_REG))
        {
            value = ((uint32_t)g_master_rx_buf[0] << 24) |
                    ((uint32_t)g_master_rx_buf[1] << 16) |
                    ((uint32_t)g_master_rx_buf[2] << 8) |
                    ((uint32_t)g_master_rx_buf[3]);
        }
        /* IDs */
        else if ((reg >= PID_REG) && (reg <= REV_REG))
        {
            value = g_master_rx_buf[0];
        }
        return value;
    }
    else
    {
        return ERROR_CODE;
    }
}

/*-------------------------------------------------------------------------*//**
  UART UI functions
 */
void pac1934_cls(void)
{
    for (uint16_t x = 0U; x < PAC1934_UI_BUF_SIZE; x++)
    {
        MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    }
}

void pac1934_draw_bar(void)
{
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)msg_pac_page_break);
}

void pac1934_blank_line(void)
{
  MSS_UART_polled_tx_string(gp_usr_uart,
                            (const uint8_t *)msg_pac_page_break_edge);
}

void pac1934_draw_name(void)
{
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)msg_pac_page_break_mid);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)msg_pac_reading_current);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)msg_pac_page_break_mid);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
}

void pac1934_draw_isense(void)
{
    float isense[4];

    memset(g_ui_buf, 0, sizeof(g_ui_buf));

    (void)pac1934_sensor_reg_check(REFRESH_REG);
    VBus[0] = pac1934_sensor_reg_check(VSENSE1_AVG_REG);
    VBus[1] = pac1934_sensor_reg_check(VSENSE2_AVG_REG);
    VBus[2] = pac1934_sensor_reg_check(VSENSE3_AVG_REG);
    VBus[3] = pac1934_sensor_reg_check(VSENSE4_AVG_REG);

    for (uint8_t i = 0U; i < 4U; i++)
    {
      isense[i] =
          (VBus[i] * PAC1934_CURRENT_SCALING_FACTOR) / PAC1934_ADC_RESOLUTION;
    }

    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)"\n\rCurrent values\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(
        gp_usr_uart, (const uint8_t *)msg_pac_column_title_current_vals);
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, PAC1934_UI_BUF_SIZE);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);

    snprintf((char *)g_ui_buf, PAC1934_UI_BUF_SIZE, msg_pac_current_prints,
             isense[0], isense[1], isense[2], isense[3]);
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, PAC1934_UI_BUF_SIZE);

    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)msg_pac_show_menu_again_prompt);
}

void pac1934_draw_vb(void)
{
    memset(g_ui_buf, 0, sizeof(g_ui_buf));

    (void)pac1934_sensor_reg_check(REFRESH_REG);
    VBus[0] = pac1934_sensor_reg_check(VBUS1_AVG_REG);
    VBus[1] = pac1934_sensor_reg_check(VBUS2_AVG_REG);
    VBus[2] = pac1934_sensor_reg_check(VBUS3_AVG_REG);
    VBus[3] = pac1934_sensor_reg_check(VBUS4_AVG_REG);

    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)"\n\rVoltage values\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(
        gp_usr_uart, (const uint8_t *)msg_pac_column_title_voltage_vals);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);

    snprintf((char *)g_ui_buf, PAC1934_UI_BUF_SIZE, msg_pac_voltage_prints,
            VBus[0] * PAC1934_VOLTAGE_FULL_SCALE / PAC1934_ADC_RESOLUTION,
            VBus[1] * PAC1934_VOLTAGE_FULL_SCALE / PAC1934_ADC_RESOLUTION,
            VBus[2] * PAC1934_VOLTAGE_FULL_SCALE / PAC1934_ADC_RESOLUTION,
            VBus[3] * PAC1934_VOLTAGE_FULL_SCALE / PAC1934_ADC_RESOLUTION);

    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, PAC1934_UI_BUF_SIZE);

    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
}

void pac1934_draw_vpoweracc(const uint32_t *VPower)
{
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)msg_pac_column_title_power_vals);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)PAC1934_LINE_SYMBOL);
    MSS_UART_polled_tx_string(gp_usr_uart,
                              (const uint8_t *)"\n\r");
}

/*-------------------------------------------------------------------------*//**
  MSS I2C accessing current sensor PAC1934
 */
int32_t pac1934_sensor_probe(void)
{
    int32_t i2c_test_result = -1;

    PLIC_init();
    PLIC_SetPriority(I2C1_MAIN_PLIC, 2U);
    PLIC_SetPriority(I2C1_ALERT_PLIC, 2U);
    PLIC_SetPriority(I2C1_SUS_PLIC, 2U);

    __enable_irq();

    PLIC_EnableIRQ(I2C1_MAIN_PLIC);
    PLIC_EnableIRQ(I2C1_ALERT_PLIC);
    PLIC_EnableIRQ(I2C1_SUS_PLIC);

    MSS_I2C_init(PAC1934_I2C_MASTER, PAC1934_MASTER_ADDR, MSS_I2C_PCLK_DIV_192);
    MSS_I2C_register_transfer_completion_handler(PAC1934_I2C_MASTER,
                                                 i2c1_completion_handler);

    VPowerACC[0] = pac1934_sensor_reg_check(PID_REG);
    VPowerACC[1] = pac1934_sensor_reg_check(MID_REG);
    VPowerACC[2] = pac1934_sensor_reg_check(REV_REG);

    if ((PID_PAC_1934 == VPowerACC[0]) && (MID_MICROCHIP == VPowerACC[1]) &&
        (REV_DIE == VPowerACC[2]))
        i2c_test_result = 0;
    else
        i2c_test_result = -1;

    return i2c_test_result;
}

/*-------------------------------------------------------------------------*//**
  I2C slave write handler
 */
mss_i2c_slave_handler_ret_t pac1934_slave_write_handler(
    mss_i2c_instance_t *this_i2c, uint8_t *p_rx_data, uint16_t rx_size)
{
    uint16_t copy_size =
        rx_size > PAC1934_BUFFER_SIZE ? PAC1934_BUFFER_SIZE : rx_size;

    for (uint16_t i = 0U; i < copy_size; i++)
    {
        g_slave_tx_buffer[i] = g_slave_rx_buffer[i];
    }

    return MSS_I2C_REENABLE_SLAVE_RX;
}

/*-------------------------------------------------------------------------*//**
  SysTick Handler (empty)
 */
void SysTick_Handler_h0_IRQHandler(void)
{
    /* Empty handler */
}
