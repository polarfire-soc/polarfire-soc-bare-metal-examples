/***************************************************************************//**
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *
 * Driver for WINBOND W25N01 QSPI flash memory.
 * This driver uses the MPFS MSS QSPI driver interface.
 */

#ifndef MSS_WINBOND_W25N_H_
#define MSS_WINBOND_W25N_H_

#include <stddef.h>
#include <stdint.h>
#include "drivers/mss/mss_qspi/mss_qspi.h"

#ifdef __cplusplus
extern "C" {
#endif

extern mss_qspi_config_t qspi_config;
#define JEDEC_ID_LENGTH     3

/*-----------------------------------------------------------------------------
 * Configure the MSS QSPI
 */
void Flash_init
(
    mss_qspi_io_format io_format
);

/* Read winbond flash device ID
 */
void Flash_readid(uint8_t* rd_buf);

/* Erase the flash memory
 */
void Flash_erase(void);

/* Erase the complete flash memory.
 */
void Flash_die_erase(void);

/* Read flash memory data
 */
void Flash_read
(
    uint8_t* buf,
    uint32_t read_addr,
    uint32_t read_len
);

/* Read winbond flash status register
 */
void Flash_read_statusreg
(
    uint8_t* rd_buf
);

/* Program the flash memory
 */
uint8_t Flash_program
(
    uint8_t* buf,
    uint32_t wr_addr,
    uint32_t wr_len
);

void Flash_sector_erase(uint32_t addr);

void Flash_read_flagstatusreg
(
    uint8_t* rd_buf
);

void Flash_force_normal_mode
(
    void
);

mss_qspi_io_format Flash_probe_io_format
(
    void
);

static inline void Flash_init_normal(void)
{
    qspi_config.clk_div =  MSS_QSPI_CLK_DIV_2;
    qspi_config.sample = MSS_QSPI_SAMPLE_POSAGE_SPICLK;
    qspi_config.spi_mode = MSS_QSPI_MODE3;
    qspi_config.xip = MSS_QSPI_DISABLE;
    qspi_config.io_format = MSS_QSPI_NORMAL;
    MSS_QSPI_configure(&qspi_config);
}

/* Winbond flash instruction set */

#define WINBOND_DEVICE_RESET                    0xFF
#define WINBOND_READ_JEDEC_ID_OPCODE            0x9F
#define WINBOND_READ_STATUS_REGISTER            0x0F
#define WINBOND_WRITE_STATUS_REG                0x01
#define WINBOND_WRITE_DISABLE                   0x04
#define WINBOND_WRITE_ENABLE                    0x06
#define WINBOND_READ_DATA                       0x03
#define WINBOND_BB_MANAGEMENT                   0xA1
#define WINBOND_READ_BBM_LUT                    0xA5
#define WINBOND_ECC_FAILURE_PAGE_ADDR           0xA9
#define WINBOND_BLOCK_ERASE                     0xD8
#define WINBOND_DATA_LOAD                       0x02
#define WINBOND_RANDOM_PROG_DATA_LOAD           0x84
#define WINBOND_QUAD_PROG_DATA_LOAD             0x32
#define WINBOND_RANDOM_QUAD_PROG_DATA_LOAD      0x34
#define WINBOND_PROGRAM_EXECUTE                 0x10
#define WINBOND_PAGE_DATA_READ                  0x13
#define WINBOND_READ                            0x03
#define WINBOND_FAST_READ                       0x0B
#define WINBOND_FAST_READ_4B_ADDR               0x0C
#define WINBOND_FAST_READ_DUAL_OUTPUT           0x3B
#define WINBOND_FAST_READ_DUAL_OUTPUT_4B_ADDR   0x3C
#define WINBOND_FAST_READ_QUAD_OUTPUT           0x6B
#define WINBOND_FAST_READ_QUAD_OUTPUT_4B_ADDR   0x3C
#define WINBOND_FAST_READ_DUAL_IO               0xBB
#define WINBOND_FAST_READ_DUAL_IO_4B_ADDR       0xBC
#define WINBOND_FAST_READ_QUAD_IO               0xEB
#define WINBOND_FAST_READ_QUAD_IO_4B_ADDR       0xEC

#ifdef __cplusplus
}
#endif

#endif /* MSS_WINBOND_W25N_H_*/
