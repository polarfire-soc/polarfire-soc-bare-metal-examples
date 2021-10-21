/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 * 
 * PolarFire SoC Microprocessor Subsystem MMUART bare metal software driver
 * implementation.
 *
 */
#include <string.h>
#include <stdio.h>
#include "mpfs_hal/mss_hal.h"
#include "mss_core_ihc_regs.h"
#include "mss_core_ihc.h"
#include "inc/common.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

#ifdef __cplusplus
extern "C" {
#endif


IHC_TypeDef           * const IHC_HO              = (IHC_TypeDef *) IHC_HO_BASE;
IHC_TypeDef           * const IHC_H1              = (IHC_TypeDef *) IHC_H1_BASE;
IHC_TypeDef           * const IHC_H2              = (IHC_TypeDef *) IHC_H2_BASE;
IHC_TypeDef           * const IHC_H3              = (IHC_TypeDef *) IHC_H3_BASE;
IHC_TypeDef           * const IHC_H4              = (IHC_TypeDef *) IHC_H4_BASE;


/*const*/  IHC_TypeDef * IHC[]={IHC_HO,IHC_H1,IHC_H2,IHC_H3,IHC_H4};


/*******************************************************************************
 * Local functions.
 */
static uint32_t copy_message_sm(uint32_t remote_hart_id, uint32_t my_hart_id);

/*******************************************************************************
 * Public Functions
 *******************************************************************************/

uint32_t tx_message(uint32_t remote_hart_id, uint32_t my_hart_id, uint8_t *message)
{
    int8_t ret_value;

    /*
     * return if RMP bit 1 indicating busy
     */
    if (RMP_MESSAGE_PRESENT == IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG & RMP_MASK)
    {
        ret_value = MP_BUSY;
    }
    else
    {
        /*
         * Fill the buffer
         */
        memcpy((uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].mesg_out[0U] , message, IHC_MESSAGE_SIZE);
        /*
         * set the MP bit. This will notify other of incoming hart message
         */
        IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG |= RMP_MESSAGE_PRESENT;
        /*
         * report status
         */
        ret_value = MESSAGE_SENT;
    }

    return (ret_value);
}


uint32_t rx_message(uint32_t remote_hart_id, uint32_t my_hart_id, QUEUE_IHC_INCOMING handle_incoming)
{
    int8_t ret_value;

    /*
     * check if we have a message
     */
    if (MP_MESSAGE_PRESENT == (IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG & MP_MASK))
    {
       // if (0U == queue_incoming(remote_hart_id, my_hart_id, (uint8_t *)&IHC[my_hart_id]->HART_IHC->mesg_in[0U] ))
        handle_incoming(remote_hart_id, my_hart_id, (uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].mesg_in[0U] );
        {
            /*
             * set MP to 0
             * Note this generates an interrupt on the other hart if it has RMPIE
             * bit set in the control register
             */
            IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG &= ~MP_MASK;
            ret_value = MESSAGE_RX;
        }
    }
    else
    {
        /*
         * report status
         */
        ret_value = NO_MESSAGE_RX;
    }

    return (ret_value);
}

__attribute__((weak))  uint32_t queue_incoming( uint32_t remote_hart_id, uint32_t my_hart_id, uint8_t * message )
{
#ifdef DEBUG_IHC
    int8_t info_string[100];

    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;
#endif
    sprintf(info_string, "\r\nHart %u rx a message, Remote Hart %u,version address 0x%lx\r\n",\
                            my_hart_id, remote_hart_id, (uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].version);
    spinlock(&hart_share->mutex_uart0);
    MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
    spinunlock(&hart_share->mutex_uart0);
#endif
    return(0U);
}

#ifdef DEBUG_IHC
void test_state_machine(void)
{
    int8_t info_string[100];
    uint32_t remote_hart_id = 1;
    uint32_t my_hart_id = 0;
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;
#endif

    while(my_hart_id < 5U)
    {
        remote_hart_id = 0U;
        while(remote_hart_id < 5U)
        {
            if(remote_hart_id == my_hart_id)
            {
                /* skip */
            }
            else
            {
                copy_message_sm(remote_hart_id, my_hart_id);
            }
            remote_hart_id++;
        }
        my_hart_id++;
    }

    return;
}
#endif /* #ifdef DEBUG_IHC */

static uint32_t copy_message_sm(uint32_t remote_hart_id, uint32_t my_hart_id)//IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG &= ~MP_MASK;
{
    int8_t ret_value;
    static uint8_t state[5U] = { 0U, 0U, 0U, 0U, 0U };

    switch(state[my_hart_id])
    {
        case 0:
            /*
             * Copy message and set MP bit if new message to be sent
             */
            if ((IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG & RMP_MESSAGE_PRESENT)&&\
                    ((IHC[remote_hart_id]->HART_IHC[my_hart_id].CTR_REG.CTL_REG & MP_MESSAGE_PRESENT) == 0U))
            {
                memcpy((uint8_t *)&IHC[remote_hart_id]->HART_IHC[my_hart_id].mesg_in[0U],(uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].mesg_out[0U] , IHC_MESSAGE_SIZE);
                IHC[remote_hart_id]->HART_IHC[my_hart_id].CTR_REG.CTL_REG |= MP_MESSAGE_PRESENT;
                state[my_hart_id] = 1U;
            }
            break;
        case 1:
            /*
              * Clear RMP bit.
              */
             if (((IHC[remote_hart_id]->HART_IHC[my_hart_id].CTR_REG.CTL_REG & MP_MESSAGE_PRESENT)==0U)&&\
                     ((IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG & RMP_MESSAGE_PRESENT) == RMP_MESSAGE_PRESENT))
             {
                 IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG &= ~RMP_MESSAGE_PRESENT;
                 state[my_hart_id] = 0U;
             }
             break;
    }

    return (ret_value);
}

uint32_t print_mem_map_core_inc(void)
{
    int8_t info_string[100];
    uint32_t remote_hart_id = 1;
    uint32_t my_hart_id = 0;
    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
#ifdef  MPFS_HAL_SHARED_MEM_ENABLED
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;
#endif

    while(my_hart_id < 5U)
    {
        sprintf(info_string, "\r\n************** My Hart %u ************\r\n",\
                                my_hart_id);
        spinlock(&hart_share->mutex_uart0);
        MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
        spinunlock(&hart_share->mutex_uart0);
        remote_hart_id = 0U;
        while(remote_hart_id < 5U)
        {
            if(remote_hart_id == my_hart_id)
            {
                /* skip */
            }
            else
            {
                /*
                 * Init
                 */
#ifdef DEBUG_IHC
                IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG.CTL_REG = 0U;
#endif

                sprintf(info_string, "\r\nHart %u, Remote Hart %u,version address 0x%lx\r\n",\
                        my_hart_id, remote_hart_id, (uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].version);
                spinlock(&hart_share->mutex_uart0);
                MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
                spinunlock(&hart_share->mutex_uart0);

                sprintf(info_string, "\r\nHart %u, Remote Hart %u,CTR_REG address 0x%lx\r\n",\
                        my_hart_id, remote_hart_id, (uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].CTR_REG);
                spinlock(&hart_share->mutex_uart0);
                MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
                spinunlock(&hart_share->mutex_uart0);

                sprintf(info_string, "\r\nHart %u, Remote Hart %u,mesg_in address 0x%lx\r\n",\
                        my_hart_id, remote_hart_id, (uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].mesg_in);
                spinlock(&hart_share->mutex_uart0);
                MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
                spinunlock(&hart_share->mutex_uart0);

                sprintf(info_string, "\r\nHart %u, Remote Hart %u,mesg out address 0x%lx\r\n",\
                        my_hart_id, remote_hart_id, (uint8_t *)&IHC[my_hart_id]->HART_IHC[remote_hart_id].mesg_out);
                spinlock(&hart_share->mutex_uart0);
                MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
                spinunlock(&hart_share->mutex_uart0);
            }
            remote_hart_id++;
        }
        sprintf(info_string, "\r\nHart %u, Remote Hart %u,interrupt_concentrator address 0x%lx\r\n",\
                my_hart_id, remote_hart_id, (uint8_t *)&IHC[my_hart_id]->interrupt_concentrator);
        spinlock(&hart_share->mutex_uart0);
        MSS_UART_polled_tx(hart_share->g_mss_uart0_lo, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
        spinunlock(&hart_share->mutex_uart0);
        my_hart_id++;
    }

    return(0U);
}


/***************************************************************************//**
 * Interrupt service routine triggered by any MSS UART interrupt. This routine
 * will call the handler function appropriate to the interrupt from the
 * handlers previously registered with the driver through calls to the
 * MSS_UART_set_*_handler() functions, or it will call the default_tx_handler()
 * function in response to transmit interrupts if MSS_UART_irq_tx() is used to
 * transmit data.
 */
static void
message_present_isr
(
    void
)
{
    uint8_t iirf;

    iirf = this_uart->hw_reg->IIR & IIRF_MASK;

    switch (iirf)
    {
        case IIRF_MODEM_STATUS:  /* Modem status interrupt */
        {
            ASSERT(NULL_HANDLER != this_uart->modemsts_handler);
            if (NULL_HANDLER != this_uart->modemsts_handler)
            {
               (*(this_uart->modemsts_handler))(this_uart);
            }
        }
        break;

        case IIRF_THRE: /* Transmitter Holding Register Empty */
        {
            ASSERT(NULL_HANDLER != this_uart->tx_handler);
            if (NULL_HANDLER != this_uart->tx_handler)
            {
                (*(this_uart->tx_handler))(this_uart);
            }
        }
        break;

        case IIRF_RX_DATA:      /* Received Data Available */
        case IIRF_DATA_TIMEOUT: /* Received Data Timed-out */
        {
            ASSERT(NULL_HANDLER != this_uart->rx_handler);
            if (NULL_HANDLER != this_uart->rx_handler)
            {
                (*(this_uart->rx_handler))(this_uart);
            }
        }
        break;

        case IIRF_RX_LINE_STATUS:  /* Line Status Interrupt */
        {
            ASSERT(NULL_HANDLER != this_uart->linests_handler);
            if (NULL_HANDLER != this_uart->linests_handler)
            {
               (*(this_uart->linests_handler))(this_uart);
            }
        }
        break;

        case IIRF_MMI:
        {
            /* Identify multi-mode interrupts and handle */

            /* Receiver time-out interrupt */
            if (this_uart->hw_reg->IIM & ERTOI_MASK)
            {
                ASSERT(NULL_HANDLER != this_uart->rto_handler);

                if (NULL_HANDLER != this_uart->rto_handler)
                {
                    (*(this_uart->rto_handler))(this_uart);
                }
            }

            /* NACK interrupt */
            if (this_uart->hw_reg->IIM &ENACKI)
            {
                ASSERT(NULL_HANDLER != this_uart->nack_handler);

                if (NULL_HANDLER != this_uart->nack_handler)
                {
                    (*(this_uart->nack_handler))(this_uart);
                }
            }

            /* PID parity error interrupt */
            if (this_uart->hw_reg->IIM & EPID_PEI)
            {
                ASSERT(NULL_HANDLER != this_uart->pid_pei_handler);

                if (NULL_HANDLER != this_uart->pid_pei_handler)
                {
                    (*(this_uart->pid_pei_handler))(this_uart);
                }
            }

            /* LIN break detection interrupt */
            if (this_uart->hw_reg->IIM & ELINBI)
            {
                ASSERT(NULL_HANDLER != this_uart->break_handler);

                if (NULL_HANDLER != this_uart->break_handler)
                {
                    (*(this_uart->break_handler))(this_uart);
                }
            }

            /* LIN Sync detection interrupt */
            if (this_uart->hw_reg->IIM & ELINSI)
            {
                ASSERT(NULL_HANDLER != this_uart->sync_handler);

                if (NULL_HANDLER != this_uart->sync_handler)
                {
                    (*(this_uart->sync_handler))(this_uart);
                }
            }
            break;
        }
        default:
        {
            ASSERT(INVALID_INTERRUPT); /* Comment to avoid LDRA warning */
        }
        break;
    }
}

#ifdef __cplusplus
}
#endif
