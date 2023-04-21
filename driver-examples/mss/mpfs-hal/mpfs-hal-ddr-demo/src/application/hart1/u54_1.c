/*******************************************************************************
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solution.
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

extern uint64_t hart_jump_ddr;
extern uint64_t ddr_test;
extern mss_uart_instance_t *g_uart;
extern char info_string[];

volatile uint32_t count_sw_ints_h1 = 0U;

#define NO_OF_ITERATIONS    2
#define DDR_BASE            0x80000000u
#define DDR_SIZE            0x40000000u

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

    pattern_test.base = DDR_BASE;
    pattern_test.size = DDR_SIZE;
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
        if((hart_jump_ddr == 1U) ||
           (hart_jump_ddr == 12U) ||
           (hart_jump_ddr == 123U) ||
           (hart_jump_ddr == 1234U))
        {
            jump_to_application(hls, M_MODE, (uint64_t)0x80000000);
        }
        if(ddr_test == 1U)
        {
            load_ddr_pattern(&pattern_test);
            MSS_UART_polled_tx(g_uart, (const uint8_t*)\
                "Press x to abort DDR test\r\n",(uint32_t)\
                    strlen("Press x to abort DDR test\r\n"));
            setup_ddr_segments(DEFAULT_SEG_SETUP);
            error |= test_ddr(NO_OF_ITERATIONS, &pattern_test);
            setup_ddr_segments(LIBERO_SEG_SETUP);

            if (++pattern_test.pattern_offset > MAX_OFFSET)
            {
                pattern_test.pattern_offset = MIN_OFFSET;
            }
            sprintf(info_string, "\r\npattern_offset: %u\r\n",\
                    pattern_test.pattern_offset);
            MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,\
                    (uint32_t)strlen(info_string));
            if ( error == 0U )
            {
                MSS_UART_polled_tx(g_uart,
                        (const uint8_t*) "M2M Test passed test_ddr\r\n",\
                            (uint32_t)\
                                strlen("M2M Test passed test_ddr\r\n"));
            }
            else
            {
                MSS_UART_polled_tx(g_uart, (const uint8_t*)\
                    "M2M Test failed test_ddr\r\n",(uint32_t)\
                        strlen("M2M Test failed test_ddr\r\n"));
            }
        }
        if(ddr_test == 3U)
        {
            ddr_read_full_range ();
            ddr_test = 0U;
        }
    }
    /* never return */
}

/* HART1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    count_sw_ints_h1++;
}
