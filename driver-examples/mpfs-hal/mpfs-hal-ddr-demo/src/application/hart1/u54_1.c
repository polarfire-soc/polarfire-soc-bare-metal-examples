/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solution.
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

#ifndef SIFIVE_HIFIVE_UNLEASHED
#include "drivers/mss_mmuart/mss_uart.h"
#else
#include "drivers/FU540_uart/FU540_uart.h"
#endif

extern uint64_t uart_lock;
extern MEM_TYPE mem_area;
extern uint64_t hart_jump_ddr;
extern uint64_t ddr_test;
extern mss_uart_instance_t *g_uart;

volatile uint32_t count_sw_ints_h1 = 0U;

#define NO_OF_ITERATIONS    2
#define DDR_BASE            0x80000000u
#define DDR_SIZE            0x40000000u

#define MIN_OFFSET          1U
#define MAX_OFFSET          16U

/* Main function for the HART1(U54_1 processor).
 * Application code running on HART1 is placed here
 *
 * The HART1 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_1(void)
{
    char info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;
    uint32_t pattern_offset = 12U;

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

    while (1U)
    {
        icount++;
        if (0x7FFFFFFFU == icount)
        {
            icount = 0U;
            sprintf(info_string,\
                    "Hart %lu, use option 6 and 7 to jump to DDR program\r\n",\
                        hartid);
            MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
        }

        if(ddr_test == 1U)
        {
            load_ddr_pattern(DDR_BASE, DDR_SIZE,pattern_offset);
            MSS_UART_polled_tx(g_uart, (const uint8_t*)"Press x to abort DDR test\r\n",(uint32_t)strlen("Press x to abort DDR test\r\n"));
            setup_ddr_segments(DEFAULT_SEG_SETUP);
        	test_ddr(NO_OF_ITERATIONS, DDR_SIZE);
        	setup_ddr_segments(LIBERO_SEG_SETUP);

        	if (pattern_offset > MAX_OFFSET)
        	{
        	    pattern_offset = MIN_OFFSET;
        	}
        }

        if(hart_jump_ddr == 1U)
        {
            MSS_UART_polled_tx_string(g_uart,
                    (const uint8_t*)"We are leaving the boot loader\r\n");
            MSS_UART_polled_tx_string(g_uart,
                    (const uint8_t*)"to run in loaded DDR program\r\n");
            jump_to_application(mem_area);
        }
    }
    /* never return */
}

/* HART1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    count_sw_ints_h1++;
}
