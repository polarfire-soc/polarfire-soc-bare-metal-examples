/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_
 *
 */

#include "pendrive_host_app.h"
#include "inc/common.h"
#include "mpfs_hal/mss_hal.h"
#include "mpfs_hal/common/mss_mpu.h"
#include "FatFs/src/ff.h"
#include "FatFs/src/diskio.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/mss/mss_usb/mss_usb_std_def.h"
#include "drivers/mss/mss_usb/mss_usb_host.h"
#include "drivers/mss/mss_usb/mss_usb_host_msc.h"

#define ESC_KEY                                         27u
#define ENTER_KEY                                       13u

#define USB_DMA_PLIC_PRIORITY                           2u
#define USB_MC_PLIC_PRIORITY                            2u

#define DO_NOT_MOUNT                                    0u
#define MOUNT_IMMEDIATELY                               1u

#define SELECT_OPTION_FROM_MENU                         0u
#define LIST_ROOT_DIRECTORY_ELEMENTS                    1u
#define COPY_FILE                                       2u
#define READ_FILE                                       3u
#define GET_DEVICE_DESCRIPTOR                           4u
#define SUSPEND_HOST                                    5u
#define START_COPY_FILE                                 33u

uint8_t g_copy_file_index = 0;
char path[] = "0:";

char local_drive_number = 0; /* Logical drive number to be mounted : 0 for USB*/

/* Global variables */
extern uint8_t volatile g_msc_driver_ready;
extern uint8_t volatile g_msc_driver_released;

extern mss_usbh_user_cb_t MSS_USBH_user_cb;
extern mss_usbh_msc_user_cb_t MSS_USBH_MSC_user_cb;

extern FATFS fs;

/* Main function for the HART1(U54_1 processor).
 * Application code running on HART1 is placed here
 *
 * The HART1 goes into WFI. HART0 brings it out of WFI when it raises the first
 * Software interrupt to this HART
 */
void u54_1(void)
{
    uint8_t rx_size = 0u;
    uint8_t key;
    uint8_t menu_level = 0u;

    /* Clear pending software interrupt in case there was any.
       Enable only the software interrupt so that the E51 core can bring this
       core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Put this hart in WFI. */
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is now out of WFI, clear the SW interrupt. Here onwards the
     * application can enable and use any interrupts as required */
    clear_soft_interrupt();
#endif

    init_memory();

    /* Bring the MMUART1 and USB out of Reset */
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_USB, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_TIMER, (uint8_t) 1, PERIPHERAL_ON);

    MSS_UART_init(&g_mss_uart1_lo,
                  MSS_UART_115200_BAUD,
                  (MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY |
                  MSS_UART_ONE_STOP_BIT));

    MSS_UART_polled_tx_string(&g_mss_uart1_lo, "\n\rUSB MSD Host Example.\n\r");

    PLIC_init();
    __enable_irq();

    PLIC_SetPriority(USB_DMA_PLIC, USB_DMA_PLIC_PRIORITY);
    PLIC_SetPriority(USB_MC_PLIC, USB_MC_PLIC_PRIORITY);

    /* Initialize SysTick */
    SysTick_Config();

    /* Initialize the USBH driver */
    MSS_USBH_init(&MSS_USBH_user_cb);

    /* Initialize the USBH-MSC driver */
    MSS_USBH_MSC_init(&MSS_USBH_MSC_user_cb);

    /*
     * Get Class driver handle from the USBH-MSC class driver and register it
     * with the USBH driver.
     * On Device attachment, USBH driver will look for Device information
     * through descriptors and if match it with the Class driver using this
     * Handle.
     */
    MSS_USBH_register_class_driver(MSS_USBH_MSC_get_handle());

    while (1U)
    {
        if (g_msc_driver_ready == 1u)
        {
            g_msc_driver_ready = 0u;
            g_msc_driver_released = 0u;
            menu_level = SELECT_OPTION_FROM_MENU;
            key = 0u;
            rx_size = 0u;
            display_main_menu();

            /*
             * Register the FatFS Work area.
             * Note:   Only one Logical unit (drive 0:) is supported.
             * If an Unformatted Drive is connected, this API will take long
             * time to complete its operation (Depending on drive size).
             */
            f_mount(&fs, &local_drive_number, MOUNT_IMMEDIATELY);
        }

        rx_size = MSS_UART_get_rx(&g_mss_uart1_lo, &key, 1u);
        if (g_msc_driver_released == 0u)
        {
            switch (menu_level)
            {
            case SELECT_OPTION_FROM_MENU:
                if (rx_size > 0u)
                {
                    switch (key)
                    {
                    case '1':/* List elements */
                        menu_level = LIST_ROOT_DIRECTORY_ELEMENTS;
                        scan_root_dir(path);
                        goto_main_menu();
                        break;

                    case '2':/* copy file */
                        menu_level = COPY_FILE;
                        scan_root_dir(path);
                        copy_file_menu();
                        break;

                    case '3':/* read file */
                        menu_level = READ_FILE;
                        scan_root_dir(path);
                        read_file_menu();
                        break;

                    case '4':/* test mode */
                        menu_level = GET_DEVICE_DESCRIPTOR;
                        get_device_descriptor();
                        break;

                    case '5':/* suspend host */
                        menu_level = SUSPEND_HOST;
                        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                (uint8_t*) "\n\n\r Host suspended\n\r");
                        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                            (uint8_t*) "\n\r Press 'Enter' to resume host\n\r");
                        MSS_USBH_suspend();
                        break;

                    default:
                        invalid_selection_menu();
                        break;
                    }
                }
                break;

            case SUSPEND_HOST:
                if (rx_size > 0u)
                {
                    switch (key)
                    {
                    case ENTER_KEY: /* ESC Key */
                        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                (uint8_t*) "\n\r Host resumed\n\r");
                        MSS_USBH_resume();
                        display_main_menu();
                        menu_level = SELECT_OPTION_FROM_MENU;
                        break;

                    default:
                        invalid_selection_menu();
                        break;
                    }
                }
                break;

            case READ_FILE:
                if (rx_size > 0u)
                {
                    switch (key)
                    {
                    case ESC_KEY: /* ESC Key */
                        display_main_menu();
                        menu_level = SELECT_OPTION_FROM_MENU;
                        break;

                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        read_root_dir_file(key - 48u);
                        goto_main_menu();
                        break;
                    default:
                        invalid_selection_menu();
                        break;
                    }
                }
                break;

            case COPY_FILE:
                if (rx_size > 0u)
                {
                    switch (key)
                    {
                    case ESC_KEY: /* ESC Key */
                        display_main_menu();
                        menu_level = SELECT_OPTION_FROM_MENU;
                        break;

                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        g_copy_file_index = (key - '0');
                        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                (uint8_t*) "\n\r Enter Alphanumeric character \
                                         to copy file.\n\r");
                        menu_level = START_COPY_FILE;
                        break;
                    default:
                        invalid_selection_menu();
                        break;
                    }
                }
                break;

            case START_COPY_FILE:
                if (rx_size > 0u)
                {
                    if (ESC_KEY == key)   //ESC Key
                    {
                        display_main_menu();
                        menu_level = SELECT_OPTION_FROM_MENU;
                    }
                    else if ((('0' <= key) & (key <= '9'))
                            || (('a' <= key) & (key <= 'z'))
                            || (('A' <= key) & (key <= 'Z')))
                    {
                        MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                (uint8_t*) "\n\r copying file...");
                        copy_root_dir_file(g_copy_file_index, key);
                        goto_main_menu();
                    }
                    else
                    {
                        invalid_selection_menu();
                    }
                }
                break;

            case GET_DEVICE_DESCRIPTOR:
                display_main_menu();
                menu_level = SELECT_OPTION_FROM_MENU;
                break;

            case LIST_ROOT_DIRECTORY_ELEMENTS:
                if (rx_size > 0u)
                {
                    switch (key)
                    {
                    case ESC_KEY: /* ESC Key */
                        display_main_menu();
                        menu_level = SELECT_OPTION_FROM_MENU;
                        break;

                    default:
                        invalid_selection_menu();
                        break;
                    }
                }
                break;

            default:
                break;
            }
            rx_size = 0;
        }
    }
  /* Never return */
}

void U54_1_sysTick_IRQHandler(void)
{
    /*
     * This function must be called. This function provides the time stamp
     * which is used to keep track of activities such as USB Reset, Settling
     * time etc. in Enumeration process.
     * The USBH driver and USBH-MSC class driver housekeeping task is also run
     * based on this timestamp.
     *
     * This avoids the application to wait for the enumeration process to
     * complete which might take time in 100s of miliseconds depending on
     * target device.
     * You may choose a general purpose timer instead of Systick.
     */
    MSS_USBH_task();
    MSS_USBH_MSC_task();
    MSS_USBH_1ms_tick();
}

/* HART1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    /* Do Nothing */
}
