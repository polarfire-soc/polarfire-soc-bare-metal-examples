/*******************************************************************************
 * Copyright 2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * CORE_AXI4DMACONTROLLER_Driver configurations.
 *
 */

#ifndef CORE_AXI4DMACONTROLLER_DRIVER__1_0_1_CONFIGURATION_HEADER
#define CORE_AXI4DMACONTROLLER_DRIVER__1_0_1_CONFIGURATION_HEADER


#define CORE_VENDOR "Microchip"
#define CORE_LIBRARY "Embedded software"
#define CORE_NAME "CORE_AXI4DMACONTROLLER_Driver"
#define CORE_VERSION "2.0.0"

#define AXI4DMA_STREAM_IF                  0           //0 or 1
#define AXI4DMA_DMA_DWIDTH                 64          //32,64,128,256,512
#define AXI4DMA_PRI_LEVELS                 1           //1 to 8

/*out of 8 defines below, valid defines are limited by AXI4DMA_PRI_LEVELS
parameter
e.g. if AXI4DMA_PRI_LEVELS                 1
then AXI4DMA_PRI_0_NUM_OF_BEATS is valid, rest are unused.
*/
#define AXI4DMA_PRI_0_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256
#define AXI4DMA_PRI_1_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256
#define AXI4DMA_PRI_2_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256
#define AXI4DMA_PRI_3_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256
#define AXI4DMA_PRI_4_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256
#define AXI4DMA_PRI_5_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256
#define AXI4DMA_PRI_6_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256
#define AXI4DMA_PRI_7_NUM_OF_BEATS         256         //1,4,8,16,32,64,128, 256

#define AXI4DMA_NUM_OF_INTERRUPTS          1           //1 to 4

/*out of 4 defines below, valid defines are limited by AXI4DMA_NUM_OF_INTERRUPTS
 parameter
e.g. if #define AXI4DMA_NUM_OF_INTERRUPTS          1
then AXI4DMA_INTR_0_QUEUE_DEPTH is valid, rest are unused.
*/
#define AXI4DMA_INTR_0_QUEUE_DEPTH          1           //1 to 8
#define AXI4DMA_INTR_1_QUEUE_DEPTH          1           //1 to 8
#define AXI4DMA_INTR_2_QUEUE_DEPTH          1           //1 to 8
#define AXI4DMA_INTR_3_QUEUE_DEPTH          1           //1 to 8

#define AXI4DMA_NUM_OF_INTERNAL_DESC        4           //4 to 32

/*out of 32 defines below, valid defines are limited by AXI4DMA_NUM_OF_INTERNAL_DESC
parameter
e.g. if #define AXI4DMA_NUM_OF_INTERNAL_DESC          4
then below listed defines are valid, rest are unused.
#define AXI4DMA_DESC0_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC1_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC2_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC3_PRIO_LEVEL            0           //0 to 7
*/
#define AXI4DMA_DESC0_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC1_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC2_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC3_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC4_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC5_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC6_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC7_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC8_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC9_PRIO_LEVEL            0           //0 to 7
#define AXI4DMA_DESC10_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC11_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC12_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC13_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC14_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC15_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC16_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC17_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC18_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC19_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC20_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC21_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC22_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC23_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC24_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC25_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC26_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC27_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC28_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC29_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC30_PRIO_LEVEL           0           //0 to 7
#define AXI4DMA_DESC31_PRIO_LEVEL           0           //0 to 7

/*out of 32 defines below, valid defines are limited by AXI4DMA_NUM_OF_INTERNAL_DESC
parameter
e.g. if #define AXI4DMA_NUM_OF_INTERNAL_DESC          4
then below listed defines are valid, rest are unused.
#define AXI4DMA_DESC0_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC1_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC2_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC3_INTR_ASSOC            0           //0 to 3
*/
#define AXI4DMA_DESC0_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC1_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC2_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC3_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC4_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC5_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC6_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC7_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC8_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC9_INTR_ASSOC            0           //0 to 3
#define AXI4DMA_DESC10_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC11_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC12_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC13_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC14_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC15_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC16_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC17_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC18_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC19_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC20_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC21_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC22_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC23_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC24_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC25_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC26_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC27_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC28_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC29_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC30_INTR_ASSOC           0           //0 to 3
#define AXI4DMA_DESC31_INTR_ASSOC           0           //0 to 3


#endif // CORE_AXI4DMACONTROLLER_DRIVER__1_0_1_CONFIGURATION_HEADER

