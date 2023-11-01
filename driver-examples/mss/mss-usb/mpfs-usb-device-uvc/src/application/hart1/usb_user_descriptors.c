/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file provides the Device Descriptors used by USB Video device.
 *
 */

#include "drivers/mss/mss_usb/mss_usb_device.h"
#include "drivers/mss/mss_usb/mss_usb_std_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Constant definitions
 */
#define  USB_MAX_STRING_DESCRIPTOR_SIZE               64u

/* String Descriptor Indexes */
#define  USB_STRING_DESCRIPTOR_IDX_LANGID             0x00
#define  USB_STRING_DESCRIPTOR_IDX_MANUFACTURER       0x01
#define  USB_STRING_DESCRIPTOR_IDX_PRODUCT            0x02
#define  USB_STRING_DESCRIPTOR_IDX_SERIAL             0x03
#define  USB_STRING_DESCRIPTOR_IDX_CONFIG             0x04
#define  USB_STRING_DESCRIPTOR_IDX_INTERFACE          0x05

#define  USB_STRING_MANUFACTURER                      "Microchip Inc"
#define  USB_STRING_PRODUCT                           "PolarFire SoC - Camera"
#define  USB_STRING_SERIAL                            "123456789ABCDEF151411111"
#define  USB_STRING_CONFIG                            "CFG-HS"
#define  USB_STRING_INTERFACE                         "Interface-VC"


/*******************************************************************************
 * Local functions.
 */
uint8_t* vc_device_descriptor(uint32_t* length);
uint8_t* vc_device_qual_descriptor(mss_usb_device_speed_t speed,
                                            uint32_t* length);
uint8_t* vc_string_descriptor(uint8_t index, uint32_t* length);
uint8_t vc_get_string(uint8_t* string, uint8_t* dest);

/*******************************************************************************
 * Device descriptor.
 */
uint8_t device_descriptor[USB_STD_DEVICE_DESCR_LEN] =
{
    USB_STD_DEVICE_DESCR_LEN,                           /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,                         /* bDescriptorType */
    0x00u,                                              /* bcdUSB LSB */
    0x02u,                                              /* bcdUSB MSB */
    0xEFu,                                              /* bDeviceClass */
    0x02u,                                              /* bDeviceSubClass */
    0x01u,                                              /* bDeviceProtocol */
    0x40u,                                              /* bMaxPacketSize0 */
    0x14u,                                              /* idVendor LSB */
    0x15u,                                              /* idVendor MSB */
    0xFFu,                                              /* idProduct LSB */
    0xFFu,                                              /* idProduct MSB */
    0x00u,                                              /* bcdDevice LSB */
    0x30u,                                              /* bcdDevice MSB */
    USB_STRING_DESCRIPTOR_IDX_MANUFACTURER,             /* iManufacturer */
    USB_STRING_DESCRIPTOR_IDX_PRODUCT,                  /* iProduct */
    USB_STRING_DESCRIPTOR_IDX_SERIAL,                   /* iSerialNumber */
    0x01u                                               /* bNumConfigurations */
};

/*******************************************************************************
 * Device qualifiers.
 */
uint8_t hs_dev_qualifier_descriptor[USB_STD_DEV_QUAL_DESCR_LENGTH] =
{
    USB_STD_DEV_QUAL_DESCR_LENGTH,                      /* bLength */
    USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,               /* bDescriptorType */
    0x00u,                                              /* bcdUSB LSB */
    0x02u,                                              /* bcdUSB MSB */
    0xEFu,                                              /* bDeviceClass */
    0x02u,                                              /* bDeviceSubClass */
    0x01u,                                              /* bDeviceProtocol */
    0x40u,                                              /* bMaxPacketSize0 */
    0x01u,                                              /* bNumConfigurations */
    0x00u                                               /* Reserved */
};

uint8_t fs_dev_qualifier_descriptor[USB_STD_DEV_QUAL_DESCR_LENGTH] =
{
    USB_STD_DEV_QUAL_DESCR_LENGTH,                      /* bLength */
    USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,               /* bDescriptorType */
    0x00u,                                              /* bcdUSB LSB */
    0x02u,                                              /* bcdUSB MSB */
    0xEFu,                                              /* bDeviceClass */
    0x02u,                                              /* bDeviceSubClass */
    0x01u,                                              /* bDeviceProtocol */
    0x40u,                                              /* bMaxPacketSize0 */
    0x01u,                                              /* bNumConfigurations */
    0x00u                                               /* Reserved */
};

uint8_t lang_string_descriptor[] =
{
    0x04u,                                              /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                         /* bDescriptorType */
    0x09u,                                              /* LangID-LSB */
    0x04u                                               /* LangID-MSB */
};

mss_usbd_user_descr_cb_t vc_descriptors_cb = {
    vc_device_descriptor,
    vc_device_qual_descriptor,
    vc_string_descriptor,
};

uint8_t g_string_descriptor[USB_MAX_STRING_DESCRIPTOR_SIZE];

uint8_t*
vc_device_descriptor
(
    uint32_t* length
)
{
   *length = sizeof(device_descriptor);
   return(device_descriptor);
}

uint8_t*
vc_device_qual_descriptor
(
    mss_usb_device_speed_t speed,
    uint32_t* length
)
{
    if(speed == MSS_USB_DEVICE_HS)
    {
        *length = sizeof(fs_dev_qualifier_descriptor);
         return(fs_dev_qualifier_descriptor);
    }
    else
    {
        *length = sizeof(hs_dev_qualifier_descriptor);
         return(hs_dev_qualifier_descriptor);
    }
}

uint8_t*
vc_string_descriptor
(
    uint8_t index,
    uint32_t* length
)
{
    switch(index)
    {
        case USB_STRING_DESCRIPTOR_IDX_LANGID:
                *length = sizeof(lang_string_descriptor);
        break;

        case USB_STRING_DESCRIPTOR_IDX_MANUFACTURER:
            *length = vc_get_string((uint8_t*)USB_STRING_MANUFACTURER,
                                             g_string_descriptor);
        break;

        case USB_STRING_DESCRIPTOR_IDX_PRODUCT:
            *length = vc_get_string((uint8_t*)USB_STRING_PRODUCT,
                                             g_string_descriptor);
        break;

        case USB_STRING_DESCRIPTOR_IDX_SERIAL:
            *length = vc_get_string((uint8_t*)USB_STRING_SERIAL,
                                             g_string_descriptor);
        break;

        case USB_STRING_DESCRIPTOR_IDX_CONFIG:
            *length = vc_get_string((uint8_t*)USB_STRING_CONFIG,
                                             g_string_descriptor);
        break;

        case USB_STRING_DESCRIPTOR_IDX_INTERFACE:
            *length = vc_get_string((uint8_t*)USB_STRING_INTERFACE,
                                             g_string_descriptor);
        break;

        default:
           /*Raise error*/
          *length = 0;
        break;
    }

    if(USB_STRING_DESCRIPTOR_IDX_LANGID == index)
    {
        return(lang_string_descriptor);
    }
    {
        return(g_string_descriptor);
    }
}

uint8_t
vc_get_string
(
    uint8_t* string,
    uint8_t* dest
)
{
    const uint8_t *idx = string ;
    uint8_t *cp_dest;

    cp_dest = dest;

    if((dest != 0u) && (string != 0u))
    {
        for (; *(idx); ++idx)
        {
            *(dest + 2u) = *(idx);
            dest++;
            *(dest + 2u) = 0x00u;
            dest++;
        }
        *cp_dest = ((idx - string) * 2u) + 2u;                /*bLength*/
        *(cp_dest + 1u) = USB_STRING_DESCRIPTOR_TYPE;        /*bDesriptorType*/
    }

    return(((idx - string) * 2u) + 2u);
}

#ifdef __cplusplus
}
#endif
