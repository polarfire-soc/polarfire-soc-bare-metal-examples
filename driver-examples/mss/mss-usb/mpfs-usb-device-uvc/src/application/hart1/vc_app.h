/***************************************************************************//**
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * USB Video Class Device example application to demonstrate the
 * PolarFire SoC MSS USB operations in device mode.
 *
 * Header for application code.
 *
 */

#ifndef VC_APP_H_
#define VC_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
  Exported functions from this file
*/

/***************************************************************************//**
  @brief APP_Init()
    This API is used to Initialize UVC application which in turn initializes
    USB Device Core driver and HID Class driver.

  @param
    This function does not take any parameter.

  @return
    This function does not return a value.

  Example:
  @code
  @endcode
*/
void
APP_Init
(
    void
);

/***************************************************************************//**
  @brief APP_Tasks()
    This function is used to provide the Usb Video class driver the lastest
    frame capture from image sensors. This function should be called as
    frequently as possible to provide the most updated information to the host.

  @param
    This function does not take any parameter.

  @return
    This function does not return a value.

  Example:
  @code
  @endcode
*/
void
APP_Tasks
(
    void
);

/***************************************************************************//**
  @brief uvc_init()
    This function is used to Initialize UVC application which in turn initializes
    USB Device Core driver and HID Class driver.

  @param
    This function does not take any parameter.

  @return
    This function does not return a value.

  Example:
  @code
  @endcode
*/
void
uvc_init
(
    void
);

#ifdef __cplusplus
}
#endif

#endif /* VC_APP_H_*/
