/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * USB HID Class Host controller example application to demonstrate the
 * PolarFire SoC MSS USB operations in USB Host mode.
 *
 * This project can detect "Mouse" category devices and read the inputs
 * periodically.
 * To know more about how to use this project please refer
 * README.md in this project's folder.
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "inc/common.h"
#include "drivers/mss/mss_usb/mss_usb_host.h"
#include "drivers/mss/mss_usb/mss_usb_std_def.h"
#include "mpfs_hal/common/mss_mpu.h"
#include "drivers/mss/mss_usb/mss_usb_host.h"
#include "drivers/mss/mss_usb/mss_usb_host_hid.h"
#include "drivers/mss/mss_usb/mss_usb_host.h"
#include "drivers/mss/mss_usb/mss_usb_std_def.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "mpfs_hal/common/mss_hart_ints.h"

#define USB_DMA_PLIC_PRIORITY                           2u
#define USB_MC_PLIC_PRIORITY                            2u

/* Global variables */
volatile uint8_t mouse_update = 0u;

typedef struct _HID_MOUSE_Data
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t button;
}
HID_MOUSE_Data_TypeDef;

HID_MOUSE_Data_TypeDef HID_MOUSE_Data;

void USB_DEV_attached(mss_usb_device_speed_t speed);
void USB_DEV_dettached(void);
void USB_DEV_oc_error(void);
void USB_DEV_enumerated(mss_usbh_tdev_info_t tdev_info);
void USB_DEV_class_driver_assigned(void);
void USB_DEV_not_supported(int8_t error_code);
void USB_DEV_permanent_erro(void);
void HID_DEV_valid_config(void);
void HID_DEV_ready(void);
void HID_DEV_driver_released(void);
void HID_DEV_error(int8_t error_code);

static void update_mouse(void)
{
    mouse_update = 1u;
}

void HID_DEV_decode(uint8_t *data)
{
    HID_MOUSE_Data.button = data[0];

    HID_MOUSE_Data.x      = data[1];
    HID_MOUSE_Data.y      = data[2];
    HID_MOUSE_Data.z      = data[3];

    update_mouse();
}

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

mss_usbh_hid_user_cb_t MSS_USBH_HID_user_cb =
{
    HID_DEV_valid_config,
    HID_DEV_ready,
    HID_DEV_driver_released,
    HID_DEV_error,
    HID_DEV_decode
};

uint8_t hex_to_char(uint8_t nibble_value)
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

void display_hex_byte(uint8_t hex_byte)
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

/* Main function for the hart1(U54 processor).
 * Application code running on hart1 is placed here.
 */
void u54_1(void)
{
    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt. */
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

    /* Bring the MMUART1, Timer and USB out of Reset */
    (void) mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_USB, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_TIMER, (uint8_t) 1, PERIPHERAL_ON);

    MSS_UART_init( &g_mss_uart1_lo,
                   MSS_UART_115200_BAUD,
                   MSS_UART_DATA_8_BITS |
                   MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string (&g_mss_uart1_lo,
    "\n\rUSB HID Host Example. Displaying mouse co-ordinates on UART port\n\r");

    PLIC_init();

    PLIC_SetPriority(USB_DMA_PLIC, USB_DMA_PLIC_PRIORITY);
    PLIC_SetPriority(USB_MC_PLIC, USB_MC_PLIC_PRIORITY);

    PLIC_EnableIRQ(USB_DMA_PLIC);
    PLIC_EnableIRQ(USB_MC_PLIC);

    /*Initialize SysTick*/
    SysTick_Config();

    __enable_irq();
    /*Initialize the USBH driver*/
    MSS_USBH_init(&MSS_USBH_user_cb);

    /*Initialize the USBH-HID driver*/
    MSS_USBH_HID_init(&MSS_USBH_HID_user_cb);

    /*
     * Get Class driver handle from the USBH-HID class driver and register it
     * with the USBH driver.
     * On Device attachment, the USBH driver examines the device descriptors to
     * obtain information about the device. It then uses this information to
     * match the device with the appropriate class driver, establishing a handle
     * for further communication and interaction with the device.
     */
    MSS_USBH_register_class_driver(MSS_USBH_HID_get_handle());

    while (1)
    {
        if (mouse_update)
        {
            mouse_update = 0;
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                      (uint8_t *)"\r\n Button:");
            display_hex_byte(HID_MOUSE_Data.button);
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\r\n X:");
            display_hex_byte(HID_MOUSE_Data.x);
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\r\n Y:");
            display_hex_byte(HID_MOUSE_Data.y);
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\r\n");
        }
    }
}

void USB_DEV_attached(mss_usb_device_speed_t speed)
{
    if (speed == MSS_USB_DEVICE_HS)
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\n\rHS ");
    if (speed == MSS_USB_DEVICE_FS)
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\n\rFS ");
    if (speed == MSS_USB_DEVICE_LS)
        MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"\n\rLS ");

    MSS_UART_polled_tx_string(&g_mss_uart1_lo,(uint8_t *)"Device detected\n\r");
}

void USB_DEV_dettached(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                              (uint8_t *)"\n\n\rUSB Device removed\n");
}

void USB_DEV_oc_error(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                              (uint8_t *)"\n\n\rOverCurrent error");
}

void USB_DEV_enumerated(mss_usbh_tdev_info_t tdev_info)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                              (uint8_t *)"\n\n\rEnumeration successful\n");
}

void USB_DEV_class_driver_assigned(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                              (uint8_t *)"\n\n\rHID Class driver assigned");
}

void USB_DEV_not_supported(int8_t error_code)
{
    switch (error_code)
    {
        case FS_DEV_NOT_SUPPORTED:
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                      (uint8_t *)"\n\rFS Device Not Supported");
            break;

        case LS_DEV_NOT_SUPPORTED:
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                      (uint8_t *)"\n\rLS Device Not Supported");
            break;

        case HUB_CLASS_NOT_SUPPORTED:
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                      (uint8_t *)"\n\rHUB Not Supported");
            break;

        case CLASS_DRIVER_NOT_MATCHED:
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                      (uint8_t *)"\n\rClass Not Supported");
            break;

        case CLASS_DRIVER_NOT_FREE:
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                      (uint8_t *)"\n\rClass driver not free");
            break;

        case CONF_DESC_POWER_LIM_EXCEED:
            MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                      (uint8_t *)"\n\rDevice needs more power");
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
                                      (uint8_t *)"Device not supported");
            break;
            
        default:
            break;
    }
}

void USB_DEV_permanent_erro(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
               (uint8_t *)"\n\rPermanent error, try reseting Polarfire SoC...");
}

void HID_DEV_valid_config(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                             (uint8_t *)"\n\n\rConfiguring HID device...");
}

void HID_DEV_ready(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                            (uint8_t *)"\n\n\r HID device ready for transfers");
}

void HID_DEV_driver_released(void)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                              (uint8_t *)"\n\n\rUSBH HID driver released\n\r");
}

void HID_DEV_error(int8_t error_code)
{
    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                              (uint8_t *)"\n\n\rHID device error");
}

void U54_1_sysTick_IRQHandler(void)
{
    /*
     * This function must be called. This function provides the time stamp
     * which is used to keep track of activities such as USB Reset, settling
     * time etc.in enumeration process.
     * The USBH driver and USBH-HID class driver housekeeping task is also
     * run based on this timestamp.
     * This prevents the application from having to wait for the enumeration
     * process to complete, which can potentially take several hundred
     * milliseconds, depending on the target device.
     * You may choose a general purpose timer instead of Systick.
     */
    MSS_USBH_task();
    MSS_USBH_HID_task();
    MSS_USBH_1ms_tick();
}

/* hart1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{

}
