/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_can.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreCAN source file
 *
 * See file "core_can.h" for description of the functions implemented in this
 * file.
 *
 */

#include "corecan_regs.h"
#include "core_can.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * CAN_init()
 * See "core_can.h" file for details of how to use this function.
 */
uint32_t
CAN_init
(
    can_instance_t * this_can,
    addr_t base_addr,
    uint32_t bitrate,
    uint32_t can_config,
    uint8_t basic_can_rx_mb,
    uint8_t basic_can_tx_mb
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(base_addr != 0u);

    HAL_ASSERT((basic_can_rx_mb <= CAN_MB_MAX) && (basic_can_rx_mb >= CAN_MB_MIN));
    HAL_ASSERT((basic_can_tx_mb <= CAN_MB_MAX) && (basic_can_tx_mb >= CAN_MB_MIN));

    if (this_can == NULL_INSTANCE)
        return (CAN_ERR);

    /* Set base address of CoreCAN hardware. */
    this_can->base_address = base_addr;

    if (((basic_can_rx_mb <= CAN_MB_MAX) && (basic_can_rx_mb >= CAN_MB_MIN)) &&
        ((basic_can_tx_mb <= CAN_MB_MAX) && (basic_can_tx_mb >= CAN_MB_MIN)))
    {
        /* Initialize the device structure */
        this_can->basic_can_rx_mb = basic_can_rx_mb;
        this_can->basic_can_tx_mb = basic_can_tx_mb;
    }
    else
    {
        this_can->basic_can_rx_mb = CAN_MB_MIN;
        this_can->basic_can_tx_mb = CAN_MB_MIN;
    }

    /* Clear all interrupts */
    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_SRC,
                                            CORECAN_INTR_SRC_REG_RW_MASK);

    /* Disable Interrupts */
    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_EN,
                                                      (uint32_t)DISABLE);

    uint32_t temp_add =
           this_can->base_address + CORECAN_RXMSG_CTRL_REG_OFFSET(CAN_MB_0);

    /* Initialize the rx mailbox */
    for (uint8_t mb_num = 0u; mb_num < CAN_MB_MAX; mb_num++)
    {
        HW_set_32bit_reg(temp_add, CORECAN_RXMSG_CTRL_WPN_MASK);
        for (uint8_t n = 1u; n < 8; n++)
        {
            temp_add += 4;
            HW_set_32bit_reg(temp_add, 0u);
        }
        temp_add += 4;
    }

    /* Configure CAN controller */
    if (bitrate == CAN_SPEED_MANUAL)
    {
        /* ToDo:- Add support for user-configurable CAN bit timing
         * parameters (CFG_bitrate, CFG_tseg1, CFG_tseg2, CFG_SJW).
         * Currently, only predefined fixed values are supported. */

        /*
         * If user wants to specify registers directly  Check if parameters
         * meet minimums.
         */
        uint32_t cfg_reg_value = HAL_get_32bit_reg(this_can->base_address,
                                                            CORECAN_CFG);

        uint8_t cfg_tseg1_value =
            (cfg_reg_value & CORECAN_CFG_CFG_TSEG1_MASK) >>
                                              CORECAN_CFG_CFG_TSEG1_SHIFT;

        if (cfg_tseg1_value < CAN_TSEG1_MIN)
        {
            return (CAN_TSEG1_TOO_SMALL);
        }
        uint8_t cfg_tseg2_value =
            (cfg_reg_value & CORECAN_CFG_CFG_TSEG2_MASK) >>
                                              CORECAN_CFG_CFG_TSEG2_SHIFT;

        if ((cfg_tseg2_value == 0u) || (cfg_tseg2_value == 1u))
        {
            return (CAN_TSEG2_TOO_SMALL);
        }

        uint8_t cfg_sjw_value =
            (cfg_reg_value & CORECAN_CFG_CFG_SJW_MASK) >>
                                                CORECAN_CFG_CFG_SJW_SHIFT;

        if ((cfg_sjw_value > cfg_tseg1_value) ||
            (cfg_sjw_value > cfg_tseg2_value))
        {
            return (CAN_SJW_TOO_BIG);
        }

        HAL_set_32bit_reg(this_can->base_address, CORECAN_CFG, can_config);
    }
    else
    {
        HAL_set_32bit_reg(this_can->base_address, CORECAN_CFG, bitrate);
    }
    return (CAN_OK);
}

/*******************************************************************************
 * CAN_set_config_reg()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_set_config_reg
(
    can_instance_t * this_can,
    uint32_t cfg
)
{
    uint32_t temp_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Clear all pending interrupts */
    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_SRC,
                                                CORECAN_INTR_SRC_REG_RW_MASK);

    /* Disable CAN Device */
    temp_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_CMD);

    temp_reg_value &= (uint32_t)(~CORECAN_CMD_RUN_STOP_MODE_MASK);
    HAL_set_32bit_reg(this_can->base_address, CORECAN_CMD, temp_reg_value);

    /* Disable global interrupt. */
    temp_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_INTR_EN);

    temp_reg_value &= (uint32_t)(~CORECAN_INTR_EN_INT_EBL_MASK);
    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_EN, temp_reg_value);

    /* Sets configuration bits */
    HAL_set_32bit_reg(this_can->base_address, CORECAN_CFG, cfg);

    CAN_start(this_can);
}

/*******************************************************************************
 * CAN_set_mode()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_set_mode
(
    can_instance_t * this_can,
    can_mode_t mode
)
{
    uint32_t cmd_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    cmd_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_CMD);

    cmd_reg_value &= (uint32_t)(~CORECAN_CMD_RUN_STOP_MODE_MASK);
    HAL_set_32bit_reg(this_can->base_address, CORECAN_CMD, cmd_reg_value);

    HAL_set_32bit_reg(this_can->base_address, CORECAN_CMD,
                                       mode << CORECAN_CMD_LISTEN_MODE_SHIFT);
}

/*******************************************************************************
 * CAN_start()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_start
(
    can_instance_t * this_can
)
{
    uint32_t temp_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Clear all pending interrupts */
    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_SRC,
                                            CORECAN_INTR_SRC_REG_RW_MASK);

    /* Enable CAN Device or CAN controller into run mode*/
    temp_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_CMD);

    temp_reg_value |= (uint32_t)CORECAN_CMD_RUN_STOP_MODE_MASK;
    HAL_set_32bit_reg(this_can->base_address, CORECAN_CMD, temp_reg_value);

    temp_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_INTR_EN);

    /* Enable interrupts from CAN device and enable receive interrupt. */
    temp_reg_value |= (uint32_t)(CORECAN_INTR_EN_RX_MSG_IE_MASK |
                                                 CORECAN_INTR_EN_INT_EBL_MASK);
    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_EN, temp_reg_value);
}

/*******************************************************************************
 * CAN_stop()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_stop
(
    can_instance_t * this_can
)
{
    uint32_t cmd_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Disable CAN Device */
    cmd_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_CMD);

    cmd_reg_value &= (uint32_t)(~CORECAN_CMD_RUN_STOP_MODE_MASK);
    HAL_set_32bit_reg(this_can->base_address, CORECAN_CMD, cmd_reg_value);
}

/*******************************************************************************
 * CAN_enable_irq()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_enable_irq
(
    can_instance_t * this_can,
    uint32_t irq_flag
)
{
    uint32_t intr_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    intr_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_INTR_EN);

    HAL_set_32bit_reg( this_can->base_address, CORECAN_INTR_EN, \
                                                 intr_reg_value | irq_flag);
}

/*******************************************************************************
 * CAN_disable_irq()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_disable_irq
(
    can_instance_t * this_can,
    uint32_t irq_flag
)
{
    uint32_t intr_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    intr_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_INTR_EN);
    intr_reg_value &= ~irq_flag;

    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_EN, intr_reg_value);
}

/*******************************************************************************
 * CAN_get_global_int_en()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_global_int_en
(
    can_instance_t * this_can
)
{
    uint32_t temp_reg_val;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    temp_reg_val = HAL_get_32bit_reg(this_can->base_address, CORECAN_INTR_EN);

    return ((temp_reg_val & CORECAN_INTR_EN_INT_EBL_NS_MASK));
}

/*******************************************************************************
 * CAN_get_int_en()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_int_en
(
    can_instance_t * this_can
)
{
    return (HAL_get_32bit_reg(this_can->base_address, CORECAN_INTR_EN));
}

/*******************************************************************************
 * CAN_get_int_src()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_int_src
(
    can_instance_t * this_can
)
{
    return (HAL_get_32bit_reg(this_can->base_address, CORECAN_INTR_SRC));
}

/*******************************************************************************
 * CAN_clear_int_src()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_clear_int_src
(
    can_instance_t * this_can,
    uint32_t irq_flag
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);

    HAL_set_32bit_reg(this_can->base_address, CORECAN_INTR_SRC, irq_flag);
}

/*******************************************************************************
 * CAN_set_rtr_message_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_set_rtr_message_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
)
{
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    /* Is buffer configured for Basic CAN? */
    if (mb_num >= (CAN_MB_MAX - this_can->basic_can_rx_mb))
    {
        return (CAN_BASIC_CAN_MAILBOX);
    }

    cntrl_reg_value =
        HW_get_32bit_reg(this_can->base_address +
                                        CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num));

    /* Is buffer configured for RTR auto-replay? */
    if (0u == ((cntrl_reg_value >> 4) & CORECAN_RXMSG_CTRL_RTR_REPLY_NS_MASK))
    {
        return (CAN_NO_RTR_MAILBOX);
    }

    if (pmsg->IDE)
    {
        pmsg->ID <<= CORECAN_RXMSG_ID_RX_ID_SHIFT;
    }
    else
    {
        pmsg->ID <<= CORECAN_MSG_STD_ID_SHIFT;
    }

    HW_set_32bit_reg(this_can->base_address +
                                CORECAN_RXMSG_ID_REG_OFFSET(mb_num), pmsg->ID);
    HW_set_32bit_reg(this_can->base_address +
                     CORECAN_RXMSG_WORD0_DATA_REG_OFFSET(mb_num), pmsg->word0);
    HW_set_32bit_reg(this_can->base_address +
                     CORECAN_RXMSG_WORD1_DATA_REG_OFFSET(mb_num), pmsg->word1);

    return (CAN_OK);
}

/*******************************************************************************
 * CAN_get_rtr_msg_abort_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_rtr_msg_abort_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
)
{
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    /* Is buffer configured for Basic CAN? */
    if (mb_num >= (CAN_MB_MAX - this_can->basic_can_rx_mb))
    {
        /* Mailbox is configured for basic CAN */
        return (CAN_BASIC_CAN_MAILBOX);
    }

    /* Set abort request */
    HW_set_32bit_reg(this_can->base_address +
       CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num), CORECAN_RXMSG_CTRL_RTRABORT_MASK);

    cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                        CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num));

    /* Check the abort is granted */
    if (0u == ((cntrl_reg_value >> CORECAN_RXMSG_CTRL_RTREPLY_PENDING_SHIFT) &
                            CORECAN_RXMSG_CTRL_RTREPLY_PENDING_NS_MASK))
    {
        /* If the RX buffer isn't busy. Abort was successful */
        return (CAN_OK);
    }
    else
    {
        /* Message not aborted.*/
        return (CAN_ERR);
    }
}

/*******************************************************************************
 * CAN_config_buffer()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_config_buffer
(
    can_instance_t * this_can,
    can_rxmsgobject_t *pmsg
)
{
    uint8_t success = CAN_NO_MSG;
    uint8_t mailbox_number;
    uint32_t cntrl_reg_value;
    addr_t base_addr = this_can->base_address;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Find next BASIC CAN buffer that has a message available */
    for (mailbox_number = CAN_MB_MAX - this_can->basic_can_rx_mb;  \
                              mailbox_number < CAN_MB_MAX; mailbox_number++)
    {
        /* Set filters */
        HW_set_32bit_reg(base_addr +
                  CORECAN_RXMSG_AMR_REG_OFFSET(mailbox_number), pmsg->AMR);
        HW_set_32bit_reg(base_addr +
                    CORECAN_RXMSG_ACR_REG_OFFSET(mailbox_number), pmsg->ACR);
        HW_set_32bit_reg(base_addr +
                CORECAN_RXMSG_AMR_DATA_REG_OFFSET(mailbox_number), pmsg->AMR_D);
        HW_set_32bit_reg(base_addr +
                CORECAN_RXMSG_ACR_DATA_REG_OFFSET(mailbox_number), pmsg->ACR_D);

        cntrl_reg_value = HW_get_32bit_reg(base_addr +
                                CORECAN_RXMSG_CTRL_REG_OFFSET(mailbox_number));

        /* Configure mailbox */
        if (mailbox_number < (CAN_MB_MAX - 1))
        {
            cntrl_reg_value |= (CORECAN_RXMSG_CTRL_WPN_MASK |
                                CORECAN_RXMSG_CTRL_LINK_FLAG_MASK |
                                CORECAN_RXMSG_CTRL_RX_INTEBL_MASK |
                                CORECAN_RXMSG_CTRL_BUFFER_ENABLE_MASK);
        }
        else
        {
            cntrl_reg_value |= (CORECAN_RXMSG_CTRL_WPN_MASK |
                                CORECAN_RXMSG_CTRL_RX_INTEBL_MASK |
                                CORECAN_RXMSG_CTRL_BUFFER_ENABLE_MASK);
        }
        HW_set_32bit_reg(base_addr +
                CORECAN_RXMSG_CTRL_REG_OFFSET(mailbox_number), cntrl_reg_value);
        success = CAN_OK;
    }
    return (success);
}

/*******************************************************************************
 * CAN_config_buffer_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_config_buffer_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
)
{
    uint32_t cntrl_reg_value;
    addr_t base_addr = this_can->base_address;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    /* Is buffer configured for Basic CAN? */
    if (mb_num >= (CAN_MB_MAX - this_can->basic_can_rx_mb))
    {
        return (CAN_BASIC_CAN_MAILBOX);
    }

    if ((mb_num >= CAN_MB_0) && (mb_num <= CAN_MB_31))
    {
        HW_set_32bit_reg(base_addr +
                       CORECAN_RXMSG_AMR_REG_OFFSET(mb_num), pmsg->AMR);
        HW_set_32bit_reg(base_addr +
                       CORECAN_RXMSG_ACR_REG_OFFSET(mb_num), pmsg->ACR);
        HW_set_32bit_reg(base_addr +
                       CORECAN_RXMSG_AMR_DATA_REG_OFFSET(mb_num), pmsg->AMR_D);
        HW_set_32bit_reg(base_addr +
                       CORECAN_RXMSG_ACR_DATA_REG_OFFSET(mb_num), pmsg->ACR_D);

        cntrl_reg_value = HW_get_32bit_reg(base_addr +
                                        CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num));

        cntrl_reg_value |= (CORECAN_RXMSG_CTRL_WPN_MASK |
                            CORECAN_RXMSG_CTRL_RX_INTEBL_MASK |
                            CORECAN_RXMSG_CTRL_BUFFER_ENABLE_MASK);

        HW_set_32bit_reg(base_addr +
                       CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num), cntrl_reg_value);
    }

    return (CAN_OK);
}

/*******************************************************************************
 * CAN_get_msg()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_msg
(
    can_instance_t * this_can,
    can_rxmsgobject_t *pmsg
)
{
    uint8_t success = CAN_NO_MSG;
    uint8_t mailbox_number;
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Find next BASIC CAN buffer that has a message available */
    for (mailbox_number = CAN_MB_MAX - this_can->basic_can_rx_mb;  \
                             mailbox_number < CAN_MB_MAX; mailbox_number++)
    {
        cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                CORECAN_RXMSG_CTRL_REG_OFFSET(mailbox_number));

        /* Get DLC, IDE and RTR */
        pmsg->DLC = (cntrl_reg_value >> CORECAN_RXMSG_CTRL_DLC_SHIFT) &
                                                CORECAN_RXMSG_CTRL_DLC_NS_MASK;
        pmsg->IDE = (cntrl_reg_value >> CORECAN_RXMSG_CTRL_IDE_SHIFT) &
                                                CORECAN_RXMSG_CTRL_IDE_NS_MASK;
        pmsg->RTR = (cntrl_reg_value >> CORECAN_RXMSG_CTRL_RTR_SHIFT) &
                                                CORECAN_RXMSG_CTRL_RTR_NS_MASK;

        /* Check that if there is a valid message */
        if (cntrl_reg_value & CORECAN_RXMSG_CTRL_MSGAV_NS_MASK)
        {
            pmsg->ID = HW_get_32bit_reg(this_can->base_address +
                                  CORECAN_RXMSG_ID_REG_OFFSET(mailbox_number));
            if (pmsg->IDE)
            {
                /* Copy ID */
                pmsg->ID >>= CORECAN_RXMSG_ID_RX_ID_SHIFT;
            }
            else
            {
                /* Copy ID */
                pmsg->ID >>= CORECAN_MSG_STD_ID_SHIFT;
            }
            /* Copy 4 of the data bytes */
            pmsg->word0 = HW_get_32bit_reg(this_can->base_address +
                          CORECAN_RXMSG_WORD0_DATA_REG_OFFSET(mailbox_number));

            /* Copy the other 4 data bytes. */
            pmsg->word1 = HW_get_32bit_reg(this_can->base_address +
                          CORECAN_RXMSG_WORD1_DATA_REG_OFFSET(mailbox_number));

            /* Ack that it's been removed from the FIFO */
            HW_set_32bit_reg(this_can->base_address +
                    CORECAN_RXMSG_CTRL_REG_OFFSET(mailbox_number),
                                            CORECAN_RXMSG_CTRL_MSGAV_NS_MASK);
            success = CAN_VALID_MSG;
            break;
        }
    }
    return (success);
}

/*******************************************************************************
 * CAN_get_msg_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_msg_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
)
{
    uint32_t cmd_reg_value;
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    /* Is buffer configured for Basic CAN? */
    if (mb_num >= (CAN_MB_MAX - this_can->basic_can_rx_mb))
    {
        return (CAN_BASIC_CAN_MAILBOX);
    }

    cmd_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_CMD);

    cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                        CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num));

    /* Get DLC, IDE and RTR */
    pmsg->DLC = ((cntrl_reg_value >> CORECAN_RXMSG_CTRL_DLC_SHIFT) &
                                              CORECAN_RXMSG_CTRL_DLC_NS_MASK);
    pmsg->IDE = ((cntrl_reg_value >> CORECAN_RXMSG_CTRL_IDE_SHIFT) &
                                              CORECAN_RXMSG_CTRL_IDE_NS_MASK);
    pmsg->RTR = ((cntrl_reg_value >> CORECAN_RXMSG_CTRL_RTR_SHIFT) &
                                            CORECAN_RXMSG_CTRL_RTR_NS_MASK);

    /* Check that a new message is available and get it */
    if (((ENABLE == (cmd_reg_value & CORECAN_CMD_RUN_STOP_MODE_NS_MASK))) &&
        (ENABLE == (cntrl_reg_value & CORECAN_RXMSG_CTRL_MSGAV_NS_MASK)))
    {
        pmsg->ID = HW_get_32bit_reg(this_can->base_address +
                                CORECAN_RXMSG_ID_REG_OFFSET(mb_num));
        if (pmsg->IDE)
        {
            /* Copy ID for extended format */
            pmsg->ID >>= CORECAN_RXMSG_ID_RX_ID_SHIFT;
        }
        else
        {
            /* Copy ID standard format */
            pmsg->ID >>= CORECAN_MSG_STD_ID_SHIFT;
        }

        /* Copy 4 data bytes */
        pmsg->word0 = HW_get_32bit_reg(this_can->base_address +
                                 CORECAN_RXMSG_WORD0_DATA_REG_OFFSET(mb_num));

        /* Copy the remaining 4 data bytes. */
        pmsg->word1 = HW_get_32bit_reg(this_can->base_address +
                                 CORECAN_RXMSG_WORD1_DATA_REG_OFFSET(mb_num));

        /* Ack that it's been removed from the FIFO */
        HW_set_32bit_reg(this_can->base_address +
                                 CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num),
                                            CORECAN_RXMSG_CTRL_MSGAV_NS_MASK);

        /* And let application know there is a message. */
        return (CAN_VALID_MSG);
    }
    else
    {
        return (CAN_NO_MSG);
    }
}

/*******************************************************************************
 * CAN_rx_msg_avail()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_rx_msg_avail
(
    can_instance_t * this_can
)
{
    uint8_t success = CAN_NO_MSG;
    uint8_t mailbox_number;
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Find next BASIC CAN buffer that has a message available */
    for (mailbox_number = CAN_MB_MAX - this_can->basic_can_rx_mb;  \
                             mailbox_number < CAN_MB_MAX; mailbox_number++)
    {
        cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                CORECAN_RXMSG_CTRL_REG_OFFSET(mailbox_number));

        /* Check that if there is a valid message */
        if (cntrl_reg_value & CORECAN_RXMSG_CTRL_MSGAV_NS_MASK)
        {
            success = CAN_VALID_MSG;
            break;
        }
    }
    return (success);
}

/*******************************************************************************
 * CAN_rx_msg_avail_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_rx_msg_avail_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
)
{
    uint8_t success = CAN_NO_MSG;
    uint8_t mailbox_number;
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    /* Is buffer configured for Basic CAN? */
    if (mb_num >= (CAN_MB_MAX - this_can->basic_can_rx_mb))
    {
        return (CAN_BASIC_CAN_MAILBOX);
    }

    cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                        CORECAN_RXMSG_CTRL_REG_OFFSET(mb_num));

    /* Check that if there is a valid message */
    if (cntrl_reg_value & CORECAN_RXMSG_CTRL_MSGAV_NS_MASK)
    {
        success = CAN_VALID_MSG;
    }
    return (success);
}

/*******************************************************************************
 * CAN_get_amcr_mask_f()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_get_amcr_mask_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);
    addr_t base_addr = this_can->base_address;

    pmsg->AMR = HW_get_32bit_reg(base_addr +
                                        CORECAN_RXMSG_AMR_REG_OFFSET(mb_num));
    pmsg->ACR = HW_get_32bit_reg(base_addr +
                                        CORECAN_RXMSG_ACR_REG_OFFSET(mb_num));
    pmsg->AMR_D = (HW_get_32bit_reg(base_addr +
                                CORECAN_RXMSG_AMR_DATA_REG_OFFSET(mb_num))) &
                                           CORECAN_RXMSG_AMR_DATA_REG_RW_MASK;
    pmsg->ACR_D = (HW_get_32bit_reg(base_addr +
                                CORECAN_RXMSG_ACR_DATA_REG_OFFSET(mb_num))) &
                                           CORECAN_RXMSG_ACR_DATA_REG_RW_MASK;
}

/*******************************************************************************
 * CAN_set_amcr_mask_f()
 * See "core_can.h" for details of how to use this function.
 */
void
CAN_set_amcr_mask_f
(
    can_instance_t* this_can,
    can_mb_t mb_num,
    can_rxmsgobject_t *pmsg
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);
    addr_t base_addr = this_can->base_address;

    HW_set_32bit_reg(base_addr +
                              CORECAN_RXMSG_AMR_REG_OFFSET(mb_num), pmsg->AMR);
    HW_set_32bit_reg(base_addr +
                              CORECAN_RXMSG_ACR_REG_OFFSET(mb_num), pmsg->ACR);
    HW_set_32bit_reg(base_addr +
                       CORECAN_RXMSG_AMR_DATA_REG_OFFSET(mb_num), pmsg->AMR_D);
    HW_set_32bit_reg(base_addr +
                       CORECAN_RXMSG_ACR_DATA_REG_OFFSET(mb_num), pmsg->ACR_D);
}

/*******************************************************************************
 * CAN_send_msg()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_send_msg
(
    can_instance_t * this_can,
    can_txmsgobject_t *pmsg
)
{
    uint8_t success = CAN_NO_MSG;
    uint8_t mailbox_number;
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Find next BASIC CAN buffer that is available */
    for (mailbox_number = CAN_MB_MAX - this_can->basic_can_tx_mb;  \
                             mailbox_number < CAN_MB_MAX; mailbox_number++)
    {
        /* Check which transmit mailbox is not busy and use it. */
        if ((CAN_get_tx_buff_sts(this_can) & (1u << mailbox_number)) == 0u)
        {
            if (pmsg->IDE)
            {
                pmsg->ID <<= CORECAN_TXMSG_ID_TX_ID_SHIFT;
            }
            else
            {
                pmsg->ID <<= CORECAN_MSG_STD_ID_SHIFT;
            }

            /* If the Tx buffer isn't busy.... */
            HW_set_32bit_reg(this_can->base_address +
                    CORECAN_TXMSG_ID_REG_OFFSET(mailbox_number), pmsg->ID);
            HW_set_32bit_reg(this_can->base_address +
              CORECAN_TXMSG_WORD1_DATA_REG_OFFSET(mailbox_number), pmsg->word1);
            HW_set_32bit_reg(this_can->base_address +
              CORECAN_TXMSG_WORD0_DATA_REG_OFFSET(mailbox_number), pmsg->word0);

            cntrl_reg_value |= (CORECAN_TXMSG_CTRL_WPN_MASK |
                               (pmsg->RTR << CORECAN_TXMSG_CTRL_RTR_SHIFT) |
                               (pmsg->IDE << CORECAN_TXMSG_CTRL_IDE_SHIFT) |
                               (pmsg->DLC << CORECAN_TXMSG_CTRL_DLC_SHIFT) |
                                CORECAN_TXMSG_CTRL_TXINTR_WPN_MASK |
                                CORECAN_TXMSG_CTRL_TX_INTEBL_MASK |
                                CORECAN_TXMSG_CTRL_TX_REQ_MASK);

            HW_set_32bit_reg(this_can->base_address +
                CORECAN_TXMSG_CTRL_REG_OFFSET(mailbox_number), cntrl_reg_value);

            success = CAN_VALID_MSG;
            break;
        }
    }
    return (success);
}

/*******************************************************************************
 * CAN_send_msg_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_send_msg_f
(
    can_instance_t * this_can,
    can_mb_t mb_num,
    can_txmsgobject_t *pmsg
)
{
    uint32_t cmd_reg_value;
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    cmd_reg_value = HAL_get_32bit_reg(this_can->base_address, CORECAN_CMD);

    /* Can't send if device is stop mode */
    if (DISABLE == (cmd_reg_value & CORECAN_CMD_RUN_STOP_MODE_NS_MASK))
    {
        /* Message not sent. */
        return (CAN_NO_MSG);
    }

    /* Is buffer configured for Basic CAN? */
    if (mb_num >= (CAN_MB_MAX - this_can->basic_can_tx_mb))
    {
        /* mailbox is configured for basic CAN */
        return (CAN_BASIC_CAN_MAILBOX);
    }

    cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                        CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num));

    if (0u == (cntrl_reg_value & CORECAN_TXMSG_CTRL_TX_REQ_NS_MASK))
    {
        if (pmsg->IDE)
        {
            pmsg->ID <<= CORECAN_TXMSG_ID_TX_ID_SHIFT;
        }
        else
        {
            pmsg->ID <<= CORECAN_MSG_STD_ID_SHIFT;
        }

        /* If the Tx buffer isn't busy.... */
        HW_set_32bit_reg(this_can->base_address +
                                CORECAN_TXMSG_ID_REG_OFFSET(mb_num), pmsg->ID);
        HW_set_32bit_reg(this_can->base_address +
                     CORECAN_TXMSG_WORD1_DATA_REG_OFFSET(mb_num), pmsg->word1);
        HW_set_32bit_reg(this_can->base_address +
                     CORECAN_TXMSG_WORD0_DATA_REG_OFFSET(mb_num), pmsg->word0);

        cntrl_reg_value |= (CORECAN_TXMSG_CTRL_WPN_MASK |
                           (pmsg->RTR << CORECAN_TXMSG_CTRL_RTR_SHIFT) |
                           (pmsg->IDE << CORECAN_TXMSG_CTRL_IDE_SHIFT) |
                           (pmsg->DLC << CORECAN_TXMSG_CTRL_DLC_SHIFT) |
                            CORECAN_TXMSG_CTRL_TXINTR_WPN_MASK |
                            CORECAN_TXMSG_CTRL_TX_INTEBL_MASK  |
                            CORECAN_TXMSG_CTRL_TX_REQ_MASK);

        HW_set_32bit_reg(this_can->base_address +
                       CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num), cntrl_reg_value);

        return (CAN_VALID_MSG);
    }
    else
    {
        /* Message not sent. */
        return (CAN_NO_MSG);
    }

#ifdef DEBUG
    cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                        CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num));

    // loop here only until the msg is tx
    while ((cntrl_reg_value & (0x1)) == 0x1u)
    {
        cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                    CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num));
    }
#endif
}

/*******************************************************************************
 * CAN_abort_tx_msg_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_abort_tx_msg_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
)
{
    uint32_t cmd_reg_value;
    uint32_t cntrl_reg_value;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    /* Is buffer configured for Basic CAN? */
    if (mb_num >= (CAN_MB_MAX - this_can->basic_can_tx_mb))
    {
        /* mailbox is configured for basic CAN */
        return (CAN_BASIC_CAN_MAILBOX);
    }

    cntrl_reg_value = HW_get_32bit_reg(this_can->base_address +
                                       CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num));

    cntrl_reg_value &= ~CORECAN_TXMSG_CTRL_TX_REQ_MASK;

    HW_set_32bit_reg(this_can->base_address +
                           CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num),
                           cntrl_reg_value | CORECAN_TXMSG_CTRL_TX_ABORT_MASK);

    /* Check the abort is granted */
    if (0u == ((HW_get_32bit_reg(this_can->base_address +
                CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num))) &
                CORECAN_TXMSG_CTRL_TX_ABORT_MASK))
    {
        /* If the Tx buffer isn't busy, Abort was successful */
        return (CAN_OK);
    }
    else
    {
        /* Message not aborted. */
        return (CAN_ERR);
    }
}

/*******************************************************************************
 * CAN_tx_msg_avail()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_tx_msg_avail
(
    can_instance_t * this_can
)
{
    uint8_t success = CAN_ERR;
    uint8_t mailbox_number;

    HAL_ASSERT(this_can != NULL_INSTANCE);

    /* Find next BASIC CAN buffer that is available */
    for (mailbox_number = CAN_MB_MAX - this_can->basic_can_tx_mb; \
                             mailbox_number < CAN_MB_MAX; mailbox_number++)
    {
        if (0u == ((HW_get_32bit_reg(this_can->base_address +
                            CORECAN_TXMSG_CTRL_REG_OFFSET(mailbox_number))) &
                            CORECAN_TXMSG_CTRL_TX_REQ_MASK))
        {
            /* Tx buffer isn't busy */
            success = CAN_OK;
            break;
        }
    }

    return (success);
}

/*******************************************************************************
 * CAN_tx_msg_avail_f()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_tx_msg_avail_f
(
    can_instance_t * this_can,
    can_mb_t mb_num
)
{
    uint8_t success = CAN_ERR;

    HAL_ASSERT(this_can != NULL_INSTANCE);
    HAL_ASSERT(mb_num <= CAN_MB_31);

    if (0u == ((HW_get_32bit_reg(this_can->base_address +
                    CORECAN_TXMSG_CTRL_REG_OFFSET(mb_num))) &
                    CORECAN_TXMSG_CTRL_TX_REQ_MASK))
    {
        /* Tx buffer isn't busy */
        success = CAN_OK;
    }
    return (success);
}

/*******************************************************************************
 * CAN_get_rx_buff_sts()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_rx_buff_sts
(
    can_instance_t * this_can
)
{
    return (HAL_get_32bit_reg(this_can->base_address, CORECAN_RXBUFF_STS));
}

/*******************************************************************************
 * CAN_get_tx_buff_sts()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_tx_buff_sts
(
    can_instance_t * this_can
)
{
    return (HAL_get_32bit_reg(this_can->base_address, CORECAN_TXBUFF_STS));
}

/*******************************************************************************
 * CAN_get_tx_err_cnt()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_tx_err_cnt
(
    can_instance_t * this_can
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);

    uint32_t err_sts_reg_value = HAL_get_32bit_reg( this_can->base_address, \
                                            CORECAN_ERR_STS);

    return (err_sts_reg_value & CORECAN_ERR_STS_TX_ERR_CNT_MASK);
}

/*******************************************************************************
 * CAN_get_rx_error_count()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_rx_err_cnt
(
    can_instance_t * this_can
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);

    uint32_t err_sts_reg_value = HAL_get_32bit_reg( this_can->base_address, \
                                            CORECAN_ERR_STS);

    err_sts_reg_value >>= CORECAN_ERR_STS_RX_ERR_CNT_SHIFT;

    return (err_sts_reg_value & CORECAN_ERR_STS_RX_ERR_CNT_NS_MASK);
}

/*******************************************************************************
 * CAN_get_err_sts()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_err_sts
(
    can_instance_t * this_can
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);

    uint32_t err_sts_reg_value = HAL_get_32bit_reg( this_can->base_address, \
                                            CORECAN_ERR_STS);

    err_sts_reg_value >>= CORECAN_ERR_STS_ERR_STATE_SHIFT;

    return (err_sts_reg_value & CORECAN_ERR_STS_ERR_STATE_NS_MASK);
}

/*******************************************************************************
 * CAN_get_tx_gte96()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_tx_gte96
(
    can_instance_t * this_can
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);

    uint32_t err_sts_reg_value = HAL_get_32bit_reg( this_can->base_address, \
                                            CORECAN_ERR_STS);

    err_sts_reg_value >>= CORECAN_ERR_STS_TXGTE96_SHIFT;

    return ((err_sts_reg_value & CORECAN_ERR_STS_TXGTE96_NS_MASK));
}

/*******************************************************************************
 * CAN_get_rx_gte96()
 * See "core_can.h" for details of how to use this function.
 */
uint32_t
CAN_get_rx_gte96
(
    can_instance_t * this_can
)
{
    HAL_ASSERT(this_can != NULL_INSTANCE);

    uint32_t err_sts_reg_value = HAL_get_32bit_reg( this_can->base_address, \
                                            CORECAN_ERR_STS);

    err_sts_reg_value >>= CORECAN_ERR_STS_RXGTE96_SHIFT;

    return ((err_sts_reg_value & CORECAN_ERR_STS_RXGTE96_NS_MASK));
}

#ifdef __cplusplus
}
#endif
