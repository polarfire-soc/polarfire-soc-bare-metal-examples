/*******************************************************************************
 * Copyright 2008 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_pwm_regs.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief IP core registers definitions. This file contains the definitions 
 * required for accessing the IP core through the hardware abstraction layer(HAL).
 *
 */
#ifndef COREPWM_REGISTERS_H_
#define COREPWM_REGISTERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * PRESCALE register:
 *------------------------------------------------------------------------------
 * The system clock cycle is multiplied by the PRESCALE value, resulting in the 
 * minimum PERIOD count timebase  
 */
#define PRESCALE_REG_OFFSET 0x00u

/*******************************************************************************
 * PERIOD register:
 *------------------------------------------------------------------------------
 * The PRESCALE value is multiplied by the PERIOD value, yielding the PWM 
 * waveform cycle.  Example: system clock = 40 ns, PRESCALE register = 255, 
 * PERIOD register = 127.  The PWM waveforms will repeat every 40 ns x 256 x 
 * 128 = 1.31 ms. The resolution of the PWM waveforms will be 1.31ms / 128 = 
 * 10.23 micros.  R/W 0h08
 */
#define PERIOD_REG_OFFSET   0x04u

/*******************************************************************************
 * PWM_ENABLE_1 register:
 *------------------------------------------------------------------------------
 * '1' enables each PWM output. R/W 0h00
 */
#define PWM_ENABLE_1_REG_OFFSET 0x08u

/*******************************************************************************
 * PWM_ENABLE_2 register:
 *------------------------------------------------------------------------------
 * '1' enables each PWM output. R/W 0h00
 */
#define PWM_ENABLE_2_REG_OFFSET 0x0Cu


/*******************************************************************************
 * PWM1_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM1 output with respect to the PERIOD resolution 
 */
#define PWM1_POSEDGE_REG_OFFSET 0x10u

/*******************************************************************************
 * PWM1_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM1 output with respect to the PERIOD resolution 
 */
#define PWM1_NEGEDGE_DACLEV_REG_OFFSET  0x14u

/*******************************************************************************
 * PWM2_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM2 output with respect to the PERIOD resolution 
 */
#define PWM2_POSEDGE_REG_OFFSET 0x18u

/*******************************************************************************
 * PWM2_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM2 output with respect to the PERIOD resolution 
 */
#define PWM2_NEGEDGE_DACLEV_REG_OFFSET  0x1Cu

/*******************************************************************************
 * PWM3_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM3 output with respect to the PERIOD resolution 
 */
#define PWM3_POSEDGE_REG_OFFSET 0x20u

/*******************************************************************************
 * PWM3_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM3 output with respect to the PERIOD resolution 
 */
#define PWM3_NEGEDGE_DACLEV_REG_OFFSET  0x24u

/*******************************************************************************
 * PWM4_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM4 output with respect to the PERIOD resolution 
 */
#define PWM4_POSEDGE_REG_OFFSET 0x28u

/*******************************************************************************
 * PWM4_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM4 output with respect to the PERIOD resolution 
 */
#define PWM4_NEGEDGE_DACLEV_REG_OFFSET  0x2Cu

/*******************************************************************************
 * PWM5_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM5 output with respect to the PERIOD resolution 
 */
#define PWM5_POSEDGE_REG_OFFSET 0x30u

/*******************************************************************************
 * PWM5_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM5 output with respect to the PERIOD resolution 
 */
#define PWM5_NEGEDGE_DACLEV_REG_OFFSET  0x34u

/*******************************************************************************
 * PWM6_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM6 output with respect to the PERIOD resolution 
 */
#define PWM6_POSEDGE_REG_OFFSET 0x38u

/*******************************************************************************
 * PWM6_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM6 output with respect to the PERIOD resolution 
 */
#define PWM6_NEGEDGE_DACLEV_REG_OFFSET  0x3Cu

/*******************************************************************************
 * PWM7_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM7 output with respect to the PERIOD resolution 
 */
#define PWM7_POSEDGE_REG_OFFSET 0x40u

/*******************************************************************************
 * PWM7_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM7 output with respect to the PERIOD resolution 
 */
#define PWM7_NEGEDGE_DACLEV_REG_OFFSET  0x44u

/*******************************************************************************
 * PWM8_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM8 output with respect to the PERIOD resolution 
 */
#define PWM8_POSEDGE_REG_OFFSET 0x48u

/*******************************************************************************
 * PWM8_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM8 output with respect to the PERIOD resolution 
 */
#define PWM8_NEGEDGE_DACLEV_REG_OFFSET  0x4Cu

/*******************************************************************************
 * PWM9_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM9 output with respect to the PERIOD resolution 
 */
#define PWM9_POSEDGE_REG_OFFSET 0x50u

/*******************************************************************************
 * PWM9_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM9 output with respect to the PERIOD resolution 
 */
#define PWM9_NEGEDGE_DACLEV_REG_OFFSET  0x54u

/*******************************************************************************
 * PWM10_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM10 output with respect to the PERIOD resolution 
 */
#define PWM10_POSEDGE_REG_OFFSET    0x58u

/*******************************************************************************
 * PWM10_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM10 output with respect to the PERIOD resolution 
 */
#define PWM10_NEGEDGE_DACLEV_REG_OFFSET 0x5Cu

/*******************************************************************************
 * PWM11_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM11 output with respect to the PERIOD resolution 
 */
#define PWM11_POSEDGE_REG_OFFSET    0x60u

/*******************************************************************************
 * PWM11_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM11 output with respect to the PERIOD resolution 
 */
#define PWM11_NEGEDGE_DACLEV_REG_OFFSET 0x64u

/*******************************************************************************
 * PWM12_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM12 output with respect to the PERIOD resolution 
 */
#define PWM12_POSEDGE_REG_OFFSET    0x68u

/*******************************************************************************
 * PWM12_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM12 output with respect to the PERIOD resolution 
 */
#define PWM12_NEGEDGE_DACLEV_REG_OFFSET 0x6Cu

/*******************************************************************************
 * PWM13_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM13 output with respect to the PERIOD resolution 
 */
#define PWM13_POSEDGE_REG_OFFSET    0x70u

/*******************************************************************************
 * PWM13_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM13 output with respect to the PERIOD resolution 
 */
#define PWM13_NEGEDGE_DACLEV_REG_OFFSET 0x74u

/*******************************************************************************
 * PWM14_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM14 output with respect to the PERIOD resolution 
 */
#define PWM14_POSEDGE_REG_OFFSET    0x78u

/*******************************************************************************
 * PWM14_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM14 output with respect to the PERIOD resolution 
 */
#define PWM14_NEGEDGE_DACLEV_REG_OFFSET 0x7Cu

/*******************************************************************************
 * PWM15_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM15 output with respect to the PERIOD resolution 
 */
#define PWM15_POSEDGE_REG_OFFSET    0x80u

/*******************************************************************************
 * PWM15_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM15 output with respect to the PERIOD resolution 
 */
#define PWM15_NEGEDGE_DACLEV_REG_OFFSET 0x84u

/*******************************************************************************
 * PWM16_POSEDGE register:
 *------------------------------------------------------------------------------
 * Sets positive edge of PWM16 output with respect to the PERIOD resolution 
 */
#define PWM16_POSEDGE_REG_OFFSET    0x88u

/*******************************************************************************
 * PWM16_NEGEDGE_DACLEV register:
 *------------------------------------------------------------------------------
 * Sets negative edge of PWM16 output with respect to the PERIOD resolution 
 */
#define PWM16_NEGEDGE_DACLEV_REG_OFFSET 0x8Cu

/*******************************************************************************
 * SYNC_UPDATE register:
 *------------------------------------------------------------------------------
 * When this bit is set to "1" and SHADOW_REG_EN is selected, all POSEDGE and 
 * NEGEDGE registers are updated synchronously. Synchronous updates to the PWM 
 * waveform occur only when SHADOW_REG_EN is asserted and SYNC_UPDATE is set to 
 * "1".When this bit is set to "0", all the POSEDGE and NEGEDGE registers are 
 * updated asynchronously. 
 */
#define SYNC_UPDATE_REG_OFFSET           0xE4u

/*******************************************************************************
 * PWM_STRETCH register:
 *------------------------------------------------------------------------------
 * When 0, the state of PWMx is determined by PWMx_POSEDGE NEGEDGE register 
 *         settings.
 * When 1, PWMx is set to PWM_STRETCH_VALUEx.
 */
#define PWM_STRETCH_REG_OFFSET            0x90u

/*******************************************************************************
 * TACHPRESCALE register:
 *------------------------------------------------------------------------------
 * Clock prescale setting. Determines effective clock rate for the counter
 * based on PCLK:
 * 0000 = divide by 1 (default)
 * 0001 = divide by 2
 * 0010 = divide by 4
 * 0011 = divide by 8
 * 0100 = divide by 16
 * 0101 = divide by 32
 * 0110 = divide by 64
 * 0111 = divide by 128
 * 1000 = divide by 256
 * 1001 = divide by 512
 * 1010 = divide by 1,024
 * 1011 = divide by 2,048
 * Others = divide by 2,048
 */
#define TACHPRESCALE_REG_OFFSET       0x94u

/*******************************************************************************
 * TACHSTATUS register:
 *------------------------------------------------------------------------------
 * TACH status register which contains one bit per TACH input, indicating
 * whether the respective TACHPULSEDUR register has been updated at
 * least once since the bit was cleared. The bits in this register gets cleared
 * by writing "1", "0" does not have any effect. 
 */
#define TACHSTATUS_REG_OFFSET            0x98u

/*******************************************************************************
 * TACHIRQMASK register:
 *------------------------------------------------------------------------------
 * TACH interrupt mask register with one bit per tachometer signal,
 * indicating whether CorePWM needs to assert an interrupt if the respective
 * bit in TACHSTATUS register is asserted.
 */
#define TACHIRQMASK_REG_OFFSET           0x9Cu

/*******************************************************************************
 * TACHMODE register:
 *------------------------------------------------------------------------------
 * TACH Mode. Sets the measurement mode used for each TACH input.
 * When 0: TACH input is continuously measured and stored in the respective 
 * TACHPULSEDUR register .
 * When 1: A one-time measurement is performed only if the respective bit in
 * TACHSTATUS register is cleared
 */
#define TACHMODE_REG_OFFSET              0xA0u

/*******************************************************************************
 * TACHPULSEDUR_0 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[0].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_0_REG_OFFSET         0xA4u

/*******************************************************************************
 * TACHPULSEDUR_1 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[1].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_1_REG_OFFSET         0xA8u

/*******************************************************************************
 * TACHPULSEDUR_2 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[2].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_2_REG_OFFSET         0xACu

/*******************************************************************************
 * TACHPULSEDUR_3 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[3].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_3_REG_OFFSET         0xB0u

/*******************************************************************************
 * TACHPULSEDUR_4 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[4].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_4_REG_OFFSET         0xB4u

/*******************************************************************************
 * TACHPULSEDUR_5 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[5].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_5_REG_OFFSET         0xB8u

/*******************************************************************************
 * TACHPULSEDUR_6 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[6].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_6_REG_OFFSET         0xBCu

/*******************************************************************************
 * TACHPULSEDUR_7 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[7].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_7_REG_OFFSET         0xC0u

/*******************************************************************************
 * TACHPULSEDUR_8 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[8].If the number of timer ticks exceeds 
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_8_REG_OFFSET         0xC4u

/*******************************************************************************
 * TACHPULSEDUR_9 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[9].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_9_REG_OFFSET         0xC8u

/*******************************************************************************
 * TACHPULSEDUR_10 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[10].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_10_REG_OFFSET        0xCCu

/*******************************************************************************
 * TACHPULSEDUR_11 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[11].If the number of timer ticks exceeds 
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_11_REG_OFFSET        0xD0u

/*******************************************************************************
 * TACHPULSEDUR_12 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[12].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_12_REG_OFFSET        0xD4u

/*******************************************************************************
 * TACHPULSEDUR_13 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[13].If the number of timer ticks exceeds 
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_13_REG_OFFSET        0xD8u

/*******************************************************************************
 * TACHPULSEDUR_14 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[14].If the number of timer ticks exceeds 
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_14_REG_OFFSET        0xDCu

/*******************************************************************************
 * TACHPULSEDUR_15 register:
 *------------------------------------------------------------------------------
 * Stores the number of timer ticks between two successive positive (or
 * negative) edges from the TACHIN[15].If the number of timer ticks exceeds
 * the maximum register value, the value of 0 shall be stored instead. 
 */
#define TACHPULSEDUR_15_REG_OFFSET        0xE0u

#ifdef __cplusplus
}
#endif

#endif /* COREPWM_REGISTERS_H_*/
