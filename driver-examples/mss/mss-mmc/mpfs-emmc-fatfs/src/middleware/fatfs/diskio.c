/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "../../middleware/fatfs/diskio.h"
#include <stdint.h>

#if defined(PHY_DRIV_MMC) && defined(PHY_DRIV_USB)
#error "please define single physical memory drive either PHY_DRIV_MMC \
or PHY_DRIV_USB in discio.h file."
#endif

#if !defined(PHY_DRIV_MMC) && !defined(PHY_DRIV_USB)
#error "please define a physical memory drive either PHY_DRIV_MMC \
or PHY_DRIV_USB in discio.h file."
#endif

#ifdef PHY_DRIV_MMC
#include "mmc_di_if.h"
#endif /* PHY_DRIV_MMC */

/* Disk status */
volatile DSTATUS Stat = STA_NOINIT;

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize(BYTE pdrv /* Physical drive number (0..) */
)
{
    DSTATUS tempStatus = STA_NOINIT;

#ifdef PHY_DRIV_MMC

    tempStatus = mmc_di_if_init();

#endif

#ifdef PHY_DRIV_USB

    /* USB drive initialisation */

#endif

    /* If initialised return Success : 0 else return STA_NOINIT */
    return (0u == tempStatus) ? 0 : STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE drv /* Physical drive number (0..) */
)
{

    return Stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read(BYTE drv,     /* Physical drive number (0..) */
                  BYTE *buff,   /* Data buffer to store read data */
                  DWORD sector, /* Sector address (LBA) */
                  BYTE count    /* Number of sectors to read (1..255) */
)
{
    DRESULT read_result = RES_ERROR;

    if (0u == drv)
    {

#ifdef PHY_DRIV_MMC

        read_result = mmc_di_if_read(sector, buff, count);

#endif

#ifdef PHY_DRIV_USB

        /* USB read function */

#endif
    }
    return read_result;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
/* The FatFs module will issue multiple sector transfer request
/  (count > 1) to the disk I/O layer. The disk function should process
/  the multiple sector transfer properly Do. not translate it into
/  multiple single sector transfers to the media, or the data read/write
/  performance may be drastically decreased. */
#if _READONLY == 0
DRESULT disk_write(BYTE pdrv,        /* Physical drive number (0..) */
                   const BYTE *buff, /* Data to be written */
                   DWORD sector,     /* Sector address (LBA) */
                   BYTE count        /* Number of sectors to write (1..255) */
)
{
    DRESULT write_result = RES_ERROR;

    if (0u == pdrv)
    {

#ifdef PHY_DRIV_MMC

        write_result = mmc_di_if_write(buff, sector, (count * 512));

#endif

#ifdef PHY_DRIV_USB

        /* USB write function */

#endif
    }
    return write_result;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
/*
 * Number of Drives(Logical Units) is always 1.
*/
DRESULT disk_ioctl(BYTE pdrv, /* Physical drive number (0..) */
                   BYTE ctrl, /* Control code */
                   void *buff /* Buffer to send/receive control data */
)
{

    if (0u == pdrv)
    {

        switch (ctrl)
        {

        case CTRL_SYNC:

            break;

        case CTRL_POWER:

            break;

        case CTRL_LOCK:

            break;

        case CTRL_EJECT:

            break;

        case GET_SECTOR_COUNT:

#ifdef PHY_DRIV_MMC

            *(UINT *)buff = DISCIO_DRIV_MMC_SECTOR_COUNT;

#endif

#ifdef PHY_DRIV_USB

            /* USB sector count */

#endif

            break;

        case GET_SECTOR_SIZE:

#ifdef PHY_DRIV_MMC
            /* Block Size for Read and Write */
            *(UINT *)buff = DISCIO_DRIV_MMC_SECTOR_SIZE;

#endif

#ifdef PHY_DRIV_USB

            /* USB sector size */

#endif

            break;

        case GET_BLOCK_SIZE:

#ifdef PHY_DRIV_MMC

            /* Minimum Erase Block count */
            *(UINT *)buff = DISCIO_DRIV_MMC_BLOCK_SIZE;

#endif

#ifdef PHY_DRIV_USB

            /* USB Block size */

#endif

            break;

        default:

            break;
        }
    }

    return (DRESULT)0;
}


