/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

typedef enum COMMAND_TYPE_
{
	CLEAR_COMMANDS        				= 0x00,       /*!< 0 default behavior */
	START_HART1_U_MODE        			= 0x01,       /*!< 1 u mode */
	START_HART2_S_MODE        			= 0x02,       /*!< 2 s mode */
} COMMAND_TYPE;


/**
 * extern variables
 */

/**
 * functions
 */
void mac_task( void *pvParameters );
void u54_2_uart0_rx_handler (mss_uart_instance_t * this_uart);
void u54_1_uart0_rx_handler (mss_uart_instance_t * this_uart);

#endif /* COMMON_H_ */
