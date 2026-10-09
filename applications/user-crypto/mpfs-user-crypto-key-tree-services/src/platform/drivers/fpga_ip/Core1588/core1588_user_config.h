/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * @file core1588_user_config.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core1588 user configuration header file
 *
 */

/*=========================================================================*/

#ifndef __CORE1588_USER_CONFIG_H
#define __CORE1588_USER_CONFIG_H 1

/***************************************************************************/ /**
   Select buffer size for the individual buffers. It is recommended to select
   buffer sizes that are a power of two.
 */
#define C1588_RX_RING_SIZE 4
#define C1588_TX_RING_SIZE 4
#define C1588_LATCH_RING_SIZE 4
#define C1588_TRIGGER_RING_SIZE 4

/***************************************************************************/ /**
   Enable or disable custom user handlers for interrupt types. If enabled,
   handler must be implemented or the application will halt.
 */
#define C1588_RX_IRQ_USER_HANDLER 0
#define C1588_TX_IRQ_USER_HANDLER 0
#define C1588_LATCH_IRQ_USER_HANDLER 0
#define C1588_TRIGGER_IRQ_USER_HANDLER 0
#define C1588_RTCSEC_IRQ_USER_HANDLER 0

#endif // __CORE1588_USER_CONFIG_H
