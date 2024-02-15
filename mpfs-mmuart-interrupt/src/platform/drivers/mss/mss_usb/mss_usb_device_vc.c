/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_usb_device_vc.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) USB Driver Stack
 *          USB Logical Layer (USB-LL)
 *            USBD Video class driver
 *
 * USBD Video class driver implementation:
 * This source file implements USB video class functionality.
 *
 */

#include "mss_usb_device_vc.h"
#include "mss_usb_device.h"
#include "mss_usb_std_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MSS_USB_DEVICE_ENABLED 

#define UV_CLASS_INTERFACE_NUM                          0x00

/* MSS USB core Endpoints used by UV class */
#define UVCP_BULK_RX_EP                                 MSS_USB_RX_EP_1
#define UVCP_BULK_TX_EP                                 MSS_USB_TX_EP_1
#define UVC_ISO_HB_RX_EP                                MSS_USB_RX_EP_1
#define UVC_ISO_HB_TX_EP                                MSS_USB_TX_EP_1

/* MSS USB core FIFO address for endpoints used by UV class */
#define UV_ISO_HB_RX_EP_FIFO_ADDR                       0xD00u
#define UV_ISO_HB_TX_EP_FIFO_ADDR                       0x100u

/* MSS USB core FIFO size for endpoints used by UV class */
#define UV_ISO_HB_TX_EP_FIFO_SIZE                       (1024u * 3)
#define UV_ISO_HB_RX_EP_FIFO_SIZE                       (1024u * 3)

/* Maximum packet size for endpoints used by UV class */
#define VCP_ISO_HB_TX_EP_MAX_PKT_SIZE                   (1024u * 3)
#define VCP_ISO_HB_RX_EP_MAX_PKT_SIZE                   (1024u * 3)

#define USB_TRANSFER_TYPE_ISOCHRONOUS                   0x01

#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)
#define DBVAL(x) (x & 0xFF),((x >> 8) & 0xFF),((x >> 16) & 0xFF),((x >> 24) & 0xFF)

/* USB2.0 standard request types */
#define USB_REQUEST_TYPE_DTH_CLASS_INTERFACE            0xA1
#define USB_REQUEST_TYPE_HTD_CLASS_INTERFACE            0x21

/* clear or disable a specific feature */
#define CLEAR_FEATURE                                   0x01

/* get status with self powered(SP) bit */
#define GET_STATUS_SP                                   0x81

/* get status with remote wakeup(RW) bit */
#define GET_STATUS_RW                                   0x82

/* get status with self powered(SP) and remote wakeup(RW) bits */
#define GET_STATUS_RW_SP                                0x83

/* video probe control request */
#define GET_REQUEST_DEF                                 0x87

#define BCDUSB_NUMBER_OF_USB_1_DOT_0                    0x0100
#define BCDUSB_NUMBER_OF_USB_2_DOT_0                    0x0200

typedef struct _VideoControl{
    uint8_t    bmHint[2];                      /* 2 */
    uint8_t    bFormatIndex[1];                /* 3 */
    uint8_t    bFrameIndex[1];                 /* 4 */
    uint8_t    dwFrameInterval[4];             /* 8 */
    uint8_t    wKeyFrameRate[2];               /* 10 */
    uint8_t    wPFrameRate[2];                 /* 12 */
    uint8_t    wCompQuality[2];                /* 14 */
    uint8_t    wCompWindowSize[2];             /* 16 */
    uint8_t    wDelay[2];                      /* 18 */
    uint8_t    dwMaxVideoFrameSize[4];         /* 22 */
    uint8_t    dwMaxPayloadTransferSize[4];    /* 26 */
    uint8_t    dwClockFrequency[4];
    uint8_t    bmFramingInfo[1];
    uint8_t    bPreferedVersion[1];
    uint8_t    bMinVersion[1];
    uint8_t    bMaxVersion[1];
}VideoControl;

/* data array for Video Probe and Commit */
VideoControl videoCommitControl =
{
    {0x00,0x00},                      /* bmHint */
    {0x01},                           /* bFormatIndex */
    {0x01},                           /* bFrameIndex */
    {DBVAL(INTERVAL)},                /* dwFrameInterval */
    {0x00,0x00,},                     /* wKeyFrameRate */
    {0x00,0x00,},                     /* wPFrameRate */
    {0x00,0x00,},                     /* wCompQuality */
    {0x00,0x00,},                     /* wCompWindowSize */
    {0x00,0x00},                      /* wDelay */
    {DBVAL(MAX_FRAME_SIZE)},          /* dwMaxVideoFrameSize */
    {0x00, 0x00, 0x00, 0x00},         /* dwMaxPayloadTransferSize */
    {0x00, 0x00, 0x00, 0x00},         /* dwClockFrequency */
    {0x00},                           /* bmFramingInfo */
    {0x00},                           /* bPreferedVersion */
    {0x00},                           /* bMinVersion */
    {0x00},                           /* bMaxVersion */
};

VideoControl videoProbeControl =
{
    {0x00,0x00},                      /* bmHint */
    {0x01},                           /* bFormatIndex */
    {0x01},                           /* bFrameIndex */
    {DBVAL(INTERVAL)},                /* dwFrameInterval */
    {0x00,0x00,},                     /* wKeyFrameRate */
    {0x00,0x00,},                     /* wPFrameRate */
    {0x00,0x00,},                     /* wCompQuality */
    {0x00,0x00,},                     /* wCompWindowSize */
    {0x00,0x00},                      /* wDelay */
    {DBVAL(MAX_FRAME_SIZE)},          /* dwMaxVideoFrameSize */
    {0x00, 0x00, 0x00, 0x00},         /* dwMaxPayloadTransferSize */
    {0x00, 0x00, 0x00, 0x00},         /* dwClockFrequency */
    {0x00},                           /* bmFramingInfo */
    {0x00},                           /* bPreferedVersion */
    {0x00},                           /* bMinVersion */
    {0x00},                           /* bMaxVersion */
};

/*****************************************************************************/
/* Full configuration descriptor length. */
#define UVC_CONF_DESCR_DESCTYPE_IDX                     1u

/* Definition for UVC application call-backs. */
mss_usbd_uvc_app_cb_t * usbd_uvc_vcp_ops;

/* USB current Speed of operation selected by user*/
mss_usb_device_speed_t g_usbd_uvc_user_speed;

/*
 * This variable tracks the current state of the USBD Video Class driver.
 */
static mss_usbd_uvc_state_t g_usbd_uvc_state = USBD_UVC_NOT_CONFIGURED;

/***************************************************************************//**
 Implementations of Call-back functions used by USBD.
 */
static uint8_t* UVC_get_descriptor_cb(uint8_t recepient,
                                      uint8_t type,
                                      uint32_t* length,
                                      mss_usb_device_speed_t musb_speed);

static uint8_t UVC_init_cb(uint8_t cfgidx, mss_usb_device_speed_t musb_speed);
static uint8_t UVC_release_cb(uint8_t cfgidx);

static uint8_t UVC_process_request_cb(mss_usbd_setup_pkt_t* setup_pkt,
                                      uint8_t** buf_pp,
                                      uint32_t* length);

static uint8_t UVC_tx_complete_cb(mss_usb_ep_num_t num, uint8_t status);
static uint8_t UVC_rx_cb(mss_usb_ep_num_t num, uint8_t status, uint32_t rx_count);
static uint8_t UVC_cep_tx_done_cb(uint8_t status);
static uint8_t UVC_cep_rx_done_cb(uint8_t status);

mss_usbd_class_cb_t usbd_uvc_class_cb = {
    UVC_init_cb,
    UVC_release_cb,
    UVC_get_descriptor_cb,
    UVC_process_request_cb,
    UVC_tx_complete_cb,
    UVC_rx_cb,
    UVC_cep_tx_done_cb,
    UVC_cep_rx_done_cb
};

uint8_t uvc_fs_conf_descr[FULL_CONFIG_DESCR_LENGTH] =
{
    /*----------------------- Configuration Descriptor -----------------------*/
    USB_STD_CONFIG_DESCR_LEN,                    /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,           /* bDescriptorType */
    FULL_CONFIG_DESCR_LENGTH,                    /* wTotalLength LSB */
    0x00u,                                       /* wTotalLength MSB */
    0x01u,                                       /* bNumInterfaces */
    0x01u,                                       /* bConfigurationValue */
    0x04u,                                       /* iConfiguration */
    0xC0u,                                       /* bmAttributes */
    0x32u,                                       /* bMaxPower */
    /*------------------------- Interface Descriptor -------------------------*/
    USB_STD_INTERFACE_DESCR_LEN,                 /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,               /* bDescriptorType */
    UV_CLASS_INTERFACE_NUM,                      /* bInterfaceNumber */
    0x00u,                                       /* bAlternateSetting */
    0x02u,                                       /* bNumEndpoints */
    0x08u,                                       /* bInterfaceClass */
    0x06u,                                       /* bInterfaceSubClass */
    0x50u,                                       /* bInterfaceProtocol */
    0x05u,                                       /* bInterface */
    /*------------------------- IN Endpoint Descriptor -----------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                  /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                /* bDescriptorType */
    (0x80u | UVC_ISO_HB_TX_EP),                  /* bEndpointAddress */
    0x02u,                                       /* bmAttributes */
    0x40u,                                       /* wMaxPacketSize LSB */
    0x00u,                                       /* wMaxPacketSize MSB */
    0xFFu,                                       /* bInterval */
    /*------------------------- OUT Endpoint Descriptor ----------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                  /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                /* bDescriptorType */
    UVC_ISO_HB_RX_EP,                            /* bEndpointAddress */
    0x02u,                                       /* bmAttributes */
    0x40u,                                       /* wMaxPacketSize LSB */
    0x00u,                                       /* wMaxPacketSize MSB */
    0xFFu                                        /* bInterval *//*Max NAK rate*/
};

uint8_t uvc_conf_descr[] = {
    /*------------ Configuration Descriptor ----------------------------------*/
    0x09,                                 /* Descriptor Size */
    0x02,                                 /* Configuration Descriptor Type */
    0xA7,0x00,                            /* Length of this descriptor and all 
                                           sub descriptors */
    0x02,                                 /* Number of interfaces */
    0x01,                                 /* Configuration number */
    0x00,                                 /* Configuration string index */
    0x80,                                 /* Config characteristics - Bus 
                                           powered */
    0xFA,                                 /* Max power consumption of device 
                                           (in 2mA unit) : 500mA */
    /*------------ Interface Association Descriptor --------------------------*/
    0x08,                                 /* Descriptor Size */
    11,                                   /* Interface Association Descriptor 
                                           Type: 11 */
    0x00,                                 /* I/f number of first VideoControl 
                                           i/f */
    0x02,                                 /* Number of Video i/f */
    0x0E,                                 /* CC_VIDEO : Video i/f class code */
    0x03,                                 /* SC_VIDEO_INTERFACE_COLLECTION : 
                                           Subclass code */
    0x00,                                 /* Protocol : Not used */
    0x00,                                 /* String desc index for interface */
    /*------------ Standard Video Control Interface Descriptor ---------------*/
    0x09,                                 /* Descriptor size */
    0x04,                                 /* Interface Descriptor type */
    0x00,                                 /* Interface number */
    0x00,                                 /* Alternate setting number */
    0x00,                                 /* Number of end points */
    0x0E,                                 /* CC_VIDEO : Interface class */
    0x01,                                 /* CC_VIDEOCONTROL : Interface sub 
                                           class */
    0x00,                                 /* Interface protocol code */
    0x00,                                 /* Interface descriptor string index*/
    /*------------ Class specific VC Interface Header Descriptor  ------------*/
    0x0D,                                 /* Descriptor size */
    0x24,                                 /* Class Specific I/f Header 
                                           Descriptor type */
    0x01,                                 /* Descriptor Sub type : VC_HEADER */
    0x00,0x01,                            /* Revision of class spec : 1.0 */
    0x27,0x00,                            /* Total Size of class specific 
                                           descriptors (till Output terminal) */
    0x80,0x8D,0x5B,0x00,                  /* Clock frequency:48MHz(Deprecated)*/
    0x01,                                 /* Number of streaming interfaces */
    0x01,                                 /* Video streaming I/f 1 belongs to 
                                           VC i/f */
    /*------------ Input (Camera) Terminal Descriptor Descriptor  ------------*/
    0x11,                                 /* Descriptor size */
    0x24,                                 /* Class specific interface desc type */
    0x02,                                 /* Input Terminal Descriptor type */
    0x01,                                 /* ID of this terminal */
    0x01,0x02,                            /* Camera terminal type */
    0x00,                                 /* No association terminal */
    0x00,                                 /* String desc index : Not used */
    0x00,0x00,                            /* No optical zoom supported */
    0x00,0x00,                            /* No optical zoom supported */
    0x00,0x00,                            /* No optical zoom supported */
    0x02,                                 /* Size of controls field for this 
                                           terminal : 2 bytes */
    0x00,0x00,                            /* No controls supported */
    /*----------- Output Terminal Descriptor ---------------------------------*/
    0x09,                                 /* Descriptor size */
    0x24,                                 /* Class specific interface desc type */
    0x03,                                 /* Output Terminal Descriptor type */
    0x02,                                 /* ID of this terminal */
    0x01,0x01,                            /* USB Streaming terminal type */
    0x00,                                 /* No association terminal */
    0x01,                                 /* Source ID : 1 : Connected to Extn 
                                           Unit */
    0x00,                                 /* String desc index : Not used */
    /* Video Streaming (VS) Interface Descriptor */
    /* Standard VS Interface Descriptor  = interface 1 */
    /* alternate setting 0 = Zero Bandwidth */
    0x09,                                 /* Descriptor size */
    0x04,                                 /* Interface Descriptor type */
    0x01,                                 /* Interface number */
    0x00,                                 /* Alternate setting number */
    0x00,                                 /* Number of end points : Zero 
                                           Bandwidth */
    0x0E,                                 /* Interface class : CC_VIDEO */
    0x02,                                 /* Interface sub class : 
                                           CC_VIDEOSTREAMING */
    0x00,                                 /* Interface protocol code: Undefined */
    0x00,                                 /* Interface descriptor string index */
    /* Class-specific Video Streaming Input Header Descriptor */
    0x0E,                                 /* Descriptor size */
    0x24,                                 /* Class-specific VS I/f Type */
    0x01,                                 /* Descriptotor Subtype : Input Header */
    0x01,                                 /* 1 format desciptor follows */
    0x4D,0x00,                            /* Total size of Class specific VS 
                                           descr: 41 Bytes */
    0x81,                                 /* EP address for BULK video data */
    0x00,                                 /* No dynamic format change supported */
    0x02,                                 /* Output terminal ID : 4 */
    0x00,                                 /* Still image capture method 1 
                                           supported */
    0x01,                                 /* Hardware trigger NOT supported */
    0x00,                                 /* Hardware to initiate still image 
                                           capture NOT supported */
    0x01,                                 /* Size of controls field : 1 byte */
    0x00,                                 /* D2: Compression quality supported */
    /* Class specific Uncompressed VS format descriptor */
    0x1B,                                 /* Descriptor size */
    0x24,                                 /* Class-specific VS I/f Type */
    0x04,                                 /* Subtype : uncompressed format I/F */
    0x01,                                 /* Format desciptor index (only one 
                                           format is supported) */
    0x01,                                 /* number of frame descriptor followed */
    0x59,0x55,0x59,0x32,                  /* GUID, globally unique identifier 
                                           used to identify streaming-encoding 
                                           format: YUY2  */
    0x00,0x00,0x10,0x00,
    0x80,0x00,0x00,0xAA,
    0x00,0x38,0x9B,0x71,
    0x10,                                 /* Number of bits per pixel used to 
                                           specify color in the decoded video 
                                           frame. 0 if not applicable: 10 bit 
                                           per pixel */
    0x01,                                 /* Optimum Frame Index for this 
                                           stream: 1 */
    0x00,                                 /* X dimension of the picture aspect 
                                           ratio: Non-interlaced in progressive 
                                           scan */
    0x00,                                 /* Y dimension of the picture aspect 
                                           ratio: Non-interlaced in progressive 
                                           scan*/
    0x00,                                 /* Interlace Flags: Progressive 
                                           scanning, no interlace */
    0x00,                                 /* duplication of the video stream 
                                           restriction: 0 - no restriction */
    /* Class specific Uncompressed VS Frame descriptor */
    0x1E,                                 /* Descriptor size */
    0x24,                                 /* Descriptor type*/
    0x05,                                 /* Subtype: uncompressed frame I/F */
    0x01,                                 /* Frame Descriptor Index */
    0x02,                                 /* Still image capture method 1 
                                           supported, fixed frame rate */
    WBVAL(USB_VIDEO_CAMERA_FRAME_WIDTH),  /* Width in pixel: 160-QVGA */
    WBVAL(USB_VIDEO_CAMERA_FRAME_HEIGHT), /* Height in pixel 120-QVGA */
    DBVAL(MIN_BIT_RATE),                  /* Min bit rate bits/s. Not specified, 
                                           taken from MJPEG */
    DBVAL(MAX_BIT_RATE),                  /* Max bit rate bits/s. Not specified, 
                                           taken from MJPEG */
    DBVAL(MAX_FRAME_SIZE),                /* Maximum video or still frame size 
                                           in bytes(Deprecated) */
    DBVAL(INTERVAL),                      /* Default Frame Interval */
    0x01,                                 /* Frame interval(Frame Rate) types: 
                                           Only one frame interval supported */
    DBVAL(INTERVAL),                      /* Shortest Frame Interval */
    /* Color Matching Descriptor */
    0x06,                                 /* bLength */
    0x24,                                 /* bDescriptorType : CS_INTERFACE */
    0x0D,                                 /* bDescriptorSubType: VS_COLORFORMAT */
    0x01,                                 /* bColorPrimarie : 1: BT.709, sRGB 
                                           (default) */
    0x01,                                 /* bTransferCharacteristics : 1: 
                                           BT.709 (default) */
    0x04,                                 /* bMatrixCoefficients : 1: BT. 709. */
    /* Standard Video Streaming Interface Descriptor (Alternate Setting 1) */
    0x09,                                 /* Descriptor size */
    0x04,                                 /* Interface Descriptor type */
    0x01,                                 /* Interface number */
    0x01,                                 /* Alternate setting number */
    0x01,                                 /* Number of end points : Zero 
                                           Bandwidth */
    0x0E,                                 /* Interface class : CC_VIDEO */
    0x02,                                 /* Interface sub class : 
                                           CC_VIDEOSTREAMING */
    0x00,                                 /* Interface protocol code: Undefined */
    0x00,                                 /* Interface descriptor string index */
    /* Endpoint Descriptor for isochronous Streaming Image Data */
    0x07,                                 /* Descriptor size */
    0x5,                                  /* Endpoint Descriptor Type */
    0x81,                                 /* Endpoint address and description */
    USB_TRANSFER_TYPE_ISOCHRONOUS,        /* Isochronous End point */
    0x00, 0x14,                           /* max packet size : 3 packets per 
                                           microframe */
    /* 0x00, 0x0C, */                     /* max packet size : 2 packets per 
                                           microframe */
    /* 0x00, 0x04, */                     /* max packet size : 1 packets per 
                                           microframe */
    0x01                                  /* Servicing interval for data 
                                           transfers */
};

/***************************************************************************//**
* See mss_usb_device_vc.h for details of how to use this function.
*/
void
MSS_USBD_UVC_init
(
    mss_usbd_uvc_app_cb_t* media_ops,
    mss_usb_device_speed_t speed
)
{
    usbd_uvc_vcp_ops = media_ops;
    g_usbd_uvc_user_speed = speed;

    MSS_USBD_set_class_cb_handler(&usbd_uvc_class_cb);
}

/***************************************************************************//**
* See mss_usb_device_vc.h for details of how to use this function.
*/
void
MSS_USBD_UVC_tx
(
    uint8_t* buf,
    uint32_t length
)
{
    g_usbd_uvc_state = 0u;
    MSS_USBD_tx_ep_write(UVC_ISO_HB_TX_EP, buf, length);
}

/***************************************************************************//**
 See mss_usb_device_vc.h for details of how to use this function.
 */
void
MSS_USBD_UVC_rx_prepare
(
    uint8_t* buf,
    uint32_t length
)
{
    MSS_USBD_rx_ep_read_prepare(UVC_ISO_HB_RX_EP, buf, length);
}

/***************************************************************************//**
 See mss_usb_device_vc.h for details of how to use this function.
 */
mss_usbd_uvc_state_t
MSS_USBD_UVC_get_state
(
    void
)
{
    return g_usbd_uvc_state;
}

/***************************************************************************//**
 returns the configuration descriptor requested by Host.
 */
static uint8_t*
UVC_get_descriptor_cb
(
    uint8_t recepient,
    uint8_t type,
    uint32_t* length,
    mss_usb_device_speed_t musb_speed
)
{
    if(USB_STD_REQ_RECIPIENT_DEVICE == recepient)
    {
        if(USB_CONFIGURATION_DESCRIPTOR_TYPE == type)
        {
            *length = sizeof(uvc_conf_descr);
            return(uvc_conf_descr);
        }
        else if (USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE == type)
        {
            *length = sizeof(uvc_conf_descr);
            return(uvc_conf_descr);
        }
    }
    else
    {
        /*Do nothing*/
    }

    return USB_FAIL;
}

/***************************************************************************//**
 UVC_init_cb() call-back is called by USB Device mode driver on receiving
 SET_CONFIGURATION command. The Video class specific configurations are 
 performed by this function.
 */
static uint8_t
UVC_init_cb
(
    uint8_t cfgidx,
    mss_usb_device_speed_t musb_speed
)
{
    uint8_t *buf_p;
    uint32_t len;

    MSS_USBD_tx_ep_configure(UVC_ISO_HB_TX_EP,
                             UV_ISO_HB_TX_EP_FIFO_ADDR,
                             UV_ISO_HB_TX_EP_FIFO_SIZE,
                             VCP_ISO_HB_TX_EP_MAX_PKT_SIZE,
                             1,
                             DMA_ENABLE,
                             MSS_USB_DMA_CHANNEL1,
                             MSS_USB_XFR_HB_ISO,
                             NO_ZLP_TO_XFR);

    MSS_USBD_rx_ep_configure(UVC_ISO_HB_RX_EP,
                             UV_ISO_HB_RX_EP_FIFO_ADDR,
                             UV_ISO_HB_RX_EP_FIFO_SIZE,
                             VCP_ISO_HB_RX_EP_MAX_PKT_SIZE,
                             1,
                             DMA_ENABLE,
                             MSS_USB_DMA_CHANNEL2,
                             MSS_USB_XFR_HB_ISO,
                             NO_ZLP_TO_XFR);

    g_usbd_uvc_state = USBD_UVC_CONFIGURED;

    MSS_USBD_set_isoupdate();

    MSS_USBD_clr_isoupdate();
    if(0 != usbd_uvc_vcp_ops->usb_uvc_init)
    {
        usbd_uvc_vcp_ops->usb_uvc_init();
    }

    if(0 != usbd_uvc_vcp_ops->usb_uvc_notification)
    {
        usbd_uvc_vcp_ops->usb_uvc_notification(&buf_p, &len);
    }

    return USB_SUCCESS;
}

/***************************************************************************//**
 UVC_release_cb() call-back is called by USB Device mode driver on 
 receiving a command to clear the configuration.
 */
static uint8_t
UVC_release_cb
(
    uint8_t cfgidx
)
{
    MSS_USB_CIF_rx_ep_disable_irq(UVC_ISO_HB_RX_EP);
    MSS_USB_CIF_rx_ep_clr_csrreg(UVC_ISO_HB_RX_EP);
    MSS_USB_CIF_dma_clr_ctrlreg(MSS_USB_DMA_CHANNEL2);

    MSS_USB_CIF_tx_ep_disable_irq(UVC_ISO_HB_TX_EP);
    MSS_USB_CIF_tx_ep_clr_csrreg(UVC_ISO_HB_TX_EP);
    MSS_USB_CIF_dma_clr_ctrlreg(MSS_USB_DMA_CHANNEL1);

    if(0 != usbd_uvc_vcp_ops->usb_uvc_release)
    {
        usbd_uvc_vcp_ops->usb_uvc_release(cfgidx);
    }

    g_usbd_uvc_state = USBD_UVC_NOT_CONFIGURED;

    return USB_SUCCESS;
}

/***************************************************************************//**
 UVC_process_request_cb() call-back function Process the Video class requests.
 */
static uint8_t
UVC_process_request_cb
(
    mss_usbd_setup_pkt_t* setup_pkt,
    uint8_t** buf_pp,
    uint32_t* length
)
{
    if(setup_pkt->request_type == USB_REQUEST_TYPE_DTH_CLASS_INTERFACE)
    {
        /* Zoom (Absolute) Control */
        if((setup_pkt->value == BCDUSB_NUMBER_OF_USB_1_DOT_0))
        {
            switch(setup_pkt->request)
            {
                case GET_STATUS_SP:
                  *buf_pp = (uint8_t*)&videoProbeControl;
                  *length = setup_pkt->length;
                break;

                case GET_STATUS_RW:
                  memset(&videoProbeControl,0x0,34);
                  *buf_pp = (uint8_t*)&videoProbeControl;
                  *length = setup_pkt->length;
                break;

                case GET_STATUS_RW_SP:
                   memset(&videoProbeControl,0xff,34);
                  *buf_pp = (uint8_t*)&videoProbeControl;
                  *length = setup_pkt->length;
                break;

                case GET_REQUEST_DEF:
                   memset(&videoProbeControl, 0xff, 34);
                  *buf_pp = (uint8_t*) &videoProbeControl;
                  *length = setup_pkt->length;
                break;

                default:
                break;
            }
        }

        if((setup_pkt->value == BCDUSB_NUMBER_OF_USB_2_DOT_0))
        {
              switch(setup_pkt->request)
              {
                  case GET_STATUS_SP:
                      *buf_pp = (uint8_t*)&videoCommitControl;
                      *length = setup_pkt->length;
                  break;

                  default:
                  break;
              }
        }
    }
    else if(setup_pkt->request_type == USB_REQUEST_TYPE_HTD_CLASS_INTERFACE)
    {
        /* Zoom (Absolute) Control */
        if((setup_pkt->value == BCDUSB_NUMBER_OF_USB_1_DOT_0))
        {
            switch(setup_pkt->request)
            {
                case CLEAR_FEATURE:
                  *buf_pp = (uint8_t*)&videoProbeControl;
                  *length = setup_pkt->length;
                break;

                default:
                break;
            }
        }
        if((setup_pkt->value == BCDUSB_NUMBER_OF_USB_2_DOT_0))
        {
            switch(setup_pkt->request)
            {
                case CLEAR_FEATURE:
                    *buf_pp = (uint8_t*)&videoCommitControl;
                    *length = setup_pkt->length;
                break;

                default:
                break;
            }
        }
    }

    return USB_SUCCESS;
}

/***************************************************************************//**
 UVC_tx_complete_cb() call-back function is called by USB Device mode driver
 on completion of the Current Data Transmissions (IN Transaction) which was
 previously initiated using MSS_USBD_tx_ep_configure().
 */
static uint8_t
UVC_tx_complete_cb
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    if (status & (TX_EP_UNDER_RUN_ERROR|TX_EP_STALL_ERROR))
    {
        MSS_USBD_tx_ep_flush_fifo(UVC_ISO_HB_TX_EP);
        g_usbd_uvc_state = 1u;
    }
    else
    {
        g_usbd_uvc_state = 1u;
    }

    if (0 != usbd_uvc_vcp_ops->usb_uvc_tx_complete)
    {
        usbd_uvc_vcp_ops->usb_uvc_tx_complete(status);
    }

    return USB_SUCCESS;
}

/***************************************************************************//**
 UVC_rx_cb() call-back function is called by USB Device mode driver on
 completion of data reception. USB Device mode driver must have been previously
 prepared for this data reception using MSS_USBD_rx_ep_read_prepare()
 */
static uint8_t
UVC_rx_cb
(
    mss_usb_ep_num_t num,
    uint8_t status,
    uint32_t rx_count
)
{
    if (UVC_ISO_HB_RX_EP == num)
    {
        if (0 != usbd_uvc_vcp_ops->usb_uvc_rx)
        {
            usbd_uvc_vcp_ops->usb_uvc_rx(status, rx_count);
        }
    }
    else
    {
        return USB_FAIL;
    }

    return USB_SUCCESS;
}

static uint8_t
UVC_cep_tx_done_cb
(
    uint8_t status
)
{
    return USB_SUCCESS;
}

static uint8_t
UVC_cep_rx_done_cb
(
    uint8_t status
)
{
    return USB_SUCCESS;
}

#endif /* MSS_USB_DEVICE_ENABLED */

#ifdef __cplusplus
}
#endif
