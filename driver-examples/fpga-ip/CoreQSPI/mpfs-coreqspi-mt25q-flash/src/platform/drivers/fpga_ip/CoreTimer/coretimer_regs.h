/*******************************************************************************
 * (c) Copyright 2007 Microchip FPGA Embedded Systems Solutions.
 * 
 * SPDX-License-Identifier: MIT
 *
 * @file coretimer_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreTimer register definitions
 *
 */

#ifndef __CORE_TIMER_REGISTERS
#define __CORE_TIMER_REGISTERS  1

/*------------------------------------------------------------------------------
 * TimerLoad register details
 */
#define TimerLoad_REG_OFFSET    0x00

/*
 * LoadValue bits.
 */
#define LoadValue_OFFSET   0x00
#define LoadValue_MASK     0xFFFFFFFF
#define LoadValue_SHIFT    0

/*------------------------------------------------------------------------------
 * TimerValue register details
 */
#define TimerValue_REG_OFFSET   0x04

/*
 * CurrentValue bits.
 */
#define CurrentValue_OFFSET   0x04
#define CurrentValue_MASK     0xFFFFFFFF
#define CurrentValue_SHIFT    0

/*------------------------------------------------------------------------------
 * TimerControl register details
 */
#define TimerControl_REG_OFFSET 0x08

/*
 * TimerEnable bits.
 */
#define TimerEnable_OFFSET   0x08
#define TimerEnable_MASK     0x00000001
#define TimerEnable_SHIFT    0

/*
 * InterruptEnable bits.
 */
#define InterruptEnable_OFFSET   0x08
#define InterruptEnable_MASK     0x00000002
#define InterruptEnable_SHIFT    1

/*
 * TimerMode bits.
 */
#define TimerMode_OFFSET   0x08
#define TimerMode_MASK     0x00000004
#define TimerMode_SHIFT    2

/*------------------------------------------------------------------------------
 * TimerPrescale register details
 */
#define TimerPrescale_REG_OFFSET    0x0C

/*
 * Prescale bits.
 */
#define Prescale_OFFSET   0x0C
#define Prescale_MASK     0x0000000F
#define Prescale_SHIFT    0

/*------------------------------------------------------------------------------
 * TimerIntClr register details
 */
#define TimerIntClr_REG_OFFSET  0x10

/*
 * TimerIntClr bits.
 */
#define TimerIntClr_OFFSET   0x10
#define TimerIntClr_MASK     0xFFFFFFFF
#define TimerIntClr_SHIFT    0

/*------------------------------------------------------------------------------
 * TimerRIS register details
 */
#define TimerRIS_REG_OFFSET 0x14

/*
 * RawTimerInterrupt bits.
 */
#define RawTimerInterrupt_OFFSET   0x14
#define RawTimerInterrupt_MASK     0x00000001
#define RawTimerInterrupt_SHIFT    0

/*------------------------------------------------------------------------------
 * TimerMIS register details
 */
#define TimerMIS_REG_OFFSET 0x18

/*
 * TimerInterrupt bits.
 */
#define TimerInterrupt_OFFSET   0x18
#define TimerInterrupt_MASK     0x00000001
#define TimerInterrupt_SHIFT    0

#endif /* __CORE_TIMER_REGISTERS */
