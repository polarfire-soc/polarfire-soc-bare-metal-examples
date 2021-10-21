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
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/fpga-ip/CoreIHC/mss_core_ihc.h"
#else
#include "drivers/FU540_uart/FU540_uart.h"
#endif

volatile uint32_t count_sw_ints_h1 = 0U;
uint8_t h1_incoming_message[IHC_MESSAGE_SIZE];

/*******************************************************************************
 * Local functions.
 */
static void test_protocl_ss(void);
static uint32_t queue_incoming_h1( uint32_t remote_hart_id, uint32_t my_hart_id, uint8_t * message );



/* Main function for the HART1(U54_1 processor).
 * Application code running on HART1 is placed here
 *
 * The HART1 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_1(void)
{
    uint8_t info_string[100];
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;
#endif

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart into WFI. */
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
       can enable and use any interrupts as required */
    clear_soft_interrupt();

    __enable_irq();

    spinlock(&hart_share->mutex_uart0);
    MSS_UART_polled_tx_string(hart_share->g_mss_uart0_lo,
            "Hello World from u54 core 1 - hart1 running from DDR\r\n");
    spinunlock(&hart_share->mutex_uart0);

    while (1U)
    {
        icount++;
        if (0x100000U == icount)
        {
            icount = 0U;
            // sprintf(info_string,"Hart %d\r\n", hartid);
            //  spinlock(&hart_share->mutex_uart0);
            //  MSS_UART_polled_tx(&g_mss_uart0_lo, info_string, strlen(info_string));
            // spinunlock(&hart_share->mutex_uart0);
            test_protocl_ss();
        }
    }
    /* never return */
}

/* HART1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}


static void test_protocl_ss(void)
{
#ifdef DEBUG_IHC
    int8_t info_string[100];
    int8_t remote_hart_id = 0U;

    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;
#endif
#endif

    if (MESSAGE_RX == rx_message(0, hls->my_hart_id, queue_incoming_h1))
    {
#ifdef DEBUG_IHC
        sprintf(info_string, "\r\nHart %u rx a message, Remote Hart %u,version address 0x%lx\r\n",\
                hls->my_hart_id, remote_hart_id, h1_incoming_message[0]);
        spinlock(&hart_share->mutex_uart0);
        MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
        spinunlock(&hart_share->mutex_uart0);
#endif
#if 0
        switch(hart1_cmd)
        {
            case 1:
                remote_hart_id = 1U;
                message[0] = 1U;
                tx_message(0, hls->my_hart_id, (uint8_t *)&message);
                break;
        }
        hart1_cmd = 0U;
#endif
    }

}


static uint32_t queue_incoming_h1( uint32_t remote_hart_id, uint32_t my_hart_id, uint8_t * message )
{
    memcpy(h1_incoming_message, message , IHC_MESSAGE_SIZE);

    return(0U);
}
