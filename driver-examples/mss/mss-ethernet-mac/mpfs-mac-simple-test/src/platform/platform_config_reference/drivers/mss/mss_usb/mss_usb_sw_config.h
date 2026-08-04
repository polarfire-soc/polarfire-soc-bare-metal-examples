/**
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * PolarFire SoC MSS USB Driver Stack
 *
 * MSS USB Driver stack configuration parameters.
 * User must choose the constant definitions in this file to select the mode of
 * operation.
 * The constants defined in this file are used by MSS USB driver stack to
 * function as per configuration.
 *
 * mss_usb_sw_config.h
 */

#ifndef MSS_USB_SW_CONFIG_H_
#define MSS_USB_SW_CONFIG_H_

/*-------------------------------------------------------------------------*//**
  User should choose the Mode in which PolarFire SoC MSS USB should operate
*/
/* #define MSS_USB_OTG_DUAL_ROLE_MODE */
/* #define MSS_USB_OTG_PERIPHERAL_MODE*/

/* Configures the MSS USB Driver Stack to operate in USB Host mode. */
/* #define MSS_USB_OTG_HOST_MODE */

/* Configures the MSS USB Driver Stack to operate in USB Device mode. */
/* #define MSS_USB_PERIPHERAL_MODE */

/* Used for internal testing of the driver. Not for Application use. */
/* #define MSS_USB_DEVICE_TEST_MODE */


#endif /* MSS_USB_SW_CONFIG_H_ */
