/**************************************************************************//**
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54.
 *
 * This example project demonstrates the use of the PolarFire MSS SPI hardware
 * block. It reads and writes the content of an external SPI flash device.
 */

#include <stdint.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_spi/mss_spi.h"
#include "drivers/off-chip/mt25ql01gbbb/mt25ql01gbbb.h"

#define BUFFER_SIZE                     3000U

/* Manufacture and device ID for MICRON MT25QL01GBBB Flash Memory */
#define FLASH_MANUFACTURER_ID       (uint8_t)0x20
#define FLASH_DEVICE_ID             (uint8_t)0xBB

/* Static Global variables */
static uint8_t g_flash_wr_buf[BUFFER_SIZE];
static uint8_t g_flash_rd_buf[BUFFER_SIZE];

/* Local Function */
static uint8_t verify_write(uint8_t* write_buff, uint8_t* read_buff, uint16_t size);
static void mss_spi_overflow_handler(uint8_t mss_spi_core);

/**************************************************************************//**
 * Main Function
 */

void u54_1(void)
{
    volatile uint32_t errors;
    uint32_t loop_count;
    uint8_t manufacturer_id = 0U;
    uint8_t device_id = 0U;
    uint32_t address = 0U;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /*Put this hart into WFI.*/

    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();
#endif

    SYSREG->SOFT_RESET_CR &= ~(0x01UL << 10U);

/**************************************************************************//**
 * Initialize write and read buffers
 */
    for (loop_count = 0U; loop_count < (BUFFER_SIZE/2); loop_count++)
    {
        g_flash_wr_buf[loop_count] = 0x44U + loop_count;
        g_flash_rd_buf[loop_count] = 0x00U;
    }

    for (loop_count = (BUFFER_SIZE / 2); loop_count < BUFFER_SIZE; loop_count++)
    {
        g_flash_wr_buf[loop_count] = 0x33U;
        g_flash_rd_buf[loop_count] = 0x00U;
    }

/**************************************************************************//**
 * Flash Driver Initialization
 */

    FLASH_init(&g_mss_spi0_lo, mss_spi_overflow_handler);

    FLASH_global_unprotect(&g_mss_spi0_lo);

    FLASH_erase_64k_block(&g_mss_spi0_lo, 0U);

/**************************************************************************//**
 * Check SPI Flash part manufacturer and device ID.
 */
    FLASH_read_device_id(&g_mss_spi0_lo, &manufacturer_id, &device_id);
    if((FLASH_MANUFACTURER_ID != manufacturer_id) || (FLASH_DEVICE_ID != device_id))
    {
        ++errors;
    }

/**************************************************************************//**
 * Write Data to Flash.
 */
    address = 200U;
    FLASH_program(&g_mss_spi0_lo, address, g_flash_wr_buf, sizeof(g_flash_wr_buf));

    /* Read Data From Flash */
    address = 200U;
    FLASH_read(&g_mss_spi0_lo, address, g_flash_rd_buf, sizeof(g_flash_wr_buf));

    errors = verify_write(g_flash_rd_buf, g_flash_wr_buf, sizeof(g_flash_wr_buf));

    while(1U)
    {
        ;
    }
}

/**************************************************************************//**
 * Read the data from SPI flash and compare the same with write buffer
 */
static uint8_t verify_write(uint8_t* write_buff, uint8_t* read_buff, uint16_t size)
{
    uint8_t error = 0U;
    uint16_t index = 0U;

    while(size != 0U)
    {
        if(write_buff[index] != read_buff[index])
        {
            error = 1U;
            break;
        }
        index++;
        size--;
    }
    return error;
}

/**************************************************************************//**
 * SPI buffer overflow handler
 * This function is called by SPI driver in case of buffer overflow.
 */
static void mss_spi_overflow_handler(uint8_t mss_spi_core)
{
    if (mss_spi_core)
    {
        /* reset SPI1 */
        /* Take SPI1 out of reset. */
        (void)mss_config_clk_rst(MSS_PERIPH_SPI1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    }
    else
    {
        /* reset SPI0 */

         /* Take SPI0 out of reset. */
        (void)mss_config_clk_rst(MSS_PERIPH_SPI0, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    }
}
