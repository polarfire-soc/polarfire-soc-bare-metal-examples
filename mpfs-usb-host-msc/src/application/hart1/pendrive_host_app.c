/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * USB MSC Class Host controller example application to demonstrate the
 * PolarFire SoC MSS USB operations in USB Host mode.
 *
 * This project can detect "Memory Stick" category devices and perform file
 * operations on it. To know more about how to use this project please refer
 * README.md in this project's folder.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "pendrive_host_app.h"
#include "inc/common.h"
#include "mpfs_hal/mss_hal.h"
#include "mpfs_hal/common/mss_mpu.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_usb/mss_usb_std_def.h"
#include "drivers/mss/mss_usb/mss_usb_host.h"
#include "drivers/mss/mss_usb/mss_usb_host_msc.h"
#include "drivers/mss/mss_usb/mss_usb_common_cif.h"

#define MAX_ELEMENT_COUNT                               20u

/* This buffer is passed to the USB driver. When USB drivers are configured to
 * use internal DMA, the address of this buffer must be modulo-4.Otherwise DMA
 * Transfer will fail.
 */
uint8_t buffer[BUFFER_SIZE] __attribute__ ((aligned (4))) = {0u};
char* file_name[MAX_ELEMENT_COUNT][13] __attribute__ ((aligned (4))) = {{0}};
uint8_t new_fname[13u] __attribute__ ((aligned (4)));
uint8_t g_dev_descr[18] __attribute__ ((aligned (4)))= {0};

BYTE file_attrib[MAX_ELEMENT_COUNT] = {0};

/* Global variables */
uint8_t volatile g_msc_driver_ready = 0u;
uint8_t volatile g_msc_driver_released = 0u;

FATFS fs;
FRESULT res;
FIL fsrc;
FIL fdest;

static void USB_DEV_attached(mss_usb_device_speed_t speed);
static void USB_DEV_dettached(void);
static void USB_DEV_oc_error(void);
static void USB_DEV_enumerated(mss_usbh_tdev_info_t tdev_info);
static void USB_DEV_class_driver_assigned(void);
static void USB_DEV_not_supported(int8_t error_code);
static void USB_DEV_permanent_erro(void);

static void MSC_DEV_valid_config(void);
static void MSC_DEV_ready(void);
static void MSC_DEV_driver_released(void);
static void MSC_DEV_error(int8_t error_code);

static uint8_t hex_to_char(uint8_t nibble_value);
static void display_hex_byte(uint8_t hex_byte);

/**
 * Implementation of mss_usbh_user_cb_t needed by USBH driver
 */
mss_usbh_user_cb_t MSS_USBH_user_cb =
{
    USB_DEV_attached,
    USB_DEV_dettached,
    USB_DEV_oc_error,
    USB_DEV_enumerated,
    USB_DEV_class_driver_assigned,
    USB_DEV_not_supported,
    USB_DEV_permanent_erro
};

mss_usbh_msc_user_cb_t MSS_USBH_MSC_user_cb =
{
    MSC_DEV_valid_config,
    MSC_DEV_ready,
    MSC_DEV_driver_released,
    MSC_DEV_error,
};

/**
 * Prints on serial port
 */
static void display_hex_byte(uint8_t hex_byte)
{
    uint8_t hex_value_msg[2];
    uint8_t upper;
    uint8_t lower;

    upper = (hex_byte >> 4u) & 0x0Fu;
    lower = hex_byte & 0x0Fu;
    hex_value_msg[0] = hex_to_char(upper);
    hex_value_msg[1] = hex_to_char(lower);
    MSS_UART_polled_tx(&g_mss_uart1_lo, hex_value_msg, sizeof(hex_value_msg));
    MSS_UART_polled_tx_string(&g_mss_uart1_lo, (uint8_t *)" ");
}

static uint8_t hex_to_char(uint8_t nibble_value)
{
    uint8_t hex_char = '0';

    if (nibble_value < 10u)
    {
        hex_char = nibble_value + '0';
    }
    else if (nibble_value < 16u)
    {
        hex_char = nibble_value + 'A' - 10;
    }

    return hex_char;
}

/**
 * Print all root directories on terminal
 */
FRESULT scan_root_dir (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int elements = 0u;

    /* This function is assuming non-Unicode cfg. */
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\rElements in Root Directory (Maximum 10):\n\r");

    res = f_opendir(&dir, path);                    /* Open the directory */
    if (res == FR_OK)
    {
        while (elements < MAX_ELEMENT_COUNT)
        {
            res = f_readdir(&dir, &fno);            /* Read a directory item */
            if (fno.fname[0] == '.') continue;      /* Ignore dot entry */

            if (res != FR_OK)
            {
                MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                        (uint8_t*) "\n\r Error reading root directory\n\r");
                break;
            }
            else
            {
                if (fno.fname[0] == 0)
                {
                    if (elements == 0u)
                    {
                        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                (uint8_t*) "\n\rRoot directory is empty\n\r");
                    }
                    break;
                }
                else
                {
                    uint8_t character = 0;
                    /* store file names and attributes for using
                     * it in File read menu. */
                    strncpy((char*) file_name[elements], fno.fname, 13);
                    file_attrib[elements] = fno.fattrib;
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                            (uint8_t*) "Testing\n\r ");
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                            (uint8_t*) "\n\r ");
                    character = elements + '0';
                    MSS_UART_polled_tx(&g_mss_uart1_lo, (uint8_t*) &character,
                            1u);
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                            (uint8_t*) " -   ");
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                            (uint8_t*) fno.fname);
                }
            }
            elements++;
        }
    }
    return res;
}

/**
 * Print message on terminal
 */
static void USB_DEV_attached(mss_usb_device_speed_t speed)
{
    if (speed == MSS_USB_DEVICE_HS)
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\n\rHS ");
    if (speed == MSS_USB_DEVICE_FS)
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\n\rFS ");
    if (speed == MSS_USB_DEVICE_LS)
        MSS_UART_polled_tx_string(  &g_mss_uart1_lo,(uint8_t *)"\n\rLS ");

    MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"Device detected\n\r");
}

/**
 * Print message on terminal
 */
static void USB_DEV_dettached(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rUSB device removed\n");
}

/**
 * Print message on terminal
 */
static void USB_DEV_oc_error(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rOverCurrent error");
}

/**
 * Print message on terminal
 */
static void USB_DEV_enumerated(mss_usbh_tdev_info_t tdev_info)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rEnumeration successful\n");
}

/**
 * Print message on terminal
 */
static void USB_DEV_class_driver_assigned(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rMSC Class driver assigned");
}

/**
 * Print message on terminal if applicable
 */
static void USB_DEV_not_supported(int8_t error_code) {
    switch (error_code) {
    case FS_DEV_NOT_SUPPORTED:
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\rFS Device Not Supported");
        break;
    case LS_DEV_NOT_SUPPORTED:
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\rLS Device Not Supported");
        break;
    case HUB_CLASS_NOT_SUPPORTED:
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\rHUB Not Supported");
        break;
    case CLASS_DRIVER_NOT_MATCHED:
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\rClass Not Supported");
        break;
    case CLASS_DRIVER_NOT_FREE:
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\rClass driver not free");
        break;
    case CONF_DESC_POWER_LIM_EXCEED:
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\rDevice needs more power");
        break;
    case CONF_DESC_WRONG_DESC_TYPE:
    case CONF_DESC_WRONG_LENGTH:
    case DEV_DESC_LS_MAXPKTSZ0_NOT8:
    case DEV_DESC_HS_MAXPKTSZ0_NOT64:
    case DEV_DESC_HS_USBBCD_NOT200:
    case DEV_DESC_WRONG_MAXPKTSZ0:
    case DEV_DESC_WRONG_USBBCD:
    case DEV_DESC_WRONG_DESC_TYPE:
    case DEV_DESC_WRONG_LENGTH:
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "Device not supported");
        break;
    default:
        break;
    }
}

/**
 * Print message on terminal
 */
static void USB_DEV_permanent_erro(void) {
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\rPermanent error, try resetting SF2....");
}

/**
 * Print message on terminal
 */
static void MSC_DEV_valid_config(void) {
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rConfiguring the MSC device...");
}

/**
 * Print message on terminal
 */
static void MSC_DEV_ready(void) {
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rThe MSC device is ready for transfers.");
    g_msc_driver_ready = 1u;
}

/**
 * Print message on terminal
 */
static void MSC_DEV_driver_released(void) {
    g_msc_driver_released = 1u;
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rUSBH MSC driver released\n\r");
}

/**
 * Print the error message on the terminal.
 */
static void MSC_DEV_error(int8_t error_code) {
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\rMSC device error");
}

/**
 * Display the main menu on the terminal
 */
void display_main_menu(void) {
    MSS_UART_polled_tx_string(&g_mss_uart1_lo, (uint8_t*) "\n\n\rSelect Menu");
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\r   1) List Root-Directory elements");
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\r   2) Copy file");
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\r   3) Read file (First 512 bytes)");
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\r   4) Get Device Descriptor");
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\r   5) Suspend Host");
}

/**
 * Print message on terminal
 */
void goto_main_menu(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\n\rPress 'ESC' to go back to the main menu");
}

/**
 * Print message on terminal
 */
void read_file_menu(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\r Choose file out of listed files by its \
                    index ('0' onwards)\n\r");
}

/**
 * Print message on terminal
 */
void copy_file_menu(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\n\r Choose file out of listed files by \
                    its index ('0' onwards)\n\r");
}

/**
 * Print the error message on the terminal.
 */
void invalid_selection_menu(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
            (uint8_t*) "\n\r\n\rInvalid selection. Try again\n\r");
}

/**
 * Read file and print content of file on terminal
 */
FRESULT read_root_dir_file (uint8_t index)
{
    FRESULT res;
    uint32_t bytes_read = 0u;

    if (file_attrib[index] & AM_DIR) {
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\n\rAccessing directories is not supported");
        return (FR_INVALID_PARAMETER);
    } else {
        res = f_open(&fsrc, (char*) file_name[index],
                FA_OPEN_EXISTING | FA_READ);
        if (FR_OK == res) {
            res = f_read(&fsrc, buffer, sizeof(buffer), (UINT*) &bytes_read);
            MSS_UART_polled_tx(&g_mss_uart1_lo, (uint8_t*) buffer,
                    sizeof(buffer));
        } else {
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                    (uint8_t*) "\n\n\rFile read error");
        }

        f_close(&fsrc);
    }
    return (res);
}

/**
 * Copy and paste the file
 */
FRESULT copy_root_dir_file(uint8_t index, uint8_t character)
{
    FRESULT res;
    uint32_t bytes_read = 0u;
    uint32_t bytes_written = 0u;

    if (file_attrib[index] & AM_DIR) {
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\n\rAccessing directories is not supported");
        return (FR_INVALID_PARAMETER);
    } else {
        res = f_open(&fsrc, (char*) file_name[index],
                FA_OPEN_EXISTING | FA_READ);
        if (res) {
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                    (uint8_t*) "\n\n\rError while opening the selected file");
        }

        strncpy((char*) new_fname, (char*) file_name[index], 13);

        new_fname[0] = character; /* Replace First character of filename */

        res = f_open(&fdest, (char*) new_fname, FA_CREATE_ALWAYS | FA_WRITE);
        if (FR_OK == res) {
            /* Copy source to destination */
            for (;;) {
                /* Read a chunk of src file */
                res = f_read(&fsrc, buffer, sizeof buffer, (UINT*) &bytes_read);

                MSS_UART_polled_tx_string(&g_mss_uart1_lo, (uint8_t*) ".");
                if (res || bytes_read == 0)
                    break; /* error or eof */

                res = f_write(&fdest, buffer, bytes_read,
                        (UINT*) &bytes_written);/* Write it to the dst file */

                if (res || bytes_written < bytes_read)
                    break; /* error or disk full */
            }

            if (res)
                MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                        (uint8_t*) "\n\n\rCopy error");
            else if (bytes_read == 0) {
                MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                        (uint8_t*) "\n\n\rCopy success \n\r New file name - ");
                MSS_UART_polled_tx(&g_mss_uart1_lo, (uint8_t*) new_fname, 13u);
            } else if (bytes_written < bytes_read)
                MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                        (uint8_t*) "\n\n\rDisk space is not sufficient");
        } else if (FR_EXIST == res) {
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                    (uint8_t*) "\n\n\rTrying to create ");
            MSS_UART_polled_tx(&g_mss_uart1_lo, (uint8_t*) new_fname,
                    sizeof(new_fname));
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                    (uint8_t*) "\n\n\rThe file already exists");
        }

        f_close(&fsrc);
        f_close(&fdest);
        return (res);
    }
}

/**
 * Print device descriptor on terminal
 */
void get_device_descriptor(void)
{
    uint8_t idx = 0u;

    if (0u == MSS_USBH_get_std_dev_descr(g_dev_descr)) {
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\n\rDevice Descriptor:");
        do {
            display_hex_byte(g_dev_descr[idx]);
            idx++;
        } while (idx < 8);
    } else {
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                (uint8_t*) "\n\n\rError reading the descriptor");
    }
}

#ifdef __cplusplus
}
#endif
