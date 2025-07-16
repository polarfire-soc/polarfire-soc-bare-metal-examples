/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_timer.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief PolarFire SoC Microprocessor Subsystem (MSS) Timer bare metal software
 * driver public API.
 *
 */
/*=========================================================================*//**
  @mainpage PolarFire® SoC MSS Timer Bare Metal Driver.

  ==============================================================================
  Introduction
  ==============================================================================
  The PolarFire® SoC Microprocessor Subsystem (MSS) includes a timer hardware
  block which is used as two independent 32-bit timers or as a single 64-bit
  timer in Periodic or One-shot mode.

  This driver provides a set of functions for controlling the MSS Timer as part
  of a bare metal system where no operating system is available. These drivers
  can be adapted for use as part of an operating system, but the implementation
  of the adaptation layer between this driver and the operating system's driver
  model is outside the scope of this driver.

  ==============================================================================
  Theory of Operation
  ==============================================================================
  The PolarFire SoC MSS Timer is used in one of two mutually exclusive modes,
  either as a single 64-bit timer or as two independent 32-bit timers. The MSS
  Timer is used in either Periodic mode or One-shot mode. A timer, when
  configured for Periodic mode, generates an interrupt and reloads its
  down-counter when it reaches zero. The timer then continues decrementing from
  its reload value without waiting for the interrupt to be cleared. A timer,
  when configured for One-shot mode, generates an interrupt once when its
  down-counter reaches zero. It must be explicitly reloaded to start
  decrementing again.

  The MSS Timer driver functions are grouped into the following categories:
    - Initialization and configuration
    - Timer control
    - Interrupt control

  The MSS Timer driver provides three initialization functions:
    - MSS_TIM1_init()
    - MSS_TIM2_init()
    - MSS_TIM64_init()

  The MSS Timer driver is initialized through calls to these functions and at
  least one of them must be called before any other MSS Timer driver function
  can be called. You should only use MSS_TIM1_init() and MSS_TIM2_init() if you
  intend to use the timer in 32-bit mode. Use MSS_TIM64_init() if you intend to
  use the MSS Timer as a single 64-bit timer. Initialization functions take a
  single parameter specifying the operating mode of the timer being initialized.

  Once initialized, a timer is controlled using the following functions:
    - MSS_TIM1_load_immediate()
    - MSS_TIM1_load_background()
    - MSS_TIM1_get_current_value()
    - MSS_TIM1_start()
    - MSS_TIM1_stop()
    - MSS_TIM2_load_immediate()
    - MSS_TIM2_load_background()
    - MSS_TIM2_get_current_value()
    - MSS_TIM2_start()
    - MSS_TIM2_stop()
    - MSS_TIM64_load_immediate()
    - MSS_TIM64_load_background()
    - MSS_TIM64_get_current_value()
    - MSS_TIM64_start()
    - MSS_TIM64_stop()

  Timer interrupts are controlled using the following functions:
    - MSS_TIM1_enable_irq()
    - MSS_TIM1_disable_irq()
    - MSS_TIM1_clear_irq()
    - MSS_TIM2_enable_irq()
    - MSS_TIM2_disable_irq()
    - MSS_TIM2_clear_irq()
    - MSS_TIM64_enable_irq()
    - MSS_TIM64_disable_irq()
    - MSS_TIM64_clear_irq()

  The timer interrupt handlers have the following function prototypes:
    - void Timer1_IRQHandler(void)
    - void Timer2_IRQHandler(void)

  Entries for these interrupt handlers are provided in the PolarFire SoC MPFS
  HAL vector table. To add a Timer 1 interrupt handler, you must implement
  Timer1_IRQHandler() as part of your application code. To add a Timer 2
  interrupt handler, you must implement Timer2_IRQHandler() as part of your
  application code. When using the MSS Timer as a 64-bit timer, you must
  implement Timer1_IRQHandler() as part of your application code. The Timer 2
  interrupt is not used when the MSS Timer is configured as a 64-bit timer.

 *//*=========================================================================*/
#ifndef MSS_TIMER_H_
#define MSS_TIMER_H_

#include "mpfs_hal/mss_hal.h"
#include "mss_timer_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

 /*****************************************************************************/
 /*                         Peripheral declaration                            */
 /*****************************************************************************/

 #define TIMER_LO                      ((TIMER_TypeDef*) TIMER_LO_BASE)
 #define TIMER_HI                      ((TIMER_TypeDef*) TIMER_HI_BASE)

/*-------------------------------------------------------------------------*//**
 * This enumeration selects between the two possible timer modes of operation:
 * Periodic and One-shot mode. It is used as an argument to the MSS_TIM1_init(),
 * MSS_TIM2_init(), and MSS_TIM64_init() function.
 *
 * MSS_TIMER_PERIODIC_MODE:
 *    The timer generates interrupts at constant intervals. On reaching zero,
 *    the counter of the timer is reloaded with a value held in a register and
 *    begins counting down again.
 *
 * MSS_TIMER_ONE_SHOT_MODE:
 *    The timer generates a single interrupt in this mode. On reaching zero, the
 *    counter of the timer halts until reprogrammed by the user.
 */
typedef enum __mss_timer_mode
{
    MSS_TIMER_PERIODIC_MODE = 0,//!< MSS_TIMER_PERIODIC_MODE
    MSS_TIMER_ONE_SHOT_MODE = 1 //!< MSS_TIMER_ONE_SHOT_MODE
} mss_timer_mode_t;

/*-------------------------------------------------------------------------*//**
 * Timer operation mask defines.
 */
/* Timer 1 interrupt enable bits */
#define TIM1_INTEN_MASK     0x00000004u

/* Timer 1 Mode bits */
#define TIM1_MODE_SHIFT     1u
#define TIM1_MODE_MASK      0x00000002u

/* Timer 1 enable bits */
#define TIM1_ENABLE_MASK    0x00000001u

/* Timer 2 interrupt enable bits */
#define TIM2_INTEN_MASK     0x00000004u

/* Timer 2 Mode bits */
#define TIM2_MODE_SHIFT     1u
#define TIM2_MODE_MASK      0x00000002u

/* Timer 2 enable bits */
#define TIM2_ENABLE_MASK    0x00000001u

/* Timer 64 interrupt enable bits */
#define TIM64_INTEN_MASK    0x00000004u

/* Timer 64 mode bits */
#define TIM64_MODE_SHIFT    1u
#define TIM64_MODE_MASK     0x00000002u

/* Timer 64 enable bits */
#define TIM64_ENABLE_MASK   0x00000001u

static uint32_t readvalue[52] = {0};

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_init() initializes the MSS Timer block for use as a 32-bit timer and
  selects the operating mode for Timer 1. The MSS Timer block is out of reset
  before executing this function. MSS_TIM1_init() stops Timer 1, disables its
  interrupt, and sets the Timer 1 operating mode.

  Note:
  The MSS Timer block cannot be used both as a 64-bit and 32-bit timer. When the
  MSS_TIM1_init() function is invoked, it overwrites any previous configuration
  of the MSS Timer as a 64-bit timer.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param mode
    The mode parameter specifies whether the timer operates in Periodic or
    One-shot mode.
    - Following are the allowed values:
      - MSS_TIMER_PERIODIC_MODE
      - MSS_TIMER_ONE_SHOT_MODE

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM1_init(TIMER_TypeDef* timer, mss_timer_mode_t mode)
{
    PLIC_DisableIRQ(TIMER1_PLIC);               /* Disable timer 1 irq */

    timer->TIM64_MODE = 0u;                     /* switch to 32 bits mode */
    readvalue[1] = timer->TIM64_MODE;

    /* Disable timer and interrupt and set mode (continuous/one-shot) */
    timer->TIM1_CTRL = TIM1_MODE_MASK & ((uint32_t)mode << TIM1_MODE_SHIFT);
    readvalue[2] = timer->TIM1_CTRL;

    timer->TIM1_RIS = 1u;                       /* clear timer 1 interrupt */
    readvalue[3] = timer->TIM1_RIS;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_start() enables Timer 1 and starts its down-counter, decrementing
  from the load_value specified in previous calls to MSS_TIM1_load_immediate()
  or MSS_TIM1_load_background().

  Note:
  MSS_TIM1_start() is also used to resume the down-counter if previously stopped
  using MSS_TIM1_stop().

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM1_start(TIMER_TypeDef* timer)
{
    timer->TIM1_CTRL |= TIM1_ENABLE_MASK;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_stop() disables Timer 1 and stops its down-counter.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM1_stop(TIMER_TypeDef* timer)
{
    timer->TIM1_CTRL &= ~((uint32_t)TIM1_ENABLE_MASK);    /* disable timer */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_get_current_value() returns the current value of the Timer 1
  down-counter.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function returns the 32-bit current value of the Timer 1 down-counter.
 */
static inline uint32_t
MSS_TIM1_get_current_value(TIMER_TypeDef* timer)
{
    return timer->TIM1_VAL;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_load_immediate() loads the value passed by the load_value parameter
  into the Timer 1 down-counter. The counter decrements immediately from this
  value once Timer 1 is enabled. The MSS Timer generates an interrupt when the
  counter reaches zero, if Timer 1 interrupts are enabled. This function is
  intended to be used when Timer 1 is configured for One-shot mode to time a
  single delay.

  Note:
  The value passed by the load_value parameter is loaded immediately into the
  down-counter regardless of whether Timer 1 is operating in Periodic or
  One-shot mode.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param load_value
    The load_value parameter specifies the value from which the Timer 1
    down-counter starts decrementing from.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM1_load_immediate(TIMER_TypeDef* timer, uint32_t load_value)
{
    timer->TIM1_LOADVAL = load_value;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_load_background() specifies the value that reloads into the Timer 1
  down-counter the next time the counter reaches zero. This function is
  typically used when Timer 1 is configured for Periodic mode to select or
  change the delay period between the interrupts generated by Timer 1.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param load_value
    The load_value parameter specifies the value that loads into the Timer 1
    down-counter the next time the down-counter reaches zero. The Timer 1
    down-counter starts decrementing from this value after the current count
    expires.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM1_load_background(TIMER_TypeDef* timer, uint32_t load_value)
{
    timer->TIM1_BGLOADVAL = load_value;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_enable_irq() enables interrupt generation for Timer 1. This function
  also enables the interrupt in the RISC-V PLIC. Timer1_IRQHandler() is called
  when a Timer 1 interrupt occurs.

  Note:
  A Timer1_IRQHandler() default implementation is defined, with weak linkage, in
  the MPFS HAL. You must provide your own implementation of Timer1_IRQHandler(),
  which overrides the default implementation, to suit your application.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
*/
static inline void
MSS_TIM1_enable_irq(TIMER_TypeDef* timer)
{
    timer->TIM1_CTRL |= TIM1_INTEN_MASK;
    readvalue[8] = timer->TIM1_CTRL;
    PLIC_EnableIRQ(TIMER1_PLIC);
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_disable_irq() disables interrupt generation for Timer 1. This
  function also disables the interrupt in the RISC-V PLIC.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM1_disable_irq(TIMER_TypeDef* timer)
{
    timer->TIM1_CTRL &= ~((uint32_t)TIM1_INTEN_MASK);
    PLIC_DisableIRQ(TIMER1_PLIC);               /* Disable timer 1 irq */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM1_clear_irq() clears a pending interrupt from Timer 1. This function
  also clears the interrupt in the RISC-V PLIC.

  Note:
  You must call MSS_TIM1_clear_irq() as part of your implementation of the
  Timer1_IRQHandler() Timer 1 Interrupt Service Routine (ISR) in order to
  prevent the same interrupt event retriggering a call to the ISR.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM1_clear_irq(TIMER_TypeDef* timer)
{
    timer->TIM1_RIS = 1u;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_init() initializes the MSS Timer block for use as a 32-bit timer and
  selects the operating mode for Timer 2. The MSS Timer block is already out of
  reset before executing MSS_TIM2_init(). This function stops Timer 2, disables
  its interrupt, and sets the Timer 2 operating mode.

  Note:
  The MSS Timer block cannot be used both as a 64-bit and 32-bit timer. When
  MSS_TIM2_init() is invoked, it overwrites any previous configuration of the
  MSS Timer as a 64-bit timer.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param mode
    The mode parameter specifies whether the timer operates in Periodic or
    One-shot mode.
    - Following are the allowed values:
      - MSS_TIMER_PERIODIC_MODE
      - MSS_TIMER_ONE_SHOT_MODE

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_init(TIMER_TypeDef* timer, mss_timer_mode_t mode)
{
    PLIC_DisableIRQ(TIMER2_PLIC);               /* Disable timer 2 irq */
    timer->TIM64_MODE = 0u;                     /* switch to 32 bits mode */

    /* Disable timer and interrupt. Set mode (continuous/one-shot) */
    timer->TIM2_CTRL = TIM2_MODE_MASK & ((uint32_t)mode << TIM2_MODE_SHIFT);

    timer->TIM2_RIS = 1u;                       /* clear timer 2 interrupt */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_start() enables Timer 2 and starts its down-counter, decrementing
  from the load_value specified in previous calls to MSS_TIM2_load_immediate()
  or MSS_TIM2_load_background().

  Note:
  MSS_TIM2_start() is also used to resume the down-counter if previously stopped
  using MSS_TIM2_stop().

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_start(TIMER_TypeDef* timer)
{
    timer->TIM2_CTRL |= TIM2_ENABLE_MASK;     /* enable timer */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_stop() disables Timer 2 and stops its down-counter.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_stop(TIMER_TypeDef* timer)
{
    timer->TIM2_CTRL &= ~((uint32_t)TIM2_ENABLE_MASK);    /* disable timer */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_get_current_value() returns the current value of the Timer 2
  down-counter.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline uint32_t
MSS_TIM2_get_current_value(TIMER_TypeDef* timer)
{
    return timer->TIM2_VAL;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_load_immediate() loads the value passed by the load_value parameter
  into the Timer 2 down-counter. The counter decrements immediately from this
  value once Timer 2 is enabled. The MSS Timer generates an interrupt when the
  counter reaches zero if Timer 2 interrupts are enabled. This function is
  intended to be used when Timer 2 is configured for One-shot mode to time a
  single delay.

  Note:
  The value passed by the load_value parameter is loaded immediately into the
  down-counter regardless of whether Timer 2 is operating in Periodic or
  One-shot mode.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param load_value
    The load_value parameter specifies the value from which the Timer 2
    down-counter starts decrementing.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_load_immediate(TIMER_TypeDef* timer, uint32_t load_value)
{
    timer->TIM2_LOADVAL = load_value;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_load_background() specifies the value that is reloaded into the Timer
  2 down-counter the next time the counter reaches zero. This function is
  typically used when Timer 2 is configured for Periodic mode to select or
  change the delay period between the interrupts generated by Timer 2.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param load_value
    The load_value parameter specifies the value that is loaded into the Timer 2
    down-counter the next time the down-counter reaches zero. The Timer 2
    down-counter starts decrementing from this value after the current count
    expires.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_load_background(TIMER_TypeDef* timer, uint32_t load_value)
{
    timer->TIM2_BGLOADVAL = load_value;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_enable_irq() enables interrupt generation for Timer 2. This function
  also enables the interrupt in the RISC-V PLIC. Timer2_IRQHandler() is called
  when a Timer 2 interrupt occurs.

  Note:
  A Timer2_IRQHandler() default implementation is defined, with weak linkage, in
  the MPFS HAL. You must provide your own implementation of Timer2_IRQHandler(),
  which overrides the default implementation, to suit your application.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_enable_irq(TIMER_TypeDef* timer)
{
    timer->TIM2_CTRL |= TIM2_INTEN_MASK;
    PLIC_EnableIRQ(TIMER2_PLIC);
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_disable_irq() disables interrupt generation for Timer 2. This
  function also disables the interrupt in the RISC-V PLIC.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_disable_irq(TIMER_TypeDef* timer)
{
    timer->TIM2_CTRL &=  ~((uint32_t)TIM2_INTEN_MASK);
    PLIC_DisableIRQ(TIMER2_PLIC);               /* Disable timer 2 irq */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM2_clear_irq() clears a pending interrupt from Timer 2. This function
  also clears the interrupt in the RISC-V PLIC.

  Note:
  You must call MSS_TIM2_clear_irq() as part of your implementation of the
  Timer2_IRQHandler() Timer 2 Interrupt Service Routine (ISR) in order to
  prevent the same interrupt event retriggering a call to the ISR.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM2_clear_irq(TIMER_TypeDef* timer)
{
    timer->TIM2_RIS = 1u;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_init() initializes the MSS Timer block for use as a single 64-bit
  timer and selects the operating mode of the timer. The MSS Timer block is
  already out of reset before executing MSS_TIM64_init(). This function stops
  the timer, disables its interrupts, and sets the timer's operating mode.

  Note:
  The MSS Timer block cannot be used both as a 64-bit and 32-bit timer. When the
  MSS_TIM64_init() function is invoked, it overwrites any previous configuration
  of the MSS Timer as a 32-bit timer.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param mode
    The mode parameter specifies whether the timer operates in Periodic or
    One-shot mode.
    - Following are the allowed values:
      - MSS_TIMER_PERIODIC_MODE
      - MSS_TIMER_ONE_SHOT_MODE

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_init(TIMER_TypeDef* timer, mss_timer_mode_t mode)
{
    PLIC_DisableIRQ(TIMER1_PLIC);               /* Disable timer 1 irq */
    PLIC_DisableIRQ(TIMER2_PLIC);               /* Disable timer 2 irq */
    timer->TIM64_MODE = 1u;                     /* switch to 64 bits mode */

    /* Disable timer and interrupt and set mode (continuous/one-shot) */
    timer->TIM64_CTRL = TIM64_MODE_MASK & ((uint32_t)mode << TIM64_MODE_SHIFT);

    timer->TIM1_RIS = 1u;                   /* clear timer 1 interrupt */
    timer->TIM2_RIS = 1u;                   /* clear timer 2 interrupt */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_start() enables the 64-bit timer and starts its down-counter,
  decrementing from the load_value specified in previous calls to
  MSS_TIM64_load_immediate() or MSS_TIM64_load_background().

  Note:
  MSS_TIM64_start() is also used to resume the down-counter if previously
  stopped using MSS_TIM64_stop().

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_start(TIMER_TypeDef* timer)
{
    timer->TIM64_CTRL |= TIM64_ENABLE_MASK;   /* enable timer */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_stop() disables the 64-bit timer and stops its down-counter.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_stop(TIMER_TypeDef* timer)
{
    timer->TIM64_CTRL &= ~((uint32_t)TIM64_ENABLE_MASK);    /* disable timer */
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_get_current_value() is used to read the current value of the 64-bit
  timer down-counter.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param load_value_u
    The load_value_u parameter is a pointer to a 32-bit variable where the upper
    32 bits of the current value of the 64-bit timer down-counter are copied.

  @param load_value_l
    The load_value_l parameter is a pointer to a 32-bit variable where the lower
    32 bits of the current value of the 64-bit timer down-counter are copied.

  @return
    This function does not return a value.

  @example
  @code
    uint32_t current_value_u = 0;
    uint32_t current_value_l = 0;
    MSS_TIM64_get_current_value(&current_value_u, &current_value_l);
  @endcode
 */
static inline void
MSS_TIM64_get_current_value
(
    TIMER_TypeDef* timer,
    uint32_t * load_value_u,
    uint32_t * load_value_l
)
{
    *load_value_l = timer->TIM64_VAL_L;
    *load_value_u = timer->TIM64_VAL_U;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_load_immediate() loads the values passed by the load_value_u and
  load_value_l parameters into the 64-bit timer down-counter. The counter
  decrements immediately from the concatenated 64-bit value once the 64-bit
  timer is enabled. The MSS Timer generates an interrupt when the counter
  reaches zero if 64-bit timer interrupts are enabled. This function is intended
  to be used when the 64-bit timer is configured for One-shot mode to time a
  single delay.

  Note:
  The value passed by the load_value parameter is loaded immediately into the
  down-counter regardless of whether the 64-bit timer is operating in Periodic
  or One-shot mode.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param load_value_u
    The load_value_u parameter specifies the upper 32 bits of the 64-bit timer
    load value from which the 64-bit timer down-counter starts decrementing.

  @param load_value_l
    The load_value_l parameter specifies the lower 32 bits of the 64-bit timer
    load value from which the 64-bit timer down-counter starts decrementing.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_load_immediate
(
    TIMER_TypeDef* timer,
    uint32_t load_value_u,
    uint32_t load_value_l
)
{
    timer->TIM64_LOADVAL_U = load_value_u;
    timer->TIM64_LOADVAL_L = load_value_l;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_load_background() specifies the 64-bit value that reloads into the
  64-bit timer down-counter the next time the counter reaches zero. This
  function is typically used when the 64-bit timer is configured for Periodic
  mode to select or change the delay period between the interrupts generated by
  the 64-bit timer.

  @param timer
    The timer parameter specifies the Timer block being used.

  @param load_value_u
    The load_value_u parameter specifies the upper 32 bits of the 64-bit timer
    load value. The concatenated 64-bit value formed from load_value_u and
    load_value_l loads into the 64-bit timer down-counter the next time the
    down-counter reaches zero. The 64-bit timer down-counter starts decrementing
    from the concatenated 64-bit value after the current count expires.

  @param load_value_l
    The load_value_l parameter specifies the lower 32 bits of the 64-bit timer
    load value. The concatenated 64-bit value formed from load_value_u and
    load_value_l loads into the 64-bit timer down-counter the next time the
    down-counter reaches zero. The 64-bit timer down-counter starts decrementing
    from the concatenated 64-bit value after the current count expires.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_load_background
(
    TIMER_TypeDef* timer,
    uint32_t load_value_u,
    uint32_t load_value_l
)
{
    timer->TIM64_BGLOADVAL_U = load_value_u;
    timer->TIM64_BGLOADVAL_L = load_value_l;
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_enable_irq() enables interrupt generation for the 64-bit timer. This
  function also enables the interrupt in the RISC-V PLIC. Timer1_IRQHandler() is
  called when a 64-bit timer interrupt occurs.

  Note:
  A Timer1_IRQHandler() default implementation is defined, with weak linkage, in
  the MPFS HAL. You must provide your own implementation of Timer1_IRQHandler(),
  which overrides the default implementation, to suit your application.

  Note:
  MSS_TIM64_enable_irq() enables and uses Timer 1 interrupts for the 64-bit
  timer. Timer 2 interrupts remain disabled.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_enable_irq(TIMER_TypeDef* timer)
{
    timer->TIM64_CTRL |= TIM64_INTEN_MASK;
    PLIC_EnableIRQ(TIMER1_PLIC);
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_disable_irq() disables interrupt generation for the 64-bit timer.
  This function also disables the interrupt in the RISC-V PLIC.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_disable_irq(TIMER_TypeDef* timer)
{
    timer->TIM64_CTRL &= ~((uint32_t)TIM64_INTEN_MASK);
    PLIC_DisableIRQ(TIMER1_PLIC);
}

/*-------------------------------------------------------------------------*//**
  MSS_TIM64_clear_irq() clears a pending interrupt from the 64-bit timer. This
  function also clears the interrupt in the RISC-V PLIC.

  Note:
  You must call MSS_TIM64_clear_irq() as part of your implementation of the
  Timer1_IRQHandler() 64-bit timer Interrupt Service Routine (ISR) in order to
  prevent the same interrupt event retriggering a call to the ISR.

  @param timer
    The timer parameter specifies the Timer block being used.

  @return
    This function does not return a value.
 */
static inline void
MSS_TIM64_clear_irq(TIMER_TypeDef* timer)
{
    timer->TIM64_RIS = 1u;
}

#ifdef __cplusplus
}
#endif

#endif /*MSS_TIMER_H_*/
