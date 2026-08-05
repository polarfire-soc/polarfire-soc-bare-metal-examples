/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include "portContext.h"

.extern freertos_risc_v_exception_handler
.extern freertos_risc_v_interrupt_handler
.extern freertos_risc_v_mtimer_interrupt_handler

.balign 256, 0
.option norvc
.global freertos_vector_tablexx

freertos_vector_tablexx:
IRQ_0:
        j freertos_risc_v_exception_handler
IRQ_1:
        j freertos_risc_v_interrupt_handler
IRQ_2:
        j freertos_risc_v_interrupt_handler
IRQ_3:
        j freertos_risc_v_interrupt_handler
IRQ_4:
        j freertos_risc_v_interrupt_handler
IRQ_5:
        j freertos_risc_v_interrupt_handler
IRQ_6:
        j freertos_risc_v_interrupt_handler
IRQ_7:
        j freertos_risc_v_mtimer_interrupt_handler
IRQ_8:
        j freertos_risc_v_interrupt_handler
IRQ_9:
        j freertos_risc_v_interrupt_handler
IRQ_10:
        j freertos_risc_v_interrupt_handler
IRQ_11:
        j freertos_risc_v_interrupt_handler
IRQ_12:
        j freertos_risc_v_interrupt_handler
IRQ_13:
        j freertos_risc_v_interrupt_handler
IRQ_14:
        j freertos_risc_v_interrupt_handler
IRQ_15:
        j freertos_risc_v_interrupt_handler
IRQ_LC0:
        j freertos_risc_v_interrupt_handler
IRQ_LC1:
        j freertos_risc_v_interrupt_handler
IRQ_LC2:
        j freertos_risc_v_interrupt_handler
IRQ_LC3:
        j freertos_risc_v_interrupt_handler
IRQ_LC4:
        j freertos_risc_v_interrupt_handler
IRQ_LC5:
        j freertos_risc_v_interrupt_handler
IRQ_LC6:
        j freertos_risc_v_interrupt_handler
IRQ_LC7:
        j freertos_risc_v_interrupt_handler
IRQ_LC8:
        j freertos_risc_v_interrupt_handler
IRQ_LC9:
        j freertos_risc_v_interrupt_handler
IRQ_LC10:
        j freertos_risc_v_interrupt_handler
IRQ_LC11:
        j freertos_risc_v_interrupt_handler
IRQ_LC12:
        j freertos_risc_v_interrupt_handler
IRQ_LC13:
        j freertos_risc_v_interrupt_handler
IRQ_LC14:
        j freertos_risc_v_interrupt_handler
IRQ_LC15:
        j freertos_risc_v_interrupt_handler

.global freertos_vector_table_h1
.balign 256, 0
freertos_vector_table_h1:
H1_IRQ_0:
        j freertos_risc_v_exception_handler
H1_IRQ_1:
        j freertos_risc_v_interrupt_handler
H1_IRQ_2:
        j freertos_risc_v_interrupt_handler
H1_IRQ_3:
        j freertos_risc_v_interrupt_handler
H1_IRQ_4:
        j freertos_risc_v_interrupt_handler
H1_IRQ_5:
        j freertos_risc_v_interrupt_handler
H1_IRQ_6:
        j freertos_risc_v_interrupt_handler
H1_IRQ_7:
        j freertos_risc_v_mtimer_interrupt_handler
H1_IRQ_8:
        j freertos_risc_v_interrupt_handler
H1_IRQ_9:
        j freertos_risc_v_interrupt_handler
H1_IRQ_10:
        j freertos_risc_v_interrupt_handler
H1_IRQ_11:
        j freertos_risc_v_interrupt_handler
H1_IRQ_12:
        j freertos_risc_v_interrupt_handler
H1_IRQ_13:
        j freertos_risc_v_interrupt_handler
H1_IRQ_14:
        j freertos_risc_v_interrupt_handler
H1_IRQ_15:
        j freertos_risc_v_interrupt_handler
H1_IRQ_LC0:
        j U54_spare_0_local_IRQHandler
H1_IRQ_LC1:
        j U54_spare_1_local_IRQHandler
H1_IRQ_LC2:
        j U54_spare_2_local_IRQHandler
H1_IRQ_LC3:
        j U54_1_mac0_mmsl_local_IRQHandler
H1_IRQ_LC4:
        j U54_1_mac0_emac_local_IRQHandler
H1_IRQ_LC5:
        j U54_1_mac0_queue3_local_IRQHandler
H1_IRQ_LC6:
        j U54_1_mac0_queue2_local_IRQHandler
H1_IRQ_LC7:
        j U54_1_mac0_queue1_local_IRQHandler
H1_IRQ_LC8:
        j U54_1_mac0_int_local_IRQ_wrapper
H1_IRQ_LC9:
        j U54_1_wdog_tout_local_IRQHandler
H1_IRQ_LC10:
        j U54_1_wdog_mvrp_local_IRQHandler
H1_IRQ_LC11:
        j U54_1_mmuart1_local_IRQHandler
H1_IRQ_LC12:
        j U54_spare_3_local_IRQHandler
H1_IRQ_LC13:
        j U54_spare_4_local_IRQHandler
H1_IRQ_LC14:
        j U54_spare_5_local_IRQHandler
H1_IRQ_LC15:
        j U54_spare_6_local_IRQHandler
H1_IRQ_LC16:
        j U54_f2m_0_local_IRQHandler
H1_IRQ_LC17:
        j U54_f2m_1_local_IRQHandler
H1_IRQ_LC18:
        j U54_f2m_2_local_IRQHandler
H1_IRQ_LC19:
        j U54_f2m_3_local_IRQHandler
H1_IRQ_LC20:
        j U54_f2m_4_local_IRQHandler
H1_IRQ_LC21:
        j U54_f2m_5_local_IRQHandler
H1_IRQ_LC22:
        j U54_f2m_6_local_IRQHandler
H1_IRQ_LC23:
        j U54_f2m_7_local_IRQHandler
H1_IRQ_LC24:
        j U54_f2m_8_local_IRQHandler
H1_IRQ_LC25:
        j U54_f2m_9_local_IRQHandler
H1_IRQ_LC26:
        j U54_f2m_10_local_IRQHandler
H1_IRQ_LC27:
        j U54_f2m_11_local_IRQHandler
H1_IRQ_LC28:
        j U54_f2m_12_local_IRQHandler
H1_IRQ_LC29:
        j U54_f2m_13_local_IRQHandler
H1_IRQ_LC30:
        j U54_f2m_14_local_IRQHandler
H1_IRQ_LC31:
        j U54_f2m_15_local_IRQHandler
H1_IRQ_LC32:
        j U54_f2m_16_local_IRQHandler
H1_IRQ_LC33:
        j U54_f2m_17_local_IRQHandler
H1_IRQ_LC34:
        j U54_f2m_18_local_IRQHandler
H1_IRQ_LC35:
        j U54_f2m_19_local_IRQHandler
H1_IRQ_LC36:
        j U54_f2m_20_local_IRQHandler
H1_IRQ_LC37:
        j U54_f2m_21_local_IRQHandler
H1_IRQ_LC38:
        j U54_f2m_22_local_IRQHandler
H1_IRQ_LC39:
        j U54_f2m_23_local_IRQHandler
H1_IRQ_LC40:
        j U54_f2m_24_local_IRQHandler
H1_IRQ_LC41:
        j U54_f2m_25_local_IRQHandler
H1_IRQ_LC42:
        j U54_f2m_26_local_IRQHandler
H1_IRQ_LC43:
        j U54_f2m_27_local_IRQHandler
H1_IRQ_LC44:
        j U54_f2m_28_local_IRQHandler
H1_IRQ_LC45:
        j U54_f2m_29_local_IRQHandler
H1_IRQ_LC46:
        j U54_f2m_30_local_IRQHandler
H1_IRQ_LC47:
        j U54_f2m_31_local_IRQHandler


U54_1_mac0_int_local_IRQ_wrapper:
    portcontextSAVE_INTERRUPT_CONTEXT
    call U54_1_mac0_int_local_IRQHandler
    portcontextRESTORE_CONTEXT
