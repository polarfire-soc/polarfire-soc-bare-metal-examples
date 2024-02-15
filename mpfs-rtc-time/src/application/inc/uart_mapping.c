/*******************************************************************************
 * Copyright 2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file uart_mapping.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief MPFS HAL Embedded Software
 *
 */


#include "inc/uart_mapping.h"

#if defined(MPFS_DISCOVERY_KIT)

mss_uart_instance_t *p_uartmap_e51   = &g_mss_uart1_lo;
mss_uart_instance_t *p_uartmap_u54_1 = &g_mss_uart4_lo;
mss_uart_instance_t *p_uartmap_u54_2 = &g_mss_uart0_lo;
mss_uart_instance_t *p_uartmap_u54_3 = &g_mss_uart3_lo;
mss_uart_instance_t *p_uartmap_u54_4 = &g_mss_uart2_lo;

#elif defined(ICICLE_KIT_ES)

mss_uart_instance_t *p_uartmap_e51   = &g_mss_uart0_lo;
mss_uart_instance_t *p_uartmap_u54_1 = &g_mss_uart1_lo;
mss_uart_instance_t *p_uartmap_u54_2 = &g_mss_uart2_lo;
mss_uart_instance_t *p_uartmap_u54_3 = &g_mss_uart3_lo;
mss_uart_instance_t *p_uartmap_u54_4 = &g_mss_uart4_lo;

#else

#warning "If this warning is displayed no board macro has been \
         defined in the project settings. The default mapping allocates \
         UART 0 to the E51, UART 1 to U54_1, UART 2 to U54_2  \
         Some boards have different UART connections which may require\
         an alternative UART mapping and schematics should be consulted \
         to ensure the correct UARTs are allocated to harts."

mss_uart_instance_t *p_uartmap_e51   = &g_mss_uart0_lo;
mss_uart_instance_t *p_uartmap_u54_1 = &g_mss_uart1_lo;
mss_uart_instance_t *p_uartmap_u54_2 = &g_mss_uart2_lo;
mss_uart_instance_t *p_uartmap_u54_3 = &g_mss_uart3_lo;
mss_uart_instance_t *p_uartmap_u54_4 = &g_mss_uart4_lo;

#endif

