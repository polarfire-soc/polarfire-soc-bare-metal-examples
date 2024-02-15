/*******************************************************************************
 * Copyright 2019-2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * PF_System_Services driver implementation. See file "core_syservices_pf.h" for
 * description of the functions implemented in this file.
 *
 */

#include "core_sysservices_pf.h"
#include "coresysservicespf_regs.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL_BUFFER                             (( uint8_t* ) 0)

static uint8_t execute_ss_command
(
    uint8_t cmd_opcode,
    const uint8_t* cmd_data,
    uint16_t cmd_data_size,
    const uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset
);

uint32_t g_css_pf_base_addr = 0u;

/***************************************************************************//**
 * SYS_init()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
void
SYS_init
(
    uint32_t base_addr
)
{
    g_css_pf_base_addr = base_addr;
}

/***************************************************************************//**
 * SYS_get_serial_number()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t
SYS_get_serial_number
(
    const uint8_t * p_serial_number,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if (p_serial_number == NULL_BUFFER)
    {
        return status;
    }

    status = execute_ss_command(SERIAL_NUMBER_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               p_serial_number,
                               SERIAL_NUMBER_RESP_LEN,
                               mb_offset,
                               0u);

    return status;
}

/***************************************************************************//**
 * SYS_get_user_code()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t
SYS_get_user_code
(
    const uint8_t * p_user_code,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if(p_user_code  == NULL_BUFFER)
    {
        return status;
    }

    status = execute_ss_command(USERCODE_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               p_user_code,
                               USERCODE_RESP_LEN,
                               mb_offset,
                               0u);
    return status;
}

/***************************************************************************//**
 * SYS_get_design_info()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t
SYS_get_design_info
(
    const uint8_t * p_design_info,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if(p_design_info  == NULL_BUFFER)
    {
        return status;
    }

    status = execute_ss_command(DESIGN_INFO_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               p_design_info,
                               DESIGN_INFO_RESP_LEN,
                               mb_offset,
                               0u);
    return status;
}

/***************************************************************************//**
 * SYS_get_device_certificate()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t
SYS_get_device_certificate
(
    const uint8_t * p_device_certificate,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if(p_device_certificate  == NULL_BUFFER)
    {
        return status;
    }

    status = execute_ss_command(DEVICE_CERTIFICATE_REQUEST_CMD,
                                NULL_BUFFER,
                                0u,
                                p_device_certificate,
                                DEVICE_CERTIFICATE_RESP_LEN,
                                mb_offset,
                                0u);
    return status;
}

/***************************************************************************//**
 * SYS_read_digest()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_read_digest
(
   const uint8_t * p_digest,
   uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if(p_digest  == NULL_BUFFER)
    {
        return status;
    }

#ifndef CORESYSSERVICES_MPFS
    status = execute_ss_command(READ_DIGEST_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               p_digest,
                               READ_DIGEST_RESP_LEN,
                               mb_offset,
                               0u);
#else
    status = execute_ss_command(READ_DIGEST_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               p_digest,
                               READ_DIGEST_MPFS_RESP_LEN,
                               mb_offset,
                               0u);
#endif
    return status;

}

/***************************************************************************//**
 * SYS_query_security()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_query_security
(
    uint8_t * p_security_locks,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;
    uint8_t idx = 0u;

    if(p_security_locks  == NULL_BUFFER)
    {
        return status;
    }

#ifndef CORESYSSERVICES_MPFS
    uint8_t buf[12] = {0};
    /* Actual QUERY_SECURITY_RESP_LEN is 9 or 33 but PF_System_Services core
     * needs number of words instead of number of bytes to be written to or read
     * from MailBox */
    status = execute_ss_command(QUERY_SECURITY_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               buf,
                               (QUERY_SECURITY_RESP_LEN + 3u),
                               mb_offset,
                               0u);

    for (idx = 0u; idx < 9u; idx++)
    {
        *(p_security_locks+idx) = buf[idx];
    }

#else
    uint8_t buf[36] = {0};

    status = execute_ss_command(QUERY_SECURITY_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               buf,
                               (QUERY_SECURITY_MPFS_RESP_LEN + 3u),
                               mb_offset,
                               0u);

    for (idx = 0u; idx < 33u; idx++)
    {
        *(p_security_locks+idx) = buf[idx];
    }

#endif

    return status;
}

/***************************************************************************//**
 * SYS_read_debug_info()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_read_debug_info
(
    const uint8_t * p_debug_info,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if(p_debug_info  == NULL_BUFFER)
    {
        return status;
    }

    status = execute_ss_command(READ_DEBUG_INFO_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               p_debug_info,
                               READ_DEBUG_INFO_RESP_LEN,
                               mb_offset,
                               0u);
    return status;
}

#ifdef CORESYSSERVICES_MPFS
/***************************************************************************//**
 * SYS_read_envm_parameter()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_read_envm_parameter
(
    uint8_t * p_envm_param,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if(p_envm_param  == NULL_BUFFER)
    {
        return status;
    }

    status = execute_ss_command(READ_ENVM_PARAM_REQUEST_CMD,
                               NULL_BUFFER,
                               0,
                               p_envm_param,
                               READ_ENVM_PARAM_RESP_LEN,
                               mb_offset,
                               0);
    return status;
}

#endif

/***************************************************************************//**
 * SYS_puf_emulation_service()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_puf_emulation_service
(
    const uint8_t * p_challenge,
    uint8_t op_type,
    uint8_t* p_response,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;
    uint8_t mb_format[20] = {0x00};
    uint8_t index = 0u;

    if((p_response  == NULL_BUFFER) || (p_challenge == NULL_BUFFER))
    {
        return status;
    }

    /* Frame the data required for mailbox */
    mb_format[index] = op_type;

    for (index = 4u; index < 20u; index++)
    {
        mb_format[index] = p_challenge[index - 4u];
    }

    status = execute_ss_command(PUF_EMULATION_SERVICE_REQUEST_CMD,
                               mb_format,
                               PUF_EMULATION_SERVICE_CMD_LEN,
                               p_response,
                               PUF_EMULATION_SERVICE_RESP_LEN,
                               mb_offset,
                               5u); /* mentioning offset to number of words instead of bytes */

    return status;
}

/***************************************************************************//**
 * SYS_digital_signature_service()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_digital_signature_service
(
    const uint8_t* p_hash,
    uint8_t format,
    uint8_t* p_response,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if((p_hash  == NULL_BUFFER) || (p_response == NULL_BUFFER))
    {
        return status;
    }

    if (format == DIGITAL_SIGNATURE_RAW_FORMAT_REQUEST_CMD)
    {
        status = execute_ss_command(DIGITAL_SIGNATURE_RAW_FORMAT_REQUEST_CMD,
                                   p_hash,
                                   DIGITAL_SIGNATURE_HASH_LEN,
                                   p_response,
                                   DIGITAL_SIGNATURE_RAW_FORMAT_RESP_SIZE,
                                   mb_offset,
                                   12u); /* mentioning offset to number of words instead of bytes */
    }
    else
    {
        status = execute_ss_command(DIGITAL_SIGNATURE_DER_FORMAT_REQUEST_CMD,
                                   p_hash,
                                   DIGITAL_SIGNATURE_HASH_LEN,
                                   p_response,
                                   DIGITAL_SIGNATURE_DER_FORMAT_RESP_SIZE,
                                   mb_offset,
                                   12u); /* mentioning offset to number of words instead of bytes */
    }

    return status;
}

/***************************************************************************//**
 * SYS_secure_nvm_write()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_secure_nvm_write
(
    uint8_t format,
    uint8_t snvm_module,
    const uint8_t* p_data,
    const uint8_t* p_user_key,
    uint16_t mb_offset
)
{
    uint8_t frame[256] = {0x00};
    uint8_t* p_frame = &frame[0];
    uint16_t index = 0u;
    uint8_t status = SYS_PARAM_ERR;

    HAL_ASSERT(!(NULL_BUFFER == p_data));
    HAL_ASSERT(!(snvm_module >= 221u));
    if (format != SNVM_NON_AUTHEN_TEXT_REQUEST_CMD)
    {
        HAL_ASSERT(!(NULL_BUFFER == p_user_key));
    }

    if ((p_data == NULL_BUFFER) || (snvm_module >= 221))
    {
        return status;
    }

    if ((format != SNVM_NON_AUTHEN_TEXT_REQUEST_CMD)
       && (p_user_key == NULL_BUFFER))
    {
        return status;
    }

    if ((format != SNVM_NON_AUTHEN_TEXT_REQUEST_CMD)
       && (format != SNVM_AUTHEN_TEXT_REQUEST_CMD)
       && (format != SNVM_AUTHEN_CIPHERTEXT_REQUEST_CMD))
    {
        return status;
    }

    *p_frame = snvm_module; /* SNVMADDR - SNVM module */

    p_frame += 4u; /* Next 3 bytes RESERVED - For alignment */

    /* Copy user key and send the command/data to mailbox. */
    if ((format == SNVM_AUTHEN_TEXT_REQUEST_CMD) ||
       (format == SNVM_AUTHEN_CIPHERTEXT_REQUEST_CMD))
    {
        /* Copy user data */
        for (index = 0u; index < (AUTHENTICATED_TEXT_DATA_LEN - USER_SECRET_KEY_LEN - 4u); index++)
        {
            *p_frame = p_data[index];
            p_frame++;
        }

        /* Copy user key */
        for (index = 0u; index < USER_SECRET_KEY_LEN; index++)
        {
            *p_frame = p_user_key[index];
            p_frame++;
        }

        status = execute_ss_command(format,
                                   &frame[0],
                                   AUTHENTICATED_TEXT_DATA_LEN,
                                   NULL_BUFFER,
                                   0u,
                                   mb_offset,
                                   0u);
    }
    else
    {
        /* Copy user data */
        for (index = 0u; index < (NON_AUTHENTICATED_TEXT_DATA_LEN - 4u); index++)
        {
            *(p_frame+index) = p_data[index];
        }

        status = execute_ss_command(format,
                                   &frame[0],
                                   NON_AUTHENTICATED_TEXT_DATA_LEN,
                                   NULL_BUFFER,
                                   0u,
                                   mb_offset,
                                   0u);
    }

    return status;
}

/***************************************************************************//**
 * SYS_secure_nvm_read()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_secure_nvm_read
(
    uint8_t snvm_module,
    const uint8_t* p_user_key,
    uint8_t* p_admin,
    uint8_t* p_data,
    uint16_t data_len,
    uint16_t mb_offset
)
{
    /* Frame the message. */
    uint8_t frame[16] = {0x00u};
    uint8_t* p_frame = &frame[0u];
    uint8_t status = SYS_PARAM_ERR;
    uint8_t response[256] = {0x00u};
    uint16_t index = 0u;

    HAL_ASSERT(!(NULL_BUFFER == p_data));
    HAL_ASSERT(!(NULL_BUFFER == p_admin));
    HAL_ASSERT(!(snvm_module > 221u));

    HAL_ASSERT(data_len == 236u || data_len == 252u);

    if((p_data  == NULL_BUFFER) ||
       (snvm_module >= 221) ||
       (p_admin == NULL_BUFFER))
    {
        return status;
    }

    *p_frame = snvm_module; /* SNVMADDR - SNVM module */

    p_frame += 4u; /* RESERVED - For alignment */

    /* Copy user key */
    if (236u == data_len)
    {
        HAL_ASSERT(p_user_key !=  NULL_BUFFER);

        if(p_user_key ==  NULL_BUFFER)
        {
            return status;
        }

        for (index = 0u; index < 12u; index++)
        {
            *p_frame = p_user_key[index];
            p_frame++;
        }
    }
    else
    {
        p_frame += 12u;
    }

    status = execute_ss_command(SNVM_READ_REQUEST_CMD,
                               &frame[0],
                               16u,
                               response,
                               (data_len + 4u),
                               mb_offset,
                               4u); /* mentioning offset to number of words instead of bytes */

    if (SYS_SUCCESS == status)
    {
        for (index = 0u; index < 4u; index++)
        {
            *(p_admin+index) = (uint32_t)response[index];
        }


       /* Copy data into user buffer. */
        for (index = 4u; index < (data_len + 4u); index++)
        {
            *(p_data + (index - 4u)) = response[index];
        }
    }
    else
    {
        ;
    }

    return status;
}

/***************************************************************************//**
 * SYS_nonce_service()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_nonce_service
(
    const uint8_t * p_nonce,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;

    if((p_nonce  == NULL_BUFFER))
    {
        return status;
    }

    status = execute_ss_command(NONCE_SERVICE_REQUEST_CMD,
                               NULL_BUFFER,
                               0u,
                               p_nonce,
                               NONCE_SERVICE_RESP_LEN,
                               mb_offset,
                               0u);

    return status;
}

/***************************************************************************//**
 * SYS_bitstream_authenticate_service()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_bitstream_authenticate_service
(
    uint32_t spi_flash_address,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;
    uint32_t l_spi_flash_address = spi_flash_address;
    status = execute_ss_command(BITSTREAM_AUTHENTICATE_CMD,
                                (uint8_t* )&l_spi_flash_address,
                                4u,
                                NULL_BUFFER,
                                0u,
                                mb_offset,
                                0u);

    return status;
}

/***************************************************************************//**
 * SYS_IAP_image_authenticate_service()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_IAP_image_authenticate_service
(
    uint8_t spi_idx
)
{
    uint8_t status = SYS_PARAM_ERR;

    HAL_ASSERT(!(spi_idx == 1u));

    if (spi_idx == 1u)
    {
        return status;
    }

    status = execute_ss_command(IAP_BITSTREAM_AUTHENTICATE_CMD,
                                NULL_BUFFER,
                                0u,
                                NULL_BUFFER,
                                0u,
                                spi_idx,
                                0u);

    return status;
}

/***************************************************************************//**
 * SYS_digest_check_service()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_digest_check_service
(
    uint32_t options,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;
    uint32_t l_options = options;

    status = execute_ss_command(DIGEST_CHECK_CMD,
                                (uint8_t* )&l_options,
                                4u,
                                NULL_BUFFER,
                                0u,
                                mb_offset,
                                0u);
    return status;
}

/***************************************************************************//**
 * SYS_iap_service()
 * See "core_sysservices_pf.h" for details of how to use this function.
 */
uint8_t SYS_iap_service
(
    uint8_t iap_cmd,
    uint32_t spiaddr,
    uint16_t mb_offset
)
{
    uint8_t status = SYS_PARAM_ERR;
    uint16_t l_mb_offset = 0u;
    uint16_t cmd_data_size = 0u;
    uint8_t* cmd_data = NULL_BUFFER;
    bool invalid_param  = false;

    if (((IAP_PROGRAM_BY_SPIIDX_CMD == iap_cmd)
    || (IAP_VERIFY_BY_SPIIDX_CMD == iap_cmd))
    && (1u == spiaddr))
    {
        invalid_param = true;
        HAL_ASSERT(!invalid_param);
    }

    if (!invalid_param)
    {
        switch(iap_cmd)
        {
        case IAP_PROGRAM_BY_SPIIDX_CMD:
        case IAP_VERIFY_BY_SPIIDX_CMD:
            /*In SPI_IDX based program and verify commands,
             *  Mailbox is not Required. Instead of mailbox offset
             *  SPI_IDX is passed as parameter.*/
            l_mb_offset = (uint16_t)(0xFFu & spiaddr);
            break;

        case IAP_PROGRAM_BY_SPIADDR_CMD:
        case IAP_VERIFY_BY_SPIADDR_CMD:
            /*In SPI_ADDR based program and verify commands,
             *  Mailbox is Required*/
            l_mb_offset = mb_offset;
            /*command data size is four bytes holding the
             * SPI Address in it.*/
            cmd_data_size = 4u;
            cmd_data = (uint8_t*)&spiaddr;
            break;

        case IAP_AUTOUPDATE_CMD:
            /*In auto update command Mailbox is not Required*/
            l_mb_offset = 0u;
            break;

        default:
            l_mb_offset = 0u;

        }

        status = execute_ss_command(
                (uint8_t)iap_cmd,
                cmd_data,
                cmd_data_size,
                NULL_BUFFER,
                0,
                (uint16_t)l_mb_offset,
                0);
    }

    return status;
}

/***************************************************************************//**
 Internal functions.
*/
/*
This function executes the SS command. If Mailbox input data is required by the
it will first load it from cmd_data into the Mailbox. If the service requires
the response data to be read from mailbox, it will do so and store it in p_response.
*/
static uint8_t execute_ss_command
(
    uint8_t cmd_opcode,
    const uint8_t* cmd_data,
    uint16_t cmd_data_size,
    const uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset
)
{
   /* Pointer used during Writing to Mailbox memory. */
    uint32_t status = 0u;
    uint16_t idx = 0u;
    uint16_t ss_command = 0u;
    uint32_t* word_buf;
    uint16_t timeout_count = SS_TIMEOUT_COUNT;

    /* making sure that the system controller is not executing any service i.e.
       SS_USER_BUSY is gone 0 */

    while (1u == HAL_get_32bit_reg_field(g_css_pf_base_addr, SS_USER_BUSY))
    {
        --timeout_count;

        if (timeout_count == 0)
        {
            return SS_USER_BUSY_TIMEOUT;
        }
    }

    /* Form the SS command: bit 0to6 is the opcode, bit 7to15 is the Mailbox offset
     For some services this field has another meaning
    (e.g. for IAP bitstream auth. it means spi_idx) */
    ss_command = ((mb_offset << 7u) |  (cmd_opcode & 0x7Fu));

    /* Load the command register with the SS request command code*/
    HAL_set_32bit_reg(g_css_pf_base_addr, SS_CMD, ss_command);

    if (cmd_data_size > 0u)
    {
        HAL_ASSERT(!(NULL_BUFFER == cmd_data));
        HAL_ASSERT(!(cmd_data_size % 4u));

        /* Load the MBX_WCNT register with number of words */
        HAL_set_32bit_reg( g_css_pf_base_addr, MBX_WCNT, (cmd_data_size/4u));

        /* Load the MBX_WADDR register with offset of input data (write to Mailbox)
           For all the services this offset remains either 0 or Not applicable
           for the services in which no Mailbox write is required.*/
        HAL_set_32bit_reg( g_css_pf_base_addr, MBX_WADDR, (0x00u + mb_offset));

    }

    if (response_size > 0u)
    {
        HAL_ASSERT(!(NULL_BUFFER == p_response));
        HAL_ASSERT(!(response_size % 4u));

        /*
         Load the MBX_RWCNT register with number of words to be read from Mailbox
        */
        HAL_set_32bit_reg( g_css_pf_base_addr, MBX_RCNT, (response_size/4u));

        /*
         Load the MBX_RADRDESC register with offset address within the mailbox
         format for that particular service.
         It will be 0 for the services where there is no output data from G5CONTROL
         is expected.
         This function assumes that this value is pre-calculated by service specific
         functions as this value is fixed for each service.
        */
        HAL_set_32bit_reg( g_css_pf_base_addr, MBX_RADDR, (response_offset + mb_offset));
    }

    /*Set the request bit in SYS_SERV_REQ register to start the service*/
    HAL_set_32bit_reg_field(g_css_pf_base_addr, SS_REQ_REQ, 0x01u);

    if (cmd_data_size > 0u)
    {
        word_buf = (uint32_t*)cmd_data;

        /* Write the user data into mail box. */
        for (idx = 0u; idx < (cmd_data_size/4u); idx++)
        {
            HAL_set_32bit_reg( g_css_pf_base_addr, MBX_WDATA, word_buf[idx]);
        }
    }

    timeout_count = SS_TIMEOUT_COUNT;
    if (response_size > 0u)
    {
        word_buf = (uint32_t*)p_response;

        for (idx = 0u; idx < (response_size/4u); idx++)
        {
            while (0u == HAL_get_32bit_reg_field(g_css_pf_base_addr,
            SS_USER_RDVLD))
            {
                --timeout_count;

                if (timeout_count == 0)
                {
                    return SS_USER_RDVLD_TIMEOUT;
                }
            }
            word_buf[idx] = HAL_get_32bit_reg(g_css_pf_base_addr, MBX_RDATA);
        }
    }

    timeout_count = SS_TIMEOUT_COUNT;
    /* make sure that service is complete i.e. SS_USER_BUSY is gone 0 */
    while (1u == HAL_get_32bit_reg_field(g_css_pf_base_addr, SS_USER_BUSY))
    {
        --timeout_count;

        if (timeout_count == 0)
        {
            return SS_USER_RDVLD_TIMEOUT;
        }
    }

    /* Read the status returned by System Controller */
    status = HAL_get_32bit_reg(g_css_pf_base_addr, SS_STAT);

    return (uint8_t)status;
}

#ifdef __cplusplus
}
#endif
