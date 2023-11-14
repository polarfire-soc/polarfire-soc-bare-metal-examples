/***************************************************************************//**
 * Copyright 2007 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Core Timer bare metal software driver public API.
 *
 * @file core_timer.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreTimer prototypes
 *
 */

/*=========================================================================*//**
  @mainpage CoreTimer Bare Metal Driver.

  ==============================================================================
  Introduction
  ==============================================================================

  The CoreTimer is an IP component that provides access to an
  interrupt-generating, programmable decrementing counter over an Advanced
  Peripheral Bus (APB) slave interface.

  This driver provides a set of functions for controlling CoreTimer as part of
  the bare metal system where no operating system is available.
  This driver can be adapted to be used as a part of an operating system, but
  the implementation of the adaptation layer between the driver and the
  operating system's driver model is outside the scope of this user guide.

  ==============================================================================
  Driver Configuration
  ==============================================================================
  Your application software must configure the CoreTimer driver through calls
  to TMR_init() for each CoreTimer instance in the hardware design.
  This function configures a default set of parameters that include a CoreTimer
  hardware instance base address, operating mode, prescaler value, and timer
  load value.

  The CoreTimer supports the following two modes of operation:
  - Continuous mode
  - One-Shot Timer mode

  Continuous mode:
  This is the default mode.
  When zero is reached, the counter is reloaded with the start value, which is
  stored in a programmable register, and continues to count down.
  If the interrupt is enabled, this mode generates an interrupt at a regular
  interval.

  One-Shot Timer mode:
  The counter decrements from its high value and halts at zero.
  The timer must be reprogrammed to begin counting down again.
  This can be achieved by either loading a new value to the timer's counter
  using TMR_reload() or by changing the operating mode through a call to
  TMR_init().

  Either mode of operation can be set during initialization using TMR_init().
  If interrupts are enabled, an interrupt is generated when the decrementing
  counter reaches zero.

  ==============================================================================
  Theory of Operation
  ==============================================================================

  The timer is loaded by writing to the load register and then counts down to
  zero if enabled. 
  To load the timer with an initial value to decrement from, use TMR_init().
  To enable the timer to count down to zero, use TMR_start().

  The prescaler value can be set to divide down the clock to decrement the
  CoreTimer counter.
  The prescaler values can be set to trigger a pulse at every 2, 4, 8, 16, 32,
  64, 128, 256, 512, or 1024 clock periods.

  Use TMR_reload() to load new values into the timer's load register.
  If TMR_reload() is called while the timer is already running, the timer
  immediately begins to decrement from the new value.

  If interrupts are enabled with TMR_enable_int(), an interrupt is generated
  when the timer reaches zero.
  To clear the interrupt, write to the interrupt clear register using TMR_clear_int().

  If the timer is operating in One-Shot Timer mode, it halts on reaching zero
  until the timer is reloaded using TMR_reload() or until the Operating mode is
  changed to Continuous mode. When operating in Continuous mode, the timer
  reloads the count value that was initially set using TMR_init() and continues
  to decrement.
  The counter effectively generates a periodic interrupt when operating in
  Continuous mode.

  To read the current counter value at any time, use TMR_current_value().

 *//*=========================================================================*/
#ifndef CORE_TIMER_H_
#define CORE_TIMER_H_

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#include "hal_assert.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
  Timer Operating Mode
  =======================================
  The following definitions select the operating mode of the CoreTimer driver.
  They allow for selecting Continuous or One-Shot Timer modes.

  | Constant            | Description                                  |
  | ------------------- | -------------------------------------------- |
  | TMR_CONTINUOUS_MODE | Coretimer will operate in Continuous mode     |
  | TMR_ONE_SHOT_MODE   | Coretimer will operate in One-Shot Timer mode |

 */
#define TMR_CONTINUOUS_MODE 0
#define TMR_ONE_SHOT_MODE   1


/***************************************************************************//**
  Prescaler Value
  =======================================
  The following definitions configure the CoreTimer prescaler.
  The prescaler divides down the clock to decrement the CoreTimer counter.
  It can be configured to divide the clock by 2, 4, 8, 16, 32, 64, 128, 256,
  512, or 1024 clock periods.

  | Constant           | Description                                   |
  | ------------------ | ----------------------------------------------|
  | PRESCALER_DIV_2    | Scale down the timer's clock frequency by 2    |
  | PRESCALER_DIV_4    | Scale down the timer's clock frequency by 4    |
  | PRESCALER_DIV_8    | Scale down the timer's clock frequency by 8    |
  | PRESCALER_DIV_16   | Scale down the timer's clock frequency by 16   |
  | PRESCALER_DIV_32   | Scale down the timer's clock frequency by 32   |
  | PRESCALER_DIV_64   | Scale down the timer's clock frequency by 64   |
  | PRESCALER_DIV_128  | Scale down the timer's clock frequency by 128  |
  | PRESCALER_DIV_256  | Scale down the timer's clock frequency by 256  |
  | PRESCALER_DIV_512  | Scale down the timer's clock frequency by 512  |
  | PRESCALER_DIV_1024 | Scale down the timer's clock frequency by 1024 |

*/
#define PRESCALER_DIV_2     0
#define PRESCALER_DIV_4     1
#define PRESCALER_DIV_8     2
#define PRESCALER_DIV_16    3
#define PRESCALER_DIV_32    4
#define PRESCALER_DIV_64    5
#define PRESCALER_DIV_128   6
#define PRESCALER_DIV_256   7
#define PRESCALER_DIV_512   8
#define PRESCALER_DIV_1024  9

/***************************************************************************//**
 There must be one instance of this structure for each instance of CoreTimer in
 your system.
 TMR_init() initializes this structure.
 It identifies the various CoreTimer hardware instances in your system.
 An initialized timer instance structure must be passed as the first parameter
 to CoreTimer driver functions to identify which CoreTimer instance must perform
 the requested operation.
 Software using this driver must only create one single instance of this data
 structure for each hardware timer instance in the system.
 */
typedef struct __timer_instance_t
{
    addr_t base_address;
} timer_instance_t;

/*------------------------Public Function-------------------------------------*/

/***************************************************************************//**
 This function initializes the data structure and sets relevant CoreTimer
 registers.
 It prepares the timer for use in a given hardware or software configuration.
 It must be called before any other timer API function.
 The timer won't start counting down immediately after this function is
 called.
 It is necessary to call TMR_start() to start the timer decrementing.
 The CoreTimer interrupt is disabled as part of this function.



 @param this_timer    Pointer to a timer_instance_t structure holding all
                      relevant data associated with the target timer hardware
                      instance. This pointer identifies the target CoreTimer
                      hardware instance in subsequent calls to the CoreTimer
                      functions.
 @param address       Base address in the processor's memory map of the
                      registers of the CoreTimer instance being initialized.
 @param mode          This parameter selects the operating mode of the timer
                      instance. This can be either TMR_CONTINUOUS_MODE
                      or TMR_ONE_SHOT_MODE.
 @param prescaler     This parameter selects the prescaler divider that divides
                      down the clock used to decrement the timer's
                      counter. This can be set using one of the
                      `PRESCALER_DIV_<n>` definitions, where `<n>` is the
                      divider's value.
 @param load_value    This parameter sets the timer's load value, from which the
                      CoreTimer counter decrements
                      In Continuous mode, this value reloads the timer's counter
                      whenever it reaches zero.
 @return              This function does not return any value.                      
 */
void TMR_init(timer_instance_t *this_timer,
              addr_t address,
              uint8_t mode,
              uint32_t prescaler,
              uint32_t load_value);

/***************************************************************************//**
 This function starts the timer counting down.
 This function must be called after calling TMR_init().
 It does not need to be called after each call to TMR_reload() when the timer is
 used in One-Shot Timer mode.
 
 @param this_timer    Pointer to a timer_instance_t structure holding all
                      relevant data associated with the target timer hardware
                      instance. This pointer identifies the target
                      CoreTimer hardware instance.
 @return              This function does not return any value.                      
 */
void TMR_start(timer_instance_t *this_timer);

/***************************************************************************//**
 This function stops the timer from counting down.
 It stops interrupts from being generated when Continuous mode is used
 and interrupts must be paused.

 @param this_timer    Pointer to a timer_instance_t structure holding all
                      relevant data associated with the target timer hardware
                      instance. This pointer identifies the target
                      CoreTimer hardware instance.
 @return              This function does not return any value.                      
 */
void TMR_stop(timer_instance_t *this_timer);

/***************************************************************************//**
 This function enables the timer interrupt. It allows the interrupt signal
 coming out of CoreTimer to be asserted.

 @param this_timer    Pointer to a timer_instance_t structure holding all
                      relevant data associated with the target timer hardware
                      instance. This pointer identifies the target
                      CoreTimer hardware instance.
 @return              This function does not return any value.                      
 */
void TMR_enable_int(timer_instance_t *this_timer);

/***************************************************************************//**
 This function clears the timer interrupt.
 It must be called within the interrupt handler, servicing interrupts from the
 timer.
 Failure to clear the timer interrupt results in the interrupt signal
 generated from CoreTimer remaining asserted. This assertion may cause the
 interrupt service routine to be continuously called, causing the system to
 lock up.

 @param this_timer    Pointer to a timer_instance_t structure holding all
                      relevant data associated with the target timer hardware
                      instance. This pointer identifies the target
                      CoreTimer hardware instance.
 @return              This function does not return any value.                                     
 */
void TMR_clear_int(timer_instance_t *this_timer);

/***************************************************************************//**
 This function returns the current value of the counter.

 @param this_timer    Pointer to a timer_instance_t structure holding all
                      relevant data associated with the target timer hardware
                      instance. This pointer identifies the target
                      CoreTimer hardware instance.

 @return              Returns the current value of the timer counter value.
 */
uint32_t TMR_current_value(timer_instance_t *this_timer);

/***************************************************************************//**
 This function is used in One-Shot Timer mode.
 It reloads the timer counter with the value passed as a parameter.
 The timer counter begins to decrement again after calling TMR_reload(), 
 it is not required to call TMR_start() again.

 @param this_timer    Pointer to a timer_instance_t structure holding all
                      relevant data associated with the target timer hardware
                      instance. This pointer identifies the target
                      CoreTimer hardware instance.
 @param load_value    This parameter sets the timer's load value, from which the
                      CoreTimer counter decrements.
 @return              This function does not return any value.                    
 */
void TMR_reload(timer_instance_t *this_timer, uint32_t load_value);

#ifdef __cplusplus
}
#endif
    
#endif /* CORE_TIMER_H_ */
