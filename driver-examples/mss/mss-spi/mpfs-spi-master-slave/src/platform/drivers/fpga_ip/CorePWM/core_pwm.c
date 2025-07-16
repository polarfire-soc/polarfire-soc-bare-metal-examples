/*******************************************************************************
 * Copyright 2008 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_pwm.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CorePWM driver implementation.
 *
 */
#include "core_pwm.h"
#include "corepwm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif
/*-------------------------------------------------------------------------*//**
 * Default definitions for TACH Interface
 */
#define COREPWM_TACHMODE_DEFAULT        0x0000u
#define COREPWM_TACHIRQMASK_DEFAULT     0x0000u
#define COREPWM_TACHSTATUS_DEFAULT      0xFFFFu

/*-------------------------------------------------------------------------*//**
 * Lookup table containing the bit mask used to enable/disable PWMs by writing
 * the PWM_ENABLE_1 and PWM_ENABLE_2 registers.
 */
static const uint8_t g_pwm_id_mask_lut[] =
{
    0x00u,
    0x01u,  /* PWM_1 */
    0x02u,  /* PWM_2 */
    0x04u,  /* PWM_3 */
    0x08u,  /* PWM_4 */
    0x10u,  /* PWM_5 */
    0x20u,  /* PWM_6 */
    0x40u,  /* PWM_7 */
    0x80u,  /* PWM_8 */
    0x01u,  /* PWM_9 */
    0x02u,  /* PWM_10 */
    0x04u,  /* PWM_11 */
    0x08u,  /* PWM_12 */
    0x10u,  /* PWM_13 */
    0x20u,  /* PWM_14 */
    0x40u,  /* PWM_15 */
    0x80u,  /* PWM_16 */
};

/*-------------------------------------------------------------------------*//**
 * Lookup table containing the bit mask used to find out the TachInput number
 * for TachInterface
 */
static const uint16_t g_pwm_tach_id_mask_lut[] =
{
    0x0000u,
    0x0001u,    /* PWM_TACH_1 */
    0x0002u,    /* PWM_TACH_2 */
    0x0004u,    /* PWM_TACH_3 */
    0x0008u,    /* PWM_TACH_4 */
    0x0010u,    /* PWM_TACH_5 */
    0x0020u,    /* PWM_TACH_6 */
    0x0040u,    /* PWM_TACH_7 */
    0x0080u,    /* PWM_TACH_8 */
    0x0100u,    /* PWM_TACH_9 */
    0x0200u,    /* PWM_TACH_10 */
    0x0400u,    /* PWM_TACH_11 */
    0x0800u,    /* PWM_TACH_12 */
    0x1000u,    /* PWM_TACH_13 */
    0x2000u,    /* PWM_TACH_14 */
    0x4000u,    /* PWM_TACH_15 */
    0x8000u,    /* PWM_TACH_16 */
};

/*-------------------------------------------------------------------------*//**
 * Lookup table containing the offset of the registers used to control the
 * positive edge of a PWM. The LUT is indexed on the PWM number from 1 to 16.
 */
static const addr_t g_pwm_posedge_offset_lut[] =
{
    0x0000u,
    PWM1_POSEDGE_REG_OFFSET,   /* PWM_1 */
    PWM2_POSEDGE_REG_OFFSET,   /* PWM_2 */
    PWM3_POSEDGE_REG_OFFSET,   /* PWM_3 */
    PWM4_POSEDGE_REG_OFFSET,   /* PWM_4 */
    PWM5_POSEDGE_REG_OFFSET,   /* PWM_5 */
    PWM6_POSEDGE_REG_OFFSET,   /* PWM_6 */
    PWM7_POSEDGE_REG_OFFSET,   /* PWM_7 */
    PWM8_POSEDGE_REG_OFFSET,   /* PWM_8 */
    PWM9_POSEDGE_REG_OFFSET,   /* PWM_9 */
    PWM10_POSEDGE_REG_OFFSET,  /* PWM_10 */
    PWM11_POSEDGE_REG_OFFSET,  /* PWM_11 */
    PWM12_POSEDGE_REG_OFFSET,  /* PWM_12 */
    PWM13_POSEDGE_REG_OFFSET,  /* PWM_13 */
    PWM14_POSEDGE_REG_OFFSET,  /* PWM_14 */
    PWM15_POSEDGE_REG_OFFSET,  /* PWM_15 */
    PWM16_POSEDGE_REG_OFFSET,  /* PWM_16 */
};

/*-------------------------------------------------------------------------*//**
 * Lookup table containing the offset of the registers used to control the
 * negative edge of a PWM. The LUT is indexed on the PWM number from 1 to 16.
 */
static const addr_t g_pwm_negedge_offset_lut[] =
{
    0x0000u,
    PWM1_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_1 */
    PWM2_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_2 */
    PWM3_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_3 */
    PWM4_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_4 */
    PWM5_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_5 */
    PWM6_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_6 */
    PWM7_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_7 */
    PWM8_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_8 */
    PWM9_NEGEDGE_DACLEV_REG_OFFSET,     /* PWM_9 */
    PWM10_NEGEDGE_DACLEV_REG_OFFSET,    /* PWM_10 */
    PWM11_NEGEDGE_DACLEV_REG_OFFSET,    /* PWM_11 */
    PWM12_NEGEDGE_DACLEV_REG_OFFSET,    /* PWM_12 */
    PWM13_NEGEDGE_DACLEV_REG_OFFSET,    /* PWM_13 */
    PWM14_NEGEDGE_DACLEV_REG_OFFSET,    /* PWM_14 */
    PWM15_NEGEDGE_DACLEV_REG_OFFSET,    /* PWM_15 */
    PWM16_NEGEDGE_DACLEV_REG_OFFSET,    /* PWM_16 */
};

/*-------------------------------------------------------------------------*//**
 * Lookup table containing the offset of the registers used to stores the
 * number of timer ticks between two successive positive (or negative) edges
 * from the TACHIN[i] The LUT is indexed on the Tachometer Input number from
 * 1 to 16.
 */
static const addr_t g_tachpulsedur_offset_lut[] =
{
    0x0000u,
    TACHPULSEDUR_0_REG_OFFSET,  /* PWM_TACH_1 */
    TACHPULSEDUR_1_REG_OFFSET,  /* PWM_TACH_2 */
    TACHPULSEDUR_2_REG_OFFSET,  /* PWM_TACH_3 */
    TACHPULSEDUR_3_REG_OFFSET,  /* PWM_TACH_4 */
    TACHPULSEDUR_4_REG_OFFSET,  /* PWM_TACH_5 */
    TACHPULSEDUR_5_REG_OFFSET,  /* PWM_TACH_6 */
    TACHPULSEDUR_6_REG_OFFSET,  /* PWM_TACH_7 */
    TACHPULSEDUR_7_REG_OFFSET,  /* PWM_TACH_8 */
    TACHPULSEDUR_8_REG_OFFSET,  /* PWM_TACH_9 */
    TACHPULSEDUR_9_REG_OFFSET,  /* PWM_TACH_10 */
    TACHPULSEDUR_10_REG_OFFSET, /* PWM_TACH_11 */
    TACHPULSEDUR_11_REG_OFFSET, /* PWM_TACH_12 */
    TACHPULSEDUR_12_REG_OFFSET, /* PWM_TACH_13 */
    TACHPULSEDUR_13_REG_OFFSET, /* PWM_TACH_14 */
    TACHPULSEDUR_14_REG_OFFSET, /* PWM_TACH_15 */
    TACHPULSEDUR_15_REG_OFFSET, /* PWM_TACH_16 */
};

/*-------------------------------------------------------------------------*//**
 * PWM_init()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_init
(
    pwm_instance_t * pwm_inst,
    addr_t base_addr,
    uint32_t prescale,
    uint32_t period
)
{
    pwm_inst->address = base_addr;

    HAL_set_8bit_reg( pwm_inst->address, PWM_ENABLE_1, 0u );
    HAL_set_8bit_reg( pwm_inst->address, PWM_ENABLE_2, 0u );

    HAL_set_32bit_reg( pwm_inst->address, PRESCALE, (uint_fast32_t)prescale );

    /*
     * The minimum allowed period parameter value is 1.
     * This simplifies the duty cycle and edge value calculations for the driver.
     */
    HAL_ASSERT( period >= 1 )

    HAL_set_32bit_reg( pwm_inst->address, PERIOD, (uint_fast32_t)period );

    /* Set positive edge to 0 for all PWMs. */
    HAL_set_32bit_reg( pwm_inst->address, PWM1_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM2_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM3_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM4_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM5_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM6_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM7_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM8_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM9_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM10_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM11_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM12_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM13_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM14_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM15_POSEDGE, 0u );
    HAL_set_32bit_reg( pwm_inst->address, PWM16_POSEDGE, 0u );
}

/*-------------------------------------------------------------------------*//**
 * PWM_enable()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_enable
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id
)
{
    uint8_t pwm_enables;
    uint8_t pwm_id_mask;

    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    if ( (pwm_id >= PWM_1) && (pwm_id <= PWM_16 ))
    {
        pwm_id_mask = g_pwm_id_mask_lut[pwm_id];

        if ( pwm_id < PWM_9 )
        {
            pwm_enables = HAL_get_8bit_reg( pwm_inst->address, PWM_ENABLE_1 );
            pwm_enables |= pwm_id_mask;
            HAL_set_8bit_reg
                (
                  pwm_inst->address,
                  PWM_ENABLE_1,
                  (uint_fast8_t)pwm_enables
                 );
        }
        else
        {
            pwm_enables = HAL_get_8bit_reg( pwm_inst->address, PWM_ENABLE_2 );
            pwm_enables |= pwm_id_mask;
            HAL_set_8bit_reg
                (
                  pwm_inst->address,
                  PWM_ENABLE_2,
                  (uint_fast8_t)pwm_enables
                );
        }
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_disable()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_disable
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id
)
{
    uint8_t pwm_enables;
    uint8_t pwm_id_mask;

    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    if ( (pwm_id >= PWM_1) && (pwm_id <= PWM_16 ))
    {
        pwm_id_mask = g_pwm_id_mask_lut[pwm_id];

        if ( pwm_id < PWM_9 )
        {
            pwm_enables = HAL_get_8bit_reg( pwm_inst->address, PWM_ENABLE_1 );
            pwm_enables &= (uint8_t)~pwm_id_mask;
            HAL_set_8bit_reg
                (
                  pwm_inst->address,
                  PWM_ENABLE_1,
                  (uint_fast8_t)pwm_enables
                 );
        }
        else
        {
            pwm_enables = HAL_get_8bit_reg( pwm_inst->address, PWM_ENABLE_2 );
            pwm_enables &= (uint8_t)~pwm_id_mask;
            HAL_set_8bit_reg
                (
                  pwm_inst->address,
                  PWM_ENABLE_2,
                  (uint_fast8_t)pwm_enables
                );
        }
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_enable_synch_update()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_enable_synch_update
(
    pwm_instance_t * pwm_inst
)
{
    HAL_set_16bit_reg( pwm_inst->address, SYNC_UPDATE, 1u );

    /*
     * Assertion will ensure PWM feature has been enabled for CorePWM
     * hardware instance.
     */
    HAL_ASSERT( HAL_get_16bit_reg( pwm_inst->address, SYNC_UPDATE ) == 1u)
}

/*-------------------------------------------------------------------------*//**
 * PWM_disable_synch_update()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_disable_synch_update
(
   pwm_instance_t * pwm_inst
)
{
    HAL_set_16bit_reg( pwm_inst->address, SYNC_UPDATE, 0u );

    /*
     * Assertion will ensure PWM feature has been enabled for CorePWM
     * hardware instance.
     */
    HAL_ASSERT( HAL_get_16bit_reg( pwm_inst->address, SYNC_UPDATE ) == 0u)
}

/*-------------------------------------------------------------------------*//**
 * PWM_set_duty_cycle()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_set_duty_cycle
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id,
    uint32_t duty_cycle
)
{

    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    if ( (pwm_id >= PWM_1) && (pwm_id <= PWM_16 ))
    {
        if ( duty_cycle == 0u )
        {
            PWM_disable( pwm_inst, pwm_id );
        }
        else
        {
            HW_set_32bit_reg
               (
                    pwm_inst->address + g_pwm_posedge_offset_lut[pwm_id],
                    0u
               );

            HW_set_32bit_reg
               (
                 pwm_inst->address + g_pwm_negedge_offset_lut[pwm_id],
                 (uint_fast32_t)duty_cycle
               );

            /*
             * Assertion will ensure PWM feature has been enabled for CorePWM
             * hardware instance.
             */
#ifndef NDEBUG
        {
            uint8_t edge_value ;
            addr_t neg_addr;

            neg_addr = g_pwm_negedge_offset_lut[pwm_id] ;
            edge_value = HW_get_8bit_reg(pwm_inst->address + neg_addr );
            HAL_ASSERT( edge_value == (uint8_t)duty_cycle )
        }
#endif
            PWM_enable( pwm_inst, pwm_id );
        }
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_set_edges()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_set_edges
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id,
    uint32_t pos_edge,
    uint32_t neg_edge
)
{
    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    /*
     * Assertion will ensure pos_edge & neg_edge is less than or equal to
     * period value.
     */
#ifndef NDEBUG
  {
    uint32_t period ;
    period = HAL_get_32bit_reg( pwm_inst->address, PERIOD );
    HAL_ASSERT( pos_edge <= period );
    HAL_ASSERT( neg_edge <= period );
  }
#endif

    if ( (pwm_id >= PWM_1) && (pwm_id <= PWM_16 ))
    {
        HW_set_32bit_reg
           (
             pwm_inst->address + g_pwm_posedge_offset_lut[pwm_id],
             (uint_fast32_t)pos_edge
            );

        /*
         * Assertion will ensure PWM feature has been enabled for CorePWM
         * hardware instance.
         */
#ifndef NDEBUG
    {
        uint8_t edge_value ;
        addr_t pos_addr;

        pos_addr = g_pwm_posedge_offset_lut[pwm_id];
        edge_value = HW_get_8bit_reg(pwm_inst->address + pos_addr );
        HAL_ASSERT( edge_value == (uint8_t)pos_edge )
    }
#endif

        HW_set_32bit_reg
           (
             pwm_inst->address + g_pwm_negedge_offset_lut[pwm_id],
             (uint_fast32_t)neg_edge
            );

        /*
         * Assertion will ensure PWM feature has been enabled for CorePWM
         * hardware instance.
         */
#ifndef NDEBUG
    {
        uint8_t edge_value ;
        addr_t neg_addr;

        neg_addr = g_pwm_negedge_offset_lut[pwm_id];
        edge_value = HW_get_8bit_reg(pwm_inst->address + neg_addr );
        HAL_ASSERT( edge_value == (uint8_t)neg_edge )
    }
#endif
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_get_duty_cycle()
 * See "core_pwm.h" for details of how to use this function.
 */
uint32_t PWM_get_duty_cycle
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id
)
{
    uint32_t pos_edge ;
    uint32_t neg_edge ;
    uint32_t duty_cycle = 0u;
    uint32_t period ;
    uint8_t pwm_enables;
    uint8_t pwm_id_mask;

    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    if ((pwm_id >= PWM_1) && (pwm_id <= PWM_16 ))
    {
        pwm_id_mask = g_pwm_id_mask_lut[pwm_id];

        /* Find out if the PWM output is enabled or disabled */
        if (pwm_id < PWM_9)
        {
            pwm_enables = HAL_get_8bit_reg( pwm_inst->address, PWM_ENABLE_1 );
            pwm_enables &= pwm_id_mask;
        }
        else
        {
            pwm_enables = HAL_get_8bit_reg( pwm_inst->address, PWM_ENABLE_2 );
            pwm_enables &= pwm_id_mask;
        }

        /*
         * If the PWM output is disabled, the duty cycle is implicitly 0,
         * so allow the initial duty_cycle = 0u assignment fall through to
         * the return() statement.
         *   NOTE: The PWM_set_duty_cycle() and PWM_generate_aligned_wave()
         *   functions disable the PWM output when a duty cycle = 0 is
         *   requested.
         *
         * Otherwise the PWM output is enabled, so read the positive and
         * negative edge and period registers and calculate the duty cycle.
         */
        if (pwm_enables)
        {
            pos_edge = HW_get_32bit_reg
                        (
                          pwm_inst->address + g_pwm_posedge_offset_lut[pwm_id]
                        );

            neg_edge = HW_get_32bit_reg
                        (
                          pwm_inst->address + g_pwm_negedge_offset_lut[pwm_id]
                        );

            period = HAL_get_32bit_reg( pwm_inst->address, PERIOD );

            /*
             * Calculate the duty cycle from the edge and period values.
             *
             * If pos_edge = neg_edge, this is the setting for PWM output
             * toggle mode (50% duty cycle). For a description of toggle mode,
             * see CorePWM Handbook, Fig1-3, PWM4.
             *   NOTE: The PWM_set_edges() function can set pos_edge = neg_edge
             *   and this is also the reset state for the edge registers. The
             *   PWM_set_duty_cycle() and PWM_generate_aligned_wave() functions
             *   cannot set pos_edge = neg_edge, instead they simply disable
             *   the PWM output when duty_cycle = 0 is requested.
             */
            if (pos_edge <= neg_edge)
            {
                duty_cycle = neg_edge - pos_edge ;
            }
            else
            {
                duty_cycle = (period - (pos_edge - neg_edge))+1u ;
            }
        }
    }

    return(duty_cycle);
}

/*-------------------------------------------------------------------------*//**
 PWM_generate_aligned_wave()
  Algorithm for each Aligned wave type follows as below:
  Parameters used in algorithm -
  pos_edge: Specifies the value of the period counter at which the
            PWM output identified by pwm_id will rise from low to high.
  neg_edge: Specifies the value of the period counter at which the
            PWM output identified by pwm_id will fall from  high to low.

  Left Aligned Waveform -    pos_edge    0
                             neg_edge    Duty_cycle

  Right Aligned Waveform -   pos_edge    PWM_Period+1- Duty_cycle
                             neg_edge    0

  Central Aligned Waveform -
        There are two cases :
        Case 1: When Period + Duty_cycle =  mathematical odd number
                             pos_edge    (PWM_Period +1 - duty_cycle)/2
                             neg_edge    (PWM_Period +1 + duty_cycle)/2

        Case 2: When Period + Duty_cycle = mathematical even number
               In this scenario, there cannot be exact central waveform,
               however, if pwm period is larger then the difference will
               not be visible.
               There can be two possible type of central waveform.
               a) Left Centered Waveform
               b) Right Centered Waveform
               Basically, these will be central waveforms only but
               slightly shifted towards left or right side. Formulae
               for the same are below:

               a) Left Centered Aligned Waveform:
                              pos_edge   (PWM_PERIOD -Duty_cycle)/2+1
                             neg_edge   (PWM_PERIOD +Duty_cycle)/2+1
               b) Right Centered Aligned Waveform:
                              pos_edge   (PWM_PERIOD -Duty_cycle)/2
                             neg_edge   (PWM_PERIOD +Duty_cycle)/2

  See "core_pwm.h" for more details of how to use this function.
 */
void PWM_generate_aligned_wave
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id,
    uint32_t duty_cycle,
    pwm_wave_align_t alignment_type
)
{
    uint32_t period;
    uint32_t pos_edge = 0u;
    uint32_t neg_edge = 0u;

    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    if( (pwm_id >= PWM_1) && (pwm_id <= PWM_16) )
    {
        period = HAL_get_32bit_reg( pwm_inst->address, PERIOD );

        /*
         * Assertion will ensure duty cycle is less than or equal to
         * period value.
         */
        HAL_ASSERT( duty_cycle <= period );

        if( 0u == duty_cycle)
        {
            /*
             * For a duty cycle of 0, we cannot set pos_edge = neg_edge because
             * this is the setting for PWM output toggle mode (50% duty cycle).
             * See CorePWM Handbook, Fig1-3, PWM4, for a description of toggle
             * mode.
             * Instead, for a duty cycle of 0, disable the PWM output.
             */
            PWM_disable( pwm_inst, pwm_id );
        }
        else
        {
            switch(alignment_type)
            {
                case PWM_LEFT_ALIGN :
                    pos_edge = 0u;
                    neg_edge = duty_cycle;
                    break;

                case PWM_CENTER_ALIGN :
                    if( (uint32_t)0x1 & (period ^ duty_cycle))
                    {
                        /*
                         * If the sum of period + duty_cycle is
                         * an odd number, then the duty cycle can
                         * be exactly centered in the period. The
                         * test for an "odd sum" may be expressed as,
                         * (lsb of period) XOR (lsb of duty_cycle) = 1
                         */
                        pos_edge = ((period - duty_cycle) +1u) >> 1 ;
                        neg_edge = pos_edge + duty_cycle ;
                    }
                    /*
                     * Else the sum of period + duty_cycle is an
                     * even number
                     */
                    else
                    {
#ifdef ALTERNATE_CENTRAL_ALIGNED_WAVE

                        /* case 1 -- right Shift */
                        pos_edge = ((period - duty_cycle)>>1) +1u ;

                        if ( period != duty_cycle )
                        {
                            neg_edge = pos_edge + duty_cycle ;
                        }
                        else
                        {
                            /*
                             * When period = duty_cycle, we’ve reached the
                             * right shift limit for the neg_edge
                             */
                            neg_edge = 0u;
                        }
#else
                        /* case 2 -- left Shift */
                        pos_edge = (period - duty_cycle) >> 1 ;
                        neg_edge = pos_edge + duty_cycle ;
#endif
                    }
                    break;

                case PWM_RIGHT_ALIGN :
                    pos_edge = (period-duty_cycle) + 1u;
                    neg_edge = 0u ;
                    break;

                default :
                    break ;
            }

            HW_set_32bit_reg
               (
                  pwm_inst->address + g_pwm_posedge_offset_lut[pwm_id],
                  (uint_fast32_t)pos_edge
               );

            HW_set_32bit_reg
               (
                  pwm_inst->address + g_pwm_negedge_offset_lut[pwm_id],
                  (uint_fast32_t)neg_edge
               );

            /*
             * Assertion will ensure PWM feature has been enabled for CorePWM
             * hardware instance.
             */
#ifndef NDEBUG
            {
            uint8_t edge_value ;
            addr_t edge_addr;

            edge_addr = g_pwm_posedge_offset_lut[pwm_id];
            edge_value = HW_get_8bit_reg(pwm_inst->address + edge_addr );
            HAL_ASSERT( edge_value == (uint8_t)pos_edge )

            edge_addr = g_pwm_negedge_offset_lut[pwm_id];
            edge_value = HW_get_8bit_reg(pwm_inst->address + edge_addr );
            HAL_ASSERT( edge_value == (uint8_t)neg_edge )
            }
#endif
            PWM_enable( pwm_inst, pwm_id );
        }
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_enable_stretch_pulse()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_enable_stretch_pulse
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id
)
{
    uint16_t stretch_value ;
    uint16_t pwm_id_mask ;

    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    if ( (pwm_id >= PWM_1) && (pwm_id <= PWM_16 ))
    {
        pwm_id_mask = g_pwm_tach_id_mask_lut[pwm_id];

        stretch_value = HAL_get_16bit_reg( pwm_inst->address,PWM_STRETCH );
        stretch_value |= pwm_id_mask;

        HAL_set_16bit_reg
            (
              pwm_inst->address,
              PWM_STRETCH,
              (uint_fast16_t)stretch_value
             );

        /*
         * Assertion will ensure Tachometer feature has been enabled
         * for CorePWM hardware instance.
         */
#ifndef NDEBUG
    {
        uint16_t pwm_stretch ;
        pwm_stretch = HAL_get_16bit_reg( pwm_inst->address, PWM_STRETCH );
        HAL_ASSERT( pwm_stretch == stretch_value )
    }
#endif
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_disable_stretch_pulse()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_disable_stretch_pulse
(
    pwm_instance_t * pwm_inst,
    pwm_id_t pwm_id
)
{
    uint16_t stretch_value ;
    uint16_t pwm_id_mask ;

    /* Assertion will ensure correct PWM output has been selected. */
    HAL_ASSERT( pwm_id >= PWM_1 )
    HAL_ASSERT( pwm_id <= PWM_16 )

    if ( (pwm_id >= PWM_1) && (pwm_id <= PWM_16 ))
    {
        pwm_id_mask = g_pwm_tach_id_mask_lut[pwm_id];

        stretch_value = HAL_get_16bit_reg( pwm_inst->address,PWM_STRETCH );
        stretch_value &= (uint16_t)~pwm_id_mask;

        HAL_set_16bit_reg
            (
              pwm_inst->address,
              PWM_STRETCH,
              (uint_fast16_t)stretch_value
            );

        /*
         * Assertion will ensure Tachometer feature has been enabled
         * for CorePWM hardware instance.
         */
#ifndef NDEBUG
    {
        uint16_t pwm_stretch ;
        pwm_stretch = HAL_get_16bit_reg( pwm_inst->address, PWM_STRETCH );
        HAL_ASSERT( pwm_stretch == stretch_value )
    }
#endif
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_init()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_tach_init
(
    pwm_instance_t * pwm_inst,
    pwm_tach_prescale_t tach_prescale
)
{
    HAL_set_16bit_reg
        (
          pwm_inst->address,
          TACHPRESCALE,
          (uint_fast16_t)tach_prescale
         );

    /*
     * Assertion will ensure Tachometer feature has been enabled
     * for CorePWM hardware instance.
     */
#ifndef NDEBUG
{
    uint16_t prescale ;
    prescale = HAL_get_16bit_reg(pwm_inst->address,TACHPRESCALE);
    HAL_ASSERT( (pwm_tach_prescale_t)prescale == tach_prescale )
}
#endif

    /*
     * Tachometer mode and IRQ mask registers are updated with default value.
     * So no need to check assertion.
     */
    HAL_set_16bit_reg(pwm_inst->address,TACHMODE,COREPWM_TACHMODE_DEFAULT );
    HAL_set_16bit_reg(pwm_inst->address,TACHIRQMASK,COREPWM_TACHIRQMASK_DEFAULT);

    /* Clear any pending interrupts for all the tachometer inputs.*/
    HAL_set_16bit_reg(pwm_inst->address,TACHSTATUS,COREPWM_TACHSTATUS_DEFAULT );

}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_set_mode()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_tach_set_mode
(
    pwm_instance_t * pwm_inst,
    pwm_tach_id_t  pwm_tach_id,
    uint16_t       pwm_tachmode
)
{
    uint16_t pwm_tach_config;
    uint16_t pwm_tach_id_mask;

    /* Assertion will ensure correct tachometer input has been selected. */
    HAL_ASSERT( pwm_tach_id >= PWM_TACH_1  )
    HAL_ASSERT( pwm_tach_id <= PWM_TACH_16 )

    if ( (pwm_tach_id >= PWM_TACH_1) && (pwm_tach_id <= PWM_TACH_16 ))
    {
        pwm_tach_id_mask = g_pwm_tach_id_mask_lut[pwm_tach_id];

        pwm_tach_config = HAL_get_16bit_reg( pwm_inst->address, TACHMODE );
        if (pwm_tachmode)
        {
            pwm_tach_config |= pwm_tach_id_mask;
        }
        else
        {
            pwm_tach_config &= (uint16_t)~pwm_tach_id_mask;
        }

        HAL_set_16bit_reg(pwm_inst->address,TACHMODE,(uint_fast16_t)pwm_tach_config);

        /*
         * Assertion will ensure Tachometer feature has been enabled
         * for CorePWM hardware instance.
         */
#ifndef NDEBUG
    {
        uint16_t tach_mode;
        tach_mode = HAL_get_16bit_reg( pwm_inst->address, TACHMODE);
        HAL_ASSERT( tach_mode == pwm_tach_config )
    }
#endif
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_read_value()
 * See "core_pwm.h" for details of how to use this function.
 */

uint16_t PWM_tach_read_value
(
    pwm_instance_t * pwm_inst,
    pwm_tach_id_t    pwm_tach_id
)
{
    uint16_t tach_value = 0u;

    /* Assertion will ensure correct tachometer input has been selected. */
    HAL_ASSERT( pwm_tach_id >= PWM_TACH_1  )
    HAL_ASSERT( pwm_tach_id <= PWM_TACH_16 )

    if ((pwm_tach_id >= PWM_TACH_1) && (pwm_tach_id <= PWM_TACH_16 ))
    {
        tach_value = HW_get_16bit_reg
                        (
                          pwm_inst->address + g_tachpulsedur_offset_lut[pwm_tach_id]
                         );
    }
    return( tach_value );
}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_clear_status()
 * See "core_pwm.h" for details of how to use this function.
 */

void PWM_tach_clear_status
(
    pwm_instance_t * pwm_inst,
    pwm_tach_id_t    pwm_tach_id
)
{
    uint16_t pwm_tach_id_mask;

    /* Assertion will ensure correct tachometer input has been selected. */
    HAL_ASSERT( pwm_tach_id >= PWM_TACH_1  )
    HAL_ASSERT( pwm_tach_id <= PWM_TACH_16 )

    if ( (pwm_tach_id >= PWM_TACH_1) && (pwm_tach_id <= PWM_TACH_16 ))
    {
        pwm_tach_id_mask = g_pwm_tach_id_mask_lut[pwm_tach_id];

        /* 0 does not have any effect. So, write 1 to the right one */
        HAL_set_16bit_reg
            (
              pwm_inst->address ,
              TACHSTATUS,
              (uint_fast16_t) pwm_tach_id_mask
            );
    }
}

/***************************************************************************//**
 * PWM_tach_read_status ()
 * See "core_pwm.h" for details of how to use this function.
 */

uint16_t PWM_tach_read_status
(
    pwm_instance_t * pwm_inst,
    pwm_tach_id_t    pwm_tach_id
)
{
    uint16_t pwm_tach_id_mask ;
    uint16_t pwm_tach_status = 0u;

    /* Assertion will ensure correct tachometer input has been selected. */
    HAL_ASSERT( pwm_tach_id >= PWM_TACH_1  )
    HAL_ASSERT( pwm_tach_id <= PWM_TACH_16 )

    if((pwm_tach_id >= PWM_TACH_1) && (pwm_tach_id <= PWM_TACH_16 ))
    {
        pwm_tach_id_mask = g_pwm_tach_id_mask_lut[pwm_tach_id];
        pwm_tach_status = HAL_get_16bit_reg( pwm_inst->address , TACHSTATUS);
        pwm_tach_status = ( pwm_tach_status & pwm_tach_id_mask);
    }
    return ( pwm_tach_status );
}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_get_irq_source()
 * See "core_pwm.h" for details of how to use this function.
 */
pwm_tach_id_t PWM_tach_get_irq_source
(
    pwm_instance_t * pwm_inst
)
{
    uint16_t status;
    uint16_t irq_mask;
    pwm_tach_id_t tach_id;
    uint16_t n;

    irq_mask = HAL_get_16bit_reg( pwm_inst->address , TACHIRQMASK );
    status = HAL_get_16bit_reg( pwm_inst->address , TACHSTATUS );
    status = status & irq_mask;

    if(0u == status)
    {
        tach_id = PWM_TACH_INVALID;
    }
    else
    {
        n = 1u;
        if((status & 0x00FFu) == 0u) {n = n + 8u; status = status >> 8;}
        if((status & 0x000Fu) == 0u) {n = n + 4u; status = status >> 4;}
        if((status & 0x0003u) == 0u) {n = n + 2u; status = status >> 2;}
        if((status & 0x0001u) == 0u) {n = n + 1u;}
        tach_id = (pwm_tach_id_t)n ;
    }

    return tach_id;
}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_enable_irq()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_tach_enable_irq
(
    pwm_instance_t * pwm_inst,
    pwm_tach_id_t  pwm_tach_id
)
{
    uint16_t pwm_tach_irq;
    uint16_t pwm_tach_id_mask;

    /* Assertion will ensure correct tachometer input has been selected. */
    HAL_ASSERT( pwm_tach_id >= PWM_TACH_1  )
    HAL_ASSERT( pwm_tach_id <= PWM_TACH_16 )

    if ( (pwm_tach_id >= PWM_TACH_1) && (pwm_tach_id <= PWM_TACH_16 ))
    {
        pwm_tach_id_mask = g_pwm_tach_id_mask_lut[pwm_tach_id];
        pwm_tach_irq = HAL_get_16bit_reg( pwm_inst->address, TACHIRQMASK );
        pwm_tach_irq |= pwm_tach_id_mask;
        HAL_set_16bit_reg
            (
              pwm_inst->address ,
              TACHIRQMASK,
              (uint_fast16_t)pwm_tach_irq
             );

        /*
         * Assertion will ensure Tachometer feature has been enabled
         * for CorePWM hardware instance.
         */
#ifndef NDEBUG
    {
        uint16_t tach_irq ;
        tach_irq = HAL_get_16bit_reg( pwm_inst->address, TACHIRQMASK) ;
        HAL_ASSERT( tach_irq == pwm_tach_irq )
    }
#endif
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_disable_irq()
 * See "core_pwm.h" for details of how to use this function.
 */
void PWM_tach_disable_irq
(
    pwm_instance_t * pwm_inst,
    pwm_tach_id_t  pwm_tach_id
)
{
    uint16_t pwm_tach_irq;
    uint16_t pwm_tach_id_mask;

    /* Assertion will ensure correct tachometer input has been selected. */
    HAL_ASSERT( pwm_tach_id >= PWM_TACH_1  )
    HAL_ASSERT( pwm_tach_id <= PWM_TACH_16 )

    if ( (pwm_tach_id >= PWM_TACH_1) && (pwm_tach_id <= PWM_TACH_16 ))
    {
        pwm_tach_id_mask = g_pwm_tach_id_mask_lut[pwm_tach_id];
        pwm_tach_irq = HAL_get_16bit_reg( pwm_inst->address, TACHIRQMASK );
        pwm_tach_irq &= (uint16_t)~pwm_tach_id_mask;
        HAL_set_16bit_reg
            (
              pwm_inst->address ,
              TACHIRQMASK,
              (uint_fast16_t)pwm_tach_irq
             );

        /*
         * Assertion will ensure Tachometer feature has been enabled
         * for CorePWM hardware instance.
         */
#ifndef NDEBUG
    {
        uint16_t tach_irq ;
        tach_irq = HAL_get_16bit_reg( pwm_inst->address, TACHIRQMASK) ;
        HAL_ASSERT( tach_irq == pwm_tach_irq )
    }
#endif
    }
}

/*-------------------------------------------------------------------------*//**
 * PWM_tach_clear_irq()
 * See "core_pwm.h" for details of how to use this function.
 */

void PWM_tach_clear_irq
(
    pwm_instance_t * pwm_inst,
    pwm_tach_id_t    pwm_tach_id
)
{
    uint16_t pwm_tach_id_mask;

    /* Assertion will ensure correct tachometer input has been selected. */
    HAL_ASSERT( pwm_tach_id >= PWM_TACH_1  )
    HAL_ASSERT( pwm_tach_id <= PWM_TACH_16 )

    if ( (pwm_tach_id >= PWM_TACH_1) && (pwm_tach_id <= PWM_TACH_16 ))
    {
        pwm_tach_id_mask = g_pwm_tach_id_mask_lut[pwm_tach_id];

        /* 0 does not have any effect. So, write 1 to the right one. */
        HAL_set_16bit_reg
            (
              pwm_inst->address ,
              TACHSTATUS,
              (uint_fast16_t)pwm_tach_id_mask
             );
    }
}

#ifdef __cplusplus
}
#endif
