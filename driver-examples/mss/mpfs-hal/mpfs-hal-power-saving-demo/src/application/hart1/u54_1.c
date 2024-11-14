/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 */
/*******************************************************************************
 *
 * Code running on U54 first hart
 *
 */

/* Comment out line below to turn on power to parked hart*/
// #ifdef TURN_OFF_POWER_TO_PARKED_HARTS

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"
#include "inc/menu_selector.h"
#include "inc/state_machine.h"
#include "drivers/off-chip/pac1934/pac1934.h"

static void check_self_refresh_status(void);
static void clear_pattern_in_memory_block(void);
static void place_pattern_in_memory_block(void);
static void verify_pattern_in_memory_block(void);

extern mss_uart_instance_t *g_uart;
extern volatile uint32_t ddr_sr_test;
extern volatile uint32_t monitor_current_flag;

uint8_t rx_size = 0;
uint8_t g_rx_buff[1] = {0};

volatile uint32_t count_sw_ints_h1 = 0U;
uint32_t user_sm_request_h1 = 0U;

#define NO_OF_ITERATIONS                2
#define DDR_NON_CACHED_BASE             BASE_ADDRESS_NON_CACHED_32_DDR
#define DDR_NON_CACHED_SIZE             0x10000000U

#define MIN_OFFSET                      1U
#define MAX_OFFSET                      16U
#define START_OFFSET                    12U

#define DDR_TOGGLE_OUTPUTS              0x1U

#define MAX_ADDRESS                     0x80001000UL

char info_string[100];

/* Main function for the HART1(U54_1 processor).
 * Application code running on HART1 is placed here
 *
 * The HART1 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_1(void)
{
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
    uint32_t error;
    volatile PATTERN_TEST_PARAMS pattern_test;

    volatile uint32_t icount = 0U;

    pattern_test.base = DDR_NON_CACHED_BASE;
    pattern_test.size = DDR_NON_CACHED_SIZE;
    pattern_test.pattern_type = DDR_TEST_FILL;
    pattern_test.pattern_offset = START_OFFSET;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /*Put this hart into WFI.*/
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();
     PLIC_init();

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t)1, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_I2C1, (uint8_t)1, PERIPHERAL_ON);

    MSS_UART_init(&g_mss_uart1_lo,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    if (0 == PAC1934_sensor_probe())
    {
        ;
    }
    else
    {
        MSS_UART_polled_tx_string (g_uart, "\r\nPAC1934 sensor probe failed");
    }

    error = 0U;
    ddr_sr_test = 0;
    monitor_current_flag = 0;

    while (1U)
    {
        /* 1  Place pattern in memory */
        if (ddr_sr_test == 1U)
        {
            clear_pattern_in_memory_block();
            ddr_sr_test = 0U;
        }
        /* 2  Clear pattern in memory */
        if (ddr_sr_test == 2U)
        {
            place_pattern_in_memory_block();
            ddr_sr_test = 0U;
        }
        /* 3  Verify if pattern is in memory */
        if (ddr_sr_test == 3U)
        {
            verify_pattern_in_memory_block();
            ddr_sr_test = 0U;
        }
        /* 4  Turn on DDR self-refresh */
        if (ddr_sr_test == 4U)
        {
            asm("fence.i");
            flush_l2_cache((uint32_t)1U);
            mpfs_hal_turn_ddr_selfrefresh_on();
            MSS_UART_polled_tx_string(g_uart,
            "DDR self-refresh turned on, check status by typing '6'\r\n");
            ddr_sr_test = 0U;
        }
        /* 5  Turn off DDR self-refresh */
        if (ddr_sr_test == 5U)
        {
            mpfs_hal_turn_ddr_selfrefresh_off();
            MSS_UART_polled_tx_string(g_uart,
            "DDR self-refresh turned off, check status by typing '6'\r\n");        
            ddr_sr_test = 0U;
        }
        /* 6  Check ddr self refresh status */
        if (ddr_sr_test == 6U)
        {
            check_self_refresh_status();
            ddr_sr_test = 0U;
        }
        /* 7  Turn off ddr pll outputs */
        if (ddr_sr_test == 7U)
        {
            mpfs_hal_ddr_logic_power_state(DDR_LOW_POWER,
                                            DDR_TOGGLE_OUTPUTS);
            MSS_UART_polled_tx_string(g_uart, 
            "DDR PLL output status: DISABLED\r\n");
            ddr_sr_test = 0U;
        }
        /* 8  Turn on ddr pll outputs */
        if (ddr_sr_test == 8U)
        {
            mpfs_hal_ddr_logic_power_state(DDR_NORMAL_POWER,
                                            DDR_TOGGLE_OUTPUTS);
            MSS_UART_polled_tx_string(g_uart, 
            "DDR PLL output status: ENABLED\r\n");
            ddr_sr_test = 0U;
        }
        /* Monitor current */
        if (monitor_current_flag == 1U)
        {
            PAC1934_drawVB();
            PAC1934_drawISense();
            monitor_current_flag = 0U;
        }

        state_machine_h1();
    }
}

static void check_self_refresh_status(void)
{
    if (mpfs_hal_ddr_selfrefresh_status() == 0)
    {
        MSS_UART_polled_tx_string(g_uart, "Self refresh status: ON\r\n");
    }
    else
    {
        MSS_UART_polled_tx_string(g_uart, "Self refresh status: OFF\r\n");
    }
}

static void clear_pattern_in_memory_block(void)
{
    volatile uint32_t *mem_pointer = (uint32_t*)BASE_ADDRESS_CACHED_32_DDR;
    
    while (mem_pointer < (uint32_t*)MAX_ADDRESS)
    {
        *mem_pointer = 0x00000000U;
        sprintf(info_string, "0x00 added to memory 0x%lx \r\n", mem_pointer);
        MSS_UART_polled_tx_string(g_uart, info_string);
        mem_pointer++;
    }
    MSS_UART_polled_tx_string(g_uart, "\r\nType 0 to show the menu again\r\n");
}

static void place_pattern_in_memory_block(void)
{
    volatile uint32_t *mem_pointer = (uint32_t*)BASE_ADDRESS_CACHED_32_DDR;
    
    while (mem_pointer < (uint32_t*)MAX_ADDRESS)
    {
        *mem_pointer = PATTERN_WALKING_ONE;
        sprintf(info_string, "0b10 added to memory 0x%lx \r\n", mem_pointer);
        MSS_UART_polled_tx_string(g_uart, info_string);
        mem_pointer++;
    }
    MSS_UART_polled_tx_string(g_uart, "\r\nType 0 to show the menu again\r\n");
}

static void verify_pattern_in_memory_block(void)
{
    volatile uint32_t *mem_pointer = (uint32_t*)BASE_ADDRESS_CACHED_32_DDR;
    volatile uint32_t count_fails = 0U;

    while (mem_pointer < (uint32_t*)MAX_ADDRESS)
    {

        sprintf(info_string, "Checking for 0b10 in memory 0x%lx: 0x%lx FOUND --> ", mem_pointer, *mem_pointer);
        MSS_UART_polled_tx_string(g_uart, info_string);

        if (*mem_pointer == PATTERN_WALKING_ONE)
        {
            MSS_UART_polled_tx_string(g_uart, "PASSED\r\n");
        }
        else
        {
            count_fails++;
            MSS_UART_polled_tx_string(g_uart, "FAILED\r\n");
        }
        mem_pointer++;
    }

    if (count_fails > 0U)
    {
        MSS_UART_polled_tx_string(g_uart, "\r\nREGISTER VERIFICATION: FAILED\r\n\r\n");
    }
    else
    {
        MSS_UART_polled_tx_string(g_uart, "\r\nREGISTER VERIFICATION: SUCCEEDED\r\n\r\n");
    }

    MSS_UART_polled_tx_string(g_uart, "Type 0 to show the menu again\r\n");
}

/* Comment out line below to turn on power to parked hart*/
// #endif
