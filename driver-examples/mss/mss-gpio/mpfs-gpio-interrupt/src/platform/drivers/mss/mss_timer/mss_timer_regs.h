/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_timer_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Register bit offsets and masks definitions for PolarFire SoC
 * Microprocessor Subsystem (MSS) Timer
 *
 */

#ifndef MSS_TIMER_REGS_H_
#define MSS_TIMER_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
/*--------------------------MPFS MSS Timer register map-----------------------*/
/*----------------------------------------------------------------------------*/
typedef struct
{
    /* Timer 1 register declaration */
    const volatile  uint32_t TIM1_VAL;
    volatile uint32_t TIM1_LOADVAL;
    volatile uint32_t TIM1_BGLOADVAL;
    volatile uint32_t TIM1_CTRL;
    volatile uint32_t TIM1_RIS;
    const volatile  uint32_t TIM1_MIS;

    /* Timer 2 register declaration */
    const volatile  uint32_t TIM2_VAL;
    volatile uint32_t TIM2_LOADVAL;
    volatile uint32_t TIM2_BGLOADVAL;
    volatile uint32_t TIM2_CTRL;
    volatile uint32_t TIM2_RIS;
    const volatile  uint32_t TIM2_MIS;

    /* Timer 64 register declaration */
    const volatile  uint32_t TIM64_VAL_U;
    const volatile  uint32_t TIM64_VAL_L;
    volatile uint32_t TIM64_LOADVAL_U;
    volatile uint32_t TIM64_LOADVAL_L;
    volatile uint32_t TIM64_BGLOADVAL_U;
    volatile uint32_t TIM64_BGLOADVAL_L;
    volatile uint32_t TIM64_CTRL;
    volatile uint32_t TIM64_RIS;
    const volatile  uint32_t TIM64_MIS;
    volatile uint32_t TIM64_MODE;
} TIMER_TypeDef;

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
#define TIMER_LO_BASE         0x20125000
#define TIMER_HI_BASE         0x28125000


#ifdef __cplusplus
}
#endif

#endif  /* MSS_TIMER_REGS_H_ */
