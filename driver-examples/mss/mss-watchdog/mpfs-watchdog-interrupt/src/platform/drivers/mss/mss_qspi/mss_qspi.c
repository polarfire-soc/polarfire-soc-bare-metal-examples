/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_qspi.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) QSPI bare metal software
 * driver implementation.
 *
 */

#include "mpfs_hal/mss_hal.h"
#include "mss_qspi.h"

#ifdef __cplusplus
extern "C" {
#endif
static void default_status_hanlder(uint32_t value);
static volatile uint32_t g_irq_rd_byte_size = 0u;
static void * g_rd_buffer;
static volatile mss_qspi_status_handler_t g_handler;

/***************************************************************************//**
 * See mss_qspi.h for details of how to use this function.
 */
void MSS_QSPI_init
(
    void
)
{
    g_handler = default_status_hanlder;

    QSPI->CONTROL = CTRL_EN_MASK |
                    CTRL_SAMPLE_SCK |
                    (0x1u << CTRL_CLKRATE) | CTRL_CLKIDL_MASK ;

    QSPI->INTENABLE = 0x0u;
}

/***************************************************************************//**
 * See mss_qspi.h for details of how to use this function.
 */
void MSS_QSPI_configure
(
    const mss_qspi_config_t* config
)
{
    QSPI->CONTROL = (uint32_t)(config->sample << CTRL_SAMPLE)    |
                    (uint32_t)(config->io_format << CTRL_QMODE0) |
                    (uint32_t)(config->clk_div << CTRL_CLKRATE)  |
                    (uint32_t)(config->xip << CTRL_XIP)          |
                    (uint32_t)(config->xip_addr << CTRL_XIPADDR) |
                    (uint32_t)(config->spi_mode << CTRL_CLKIDL)  |
                    CTRL_EN_MASK;
}

/***************************************************************************//**
 * See mss_qspi.h for details of how to use this function.
 */
void MSS_QSPI_get_config
(
    mss_qspi_config_t* config
)
{
   volatile uint32_t reg =0;

   reg = QSPI->CONTROL;

   config->spi_mode = ((reg & CTRL_CLKIDL_MASK) >> CTRL_CLKIDL);

   config->clk_div = (mss_qspi_clk_div)((reg & CTRL_CLKRATE_MASK)
                                                               >> CTRL_CLKRATE);
   config->xip = (uint8_t)((reg & CTRL_XIP_MASK) >> CTRL_XIP);
   config->xip_addr = (uint8_t)((reg & CTRL_XIPADDR_MASK) >> CTRL_XIPADDR);
   config->sample = (uint8_t)((reg & CTRL_SAMPLE_MASK) >> CTRL_SAMPLE);

   reg = reg & (uint32_t )((uint32_t )CTRL_QMODE12_MASK | (uint32_t )CTRL_QMODE0_MASK);
   reg = reg >> CTRL_QMODE0;

   config->io_format = (mss_qspi_io_format)reg;
}

/***************************************************************************//**
 * See mss_qspi.h for details of how to use this function.
 */
void MSS_QSPI_polled_transfer_block
(
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
)
{
    uint32_t idx;
    uint8_t* buf8 = (uint8_t*)tx_buffer;
    uint32_t* buf32 = (uint32_t*)tx_buffer;
    volatile uint32_t skips;
    uint32_t cbytes;
    uint32_t total_byte_cnt;
    uint32_t words = 0u;

    cbytes = 1u + tx_byte_size + num_addr_bytes;
    total_byte_cnt = 1u + tx_byte_size + num_addr_bytes + rd_byte_size;

    QSPI->INTENABLE = 0u;

    while ((QSPI->STATUS & STTS_READY_MASK) == 0u){};

    /*bit16 to 31 define the number of Upper bytes when count is >65535
    Write to lower 16 bit is ignored*/
    QSPI->FRAMESUP = total_byte_cnt & FRMS_UBYTES_MASK;

    num_idle_cycles = (num_idle_cycles << 3u);

    skips  = (total_byte_cnt & 0x0000FFFFu);
    skips |= (cbytes << FRMS_CBYTES);
    skips |= (((QSPI->CONTROL & CTRL_QMODE12_MASK)? 1u:0u) << FRMS_QSPI);
    skips |= ((uint32_t)num_idle_cycles) << 23u;
    skips |= FRMS_FWORD_MASK;

    QSPI->FRAMES = (uint32_t)skips;

    QSPI->CONTROL |= CTRL_FLAGSX4_MASK;

    words = cbytes / (uint32_t)4u;

    for (idx = 0u; idx < words; ++idx)
    {
        while (QSPI->STATUS & STTS_TFFULL_MASK){};

        QSPI->TXDATAX4 = (uint32_t)buf32[idx];
    }

    QSPI->CONTROL &= ~CTRL_FLAGSX4_MASK;

    /*
     * This delay is needed only for the EXT_RO and EXT_RW modes
     * with out this delay the TXDATAX1 FIFO is not getting updated
     * with proper data.
     * Ex: if the bytes to send are 267 then the last 3 bytes that
     * are getting written into the TXDATAX1 FIFO are not properly
     * written.
     * Where as this delay is not needed for other modes like NORMAL,
     * FULL_QUAD/FULL_DUAL.
     */
    sleep_ms(10);
    for (idx = (cbytes - (cbytes % 4u)); idx < cbytes; ++idx)
    {
        while (QSPI->STATUS & STTS_TFFULL_MASK){};

        QSPI->TXDATAX1 = (uint8_t)buf8[idx];
    }

    buf32 = (uint32_t*)rd_buffer;
    buf8 = (uint8_t*)rd_buffer;

    if (rd_byte_size)
    {
        words = rd_byte_size / 4u;

        QSPI->CONTROL |= CTRL_FLAGSX4_MASK;

        for (idx = 0u; idx < words; ++idx)
        {
            while (QSPI->STATUS & STTS_RFEMPTY_MASK){};
            buf32[idx] = QSPI->RXDATAX4;
        }

        QSPI->CONTROL &= ~CTRL_FLAGSX4_MASK;

        for (idx = (rd_byte_size - (rd_byte_size % 4u));
                                                      idx < rd_byte_size; ++idx)
        {
            while (QSPI->STATUS & STTS_RFEMPTY_MASK){};
            buf8[idx] = QSPI->RXDATAX1;
        }

        while (0u == (QSPI->STATUS & STTS_RDONE_MASK))
        {
            skips = (uint64_t)((QSPI->STATUS & STTS_FLAGSX4_MASK) ?
                                               QSPI->RXDATAX4 : QSPI->RXDATAX1);
        }
     }
}

/***************************************************************************//**
 * See mss_qspi.h for details of how to use this function.
 */
uint8_t MSS_QSPI_irq_transfer_block
(
    uint8_t num_addr_bytes,
    const void * const tx_buffer,
    uint32_t tx_byte_size,
    const void * const rd_buffer,
    uint32_t rd_byte_size,
    uint8_t num_idle_cycles
)
{
    uint32_t idx;
    uint32_t cbytes;
    uint32_t total_byte_cnt;
    const uint8_t* buf8 = tx_buffer;
    const uint32_t* buf32 = tx_buffer;
    volatile uint32_t skips = 0;
    uint8_t returnval = 0u;
    uint32_t words = 0u;
    uint32_t enable = 0u;

    g_rd_buffer = (uint32_t*)rd_buffer;
    cbytes = 1u + tx_byte_size + num_addr_bytes;
    total_byte_cnt = 1u + tx_byte_size + num_addr_bytes + rd_byte_size;
    while ((QSPI->STATUS & STTS_READY_MASK) == 0u){};

    enable = INTE_TDONE_MASK;
    /*bit16 to 31 define the number of Upper bytes when count is >65535
     Write to lower 16 bit is ignored*/
    QSPI->FRAMESUP = total_byte_cnt & FRMS_UBYTES_MASK;
    skips  = (total_byte_cnt & 0x0000FFFFu);
    skips |= (cbytes << FRMS_CBYTES);
    skips |= (((QSPI->CONTROL & CTRL_QMODE12_MASK)? 1u:0u) << FRMS_QSPI);
    skips |= ((uint32_t)num_idle_cycles) << 26u;

    if (rd_buffer)
    {
        skips |= FRMS_FBYTE_MASK;
        QSPI->FRAMES = skips;
        QSPI->CONTROL &= ~CTRL_FLAGSX4_MASK;
        for (idx = 0u; idx < cbytes; ++idx)
        {
             while (QSPI->STATUS & STTS_TFFULL_MASK){};
             QSPI->TXDATAX1 = (uint8_t)buf8[idx];
        }

    }
    else
    {
        skips |= FRMS_FWORD_MASK;
        QSPI->FRAMES = skips;
        QSPI->CONTROL |= CTRL_FLAGSX4_MASK;
        words = cbytes / (uint32_t)4u;
        for (idx = 0u; idx < words; ++idx)
        {
            while (QSPI->STATUS & STTS_TFFULL_MASK){};
            QSPI->TXDATAX4 = (uint32_t)buf32[idx];
        }
        QSPI->CONTROL &= ~CTRL_FLAGSX4_MASK;

        /*
         * This delay is needed only for the EXT_RO and EXT_RW modes
         * with out this delay the TXDATAX1 FIFO is not getting updated
         * with proper data.
         * Ex: if the bytes to send are 267 then the last 3 bytes that
         * are getting written into the TXDATAX1 FIFO are not properly
         * written.
         * Where as this delay is not needed for other modes like SPI,
         * FULL_QUAD/FULL_DUAL.
         */
        sleep_ms(10);
        for (idx = (cbytes - (cbytes % 4u)); idx < cbytes; ++idx)
        {
            while (QSPI->STATUS & STTS_TFFULL_MASK){};
            QSPI->TXDATAX1 = (uint8_t)buf8[idx];
        }
    }

    if (rd_byte_size)
    {
        g_irq_rd_byte_size = rd_byte_size;
        enable |= (uint32_t )(INTE_RDONE_MASK | INTE_RAVLB_MASK);
    }

    QSPI->INTENABLE = enable;

    /*
     * This delay is needed to get proper data into the RXFIFOs
     * With out this delay, once the RX_AVAILABLE bit is set
     * the data available in RXFIFO is incorrect.
     */
    if (rd_byte_size != 0)
       sleep_ms(10);

    return 0;
}

/***************************************************************************//**
 * See mss_qspi.h for details of how to use this function.
 */
void MSS_QSPI_set_status_handler
(
   mss_qspi_status_handler_t handler
)
{
    if ((mss_qspi_status_handler_t)0 != handler)
    {
        g_handler = handler;
    }
}

static void qspi_isr(void)
{
    uint32_t idx;
    static uint32_t empty = 0u;
    static uint32_t tx_fifo_full = 0u;
    uint32_t status = 0;
    uint32_t skips = 0;

    status = QSPI->STATUS;

    if (STTS_TDONE_MASK == (uint32_t)(status & STTS_TDONE_MASK))
    {
        g_handler(STTS_TDONE_MASK);
        QSPI->STATUS |= STTS_TDONE_MASK;
    }

    if (STTS_RAVLB_MASK == (uint32_t)(status & STTS_RAVLB_MASK))
    {
        QSPI->STATUS |= STTS_RAVLB_MASK;
        uint8_t* buf8 = g_rd_buffer;
        uint32_t* buf32 = g_rd_buffer;
        uint32_t words = 0u;

        if (g_irq_rd_byte_size >= 4)
        {
           words = g_irq_rd_byte_size / 4u;
           QSPI->CONTROL |= CTRL_FLAGSX4_MASK;

           for (idx = 0u; idx < words; ++idx)
           {
               while (QSPI->STATUS & STTS_RFEMPTY_MASK){};
               buf32[idx] = QSPI->RXDATAX4;
           }
        }

        QSPI->CONTROL &= ~CTRL_FLAGSX4_MASK;
        for (idx = (g_irq_rd_byte_size - (g_irq_rd_byte_size % 4u));
                                        idx < g_irq_rd_byte_size; ++idx)
        {
           while (QSPI->STATUS & STTS_RFEMPTY_MASK){};
           buf8[idx] = QSPI->RXDATAX1;
        }
    }

    if (STTS_RDONE_MASK == (uint32_t)(status & STTS_RDONE_MASK))
    {
        /*This means receive transfer is now complete. invoke the callback
         * function*/
        g_handler(STTS_RDONE_MASK);
        /*disable RXDONE, RXEMPTY, RXAVLBL interrupt*/
        QSPI->INTENABLE &= ~(INTE_RDONE_MASK | INTE_RAVLB_MASK);
        QSPI->STATUS |= STTS_RDONE_MASK;
    }
}

static void default_status_hanlder(uint32_t value)
{
    /*Take some default interrupt handling action here*/
}

/*QSPI interrupt handler function*/
uint8_t qspi_plic_IRQHandler(void)
{
    qspi_isr();
    return (uint8_t)EXT_IRQ_KEEP_ENABLED;
}

#ifdef __cplusplus
}
#endif
