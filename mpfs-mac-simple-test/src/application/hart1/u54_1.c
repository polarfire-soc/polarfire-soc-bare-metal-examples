/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#if ((MPFS_HAL_FIRST_HART == 1) && (MPFS_HAL_LAST_HART == 1))
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/common.h"

volatile uint32_t count_sw_ints_h1 = 0U;

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void
u54_1(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart in WFI. */
    do
    {
        __asm("wfi");
    } while (0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();

    MSS_UART_init(&g_mss_uart1_lo, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY);

    MSS_UART_polled_tx_string(&g_mss_uart1_lo, "Hello World from U54_1\r\n");

    /*
     * Enable mac local interrupts to hart 1, U54 1
     */
    SYSREG->FAB_INTEN_MISC = FAB_INTEN_MAC0_U54_1_EN_MASK;
    // SysTick_Config();  /* Let hart 0 run the timer */

    // CLINT->MSIP[2] = 1; /* Kick start hart 2 */
    __enable_irq();

    while (1U)
    {
        icount++;
        mac_task(0);

        if (0x100000U == icount)
        {
            icount = 0U;
            sprintf(info_string, "hart %d\r\n", hartid);
            MSS_UART_polled_tx(&g_mss_uart1_lo, info_string, strlen(info_string));
        }
    }

    /* Never return */
}

/* hart1 software interrupt handler */
void
U54_1_software_IRQHandler(void)
{
    count_sw_ints_h1++;
}

extern volatile uint64_t g_tick_counter;
/*==============================================================================
 *
 */
void
U54_1_sysTick_IRQHandler(void)
{
    g_tick_counter += HART1_TICK_RATE_MS;
}
#endif /* ((MPFS_HAL_FIRST_HART == 1) && (MPFS_HAL_LAST_HART ==1)) */
