/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core1588_interrupt.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core1588 interrupt source file
 *
 */

#include "core1588.h"

/*------------------------------------------------------------------------------
 * User implemented handler called after Rx interrupts. Handler must be
 * modified if enabled in core1588_user_config.h to avoid the application
 * halting upon calling.
 */
void core1588_ptp_rx_user_handler(c1588_instance_t *this_c1588,
                                  uint32_t rx_irq_mask) {
  HAL_ASSERT(0)
}

/*------------------------------------------------------------------------------
 * User implemented handler called after Tx interrupts. Handler must be
 * modified if enabled in core1588_user_config.h to avoid the application
 * halting upon calling.
 */
void core1588_ptp_tx_user_handler(c1588_instance_t *this_c1588,
                                  uint32_t tx_irq_mask) {
  HAL_ASSERT(0)
}

/*------------------------------------------------------------------------------
 * User implemented handler called after latch interrupts. Handler must be
 * modified if enabled in core1588_user_config.h to avoid the application
 * halting upon calling.
 */
void core1588_latch_user_handler(c1588_instance_t *this_c1588,
                                 uint32_t latch_mask) {
  HAL_ASSERT(0)
}

/*------------------------------------------------------------------------------
 * User implemented handler called after trigger interrupts. Handler must be
 * modified if enabled in core1588_user_config.h to avoid the application
 * halting upon calling.
 */
void core1588_trigger_user_handler(c1588_instance_t *this_c1588,
                                   uint32_t trigger_mask) {
  HAL_ASSERT(0)
}

/*------------------------------------------------------------------------------
 * User implemented handler called after RTC seconds interrupts. Handler must
 * be modified if enabled in core1588_user_config.h to avoid the application
 * halting upon calling.
 */
void core1588_rtcsec_user_handler(c1588_instance_t *this_c1588) {
  HAL_ASSERT(0)
}
