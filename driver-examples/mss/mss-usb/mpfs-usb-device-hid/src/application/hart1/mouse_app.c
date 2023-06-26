/***************************************************************************//**
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * USB HID Class Mouse Device example application to demonstrate the PolarFire
 * SoC MSS USB operations in device mode.
 *
 * This file uses MSS USB Driver stack (inclusive of USBD-HID class driver) to
 * connect to USB Host as USB HID Mouse device.
 *
 */

#include "mouse_app.h"
#include "drivers/mss/mss_gpio/mss_gpio.h"
#include "drivers/mss/mss_usb/mss_usb_device.h"
#include "drivers/mss/mss_usb/mss_usb_device_hid.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DELAY_BETWEEN_SENSES                            50000u

/*This buffer is passed to the USB driver. When USB drivers are configured to
use internal DMA, the address of this buffer must be modulo-4.Otherwise DMA
Transfer will fail.*/
mss_usbd_hid_report_t report __attribute__ ((aligned (4))) = {0};

/* USB descriptors for HID class mouse enumeration. */
extern mss_usbd_user_descr_cb_t hid_mouse_descriptors_cb;

void
MOUSE_task
(
    void
)
{
    volatile uint32_t delay;

    if(1 == MSS_USBD_HID_tx_done())
    {
        MSS_USBD_HID_tx_report ((uint8_t*)&report, sizeof(report));
    }
    else
    {
        for(delay = 0; delay < DELAY_BETWEEN_SENSES; ++delay)
        {
            ;
        }

        if(report.x_move < (int8_t)30)
        {
            ++report.x_move;
        }
        else
        {
            report.x_move = -30;
        }
    }
}

void
MOUSE_init
(
void
)
{
    /* Assign call-back function handler structure needed by USB Device Core
     * driver
     */
    MSS_USBD_set_descr_cb_handler(&hid_mouse_descriptors_cb);

    /* Initialize HID Class driver. */
    MSS_USBD_HID_init(MSS_USB_DEVICE_HS);

    /* Initialize USB Device Core driver. */
    MSS_USBD_init(MSS_USB_DEVICE_HS);
}

#ifdef __cplusplus
}
#endif
