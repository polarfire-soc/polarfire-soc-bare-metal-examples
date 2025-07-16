/*******************************************************************************
 * (c) Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 * 
 * SPDX-License-Identifier: MIT
 *
 * @file core_axi4protoconv.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreAXI4ProtoConv IP bare metal driver implementation.
 * See file "core_axi4protoconv.h" for description of the functions implemented
 * in this file.
 *
 */

#include "coreaxi4protoconv_regs.h"
#include "core_axi4protoconv.h"
#include "coreaxi4protoconv_user_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Null parameters with appropriate type definitions
 */

#define NULL_INSTANCE           (( PCDMA_instance_t* ) 0)
#define NULL_VAL                ( 0)
#define PCDMA_ADDR1_SHIFT       ( (uint8_t) (32) )
#define CLEAR_ALL_S2MM_IRQ      (uint32_t)0x1DU
#define CLEAR_ALL_MM2S_IRQ      (uint32_t)0x5U
#define DISABLE                 0u
#define ENABLE                  1u


/*******************************************************************************
 * PCDMA_init()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void
PCDMA_init
(
    PCDMA_instance_t  * this_pcdma,
    addr_t base_addr
)
{
    HAL_ASSERT( this_pcdma != NULL_INSTANCE );
    HAL_ASSERT( base_addr != 0u );

    if( this_pcdma != NULL_INSTANCE )
    {
        /* Set base address of CoreAXI4ProtoConv hardware. */
        this_pcdma->base_address = base_addr;

        /* Disable S2MM and MM2S Interrupts */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                                COREAXI4PROTOCONV_REGS_S2MM_INTRENB, DISABLE);

        HAL_set_32bit_reg( this_pcdma->base_address, \
                                COREAXI4PROTOCONV_REGS_MM2S_INTRENB, DISABLE);

        /* Clear S2MM and MM2S interrupts */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                      COREAXI4PROTOCONV_REGS_S2MM_INTRSRC, CLEAR_ALL_S2MM_IRQ);

        HAL_set_32bit_reg( this_pcdma->base_address, \
                      COREAXI4PROTOCONV_REGS_MM2S_INTRSRC, CLEAR_ALL_MM2S_IRQ);
    }
}

#ifndef S2MM_ENABLE
/*******************************************************************************
 * PCDMA_S2MM_configure()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_S2MM_configure
(
    PCDMA_instance_t  * this_pcdma,
    uint32_t xfr_size,
    uint64_t src_add,
    uint16_t cmd_id,
    uint8_t burst_type
)
{

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );
    HAL_ASSERT( xfr_size != 0u );
    HAL_ASSERT( cmd_id <= S2MM_CMD_STS_FIFO_DEPTH_MAX );
    HAL_ASSERT( burst_type <= PCDMA_BURST_TYPE_INCR );

    if((xfr_size != 0u) && (cmd_id <= S2MM_CMD_STS_FIFO_DEPTH_MAX) && \
                                      (burst_type <= PCDMA_BURST_TYPE_INCR) )
    {
        /* Configure transfer size for the total number of bytes to be
         * transmitted.
         */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                                   COREAXI4PROTOCONV_REGS_S2MM_LEN, xfr_size );

        /* Configure the lower 32-bits address of the AXI4 memory mapped
         * interface.
         */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                             COREAXI4PROTOCONV_REGS_S2MM_ADDR0, \
          (uint32_t) (COREAXI4PROTOCONV_REGS_S2MM_ADDR0_REG_RW_MASK & src_add));

        /* Configure the upper 32-bits address of the AXI4 memory mapped
         * interface.
         */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                            COREAXI4PROTOCONV_REGS_S2MM_ADDR1, \
                (uint32_t) (COREAXI4PROTOCONV_REGS_S2MM_ADDR1_REG_RW_MASK & \
                                           (src_add >> PCDMA_ADDR1_SHIFT)));

        /* Configure the burst type */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                                   COREAXI4PROTOCONV_REGS_S2MM_CTRL,  \
             ((cmd_id << COREAXI4PROTOCONV_REGS_S2MM_CTRL_CMDID_SHIFT) |     \
               (burst_type << COREAXI4PROTOCONV_REGS_S2MM_CTRL_BURTYP_SHIFT)));
    }

}

/*******************************************************************************
 * PCDMA_S2MM_start_transfer()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_S2MM_start_transfer
(
    PCDMA_instance_t  * this_pcdma
)
{
    uint32_t control_reg_value;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    if( this_pcdma != NULL_INSTANCE )
    {
        control_reg_value = HAL_get_32bit_reg( this_pcdma->base_address,\
                                    COREAXI4PROTOCONV_REGS_S2MM_CTRL);

        HAL_set_32bit_reg( this_pcdma->base_address, \
                COREAXI4PROTOCONV_REGS_S2MM_CTRL, (control_reg_value | \
                            COREAXI4PROTOCONV_REGS_S2MM_CTRL_START_NS_MASK) );
    }

}

/*******************************************************************************
 * PCDMA_S2MM_get_status()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
uint32_t PCDMA_S2MM_get_status
(
    PCDMA_instance_t  * this_pcdma
)
{
    uint32_t status;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    if( this_pcdma != NULL_INSTANCE )
    {
        status = HAL_get_32bit_reg( this_pcdma->base_address, \
                                         COREAXI4PROTOCONV_REGS_S2MM_STS);
    }
    return (status);
}

/*******************************************************************************
 * PCDMA_S2MM_enable_irq()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_S2MM_enable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
)
{
    uint32_t reg_value;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    reg_value = HAL_get_32bit_reg( this_pcdma->base_address, \
                                        COREAXI4PROTOCONV_REGS_S2MM_INTRENB);
    reg_value |= irq_type;

    HAL_set_32bit_reg( this_pcdma->base_address, \
                            COREAXI4PROTOCONV_REGS_S2MM_INTRENB, reg_value);
}

/*******************************************************************************
 * PCDMA_S2MM_disable_irq()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_S2MM_disable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
)
{
    uint32_t reg_value;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    reg_value = HAL_get_32bit_reg( this_pcdma->base_address, \
                                        COREAXI4PROTOCONV_REGS_S2MM_INTRENB);
    reg_value &= ~irq_type;

    HAL_set_32bit_reg( this_pcdma->base_address, \
                            COREAXI4PROTOCONV_REGS_S2MM_INTRENB, reg_value);
}

/*******************************************************************************
 * PCDMA_S2MM_get_int_src()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
uint32_t PCDMA_S2MM_get_int_src
(
   PCDMA_instance_t  * this_pcdma
)
{
    return (HAL_get_32bit_reg( this_pcdma->base_address, \
                                    COREAXI4PROTOCONV_REGS_S2MM_INTRSRC));
}

/*******************************************************************************
 * PCDMA_S2MM_clr_int_src()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_S2MM_clr_int_src
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
)
{

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    HAL_set_32bit_reg( this_pcdma->base_address, \
                            COREAXI4PROTOCONV_REGS_S2MM_INTRSRC, irq_type);
}

#ifdef IP_Config_S2MM_UNDEFINED_BURST_LENGTH_ENABLE

/*******************************************************************************
 * PCDMA_S2MM_get_len()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
uint32_t PCDMA_S2MM_get_len
(
   PCDMA_instance_t  * this_pcdma
)
{
    return (HAL_get_32bit_reg( this_pcdma->base_address, \
                                    COREAXI4PROTOCONV_REGS_S2MM_LEN));
}

#endif
#endif

#ifndef MM2S_ENABLE
/*******************************************************************************
 * PCDMA_MM2S_configure()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_MM2S_configure
(
    PCDMA_instance_t  * this_pcdma,
    uint32_t xfr_size,
    uint64_t src_add,
    uint16_t cmd_id,
    uint8_t burst_type
)
{

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );
    HAL_ASSERT( xfr_size != 0u );
    HAL_ASSERT( cmd_id <= MM2S_CMD_STS_FIFO_DEPTH_MAX );
    HAL_ASSERT( burst_type <= PCDMA_BURST_TYPE_INCR );

    if((xfr_size != 0) && (cmd_id <= MM2S_CMD_STS_FIFO_DEPTH_MAX) && \
                                     (burst_type <= PCDMA_BURST_TYPE_INCR) )
    {
        /* Configure transfer size for the total number of bytes to be
         * transmitted.
         */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                                COREAXI4PROTOCONV_REGS_MM2S_LEN, xfr_size );

        /* Configure the lower 32-bits address of the AXI4 memory mapped
         * interface.
         */
        HAL_set_32bit_reg( this_pcdma->base_address, \
                                COREAXI4PROTOCONV_REGS_MM2S_ADDR0, \
          (uint32_t) (COREAXI4PROTOCONV_REGS_MM2S_ADDR0_REG_RW_MASK & src_add));

        /* Configure the upper 32-bits address of the AXI4 memory mapped
         * interface.
         */
         HAL_set_32bit_reg( this_pcdma->base_address, \
                                 COREAXI4PROTOCONV_REGS_MM2S_ADDR1, \
                  (uint32_t) (COREAXI4PROTOCONV_REGS_MM2S_ADDR1_REG_RW_MASK & \
                                              (src_add >> PCDMA_ADDR1_SHIFT)));

            /* Configure the burst type */
         HAL_set_32bit_reg( this_pcdma->base_address, \
                                 COREAXI4PROTOCONV_REGS_MM2S_CTRL, \
            ((cmd_id << COREAXI4PROTOCONV_REGS_MM2S_CTRL_CMDID_SHIFT) | \
               (burst_type << COREAXI4PROTOCONV_REGS_MM2S_CTRL_BURTYP_SHIFT)));
    }
}

/*******************************************************************************
 * PCDMA_MM2S_start_transfer()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_MM2S_start_transfer
(
    PCDMA_instance_t  * this_pcdma
)
{
    uint32_t control_reg_value;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    if( this_pcdma != NULL_INSTANCE )
    {
        control_reg_value = HAL_get_32bit_reg( this_pcdma->base_address,\
                                             COREAXI4PROTOCONV_REGS_MM2S_CTRL);

        HAL_set_32bit_reg( this_pcdma->base_address, \
                                       COREAXI4PROTOCONV_REGS_MM2S_CTRL, \
        ( control_reg_value | COREAXI4PROTOCONV_REGS_MM2S_CTRL_START_NS_MASK) );
    }
}

/*******************************************************************************
 * PCDMA_MM2S_get_status()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
uint32_t PCDMA_MM2S_get_status
(
    PCDMA_instance_t  * this_pcdma
)
{
    uint32_t status;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    if( this_pcdma != NULL_INSTANCE )
    {
        status = HAL_get_32bit_reg( this_pcdma->base_address, \
                                         COREAXI4PROTOCONV_REGS_MM2S_STS);
    }
    return status;
}

/*******************************************************************************
 * PCDMA_MM2S_enable_irq()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_MM2S_enable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
)
{
    uint32_t reg_value;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    reg_value = HAL_get_32bit_reg( this_pcdma->base_address, \
                                        COREAXI4PROTOCONV_REGS_MM2S_INTRENB);
    reg_value |= irq_type;

    HAL_set_32bit_reg( this_pcdma->base_address, \
                            COREAXI4PROTOCONV_REGS_MM2S_INTRENB, reg_value);
}

/*******************************************************************************
 * PCDMA_MM2S_disable_irq()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_MM2S_disable_irq
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
)
{
    uint32_t reg_value;

    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    reg_value = HAL_get_32bit_reg( this_pcdma->base_address, \
                                        COREAXI4PROTOCONV_REGS_MM2S_INTRENB);
    reg_value &= ~irq_type;

    HAL_set_32bit_reg( this_pcdma->base_address, \
                            COREAXI4PROTOCONV_REGS_MM2S_INTRENB, reg_value);
}

/*******************************************************************************
 * PCDMA_MM2S_get_int_src()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
uint32_t PCDMA_MM2S_get_int_src
(
   PCDMA_instance_t  * this_pcdma
)
{
    return (HAL_get_32bit_reg( this_pcdma->base_address, \
                                    COREAXI4PROTOCONV_REGS_MM2S_INTRSRC));
}

/*******************************************************************************
 * PCDMA_MM2S_clr_int_src()
 * See "core_axi4protoconv.h" file for details of how to use this function.
 */
void PCDMA_MM2S_clr_int_src
(
   PCDMA_instance_t  * this_pcdma,
   uint32_t irq_type
)
{
    HAL_ASSERT( this_pcdma != NULL_INSTANCE );

    HAL_set_32bit_reg( this_pcdma->base_address, \
                            COREAXI4PROTOCONV_REGS_MM2S_INTRSRC, irq_type);
}

#endif

#ifdef __cplusplus
}
#endif
