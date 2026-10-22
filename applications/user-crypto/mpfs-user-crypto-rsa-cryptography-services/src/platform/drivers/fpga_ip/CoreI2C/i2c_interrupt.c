/**
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file i2c_interrupt.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreI2C i2c interrupt source file
 *
 */

#include "core_i2c.h"

/*------------------------------------------------------------------------------
 * This function must be modified to enable interrupts generated from the
 * CoreI2C instance identified as parameter.
 */
void I2C_enable_irq( i2c_instance_t * this_i2c )
{
    HAL_ASSERT(0)
}

/*------------------------------------------------------------------------------
 * This function must be modified to disable interrupts generated from the
 * CoreI2C instance identified as parameter.
 */
void I2C_disable_irq( i2c_instance_t * this_i2c )
{
    HAL_ASSERT(0)
}
