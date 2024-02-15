/*******************************************************************************
 * (c) Copyright 2019 Microsemi SoC Products Group. All rights reserved.
 *
 * Core MultiMedia Controller bare metal software driver public API.
 *
 * SVN $Revision$
 * SVN $Date$
 */
/*=========================================================================*//**
  @mainpage CoreMMC Bare Metal Driver.
  
  CoreMMC is an IP component that facilitates connection to an external embedded
  Multi-Media Card with configurable MMC bus width and data rates. This driver 
  provides support for eMMC devices with a capacity of greater than 2 Gigabytes 
  exclusively, with bus widths of 1-, 4- and 8-bit at MMC clock rates of up to 
  52 MHz.
  
  This driver provides a set of functions for configuring and controlling 
  CoreMMC as part of a bare metal system where no operating system is available.
  This driver can be adapted as part of an operating system, but the 
  implementation of the adaptation layer between the driver and the operating
  system's driver model is outside the scope of this User's Guide.
  
  This driver is not compatible with CoreMMC versions below v3.0.
  
  Features
    
  The CoreMMC driver provides support for the following features:
    - Initialization of the CoreMMC Hardware
    - Configuration of the MMC Data Rate
    - Automatically drives the Bus Width Connected (1-,4-,8-bit Connections)
    - Single 512 Byte Block Write
    - Single 512 Byte Block Read
    - Multiple 512-byte block writes
    - Multiple 512-byte block reads

  The CoreMMC driver is provided as C source code.
  
  Supported Hardware IP
  The CoreMMC bare metal driver can be used with the CoreMMC v3.0 or higher only.
  
  @section driver_configuration Driver Configuration
  
  Your application software should configure the CoreMMC driver through calls to
  the MMC_param_config() and MMC_init() functions for each CoreMMC instance in
  the hardware design. These functions configure a default set of parameters 
  that include a CoreMMC hardware instance base address and the MMC clock rate 
  divider for this instance.
  
  The CoreMMC instance is configured at the time of instantiation in the 
  hardware design for the FIFO depth and the MMC data width parameters.
  
  This driver supports the control of a single eMMC device connected to the MMC 
  data bus for each CoreMMC hardware instance.
  
  Note 1: When using the driver, there are clock divider settings that need 
          adjusting depending on the clock speed of the related AHB HCLK in the
          Libero design.
  Note 2: If using the non-blocking single read and write API functions or the 
          multiple block read / write API functions, the interrupt signal from 
          CoreMMC must be connected to the processor in the Libero design.
  Note 3: When using the multiple block read/write API functions, the size of 
          the multi-block read or write (number of blocks * block size) used 
          with these functions should not be greater than the FIFO size 
          configured for CoreMMC in the Libero design.

  @section theory_op Theory of Operation
  
  The CoreMMC driver functions allow 512 Byte blocks of data to be written to 
  and read from a single eMMC device connected to each instance of the CoreMMC 
  hardware. The blocks can be read/written singly or using the multi-block 
  functions. There are two variants of the single block functions. One set are 
  blocking, the other are nonblocking. The multi-block functions are
  non-blocking.
  
  Note: The eMMC device connected to the CoreMMC hardware instance must 
        support a sector size of 512 bytes. This is the default block size for 
        MMC device > 2GB.
  
  Initialization
  The initialization of the CoreMMC driver involves the following main steps:
    - Initialize the mmc_params_t data structure.
    - Specify the custom clk_rate_div and init_clk_rate_div configuration
    - Instantiate the HAL interrupt handler associated with the interrupt line 
      connected to the CoreMMC instance in the Libero hardware design. Call the
      MSS_isr() function from within that handler.      
    - Call the MMC_init() function.
  
  During the first step, the MMC_param_config() function is called, which loads
  the mmc_params_t data structure with default values.
  This configures the clk_rate_div parameter such that the MMC clock rate is
  1/2 of HCLK. This also configures the init_clk_rate_div parameter such that
  the MMC clock rate is 1/128 of HCLK on startup.

  If the default clock rate is sufficient for your clock customization 
  application, this next step can be omitted from the initialization process.

  Note: On start-up, the clock rate of the eMMC should be a maximum of 400KHz. 
        The init_clk_rate_div parameter should be adjusted to achieve this 
        using the formula shown below.

  The maximum permitted clock rate of the eMMC, which is set by clk_rate_div, 
  is 52MHz.
  The clk_rate_div parameter in the mmc_params_t data structure is modified in
  between the MMC_param_config() and MMC_init() function calls. If the
  application program modifies the value of clk_rate_div after calling the
  MMC_init() function, the modified value will have no effect.

  MMC Clock Rate = HCLK / (2 * (clk_rate_div + 1))
  
  The final step involves calling the MMC_init() function. The MMC_init() 
  function takes a pointer to a global CoreMMC instance data structure of type
  mmc_instance_t, the base address of the CoreMMC instance as defined by the
  hardware design and a pointer to a global parameter data structure of type
  mmc_params_t. The CoreMMC global instance data structure is used by the 
  driver to store the state information for each CoreMMC instance. A pointer to
  this data structure is also used as the first parameter to any of the driver
  functions, except the MMC_param_config() function, to identify which CoreMMC
  instance will be used by the called function. It is the responsibility of the
  application programmer to create and maintain these global CoreMMC instance
  and parameter data structures. 
  Note: The MMC_init() function must be called before any other driver 
        functions are called.
  
  MMC Single Block Transfer Control 
  
  Write Transfer
  To write a single block of data to the eMMC device in non blocking mode, a 
  call is made to the MMC_single_block_write_nb() function, specifying the 
  Core MMC hardware instance to which the required eMMC device is connected, 
  the base address of the 512-byte buffer holding the data to be transferred 
  and the destination sector address, where the data needs to be stored within 
  the eMMC device and the current system tick and the number of system ticks 
  before a timeout occurs. A call to MMC_status() function is required to 
  determine when the write transfer is finished.
  Alternatively, the MMC_single_block_write() function can be used to perform 
  a single block write operation in blocking mode. This function will wait 
  until the write operation is completed or an error occurs.
  
  Read Transfer
  To read a single block of data stored within the eMMC device in non blobking 
  mode, a call is made to the MMC_single_block_read_nb() function, specifying 
  the Core MMC hardware instance to which the required eMMC device is 
  connected, the sector address within the eMMC device where the requested data 
  is located and the base address of the 512 byte buffer where the data read 
  from the eMMC device will be stored. A call to MMC_status() function is 
  required to determine when the read transfer is finished.
  Alternatively, the MMC_single_block_read() function can be used to perform 
  a single block read operation in blocking mode. This function will wait until 
  the read operation is completed or an error occurs.
  
  MMC Multiple Block Transfer Control

  Multiple Block Write Transfer
  To write multiple blocks of data to the eMMC device, a call is made to the
  MMC_multi_block_write() function, specifying the CoreMMC hardware instance to
  which the required eMMC device is connected and the destination sector address
  on the eMMC device and the number of sectors to be written along with the
  current system tick and the number of system ticks before a timeout occurs.
  You should call MMC_status() function to check the status of the write 
  operation.
  The source data is assumed to be present in or in the process of being
  transferred (using DMA) to the CoreMMC FIFO. The maximum size that can be
  transferred per function call is limited by the maximum size of the CoreMMC 
  FIFO buffer that has been instantiated. Currently the maximum possible size 
  is 32 Kbytes.

  Multiple Block Read Transfer
  To read a multiple block of data stored within the eMMC device, a call is made
  to the MMC_multi_block_read() function, specifying the CoreMMC hardware
  instance to which the required eMMC device is connected, the sector address
  within the eMMC device where the requested data is located and the number of
  blocks to be read along with along with the current system tick and the number
  of system ticks before a timeout occurs. 
  You should call MMC_status() function to check the status of read operation. 
  If the read operation is successful, the received data will be stored in the 
  Read FIFO.
  The size of the read is limited to the maximum size of the CoreMMC FIFO buffer, 
  that has been instantiated. Currently maximum possible size is 32 Kbytes.

 *//*=========================================================================*/
#ifndef __CORE_MMC_H
#define __CORE_MMC_H

#include "cpu_types.h"

#ifdef __cplusplus
extern "C"
#endif

/***************************************************************************//**
  This enumeration is used to indicate the return status of the MMC data 
  transfer. A variable of this type is returned by the MMC_init(),MMC_status()
  MMC_single_block_write() , MMC_single_block_read(),
  MMC_single_block_write_nb(), MMC_single_block_read_nb(),
  MMC_multi_block_write() and MMC_multi_block_read() functions.
 */

typedef enum
{   
    /*
     * Failure during block data transfer. This response indicates that an error
     * was detected within the eMMC device's Card Status Register. 
     */
    MMC_TRANSFER_FAIL = 0u,
    
    /*
     * Successful MMC block data transfer. This response indicates that the data
     * transfer has been completed successfully and that the driver is ready to 
     * proceed with subsequent transfers.
     */
    MMC_TRANSFER_SUCCESS = 1u,
    
    /*
     * Initialization of the driver, the CoreMMC hardware and the eMMC device is
     * completed successfully.
     * Note: No other driver functions should be called until the MMC_init()
     * function has returned this response.
     */
    MMC_INIT_SUCCESS = 2u,
    
    /*
     * MMC data width configuration Error. Indicates that the MMC card is 
     * unreachable and may be in lock down state, having previously received an 
     * invalid command.
     */
    MMC_DWIDTH_ERR = 3u,
    
    /*
     * Error occurred whilst assigning a Relative Card Address to the eMMC 
     * device. This may indicate that multiple devices are connected to the same
     * CoreMMC hardware instance, which is not supported by the driver.   
     */
    MMC_RCA_ERROR = 4u,
    
    /*
     * Error occurred whilst reading the Card Identification register. This may 
     * indicate that the MMC device entered into lock down mode due to
     * incompatibility issues.
     */
    MMC_CID_RESP_ERR = 5u,
    
    /*
     * Error occurred whilst specifying the interface operating conditions to 
     * eMMC device. 
     * Note: Device now in lock down mode and may only be recovered by 
     * performing a hard reset of the system.
     */
    MMC_OP_COND_ERR = 6u,

    /*
     * Error occurred during the reset cycle of the initialization. This may 
     * indicate that the device is incorrectly wired to the CoreMMC hardware
     * instance.  
     */
    MMC_RESET_ERR = 7u,

    /*
     * CRC error detected on the MMC interface during a block write or block 
     * read data transfer.
     */
    MMC_CRC_ERR = 8u,
    
    /*
     * Indicates presence of data in the Hardware FIFO prior to a call to the 
     * MMC_single_block_read() function.
     */
    MMC_FIFO_N_EMPTY = 9u,
    
    /*
     * Indicates that a Control Error occurred during a data read transfer. This
     * may indicate that a FIFO overflow occurred or that a start/stop bit error
     * on the MMC Command Interface.
     */
    MMC_READ_CTRL_ERR = 10u,
    
    /*
     * Indicates that an error occurred whilst configuring the device to operate
     * in high-speed mode. This may indicate that the device does not support 
     * the MMC clock rate configured, or that it does not have a high speed mode
     * configuration setting in the EXT CSD register.
     */
    MMC_CLK_DIV_ERR = 11u,
    /*
     * Busy, wait a while
     */
    MMC_CMD_PROCESSING = 12u,
    /*
     * Multiblock-write fail-
     */
    MMC_MBW_FAIL = 13u,
    /*
     * Multiblock-read fail-
     */
    MMC_MBR_FAIL = 15u,
    /*
     * Single block-write fail-
     */
    MMC_SBW_FAIL = 16u,
    /*
     * Single block-read fail-
     */
    MMC_SBR_FAIL = 17u,
    /*
     * Application timeout- Passed timeout set by application
     */
    MMC_APP_TIMEOUT_FAIL = 18u,
    /*
     * Interrupt status error
     */
    MMC_INT_STATUS_FAIL = 19u,
    /*
     * Application has over or under filled the FIFO
     */
    MMC_USER_ERROR_FAIL = 20u,
    /*
     * Interrupt flags showed error
     */
    MMC_OTHER_FAIL = 21u,
    /*
     * CoreMMC timed out when sending or receiving.
     */
    MMC_TIMOUT_FAIL    = 22u,
    /*
     * Hardware revision of CoreMMC not supported by this driver
     */
    MMC_UNSUPPORTED_HW_REVISION = 23u,
    /*
     * No response to command from eMMC
     */
    MMC_NO_RESPONSE_FAIL = 24u
} mmc_transfer_status_t;

/***************************************************************************//**
  States used in CoreMMC interrupt state machine
 */
typedef enum mmc_state_
{
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


#define READ_SEND_EXT_CSD    0xFFFFFFFFU    /* number outside of normal range- used to tell read */
                                            /* block routine to read extended CSD register */

/***************************************************************************//**
  This structure contains status information relating to current Firmware driver operations
  and the particular instance of CoreMMC.
 */

typedef struct
{
    mmc_state int_state;              /* contains the current CoreMMC interrupt state machine state */
    uint16_t no_of_blks;              /* number of blocks to either send or received in current command */
    uint32_t blk_addr;                /* block to start read/write from */
    uint32_t start_systick;           /* set at start- to enable timeout */
    uint32_t timeout_value_ticks;     /* In systicks */
    mmc_transfer_status_t err_state;            /* error report from interrupt */
    mmc_transfer_status_t temp_err_state;    /* Temporary store for error report from interrupt */
} mmc_status;

/***************************************************************************//**
  This structure is used to store information on each instance of CoreMMC
  hardware instances in your system. Your application software should declare
  one instance of this structure for each instance of CoreMMC in your system.
  The MMC_init() function initializes this structure. A pointer to an
  initialized instance of this structure should be passed as the first parameter
  to the CoreMMC driver functions, to identify which CoreMMC hardware instance
  should perform the requested operation.
 */
typedef struct
{
    /* 
     * The base_addr member represents the base address in the processor's 
     * memory map of the registers for this CoreMMC instance. 
     */
    addr_t base_addr;
    
    /*
     *  The hw_sec_count is the sector count read from the eMMC.It is the max sector count
     *  supported by the particular eMMC device. The size of the device is hw_sec_count * 512
     */
    uint32_t hw_sec_count;

    /*
     *  This contains the extended CSD rev read during eMMC initialisation.
     */
    uint8_t  hw_ext_csd_rev;

    /*
     * This contains the cared type read from the extended CSD register.
     */
    uint8_t  hw_card_type;

    /*
     * The sector_size member indicates the size of the memory sectors 
     * (in Bytes) within the eMMC device. All operations must be carried out on 
     * complete sectors.
     * Note: This driver provides support for a 512 byte sector size 
     * exclusively.
     * Note: The sector_size member is maintained within the driver and must
     * not be modified by the user application.
     */
    uint16_t sector_size;
    
    /* 
     * The access_mode member indicates the memory addressing system (Byte or 
     * Sector Addressing) used within the eMMC device. This driver supports 
     * sector access exclusively.
     * Note: The access_mode member is maintained within the driver and must
     * not be modified by the user application.
     */
    uint8_t access_mode;
    
    /* 
     * The hw_fifo_depth member indicates the size of the FIFO instantiated 
     * within this instance of the CoreMMC hardware.
     * Note: The hw_fifo_depth member is maintained within the driver and must
     * not be modified by the user application.
     */
    uint8_t hw_fifo_depth;

    /*
    * This is the major version of CoreMMC. Only present in Cores >= 3.00
    */
    uint8_t hw_maj_revision;

    /*
    * This is the minor version of CoreMMC. Only present in Cores >= 3.00
    * >= 3.0
    */
    uint8_t hw_min_revision;

    /*
     * The hw_mmc_dwidth member denotes the width of the MMC interface 
     * connected between the eMMC device and this instance of the CoreMMC 
     * hardware. 
     * Note: This driver will automatically configure the device and hardware 
     * to drive the width of the bus connected. i.e 1-, 4-, 8-bit Interface.
     * Note: The hw_mmc_dwidth member is maintained within the driver and must
     * not be modified by the user application.
     */
    uint8_t hw_mmc_dwidth;
    
    /* 
     * The clk_rate_div member internally sets the clock rate of the MMC 
     * interface.
     * Note: The clk_rate_div member is maintained within the driver and must
     * not be modified by the user application.
     */
    uint8_t clk_rate_div;
    
    /*
     * The init_clk_rate_div member internally sets the initialization clock
     * rate of the MMC interface.
     * Note: The init_clk_rate_div member is maintained within the driver and
     * must not be modified by the user application.
     */
    uint8_t init_clk_rate_div;

    /*
     * Firmware_status
     */
    mmc_status status;
    /*
        * Data timeout
        * Sets the length in cycles before timeout occurs
        */
    uint32_t data_timeout;
    /*
     * contains contents read from extended CSD register HS_TIMING byte
     * Zero on initialization
     * set to one when in high speed mode
     */
    uint8_t hw_hs_timing;
} mmc_instance_t;

/***************************************************************************//**
  This structure is used to pass configuration parameters to the MMC_init() 
  function. Your application software should declare one instance of this 
  structure for each instance of CoreMMC in your system. The MMC_param_config()
  function initializes this structure. A pointer to an initialized instance of 
  this structure should be passed to the MMC_init() function, to provide the 
  custom configuration parameters to configure the driver for your specific 
  application.
 */
typedef struct
{
    /* 
     * The clk_rate_div member of this structure assigns a default MMC clock 
     * rate of HCLK/2. If this MMC clock rate is not suitable for the custom
     * application, then the clk_rate_div can be modified to be to any value in 
     * the range of 0-255, giving an MMC clock rate range of HCLK/2 to HCLK/512.
     * The relationship between the MMC clock rate, HCLK and the clk_rate_div 
     * is as follows:
     * MMC Clock = HCLK/(2(clk_rate_div + 1))
     */
    uint8_t clk_rate_div;
    
    /* 
     * MAX clock frequency on startup is 400KHz
     * The init_clk_rate_div member of this structure assigns a default MMC 
     * clock initialization rate of HCLK/128. If this MMC clock rate is not
     * suitable for the custom application, then the init_clk_rate_div can be
     * modified to be to any value in the range of 0-255, giving an MMC
     * initialization clock rate range of HCLK/2 to HCLK/512. The relationship
     * between the MMC clock rate, HCLK and the init_clk_rate_div is as follows:
     * MMC Clock = HCLK/(2(init_clk_rate_div + 1))
     */
    uint8_t init_clk_rate_div;

    /*
     * The valid_param_config parameter is maintained within the driver and must
     * not be modified by the user application.
     */
    uint8_t valid_param_config;

    /*
     * Data timeout-
     * Sets the length in cycles before timeout occurs
     */
    uint32_t data_timeout;
} mmc_params_t;

/*==============================================================================
 * Public functions
 *============================================================================*/

/***************************************************************************//**
  The MMC_param_config() function assigns default values to the members of the 
  mmc_params_t structure. This function must be called prior to calling the 
  MMC_init() function. By default, this function sets the clock divider rate to
  1/2 times that of HCLK.
  
  @param this_mmc_params
  The this_mmc_params parameter is a pointer to the mmc_params_t structure which
  holds the parameters required to initialize this instance of CoreMMC. This 
  parameter must point to the instance of this structure, defined within the
  application code.

  @return 
  This function does not return any value.
 */
void
MMC_param_config
(
    mmc_params_t * this_mmc_params
);

/***************************************************************************//**
  The MMC_init() function initializes the data structures of a CoreMMC instance
  referenced by the this_mmc parameter. The CoreMMC hardware and the eMMC device
  connected to the MMC interface of the CoreMMC hardware instance.
  This function must be called for each CoreMMC instance with a unique this_mmc
  and base_address parameter combination. The MMC_init() function must be called
  prior to calling any MMC data transfer functions, and must be preceded by
  a call to the MMC_param_config() function.

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure 
  holding all data related to the specific CoreMMC hardware instance.

  @param base_address
  The base_address parameter specifies the base address in the processor's 
  memory map for the registers of the CoreMMC instance being initialized. 

  @param this_mmc_params
  The this_mmc_params parameter is a pointer to the mmc_params_t structure 
  holding the parameters required for the initial configuration of the CoreMMC
  instance.

  @return 
  This function returns the initialization status of the MMC device of
  type mmc_transfer_status_t.

  Example:
  This example shows how to initialize the device and configure the MMC Data 
  Rate to 1/10 of HCLK.
  @code
    #define MMC0_BASE_ADDR 0x30000000u

    mmc_instance_t g_mmc0
    mmc_params_t g_mmc0_param
    mmc_transfer_status_t resp_reg
    
    MMC_param_config(&g_mmc0_param)
    
    Set the MMC clock rate to 1/10 HCLK.
    g_mmc0_param.clk_rate_div = 4u;
    resp_reg = MMC_init(&g_mmc0, MMC0_BASE_ADDR, &g_mmc0_param);
    if(MMC_INIT_SUCCESS == resp_reg)
    {
      ...
    }
  @endcode
 */
mmc_transfer_status_t MMC_init
(
    mmc_instance_t * this_mmc,
    addr_t base_address,
    const mmc_params_t * this_mmc_params
);

/***************************************************************************//**
  The MMC_get_fifo_write_address() function returns the address of the Write 
  FIFO data register of the CoreMMC. This is required when setting up DMA to 
  write to the FIFO. It can also be used to write to the FIFO using a pointer 
  in preference to the HAL register access macros, which are slower.

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure 
  holding all data related to a specific CoreMMC hardware instance. 

  @return
  This function returns the address of the CoreMMC Write FIFO.

  Example:
  This example shows how to use the MMC_get_fifo_write_address() function
  @code
    uint8_t * pFifoData;
    ...
    pFifoData = (uint8_t *)MMC_get_fifo_write_address(&g_emmc_1);
    MMC_init_fifo( &g_emmc_1);
    iCount= no_blocks * size_of_block;
    while(iCount--)
    {
        *pFifoData = storage[iCount];
    }
  @endcode
 */
addr_t MMC_get_fifo_write_address
(
    mmc_instance_t * this_mmc
);

/***************************************************************************//**
  The MMC_get_fifo_read_address() function returns the address of Read FIFO
  data register of the CoreMMC. This is required when setting up DMA to read 
  from the FIFO. It can also be used to read from FIFO using a pointer in 
  preference to the HAL register access macros, which are slower.

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure 
  holding all data related to a specific CoreMMC hardware instance.

  @return
  This function returns the address of the CoreMMC Read FIFO.

  Example:
  This example shows how to use the MMC_get_fifo_read_address() function
  @code
    uint8_t * pFifoData;
    ...
    pFifoData = (uint8_t *)MMC_get_fifo_read_address(&g_emmc_1);
    iCount= no_blocks * size_of_block;
    while(iCount--)
    {
        storage[iCount] = *pFifoData; ..fills memory from FIFO
    }
  @endcode
 */
addr_t MMC_get_fifo_read_address
(
    mmc_instance_t * this_mmc
);

/***************************************************************************//**
  The MMC_status() function returns the current state of the read/write process 
  initiated by any of the non-blocking functions. The non-blocking functions, 
  which can be used to start a read/write transfer are MMC_multi_block_write(), 
  MMC_single_block_write_nb(), MMC_single_block_read_nb(), 
  MMC_multi_block_write() and MMC_multi_block_read().

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure 
  holding all data related to a specific CoreMMC hardware instance.

  @param current_timeout_ticks
  The current_timeout_ticks parameter passes the current tick in milliseconds
  to the function to allow it determine if operation has timed out.

  @return
  This function returns the transfer status of type mmc_transfer_status_t.

  Example:
  This example shows how to use MMC_status() function with MMC_multi_block_write() 
  command
  @code
      ...
    ret_var = MMC_multi_block_write( &g_emmc_1, no_blocks, sect_addr, timeout);
    while(MMC_CMD_PROCESSING == MMC_status(&g_emmc_1))
    {
        ;    wait for MMC_multi_block_write() to finish
            Could do other stuff here
    }
    if(MMC_TRANSFER_SUCCESS != MMC_status(&g_emmc_1, ))
    {
        handle the error
    }
    else
    {
        write success, continue on
    }
  @endcode
 */
mmc_transfer_status_t MMC_status
(
    mmc_instance_t * this_mmc,
    uint32_t current_timeout_ticks
);

/***************************************************************************//**
  The MMC_init_fifo() function clears the content of both the Read and Write 
  FIFOs and returns them to their reset state. The MMC_init_fifo() function 
  should be called before transferring data into the Read FIFO or Write FIFO 
  when using the non-blocking read and write functions. The non-blocking 
  functions are MMC_single_block_write_nb(), MMC_single_block_read_nb(), 
  MMC_multi_block_write() and MMC_multi_block_read().

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure
  holding all data related to a specific CoreMMC hardware instance.

  @return
  This function does not return a value.

  Example:
  This example shows how to use MMC_init_fifo() when filling FIFO
  @code
    uint8_t * pFifoData;
    ...
    pFifoData = (uint8_t *)MMC_get_fifo_write_address(&g_emmc_1);
    MMC_init_fifo( &g_emmc_1);  makes sure FIFO is empty before starting
    iCount= no_blocks * size_of_block;
    while(iCount--)
    {
        *pFifoData = storage[iCount];
    }
  @endcode
 */
void MMC_init_fifo
(
    const mmc_instance_t * this_mmc
);

/***************************************************************************//**
  The MMC_single_block_write() function is used to transmit a single block of 
  data from the MMC host to the eMMC device. This function writes the block of 
  data into the hardware FIFO, transfer of the data from the hardware FIFO to 
  the eMMC device and waits for write operation to complete.
  The size of the block of data transferred by this function is set to 512 
  bytes, which is the standard sector size for all eMMC devices with a capacity
  of greater than 2 GB.
  Note 1: This driver does not support devices with a capacity of less than 2 GB.
  Note 2: This function is a blocking function and will not return until the 
          write operation is successful or an error occurs.
 
  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure
  holding all data related to a specific CoreMMC hardware instance.
 
  @param src_addr
  The src_addr parameter is a pointer to a buffer containing the data to be 
  stored in the eMMC device. The buffer should be a minimum size of 512 bytes. 
  Note: If the buffer is not declared with adequate size, then the eMMC sector 
        may be loaded with erroneous data.
  
  @param dst_addr
  The dst_addr parameter specifies the sector address in the MMC device where
  the block is to be stored.
  Note: For eMMC devices of greater than 2 GB in size, this address refers to a 
        512-byte sector.

  @return
  This function returns a value of type mmc_transfer_status_t which specifies the
  transfer status of the operation.
   
  Example:
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
           ...
        }
    }
  @endcode
 */
mmc_transfer_status_t MMC_single_block_write
(
    const mmc_instance_t * this_mmc,
    const uint32_t * src_addr,
    uint32_t dst_addr
);

/***************************************************************************//**
  The MMC_single_block_write_nb() function is used to transfer a single block 
  of data from the MMC host to the eMMC device. This function requires the 
  program to fill the CoreMMC Write FIFO with the correct amount of data in 
  advance. This can be achieved by doing a memory copy before calling this 
  function, or setting up a DMA transfer before calling this function. 
  The MMC_single_block_write_nb() function initiates the transfer of the data 
  from the Write FIFO to the eMMC device.
  The size of the block of data transferred by this function is set to 512 
  bytes, which is the standard sector size for all eMMC devices with a 
  capacity of greater than 2 GB.
  You should call MMC_status() function to check the status of the write 
  operation.
  Note 1: This driver does not support devices with a capacity of less than 2 GB.
  Note 2: The MMC_init_fifo() function should be used called to clear the Write 
          FIFO before writing any data to the FIFO and before calling this 
          function.
  Note 3: This function is a non-blocking function and will return immediately 
          after initiating the write transfer.

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param dst_addr
  The dst_addr parameter specifies the sector address in the MMC device where
  the block is to be stored.
  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
        512-byte sector.

  @param current_timeout_ticks
  This current_timeout_ticks parameter specifies current tick in milliseconds.

  @param timeout_ticks
  This timeout_ticks specifies the timeout abort value, in milliseconds.

  @return
  This function returns a value of type mmc_transfer_status_t which specifies the
  transfer status of the operation.

  Example:
  This example shows how to initialize the device and perform a single block transfer.
  
  @code
    MMC_init_fifo(&g_emmc_1)
    fill the FIFO - from g_buffer_a to FIFO 
    iCount= 512/4;
    while(iCount--)
    {
        *pFifoData = p_tx_buff[iCount];
    }
    ret_var = MMC_single_block_write_nb( &g_emmc_1, sect_addr,
                                         current_timeout_ticks, timeout);
    while(MMC_CMD_PROCESSING == MMC_status(&g_emmc_1,GetTickCount()))
    {
        ;
    }
    if(MMC_TRANSFER_SUCCESS != MMC_status(&g_emmc_1,GetTickCount()))
    {
        ..handle the error
    }
    else
    {
        ..success- continue
    }
  @endcode
 */
mmc_transfer_status_t MMC_single_block_write_nb
(
    mmc_instance_t * this_mmc,
    uint32_t dst_addr,
    uint32_t current_timeout_ticks,
    uint32_t timeout_ticks
);

/***************************************************************************//**
  The MMC_multi_block_write() function is used to transfer multiple blocks of
  data from the MMC host to the eMMC device. This function requires the program
  to fill the CoreMMC Write FIFO with the correct amount of data in advance.
  This can be achieved by doing a memory copy before calling this function, or 
  setting up a DMA transfer before calling this function.
  The size of the individual block of data transferred by this function is set
  to 512 bytes, which is the standard sector size for all eMMC devices with a 
  capacity of greater than 2 GB.
  You should call MMC_status() function to check the status of the write 
  operation.
  Note 1: This driver does not support devices with a capacity of less than 2 GB.
  Note 2: The MMC_init_fifo() function should be called to clear the Write FIFO 
          before writing any data to the FIFO and before calling this function.
  Note 3: This function is a non-blocking function and returns immediately after 
          initiating the transfer.

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param no_of_blks
  The no_of_blks parameter specifies the number of contiguous blocks to be transferred.

  @param dst_addr
  The dst_addr parameter specifies the sector address in the MMC device where
  the block is to be stored.
  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
        512-byte sector.

  @param current_timeout_ticks
  The current_timeout_ticks parameter specifies the current tick in milliseconds

  @param timeout_ticks
  The timeout_ticks parameter specifies the timeout abort value in milliseconds.

  @return
  This function returns a value of type mmc_transfer_status_t which specifies the
  transfer status of the operation.

  Example:
  This example shows how to initialize the device and perform a multiple block
  transfer.
  @code
      ...
    pFifoData = (uint32_t *)MMC_get_fifo_write_address(&g_emmc_1);
    MMC_init_fifo( &g_emmc_1);  -> make sure FIFO empty

    iCount= no_blocks * 512;
    while(iCount--)            -> fill the FIFO - from g_buffer_a to FIFO
    {
        *pFifoData = g_write_data_buff[iCount]; note:in this case filling in reverse
    }
    ret_var = MMC_multi_block_write( &g_emmc_1, no_blocks, sect_addr,
                                     GetTickCount(), TEN_SEC_TIMEOUT);
    while(MMC_CMD_PROCESSING == ret_var)
    {
        ret_var = MMC_status(&g_emmc_1,GetTickCount());
    }

    if(MMC_TRANSFER_SUCCESS != MMC_status(&g_emmc_1,GetTickCount()))
    {
        ... handle the error
    }
    else
    {
        ... success
    }
  @endcode
 */
mmc_transfer_status_t MMC_multi_block_write
(
    mmc_instance_t * this_mmc,
    uint16_t no_of_blks,
    uint32_t dst_addr,
    uint32_t current_timeout_ticks,
    uint32_t timeout_ticks
 );

/***************************************************************************//**
  The MMC_single_block_read function is used to read single block of data from 
  the eMMC slave to the FIFO on the CoreMMC. The function configures the CoreMMC 
  Read FIFO hardware to receive a block of data, instructs the eMMC device to 
  transmit the data stored at the sector pointed to by the src_addr parameter 
  and waits to complete the read operation. The size of the block of data 
  transferred by this function is set to 512 bytes, which is the standard sector 
  size for all eMMC devices with a capacity of greater than 2 GB. 
  Note 1: This driver does not support devices with a capacity of less than 2 GB.
  Note 2: This function is a blocking function and will not return until the 
          read operation is successful or an error occurs.

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure 
  holding all data related to a specific CoreMMC hardware instance.

  @param src_addr
  The src_addr parameter specifies the sector address in the MMC device where 
  the 512-byte block of data will be read from.

  @param dst_addr
  The dst_addr parameter is a pointer to a buffer where the data read from the 
  eMMC device will be stored. The buffer should be a minimum size of 512 bytes. 
  Note: If the buffer is not declared with adequate size, the response data 
        read-back from the eMMC device will overflow from the receive buffer 
        and its storage will be unpredictable

  @return
  This function returns a value of type mmc_transfer_status_t which specifies the
  transfer status of the operation.

  Example:
  This example shows how to initialize the device, perform a single block 
  transfer and read back the data from the sector written to within the eMMC 
  device.
  @code
    #define MMC0_BASE_ADDR 0x30000000u
    #define BLOCK_1        0x00000001u
    
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
               ...
           }
       }
    }

  @endcode
 */
mmc_transfer_status_t MMC_single_block_read
(
    const mmc_instance_t * this_mmc,
    uint32_t src_addr,
    uint32_t * dst_addr
);

/***************************************************************************//** 
  The MMC_single_block_read_nb() function is used to read a single block of 
  data from the eMMC device to the MMC host. The function configures the CoreMMC 
  Read FIFO to receive a block of data and instructs the eMMC device to transmit 
  the data stored at the sector pointed to by the src_addr parameter. The size 
  of the block of data transferred by this function is set to 512 bytes, which 
  is the standard sector size for all eMMC devices with a capacity of greater 
  than 2 GB. 
  You should call the MMC_status() function to check the status of read operation.
  If read operation is successful, the received data will be stored in the Read 
  FIFO. You should call the MMC_get_fifo_read_address() function to get the 
  address of the Read FIFO data register and copy the data from the Read FIFO 
  register to a buffer after the read operation has completed.
  Note 1: This driver does not support devices with a capacity of less than 2 GB.
  Note 2: The MMC_init_fifo() function must be used to clear the FIFO before 
          calling this function.
  Note 3: This function is a non-blocking function and will return immediately 
          after initiating the read transfer.
  
  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param src_addr
  The src_addr parameter specifies the sector address in the eMMC device where
  the 512-byte block of data will be read from.

  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
        512-byte sector.

  @param current_timeout_ticks
  The current_timeout_ticks parameter specifies current tick in milliseconds.

  @param timeout_ticks
  This timeout_ticks parameter specifies the timeout abort value in milliseconds.

  @return
  This function returns a value of type mmc_transfer_status_t which specifies the
  transfer status of the operation.

  Example:
  This example shows how to initialize the device, perform a single block
  transfer and read back the data from the sector written to within the eMMC
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
            empty the FIFO -
            pFifoData = (uint8_t *)MMC_get_fifo_read_address(&g_emmc_1);
            while( size_of_transfer--)
            {
                g_write_data_buff[temp_compare] = *pFifoData;
            }
        }
        else
        {
            .. handle the error
        }
    }  end of processing- command finished
  @endcode
 */
mmc_transfer_status_t MMC_single_block_read_nb
(
    mmc_instance_t * this_mmc,
    uint32_t src_addr,
    uint32_t current_timeout_ticks,
    uint32_t timeout_ticks
);

/***************************************************************************//**
  The MMC_multi_block_read() function is used to read multiple contiguous blocks 
  of data from the eMMC device to the MMC host. The function configures the 
  CoreMMC Read FIFO to receive multiple blocks of data and instructs the eMMC 
  device to transmit the data pointed to by the src_addr parameter. 
  The size of the individual blocks of data transferred by this function is 
  set to 512-byte, which is the standard sector size for all eMMC devices 
  with a capacity of greater than 2 GB.
  You should call MMC_status() function to check the status of read operation. 
  If the read operation is successful, the received data will be stored in the 
  Read FIFO. You should call the MMC_get_fifo_read_address() function to get 
  the address of the Read FIFO data register and copy the data from the Read 
  FIFO to a buffer after the read operation has completed.
  
  Note 1: This driver does not support devices with a capacity of less than 2 
          GB.
  Note 2: MMC_init_fifo() function must be called to clear FIFO before calling 
          this function.
  Note 3: This function is a non-blocking function and it returns immediately 
          after initiating the read transfer. 

  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure
  holding all data related to a specific CoreMMC hardware instance.

  @param no_of_blks
  This no_of_blks parameter specifies the number of contiguous blocks to be
  transferred.
  Note: An error is returned if src_addr + no_of_blocks > end address of eMMC 
        device.
  
  @param src_addr
  The src_addr parameter specifies the sector address in the MMC device where
  the 512-byte block of data will be read from.
  Note: For eMMC devices of greater than 2 GB in size, this address refers to a
        512-byte sector.

  @param current_timeout_ticks
  The current_timeout_ticks parameter specifies the current tick in milliseconds

  @param timeout_ticks
  This timeout_ticks parameter specifies the timeout abort value, entered in
  system ticks.

  @return
  This function returns a value of type mmc_transfer_status_t which specifies
  the transfer status of the operation.

  Example:
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
            empty the FIFO -
            pFifoData = (uint8_t *)MMC_get_fifo_read_address(&g_emmc_1);
            while( index < size_of_transfer)
            {
                p_rx_buff[index] = *pFifoData;
            }
        }
        else
        {
             handle the error
        }
    } end of processing- command finished
  @endcode
 */
mmc_transfer_status_t MMC_multi_block_read
(
    mmc_instance_t * this_mmc,
    uint16_t no_of_blks,
    uint32_t src_addr,
    uint32_t current_timeout_ticks,
    uint32_t timeout_ticks
);

/***************************************************************************//**
  The MMC_isr() function controls the eMMC data transfer operations within the 
  driver for the non-blocking read and write functions. It must be called from 
  the HAL interrupt handler associated with the interrupt line connected to the 
  CoreMMC instance in the Libero hardware design. This interrupt handler must 
  be instantiated in the application program.
  
  @param this_mmc
  The this_mmc parameter is a pointer to the mmc_instance_t data structure
  holding all data related to a specific CoreMMC hardware instance.

  @return
  This function does not return value.

  Example:
  This example shows MMC_isr() fucntion being called from HAL Interrupt handler,
  which is in user application.
  @code
    void FabricIrq0_IRQHandler(void)  Interrupt from Libero design.
    {
        MMC_isr(&g_emmc_1);
    }
  @endcode
 */
void MMC_isr
(
    mmc_instance_t * this_mmc
);


#ifdef __cplusplus
}
#endif

#endif  /* __CORE_MMC_H */
