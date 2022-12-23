/*******************************************************************************
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is working as interface for diskio file to provide low level memory
 * read, write and initialisation related operation for MMC.
 *
 */

/*-------------------------- Header file section -----------------------------*/
#include "drivers/mss/mss_mmc/mss_mmc.h"
#include "drivers/mss/mss_mmc/mss_mmc_if.h"
#include "drivers/mss/mss_mmc/mss_mmc_regs.h"
#include "drivers/mss/mss_mmc/mss_mmc_types.h"
#include "mmc_di_if.h"
#include "mpfs_hal/mss_hal.h"

#ifdef PHY_DRIV_MMC
/*-------------------------- Local variable section --------------------------*/
static uint8_t g_mmc_initialized = false;

/*------------------------ Local function declaration section ----------------*/
static void mmc_reset_block(void);

/*------------------------- Local function definition section ----------------*/
/*This function will reset mmc block*/
static void mmc_reset_block(void)
{
    SYSREG->SUBBLK_CLOCK_CR |= (uint32_t)(SUBBLK_CLOCK_CR_MMC_MASK);
    SYSREG->SOFT_RESET_CR |= (uint32_t)(SOFT_RESET_CR_MMC_MASK);
    SYSREG->SOFT_RESET_CR &= ~(uint32_t)(SOFT_RESET_CR_MMC_MASK);
}

/*------------------------ Global function definition section ----------------*/
/* This function will initialise the MMC */
DSTATUS mmc_di_if_init(void)
{
    DRESULT status = RES_ERROR;
    mss_mmc_status_t ret_status = MSS_MMC_INIT_FAILURE;
    /* MMC initialisation */
    mss_mmc_cfg_t g_mmc = {0u};

    /* If MMC is initialized already not initialize again */
    if (true == g_mmc_initialized)
    {
        ret_status = MSS_MMC_INIT_SUCCESS;
    }
    else
    {
        ASSERT(mss_does_xml_ver_support_switch() == true);

        if (switch_mssio_config(EMMC_MSSIO_CONFIGURATION) == false)
        {
            while (1u)
                ;
        }

        switch_external_mux(EMMC_MSSIO_CONFIGURATION);

        /* eMMC configuration */
        g_mmc.clk_rate = MSS_MMC_CLOCK_50MHZ;
        g_mmc.card_type = MSS_MMC_CARD_TYPE_MMC;
        g_mmc.bus_speed_mode = MSS_MMC_MODE_SDR;
        g_mmc.data_bus_width = MSS_MMC_DATA_WIDTH_8BIT;
        g_mmc.bus_voltage = MSS_MMC_1_8V_BUS_VOLTAGE;

        mmc_reset_block();

        ret_status = MSS_MMC_init(&g_mmc);

        if (MSS_MMC_INIT_SUCCESS == ret_status)
        {
            g_mmc_initialized = true;
            Stat = false;
        }
    }

    status = (MSS_MMC_INIT_SUCCESS == ret_status) ? 0 : STA_NOINIT;
    return status;
}

/* MMC write function */
DRESULT mmc_di_if_write(const BYTE *buff, DWORD sector, BYTE count)
{
    DRESULT status = RES_ERROR;
    mss_mmc_status_t mmc_status = MSS_MMC_TRANSFER_FAIL;

    /* Check status */
    do
    {
        mmc_main_plic_IRQHandler();
        mmc_status = MSS_MMC_get_transfer_status();
    } while (MSS_MMC_TRANSFER_IN_PROGRESS == mmc_status);

    /* Write to physical memory */
    mmc_status = MSS_MMC_adma2_write(buff, sector, (count * 512));

    /* Check status */
    if (MSS_MMC_TRANSFER_IN_PROGRESS == mmc_status)
    {
        do
        {
            mmc_main_plic_IRQHandler();
            mmc_status = MSS_MMC_get_transfer_status();
        } while (MSS_MMC_TRANSFER_IN_PROGRESS == mmc_status);
    }

    status = (MSS_MMC_TRANSFER_SUCCESS == mmc_status) ? RES_ERROR : RES_OK;
    return status;
}

/* MMC Read function */
DRESULT mmc_di_if_read(DWORD sector, BYTE *buff, BYTE count)
{

    DRESULT status = RES_ERROR;
    mss_mmc_status_t mmc_status = MSS_MMC_TRANSFER_FAIL;
    /* Check status */
    do
    {
        mmc_main_plic_IRQHandler();
        mmc_status = MSS_MMC_get_transfer_status();
    } while (MSS_MMC_TRANSFER_IN_PROGRESS == mmc_status);

    /* Read physical memory */
    mmc_status = MSS_MMC_adma2_read(sector, buff, (count * 512));

    /* Check status */
    if (MSS_MMC_TRANSFER_IN_PROGRESS == mmc_status)
    {
        do
        {
            mmc_main_plic_IRQHandler();
            mmc_status = MSS_MMC_get_transfer_status();
        } while (MSS_MMC_TRANSFER_IN_PROGRESS == mmc_status);
    }

    status = (MSS_MMC_TRANSFER_SUCCESS != mmc_status) ? RES_ERROR : RES_OK;
    return status;
}

#endif
/* End of file */


