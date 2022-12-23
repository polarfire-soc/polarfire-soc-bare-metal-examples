/*******************************************************************************
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is working as interface for diskio file to provide low level memory
 * read, write and initialisation related operation for MMC.
 *
 */
#ifndef MMC_DI_IF_H
#define MMC_DI_IF_H

/*-------------------------- Header file section -----------------------------*/
#include "diskio.h"
#include <stdint.h>

#ifdef PHY_DRIV_MMC

#ifdef __cplusplus
extern "C"
{
#endif

/* MMC Device Parameters */

#define DISCIO_DRIV_MMC_SECTOR_COUNT   (0x82000U)
#define DISCIO_DRIV_MMC_SECTOR_SIZE   (0x200U) /* sector size after power up */
#define DISCIO_DRIV_MMC_BLOCK_SIZE   (0x400U) /* This size is given as minimum
number of write blocks that can be erased in a single erase command */

/*----------------- Global function declaration section ----------------*/
DSTATUS mmc_di_if_init(void);
DRESULT mmc_di_if_write(const BYTE *buff, DWORD sector, BYTE count);
DRESULT mmc_di_if_read(DWORD sector, BYTE *buff, BYTE count);

#ifdef __cplusplus
}
#endif

#endif /* PHY_DRIV_MMC */

#endif /* MMC_DI_IF_H */


