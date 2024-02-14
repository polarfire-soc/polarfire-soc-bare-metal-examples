/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file uart_mapping.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief MPFS HAL Embedded Software
 *
 */

/******************************************************************************
 * This file allows to map the UART assigned to each hart. A re-mapping might be
 * needed in case not all of them are routed on the pcb or in a different order
 * as for the Discovery Kit. Be aware that a mapping different from the default
 * prevents you from using UART local interrupts.
 *****************************************************************************/

#ifndef _UART_MAPPING_H
#define _UART_MAPPING_H
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "mpfs_hal/common/mss_peripherals.h"

#if defined(MPFS_DISCOVERY_KIT)

#define MSS_PERIPH_MMUART_E51   MSS_PERIPH_MMUART1
#define MSS_PERIPH_MMUART_U54_1 MSS_PERIPH_MMUART4
#define MSS_PERIPH_MMUART_U54_2 MSS_PERIPH_MMUART0
#define MSS_PERIPH_MMUART_U54_3 MSS_PERIPH_MMUART3
#define MSS_PERIPH_MMUART_U54_4 MSS_PERIPH_MMUART2

#define PLIC_IRQ_MMUART_E51   MMUART1_PLIC
#define PLIC_IRQ_MMUART_U54_1 MMUART4_PLIC
#define PLIC_IRQ_MMUART_U54_2 MMUART0_PLIC_77
#define PLIC_IRQ_MMUART_U54_3 MMUART3_PLIC
#define PLIC_IRQ_MMUART_U54_4 MMUART2_PLIC

#elif defined(ICICLE_KIT_ES)

#define MSS_PERIPH_MMUART_E51   MSS_PERIPH_MMUART0
#define MSS_PERIPH_MMUART_U54_1 MSS_PERIPH_MMUART1
#define MSS_PERIPH_MMUART_U54_2 MSS_PERIPH_MMUART2
#define MSS_PERIPH_MMUART_U54_3 MSS_PERIPH_MMUART3
#define MSS_PERIPH_MMUART_U54_4 MSS_PERIPH_MMUART4

#define PLIC_IRQ_MMUART_E51   MMUART0_PLIC_77
#define PLIC_IRQ_MMUART_U54_1 MMUART1_PLIC
#define PLIC_IRQ_MMUART_U54_2 MMUART2_PLIC
#define PLIC_IRQ_MMUART_U54_3 MMUART3_PLIC
#define PLIC_IRQ_MMUART_U54_4 MMUART4_PLIC

#else


#define MSS_PERIPH_MMUART_E51   MSS_PERIPH_MMUART0
#define MSS_PERIPH_MMUART_U54_1 MSS_PERIPH_MMUART1
#define MSS_PERIPH_MMUART_U54_2 MSS_PERIPH_MMUART2
#define MSS_PERIPH_MMUART_U54_3 MSS_PERIPH_MMUART3
#define MSS_PERIPH_MMUART_U54_4 MSS_PERIPH_MMUART4

#define PLIC_IRQ_MMUART_E51   MMUART0_PLIC_77
#define PLIC_IRQ_MMUART_U54_1 MMUART1_PLIC
#define PLIC_IRQ_MMUART_U54_2 MMUART2_PLIC
#define PLIC_IRQ_MMUART_U54_3 MMUART3_PLIC
#define PLIC_IRQ_MMUART_U54_4 MMUART4_PLIC

#endif

#endif //_UART_MAPPING_H
