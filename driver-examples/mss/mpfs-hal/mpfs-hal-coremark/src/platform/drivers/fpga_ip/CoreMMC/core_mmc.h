/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_mmc.h
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreMMC prototypes
 *
 */

/**
  @mainpage CoreMMC Bare Metal Driver

  @section intro Introduction
  Core Multi-Media Card (CoreMMC) is an IP component that facilitates
  connection to an external embedded Multi-Media Card (MMC) with configurable
  MMC bus width and data rates. This driver only supports eMMC devices with a
  capacity more than 2 GB, bus widths of 1-bit, 4-bits, and 8-bits and at MMC
  clock rates of up to 52 MHz.

  This driver provides a set of functions for configuring and controlling
  CoreMMC as part of a bare metal system, where no operating system is
  available. This driver can be adapted as part of an operating system, but the
  implementation of the adaptation layer between the driver and the operating
  system's driver model is outside the scope of this user guide.

  This driver is not compatible with the CoreMMC versions below v3.0.

  @subsection features Features



  The CoreMMC driver provides support for the following features:

    - Initialization of the CoreMMC hardware
    - Configuration of the MMC data rate
    - Automatically drives the bus width connected (1-bit, 4-bit, 8-bit
  connections)
    - Single 512-byte block write
    - Single 512-byte block read
    - Multiple 512-byte block writes
    - Multiple 512-byte block reads

  The CoreMMC driver is provided as C source code.

  Supported Hardware IP:  The CoreMMC bare metal driver is only compatible
  with CoreMMC v3.0 and higher.

  @section driver_configuration Driver Configuration

  For each CoreMMC instance in the hardware design, your application software
  must configure the CoreMMC driver by calling `MMC_param_config()` and `
  MMC_init()`. These functions configure a default set of parameters for this
  instance, that includes the CoreMMC hardware instance base address and the
  MMC clock rate divider.

  The CoreMMC instance is configured at the time of instantiation in the
  hardware design for the FIFO depth and the MMC data width parameters.

  This driver supports the control of a single eMMC device connected to the MMC
  data bus for each CoreMMC hardware instance.

  Note 1: When using the driver, the clock divider settings must be adjusted
  based on the clock speed of the associated AHB HCLK in the Libero design.

  Note 2: When using the non-blocking single read and write API functions, or
  the multiple block read/write API functions, the interrupt signal from
  CoreMMC must be connected to the processor in the Libero design.

  Note 3: When using the multiple block read/write API functions, the size of
  the multi-block read or write (number of blocks  block size) used with these
  functions must not be greater than the FIFO size configured for CoreMMC in
  the Libero design.

  @section theory_op Theory of Operation

  The CoreMMC driver functions allows 512-byte blocks of data to be written to
  and read from a single eMMC device connected to each instance of the CoreMMC
  hardware.
  The blocks can be read/written individually or by using the multi-block
  functions. The single block functions are available in two variants. One set
  is blocking, the other is non-blocking. The multi-block functions are
  non-blocking.

  Note: The eMMC device connected to the CoreMMC hardware instance must support
  a 512-byte sector size. This is the default block size for the MMC device
  less than 2 GB.

  @section init Initialization
  The initialization of the CoreMMC driver involves the following main steps:
    - Initialize the `mmc_params_t` data structure.
    - Specify the custom `clk_rate_div` and `init_clk_rate_div` configuration.
    - Instantiate the HAL interrupt handler associated with the interrupt line
     connected to the CoreMMC instance in the Libero hardware design.
    - Call `MSS_isr()` from within that handler.
    - Call `MMC_init()`.

  During the first step, `MMC_param_config()` is called, which loads
  the `mmc_params_t` data structure with the default values.
  This configures the `clk_rate_div` parameter such that the MMC clock rate is
  1/2 of HCLK. This also configures the `init_clk_rate_div` parameter such that
  the MMC clock rate is 1/128 of HCLK on startup.

  If the default clock rate is sufficient for your clock customization
  application, you can skip this step in the initialization process.

  Note: On start-up, the clock rate of the eMMC must be set to a maximum of 400
  KHz.
  The `init_clk_rate_div` parameter is adjusted to achieve this using the
  following formula.

  MMC Clock Rate = HCLK / (2  (clk_rate_div + 1))

  The maximum permitted clock rate of the eMMC, which is set by clk_rate_div,
  is 52 MHz.

  The `clk_rate_div` parameter in the `mmc_params_t` data structure must be
  modified based on your system's clock rate by calling `MMC_param_config()`
  and `MMC_init()`.
  If the application program modifies the value of `clk_rate_div` after calling
  `MMC_init()`, the modified value has no effect.

  The final step involves calling `MMC_init()`.
  `MMC_init()` takes: a pointer to a global CoreMMC instance data structure
  of the `mmc_instance_t` type; the base address of the CoreMMC instance as
  defined by the hardware design; and a pointer to a global parameter data
  structure of the `mmc_params_t` type.

  The driver uses the CoreMMC global instance data structure to store the state
  information for each CoreMMC instance. Except for MMC_param_config(), a
  pointer to this data structure is also used as the first parameter to any of
  the driver functions to identify which CoreMMC instance is being used by the
  called function. The application programmer is responsible for creating and
  maintaining these global CoreMMC instances and parameter data structures.

  Note:  `MMC_init()` must be called before any other driver functions is
  called.

  @section mmc_sb_tf_ctrl MMC Single Block Transfer Control

  @subsection wr_tf Write Transfer

  To write a single block of data to the eMMC device in non-blocking mode, call
  `MC_single_block_write_nb()`.
  The user must specify the following information:

   - The CoreMMC hardware instance connected to the required eMMC device.
   - The base address of the 512-byte buffer holding the data to transfer.
   - The destination sector address, where the data is stored within the eMMC
  device.
   - The current system ticks.
   - The number of system ticks before a timeout occurs.

  To check the status of the write transfer, use `MMC_status()`.
  Alternatively, use `MMC_single_block_write()` to perform a single block write
  operation in blocking mode. This function waits until an error occurs, or the
  write operation completes.

  @subsection rd_tf Read Transfer

  To read a single block of data within the eMMC device in non-blocking mode,
  call `MMC_single_block_read_nb()`. The user must specify the following
  information:

   - The CoreMMC hardware instance to which the required eMMC device is
  connected.
   - The sector address within the eMMC device where the requested data is
  located.
   - The base address of the 512-byte buffer where the data read from the eMMC
  device is stored.

  To check the status of the read transfer, use `MMC_status()`.
  Alternatively, use `MMC_single_block_read()` to perform a single block read
  operation in blocking mode. This function waits until an error occurs, or the
  read operation completes.

  @section mmc_mb_tf_ctrl MMC Multiple Block Transfer Control

  @subsection multi_blk_wr_tf Multiple Block Write Transfer

  To write multiple blocks of data to the eMMC device, call
  `MMC_multi_block_write()`. The user must specify the following information:

   - The CoreMMC hardware instance to which the required eMMC device is
  connected.
   - The destination sector address on the eMMC device
   - The number of sectors to write.
   - The current system ticks.
   - The number of system ticks before a timeout occurs.

  To check the status of the read operation, call `MMC_status()`.
  The source data is assumed to be present in or in the process of being
  transferred (using the DMA) to the CoreMMC FIFO.
  The maximum size that can be transferred per function call is limited by the
  maximum size of the CoreMMC FIFO buffer that is instantiated.
  Currently the maximum possible size is 32 KB.

  @subsection multi_blk_rd_tf Multiple Block Read Transfer

  To read multiple blocks of data stored within the eMMC device, call
  `MMC_multi_block_read()`. The user must specify the following information:

   - The CoreMMC hardware instance to which the required eMMC device is
  connected.
   - The sector address within the eMMC device where the requested data is
  located.
   - The number of blocks to read.
   - The current system ticks.
   - The number of system ticks before a timeout occurs.

  To check the status of the read operation, call `MMC_status()`.
  If the read operation is successful, the received data is stored in the
  Read FIFO.
  The size of the read is limited to the maximum size of the CoreMMC FIFO
  buffer, that is instantiated. Currently, the maximum possible size is 32 KB.
 */

#ifndef __CORE_MMC_H
#define __CORE_MMC_H

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/cpu_types.h"

#else
#include "cpu_types.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
  The `mmc_transfer_status_t` enumeration indicates the return status of the
  MMC data transfer.
  A variable of this type is returned by the `MMC_init()`, `MMC_status()`,
  `MMC_single_block_write()`, `MMC_single_block_read()`,
  `MMC_single_block_write_nb()`, `MMC_single_block_read_nb()`,
  `MMC_multi_block_write()`, and `MMC_multi_block_read()` functions.

  - `MMC_TRANSFER_FAIL`: A failure during block data transfer. This response
  indicates
  that an error is detected within the eMMC device's card status register.

  - `MMC_TRANSFER_SUCCESS`: Successful MMC block data transfer. This response
  indicates that the data transfer has been completed successfully and that the
  driver is ready to proceed with subsequent transfers.

  - `MMC_INIT_SUCCESS`: Initialization of the driver, the CoreMMC hardware, and
  the eMMC device is completed successfully.

  Note: No other driver functions can be called until `MMC_init()` has returned
  this response.

  - `MMC_DWIDTH_ERR`: MMC data width configuration error.
  This response indicates that the MMC card is unreachable and may be in a
  lockdown state, having previously received an invalid command.

  - `MMC_RCA_ERROR`: An error occurred while assigning a relative card address
  to the eMMC device.
  This response indicates that multiple devices are connected to the same
  CoreMMC hardware instance, which is not supported by the driver.

  - `MMC_CID_RESP_ERR`: An error occurred while reading the card identification
  register.
  This response indicates that the MMC device entered Lockdown mode due to
  incompatibility issues.

  - `MMC_OP_COND_ERR`: An error occurred while specifying the interface
  operating conditions for the eMMC device.

  Note: The device is now in Lockdown mode and may only be recovered by
  performing a hard reset of the system.

  - `MMC_RESET_ERR`: An error occurred during the reset cycle of the
  initialization.
  This response indicates that the device is incorrectly wired to the CoreMMC
  hardware instance.

  - `MMC_CRC_ERR`: A CRC error is detected on the MMC interface during a block
  write or block read data transfer.

  - `MMC_FIFO_N_EMPTY`: This response indicates the presence of data in the
  hardware FIFO prior to calling the `MMC_single_block_read()` function.

  - `MMC_FIFO_N_EMPTY`: This response indicates the presence of data in the
  hardware FIFO prior to calling the `MMC_single_block_read()` function.

  - `MMC_READ_CTRL_ERR`: A control error occurred during a data read transfer.
  This response indicates that a FIFO overflow occurred or that there is a
  start/stop bit error on the MMC command interface.

  - `MMC_CLK_DIV_ERR`: An error occurred while configuring the device to
  operate in high-speed mode.
  This response indicates that the device does not support the MMC clock rate
  configured or that it does not have a high-speed mode configuration setting in
  the EXT CSD register.
 */

typedef enum mmc_transfer_status_ {
  MMC_TRANSFER_FAIL = 0u,
  MMC_TRANSFER_SUCCESS = 1u,
  MMC_INIT_SUCCESS = 2u,
  MMC_DWIDTH_ERR = 3u,
  MMC_RCA_ERROR = 4u,
  MMC_CID_RESP_ERR = 5u,
  MMC_OP_COND_ERR = 6u,
  MMC_RESET_ERR = 7u,
  MMC_CRC_ERR = 8u,
  MMC_FIFO_N_EMPTY = 9u,
  MMC_READ_CTRL_ERR = 10u,
  MMC_CLK_DIV_ERR = 11u,    /**< Busy, wait a while*/
  MMC_CMD_PROCESSING = 12u, /**< Multiblock-write fail*/
  MMC_MBW_FAIL = 13u,       /**< Multiblock-read fail*/
  MMC_MBR_FAIL = 15u,       /**< Single block-write fail*/
  MMC_SBW_FAIL = 16u,       /**< Single block-read fail*/
  MMC_SBR_FAIL =
      17u, /**< Application timeout- Passed timeout set by application*/
  MMC_APP_TIMEOUT_FAIL = 18u, /**< Interrupt status error*/
  MMC_INT_STATUS_FAIL =
      19u, /**< Application has over or under filled the FIFO*/
  MMC_USER_ERROR_FAIL = 20u, /**< Interrupt flags showed error*/
  MMC_OTHER_FAIL = 21u,      /**< CoreMMC timed out when sending or receiving*/
  MMC_TIMOUT_FAIL =
      22u, /**< Hardware revision of CoreMMC not supported by this driver*/
  MMC_UNSUPPORTED_HW_REVISION = 23u, /**< No response to command from eMMC*/
  MMC_NO_RESPONSE_FAIL = 24u
} mmc_transfer_status_t;

/**
  States used in the CoreMMC interrupt state machine.
 */
typedef enum mmc_state_ {
  MMC_ISR_MW_PARSE_CMD13,
  MMC_ISR_MW_PARSE_CMD23,
  MMC_ISR_MW_PARSE_CMD25,
  MMC_ISR_MW_END,
  MMC_ISR_SW_PARSE_CMD13,
  MMC_ISR_SW_PARSE_CMD24,
  MMC_ISR_SW_END,
  MMC_ISR_MR_PARSE_CMD13,
  MMC_ISR_MR_PARSE_CMD23,
  MMC_ISR_MR_PARSE_CMD18,
  MMC_ISR_MR_END,
  MMC_ISR_SR_PARSE_CMD13,
  MMC_ISR_EXT_CSD_PARSE_CMD13,
  MMC_ISR_SR_PARSE_CMD17,
  MMC_ISR_SR_END,
  MMC_ISR_STOP_TRANSMISSION,
  MMC_ISR_ERROR,
  MMC_ISR_IDLE
} mmc_state;

/**
  ## READ_SEND_EXT_CSD

  A number outside of the normal range is used to instruct the read block
  routine to read the extended CSD register.
 */
#define READ_SEND_EXT_CSD 0xFFFFFFFFU

/**
  The `mmc_status` structure contains status information relating to current
  Firmware driver operations and the particular instance of CoreMMC.

  -  `int_state`: contains the current state of the CoreMMC interrupt state
  machine.
  -  `no_of_blks`: number of blocks to either send or receive in the current
  command.
  -  `blk_addr`: block to start read/write from.
  -  `start_systick`: set at start to enable timeout.
  -  `timeout_value_ticks`: in systicks.
  -  `err_state`: error report from interrupt.
  -  `temp_err_state`: temporary store for error reports from interrupt.
 */

typedef struct mmc_status_ {
  mmc_state int_state;
  uint16_t no_of_blks;
  uint32_t blk_addr;
  uint32_t start_systick;
  uint32_t timeout_value_ticks;
  mmc_transfer_status_t err_state;
  mmc_transfer_status_t temp_err_state;
} mmc_status;

/**
  The `mmc_instance_t` structure stores information on each instance of CoreMMC
  hardware in your system.
  Your application software must declare one instance of this structure for
  each instance of CoreMMC in your system.

  `MMC_init()` initializes this structure.
  A pointer to an initialized instance of this structure must be passed as the
  first parameter to the CoreMMC driver functions.
  This identifies which CoreMMC hardware instance will perform the requested
  operation.

  - `base_addr`: `base_addr` represents the base address in the processor's
  memory map of the registers for this CoreMMC instance.

  - `hw_sec_count`: `hw_sec_count` is the sector count read from the eMMC.
  It is the maximum sector count supported by the particular eMMC device.
  The device has a size of `hw_sec_count` * 512.

  - `sector_size`: `sector_size` indicates the size of the memory sectors
  (in Bytes) within the eMMC device.
  All operations must be carried out on entire sectors.

  Note 1: This driver only supports 512-byte sector size.

  Note 2: `sector_size` is maintained within the driver and must
  not be modified by the user application.

  - `access_mode`: `access_mode` indicates the memory addressing system
  (Byte or Sector Addressing) used within the eMMC device.
  This driver only supports sector access.

  Note: `access_mode` is maintained within the driver and must
  not be modified by the user application.

  - `hw_fifo_depth`: `hw_fifo_depth` indicates the size of the FIFO
  instantiated within this instance of the CoreMMC hardware.

  Note: `hw_fifo_depth` is maintained within the driver and must
  not be modified by the user application.

  - `hw_mmc_dwidth`: `hw_mmc_dwidth` denotes the width of the MMC interface
  connected between the eMMC device and this instance of the CoreMMC hardware.

  Note 1: This driver automatically configures the device and hardware to drive
  the width of the bus connected, for example a 1-bit, 4-,bit or 8-bit
  interface.

  Note 2: `hw_mmc_dwidth` is maintained within the driver and must
  not be modified by the user application.

  - `clk_rate_div`: `clk_rate_div` internally sets the clock rate of the MMC
  interface.

  Note: `clk_rate_div` is maintained within the driver and must
  not be modified by the user application.

  - `init_clk_rate_div`: `init_clk_rate_div` internally sets the initialization
  clock rate of the MMC interface.

  Note: `init_clk_rate_div` is maintained within the driver and
  must not be modified by the user application.

  - `hw_hs_timing`: `hw_hs_timing` contains contents read from the extended CSD
  register's `HS_TIMING` byte. It has a default value of 0x00 and is set to
  0x01 when the eMMC device operates in high-speed mode.
 */

typedef struct mmc_instance {
  addr_t base_addr;
  uint32_t hw_sec_count;
  uint8_t hw_ext_csd_rev; /**< This contains the extended CSD rev read during
                    eMMC initialisation*/
  uint8_t hw_card_type; /**< This contains the cared type read from the extended
                  CSD register*/
  uint16_t sector_size;
  uint8_t access_mode;
  uint8_t hw_fifo_depth;
  uint8_t hw_maj_revision; /**< This is the major version of CoreMMC. Only
                    present in Cores >= 3.00*/
  uint8_t hw_min_revision; /**< This is the minor version of CoreMMC. Only
                    present in Cores >= 3.00 >= 3.0*/
  uint8_t hw_mmc_dwidth;
  uint8_t clk_rate_div;
  uint8_t init_clk_rate_div;
  mmc_status status;     /**< Firmware_status*/
  uint32_t data_timeout; /**< Sets the length in cycles before timeout occurs*/
  uint8_t hw_hs_timing;
} mmc_instance_t;

/**
  The `mmc_params_t` structure passes configuration parameters to `MMC_init()`.
  Your application software must declare one instance of this structure for
  each instance of CoreMMC in your system. `MMC_param_config()` initializes
  this structure. To configure the driver for your specific application,
  provide custom configuration parameters by passing a pointer to an
  initialized instance of this structure to `MMC_init()`.

  - `clk_rate_div`: `clk_rate_div` assigns a default MMC clock
  rate of HCLK/2.
  If this MMC clock rate is not suitable for the custom application, then
  `clk_rate_div` can be modified to be to any value in the range of 0-255,
  giving an MMC clock rate range of HCLK/2 to HCLK/512. The relationship
  between the MMC clock rate, HCLK, and `clk_rate_div` is as follows: MMC Clock
  = HCLK/(2(`clk_rate_div` + 1))

  - `init_clk_rate_div`: The maximum clock frequency on startup is 400 KHz.
  `init_clk_rate_div` of this structure assigns a default MMC clock
  initialization rate of HCLK/128. If this MMC clock rate is not suitable for
  the custom application, then `init_clk_rate_div` can be modified to be to any
  value in the range of 0-255, giving an MMC initialization clock rate range of
  HCLK/2 to HCLK/512. The relationship between the MMC clock rate, HCLK, and
  `init_clk_rate_div` is as follows: MMC Clock = HCLK/(2(`init_clk_rate_div` +
  1))

  - `valid_param_config`: `valid_param_config` is maintained within the driver
  and must not be modified by the user application.
 */

typedef struct mmc_params {
  uint8_t clk_rate_div;
  uint8_t init_clk_rate_div;
  uint8_t valid_param_config;
  uint32_t data_timeout; /**< Sets the length in cycles before timeout occurs */
} mmc_params_t;

/*------------------------Public Functions------------------------------------*/

/**
  `MMC_param_config()` assigns default values to the members of the
  `mmc_params_t` structure. This function must be called prior to calling
  `MMC_init()`. By default, this function sets the clock divider rate to 1/2
  times that of HCLK.

  @param this_mmc_params
  The `this_mmc_params` parameter is a pointer to the `mmc_params_t` structure,
  which holds the parameters required to initialize this instance of CoreMMC.
  This parameter must point to the instance of this structure, defined within
  the application code.

  @return
  This function does not return any value.
 */
void MMC_param_config(mmc_params_t *this_mmc_params);

/**
  `MMC_init()` initializes:

   - The data structures of a CoreMMC instance, referenced by the `this_mmc`
  parameter.
   - The CoreMMC hardware.
   - The eMMC device connected to the MMC interface of the CoreMMC hardware
  instance.

  This function must be called for each CoreMMC instance with a unique this_mmc
  parameter and `base_address` parameter combination.
  `MMC_init()` must be called prior to calling any MMC data transfer functions,
  and must be preceded by calling the `MMC_param_config()` function.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to the specific CoreMMC hardware instance.

  @param base_address
  The `base_address` parameter specifies the base address in the processor's
  memory map for the registers of the CoreMMC instance being initialized.

  @param this_mmc_params
  The `this_mmc_params` parameter is a pointer to the `mmc_params_t` structure,
  holding the parameters required for the initial configuration of the CoreMMC
  instance.

  @return
  This function returns the initialization status of the MMC device of
  the `mmc_transfer_status_t` type.

  @example
  This example shows how to initialize the device and configure the MMC data
  rate to 1/10 of HCLK.

  @code
    #define MMC0_BASE_ADDR 0x30000000u

    mmc_instance_t g_mmc0;
    mmc_params_t g_mmc0_param;
    mmc_transfer_status_t resp_reg;

    MMC_param_config(&g_mmc0_param);

    // Set the MMC clock rate to 1/10 HCLK.
    g_mmc0_param.clk_rate_div = 4u;
    resp_reg = MMC_init(&g_mmc0, MMC0_BASE_ADDR, &g_mmc0_param);
    if(MMC_INIT_SUCCESS == resp_reg)
    {
      // ...
    }
  @endcode
 */

mmc_transfer_status_t MMC_init(mmc_instance_t *this_mmc, addr_t base_address,
                               const mmc_params_t *this_mmc_params);

/**
  `MMC_get_fifo_write_address()` returns the address of the Write
  FIFO data register of the CoreMMC.
  This is required when setting up DMA to write to the FIFO.
  It can also be used to write to the FIFO using a pointer in preference to the
  HAL register access macros, which are slower.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @return
  This function returns the address of the CoreMMC Write FIFO.

  @example
  This example shows how to use `MMC_get_fifo_write_address()`.

  @code
    uint8_t *pFifoData;
    // ...
    pFifoData = (uint8_t *)MMC_get_fifo_write_address(&g_emmc_1);
    MMC_init_fifo( &g_emmc_1);
    iCount= no_blocks * size_of_block;
    while(iCount--)
    {
       pFifoData = storage[iCount];
    }
  @endcode
 */

addr_t MMC_get_fifo_write_address(mmc_instance_t *this_mmc);

/**
  `MMC_get_fifo_read_address()` returns the address of Read FIFO data register
  for the CoreMMC.
  This is required when setting up DMA to read from the FIFO.
  It can also be used to read from FIFO using a pointer in preference to the
  HAL register access macros, which are slower.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @return
  This function returns the address of the CoreMMC Read FIFO.

  @example
  This example shows how to use `MMC_get_fifo_read_address()`.

  @code
    uint8_t *pFifoData;
    // ...
    pFifoData = (uint8_t *)MMC_get_fifo_read_address(&g_emmc_1);
    iCount= no_blocks *size_of_block;
    while(iCount--)
    {
      storage[iCount] = *pFifoData; ..fills memory from FIFO
    }
  @endcode
 */

addr_t MMC_get_fifo_read_address(mmc_instance_t *this_mmc);

/**
  `MMC_status()` returns the current state of the read/write process initiated
  by any of the non-blocking functions. The non-blocking functions, which can
  be used to start a read/write transfer are `MMC_multi_block_write()`,
  `MMC_single_block_write_nb()`, `MMC_single_block_read_nb()`,
  `MMC_multi_block_write()`, and `MMC_multi_block_read()`.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param current_timeout_ticks
  The `current_timeout_ticks` parameter passes the current tick in milliseconds
  to the function to allow it determine if operation timed out.

  @return
  This function returns the transfer status of the `mmc_transfer_status_t`
  type.

  @example
  This example shows how to use `MMC_status()` with `MMC_multi_block_write()`
  command.

  @code
    // ...
    ret_var = MMC_multi_block_write( &g_emmc_1, no_blocks, sect_addr, timeout);
    while(MMC_CMD_PROCESSING == MMC_status(&g_emmc_1))
    {
      ;
      // wait for MMC_multi_block_write() to finish
      // Could do other stuff here
    }
    if(MMC_TRANSFER_SUCCESS != MMC_status(&g_emmc_1, ))
    {
      // handle the error
    }
    else
    {
      // write success, continue on
    }
  @endcode
 */

mmc_transfer_status_t MMC_status(mmc_instance_t *this_mmc,
                                 uint32_t current_timeout_ticks);

/**
  `MMC_init_fifo()` clears the content of both the Read and Write FIFOs, and
  returns them to their reset state. `MMC_init_fifo()` must be called before
  transferring data into the Read FIFO or Write FIFO, when using the
  non-blocking read and write functions. The non-blocking functions are
  `MMC_single_block_write_nb()`, `MMC_single_block_read_nb()`,
  `MMC_multi_block_write()`, and `MMC_multi_block_read()`.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @return
  This function does not return a value.

  @example
  This example shows how to use `MMC_init_fifo()` when filling FIFO.

  @code
    uint8_t *pFifoData;
    // ...
    pFifoData = (uint8_t *)MMC_get_fifo_write_address(&g_emmc_1);
    MMC_init_fifo( &g_emmc_1);  // makes sure FIFO is empty before starting
    iCount= no_blocks *size_of_block;
    while(iCount--)
    {
       *pFifoData = storage[iCount];
    }
  @endcode
 */

void MMC_init_fifo(const mmc_instance_t *this_mmc);

/**
  `MMC_single_block_write()` transmits a single block of data from the MMC host
  to the eMMC device. This function writes the block of data into the hardware
  FIFO, transfers the data from the hardware FIFO to the eMMC device, and waits
  for write operation to complete. The size of the block of data transferred by
  this function is set to 512 bytes, which is the standard sector size for all
  eMMC devices with a capacity of greater than 2 GB.

  Note 1: This driver does not support devices with a capacity of less than 2
  GB.

  Note 2: This function is a blocking function, it does not return until the
  write operation is successful or an error occurs.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param src_addr
  The `src_addr` parameter is a pointer to a buffer containing the data that is
  stored in the eMMC device.
  The buffer must be a minimum size of 512 bytes.

  Note: If the buffer is not declared with adequate size, then the eMMC sector
  may be loaded with erroneous data.

  @param dst_addr
  The `dst_addr` parameter specifies the sector address in the MMC device where
  the block must be stored.

  Note: For eMMC devices of greater than 2 GB in size this address refers to a
  512-byte sector.

  @return
  This function returns a value of the `mmc_transfer_status_t` type which
  specifies the transfer status of the operation.

  @example
  This example shows how to initialize the device and perform a single block
  transfer.

  @code
    #define MMC0_BASE_ADDR 0x30000000u
    #define BLOCK_1      0x00000001u

    mmc_instance_t g_mmc0;
    mmc_params_t g_mmc0_param;

    mmc_transfer_status_t resp_reg;
    uint8_t data_buffer[512] = {0u};

    MMC_param_config(&g_mmc0_param);
    NVIC_EnableIRQ(FabricIrq0_IRQn);

    Set the MMC clock rate to 1/10 HCLK.
    g_mmc0_param.clk_rate_div = 4u;
    resp_reg = MMC_init(&g_mmc0, MMC0_BASE_ADDR, &g_mmc0_param);
    if(MMC_INIT_SUCCESS == resp_reg)
    {
      resp_reg= MMC_single_block_write(&g_mmc0,
                             data_buffer,
                             BLOCK_1);
      if(MMC_TRANSFER_SUCCESS == resp_reg)
      {
        // ...
      }
    }
  @endcode
 */

mmc_transfer_status_t MMC_single_block_write(const mmc_instance_t *this_mmc,
                                             const uint32_t *src_addr,
                                             uint32_t dst_addr);

/**
  `MMC_single_block_write_nb()` transfers a single block
  of data from the MMC host to the eMMC device. This function requires the
  program to fill the CoreMMC Write FIFO with the correct amount of data in
  advance. This can be achieved by doing a memory copy before calling this
  function, or setting up a DMA transfer before calling this function.
  `MMC_single_block_write_nb()` initiates the transfer of the data
  from the Write FIFO to the eMMC device.
  The size of the block of data transferred by this function is set to 512
  bytes, which is the standard sector size for all eMMC devices with a
  capacity of greater than 2 GB.
  To check the status of a read operation, call `MMC_status()`.

  Note 1: This driver does not support devices with a capacity of less than 2
  GB.

  Note 2: `MMC_init_fifo()` must be used called to clear the Write FIFO before
  writing any data to the FIFO and before calling this function.

  Note 3: This function is a non-blocking function, it returns immediately
  after initiating the write transfer.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param dst_addr
  The `dst_addr` parameter specifies the sector address in the MMC device where
  the block must be stored.

  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
  512-byte sector.

  @param current_timeout_ticks
  The `current_timeout_ticks` parameter specifies the current tick in
  milliseconds.

  @param timeout_ticks
  This `timeout_ticks` parameter  specifies the timeout abort value, in
  milliseconds.

  @return
  This function returns a value of the `mmc_transfer_status_t` type which
  specifies the transfer status of the operation.

  @example
  This example shows how to initialize the device and perform a single block
  transfer.

  @code
    #define MMC0_BASE_ADDR 0x30000000u
    #define BLOCK_1        0x00000001u

    mmc_instance_t g_mmc0;
    mmc_params_t g_mmc0_param;

    mmc_transfer_status_t resp_reg;
    uint8_t data_buffer[512] = {0u};

    MMC_param_config(&g_mmc0_param);

    // Set the MMC clock rate to 1/10 HCLK.
    g_mmc0_param.clk_rate_div = 4u;
    resp_reg = MMC_init(&g_mmc0, MMC0_BASE_ADDR, &g_mmc0_param);
    if (MMC_INIT_SUCCESS == resp_reg)
    {
      resp_reg = MMC_single_block_write(&g_mmc0, data_buffer, BLOCK_1);
      if (MMC_TRANSFER_SUCCESS == resp_reg)
      {
        // ...
      }
    }
  @endcode
 */

mmc_transfer_status_t MMC_single_block_write_nb(mmc_instance_t *this_mmc,
                                                uint32_t dst_addr,
                                                uint32_t current_timeout_ticks,
                                                uint32_t timeout_ticks);

/**
  `MMC_multi_block_write()` transfers multiple blocks of data from the MMC host
  to the eMMC device. This function requires the program to fill the CoreMMC
  Write FIFO with the correct amount of data in advance. This can be achieved
  by doing a memory copy before calling this function, or setting up a DMA
  transfer before calling this function. The size of the individual block of
  data transferred by this function is set to 512 bytes, which is the standard
  sector size for all eMMC devices with a capacity of greater than 2 GB. To
  check the status of a read operation, call `MMC_status()`.

  Note 1: This driver does not support devices with a capacity of less than 2
  GB.

  Note 2: `MMC_init_fifo()` must be called to clear the Write FIFO before
  writing any data to the FIFO and before calling this function.

  Note 3: This function is a non-blocking function and returns immediately
  after initiating the transfer.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param no_of_blks
  The `no_of_blks` parameter specifies the number of contiguous blocks to
  transfer.

  @param dst_addr
  The `dst_addr` parameter specifies the sector address in the MMC device where
  the block must be stored.

  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
  512-byte sector.

  @param current_timeout_ticks
  The `current_timeout_ticks` parameter specifies the current tick in
  milliseconds.

  @param timeout_ticks
  The `timeout_ticks` parameter specifies the timeout abort value in
  milliseconds.

  @return
  This function returns a value of the `mmc_transfer_status_t` type which
  specifies the transfer status of the operation.

  @example
  This example shows how to initialize the device and perform a multiple block
  transfer.

  @code
    volatile uint32_t *write_fifo =
        (uint32_t*)MMC_get_fifo_write_address(&g_emmc_1);
    MMC_init_fifo(&g_emmc_1); // Ensure the write FIFO is empty

    mmc_command_status =
        MMC_multi_block_write((mmc_instance_t *)&g_emmc_1,
                              number_of_sectors,
                              sector_address,
                              get_clock_ticks(),
                              TIMEOUT_10_SECS);

    // Fill the write FIFO form the write buffer
    for (uint32_t index = 0; index < (number_of_sectors * SECTOR_SIZE_WORDS);
        index++)
    {
      *write_fifo = *(write_buffer + index);
    }

    while (MMC_CMD_PROCESSING == mmc_command_status)
    {
      mmc_command_status = MMC_status(&g_emmc_1, get_clock_ticks());
    }
    if (MMC_TRANSFER_SUCCESS == mmc_command_status)
    {
      // Success, continue
    }
    else
    {
      // Handle the error
    }
  @endcode
 */

mmc_transfer_status_t MMC_multi_block_write(mmc_instance_t *this_mmc,
                                            uint16_t no_of_blks,
                                            uint32_t dst_addr,
                                            uint32_t current_timeout_ticks,
                                            uint32_t timeout_ticks);

/**
  `MMC_single_block_read` reads a single block of data from the eMMC slave to
  the FIFO on the CoreMMC. The function configures the CoreMMC Read FIFO
  hardware to receive a block of data, instructs the eMMC device to transmit
  the data stored at the sector pointed to by the `src_addr` parameter, and
  waits to complete the read operation. The size of the block of data
  transferred by this function is set to 512 bytes, which is the standard
  sector size for all eMMC devices with a capacity of greater than 2 GB.

  Note 1: This driver does not support devices with a capacity of less than 2
  GB.

  Note 2: This function is a blocking function, it does not return until the
  read operation is successful or an error occurs.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param src_addr
  The `src_addr` parameter specifies the sector address in the MMC device where
  the 512-byte block of data is read from.

  @param dst_addr
  The `dst_addr` parameter is a pointer to a buffer where the data read from
  the eMMC device is stored. The buffer must be a minimum size of 512 bytes.

  Note: If the buffer is not declared with adequate size, the response data
  read-back from the eMMC device overflows from the receive buffer and causes
  its storage to become unpredictable

  @return
  This function returns a value of the `mmc_transfer_status_t` type which
  specifies the transfer status of the operation.

  @example
  This example shows how to initialize the device, perform a single block
  transfer, and read back the data from the sector written to within the eMMC
  device.

  @code
    #define MMC0_BASE_ADDR 0x30000000u
    #define BLOCK_1      0x00000001u

    mmc_instance_t g_mmc0;
    mmc_params_t g_mmc0_param;

    mmc_transfer_status_t resp_reg;
    uint8_t tx_data_buffer[512] = {0u};
    uint8_t rx_data_buffer[512] = {0u};

    MMC_param_config(&g_mmc0_param);

    Set the MMC clock rate to 1/10 HCLK.
    g_mmc0_param.clk_rate_div = 4u;

    resp_reg = MMC_init(&g_mmc0, MMC0_BASE_ADDR, &g_mmc0_param);
    if(MMC_INIT_SUCCESS == resp_reg)
    {
      resp_reg= MMC_single_block_write(&g_mmc0,
                            tx_data_buffer,
                            BLOCK_1);
      if(MMC_TRANSFER_SUCCESS == resp_reg)
      {
         resp_reg = MMC_single_block_read(&g_mmc0,
                              BLOCK_1,
                              rx_data_buffer);
         if(MMC_TRANSFER_SUCCESS == resp_reg)
         {
            // ...
         }
      }
    }

  @endcode
 */
mmc_transfer_status_t MMC_single_block_read(const mmc_instance_t *this_mmc,
                                            uint32_t src_addr,
                                            uint32_t *dst_addr);

/**
  `MMC_single_block_read_nb()` reads a single block of data from the eMMC
  device to the MMC host. The function configures the CoreMMC Read FIFO to
  receive a block of data and instructs the eMMC device to transmit the data
  stored at the sector pointed to by the `src_addr` parameter. The size of the
  block of data transferred by this function is set to 512 bytes, which is the
  standard sector size for all eMMC devices with a capacity of greater than 2
  GB. To check the status of a read operation, call `MMC_status()`. If read
  operation is successful, the received data is stored in the Read FIFO. You
  must call `MMC_get_fifo_read_address()` to get the address of the Read FIFO
  data register and copy the data from the Read FIFO register to a buffer after
  the read operation has completed.

  Note 1: This driver does not support devices with a capacity of less than 2
  GB.

  Note 2: `MMC_init_fifo()` must be used to clear the FIFO before calling this
  function.

  Note 3: This function is a non-blocking function and returns immediately
  after initiating the read transfer.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param src_addr
  The `src_addr` parameter specifies the sector address in the eMMC device
  where the 512-byte block of data is read from.

  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
  512-byte sector.

  @param current_timeout_ticks
  The `current_timeout_ticks` parameter specifies the current tick in
  milliseconds.

  @param timeout_ticks
  This `timeout_ticks` parameter specifies the timeout abort value in
  milliseconds.

  @return
  This function returns a value of the `mmc_transfer_status_t` type which
  specifies the transfer status of the operation.

  @example
  This example shows how to initialize the device, perform a single block
  transfer, and read back the data from the sector written to within the eMMC
  device.

  @code
    MMC_init_fifo( &g_emmc_1);
    ret_var = MMC_single_block_read_nb( &g_emmc_1, sect_addr ,
                            GetTickCount(), TEN_SEC_TIMEOUT);
    if(MMC_CMD_PROCESSING == ret_var)
    {
      while(MMC_CMD_PROCESSING == MMC_status(&g_emmc_1,GetTickCount()))
      {
         ;
      }
      if(MMC_TRANSFER_SUCCESS == MMC_status(&g_emmc_1,GetTickCount()))
      {
        // empty the FIFO
        pFifoData = (uint8_t *)MMC_get_fifo_read_address(&g_emmc_1);
        while (size_of_transfer--)
        {
          g_write_data_buff[temp_compare] = *pFifoData;
        }
      }
      else
      {
        // handle the error
      }
    }
    // end of processing - command finished
  @endcode
 */

mmc_transfer_status_t MMC_single_block_read_nb(mmc_instance_t *this_mmc,
                                               uint32_t src_addr,
                                               uint32_t current_timeout_ticks,
                                               uint32_t timeout_ticks);

/**
  `MMC_multi_block_read()` reads multiple contiguous blocks of data from the
 eMMC
  device to the MMC host.
  The function configures the CoreMMC Read FIFO to receive multiple blocks of
 data,
  and instructs the eMMC device to transmit the data pointed to by the src_addr
  parameter.
  The size of the individual blocks of data transferred by this function is
  set to 512 Bytes, which is the standard sector size for all eMMC devices
  with a capacity of greater than 2 GB.
  To check the status of a read operation, call `MMC_status()`.
  If the read operation is successful, the received data is stored in the Read
  FIFO.
  You must call `MMC_get_fifo_read_address()` to get the address of the Read
 FIFO
  data register and copy the data from the Read FIFO to a buffer after the read
  operation has completed.

  Note 1: This driver does not support devices with a capacity of less than 2
 GB.

  Note 2: `MMC_init_fifo()` must be called to clear FIFO before calling this
 function.

  Note 3: This function is a non-blocking function, it returns immediately
 after
  initiating the read transfer.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param no_of_blks
  The `no_of_blks` parameter specifies the number of contiguous blocks to
 transfer.

  Note: An error is returned if `src_addr` + `no_of_blocks` > end address of
 eMMC
  device.

  @param src_addr
  The `src_addr` parameter specifies the sector address in the MMC device where
  the 512-byte block of data is read from.

  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
  512-byte sector.

  @param current_timeout_ticks
  The `current_timeout_ticks` parameter specifies the current tick in
 milliseconds

  @param timeout_ticks
  This `timeout_ticks` parameter specifies the timeout abort value, entered in
  system ticks.

  @return
  This function returns a value of the `mmc_transfer_status_t` type which
 specifies
  the transfer status of the operation.

  @example
  This example shows how to initialize the device and perform a multiple block
  read operation.

  @code
    MMC_init_fifo( &g_emmc_1);
    ret_var = MMC_multi_block_read( &g_emmc_1, no_blocks, sect_addr,
                         GetTickCount(), TEN_SEC_TIMEOUT);
    if(MMC_CMD_PROCESSING == ret_var)
    {
      while(MMC_CMD_PROCESSING == MMC_status(&g_emmc_1,GetTickCount()))
      {
         ;
      }
      if(MMC_TRANSFER_SUCCESS == MMC_status(&g_emmc_1,GetTickCount()))
      {
        // empty the FIFO
        pFifoData = (uint8_t *)MMC_get_fifo_read_address(&g_emmc_1);
        while (index < size_of_transfer)
        {
          p_rx_buff[index] = *pFifoData;
        }
      }
      else
      {
        // handle the error
      }
    }
    // end of processing - command finished
  @endcode
 */
mmc_transfer_status_t MMC_multi_block_read(mmc_instance_t *this_mmc,
                                           uint16_t no_of_blks,
                                           uint32_t src_addr,
                                           uint32_t current_timeout_ticks,
                                           uint32_t timeout_ticks);

/**
  `MMC_isr()` controls the eMMC data transfer operations within the
  driver for the non-blocking read and write functions.
  It must be called from the HAL interrupt handler associated with the
  interrupt line connected to the CoreMMC instance in the Libero hardware
  design. This interrupt handler must be instantiated in the application
  program.

  @param this_mmc
  The `this_mmc` parameter is a pointer to the `mmc_instance_t` data structure
  holding all data related to a specific CoreMMC hardware instance.

  @return
  This function does not return a value.

  @example
  This example shows `MMC_isr()` function being called from HAL Interrupt
  handler, which is in user application.

  @code
    void FabricIrq0_IRQHandler(void)  // Interrupt from Libero design.
    {
      MMC_isr(&g_emmc_1);
    }
  @endcode
 */

void MMC_isr(mmc_instance_t *this_mmc);

#ifdef __cplusplus
}
#endif

#endif /* __CORE_MMC_H */
