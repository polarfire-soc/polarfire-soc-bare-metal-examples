/**
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core_mmc.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief CoreMMC Source
 *
 */

#include "core_mmc.h"
#include "core_mmc_cmd_defs.h"
#include "core_mmc_if.h"
#include "core_mmc_regs.h"

#ifndef LEGACY_DIR_STRUCTURE
#include "hal/hal.h"
#include "hal/hal_assert.h"

#else
#include "hal.h"
#include "hal_assert.h"
#endif

/*--------------------------Private functions---------------------------------*/

static cif_response_t set_mmc_clk_div(const mmc_instance_t *this_mmc);
static cif_response_t set_mmc_dwidth(const mmc_instance_t *this_mmc);
static mmc_transfer_status_t get_hardware_config(mmc_instance_t *this_mmc);
static void get_card_sector_size(mmc_instance_t *this_mmc);
static mmc_transfer_status_t oper_cond_config(const mmc_instance_t *this_mmc);
static mmc_transfer_status_t data_transfer_config(mmc_instance_t *this_mmc);
static mmc_transfer_status_t dev_addr_config(const mmc_instance_t *this_mmc);

/**
  Macros for commands
 */
#define NO_ERROR 0u
#define SET_BIT 1u
#define DONE 1u
#define BLK_SIZE 512u
#define SECTOR_SIZE_512 512u
#define BUFF_EMPTY 0u
#define RCA_VALUE 0x0001FFFFu
#define STUFF_BITS 0x0000FFFFu
#define OP_COND 0x40000080u
#define ISR_READ_ERR 0xF2u
#define BISR_READ_ERR 0x3Cu
#define BUSY_BIT_MASK 0x80u
#define SECT_SIZE_LWR_CSD_MASK 0x03u
#define SECT_SIZE_UPR_CSD_MASK 0x0Cu
#define FIFO_EMPTY 1u
#define NO_BLOCK_TRAN_ERR 0x24u
#define SECTOR_ACCESS_MODE_MASK 0x60u
#define SECTOR_MODE 0x02u
#define NULL_PARAMS 0xFEu
#define HCLK_DIV_2 0u
#define HCLK_DIV_128 63u
#define SECTOR_SHIFT 5u
#define RESET_ARG 0x00000000u
#define MMC_DW_CSD 0x03B70000u
#define MMC_HS_MODE 0x03B90100u

/*--------------------------CONFIG APIs---------------------------------------*/

/**
 * MMC_param_config()
 * See ".h" for details of how to use this function.
 */
void MMC_param_config(mmc_params_t *this_mmc_params) {
  this_mmc_params->clk_rate_div = HCLK_DIV_2;
  this_mmc_params->init_clk_rate_div = HCLK_DIV_128;
  this_mmc_params->valid_param_config = NULL_PARAMS;
}

/**
 * MMC_init()
 * See ".h" for details of how to use this function.
 */
mmc_transfer_status_t MMC_init(mmc_instance_t *this_mmc, addr_t base_address,
                               const mmc_params_t *this_mmc_params) {
  mmc_transfer_status_t ret_status;
  cif_response_t response_status;
  uint8_t response_reg;
  uint32_t csd_reg[512 / 4];
  uint8_t *pCsd_reg;
  volatile uint32_t delay;

  HAL_ASSERT(NULL_PARAMS == this_mmc_params->valid_param_config);

  this_mmc->clk_rate_div = this_mmc_params->clk_rate_div;
  this_mmc->init_clk_rate_div = this_mmc_params->init_clk_rate_div;
  this_mmc->base_addr = (addr_t)base_address;
  this_mmc->data_timeout = this_mmc_params->data_timeout;

  /* Configure data timeout */
  HAL_set_32bit_reg(this_mmc->base_addr, DATATO, this_mmc->data_timeout);
  HAL_set_32bit_reg(this_mmc->base_addr, DATATO, 0x7fffffff);

  /* Configure the pre-scaler for initialization clock */
  HAL_set_8bit_reg(this_mmc->base_addr, CLKR, this_mmc->init_clk_rate_div);

  delay = 0x111BAU;
  while (delay) {
    delay--;
  }
  ret_status =
      get_hardware_config(this_mmc); /* If issue with HW config, stop now */

  if (MMC_TRANSFER_SUCCESS == ret_status) {
    /*
     * Reset the device
     * 4- Send CMD0 to reset the bus, keep CS line high during this step.
     * Wait for 1ms, then wait for 74 more clock cycles between
     * setting clock to 400KHz and sending CMD0
     */
    response_status =
        cif_send_cmd(this_mmc->base_addr, RESET_ARG, MMC_CMD_0_GO_IDLE_STATE);
    delay = 0xFFFFU;
    while (delay) {
      delay--;
    }

    if (TRANSFER_IF_SUCCESS == response_status) {
      /* Specify the operating conditions to the device */
      ret_status = oper_cond_config(this_mmc);
      if (MMC_TRANSFER_SUCCESS == ret_status) {
        delay = 0xFFFFU;
        while (delay--) {
          ;
        }
        response_reg = HAL_get_8bit_reg(this_mmc->base_addr, RR1);
        this_mmc->access_mode =
            ((SECTOR_ACCESS_MODE_MASK & response_reg) >> SECTOR_SHIFT);
        HAL_ASSERT(SECTOR_MODE == this_mmc->access_mode);
        /* Assign an RCA to the device */
        ret_status = dev_addr_config(this_mmc);
        if (MMC_TRANSFER_SUCCESS == ret_status) {
          /* Configure the data transfer */
          ret_status = data_transfer_config(this_mmc);
          /* Fill in some items from the extended  SCSD register */
          if (MMC_INIT_SUCCESS == ret_status) {
            ret_status =
                MMC_single_block_read(this_mmc, READ_SEND_EXT_CSD, csd_reg);
            if (MMC_TRANSFER_SUCCESS == ret_status) {
              pCsd_reg = (uint8_t *)csd_reg;
              this_mmc->hw_sec_count =
                  csd_reg[212 /
                          4]; /* Offsets defined in JESD84-A441 extended CSD */
              this_mmc->hw_ext_csd_rev = (uint8_t)(csd_reg[192 / 4] & 0xFFU);
              this_mmc->hw_card_type = (uint8_t)(csd_reg[196 / 4] & 0xFFU);
              this_mmc->hw_hs_timing = pCsd_reg[185];
              if (!this_mmc->hw_hs_timing) {
                ret_status = MMC_CLK_DIV_ERR;
              } else {
                ret_status = MMC_INIT_SUCCESS;
              }
            }
          }
        } else {
          ret_status = MMC_RCA_ERROR;
        }
      } else {
        ret_status = MMC_OP_COND_ERR;
      }
    } else {
      ret_status = MMC_RESET_ERR;
    }
  } /* end of if(MMC_TRANSFER_SUCCESS ==  response_status) */

  /* Clear interrupts in the BISR & ISR*/
  HAL_set_32bit_reg(this_mmc->base_addr, ICR, CLEAR_ALL_32);

  return (ret_status);
}
/*--------------------------UTILITY APIs--------------------------------------*/

/**
 * Returns write address of FIFO
 * Used in instead of
 * HAL_set_32bit_reg(this_mmc->base_addr, WDR, value);
 * as faster.
 * See ".h" for details of how to use this function.
 */
addr_t MMC_get_fifo_write_address(mmc_instance_t *this_mmc) {
  return ((addr_t)(this_mmc->base_addr + FIELD_OFFSET(WDR_REG)));
}

/**
 * Returns address of read FIFO register (RDR)
 * Used in instead of
 * HAL_get_32bit_reg(this_mmc->base_addr, RDR);
 * as faster.
 * See ".h" for details of how to use this function.
 */
addr_t MMC_get_fifo_read_address(mmc_instance_t *this_mmc) {
  return ((addr_t)(this_mmc->base_addr + FIELD_OFFSET(RDR_REG)));
}

/**
 * Returns status of current operation
 */
mmc_transfer_status_t MMC_status(mmc_instance_t *this_mmc,
                                 uint32_t current_timeout_ticks) {
  /* Check if timeout passed */
  if ((current_timeout_ticks - this_mmc->status.start_systick) >
      (this_mmc->status.timeout_value_ticks)) {
    /* Disable interrupts */
    HAL_set_32bit_reg(this_mmc->base_addr, IMR, CLEAR_ALL_32);
    /* And clear them- perhaps just clear on start sequence */
    HAL_set_32bit_reg(this_mmc->base_addr, ICR, CLEAR_ALL_32);
    /* Let application know we have timed out */
    this_mmc->status.int_state = MMC_ISR_ERROR;
    this_mmc->status.err_state = MMC_APP_TIMEOUT_FAIL;
  }

  return (this_mmc->status.err_state);
}

/**
 * Initializes FIFO on CoreMMC
 */
void MMC_init_fifo(const mmc_instance_t *this_mmc) {
  HAL_set_8bit_reg_field(this_mmc->base_addr, CTRL_RFIFO, SET_BIT);
}

/*--------------------------WRITE APIs----------------------------------------*/

/**
 * MMC_single_block_write()
 * See ".h" for details of how to use this function.
 */
mmc_transfer_status_t MMC_single_block_write(const mmc_instance_t *this_mmc,
                                             const uint32_t *src_addr,
                                             uint32_t dst_addr) {
  cif_response_t response_status;
  mmc_transfer_status_t ret_status;
  uint16_t word_cnt = BUFF_EMPTY;
  uint8_t blk_tran_err;
  uint8_t blk_write;

  /*
   * Ensure that the write FIFO is empty and load the block of data into the
   * FIFO, one byte at a time through the Write Data Register.
   */
  MMC_init_fifo(this_mmc);

  do {
    response_status =
        cif_send_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS);
  } while (DEVICE_BUSY == response_status);

  /*
   * Configure the eMMC device to receive a single block, set the block
   * Write Start Bit in hardware to transmit a block of data from the FIFO
   * to the MMC device and wait until the BDONE signal is set.
   */
  response_status = cif_send_cmd(this_mmc->base_addr, dst_addr,
                                 MMC_CMD_24_WRITE_SINGLE_BLOCK);
  HAL_set_8bit_reg_field(this_mmc->base_addr, BCSR_BWSTRT, SET_BIT);

  while ((BLK_SIZE / 4) > word_cnt) {
    HAL_set_32bit_reg(this_mmc->base_addr, WDR, src_addr[word_cnt]);
    ++word_cnt;
  }

  /* Check is block write complete */
  do {
    blk_write = HAL_get_8bit_reg_field(this_mmc->base_addr, BISR_SBWDONE);
  } while (DONE != blk_write);
  blk_tran_err =
      (NO_BLOCK_TRAN_ERR & HAL_get_8bit_reg(this_mmc->base_addr, BISR));
  if (NO_ERROR == blk_tran_err) {
    /*
     * Ensure no error fields in Card Status register are set and that
     * the device is idle before this function returns.
     */
    do {
      response_status =
          cif_send_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS);
    } while (DEVICE_BUSY == response_status);

    if (TRANSFER_IF_SUCCESS == response_status) {
      ret_status = MMC_TRANSFER_SUCCESS;
    } else {
      ret_status = MMC_TRANSFER_FAIL;
    }
  } else {
    ret_status = MMC_CRC_ERR;
  }

  /* Clear interrupts in the BISR & ISR*/
  HAL_set_32bit_reg(this_mmc->base_addr, ICR, CLEAR_ALL_32);

  return (ret_status);
}

/**
 * MMC_single_block_write_nb()
 * See ".h" for details of how to use this function.
 * Before entering this function, MMC_init_fifo must be called, and mechanism
 * set-up to fill FIFO.
 */
mmc_transfer_status_t MMC_single_block_write_nb(mmc_instance_t *this_mmc,
                                                uint32_t dst_addr,
                                                uint32_t current_timeout_ticks,
                                                uint32_t timeout_ticks) {
  mmc_transfer_status_t ret_status;

  /* Parse input parameters */
  HAL_ASSERT(0x0U != this_mmc);
  HAL_ASSERT(0x0U != (uint8_t)timeout_ticks);

  /* Initialize interrupt state machine */
  this_mmc->status.int_state =
      MMC_ISR_SW_PARSE_CMD13; /* Set the current CoreMMC interrupt state machine
                                 state */
  this_mmc->status.no_of_blks =
      1U; /* Number of blocks to either send or received in current command */
  this_mmc->status.blk_addr = dst_addr; /* Block to start read/write from */
  this_mmc->status.start_systick =
      current_timeout_ticks; /* Set at start- to enable timeout */
  this_mmc->status.timeout_value_ticks = timeout_ticks; /* In systicks */
  this_mmc->status.err_state =
      MMC_CMD_PROCESSING; /* Error report from interrupt */

  /* Enable required interrupts in CoreMMC */
  HAL_set_8bit_reg(this_mmc->base_addr, IMR, IMR_MASK_MULTI_WRITE);

  /* Send Command to the MMC device */
  send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
               CHECK_IF_CMD_SENT_NO);

  /* Interrupt handler has now taken over */
  /* use mmc_status() to check progress */
  ret_status = MMC_CMD_PROCESSING;

  return (ret_status);
}

/**
 * MMC_multi_block_write()
 * See ".h" for details of how to use this function.
 * Before entering this function, MMC_init_fifo must be called, and mechanism
 * set-up to fill FIFO.
 */
mmc_transfer_status_t MMC_multi_block_write(mmc_instance_t *this_mmc,
                                            uint16_t no_of_blks,
                                            uint32_t dst_addr,
                                            uint32_t current_timeout_ticks,
                                            uint32_t timeout_ticks) {
  mmc_transfer_status_t ret_status;

  /* Parse input parameters */
  HAL_ASSERT(0U != this_mmc);
  HAL_ASSERT(0U != no_of_blks);
  HAL_ASSERT(0u != timeout_ticks);

  /* Initialize interrupt state machine */
  this_mmc->status.int_state =
      MMC_ISR_MW_PARSE_CMD13; /* Set the current CoreMMC interrupt state machine
                                 state */
  this_mmc->status.no_of_blks = no_of_blks; /* Number of blocks to either send
                                               or received in current command */
  this_mmc->status.blk_addr = dst_addr;     /* Block to start read/write from */
  this_mmc->status.start_systick =
      current_timeout_ticks; /* Set at start- to enable timeout */
  this_mmc->status.timeout_value_ticks = timeout_ticks; /* In systicks */
  this_mmc->status.err_state =
      MMC_CMD_PROCESSING; /* Error report from interrupt */

  /* Enable required interrupts in CoreMMC */
  HAL_set_8bit_reg(this_mmc->base_addr, IMR, IMR_MASK_MULTI_WRITE);

  /* Send Command to the MMC device */
  send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
               CHECK_IF_CMD_SENT_NO);

  /* Interrupt handler has now taken over */
  /* use mmc_status() to check progress */
  ret_status = MMC_CMD_PROCESSING;

  return (ret_status);
}

/*---------------------------READ APIs----------------------------------------*/

/**
 * MMC_single_block_read()
 * See ".h" for details of how to use this function.
 */
mmc_transfer_status_t MMC_single_block_read(const mmc_instance_t *this_mmc,
                                            uint32_t src_addr,
                                            uint32_t *dst_addr) {
  uint32_t isr_errors;
  uint8_t blk_read;
  uint16_t word_cnt = (BLK_SIZE / 4);
  uint32_t idx_cnt = 0u;
  cif_response_t response_status;
  mmc_transfer_status_t ret_status;

  /*
   * Ensure that the Read FIFO is empty.
   */
  MMC_init_fifo(this_mmc);

  do {
    response_status =
        cif_send_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS);
  } while (DEVICE_BUSY == response_status);

  /*
   * Configure the hardware FIFO to receive a single block, instruct the eMMC
   * device to send a block of data and wait until BRDONE is set.
   */
  HAL_set_8bit_reg_field(this_mmc->base_addr, BCSR_BRSTRT, SET_BIT);
  if (READ_SEND_EXT_CSD == src_addr) {
    response_status =
        cif_send_cmd(this_mmc->base_addr, 0U, MMC_CMD_8_SEND_EXT_CSD);
  } else {
    response_status = cif_send_cmd(this_mmc->base_addr, src_addr,
                                   MMC_CMD_17_READ_SINGLE_BLOCK);
  }
  /* Check is block read complete */
  do {
    blk_read = HAL_get_8bit_reg_field(this_mmc->base_addr, BISR_SBRDONE);
  } while ((DONE != blk_read));

  /* Read in the contents of the FIFO */
  do {
    dst_addr[idx_cnt] = HAL_get_32bit_reg(this_mmc->base_addr, RDR);
    ++idx_cnt;
    --word_cnt;
  } while (word_cnt > (BUFF_EMPTY));

  isr_errors = HAL_get_32bit_reg(this_mmc->base_addr, ISR);

  if (!(ISR_BISR_MBSIR_ERR & isr_errors)) /* Abort if any errors*/
  {
    uint8_t read_err_isr;

    read_err_isr = ISR_READ_ERR & HAL_get_8bit_reg(this_mmc->base_addr, ISR);
    if (NO_ERROR == read_err_isr) {
      /*
       * Ensure no error fields in Card Status register are set and that
       * the device is idle before this function returns.
       */
      do {
        response_status = cif_send_cmd(this_mmc->base_addr, RCA_VALUE,
                                       MMC_CMD_13_SEND_STATUS);
      } while (DEVICE_BUSY == response_status);

      if (TRANSFER_IF_SUCCESS == response_status) {
        ret_status = MMC_TRANSFER_SUCCESS;
      } else {
        ret_status = MMC_TRANSFER_FAIL;
      }
    } else {
      ret_status = MMC_READ_CTRL_ERR;
    }
  } else {
    ret_status = MMC_CRC_ERR;
  }

  /* Clear interrupts in the BISR & ISR*/
  HAL_set_32bit_reg(this_mmc->base_addr, ICR, CLEAR_ALL_32);

  return (ret_status);
}

/**
 * MMC_single_block_read_nb()
 * See ".h" for details of how to use this function.
 */
mmc_transfer_status_t MMC_single_block_read_nb(mmc_instance_t *this_mmc,
                                               uint32_t src_addr,
                                               uint32_t current_timeout_ticks,
                                               uint32_t timeout_ticks) {
  mmc_transfer_status_t ret_status;

  /* Parse input parameters */
  HAL_ASSERT(0U != this_mmc);
  HAL_ASSERT(0U != timeout_ticks);

  if (READ_SEND_EXT_CSD == src_addr) {
    /* Set the current CoreMMC interrupt state machine state */
    this_mmc->status.int_state = MMC_ISR_EXT_CSD_PARSE_CMD13;
  } else {
    /* Set the current CoreMMC interrupt state machine state */
    this_mmc->status.int_state = MMC_ISR_SR_PARSE_CMD13;
  }

  /* Initialize interrupt state machine */
  this_mmc->status.no_of_blks =
      0x1U; /* Number of blocks to either send or received in current command */
  this_mmc->status.blk_addr = src_addr; /* Block to start read/write from */
  this_mmc->status.start_systick =
      current_timeout_ticks; /* Set at start- to enable timeout */
  this_mmc->status.timeout_value_ticks = timeout_ticks; /* In systicks */
  this_mmc->status.err_state =
      MMC_CMD_PROCESSING; /* Error report from interrupt */

  /* Enable required interrupts in CoreMMC- for sending commands / error
   * checking */
  HAL_set_8bit_reg(this_mmc->base_addr, IMR, IMR_MASK_MULTI_READ);

  /* Set number of blocks to send in the coreMMC */
  HAL_set_16bit_reg(this_mmc->base_addr, MBCOUNT, this_mmc->status.no_of_blks);

  /* Send Command to the MMC device */
  send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
               CHECK_IF_CMD_SENT_NO);

  /* Interrupt handler has now taken over */
  /* use mmc_status() to check progress */
  ret_status = MMC_CMD_PROCESSING;

  return (ret_status);
}

/**
 * MMC_multi_block_read()
 * See ".h" for details of how to use this function.
 */
mmc_transfer_status_t MMC_multi_block_read(mmc_instance_t *this_mmc,
                                           uint16_t no_of_blks,
                                           uint32_t src_addr,
                                           uint32_t current_timeout_ticks,
                                           uint32_t timeout_ticks) {
  mmc_transfer_status_t ret_status;

  /* Parse input parameters */
  HAL_ASSERT(0U != this_mmc);
  HAL_ASSERT(0U != no_of_blks);
  HAL_ASSERT(0U != timeout_ticks);

  /* Set the current CoreMMC interrupt state machine state */
  this_mmc->status.int_state = MMC_ISR_MR_PARSE_CMD13;

  /* Initialize interrupt state machine */
  this_mmc->status.no_of_blks = no_of_blks; /* Number of blocks to either send
                                               or received in current command */
  this_mmc->status.blk_addr = src_addr;     /* Block to start read/write from */
  this_mmc->status.start_systick =
      current_timeout_ticks; /* Set at start- to enable timeout */
  this_mmc->status.timeout_value_ticks = timeout_ticks; /* In systicks */
  this_mmc->status.err_state =
      MMC_CMD_PROCESSING; /* Error report from interrupt */

  /* Enable required interrupts in CoreMMC- for sending commands / error
   * checking */
  HAL_set_8bit_reg(this_mmc->base_addr, IMR, IMR_MASK_MULTI_READ);

  /* Set number of blocks to send in the coreMMC */
  HAL_set_16bit_reg(this_mmc->base_addr, MBCOUNT, this_mmc->status.no_of_blks);

  /* Send Command to the MMC device */
  send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
               CHECK_IF_CMD_SENT_NO);

  /* Interrupt handler has now taken over */
  /* use mmc_status() to check progress */
  ret_status = MMC_CMD_PROCESSING;

  return (ret_status);
}

/*--------------------------Private Functions---------------------------------*/

/**
 * Set MMC data width function
 *
 * The set_mmc_dwidth() function configures the number of data lines used in the
 * interface between the eMMC device and this instance of the CoreMMC hardware.
 * This function sets the number of data lines used in accordance to number of
 * data lines connected, specified in the version register, and then informs the
 * eMMC device of the number of lines configured, before waiting on the device
 * to return to idle, indicating that the MMC data width was modified
 * successfully. This function can only be called when the device is in Transfer
 * State.
 */
static cif_response_t set_mmc_dwidth(const mmc_instance_t *this_mmc) {
  cif_response_t response_status;

  HAL_set_8bit_reg(this_mmc->base_addr, DCTRL, this_mmc->hw_mmc_dwidth);

  /* Set MMC data bus width */
  response_status = cif_send_cmd(
      this_mmc->base_addr,
      MMC_DW_CSD | ((uint32_t)this_mmc->hw_mmc_dwidth << 8u), MMC_CMD_6_SWITCH);
  /*
   * Writing to the EXT CSD register takes significant time, so function must
   * not return until the 'READY FOR DATA' bit is set in the Card Status
   * Register.
   */
  while (DEVICE_BUSY == response_status) {
    response_status =
        cif_send_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS);
  }

  return (response_status);
}

/**
 * Get hardware configuration function
 *
 * The get_hardware_config() function reads in the contents of the version
 * register in hardware, parsers it and stores the resultant configuration data
 * in the relative fields of the mmc_instance_t structure corresponding to this
 * instance of the core.
 */
static mmc_transfer_status_t get_hardware_config(mmc_instance_t *this_mmc) {
  mmc_transfer_status_t ret_status = MMC_TRANSFER_SUCCESS;

  this_mmc->hw_fifo_depth =
      HAL_get_8bit_reg_field(this_mmc->base_addr, VR_MMCFD);
  this_mmc->hw_mmc_dwidth =
      HAL_get_8bit_reg_field(this_mmc->base_addr, VR_MMCDW);
  this_mmc->hw_maj_revision =
      HAL_get_8bit_reg_field(this_mmc->base_addr, VR_REV);

  /* If revision is equal to 3, indicates Major revision now stored in
   * VR_MAJ_REG and minor in VR_MIN_REG */
  if (this_mmc->hw_maj_revision == 0x03U) {
    this_mmc->hw_maj_revision = HAL_get_8bit_reg(this_mmc->base_addr, VR_MAJ);
    this_mmc->hw_min_revision = HAL_get_8bit_reg(this_mmc->base_addr, VR_MIN);
  } else {
    this_mmc->hw_min_revision = 0U;
    ret_status = MMC_UNSUPPORTED_HW_REVISION;
  }

  return ret_status;
}

/**
 * MMC Read Device Sector Size Function
 *
 * The get_card_sector_size() function is used to determine the sector size
 * supported by the eMMC device. This function reads in the sector size from the
 * CSD Register, performs base conversion to convert the value to decimal base
 * and stores the result in the mmc_instance_t structure corresponding to this
 * instance of the Core. This function can only be called when the device is in
 * Standby State.
 */
static void get_card_sector_size(mmc_instance_t *this_mmc) {
  uint32_t csd_max_sector_lwr;
  uint32_t csd_max_sector_upr;
  uint32_t max_sector_len;
  cif_response_t ret_val;

  ret_val = cif_send_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_9_SEND_CSD);

  csd_max_sector_lwr =
      SECT_SIZE_LWR_CSD_MASK &
      ((uint32_t)HAL_get_8bit_reg(this_mmc->base_addr, RR13) >> 6u);

  csd_max_sector_upr =
      SECT_SIZE_UPR_CSD_MASK &
      ((uint32_t)HAL_get_8bit_reg(this_mmc->base_addr, RR12) << 2u);

  max_sector_len = csd_max_sector_lwr | csd_max_sector_upr;
  this_mmc->sector_size = (uint16_t)((uint32_t)1u << max_sector_len);
}

/**
 * Set MMC clock divider rate function
 *
 * The set_mmc_clk_div() function sets the clock divider rate which governs the
 * data rate of the MMC interface with respect to HCLK. The function sets the
 * clock divider rate in the hardware register, configures the device for
 * high speed operation and waits for the device to become idle before
 * returning.This function can only be called when the eMMC device is in
 * Transfer State.
 */
static cif_response_t set_mmc_clk_div(const mmc_instance_t *this_mmc) {
  cif_response_t response_status;

  do {
    response_status =
        cif_send_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS);
  } while (DEVICE_BUSY == response_status);

  /* From JEDEC Standard No. 84-A441:
   * After the host verifies that the card complies with version 4.0, or
   * higher, of this standard, it has to enable the high speed mode timing in
   * the card, before changing the clock frequency to a frequency higher than
   * 20MHz.
   * After power-on, or software reset, the interface timing of the card is
   * set as specified in Table 114 on page 177. For the host to change to a
   * higher clock frequency, it has to enable the high speed interface timing.
   * The host uses the SWITCH command to write 0x01 to the HS_TIMING byte, in
   * the Modes segment of the EXT_CSD register.
   * The valid values for this register are defined in "HS_TIMING [185]" on
   * page 141. If the host tries to write an invalid value, the HS_TIMING byte
   * is not changed, the high speed interface timing is not enabled, and the
   * SWITCH_ERROR bit is set.
   */

  /* Set high speed mode bit in EXT CSD Register of device */
  if (TRANSFER_IF_SUCCESS == response_status) {
    response_status =
        cif_send_cmd(this_mmc->base_addr, MMC_HS_MODE, MMC_CMD_6_SWITCH);
  }

  if ((TRANSFER_IF_SUCCESS == response_status) ||
      (DEVICE_BUSY == response_status)) {
    do {
      response_status =
          cif_send_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS);
    } while (DEVICE_BUSY == response_status);

    /* Up the CoreMMC clock frequency */
    HAL_set_8bit_reg(this_mmc->base_addr, CLKR, this_mmc->clk_rate_div);
  }

  return (response_status);
}

/**
 * Operating conditions configuration function
 *
 * The oper_cond_config() function specifies the operating conditions of the MMC
 * host to the eMMC slave device. These conditions are re-specified until the
 * power-up status bit is set in the OCR Register, provided that a transmission
 * error does not occur during the process.
 * From JEDEC Standard No. 84-A441:
 * 5- Send CMD1, with the intended voltage range in the argument
 *    (either 0x00FF8000 or 0x00000080)
 * 6- Receive R3
 * 7- If the OCR busy bit is "0", repeat steps 5 and 6
 */
static mmc_transfer_status_t oper_cond_config(const mmc_instance_t *this_mmc) {
  mmc_transfer_status_t ret_status = MMC_TRANSFER_FAIL;
  cif_response_t response_status;
  uint8_t response_reg;
  uint8_t power_up_status;

  do {
    response_status =
        cif_send_cmd(this_mmc->base_addr, OP_COND, MMC_CMD_1_SEND_OP_COND);

    response_reg = HAL_get_8bit_reg(this_mmc->base_addr, RR1);

    power_up_status = BUSY_BIT_MASK & response_reg;

  } while ((BUSY_BIT_MASK != power_up_status) &&
           (TRANSFER_IF_FAIL != response_status));
  if (TRANSFER_IF_FAIL != response_status) {
    ret_status = MMC_TRANSFER_SUCCESS;
  }

  return (ret_status);
}

/*-----------------------------------------------------------------------------
 * Data transfer configuration function
 *
 * The data_transfer_config() function configures the MMC data width
 * and data rate used during MMC transfers and verifies that the device
 * supports 512 byte sector size.
 */
/*
 * JEDEC Standard No. 84-A441:
 * Switching to high-speed mode
 * The following steps are supported by cards implementing version 4.0 or
 * higher. Do these steps after the bus is initialized according to section
 * Annex A.8.1 on page 203.
 * 22-Send CMD7 with the cards RCA to place the card in transmit state
 * 23-Send CMD8, SEND_EXT_CSD. From the EXT_CSD the host can learn the power
 * class of the card, and choose to work with a wider data bus (See steps 26-37)
 * 24-Send CMD6, writing 0x1 to the HS_TIMING byte of the EXT_CSD. The argument
 * 0x03B9_0100 will do it.
 * 24.1-The card might enter BUSY right after R1, if so, wait until the BUSY
 * signal is de-asserted
 * 24.2-After the card comes out of BUSY it is configured for high speed
 * timing
 * 25-Change the clock frequency to the chosen frequency (any frequency
 * between 0 and 26/52MHz).
 */
static mmc_transfer_status_t data_transfer_config(mmc_instance_t *this_mmc) {
  mmc_transfer_status_t ret_status = MMC_DWIDTH_ERR;
  volatile cif_response_t response_status;

  get_card_sector_size(this_mmc);
  /* Verify that the device supports 512 byte block size */
  HAL_ASSERT(SECTOR_SIZE_512 == this_mmc->sector_size);

  response_status = cif_send_cmd(this_mmc->base_addr, RCA_VALUE,
                                 MMC_CMD_7_SELECT_DESELECT_CARD);
  if (TRANSFER_IF_SUCCESS == response_status) {
    response_status = set_mmc_dwidth(this_mmc);
    if (TRANSFER_IF_SUCCESS == response_status) {
      response_status = set_mmc_clk_div(this_mmc);
      if (TRANSFER_IF_SUCCESS == response_status) {
        ret_status = MMC_INIT_SUCCESS;
      } else {
        ret_status = MMC_CLK_DIV_ERR;
      }
    }
  }

  return (ret_status);
}

/**
 * Device address configuration function
 *
 * The dev_addr_config() function assigns a relative card address to a single
 * MMC device connected to the CoreMMC hardware. This function reads in the card
 * identification data from the CID Register and sets the relative card address
 * to 0x0001.
 */
static mmc_transfer_status_t dev_addr_config(const mmc_instance_t *this_mmc) {
  mmc_transfer_status_t ret_status;
  cif_response_t response_status;

  response_status =
      cif_send_cmd(this_mmc->base_addr, STUFF_BITS, MMC_CMD_2_ALL_SEND_CID);
  if (TRANSFER_IF_SUCCESS == response_status) {
    response_status = cif_send_cmd(this_mmc->base_addr, RCA_VALUE,
                                   MMC_CMD_3_SET_RELATIVE_ADDR);
    if (TRANSFER_IF_SUCCESS == response_status) {
      ret_status = MMC_TRANSFER_SUCCESS;
    } else {
      ret_status = MMC_RCA_ERROR;
    }
  } else {
    ret_status = MMC_CID_RESP_ERR;
  }

  return (ret_status);
}

/**
 *  ISR related to CoreMMC. See core_mmc.h for details.
 */
void MMC_isr(mmc_instance_t *this_mmc) {
  uint8_t read_err_sr;
  uint32_t isr_flags, MMC_Response;

  /* Check for errors in CoreMMC */
  isr_flags = HAL_get_32bit_reg(this_mmc->base_addr, ISR);
  HAL_set_32bit_reg(this_mmc->base_addr, ICR, CLEAR_ALL_32);

  if (ISR_BISR_MBSIR_ERR & isr_flags) {
    /* FIFO timeout error */
    if (((MBISR_MWBFIFOTOERR_MASK << 16) | (BISR_SWBFIFOTOERR_MASK << 8)) &
        isr_flags) {
      this_mmc->status.temp_err_state = MMC_TIMOUT_FAIL;
    }
    /* Error symptomatic of eMMC card in wrong state due to write error-
     * no response to non-interrupt(not CM12/13) command being sent */
    else if (((ISR_SSBI_MASK | ISR_SRRI_MASK | ISR_SCSI_MASK) & isr_flags) ==
             (ISR_SCSI_MASK | ISR_SSBI_MASK)) {
      this_mmc->status.temp_err_state = MMC_NO_RESPONSE_FAIL;
    } else {
      this_mmc->status.temp_err_state = MMC_OTHER_FAIL;
    }
    send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                 CHECK_IF_CMD_SENT_NO);
    this_mmc->status.int_state = MMC_ISR_STOP_TRANSMISSION;
    /* Most important feedback is user error- over or under filling the
     * FIFO */
    if (ISR_SUER_MASK & isr_flags) {
      this_mmc->status.temp_err_state = MMC_USER_ERROR_FAIL;
    } else {
      ;
    }
  } else {
    switch (this_mmc->status.int_state) {
    /* Multi write */
    case MMC_ISR_MW_PARSE_CMD13:
      /* Parse CMD13 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if ((READY_FOR_DATA & MMC_Response) == 0U) /* Not ready */
      {
        /* Re-send as busy */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (CURRENT_STATE_TRAN !=
                 (CURRENT_STATE12_9_MASK & MMC_Response)) /* Wrong state */
      {
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (R1_ERR_RESP_MASK & MMC_Response) /* Error */
      {
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else {
        ; /* No error */
      }
      send_mmc_cmd(this_mmc->base_addr, (uint32_t)(this_mmc->status.no_of_blks),
                   MMC_CMD_23_SET_BLOCK_COUNT, CHECK_IF_CMD_SENT_NO);
      this_mmc->status.int_state = MMC_ISR_MW_PARSE_CMD23;
      break;
    case MMC_ISR_MW_PARSE_CMD23:
      /* Parse CMD23 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if (R1_ERR_RESP_MASK &
          HAL_get_32bit_reg(this_mmc->base_addr, RR1)) /* Error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBW_FAIL;
        break;
      } else if ((READY_FOR_DATA & MMC_Response) == 0U) /* Not ready */
      {
        /* Re-send as busy */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else {
        ; /* No error */
      }
      /* Set number of blocks to send in the coreMMC */
      HAL_set_16bit_reg(this_mmc->base_addr, MBCOUNT,
                        this_mmc->status.no_of_blks);
      /* Send command 25- kick-off block count */
      send_mmc_cmd(this_mmc->base_addr, this_mmc->status.blk_addr,
                   MMC_CMD_25_WRITE_MULTI_BLOCK, CHECK_IF_CMD_SENT_NO);
      this_mmc->status.int_state = MMC_ISR_MW_PARSE_CMD25;
      break;
    case MMC_ISR_MW_PARSE_CMD25:
      /* Parse CMD25 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if (R1_ERR_RESP_MASK & MMC_Response) /* error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBW_FAIL;
        break;
      }
      /* No need to check to see if busy as CoreMMC will handle this situation
       * when sending */
      /* tell CoreMMC to kick-off write */
      HAL_set_8bit_reg(this_mmc->base_addr, MBIMR,
                       MBIMR_MASK_MULTI_WRITE); /* Set required interrupts */
      HAL_set_8bit_reg_field(this_mmc->base_addr, MBCSR_MBWSTRT,
                             SET_BIT); /* Tell MMC to kick-off write */
      this_mmc->status.int_state = MMC_ISR_MW_END;
      break;
    case MMC_ISR_MW_END:
      /* Check CoreMMC multi-write bit finished*/
      if ((MBISR_MBWDONE_MASK << 16) & isr_flags) {
        /* Success- report it */
        this_mmc->status.int_state = MMC_ISR_IDLE;
        this_mmc->status.err_state = MMC_TRANSFER_SUCCESS;
      } else /* Anything else is a fail */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBW_FAIL;
      }
      /* Clear masks */
      HAL_set_32bit_reg(this_mmc->base_addr, IMR, 0x00000000);
      break;

      /* Single write */
    case MMC_ISR_SW_PARSE_CMD13:
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if ((READY_FOR_DATA & MMC_Response) == 0U) /* Not ready */
      {
        /* Re-send as busy */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (CURRENT_STATE_TRAN !=
                 (CURRENT_STATE12_9_MASK & MMC_Response)) /* Wrong state */
      {
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (R1_ERR_RESP_MASK & MMC_Response) /* Error */
      { /* Error clear by ready, hopefully gone next time through */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else {
        ; /* No error */
      }
      /*
       * Configure the eMMC device to receive a single block, set the block
       * Write Start Bit in hardware to transmit a block of data from the FIFO
       * to the MMC device and wait until the BDONE signal is set.
       */
      send_mmc_cmd(this_mmc->base_addr, this_mmc->status.blk_addr,
                   MMC_CMD_24_WRITE_SINGLE_BLOCK, CHECK_IF_CMD_SENT_NO);
      this_mmc->status.int_state = MMC_ISR_SW_PARSE_CMD24;
      break;
    case MMC_ISR_SW_PARSE_CMD24:
      /* Parse CMD24 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if (R1_ERR_RESP_MASK & MMC_Response) /* error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_SBW_FAIL;
        break;
      }
      /* No need to check to see if busy as CoreMMC will handle this situation
       * when sending */
      /* tell CoreMMC to kick-off write */
      HAL_set_8bit_reg(this_mmc->base_addr, BIMR,
                       BIMR_MASK_SINGLE_WRITE); /* Set required interrupts */
      HAL_set_8bit_reg_field(this_mmc->base_addr, BCSR_BWSTRT,
                             SET_BIT); /* Tell MMC to kick-off write */
      this_mmc->status.int_state = MMC_ISR_SW_END;
      break;
    case MMC_ISR_SW_END:
      /* Check CoreMMC single-write bit finished*/
      if ((BIMR_SBWDONE_MASK << 8) & isr_flags) {
        /* success- report it */
        this_mmc->status.int_state = MMC_ISR_IDLE;
        this_mmc->status.err_state = MMC_TRANSFER_SUCCESS;
      } else /* Anything else is a fail */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_SBW_FAIL;
      }
      /* Clear masks */
      HAL_set_32bit_reg(this_mmc->base_addr, IMR, 0x00000000);
      break;

      /* Multi-read */
    case MMC_ISR_MR_PARSE_CMD13:
      /* Parse CMD23 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if ((READY_FOR_DATA & MMC_Response) == 0U) /* Not ready */
      {
        /* Re-send as busy */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (CURRENT_STATE_TRAN !=
                 (CURRENT_STATE12_9_MASK & MMC_Response)) /* wrong state */
      {
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (R1_ERR_RESP_MASK & MMC_Response) /* error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBR_FAIL;
        break;
      } else {
        ; /* No error */
      }
      send_mmc_cmd(this_mmc->base_addr, (uint32_t)(this_mmc->status.no_of_blks),
                   MMC_CMD_23_SET_BLOCK_COUNT, CHECK_IF_CMD_SENT_NO);
      this_mmc->status.int_state = MMC_ISR_MR_PARSE_CMD23;
      break;
    case MMC_ISR_MR_PARSE_CMD23:
      /* Parse CMD23 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if (R1_ERR_RESP_MASK & MMC_Response) /* error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBR_FAIL;
        break;
      } else if ((READY_FOR_DATA & MMC_Response) == 0U) /* not ready */
      {
        /* Re-set eMMC as busy */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else {
        ; /* Do nothing*/
      }
      /* Verify read FIF0 empty before start */
      read_err_sr = HAL_get_8bit_reg(this_mmc->base_addr, SR);
      if (!(SR_SRFE_MASK & read_err_sr)) {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBR_FAIL;
        break;
      }
      /* Set-up CoreMMC to expect read */
      HAL_set_8bit_reg(this_mmc->base_addr, MBIMR,
                       MBIMR_MASK_MULTI_READ); /* set required interrupts */
      HAL_set_8bit_reg_field(this_mmc->base_addr, MBCSR_MBRSTRT,
                             SET_BIT); /* tell MMC to expect read */
      /* Send command 18- kickoff block read */
      send_mmc_cmd(this_mmc->base_addr, this_mmc->status.blk_addr,
                   MMC_CMD_18_READ_MULTIPLE_BLOCK, CHECK_IF_CMD_SENT_NO);
      this_mmc->status.int_state = MMC_ISR_MR_PARSE_CMD18;
      break;
    case MMC_ISR_MR_PARSE_CMD18:
      /* Parse CMD18 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if (R1_ERR_RESP_MASK & MMC_Response) /* error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBR_FAIL;
        break;
      }
      /* No need to check to see if busy as CoreMMC will handle this situation
       * when sending or receiving multi blocks no need to do anything else,
       * read will be signaled via DMA, or next state if manually transferring.
       */
      this_mmc->status.int_state = MMC_ISR_MR_END;
      break;
    case MMC_ISR_MR_END:
      /* Check CoreMMC multi-read bit finished*/
      if ((MBIMR_MBRDONE_MASK << 16) & isr_flags) {
        /* Success- report it */
        this_mmc->status.int_state = MMC_ISR_IDLE;
        this_mmc->status.err_state = MMC_TRANSFER_SUCCESS;
      } else /* Anything else is a fail */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_MBR_FAIL;
      }
      /* Clear masks */
      HAL_set_32bit_reg(this_mmc->base_addr, IMR, 0x00000000);
      break;

    /* Single-read */
    case MMC_ISR_SR_PARSE_CMD13:
    case MMC_ISR_EXT_CSD_PARSE_CMD13:
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if ((READY_FOR_DATA & MMC_Response) == 0U) /* Not ready */
      {
        /* Re-send as busy */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (CURRENT_STATE_TRAN !=
                 (CURRENT_STATE12_9_MASK & MMC_Response)) /* Wrong state */
      {
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (R1_ERR_RESP_MASK & MMC_Response) /* Error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_SBR_FAIL;
        break;
      } else {
        ; /* No error */
      }
      /* Set-up CoreMMC to expect read */
      HAL_set_8bit_reg(this_mmc->base_addr, BIMR,
                       BIMR_MASK_SINGLE_READ); /* Set required interrupts */
      HAL_set_8bit_reg_field(this_mmc->base_addr, BCSR_BRSTRT,
                             SET_BIT); /* Tell MMC to expect read */
      /* Send command 17- kickoff single read */
      if (MMC_ISR_SR_PARSE_CMD13 == this_mmc->status.int_state) {
        send_mmc_cmd(this_mmc->base_addr, this_mmc->status.blk_addr,
                     MMC_CMD_17_READ_SINGLE_BLOCK, CHECK_IF_CMD_SENT_NO);
      } else {
        send_mmc_cmd(this_mmc->base_addr, 0, MMC_CMD_8_SEND_EXT_CSD,
                     CHECK_IF_CMD_SENT_NO);
      }
      this_mmc->status.int_state = MMC_ISR_SR_PARSE_CMD17;
      break;
    case MMC_ISR_SR_PARSE_CMD17:
      /* Parse CMD18 response */
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if (R1_ERR_RESP_MASK & MMC_Response) /* error */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_SBR_FAIL;
        break;
      }
      /* No need to check to see if busy as CoreMMC will handle this situation
       * when sending or receiving multi blocks no need to do anything else,
       * read will be signaled via DMA, or next state if manually transferring.
       */
      this_mmc->status.int_state = MMC_ISR_SR_END;
      break;
    case MMC_ISR_SR_END:
      if ((BIMR_SBRDONE_MASK << 8) & isr_flags) {
        /* Success- report it */
        this_mmc->status.int_state = MMC_ISR_IDLE;
        this_mmc->status.err_state = MMC_TRANSFER_SUCCESS;
      } else /* Anything else is a fail */
      {
        this_mmc->status.int_state = MMC_ISR_ERROR;
        this_mmc->status.err_state = MMC_SBR_FAIL;
      }
      /* Clear masks */
      HAL_set_32bit_reg(this_mmc->base_addr, IMR, 0x00000000);
      break;

    case MMC_ISR_ERROR: /* An easy way for driver to know if something went
                           wrong */
      /* Clear masks */
      HAL_set_32bit_reg(this_mmc->base_addr, IMR, 0x00000000U);
      this_mmc->status.int_state = MMC_ISR_ERROR;
      break;
    case MMC_ISR_IDLE: /* Should never get an interrupt here, if we do, go to
                          error state */
      /* Clear masks */
      HAL_set_32bit_reg(this_mmc->base_addr, IMR, 0x00000000U);
      this_mmc->status.int_state = MMC_ISR_ERROR;
      break;
    /* Common states */
    default:
    case MMC_ISR_STOP_TRANSMISSION:
      MMC_Response = HAL_get_32bit_reg(this_mmc->base_addr, RR1);
      if ((READY_FOR_DATA & MMC_Response) == 0U) /* Not ready */
      {
        /* Re-send as busy */
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      }

      else if (R1_ERR_RESP_MASK & MMC_Response) /* Error */
      {
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE, MMC_CMD_13_SEND_STATUS,
                     CHECK_IF_CMD_SENT_NO);
        break;
      } else if (CURRENT_STATE_TRAN !=
                 (CURRENT_STATE12_9_MASK & MMC_Response)) /* Wrong state */
      {
        send_mmc_cmd(this_mmc->base_addr, RCA_VALUE,
                     MMC_CMD_12_STOP_TRANSMISSION, CHECK_IF_CMD_SENT_NO);
        break;
      } else {
        ; /* No error */
      }

      this_mmc->status.int_state = MMC_ISR_ERROR;
      this_mmc->status.err_state = this_mmc->status.temp_err_state;
      HAL_set_32bit_reg(this_mmc->base_addr, IMR, 0x00000000);
      break;
    }
  }
}
