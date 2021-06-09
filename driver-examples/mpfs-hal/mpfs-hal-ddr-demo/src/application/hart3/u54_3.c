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
 * Code running on U54 hart 3
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

volatile uint32_t count_sw_ints_h3 = 0U;

extern uint64_t uart_lock;
extern MEM_TYPE mem_area;
extern uint64_t hart_jump_ddr;
extern mss_uart_instance_t *g_uart;

/* Main function for the HART3(U54_3 processor).
 * Application code running on HART3 is placed here
 *
 * The HART3 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_3(void)
{
    char info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /*put this hart into WFI.*/
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
                    "Hart %lu, use option 6 and 8 to jump to DDR program\r\n",\
                        hartid);
            mss_take_mutex((uint64_t)&uart_lock);
            MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
            mss_release_mutex((uint64_t)&uart_lock);
        }
        if(hart_jump_ddr == 3U)
        {
            mss_take_mutex((uint64_t)&uart_lock);
            MSS_UART_polled_tx_string(g_uart,
                    (const uint8_t*)"We are leaving the boot loader\r\n");
            MSS_UART_polled_tx_string(g_uart,
                    (const uint8_t*)"to run in loaded DDR program\r\n");
            mss_release_mutex((uint64_t)&uart_lock);
            jump_to_application(mem_area);
        }
    }
    /* never return */
}

/* HART3 Software interrupt handler */
void Software_h3_IRQHandler(void)
{
    count_sw_ints_h3++;
}
