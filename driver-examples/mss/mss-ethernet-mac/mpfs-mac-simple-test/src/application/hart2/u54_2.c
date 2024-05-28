/***********************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54 second hart
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#if ((MPFS_HAL_FIRST_HART == 2) && (MPFS_HAL_LAST_HART == 2))
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_registers.h"
#include "platform_config_hw/drivers/mss_ethernet_mac/mss_ethernet_mac_sw_cfg.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_regs.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac.h"
#include "inc/common.h"

#if !(MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_GMII_LOCAL)

volatile uint32_t count_sw_ints_h2 = 0U;

/* Main function for the hart2(U54_2 processor).
 * Application code running on hart4 is placed here
 *
 * The hart2 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void
u54_2(void)
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

    MSS_UART_init(&g_mss_uart2_lo, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY);

    MSS_UART_polled_tx_string(&g_mss_uart2_lo, "Hello World from U54_2\r\n");

    SYSREG->FAB_INTEN_MISC = FAB_INTEN_MAC0_U54_2_EN_MASK;

    while (1U)
    {
        icount++;
        mac_task(0);
        if (0x100000U == icount)
        {
            icount = 0U;
            sprintf(info_string, "hart %d\r\n", hartid);
            MSS_UART_polled_tx(&g_mss_uart2_lo, info_string, strlen(info_string));
        }
    }

    /* Never return */
}

/* hart2 software interrupt handler */
void
U54_2_software_IRQHandler(void)
{
    count_sw_ints_h2++;
}

extern volatile uint64_t g_tick_counter;
/*==============================================================================
 *
 */
void
U54_2_sysTick_IRQHandler(void)
{
    g_tick_counter += HART2_TICK_RATE_MS;
}

#endif
#endif /* ((MPFS_HAL_FIRST_HART == 2) && (MPFS_HAL_LAST_HART ==2)) */
