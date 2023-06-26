/***************************************************************************//**
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * USB HID Class Mouse Device Example application to demonstrate the PolarFire
 * SoC MSS USB operations in device mode.
 *
 * Header file for mouse_app.c
 *
 */
#ifndef __MOUSE_APP_H_
#define __MOUSE_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
  Functions exported by the mouse application.
 */

/***************************************************************************//**
  @brief MOUSE_init()
    This API is used to Initialize Mouse application which in turn initializes
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
MOUSE_init
(
   void
);

/***************************************************************************//**
  @brief MOUSE_task()
    This function is used to provide the HID class driver the latest report
    about Mouse/switches Position. This function should be called as frequently
    as possible to provide the most updated information to the host.

  @param
    This function does not take any parameter.

  @return
    This function does not return a value.

  Example:
  @code
  @endcode
*/
void
MOUSE_task
(
   void
);

#ifdef __cplusplus
}
#endif

#endif  /* __MOUSE_APP_H_ */
