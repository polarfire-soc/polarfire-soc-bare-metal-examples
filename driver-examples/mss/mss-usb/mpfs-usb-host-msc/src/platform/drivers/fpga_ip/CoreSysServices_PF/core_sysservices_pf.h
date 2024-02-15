/*******************************************************************************
 * Copyright 2019-2023 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * This file contains the application programming interface for the
 * CoreSysServices_PF bare metal driver.
 */
/*=========================================================================*//**
  @mainpage CoreSysServices_PF Bare Metal Driver.

  @section intro_sec Introduction
  The PolarFire System Services (PF_SYSTEM_SERVICES) SgCore enables executing
  the system services on the PolarFire and PolarFire SoC device. The system services 
  are the system controller actions initiated by the System Controller's System 
  Service Interface (SSI). The PolarFire System Services "SgCore" provides a 
  method to initiate these system services. The PF_SYSTEM_SERVICES interacts 
  with the system controller on SSI and Mailbox interface to initiate system 
  services, exchange data required for that services, and to know the successful
  completion or error status.

  The PF_SYSTEM_SERVICES provides an APB interface for controlling the registers 
  functions for controlling the PF_SYSTEM_SERVICES as part of a bare metal system
  register implemented within it. This software driver provides a set of where no 
  part of an operating system but the implementation of the adaptation layer
  operating system is available. This driver is adapted for use in
  between this driver and the operating system's driver model is outside the
  scope of this driver.

 ## Features
  The CoreSysServices_PF driver provides the following features:
          -   Executing device and design information services
          -   Executing design services
          -   Executing data security services
          -   Executing Fabric services

  The CoreSysServices_PF driver is provided as C source code.

  @section Driver Configuration
  The application software should configure the CoreSysServices_PF driver through
  calling the SYS_init() function. Only one instance of PF_SYSTEM_SERVICES SgCore is
  supported. No additional configuration files are required to use the driver.
  If using this driver on RT PolarFire device FPGA, define RT_DEVICE_FAMILY
  macro in application.

  @section theory_op Theory of Operation
  The CoreSysServices_PF driver provides access to the PolarFire system services.
  These system services are grouped into the following categories:

    Device and Design Information Service
          -  Serial Number Service
          -  USERCODE Service
          -  Design Info Service
          -  Device Certificate Services
          -  Read Digests
          -  Query Security
          -  Read Debug Info
          -  Read eNVM param

    Design Services
          -  Bitstream authentication service
          -  IAP bitstream authentication service

    Data Security Services
          -  Digital Signature Service
          -  Secure NVM (SNVM) Functions
          -  PUF Emulation Service
          -  Nonce Service

    Fabric Services
          -  Digest Check Service
          -  In Application programming(IAP)/Auto-Update service

  Initialization and Configuration

  The CoreSysServices_PF driver is initialized by calling the SYS_init()
  function. The SYS_init() function must be called before calling any other
  CoreSysServices_PF driver functions.

  Device and Design Information Services

  The CoreSysServices_PF driver is used to read information about the device
  and the design using the following functions:
      - SYS_get_serial_number()
      - SYS_get_user_code()
      - SYS_get_design_info()
      - SYS_get_device_certificate()
      - SYS_read_digest()
      - SYS_query_security()
      - SYS_read_debug_info()

  Design Authentication Services

  The CoreSysServices_PF driver is used to execute design services using the
  following functions:
      - SYS_bitstream_authenticate_service()
      - SYS_IAP_image_authenticate_service()

  Data Security Services

  The CoreSysServices_PF driver is used to execute data security services
  using the following functions:
      - SYS_digital_signature_service()
      - SYS_secure_nvm_write()
      - SYS_secure_nvm_read()
      - SYS_puf_emulation_service ()
      - SYS_nonce_service ()

  Executing Fabric Services
  
  The CoreSysServices_PF driver is used to execute fabric services using the
  following functions:
      - SYS_digest_check_service()
      - SYS_iap_service()

  All the service execution functions return the 8-bit status, which is returned
  by the system controller on executing the given service. A '0' value indicates
  successful execution of that service. A non-zero value indicates error.
  The error codes for each service are different. See individual function
  description to know the exact meanings of the error codes for each service.

  The function descriptions in this file mainly focus on the details required 
  by the user to use the APIs provided by this driver to execute the services.
  To know the complete details of the system services, see the 
  PolarFire FPGA and PolarFire SoC FPGA System Services [document](https://onlinedocs.microchip.com/pr/GUID-1409CF11-8EF9-4C24-A94E-70979A688632-en-US-3/index.html)
    
  *//*=========================================================================*/
#ifndef __CORE_SYSSERV_PF_H
#define __CORE_SYSSERV_PF_H 1

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"

#else
#include "hal.h"
#include "hal_assert.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
* # Service Execution Success and Error Status Codes
*
* The following status codes are the return values from the system service functions.
* For any service, a return value '0' indicates that the service was executed
* successfully. A non-zero return value indicates that the service was not
* executed successfully. For all the services, the return value represents the
* status code returned by the system controller for the respective service,
* except the values SYS_PARAM_ERR, SS_USER_BUSY_TIMEOUT, and SS_USER_RDVLD_TIMEOUT.
* These three values indicate the error conditions detected by this driver and
* they do not overlap with the status code returned by the system controller for
* any of the system service.
*
* SYS_SUCCESS  
*   System service executed successfully
*
* SYS_PARAM_ERR  
*   System service cannot be executed as one or more parameters are not as
*   expected by this driver. No read/write access is performed with the
*   IP.
*
* SS_USER_BUSY_TIMEOUT  
*   The System service request is initiated and the driver timed-out while
*   waiting for the system service to complete. The System Service
*   completion is indicated by de-assertion of the SS_USER_BUSY bit by the
*   IP.
*
* SS_USER_RDVLD_TIMEOUT  
*   The System service request is initiated and the driver timed-out while
*   waiting for SS_USER_RDVLD bit, which indicates availability of data to
*   be read from the mailbox, to become active.
*/
#define SYS_SUCCESS                                     0u
#define SYS_PARAM_ERR                                   0xFFu
#define SS_USER_BUSY_TIMEOUT                            0xFAu
#define SS_USER_RDVLD_TIMEOUT                           0xFBu

/**
*  # System Service Timeout Count
*
*    The SS_TIMEOUT_COUNT  value is used by the driver as a timeout count
*    while waiting for either the SS_USER_BUSY or SS_USER_RDVLD. This empirical
*    value is sufficiently large so that the operations are falsely
*    timeout in the normal circumstance. It is provided as a way to provide
*    more debug information to the application in case there are some
*    unforeseen issues. You may change this value for your need based on your
*    system design.
*/
#define SS_TIMEOUT_COUNT                                40000u
/*
 * SYS_DCF_DEVICE_MISMATCH
 *   Public key or FSN do not match device
 *
 *
 * SYS_DCF_INVALID_SIGNATURE
 *   Certificate signature is invalid
 *
 * SYS_DCF_SYSTEM_ERROR
 *   PUF or storage failure
 */
#define SYS_DCF_DEVICE_MISMATCH                         1u
#define SYS_DCF_INVALID_SIGNATURE                       2u
#define SYS_DCF_SYSTEM_ERROR                            3u

/*
 * SYS_NONCE_PUK_FETCH_ERROR
 *   Error fetching PUK
 *
 * SYS_NONCE_SEED_GEN_ERROR
 *   Error generating seed
 */
#define SYS_NONCE_PUK_FETCH_ERROR                       1u
#define SYS_NONCE_SEED_GEN_ERROR                        2u

/** 
 * # Secure Nvm Write Error Codes
 *
 * SNVM_WRITE_INVALID_SNVMADDR  
 *   Illegal page address
 *
 * SNVM_WRITE_FAILURE  
 *   PNVM program/verify failed
 *
 * SNVM_WRITE_SYSTEM_ERROR  
 *   PUF or storage failure
 *
 * SNVM_WRITE_NOT_PERMITTED  
 *   Write is not permitted
 */
#define SNVM_WRITE_INVALID_SNVMADDR                     1u
#define SNVM_WRITE_FAILURE                              2u
#define SNVM_WRITE_SYSTEM_ERROR                         3u
#define SNVM_WRITE_NOT_PERMITTED                        4u

/** 
 * # Secure Nvm Read Error Codes
 *
 * SNVM_READ_INVALID_SNVMADDR  
 *   Illegal page address
 *
 * SNVM_READ_AUTHENTICATION_FAILURE  
 *   Storage corrupt or incorrect USK
 *
 * SNVM_READ_SYSTEM_ERROR  
 *   PUF or storage failure
 *
 */
#define SNVM_READ_INVALID_SNVMADDR                      1u
#define SNVM_READ_AUTHENTICATION_FAILURE                2u
#define SNVM_READ_SYSTEM_ERROR                          3u

/** 
 * # Digital Signature Service Error Codes
 *
 * DIGITAL_SIGNATURE_FEK_FAILURE_ERROR  
 *   Error retrieving FEK
 *
 * DIGITAL_SIGNATURE_DRBG_ERROR  
 *   Failed to generate nonce
 *
 * DIGITAL_SIGNATURE_ECDSA_ERROR  
 *   ECDSA failed
 */
#define DIGITAL_SIGNATURE_FEK_FAILURE_ERROR             1u
#define DIGITAL_SIGNATURE_DRBG_ERROR                    2u
#define DIGITAL_SIGNATURE_ECDSA_ERROR                   3u

/** 
 * # Digest Check Error Codes
 *
 * NOTE: When these error occur, the DIGEST tamper flag is triggered.
 *
 * DIGEST_CHECK_FABRICERR  
 *   Fabric digest check error
 *
 * DIGEST_CHECK_CCERR  
 *   UFS Fabric Configuration (CC) segment digest check error
 *
 * DIGEST_CHECK_SNVMERR  
 *   ROM digest in SNVM segment digest check error
 *
 * DIGEST_CHECK_ULERR  
 *   UFS UL segment digest check error
 *
 * DIGEST_CHECK_UK0ERR  
 *   UKDIGEST0 in User Key segment digest check error
 *
 * DIGEST_CHECK_UK1ERR  
 *   UKDIGEST1 in User Key segment digest check error
 *
 * DIGEST_CHECK_UK2ERR  
 *   UKDIGEST2 in User Key segment (UPK1) digest check error
 *
 * DIGEST_CHECK_UK3ERR  
 *   UKDIGEST3 in User Key segment (UK1) digest check error
 *
 * DIGEST_CHECK_UK4ERR  
 *   UKDIGEST4 in User Key segment (DPK) digest check error
 *
 * DIGEST_CHECK_UK5ERR  
 *   UKDIGEST5 in User Key segment (UPK2) digest check error
 *
 * DIGEST_CHECK_UK6ERR  
 *   UKDIGEST6 in User Key segment (UK2) digest check error
 *
 * DIGEST_CHECK_UPERR  
 *   UFS Permanent Lock (UPERM) segment digest check error
 *
 * DIGEST_CHECK_SYSERR  
 *   M3 ROM, Factory and Factory Key Segments digest check error
 *
 */
#define DIGEST_CHECK_FABRICERR                          0x00u
#define DIGEST_CHECK_CCERR                              0x01u
#define DIGEST_CHECK_SNVMERR                            0x02u
#define DIGEST_CHECK_ULERR                              0x03u
#define DIGEST_CHECK_UK0ERR                             0x04u
#define DIGEST_CHECK_UK1ERR                             0x05u
#define DIGEST_CHECK_UK2ERR                             0x06u
#define DIGEST_CHECK_UK3ERR                             0x07u
#define DIGEST_CHECK_UK4ERR                             0x08u
#define DIGEST_CHECK_UK5ERR                             0x09u
#define DIGEST_CHECK_UK6ERR                             0x10u
#define DIGEST_CHECK_UPERR                              0x11u
#define DIGEST_CHECK_SYSERR                             0x12u

/**
 * # Bitstream Authentication and Iap Bitstream Authentication Return Status
 *
 * BSTREAM_AUTH_CHAINING_MISMATCH_ERR  
 *   Validator or hash chaining mismatch. Incorrectly constructed bitstream or
 *   wrong key used.
 *
 * BSTREAM_AUTH_UNEXPECTED_DATA_ERR  
 *   Unexpected data received.
 *   Additional data received after end of EOB component.
 *
 * BSTREAM_AUTH_INVALID_ENCRY_KEY_ERR  
 *   Invalid/corrupt encryption key.
 *   The requested key mode is disabled or the key could not be read/reconstructed.
 *
 * BSTREAM_AUTH_INVALID_HEADER_ERR  
 *   Invalid component header
 *
 * BSTREAM_AUTH_BACK_LEVEL_NOT_SATISFIED_ERR  
 *   Back level not satisfied
 *
 * BSTREAM_AUTH_ILLEGAL_BITSTREAM_MODE_ERR  
 *   Illegal bitstream mode.
 *   Requested bitstream mode is disabled by user security.
 *
 * BSTREAM_AUTH_DNS_BINDING_MISMATCH_ERR  
 *   DSN binding mismatch
 *
 * BSTREAM_AUTH_ILLEGAL_COMPONENT_SEQUENCE_ERR  
 *   Illegal component sequence
 *
 * BSTREAM_AUTH_INSUFF_DEVICE_CAPAB_ERR  
 *   Insufficient device capabilities
 *
 * BSTREAM_AUTH_INCORRECT_DEVICEID_ERR  
 *   Incorrect DEVICEID
 *
 * BSTREAM_AUTH_PROTOCOL_VERSION_ERR  
 *   Unsupported bitstream protocol version (regeneration required)
 *
 * BSTREAM_AUTH_VERIFY_ERR  
 *   Verify not permitted on this bitstream
 *
 * BSTREAM_AUTH_INVALID_DEV_CERT_ERR  
 *   Invalid Device Certificate.
 *   Device SCAC is invalid or not present.
 *
 * BSTREAM_AUTH_INVALID_DIB_ERR  
 *   Invalid DIB
 *
 * BSTREAM_AUTH_SPI_NOT_MASTER_ERR  
 *   Device not in SPI Master Mode.
 *   Error may occur only when bitstream is executed through IAP mode.
 *
 * BSTREAM_AUTH_AUTOIAP_NO_VALID_IMAGE_ERR  
 *   No valid images found.
 *   Error may occur when bitstream is executed through Auto Update mode.
 *   Occurs when no valid image pointers are found.
 *
 * BSTREAM_AUTH_INDEXIAP_NO_VALID_IMAGE_ERR  
 *   No valid images found.
 *   Error may occur when bitstream is executed through IAP mode via Index Mode.
 *   Occurs when No valid image pointers are found.
 *
 * BSTREAM_AUTH_NEWER_DESIGN_VERSION_ERR  
 *   Programmed design version is newer than AutoUpdate image found.
 *   Error may occur when bitstream is executed through Auto Update mode.
 *
 * BSTREAM_AUTH_INVALID_IMAGE_ERR  
 *   Selected image was invalid and no recovery was performed due to valid design
 *   in device.
 *   Error may occur only when bitstream is executed through Auto Update or IAP mode
 *   (This error is here for completeness but only can be observed by running the
 *   READ_DEBUG_INFO instruction and looking at IAP Error code field).
 *
 * BSTREAM_AUTH_IMAGE_PROGRAM_FAILED_ERR  
 *   Selected and Recovery image failed to program.
 *   Error may occur only when bitstream is executed through Auto Update or
 *   IAP mode
 *   (This error is here for completeness but only can be observed by running the
 *   READ_DEBUG_INFO instruction and looking at IAP Error code field).
 *
 * BSTREAM_AUTH_ABORT_ERR  
 *   Abort.
 *   Non-bitstream instruction executed during bitstream loading.
 *
 * BSTREAM_AUTH_NVMVERIFY_ERR  
 *   Fabric/UFS verification failed (min or weak limit)
 *
 * BSTREAM_AUTH_PROTECTED_ERR  
 *   Device security prevented modification of non-volatile memory
 *
 * BSTREAM_AUTH_NOTENA  
 *   Programming mode not enabled
 *
 * BSTREAM_AUTH_PNVMVERIFY  
 *   pNVM verify operation failed
 *
 * BSTREAM_AUTH_SYSTEM  
 *   System hardware error (PUF or DRBG)
 *
 * BSTREAM_AUTH_BADCOMPONENT  
 *   An internal error was detected in a component payload
 *
 * BSTREAM_AUTH_HVPROGERR  
 *   HV programming subsystem failure (pump failure)
 *
 * BSTREAM_AUTH_HVSTATE  
 *   HV programming subsystem in unexpected state (internal error)
 *
 */
#define BSTREAM_AUTH_CHAINING_MISMATCH_ERR              1
#define BSTREAM_AUTH_UNEXPECTED_DATA_ERR                2
#define BSTREAM_AUTH_INVALID_ENCRY_KEY_ERR              3
#define BSTREAM_AUTH_INVALID_HEADER_ERR                 4
#define BSTREAM_AUTH_BACK_LEVEL_NOT_SATISFIED_ERR       5
#define BSTREAM_AUTH_ILLEGAL_BITSTREAM_MODE_ERR         6
#define BSTREAM_AUTH_DNS_BINDING_MISMATCH_ERR           7
#define BSTREAM_AUTH_ILLEGAL_COMPONENT_SEQUENCE_ERR     8
#define BSTREAM_AUTH_INSUFF_DEVICE_CAPAB_ERR            9
#define BSTREAM_AUTH_INCORRECT_DEVICEID_ERR             10
#define BSTREAM_AUTH_PROTOCOL_VERSION_ERR               11
#define BSTREAM_AUTH_VERIFY_ERR                         12
#define BSTREAM_AUTH_INVALID_DEV_CERT_ERR               13
#define BSTREAM_AUTH_INVALID_DIB_ERR                    14
#define BSTREAM_AUTH_SPI_NOT_MASTER_ERR                 21
#define BSTREAM_AUTH_AUTOIAP_NO_VALID_IMAGE_ERR         22
#define BSTREAM_AUTH_INDEXIAP_NO_VALID_IMAGE_ERR        23
#define BSTREAM_AUTH_NEWER_DESIGN_VERSION_ERR           24
/* 25            Reserved */
#define BSTREAM_AUTH_INVALID_IMAGE_ERR                  26
#define BSTREAM_AUTH_IMAGE_PROGRAM_FAILED_ERR           27
#define BSTREAM_AUTH_ABORT_ERR                          127
#define BSTREAM_AUTH_NVMVERIFY_ERR                      128
#define BSTREAM_AUTH_PROTECTED_ERR                      129
#define BSTREAM_AUTH_NOTENA                             130
#define BSTREAM_AUTH_PNVMVERIFY                         131
#define BSTREAM_AUTH_SYSTEM                             132
#define BSTREAM_AUTH_BADCOMPONENT                       133
#define BSTREAM_AUTH_HVPROGERR                          134
#define BSTREAM_AUTH_HVSTATE                            135

/***************************************************************************//**
 * # Mailbox ECC Status
 *  Provides ECC status when the mailbox is read. The values are as follows:  
 *  00: No ECC errors detected, data is correct.  
 *  01: Exactly one bit error occurred and has been corrected.  
 *  10: Exactly two bits error occurred and no correction performed.  
 *  11: Reserved.
 */
#define SYS_MBOX_ECC_NO_ERROR_MASK                      0x00u
#define SYS_MBOX_ONEBIT_ERROR_CORRECTED_MASK            0x40u
#define SYS_MBOX_TWOBIT_ERROR_MASK                      0xC0u

/***************************************************************************//**
 * Service request command opcodes:
*/
#define SERIAL_NUMBER_REQUEST_CMD                       0x00u
#define USERCODE_REQUEST_CMD                            0x01u
#define DESIGN_INFO_REQUEST_CMD                         0x02u
#define DEVICE_CERTIFICATE_REQUEST_CMD                  0x03u
#define READ_DIGEST_REQUEST_CMD                         0x04u
#define QUERY_SECURITY_REQUEST_CMD                      0x05u
#define READ_DEBUG_INFO_REQUEST_CMD                     0x06u
#define READ_ENVM_PARAM_REQUEST_CMD                     0x07u
#define SNVM_NON_AUTHEN_TEXT_REQUEST_CMD                0x10u
#define SNVM_AUTHEN_TEXT_REQUEST_CMD                    0x11u
#define SNVM_AUTHEN_CIPHERTEXT_REQUEST_CMD              0x12u
#define SNVM_READ_REQUEST_CMD                           0x18u
#define DIGITAL_SIGNATURE_RAW_FORMAT_REQUEST_CMD        0x19u
#define PUF_EMULATION_SERVICE_REQUEST_CMD               0x20u
#define NONCE_SERVICE_REQUEST_CMD                       0x21u
#define DIGITAL_SIGNATURE_DER_FORMAT_REQUEST_CMD        0x1Au

#define BITSTREAM_AUTHENTICATE_CMD                      0x23u
#define IAP_BITSTREAM_AUTHENTICATE_CMD                  0x22u

#define DIGEST_CHECK_CMD                                0x47u

#define IAP_PROGRAM_BY_SPIIDX_CMD                       0x42u
#define IAP_VERIFY_BY_SPIIDX_CMD                        0x44u
#define IAP_PROGRAM_BY_SPIADDR_CMD                      0x43u
#define IAP_VERIFY_BY_SPIADDR_CMD                       0x45u
#define IAP_AUTOUPDATE_CMD                              0x46u

/***************************************************************************//**
 * Service request Mailbox return data length
 */
#define SERIAL_NUMBER_RESP_LEN                          16u
#define USERCODE_RESP_LEN                               4u
#define DESIGN_INFO_RESP_LEN                            36u
#define DEVICE_CERTIFICATE_RESP_LEN                     1024u
#define READ_DIGEST_RESP_LEN                            416u
#define QUERY_SECURITY_RESP_LEN                         9u
#define READ_DEBUG_INFO_RESP_LEN                        76u
#define READ_ENVM_PARAM_RESP_LEN                        256u
#define NONCE_SERVICE_RESP_LEN                          32u

#define PUF_EMULATION_SERVICE_CMD_LEN                   20u
#define PUF_EMULATION_SERVICE_RESP_LEN                  32u

#define DIGITAL_SIGNATURE_HASH_LEN                      48u
#define DIGITAL_SIGNATURE_RAW_FORMAT_RESP_SIZE          96u
#define DIGITAL_SIGNATURE_DER_FORMAT_RESP_SIZE          104u

#define USER_SECRET_KEY_LEN                             12u

/* Same driver can be used on PolarFire SoC platform and the response length
 * is different for PolarFire SoC. Constants defined below are used only when the
 * PF System services driver is used with PolarFire SoC Platform.
 */
#define READ_DIGEST_MPFS_RESP_LEN                      576u
#define QUERY_SECURITY_MPFS_RESP_LEN                   33u

/* SNVM Input data length from sNVM write. */
#ifndef RT_DEVICE_FAMILY
/* SNVMADDR + RESERVED + PT + USK */
#define NON_AUTHENTICATED_TEXT_DATA_LEN                 256u

/* SNVMADDR + RESERVED + PT */
#define AUTHENTICATED_TEXT_DATA_LEN                     252u
#else
/* SNVMADDR + RESERVED + PT + USK */
#define NON_AUTHENTICATED_TEXT_DATA_LEN                 224u

/* SNVMADDR + RESERVED + PT */
#define AUTHENTICATED_TEXT_DATA_LEN                     220u
#endif

/** 
 * # Digest Check Input Options
 *
 * DIGEST_CHECK_FABRIC  
 *   Carry out digest check on Fabric
 *
 * DIGEST_CHECK_CC  
 *   Carry out digest check on UFS Fabric Configuration (CC) segment
 *
 * DIGEST_CHECK_SNVM  
 *   Carry out digest check on ROM digest in SNVM segment
 *
 * DIGEST_CHECK_UL  
 *   Carry out digest check on UFS UL segment
 *
 * DIGEST_CHECK_UKDIGEST0  
 *   Carry out digest check on UKDIGEST0 in User Key segment
 *
 * DIGEST_CHECK_UKDIGEST1  
 *   Carry out digest check on UKDIGEST1 in User Key segment
 *
 * DIGEST_CHECK_UKDIGEST2  
 *   Carry out digest check on UKDIGEST2 in User Key segment (UPK1)
 *
 * DIGEST_CHECK_UKDIGEST3  
 *   Carry out digest check on UKDIGEST3 in User Key segment (UK1)
 *
 * DIGEST_CHECK_UKDIGEST4  
 *   Carry out digest check on UKDIGEST4 in User Key segment (DPK)
 *
 * DIGEST_CHECK_UKDIGEST5  
 *   Carry out digest check on UKDIGEST5 in User Key segment (UPK2)
 *
 * DIGEST_CHECK_UKDIGEST6  
 *   Carry out digest check on UKDIGEST6 in User Key segment (UK2)
 *
 * DIGEST_CHECK_UPERM  
 *   Carry out digest check on UFS Permanent lock (UPERM) segment
 *
 * DIGEST_CHECK_SYS  
 *   Carry out digest check on Factory and Factory Key Segments
 *
 */
#define DIGEST_CHECK_FABRIC                             (0x01<<0x00u)  /*Fabric digest*/
#define DIGEST_CHECK_CC                                 (0x01<<0x01u)  /*UFS Fabric Configuration (CC) segment*/
#define DIGEST_CHECK_SNVM                               (0x01<<0x02u)  /*ROM digest in SNVM segment*/
#define DIGEST_CHECK_UL                                 (0x01<<0x03u)  /*UFS UL segment*/
#define DIGEST_CHECK_UKDIGEST0                          (0x01<<0x04u)  /*UKDIGEST0 in User Key segment*/
#define DIGEST_CHECK_UKDIGEST1                          (0x01<<0x05u)  /*UKDIGEST1 in User Key segment*/
#define DIGEST_CHECK_UKDIGEST2                          (0x01<<0x06u)  /*UKDIGEST2 in User Key segment (UPK1)*/
#define DIGEST_CHECK_UKDIGEST3                          (0x01<<0x07u)  /*UKDIGEST3 in User Key segment (UK1)*/
#define DIGEST_CHECK_UKDIGEST4                          (0x01<<0x08u)  /*UKDIGEST4 in User Key segment (DPK)*/
#define DIGEST_CHECK_UKDIGEST5                          (0x01<<0x09u)  /*UKDIGEST5 in User Key segment (UPK2)*/
#define DIGEST_CHECK_UKDIGEST6                          (0x01<<0x0au)  /*UKDIGEST6 in User Key segment (UK2)*/
#define DIGEST_CHECK_UPERM                              (0x01<<0x0bu)  /*UFS Permanent lock (UPERM) segment*/
#define DIGEST_CHECK_SYS                                (0x01<<0x0cu)  /*Factory and Factory Key Segments.*/

/***************************************************************************//**
 * The function SYS_init() is used to initialize the internal data structures of
 * this driver. Currently this function is empty.
 *
 * @param base_addr  The base_addr parameter specifies the base address of the
 *                     PF_System_services core.
 *
 * @return      This function does not return a value.
 */
void
SYS_init
(
    uint32_t base_addr
);

/***************************************************************************//**
 * The function SYS_get_serial_number() is used to execute "serial number" system
 * service.
 *
 * @param p_serial_number  The p_serial_number parameter is a pointer to a buffer
 *                         in which the data returned by system controller
 *                         is copied.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              This function returns the status code returned by the
 *                      system controller for this service. A '0' status code
 *                      means that the service was executed successfully.
 */
uint8_t
SYS_get_serial_number
(
    const uint8_t * p_serial_number,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The function SYS_get_user_code() is used to execute "USERCODE" system
 * service.
 * @param p_user_code   The p_user_code parameter is a pointer to a buffer
 *                      in which the data returned by system controller is
 *                      copied.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              This function returns the status code returned by the
 *                      system controller for this service. A '0' status code
 *                      means that the service was executed successfully.
 */
uint8_t
SYS_get_user_code
(
    const uint8_t * p_user_code,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The function SYS_get_design_info() is used to execute "Get Design Info" system
 * service.
 *
 * @param p_design_info  The p_design_info parameter is a pointer to a buffer
 *                       in which the data returned by system controller is
 *                       copied. Total size of debug information is 36 bytes.
 *                       The data from the system controller includes the 256-bit
 *                       user-defined design ID, 16-bit design version, and 16-bit
 *                       design back level.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              This function returns the status code returned by the
 *                      system controller for this service. A '0' status code
 *                      means that the service was executed successfully.
 */
uint8_t
SYS_get_design_info
(
    const uint8_t * p_design_info,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The function SYS_get_device_certificate() is used to execute "Get Device
 * Certificate" system service.
 *
 * @param p_device_certificate The p_device_certificate parameter is a pointer
 *                             to a buffer in which the data returned by the
 *                             system controller is copied.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              This function returns the status code returned by the
 *                      system controller for this service. A '0' status code means that
 *                      the service was executed successfully.
 *
 */
uint8_t
SYS_get_device_certificate
(
    const uint8_t * p_device_certificate,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The function SYS_read_digest() is used to execute "Read Digest" system service.
 *
 * @param p_digest      The p_digest parameter is a pointer to a buffer
 *                      in which the data returned by system controller is
 *                      copied.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              This function returns the status code returned by the
 *                      system controller for this service. A '0' status code
 *                      means that the service was executed successfully.
 */
uint8_t SYS_read_digest
(
   const uint8_t * p_digest,
   uint16_t mb_offset
);

/***************************************************************************//**
 * The function SYS_query_security() is used to execute "Query Security" system
 * service.
 *
 * @param p_security_locks The p_security_locks parameter is a pointer to a buffer
 *                         in which the data returned by system controller is copied.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              This function returns the status code returned by the
 *                      system controller for this service. A '0' status code means that
 *                      the service was executed successfully.
 */
uint8_t SYS_query_security
(
    uint8_t * p_security_locks,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The function SYS_read_debug_info() is used to execute "Read Debug info" system
 * service.
 *
 * @param p_debug_info  The p_debug_info parameter is a pointer to a buffer
 *                      in which the data returned by system controller is
 *                      copied.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              This function returns the status code returned by the
 *                      system controller for this service. A '0' status code
 *                      means that the service was executed successfully.
 */
uint8_t SYS_read_debug_info
(
    const uint8_t * p_debug_info,
    uint16_t mb_offset
);

#ifdef CORESYSSERVICES_PFSOC
/***************************************************************************//**
 * The function SYS_read_envm_parameter() is used to retrieve all parameters needed
 * for the eNVM operation and programming.
 *
 * NOTE: This service is available only on PolarFire SoC Platform.
 *       This service is not yet supported by PF_SYSTEM_SERVICES 3.0.100.
 *
 * @param p_envm_param  The p_envm_param parameter is a pointer to a buffer
 *                      in which the data returned by system controller is copied.
 *                      This buffer stores all the eNVM parameters.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return              The SYS_read_envm_parameter service will return zero if the
 *                      service executed successfully, otherwise, it will return
 *                      one indicating error.
 */
uint8_t SYS_read_envm_parameter
(
    uint8_t * p_envm_param,
    uint16_t mb_offset
);
#endif
/***************************************************************************//**
 * The function SYS_puf_emulation_service() is used to authenticate a device.
 *
 * The SYS_puf_emulation_service() function accept a challenge comprising a
 * 8-bit optype and 128-bit challenge and return a 256-bit response unique to
 * the given challenge and the device.
 *
 * @param p_challenge  The p_challenge parameter specifies the 128-bit challenge
 *                     to generate the 256-bits unique response.
 *
 * @param op_type      The op_type parameter specifies the operational parameter
 *                     to generate the 256-bits unique response.
 *
 * @param p_response   The p_response parameter is a pointer to a buffer where 
 *                     the data returned which is the response by system controller
 *                     is copied.
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return             The SYS_puf_emulation_service function will return zero
 *                     if the service executed successfully, otherwise, it will
 *                     return one indicating error.
 */
uint8_t SYS_puf_emulation_service
(
    const uint8_t * p_challenge,
    uint8_t op_type,
    uint8_t* p_response,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The SYS_digital_signature_service() function is used to generate P-384 ECDSA
 * signature based on SHA384 hash value.
 *
 * @param p_hash       The p_hash parameter is a pointer to the buffer which
 *                     contain the 48 bytes SHA384 Hash value (input value).
 *
 * @param format       The format parameter specifies the output format of
 *                     generated SIGNATURE field. The different types of output
 *                     signature formats are as follow:
 *                      - DIGITAL_SIGNATURE_RAW_FORMAT
 *                      - DIGITAL_SIGNATURE_DER_FORMAT
 *
 * @param p_response   The p_response parameter is a pointer to a buffer that
 *                     contains the generated ECDSA signature. The field may be
 *                     96 bytes or 104 bytes depending upon the output format.
 *
 * @param mb_offset    The mb_offset parameter specifies the offset from
 *                     the start of Mailbox where the data related to this service
 *                     is available. Note that all accesses to the mailbox
 *                     are of word length (4 bytes). Value '10' of this parameter
 *                     means that the data access area for this service
 *                     starts from 11th word (offset 10) in the Mailbox.
 *
 * @return             The SYS_digital_signature_service function returns
 *                     zero if the service executed successfully, otherwise, it
 *                     returns non-zero values indicating error.
 */
uint8_t SYS_digital_signature_service
(
    const uint8_t* p_hash,
    uint8_t format,
    uint8_t* p_response,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The SYS_secure_nvm_write() function writes data in the sNVM region.
 * Data gets stored in the following format:
 *   - Non-authenticated plaintext
 *   - Authenticated plaintext
 *   - Authenticated ciphertext
 *
 * Note: If you are executing this function with Authenticated plaintext
 *   or Authenticated ciphertext on a device whose sNVM was never previously
 *   written to, then the service may fail. For it to work, you must first write
 *   Authenticated data to the sNVM using Libero along with USK client and
 *   custom security. This flow generates the SMK. See UG0753 PolarFire FPGA
 *   Security User Guide for further details.

 * @param format       The format parameter specifies the format used to write
 *                     data in sNVM region. The different type of text formats
 *                     are as follow:
 *                          - NON_AUTHENTICATED_PLAINTEXT_FORMAT
 *                          - AUTHENTICATED_PLAINTEXT_FORMAT
 *                          - AUTHENTICATED_CIPHERTEXT_FORMAT
 *
 * @param snvm_module   The snvm_module parameter specifies the the sNVM module
 *                     in which the data need to be written.
 *
 * @param p_data       The p_data parameter is a pointer to a buffer which
 *                     contains the data to be stored in sNVM region. The data
 *                     length to be written is fixed depending on the format
 *                     parameter. If NON_AUTHENTICATED_PLAINTEXT_FORMAT is
 *                     selected, then you can write 252 bytes in the sNVM module.
 *                     For other two formats the data length is 236 bytes.
 *
 * @param p_user_key   The p_user_key parameter is a pointer to a buffer which
 *                     contain the 96-bit key USK (user secret key). This user
 *                     secret key will enhance the security when authentication
 *                     is used. That is, when Authenticated plaintext and
 *                     Authenticated ciphertext format is selected.
 *
 * @param mb_offset    The mb_offset parameter specifies the offset from
 *                     the start of Mailbox where the data related to this service
 *                     is available. Note that all accesses to the mailbox
 *                     are of word length (4 bytes). Value '10' of this parameter
 *                     means that the data access area for this service
 *                     starts from 11th word (offset 10) in the Mailbox.
 *
 * @return             The SYS_digital_signature_service function returns
 *                     zero if the service executed successfully, otherwise, it
 *                     returns non-zero values indicating error.
 */
uint8_t SYS_secure_nvm_write
(
    uint8_t format,
    uint8_t snvm_module,
    const uint8_t* p_data,
    const uint8_t* p_user_key,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The SYS_secure_nvm_read() function is used to read data present in sNVM region.
 * User should provide USK key, if the data was programmed using authentication.
 * If the data was written in the sNVM using the authenticated plaintext or the
 * authenticated ciphertext service option then this service will return the
 * valid data only when authentication is successful. For more details, see
 * SYS_secure_nvm_write() function. If the data was written in
 * the sNVM using the authenticated plaintext or the authenticated ciphertext
 * service option then this service will return the valid data only when
 * authentication is successful. For more details, see SYS_secure_nvm_write()
 * function and its parameter description.
 *
 * @param snvm_module  The snvm_module parameter specifies the sNVM module
 *                     from which the data need to be read.
 *
 * @param p_user_key   The p_user_key parameter is a pointer to a buffer which
 *                     contain the 96-bit key USK (user secret key). User should
 *                     provide same secret key which is previously used for
 *                     authentication while writing data in sNVM region.
 *
 * @param p_admin      The p_admin parameter is a pointer to the buffer where
 *                     the output page admin data is stored. The page admin
 *                     data is 4 bytes long.
 *
 * @param p_data       The p_data parameter is a pointer to a buffer which
 *                     contains the data read from sNVM region. User should
 *                     provide the buffer large enough to store the read data.
 *
 * @param data_len     The data_len parameter specifies the number of bytes to be
 *                     read from sNVM.
 *                     The application should know whether the data written in the
 *                     chose sNVM module was previously stored using Authentication
 *                     or not.
 *                     The data_len should be 236 bytes, for authenticated data.
 *                     For not authenticated data the data_len should be 252 bytes.
 *
 * @param mb_offset    The mb_offset parameter specifies the offset from
 *                     the start of Mailbox where the data related to this service
 *                     is available. Note that all accesses to the mailbox
 *                     are of word length (4 bytes). Value '10' of this parameter
 *                     means that the data access area for this service
 *                     starts from 11th word (offset 10) in the Mailbox.
 *
 * @return             The SYS_digital_signature_service function returns
 *                     zero if the service executed successfully, otherwise, it
 *                     returns non-zero values indicating error.
 */
uint8_t SYS_secure_nvm_read
(
    uint8_t snvm_module,
    const uint8_t* p_user_key,
    uint8_t* p_admin,
    uint8_t* p_data,
    uint16_t data_len,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The function SYS_nonce_service() is used to issue "Nonce Service" system
 * service to the system controller.
 *
 * @param p_nonce  The p_nonce parameter is a pointer to a buffer
 *                 in which the data returned by system controller is copied.
 *
 * @param mb_offset    The mb_offset parameter specifies the offset from
 *                     the start of Mailbox where the data related to this service
 *                     is available. Note that all accesses to the mailbox
 *                     are of word length (4 bytes). Value '10' of this parameter
 *                     means that the data access area for this service
 *                     starts from 11th word (offset 10) in the Mailbox.
 *
 * @return          This function returns the status code returned by the
 *                  system controller for this service. A '0' status code means 
 *                  that the service was executed successfully and a non-zero 
 *                  value indicates error. See the document link 
 *                  provided in the theory of operation section to know more 
 *                  about the service and service response.
 */
uint8_t SYS_nonce_service
(
    const uint8_t * p_nonce,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The SYS_bitstream_authenticate_service() function is used to authenticate
 * the Bitstream which is located in SPI through a system service routine. Prior
 * to using the IAP service, it may be required to first validate the new
 * bitstream before committing the device to reprogramming, thus avoiding the
 * need to invoke recovery procedures if the bitstream is invalid.
 *
 * This service is applicable to bitstreams stored in SPI Flash memory only.
 *
 * @param spi_flash_address
 *              The spi_flash_address parameter specifies the address within
 *              SPI Flash memory where the bit-stream is stored.
 *
 * @param mb_offset  The mb_offset parameter specifies the offset from
 *                   the start of Mailbox where the data related to this service
 *                   is available. Note that all accesses to the mailbox
 *                   are of word length (4 bytes). Value '10' of this parameter
 *                   means that the data access area for this service
 *                   starts from 11th word (offset 10) in the Mailbox.
 *
 * @return           The SYS_bitstream_authenticate_service function will return
 *                   zero if the service executed successfully and the non-zero
 *                   response from system controller indicates error. See
 *                   the document link provided in the theory of
 *                   operation section to know more about the service and service
 *                   response.
 */
uint8_t SYS_bitstream_authenticate_service
(
    uint32_t spi_flash_address,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The SYS_IAP_image_authenticate_service() function is used to authenticate
 * the IAP image which is located in SPI through a system service routine. The
 * service checks the image descriptor and the referenced bitstream and optional
 * initialization data.  If the image is authenticated successfully, then the
 * image is guaranteed to be valid when used by an IAP function.
 *
 * This service is applicable to bitstreams stored in SPI Flash memory only.
 *
 * @param spi_idx
 *              The spi_idx parameter specifies the index in the SPI directory to
 *              be used where the IAP bit-stream is stored.  
 *  Note: To support recovery SPI_IDX=1 should be an empty slot and the recovery
 *        image should be located in SPI_IDX=0. Since SPI_IDX=1 should be an
 *        empty slot, it shouldn’t be passed into the system service.
 *
 * @return           The SYS_IAP_image_authenticate_service function will return
 *                   zero if the service executed successfully the non-zero
 *                   response from system controller indicates error. Please
 *                   refer to the document link provided in the theory of
 *                   operation section to know more about the service and service
 *                   response.
 */
uint8_t SYS_IAP_image_authenticate_service
(
    uint8_t spi_idx
);

/***************************************************************************//**
 * The SYS_digest_check_service() function is used to Recalculates and compares
 * digests of selected non-volatile memories. If the fabric digest is to be
 * checked, then the user design must follow all prerequisite steps for the
 * FlashFreeze service before invoking this service.  
 * This service is applicable to bitstreams stored in SPI Flash memory only.
 * @param options
 *              The options parameter specifies the digest check options which
 *              indicate the area on which the digest check should be performed.
 *              Below is the list of options. You can OR these options to indicate
 *              to perform digest check on multiple segments.  
 *              Note: The options parameter is of 2 bytes when used with PF
 *              device and 4 bytes when used with PolarFire SoC device.  
 *                  Options[i]     | Description
 *                  ---------------|----------------------------------
 *                     0x01        | Fabric digest
 *                     0x02        | Fabric Configuration (CC) segment
 *                     0x04        | ROM digest in SNVM segment
 *                     0x08        | UL segment
 *                     0x10        | UKDIGEST0 in User Key segment
 *                     0x20        | UKDIGEST1 in User Key segment
 *                     0x40        | UKDIGEST2 in User Key segment (UPK1)
 *                     0x80        | UKDIGEST3 in User Key segment (UK1)
 *                     0x100       | UKDIGEST4 in User Key segment (DPK)
 *                     0x200       | UKDIGEST5 in User Key segment (UPK2)
 *                     0x400       | UKDIGEST6 in User Key segment (UK2)
 *                     0x800       | UFS Permanent lock (UPERM) segment
 *                     0x1000      | Factory and Factory Key Segments.
 *                     0x2000      | UKDIGEST7 in User Key segment (HWM) (PFSoC)
 *                     0x4000      | ENVMDIGEST (PFSoC only)
 *                     0x8000      | UKDIGEST8 for MSS Boot Info (PFSoC only)
 *                     0x10000     | SNVM_RW_ACCESS_MAP Digest (PFSoC only)
 *                     0x20000     | SBIC revocation digest (PFSoC only)
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *
 * @return           The SYS_digest_check_service function will return
 *                   zero if the service executed successfully the non-zero
 *                   response from system controller indicates error. Pleaes
 *                   refer to the document link provided in the theory of
 *                   operation section to know more about the service and service
 *                   response.
 */
uint8_t SYS_digest_check_service
(
    uint32_t options,
    uint16_t mb_offset
);

/***************************************************************************//**
 * The SYS_iap_service() function is used to IAP service. The IAP service allows 
 * the user to reprogram the device without the need for an external master.  The
 * user design writes the bitstream to be programmed into a SPI Flash connected
 * to the SPI port.  When the service is invoked, the System Controller
 * automatically reads the bitstream from the SPI flash and programs the device.
 * The service allows the image to be executed in either VERIFY or PROGRAM modes.
 * Another option for IAP is to perform the auto-update sequence. In this case
 * the newest image of the first two images in the SPI directory is chosen to be
 * programmed.
 *
 * @param iap_cmd
 *              The iap_cmd parameter specifies the specific IAP command which
 *              depends upon VERIFY or PROGRAM modes and the SPI address method.
 *              iap_cmd                | Description
 *              -----------------------|------------
 *        IAP_PROGRAM_BY_SPIIDX_CMD    | IAP program.
 *        IAP_VERIFY_BY_SPIIDX_CMD     | Fabric Configuration (CC) segment
 *        IAP_PROGRAM_BY_SPIADDR_CMD   | ROM digest in SNVM segment
 *        IAP_VERIFY_BY_SPIADDR_CMD    | UL segment
 *        IAP_AUTOUPDATE_CMD           | UKDIGEST0 in User Key segment
 *
 * @param spiaddr
 *              The spiaddr parameter specifies either the index
 *              in the SPI directory or the SPI address in the SPI Flash memory.
 *              Below is the list of the possible meaning of spiaddr parameter
 *              in accordance with the iap_cmd parameter.
 *                      iap_cmd               |          spiaddr
 *                      ----------------------|-----------------
 *              IAP_PROGRAM_BY_SPIIDX_CMD     |  Index in the SPI directory.
 *              IAP_VERIFY_BY_SPIIDX_CMD      |  Index in the SPI directory.
 *              IAP_PROGRAM_BY_SPIADDR_CMD    |  SPI address in the SPI Flash memory
 *              IAP_VERIFY_BY_SPIADDR_CMD     |  SPI address in the SPI Flash memory
 *              IAP_AUTOUPDATE_CMD            |  spiaddr is ignored as No index/address required for this command.  
 *
 * @param mb_offset     The mb_offset parameter specifies the offset from
 *                      the start of Mailbox where the data related to this service
 *                      is available. Note that all accesses to the mailbox
 *                      are of word length (4 bytes). Value '10' of this parameter
 *                      means that the data access area for this service
 *                      starts from 11th word (offset 10) in the Mailbox.
 *  Note: For the IAP services with command IAP_PROGRAM_BY_SPIIDX_CMD and
 *        IAP_VERIFY_BY_SPIIDX_CMD To support recovery SPI_IDX=1 should be an
 *        empty slot and the recovery image should be located in SPI_IDX=0.
 *        Since SPI_IDX=1 should be an empty slot it shouldn’t be passed into
 *        the system service.
 *
 * @return      The SYS_iap_service function will return zero if the service
 *              executed successfully and the non-zero response from system
 *              controller indicates error. Please refer to the document
 *              link provided in the theory of operation section to know
 *              more about the service and service response.
 */
uint8_t SYS_iap_service
(
    uint8_t iap_cmd,
    uint32_t spiaddr,
    uint16_t mb_offset
);

#ifdef __cplusplus
}
#endif

#endif /* __CORE_SYSSERV_PF_H */
