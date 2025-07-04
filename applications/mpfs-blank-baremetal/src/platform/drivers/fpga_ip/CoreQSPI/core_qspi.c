/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_qspi.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreQSPI driver implementation. Please refer to core_qspi.h file for
 * description of functions implemented in this file.
 *
 */

#include "core_qspi_regs.h"
#include "core_qspi.h"
#include "hal/hal.h"
#include "hal/hal_assert.h"

#define NULL_QSPI_INSTANCE ((qspi_instance_t *)0)
#define NULL_BUFFER ((uint8_t*)0)

static void default_status_hanlder(uint32_t value);
static volatile uint32_t g_irq_rd_byte_size = 0u;
static void * g_rd_buffer;
static volatile qspi_status_handler_t g_handler;

/***************************************************************************//**
 * QSPI_init()
 * See "core_qspi.h" for details of how to use this function.
 */
void
QSPI_init
(
    qspi_instance_t *this_qspi,
    addr_t addr
)
{
    uint32_t reg_read = 0u;
    g_handler = default_status_hanlder;

    HAL_ASSERT(this_qspi != NULL_QSPI_INSTANCE);

    this_qspi->base_address = addr;

    reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);
    HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
                                            ((reg_read) | CTRL_ENABLE_MASK));

    /* Enable the core, core samples SDI in master mode, sets SPI clock rate,
     * allows spi memory operations in mode 0 or mode 3.
     */
    HAL_set_32bit_reg_field(addr, CTRL_ENABLE, 1u);
    HAL_set_32bit_reg_field(addr, CTRL_SAMPLE, 0u);
    HAL_set_32bit_reg_field(addr, CTRL_CLKRATE, 1u);
    HAL_set_32bit_reg_field(addr, CTRL_CLKIDLE, 1u);

    /* Interrupt enable register -> Enables the external interrupt for the
     * corresponding interrupt*/
    HAL_set_32bit_reg(addr, INT_ENABLE, 0u);
}

/***************************************************************************//**
 * QSPI_configure()
 * See core_qspi.h for details of how to use this function.
 */
void
QSPI_configure
(
    qspi_instance_t *this_qspi,
    const qspi_config_t* config
)
{
    /* Configure QSPI. Called from micron_mt25q.c flash_init() */
    uint32_t temp_reg_read = 0u;

    temp_reg_read = (uint32_t)((config->sample) << CTRL_SAMPLE_SHIFT)    |
                    (uint32_t)((config->io_format) << CTRL_QMODE0_SHIFT) |
                    (uint32_t)((config->clk_div) << CTRL_CLKRATE_SHIFT)  |
                    (uint32_t)((config->xip) << CTRL_XIP_SHIFT)          |
                    (uint32_t)((config->xip_addr) << CTRL_XIPADDR_SHIFT) |
                    (uint32_t)((config->spi_mode) << CTRL_CLKIDLE_SHIFT) |
                    CTRL_ENABLE_MASK;

    HAL_set_32bit_reg(this_qspi->base_address, CONTROL, temp_reg_read);
}

/***************************************************************************//**
 * QSPI_get_config()
 * See core_qspi.h for details of how to use this function.
 */
void QSPI_get_config
(
    qspi_instance_t *this_qspi,
    qspi_config_t* config
)
{
    volatile uint32_t reg =0;

    reg = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);

    config->spi_mode = ((reg & CTRL_CLKIDLE_MASK) >> CTRL_CLKIDLE_SHIFT);

    reg = reg & (uint32_t )((uint32_t )CTRL_QMODE12_MASK
                          | (uint32_t )CTRL_QMODE0_MASK);

    reg = reg >> CTRL_QMODE0_SHIFT;

    config->io_format = (qspi_io_format)reg;
    config->clk_div = (qspi_clk_div)((reg & CTRL_CLKRATE_MASK)
            >> CTRL_CLKRATE_SHIFT);

    config->xip = (uint8_t)((reg & CTRL_XIP_MASK) >> CTRL_XIP_SHIFT);
    config->xip_addr = (uint8_t)((reg & CTRL_XIPADDR_MASK) >> CTRL_XIPADDR_SHIFT);
    config->sample = (uint8_t)((reg & CTRL_SAMPLE_MASK) >> CTRL_SAMPLE_SHIFT);
}

/***************************************************************************//**
 * QSPI_polled_transfer_block()
 * See mss_qspi.h for details of how to use this function.
 */
void
QSPI_polled_transfer_block
(
    qspi_instance_t *this_qspi,
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
)
{
    uint32_t idx;
    uint32_t reg_read = 0;
    uint8_t* buf8 = (uint8_t*)tx_buffer;
    uint32_t* buf32 = (uint32_t*)tx_buffer;
    volatile uint32_t skips;
    uint32_t cbytes;
    uint32_t total_byte_cnt;
    uint32_t words = 0u;
    uint32_t total_bytes_tx = 0u;

    HAL_ASSERT(tx_byte_size <= 0xFFFFu);
    HAL_ASSERT(num_addr_bytes <= 4u);
    HAL_ASSERT(num_idle_cycles <= 15u);

    cbytes = 1u + num_addr_bytes;
    total_bytes_tx = 1u + tx_byte_size + num_addr_bytes;
    total_byte_cnt = 1u + tx_byte_size + num_addr_bytes + rd_byte_size;

    HAL_set_32bit_reg(this_qspi->base_address, INT_ENABLE, 0u);

    do{
        reg_read = HAL_get_32bit_reg(this_qspi->base_address, STATUS);
    } while(((reg_read) & STATUS_READY_MASK) == 0u);

    /* bit16 to 31 define the number of Upper bytes when count is >65535
           Write to lower 16 bit is ignored */
    HAL_set_32bit_reg(this_qspi->base_address, FRAMESUP,
            total_byte_cnt & FRAMESUP_BYTESUPPER_MASK);

    if ((tx_byte_size <= 0xFFFFu) &&
            (num_addr_bytes <= 4u) &&
            (num_idle_cycles <= 15u))
    {
        /* Calculating skip bits */
        skips  = (total_byte_cnt & FRAMES_TOTALBYTES_MASK);
        skips |= (cbytes << FRAMES_COMMANDBYTES_SHIFT);

        reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);
        skips |= (((reg_read & CTRL_QMODE12_MASK) ? 1u : 0u) << FRAMES_QSPI_SHIFT);
        skips |= ((uint32_t)num_idle_cycles) << FRAMES_IDLE_SHIFT;
        skips |= FRAMES_FLAGWORD_MASK;

        /* Write the calculated skip bits into frames register */
        HAL_set_32bit_reg(this_qspi->base_address, FRAMES, (uint32_t)skips);
    }

    reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);
    HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
            ((reg_read) | CTRL_FLAGSX4_MASK));

    words = total_bytes_tx / (uint32_t)4u;

    /* Load the transmit data into transmitdatax4 data register */
    for (idx = 0u; idx < words; ++idx)
    {
        do{
            reg_read = HAL_get_32bit_reg(this_qspi->base_address, STATUS);
        } while((reg_read) & STATUS_TXFIFOFULL_MASK);

        HAL_set_32bit_reg(this_qspi->base_address, X4TRANSMIT_DATA, (uint32_t)buf32[idx]);
    }

    reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);
    HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
            ((reg_read) & ~(CTRL_FLAGSX4_MASK)));

    uint32_t l_idx = (total_bytes_tx - (total_bytes_tx % 4u));

    /* Load the transmit data into transmit data register */
    for (idx = l_idx; idx < total_bytes_tx; ++idx)
    {
        do{
            reg_read = HAL_get_32bit_reg(this_qspi->base_address, STATUS);
        } while((reg_read) & STATUS_TXFIFOFULL_MASK);

        HAL_set_32bit_reg(this_qspi->base_address, TRANSMIT_DATA, (uint8_t)buf8[idx]);
    }

    buf32 = (uint32_t*)rd_buffer;
    buf8 = (uint8_t*)rd_buffer;

    /* Receive data */
    if (rd_byte_size)
    {
        words = rd_byte_size / 4u;

        reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);
        HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
                ((reg_read) | CTRL_FLAGSX4_MASK));

        /* Load the TransmitX4 receive data into buf32 */
        for (idx = 0u; idx < words; ++idx)
        {
            do{
                reg_read = HAL_get_32bit_reg(this_qspi->base_address, STATUS);
            } while((reg_read) & STATUS_RXFIFOEMPTY_MASK);

            buf32[idx] = HAL_get_32bit_reg(this_qspi->base_address, X4RECEIVE_DATA);
        }

        reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);
        HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
                ((reg_read) & ~(CTRL_FLAGSX4_MASK)));

        l_idx = (rd_byte_size - (rd_byte_size % 4u));

        /* Load the transmit data register contents into buf8 */
        for (idx = l_idx; idx < rd_byte_size; ++idx)
        {
            do{
                reg_read = HAL_get_32bit_reg(this_qspi->base_address, STATUS);
            } while((reg_read) & STATUS_RXFIFOEMPTY_MASK);

            buf8[idx] = HAL_get_32bit_reg(this_qspi->base_address, RECEIVE_DATA);
        }

        /* Wait for RXDONE to complete and ignore the skip bits */
        do{
           skips = (uint64_t)(((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, STATUS))
                    & (uint32_t)(STATUS_FLAGSX4_MASK))
                    ? (uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, X4RECEIVE_DATA))
                    : (uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, RECEIVE_DATA)));
        } while (0u == (uint32_t)((HAL_get_32bit_reg(this_qspi->base_address, STATUS))
                     & STATUS_RXDONE_MASK));
    }
}

/***************************************************************************//**
 * QSPI_irq_transfer_block()
 * See mss_qspi.h for details of how to use this function.
 */
uint8_t
QSPI_irq_transfer_block
(
    qspi_instance_t *this_qspi,
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
)
{
    uint32_t idx;
    uint32_t reg_read = 0u;
    uint32_t cbytes;
    uint32_t total_byte_cnt;
    const uint8_t* buf8 = tx_buffer;
    const uint32_t* buf32 = tx_buffer;
    volatile uint32_t skips = 0;
    uint8_t returnval = 0u;
    uint32_t l_idx = 0u;
    uint32_t words = 0u;
    uint32_t enable = 0u;

    HAL_ASSERT(tx_byte_size <= 0xFFFFu);
    HAL_ASSERT(num_addr_bytes <= 4u);
    HAL_ASSERT(num_idle_cycles <= 15u);

    g_rd_buffer = (uint32_t*)rd_buffer;

    cbytes = 1u + tx_byte_size + num_addr_bytes;
    total_byte_cnt = 1u + tx_byte_size + num_addr_bytes + rd_byte_size;

    do{
        reg_read = HAL_get_32bit_reg(this_qspi->base_address, STATUS);
    } while(((reg_read) & STATUS_READY_MASK) == 0u);

    enable = INT_ENABLE_TXDONE_MASK;

    /* bit16 to 31 define the number of Upper bytes when count is >65535
     Write to lower 16 bit is ignored */
    HAL_set_32bit_reg(this_qspi->base_address, FRAMESUP,
            (total_byte_cnt & FRAMESUP_BYTESUPPER_MASK));

    if((tx_byte_size <= 0xFFFFu) &&
                (num_addr_bytes <= 4u) &&
                (num_idle_cycles <= 15u))
    {
    /* Calculating skip bits */
    skips  = (total_byte_cnt & FRAMES_TOTALBYTES_MASK);
    skips |= (cbytes << FRAMES_COMMANDBYTES_SHIFT);

    reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);
    skips |= (((reg_read & CTRL_QMODE12_MASK) ? 1u : 0u) << FRAMES_QSPI_SHIFT);

    skips |= ((uint32_t)num_idle_cycles) << FRAMES_IDLE_SHIFT;
    }

    skips |= FRAMES_FLAGWORD_MASK;
    HAL_set_32bit_reg(this_qspi->base_address, FRAMES, skips);
    reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);

    HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
                (reg_read | CTRL_FLAGSX4_MASK));
    words = cbytes / (uint32_t)4u;

    for (idx = 0u; idx < words; ++idx)
    {
        while ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, STATUS)) &
                                 (uint32_t)STATUS_TXFIFOFULL_MASK){};

        HAL_set_32bit_reg(this_qspi->base_address, X4TRANSMIT_DATA,
                                           (uint32_t)buf32[idx]);
    }

    reg_read = HAL_get_32bit_reg(this_qspi->base_address, CONTROL);

    HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
                (reg_read & (~CTRL_FLAGSX4_MASK)));

    sleep_ms(10);
    for (idx = (cbytes - (cbytes % 4u)); idx < cbytes; ++idx)
    {
        while ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, STATUS)) &
                                (uint32_t)STATUS_TXFIFOFULL_MASK){};
        HAL_set_32bit_reg(this_qspi->base_address, TRANSMIT_DATA,
                                          (uint8_t)buf8[idx]);
    }

    g_irq_rd_byte_size = rd_byte_size;
    enable |= (uint32_t )(INT_ENABLE_RXDONE_MASK | INT_ENABLE_RXAVAILABLE_MASK);


    HAL_set_32bit_reg(this_qspi->base_address, INT_ENABLE, enable);

    /*
     * This delay is needed to get proper data into the RXFIFOs
     * With out this delay, once the RX_AVAILABLE bit is set
     * the data available in RXFIFO is incorrect.
     */
    if (rd_byte_size != 0)
       sleep_ms(10);

    return (returnval);
}

/***************************************************************************//**
 * QSPI_set_status_handler()
 * See mss_qspi.h for details of how to use this function.
 */
void
QSPI_set_status_handler
(
    qspi_status_handler_t handler
)
{
    if ((qspi_status_handler_t)0 != handler)
    {
        g_handler = handler;
    }
}

void
qspi_isr
(
    qspi_instance_t *this_qspi
)
{
    uint32_t idx;
    static uint32_t empty = 0u;
    static uint32_t tx_fifo_full = 0u;
    uint32_t status;
    uint32_t l_idx = 0;

    status = HAL_get_32bit_reg(this_qspi->base_address, STATUS);

    if (STATUS_TXDONE_MASK == (uint32_t)(status & STATUS_TXDONE_MASK))
    {
       g_handler(STATUS_TXDONE_MASK);

       HAL_set_32bit_reg(this_qspi->base_address, STATUS,
                ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, STATUS))
                        | (uint32_t)STATUS_TXDONE_MASK));

    }

    if (STATUS_RXAVAILABLE_MASK == (uint32_t)(status & STATUS_RXAVAILABLE_MASK))
    {

       HAL_set_32bit_reg(this_qspi->base_address, STATUS,
                ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, STATUS))
                        | (uint32_t)STATUS_RXAVAILABLE_MASK));

       uint8_t* buf8 = g_rd_buffer;
       uint32_t* buf32 = g_rd_buffer;
       uint32_t words = 0u;

       words = g_irq_rd_byte_size / 4u;

       HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
               ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, CONTROL))
                            | (uint32_t)CTRL_FLAGSX4_MASK));

       for (idx = 0u; idx < words; ++idx)
       {
           while ((HAL_get_32bit_reg(this_qspi->base_address, STATUS)) & STATUS_RXFIFOEMPTY_MASK){};
           buf32[idx] = HAL_get_32bit_reg(this_qspi->base_address, X4RECEIVE_DATA);
       }

       HAL_set_32bit_reg(this_qspi->base_address, CONTROL,
               ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, CONTROL))
                            & ~(uint32_t)(CTRL_FLAGSX4_MASK)));

       l_idx = (g_irq_rd_byte_size - (g_irq_rd_byte_size % 4u));
       for (idx = l_idx; idx < g_irq_rd_byte_size; ++idx)
       {
           while ((HAL_get_32bit_reg(this_qspi->base_address, STATUS)) & STATUS_RXFIFOEMPTY_MASK){};
           buf8[idx] = HAL_get_32bit_reg(this_qspi->base_address, RECEIVE_DATA);
       }

       uint32_t skips = 0;

       while (0u == ((HAL_get_32bit_reg(this_qspi->base_address,
               STATUS)) & STATUS_RXFIFOEMPTY_MASK))
       {
           /* Make sure that the Receive FIFO is empty and any
              remaining data is read from it after desired bytes
              have been received. */
           skips = (uint64_t)(((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, STATUS))
                   & (uint32_t)STATUS_FLAGSX4_MASK)
                   ? (uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, X4RECEIVE_DATA))
                   : (uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, RECEIVE_DATA)));
       }
    }

    if (STATUS_RXDONE_MASK == (uint32_t)(status & STATUS_RXDONE_MASK))
    {

        /* This means receive transfer is now complete. invoke the callback
         * function */
        g_handler(STATUS_RXDONE_MASK);

        /* disable RXDONE, RXEMPTY, RXAVLBL interrupt */
        HAL_set_32bit_reg(this_qspi->base_address, INT_ENABLE,
                ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, INT_ENABLE))
                        & ~(uint32_t)(INT_ENABLE_RXDONE_MASK | INT_ENABLE_RXAVAILABLE_MASK)));

        HAL_set_32bit_reg(this_qspi->base_address, STATUS,
                ((uint32_t)(HAL_get_32bit_reg(this_qspi->base_address, STATUS))
                        | (uint32_t)STATUS_RXDONE_MASK));

    }
}

static void
default_status_hanlder
(
    uint32_t value
)
{
    /* Take some default interrupt handling action here */
}

#ifdef __cplusplus
}
#endif
