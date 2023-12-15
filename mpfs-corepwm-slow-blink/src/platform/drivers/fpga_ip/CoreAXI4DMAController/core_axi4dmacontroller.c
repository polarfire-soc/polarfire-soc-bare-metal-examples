/*******************************************************************************
 * Copyright 2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * CoreAXI4DMAController bare metal software driver implementation.
 *
 */
#include "coreaxi4dmacontroller_regs.h"
#include "coreaxi4dmacontroller_user_config.h"
#include "core_axi4dmacontroller.h"

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif 
/*-------------------------------------------------------------------------*//**
 *
 */
#define NULL_POINTER                            (void *)0
#define PARAM_ERROR                             -1

/***************************************************************************//**
 * See core_axi4dmacontroller.h for description of this function.
 */
int32_t
AXI4DMA_init
(
    axi4dma_instance_t* this_dmac,
    uint32_t base_addr
)
{
    uint8_t idx = 0;
    int32_t ret = PARAM_ERROR;

    HAL_ASSERT(NULL_POINTER != this_dmac);
    HAL_ASSERT(0u != base_addr);

    if((0u != base_addr) && (NULL_POINTER != this_dmac))
    {
        this_dmac->base_addr = base_addr;

        for(idx = 0; idx < AXI4DMA_NUM_OF_INTERRUPTS; idx++)
        {
            this_dmac->intr_mask[idx] = 0x0u;
        }

        ret = 0;
    }

    return(ret);
}

/***************************************************************************//**
 * See core_axi4dmacontroller.h for description of this function.
 */
int8_t
AXI4DMA_configure_stream
(
    axi4dma_instance_t* this_dmac,
    axi4dma_stream_desc_t* stream_desc_ptr,
    axi4dma_tdest_signal_t tdest_signal_num,
    uint8_t config,
    uint32_t xfr_count,
    uint32_t dest_addr
)
{
    int32_t ret = PARAM_ERROR;

    HAL_ASSERT(NULL_POINTER != this_dmac);
    HAL_ASSERT(NULL_POINTER != stream_desc_ptr);
    HAL_ASSERT(0 != dest_addr);
    HAL_ASSERT(0 != xfr_count);
    HAL_ASSERT(tdest_signal_num < TDEST_3);

    if ((0 != stream_desc_ptr) && (NULL_POINTER != stream_desc_ptr) &&
        (0 != dest_addr) && (0 != xfr_count) && (tdest_signal_num < TDEST_3))
    {
        stream_desc_ptr->config = config;
        stream_desc_ptr->xfr_count = xfr_count;
        stream_desc_ptr->dest_addr = dest_addr;

       HAL_set_32bit_reg((this_dmac->base_addr + (tdest_signal_num * 0x4)),
                          STR0ADDR,
                          (0xFFFFFFFFu & (uintptr_t)stream_desc_ptr));

        ret = 0;
    }

    return(ret);
}

/***************************************************************************//**
 * See core_axi4dmacontroller.h for description of this function.
 */
uint32_t
AXI4DMA_transfer_status
(
    axi4dma_instance_t* this_dmac,
    axi4dma_irq_num_t irq_num,
    axi4dma_desc_id_t* desc_id, //return param
    uint32_t* ext_desc_addr //return param
)
{
    uint32_t status;
    int32_t ret = PARAM_ERROR;

    HAL_ASSERT(NULL_POINTER != this_dmac);
    HAL_ASSERT(NULL_POINTER != desc_id);
    HAL_ASSERT(NULL_POINTER != ext_desc_addr);

    if((NULL_POINTER != this_dmac) && (NULL_POINTER != desc_id) &&
            (NULL_POINTER != ext_desc_addr))
    {
        status = HAL_get_32bit_reg(((this_dmac->base_addr) +
                                    (irq_num *0x10u)),
                                    I0ST);

        *desc_id = status & I0ST_DESCNO_MASK;
        *desc_id >>= I0ST_DESCNO_SHIFT;

        if ((EXT_DESC_32 == *desc_id) || (STREAM_DESC_33 == *desc_id))
        {
            *ext_desc_addr = HAL_get_32bit_reg(((this_dmac->base_addr) +
                                                (irq_num *0x10u)),
                                                I0EXADRR);
        }
        else
        {
            *ext_desc_addr = 0x00000000u;
        }
    }

    return status;
}

/***************************************************************************//**
 * See core_axi4dmacontroller.h for description of this function.
 */
int8_t
AXI4DMA_configure
(
    axi4dma_instance_t* this_dmac,
    axi4dma_desc_id_t desc_id,
    uint8_t src_op,
    uint8_t dest_op,
    uint32_t byte_count,
    uint32_t src_addr,
    uint32_t dest_addr
)
{
    int32_t ret = PARAM_ERROR;

    HAL_ASSERT(NULL_POINTER != this_dmac);
    HAL_ASSERT(desc_id <= INTRN_DESC_31);
    HAL_ASSERT(0u != byte_count);
    HAL_ASSERT(0u != src_addr);
    HAL_ASSERT(0u != dest_addr);

    if ((NULL_POINTER != this_dmac) && (desc_id <= INTRN_DESC_31) &&
            (0u != byte_count) && (0u != src_addr) && (0u != dest_addr))
    {
        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0CFG_REG_OFFSET + (desc_id * 0x20))),
                                ID0CFG_SRC_OP,
                                (src_op & ID0CFG_SRC_OP_MASK));    //Source OP

        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0CFG_REG_OFFSET + (desc_id * 0x20))),
                                ID0CFG_DEST_OP,
                                (dest_op & ID0CFG_SRC_OP_MASK) << ID0CFG_DEST_OP_SHIFT);    //Destination OP

        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0CFG_REG_OFFSET + (desc_id * 0x20))),
                                ID0CFG_CHAIN,
                                0); //chain = 0

        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0CFG_REG_OFFSET + (desc_id * 0x20))),
                                ID0CFG_EXDESC,
                                0); //Ext Desc = 0

        //single descriptor without chain bit always generates interrupt

        //Next Descriptor ignored

        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0BYTECNT_REG_OFFSET + (desc_id * 0x20))),
                                ID0BYTECNT_NUM,
                                byte_count);                        //Byte count

        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0SRCADDR_REG_OFFSET + (desc_id * 0x20))),
                                ID0SRCADDR_ADDR,
                                src_addr);                         //Source Address

        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0DESTADDR_REG_OFFSET + (desc_id * 0x20))),
                                ID0DESTADDR_ADDR,
                                dest_addr);                        //Destination address

        /* Desc_valid bit must be written last as any changes to desc registers clears this bit
         * automatically if it was set.
         * Data Valid and Desc_valid bit are being set together as there could be a chance that
         * if start bit is already set or external Start input line is already high and Data Valid
         * is set before setting desc_valid bit could result rarely into interrupt for starting on
         * invalid descriptor.
         * */
        HAL_set_32bit_reg_field((this_dmac->base_addr + (ID0CFG_REG_OFFSET + (desc_id * 0x20))),
                                ID0CFG_DESCVALID,
                                0x06);    //Data ready and descriptor valid set together

        ret = 0;
    }

    return(ret);
}

/***************************************************************************//**
 * See core_axi4dmacontroller.h for description of this function.
 */
int8_t
AXI4DMA_configure_chain
(
    axi4dma_instance_t* this_dmac,
    axi4dma_desc_t* desc_data,
    uint8_t num_desc
)
{
    uint8_t idx = 0;
    axi4dma_desc_id_t desc_id = 0;
    axi4dma_desc_t* this_cfg= 0;
    int32_t ret = PARAM_ERROR;

    HAL_ASSERT(NULL_POINTER != this_dmac);
    HAL_ASSERT(NULL_POINTER != desc_data);

    if((NULL_POINTER != this_dmac) && (NULL_POINTER != desc_data))
    {
        for(idx = 0; idx < num_desc; idx++)
        {
            this_cfg = (desc_data + idx);
            desc_id = this_cfg->desc_id;

            if(desc_id > INTRN_DESC_31)
            {
                (*((uint32_t*)desc_id + 1)) = desc_data->xfr_count;
                (*((uint32_t*)desc_id + 2)) = desc_data->src_addr;
                (*((uint32_t*)desc_id + 3)) = desc_data->dest_addr;
                (*((uint32_t*)desc_id + 4)) = desc_data->next_desc;
                (*(uint32_t*)desc_id) = desc_data->desc_cfg;
            }
            else
            {
                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0CFG_SRC_OP,
                                        this_cfg->desc_cfg | ID0CFG_SRC_OP_MASK);   //Source OP

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0CFG_DEST_OP,
                                        this_cfg->desc_cfg | ID0CFG_DEST_OP_MASK);  //Destination OP

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0CFG_CHAIN,
                                        this_cfg->desc_cfg | ID0CFG_CHAIN_MASK);    //chain

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0CFG_EXDESC,
                                        (this_cfg->desc_cfg | ID0CFG_EXDESC_MASK)); //Ext Desc

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                        (desc_id * 0x20)),
                                        ID0CFG_INTR,
                                        (this_cfg->desc_cfg | ID0CFG_INTR_MASK));   //interrupt on process

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0NEXTDESC_ADDR,
                                        this_cfg->next_desc);                       //Next Descriptor

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0BYTECNT_NUM,
                                        this_cfg->xfr_count);                       //Byte count

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0SRCADDR_ADDR,
                                        this_cfg->src_addr);                        //Source Address

                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0DESTADDR_ADDR,
                                        this_cfg->dest_addr);                       //Destination address

                /* Desc_valid bit must be written last as any changes to desc registers clears this bit
                 * automatically if it was set.
                 * Data Valid and Desc_valid bit are being set together as there could be a chance that
                 * if start bit is already set or external Start input line is already high and Data Valid
                 * is set before setting desc_valid bit could result rarely into interrupt for starting on
                 * invalid descriptor.
                 * */
                HAL_set_32bit_reg_field((this_dmac->base_addr +
                                        (desc_id * 0x20)),
                                        ID0CFG_DESCVALID,
                                        0x06);    //Data ready and descriptor valid set together
            }
        }

        ret = 0;
    }

    return(ret);
}

/***************************************************************************//**
 * See core_axi4dmacontroller.h for description of this function.
 */
void
AXI4DMA_start_transfer
(
    axi4dma_instance_t* this_dmac,
    axi4dma_desc_id_t desc_id
)
{
    HAL_set_32bit_reg(this_dmac->base_addr, START, (0x00000001u << desc_id));
}

#ifdef __cplusplus
}
#endif
