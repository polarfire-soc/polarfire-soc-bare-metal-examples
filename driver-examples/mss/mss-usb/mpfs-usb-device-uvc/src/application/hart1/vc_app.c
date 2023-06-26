/***************************************************************************//**
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * USB Video Class Device example application to demonstrate the
 * PolarFire SoC MSS USB operations in device mode.
 *
 * Drivers used:
 * This file uses MSS USB Driver stack (inclusive of USBD-VC class driver) to
 * connect to USB Host as USB Camera device.
 */
#include <stdio.h>
#include <string.h>
#include "vc_app.h"
#include "drivers/mss/mss_usb/mss_usb_device.h"
#include "drivers/mss/mss_usb/mss_usb_device_vc.h"
#include <drivers/mss/mss_gpio/mss_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
  Local Macros
*/

#define min(a,b) (a<b)?(a):(b)
/******************************************************************************
  Static variables
*/

static uint32_t tosend;
static uint32_t expectedPixels      = IMAGE_SIZE;
static uint32_t picture_index       = 0u;
static uint8_t packet[PAYLOAD_SIZE] = {0x00};
static uint8_t frame_toggle         = 0u;
static uint8_t data_delivered       = 0u;

/******************************************************************************
  extern variables and structures
*/
extern const uint8_t yuv_picture[IMAGE_SIZE];
extern mss_usbd_user_descr_cb_t vc_descriptors_cb;

/******************************************************************************
  Private function declarations
*/
static void app_usb_uvc_init(void);
static void app_usb_uvc_release(uint8_t cfgidx);
static uint8_t app_usb_uvc_process_request(mss_usbd_setup_pkt_t *setup_pkt,
                                            uint8_t **buf, uint32_t *length);
static uint8_t app_usb_uvc_tx_complete(uint8_t status);
static uint8_t app_usb_uvc_rx(uint8_t status, uint32_t rx_count);
static void  app_usb_uvc_notification(uint8_t** buf_p, uint32_t* length_p);
static uint8_t app_usb_uvc_cep_tx_complete(uint8_t status);
static uint8_t app_usb_uvc_cep_rx(uint8_t status);

/* Implementation of mss_usbd_uvc_app_cb_t needed by USB UVC Class Driver */
mss_usbd_uvc_app_cb_t usb_uvc_app = {
    app_usb_uvc_init,
    app_usb_uvc_release,
    app_usb_uvc_process_request,
    app_usb_uvc_tx_complete,
    app_usb_uvc_rx,
    app_usb_uvc_notification,
    app_usb_uvc_cep_tx_complete,
    app_usb_uvc_cep_rx
};

/******************************************************************************
  Function:
    void APP_Init ( void )

  Remarks:
    See prototype in app.h.
*/
void APP_Init (void )
{
    uvc_init();
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks (void )
{
    uint8_t frame = 0u;
    if (1u == data_delivered)
    {
        data_delivered = 0;

        /* Transmits single frame */
        if (expectedPixels > 0)
        {
            tosend = min((IMAGE_SIZE - picture_index), (PAYLOAD_SIZE - PAYLOAD_HEADER_LENGTH));
            memcpy(packet + PAYLOAD_HEADER_LENGTH, yuv_picture + picture_index, tosend);

            expectedPixels -= tosend;
            picture_index = picture_index + tosend;

            packet[0] = 0x0c;  /* header length */
            packet[2] = 0xef;  /* PTS(Presentation Time Stamp) [7:0] */
            packet[3] = 0x21;  /* PTS [15:8]  */
            packet[4] = 0xb1;  /* PTS [23:16] */
            packet[5] = 0xf0;  /* PTS [31:24] */
            packet[6] = 0xef;  /* SCR(Source Clock Reference) [7:0]*/
            packet[7] = 0x21;  /* SCR [15:8]  */
            packet[8] = 0xb1;  /* SCR [23:16] */
            packet[9] = 0xf0;  /* SCR [31:24] */
            packet[10] = 0xef; /* SCR [39:32] */
            packet[11] = 0x21; /* SCR [47:40] */

            /* Last packet - need to configure back */
            if (tosend < (PAYLOAD_SIZE- PAYLOAD_HEADER_LENGTH))
            {
                if(frame)
                {
                    packet[1] = 0x8e; /* toggle frame bit */
                }
                else
                {
                    packet[1] = 0x8f; /* toggle frame bit */
                }

                picture_index = 0;
                frame_toggle ^= 1;
                frame = ~frame;
                expectedPixels = IMAGE_SIZE;
            }
            else
            {
                if(frame)
                {
                    packet[1] = 0x8c; //toggle frame bit
                }
                else
                {
                    packet[1] = 0x8d; //toggle frame bit
                }
            }

            MSS_USBD_UVC_tx((void *)packet, tosend + PAYLOAD_HEADER_LENGTH);
        }
    }
}

/******************************************************************************
  See vc_app.h for details of how to use this function.
*/
void
uvc_init
(
    void
)
{
    /* Initialize USB driver */
    MSS_USBD_init(MSS_USB_DEVICE_HS);

    /* Assign call-back function Interface needed by USBD driver */
    MSS_USBD_set_descr_cb_handler(&vc_descriptors_cb);

    /* Assign call-back function handler structure needed by MSD class driver */
    MSS_USBD_UVC_init(&usb_uvc_app, MSS_USB_DEVICE_HS);

    HAL_enable_interrupts();
}

/******************************************************************************
  Local function definitions
*/
static void app_usb_uvc_init(void)
{
    /* Read the first packet from Image buffer. */
    data_delivered = 1;
}

static void  app_usb_uvc_release(uint8_t cfgidx)
{
    
}

static uint8_t  app_usb_uvc_process_request(mss_usbd_setup_pkt_t* setup_pkt,
                                   uint8_t** buf,
                                   uint32_t* length)
{
    
}

static uint8_t app_usb_uvc_tx_complete(uint8_t status)
{
    data_delivered = 1;
}

static uint8_t app_usb_uvc_rx(uint8_t status, uint32_t rx_count)
{
    
}

static void app_usb_uvc_notification(uint8_t** buf_p, uint32_t* length_p)
{
    
}

static uint8_t app_usb_uvc_cep_tx_complete(uint8_t status)
{
    
}

static uint8_t app_usb_uvc_cep_rx(uint8_t status)
{

}

#ifdef __cplusplus
}
#endif
