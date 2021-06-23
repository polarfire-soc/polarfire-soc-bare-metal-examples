/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include "drivers/mss/mss_mmuart/mss_uart.h"

typedef enum COMMAND_TYPE_
{
    CLEAR_COMMANDS                  = 0x00,       /*!< 0 default behavior */
    START_HART1_U_MODE              = 0x01,       /*!< 1 u mode */
    START_HART2_S_MODE              = 0x02,       /*!< 2 s mode */
}   COMMAND_TYPE;


typedef enum MODE_CHOICE_
{
    M_MODE              = 0x00,       /*!< 0 m mode */
    S_MODE              = 0x01,       /*!< s mode */
}   MODE_CHOICE;


typedef struct HART_SHARED_DATA_
{
    uint64_t init_marker;
    volatile long mutex_uart0;
    mss_uart_instance_t *g_mss_uart0_lo;
} HART_SHARED_DATA;

/**
 * extern variables
 */

/**
 * functions
 */
void jump_to_application(HLS_DATA* hls, MODE_CHOICE mode_choice, uint64_t next_addr);
void
uart_tx_with_mutex
(
    mss_uart_instance_t * this_uart,
    uint64_t mutex_addr,
    const uint8_t * pbuff,
    uint32_t tx_size
);
void
uart_tx_string_with_mutex
(
    mss_uart_instance_t * this_uart,
    uint64_t mutex_addr,
    const uint8_t * pbuff
);

#endif /* COMMON_H_ */
