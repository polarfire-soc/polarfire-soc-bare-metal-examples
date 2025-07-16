/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_usb_config.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) USB Driver Stack
 *
 * MSS USB Driver stack configuration parameters.
 * User must choose the constant definitions in this file to select the mode of
 * operation.
 * The constants defined in this file are used by MSS USB driver stack to
 * function as per configuration.
 *
 */

#ifndef __MSS_USB_CONFIG_H_
#define __MSS_USB_CONFIG_H_

#include "mpfs_hal/mss_hal.h"

/*-------------------------------------------------------------------------*//**
  Definitions Internally generated for use in the Core and logical layer.
*/
#ifdef MSS_USB_OTG_DUAL_ROLE_MODE
    #define MSS_USB_HOST_ENABLED
    #define MSS_USB_DEVICE_ENABLED
    #define MSS_USB_OTG_SRP_ENABLED
    #define MSS_USB_OTG_HNP_ENABLED
#endif

#ifdef MSS_USB_OTG_PERIPHERAL_MODE
    #define MSS_USB_DEVICE_ENABLED
    #define MSS_USB_OTG_SRP_ENABLED
#endif

#ifdef MSS_USB_PERIPHERAL_MODE
    #define MSS_USB_DEVICE_ENABLED
    #define MSS_USB_DEVICE_PRINTER
#endif

#ifdef MSS_USB_OTG_HOST_MODE
    #define MSS_USB_HOST_ENABLED
    #define MSS_USB_OTG_SRP_ENABLED
#endif

#endif  /* __MSS_USB_CONFIG_H_ */
