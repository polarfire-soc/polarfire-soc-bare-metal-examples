/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "drivers/mss/mss_usb/mss_usb_host_msc.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		2	/* Example: Map Ramdisk to physical drive 2 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		0	/* Example: Map USB MSD to physical drive 0 */

#define PHY_DRIVE_ZERO 0u /* Physical drive number 0 : USB MSD */
#define SUCCESS        0U
#define ERROR          1U

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    return 0; /* success */
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    return 0; /* success */
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
    if( (PHY_DRIVE_ZERO != pdrv) && (NULL == buff) )
    {
        return(RES_ERROR);
    }

    if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
    {
        return(RES_NOTRDY);
    }
    else if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
    {

        if(SUCCESS == MSS_USBH_MSC_read(buff, sector, count))
        {
            while(MSS_USBH_MSC_is_scsi_req_complete());
            return(RES_OK);
        }
        else
        {
            return(RES_ERROR);
        }
    }
    else
    {
        return(RES_ERROR);
    }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
    if( (PHY_DRIVE_ZERO != pdrv) && (NULL == buff) )
    {
        return(RES_ERROR);
    }

    if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
    {
        return(RES_NOTRDY);
    }
    else if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
    {
        if(SUCCESS == MSS_USBH_MSC_write((uint8_t*)buff, sector, count))
        {
            while(MSS_USBH_MSC_is_scsi_req_complete());
            return(RES_OK);
        }
        else
        {
            return(RES_ERROR);
        }
    }
    else
    {
        return(RES_ERROR);
    }
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    UINT *result;

    if( (NULL != buff) && (PHY_DRIVE_ZERO == pdrv))
    {
        result = (UINT *)buff;
    }
    else
    {
        return(RES_ERROR);
    }

    switch (cmd) {
    case CTRL_SYNC:
        break;
    case CTRL_POWER:
        break;
    case CTRL_LOCK:
        break;
    case CTRL_EJECT:
        break;
    case GET_SECTOR_COUNT:
        if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
        {
            *result = MSS_USBH_MSC_get_sector_count();
            return RES_OK;
        }
        else if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
        {
            *result = 0u;
            return RES_NOTRDY;
        }
        else
        {
            *result = 0u;
            return RES_ERROR;
        }

    case GET_SECTOR_SIZE:
        if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
        {
            *result = MSS_USBH_MSC_get_sector_size();
            return RES_OK;
        }
        else if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
        {
            *result = 0u;
            return RES_NOTRDY;
        }
        else
        {
            *result = 0u;
            return RES_ERROR;
        }

    case GET_BLOCK_SIZE:
        *result = 512;/*Erase Block Size */
        break;
    default:
        break;
    }
    return (DRESULT)0;
}

