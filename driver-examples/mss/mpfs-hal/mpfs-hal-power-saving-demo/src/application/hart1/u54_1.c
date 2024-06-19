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
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "inc/common.h"

/* Comment out line below to turn on power to parked hart*/
// #ifdef TURN_OFF_POWER_TO_PARKED_HARTS

static void place_pattern_in_memory(void);
static void clear_pattern_in_memory(void);
static uint8_t verify_data_in_mem(void);
static void check_self_refresh_status(void);

extern uint64_t ddr_test;
extern mss_uart_instance_t *g_uart;
extern uint32_t  ddr_sr_test;

volatile uint32_t count_sw_ints_h1 = 0U;

#define NO_OF_ITERATIONS    2
#define DDR_NON_CACHED_BASE            BASE_ADDRESS_NON_CACHED_32_DDR
#define DDR_NON_CACHED_SIZE            0x10000000U

#define MIN_OFFSET          1U
#define MAX_OFFSET          16U
#define START_OFFSET        12U

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

    error = 0U;

    while (1U)
    {
        /* 1  Place pattern in memory */
        if(ddr_sr_test == 1U)
        {
            ddr_sr_test = 0U;
            clear_pattern_in_memory();
        }
        /* 2  Clear pattern in memory */
        if(ddr_sr_test == 2U)
        {
            ddr_sr_test = 0U;
            place_pattern_in_memory();
        }
        /* 3  Verify if pattern is in memory */
        if(ddr_sr_test == 3U)
        {
            ddr_sr_test = 0U;
            verify_data_in_mem();
        }
        /* 4  Turn on DDR self-refresh */
        if(ddr_sr_test == 4U)
        {
            ddr_sr_test = 0U;
            asm("fence.i");
            flush_l2_cache((uint32_t)1U);
            mpfs_hal_turn_ddr_selfrefresh_on();
            MSS_UART_polled_tx_string(g_uart, 
                "DDR self-refresh turned on, check status by typing '6'\r\n");
        }
        /* 5  Turn off DDR self-refresh */
        if(ddr_sr_test == 5U)
        {
            ddr_sr_test = 0U;
            mpfs_hal_turn_ddr_selfrefresh_off();
            MSS_UART_polled_tx_string(g_uart, 
            "DDR self-refresh turned off, check status by typing '6'\r\n");        
        }
        /* 6  Check ddr self refresh status */
        if(ddr_sr_test == 6U)
        {
            ddr_sr_test = 0U;
            check_self_refresh_status();
        }
    }
}

static void check_self_refresh_status(void)
{
    if (mpfs_hal_ddr_selfrefresh_status() == 0)
    {
        MSS_UART_polled_tx_string(g_uart, 
                                  "Self refresh status: ON\r\n");
    }
    else
    {
        MSS_UART_polled_tx_string(g_uart, 
                                  "Self refresh status: OFF\r\n");
    }
}

static void place_pattern_in_memory(void)
{
    volatile uint32_t *ddr_mem = DDR_NON_CACHED_BASE;
    *ddr_mem = 0x123456U;
    MSS_UART_polled_tx_string(g_uart, "0x123456 placed in DDR memory\r\n");
}

static void clear_pattern_in_memory(void)
{
    volatile uint32_t *ddr_mem = DDR_NON_CACHED_BASE;
    *ddr_mem = 0x000000U;
    MSS_UART_polled_tx_string(g_uart, "0x000000 placed in DDR memory\r\n");
}

static uint8_t verify_data_in_mem(void)
{
    volatile uint32_t *ddr_mem = DDR_NON_CACHED_BASE;
    if (*ddr_mem == 0x123456U)
    {
        MSS_UART_polled_tx_string(g_uart, "DDR verification: PASSED\r\n");
    }
    else
    {
        MSS_UART_polled_tx_string(g_uart, "DDR verification: FAILED\r\n");
    }
}

// #endif
