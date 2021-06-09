/**************************************************************************//**
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file implements access functions for the PAC1934 using the MSS I2C
 * driver APIs
 *
 */

#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_i2c/mss_i2c.h"
#include "drivers/mss_mmuart/mss_uart.h"
#include "./pac1934_regs.h"

static mss_uart_instance_t * const gp_usr_uart = &g_mss_uart0_lo;
unsigned long VPowerACC[4] = {0},VBus[4] = {0}, VSense[4] = {0}, \
            VBusAvg[4] = {0}, VSenseAvg[4] = {0}, VPower[4] = {0};

void i2c1_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status);

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
 * I2C slave serial address.
 */
#define SLAVE_SER_ADDR          0x20
#define CURREN_SNSOR_ADDR       0x10

#define BUFFER_SIZE    32u
static uint8_t g_master_rx_buf[BUFFER_SIZE];
static uint8_t g_master_tx_buf[BUFFER_SIZE];
static uint8_t g_slave_rx_buffer[BUFFER_SIZE];
static uint8_t g_slave_tx_buffer[BUFFER_SIZE];

/*-----------------------------------------------------------------------------
 * I2C operation timeout value in mS. Define as MSS_I2C_NO_TIMEOUT to disable
 * the timeout functionality.
 */
#define DEMO_I2C_TIMEOUT    3000u

/*-----------------------------------------------------------------------------
 * UI Buffer
 */
#define UIBufSize 100
#define lineSymbol "+"
uint8_t g_ui_buf[UIBufSize] = {0};

/* Draw UI */
#define windowWidth UIBufSize
#define windowHeight UIBufSize

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

    status = MSS_I2C_wait_complete(I2C_MASTER, MSS_I2C_NO_TIMEOUT);

    return status;
}

/* Check PAC1934 sensor register value */
static unsigned long sensor_reg_check(uint32_t reg)
{
    long value = 0;
    mss_i2c_status_t instance;
    uint8_t num_receive_bytes = 0;
    float voltage = 0;

    g_master_tx_buf[0] = reg;

    if((reg == 0x00u) | (reg == 0x1Eu) | (reg == 0x1Fu))   /* refresh */
    {
        instance = do_write_transaction(CURREN_SNSOR_ADDR, g_master_tx_buf, 1u);
        for(uint32_t cnt=0u; cnt < 7250000u; cnt++);  /* min 2ms TODO: improve this*/
    }
    else if ((reg >= 0x3u) && (reg <= 0x6u))          /* Vpower_acc */
    {
        num_receive_bytes = 6u;
        instance = do_write_read_transaction(CURREN_SNSOR_ADDR,
                                             g_master_tx_buf,
                                             0x01u,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else if ((reg >= 0x7u) && (reg <= 0x16u)) /* vbus, Vsense, Vsense_avg*/
    {
        num_receive_bytes = 2u;
        instance = do_write_read_transaction(CURREN_SNSOR_ADDR,
                                             g_master_tx_buf,
                                             0x01U,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else if ((reg >= 0x17u) && (reg <= 0x1Au)) /* Vpower */
    {
        num_receive_bytes = 4u;
        instance = do_write_read_transaction(CURREN_SNSOR_ADDR,
                                             g_master_tx_buf,
                                             0x01U,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else if ((reg >= 0xFD) && (reg <= 0xFF))    /* IDs */
    {
        num_receive_bytes = 1u;
        instance = do_write_read_transaction(CURREN_SNSOR_ADDR,
                                             g_master_tx_buf,
                                             0x01U,
                                             g_master_rx_buf,
                                             num_receive_bytes);
    }
    else
    {
        ASSERT(0U);
    }

     if(MSS_I2C_SUCCESS == instance)
     {
         if ((reg >= 0x3) && (reg <= 0x6))  /*Vpower_acc*/
         {
             value = g_master_rx_buf[0];
             value = value << 8;
             value = value + g_master_rx_buf[1];
             value = value << 8;
             value = value + g_master_rx_buf[2];
             value = value << 8;
             value = value + g_master_rx_buf[3];
             value = value << 8;
             value = value + g_master_rx_buf[4];
             value = value << 8;
             value = value + g_master_rx_buf[5];
         }
         else if ((reg >= 0x7) && (reg <= 0x16))   /* vbus, Vsense, Vsense_avg */
         {
             value = g_master_rx_buf[0];
             value = value << 8;
             value = value + g_master_rx_buf[1];
         }
         else if ((reg >= 0x17) && (reg <= 0x1A))   /* Vpower */
         {
             value = g_master_rx_buf[0];
             value = value << 8;
             value = value + g_master_rx_buf[1];
             value = value << 8;
             value = value + g_master_rx_buf[2];
             value = value << 8;
             value = value + g_master_rx_buf[3];
         }
         else if ((reg >= 0xFD) && (reg <= 0xFF))   /* IDs */
         {
             value = g_master_rx_buf[0];
         }

         return(value);
     }
     else
     {
         return (0xFFFFFFFFUL);
     }
}

void cls(void)
{
    for (uint16_t x = 0u; x < windowHeight; x++)
    {
        MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    }
}

void drawBar(void)
{
        /* Draw bar */
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");

}

void blankLine(void)
{
        /* Draw name */
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"+                                                             +");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");

}

void drawName(void)
{
    /* Draw name */
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"              ----------------------------------             ");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"+++++++++++++++| Reading Current sensor over I2C |++++++++++++++");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"              ----------------------------------             ");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
}

void PAC1934_drawISense(void)
{
    volatile float isense0,isense1,isense2,isense3;

    for (uint16_t cnt = 0u; cnt < UIBufSize; cnt++)
    {
        g_ui_buf[cnt] = 0u;
    }

    sensor_reg_check(REFRESH_REG);
    VBus[0] = sensor_reg_check(VSENSE1_AVG_REG);
    isense0 = (VBus[0u]*10000.0)/65535.0;

    VBus[1] = sensor_reg_check(VSENSE2_AVG_REG);
    isense1 = (VBus[1u]*10000.0)/65535.0;

    VBus[2] = sensor_reg_check(VSENSE3_AVG_REG);
    isense2 = (VBus[2u]*10000.0)/65535.0;

    VBus[3] = sensor_reg_check(VSENSE4_AVG_REG);
    isense3 = (VBus[3u]*10000.0)/65535.0;

    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\rCurrent values");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"    IVDD_REG       IVDDA25        IVDD25         IVDDA_REG     ");
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, UIBufSize);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    sprintf((char*)&g_ui_buf, "    %08.3fma     %08.3fma     %08.3fma     %08.3fma    ",
                            (VBus[0u]*10000.0)/65535.0,
                            (VBus[1u]*10000.0)/65535.0,
                            (VBus[2u]*10000.0)/65535.0,
                            (VBus[3u]*10000.0)/65535.0);
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, UIBufSize);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r\n\r\n\r\n\r");
}

void PAC1934_drawVB(void)
{
    for (uint16_t cnt = 0u; cnt < UIBufSize; cnt++)
    {
        g_ui_buf[cnt] = 0u;
    }
    sensor_reg_check(REFRESH_REG);
    VBus[0u] = sensor_reg_check(VBUS1_AVG_REG);
    VBus[1u] = sensor_reg_check(VBUS2_AVG_REG);
    VBus[2u] = sensor_reg_check(VBUS3_AVG_REG);
    VBus[3u] = sensor_reg_check(VBUS4_AVG_REG);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r\n\rVoltage values");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"    VDD_REG        VDDA25         VDD25          VDDA_REG      ");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    sprintf((char*)&g_ui_buf, "     %04.2fv         %04.2fv          %04.2fv         %04.2fv          ",
                            VBus[0]*32.0/65535.0,
                            VBus[1]*32.0/65535.0,
                            VBus[2]*32.0/65535.0,
                            VBus[3]*32.0/65535.0);
    MSS_UART_polled_tx(gp_usr_uart, g_ui_buf, UIBufSize);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
}

void drawVPowerAcc(uint32_t *VPower)
{
        /* Draw name */
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"    VPowerACC1    VPowerACC2    VPowerACC3    VPowerACC4     ");
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)lineSymbol);
    MSS_UART_polled_tx_string(gp_usr_uart, (const uint8_t *)"\n\r");
}

/**************************************************************************//**
 * MSS I2C accessing current sensor PAC1934
 */
int32_t PAC1934_sensor_probe(void)
{
    uint8_t cmd_idx = 0;
    int32_t i2c_test_result = -1;

    PLIC_init();
    PLIC_SetPriority(I2C1_MAIN_PLIC, 2);
    PLIC_SetPriority(I2C1_ALERT_PLIC, 2);
    PLIC_SetPriority(I2C1_SUS_PLIC, 2);

    __enable_irq();

    PLIC_EnableIRQ(I2C1_MAIN_PLIC);
    PLIC_EnableIRQ(I2C1_ALERT_PLIC);
    PLIC_EnableIRQ(I2C1_SUS_PLIC);

    /*-------------------------------------------------------------------------
     * Initialize the MSS I2C master Driver
     */
    MSS_I2C_init(I2C_MASTER, MASTER_SER_ADDR, MSS_I2C_PCLK_DIV_192);
    MSS_I2C_register_transfer_completion_handler(I2C_MASTER, i2c1_completion_handler);


    VPowerACC[0] = sensor_reg_check(PID_REG);
    VPowerACC[1] = sensor_reg_check(MID_REG);
    VPowerACC[2] = sensor_reg_check(REV_REG);

    if ((0x5b == VPowerACC[0]) && (0x5d == VPowerACC[1]) && (0x03 == VPowerACC[2]))
    {
        i2c_test_result= 0u;
    }
    else
    {
        i2c_test_result= -1;
    }

    return(i2c_test_result);
}

mss_i2c_slave_handler_ret_t slave_write_handler
(
    mss_i2c_instance_t * this_i2c,
    uint8_t * p_rx_data,
    uint16_t rx_size
)
{
    uint8_t loop_count;

    if (rx_size > BUFFER_SIZE) /* Safety check and limit the data length */
    {
        rx_size = BUFFER_SIZE;
    }

    /* Copy only the data we have received */
    for (loop_count = 0; loop_count < rx_size; loop_count++)
    {
        g_slave_tx_buffer[loop_count] = g_slave_rx_buffer[loop_count];
    }

    return MSS_I2C_REENABLE_SLAVE_RX;
}

/*------------------------------------------------------------------------------
  I2C-1 completion handler
 */
void i2c1_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status)
{
    if(status == MSS_I2C_SUCCESS)
    {
    }
    else
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
void SysTick_Handler_h0_IRQHandler(void)
{
}

