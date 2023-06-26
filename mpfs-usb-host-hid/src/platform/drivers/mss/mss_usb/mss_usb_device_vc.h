/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_usb_device_vc.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) USB Driver Stack
 *         USB Logical Layer (USB-LL)
 *           USBD Video class driver
 *
 * USBD Video class driver public API.
 *
 */

/*=========================================================================*//**
  @mainpage PolarFire SoC MSS USB driver (USBD-VC)
                USB Logical Layer (USB-LL)
                    USBD Video class driver

  @section intro_sec Introduction
  The Video Class device driver implements the USB Video device as specified by
  the USB-IF. This driver enables easy implementation of video driver
  functionality on the PolarFire SoC devices.

  This driver implements the video class using isochronous high bandwidth
  (ISO HB) data transfer. High speed isochronous transfers have an extended
  control system to allow up to 3 isochronous transactions per microframe. The
  specification refers to an isochronous endpoint with more than 1 packet per
  microframe as a 'High Bandwidth' endpoint.

  @section theory_op Theory of Operation
  The following steps are involved in the operation of the USBD-VC driver:
        •    Configuration
        •    Initialization
        •    Enumeration
        •    Class Specific requests
        •    data transfer

  Configuration
  To use this driver, the MSS USB driver must first be configured in the USB
  device mode using the MSS_USB_PERIPHERAL_MODE. No other configuration is
  necessary.

  Initialization
  The UVC class driver must be initialized using the MSS_USBD_UVC_init()
  function. Once initialized, this driver gets configured by the USBD driver
  during the enumeration process. The call-back function UVC_init_cb() is
  implemented by this driver which will be called by the USBD driver when the
  host configures this device. The UVC_get_descriptor_cb() function is
  implemented to provide the class specific descriptor table to the USBD driver.

  This driver defines a descriptor table which contains a configuration
  descriptor, interface descriptor, video class-specific descriptors and
  endpoint descriptors for successful enumeration as a video class device.

  Note: For successful enumeration, the device specific descriptors must also be
  provided by the application using the MSS_USBD_set_descr_cb_handler()
  function to the USBD Driver. Since the device descriptor, string descriptors
  etc. are not class specific, they are not part of the video class driver.

  Class Specific requests
  The UVC_process_request_cb() call-back function is implemented in this driver
  to process video class specific requests.

  Data transfer
  The Video class driver performs the data transfers using one ISOCHRONOUS IN
  endpoint and one ISOCHRONOUS OUT endpoint. The data transfers follow the
  Isochronous transfer. This driver implements the UVC_tx_complete_cb() function
  to get the information on data transfer events on the USB bus which are called
  by the USBD Driver.

*//*==========================================================================*/

#ifndef __MSS_USB_DEVICE_MSD_H_
#define __MSS_USB_DEVICE_MSD_H_

#include <stdint.h>
#include "mss_usb_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MSS_USB_DEVICE_ENABLED

/*******************************************************************************
 * Still Image frame details.
 */

/* Packets per microframe */
#define USB_VIDEO_CAMERA_PACKET_PER_UFRAME           3

/* Maximum packet size per microframe */
#define USB_VIDEO_CAMERA_PACKET_SIZE                 (unsigned int)(1024)

/* Image/Video resolution */
#define USB_VIDEO_CAMERA_FRAME_WIDTH                 (unsigned int)640
#define USB_VIDEO_CAMERA_FRAME_HEIGHT                (unsigned int)480
#define USB_VIDEO_CAMERA_FRAME_DATA_BITS             (unsigned int)16
#define PAYLOAD_HEADER_LENGTH                        (12u)
#define CAM_FPS                                      25u

#define PAYLOAD_SIZE          (USB_VIDEO_CAMERA_PACKET_SIZE * \
                                    USB_VIDEO_CAMERA_PACKET_PER_UFRAME)

#define MIN_BIT_RATE          (unsigned long)(USB_VIDEO_CAMERA_FRAME_WIDTH * \
                                            USB_VIDEO_CAMERA_FRAME_HEIGHT * \
                                            USB_VIDEO_CAMERA_FRAME_DATA_BITS * \
                                            CAM_FPS)

#define MAX_BIT_RATE          (unsigned long)(USB_VIDEO_CAMERA_FRAME_WIDTH * \
                                            USB_VIDEO_CAMERA_FRAME_HEIGHT * \
                                            USB_VIDEO_CAMERA_FRAME_DATA_BITS * \
                                            CAM_FPS)

#define MAX_FRAME_SIZE        (unsigned long)(USB_VIDEO_CAMERA_FRAME_WIDTH * \
                                              USB_VIDEO_CAMERA_FRAME_HEIGHT * 2)

#define INTERVAL              (unsigned long)(10000000/CAM_FPS)

#define IMAGE_SIZE            MAX_FRAME_SIZE

/*******************************************************************************
 * User Descriptor lengths
 */

/* Full configuration descriptor length*/
#define FULL_CONFIG_DESCR_LENGTH                (USB_STD_CONFIG_DESCR_LEN + \
                                                USB_STD_INTERFACE_DESCR_LEN + \
                                                USB_STD_ENDPOINT_DESCR_LEN + \
                                                USB_STD_ENDPOINT_DESCR_LEN )

/***************************************************************************//**
 Exported Types from USBD-VC class driver
 */
/***************************************************************************//**
  mss_usbd_uvc_state_t
  The mss_usbd_uvc_state_t provides a type to identify the current state of the
  Video class driver.
    USBD_UVC_NOT_CONFIGURED – The USB UVC class driver is not configured and it
                              cannot perform the data transfers.
    USBD_UVC_CONFIGURED     - The USB UVC class driver is configured by the host
                              and it can perform the data transfers.
*/
typedef enum mss_usbd_uvc_state {
    USBD_UVC_NOT_CONFIGURED,
    USBD_UVC_CONFIGURED
}mss_usbd_uvc_state_t;

/***************************************************************************//**
 mss_usbd_uvc_app_cb_t
  The mss_usbd_uvc_app_cb_t provides the prototype for all the call-back
  functions which must be implemented by the user application. The user
  application must define and initialize a structure of this type and provide
  the address of that structure as parameter to the MSS_USBD_UVC_init() function.

  usb_uvc_init
  The function pointed by the usb_uvc_init function pointer will be called to
  indicate that the video driver is configured by the USB host. The application
  can use this function to prepare itself for data exchange with the host.
  This driver cannot perform data transfers unless it is configured by the USB
  host to do so.

  usb_uvc_release
  The function pointed by the usb_uvc_release element will be called when the
  device is disconnected from the host. The disconnect event is detected by the
  USBD driver and is communicated to the class driver using this element by
  passing the parameter cfgidf = 0xFF. The application can use this call-back
  function and its parameter to take appropriate action as required.

  usb_uvc_process_request
  The function pointed by the usb_uvc_process_request element will be called
  when the USB device receives class specific request from USB host. The
  parameter setup_pkt is the pointer to the setup packet sent by the host. The
  parameters buf_p and length are used for the data which might be associated
  with the current setup packet. If the host requests data from the device, the
  USBD-Class driver must provide the address of the buffer containing the data
  in buf_p parameter and the length of this buffer in bytes in length parameter.
  If the host wants to send data to the device then USBD-Class driver must
  provide the address of the buffer where the data must be placed in buf_p
  parameter and the size of the buffer in bytes in the length parameter. For the
  zero data length request the buf_p and the length parameters are not
  meaningful.

  usb_uvc_tx_complete
  The function used to get status of data delivery.
*/

typedef struct mss_usbd_uvc_app_cb {
    void (*usb_uvc_init)(void);
    void (*usb_uvc_release)(uint8_t cfgidx);
    uint8_t (*usb_uvc_process_request)(mss_usbd_setup_pkt_t* setup_pkt,
                                       uint8_t** buf,
                                       uint32_t* length);
    uint8_t(*usb_uvc_tx_complete)(uint8_t status);
    uint8_t(*usb_uvc_rx)(uint8_t status, uint32_t rx_count);
    void(*usb_uvc_notification)(uint8_t** buf_p, uint32_t* length_p);
    uint8_t(*usb_uvc_cep_tx_complete)(uint8_t status);
    uint8_t(*usb_uvc_cep_rx)(uint8_t status);
} mss_usbd_uvc_app_cb_t;

/***************************************************************************//**
 Exported functions from USBD-UVC class driver
 */
void
MSS_USBD_UVC_init
(
    mss_usbd_uvc_app_cb_t* media_ops,
    mss_usb_device_speed_t speed
);

/***************************************************************************//**
 Exported functions from USBD-UVC class driver
 */
void
MSS_USBD_UVC_tx
(
    uint8_t* buf,
    uint32_t length
);

/***************************************************************************//**
  @brief MSS_USBD_UVC_get_state()
  The MSS_USBD_UVC_get_state() function can be used in the application to find
  out the current state of the video class driver.

  @param media_ops
    This function does not take a parameter.

  @return
    This function returns a value of type mss_usbd_uvc_state_t indicating the
    current state of the Video class driver.

*/
mss_usbd_uvc_state_t
MSS_USBD_UVC_get_state
(
    void
);

#endif  /* MSS_USB_DEVICE_ENABLED */

#ifdef __cplusplus
}
#endif

#endif  /* __MSS_USB_DEVICE_MSD_H_ */
