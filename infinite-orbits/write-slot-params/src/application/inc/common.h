/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 */
/*******************************************************************************
 * Used for common defines across application code
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include "drivers/mss/mss_mmuart/mss_uart.h"




#define   ENTER               0x0DU
#define NUM_BLOCKS 1364  // replace with actual size of BIOS boot partition
#define BUFFER_SIZE (BLOCK_SIZE * 4)  // buffer size is 4 blocks (2KB)

typedef enum COMMAND_TYPE_
{
    CLEAR_COMMANDS                  = 0x00,       /*!< 0 default behavior */
    START_HART1_U_MODE              = 0x01,       /*!< 1 u mode */
    START_HART2_S_MODE              = 0x02,       /*!< 2 s mode */
}   COMMAND_TYPE;

typedef struct HART_SHARED_DATA_
{
    uint64_t init_marker;
    volatile long mutex_uart1;
    mss_uart_instance_t *g_mss_uart1_lo;
    mss_uart_instance_t *g_mss_uart2_lo;


} HART_SHARED_DATA;

#endif /* COMMON_H_ */
