/**
 * Copyright 2025 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file core1588.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Core1588 source file
 *
 * See file "core1588.h" for description of the functions implemented
 * in this file.
 *
 */

#include "core1588.h"
#include "core1588_regs.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Null parameters with appropriate type definitions
 */
#define C1588_NULL_INSTANCE ((c1588_instance_t *)0)
#define C1588_NULL_CONFIG ((c1588_cfg_t *)0)
#define C1588_NULL_TIMESTAMP ((c1588_timestamp_t *)0)
#define C1588_NULL_TIME_INCR ((c1588_time_incr_t *)0)
#define C1588_NULL_PACKET_INFO ((c1588_ptp_packet_info_t *)0)
#define C1588_NULL_RTC_EVENT_TIMESTAMP ((c1588_rtc_event_timestamp_t *)0)

/*------------------------------------------------------------------------------
 * Core1588 user interrupt control functions implemented "core1588_interrupt.c".
 * The implementation of these functions is user dependent.
 */
void core1588_ptp_rx_user_handler(c1588_instance_t *this_c1588,
                                  uint32_t rx_irq_mask);
void core1588_ptp_tx_user_handler(c1588_instance_t *this_c1588,
                                  uint32_t tx_irq_mask);
void core1588_latch_user_handler(c1588_instance_t *this_c1588,
                                 uint32_t latch_mask);
void core1588_trigger_user_handler(c1588_instance_t *this_c1588,
                                   uint32_t trigger_mask);
void core1588_rtcsec_user_handler(c1588_instance_t *this_c1588);

/*******************************************************************************
 * core1588_cfg_struct_def_init()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_cfg_struct_def_init(c1588_cfg_t *cfg) {
  HAL_ASSERT(cfg != C1588_NULL_CONFIG);

  if (cfg != C1588_NULL_CONFIG) {
    memset(cfg, 0, sizeof(c1588_cfg_t));
  }
}

/*******************************************************************************
 * core1588_init()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_init(c1588_instance_t *this_c1588, addr_t base_addr) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(base_addr != 0u);

  if (this_c1588 != C1588_NULL_INSTANCE) {
    /* Set base address of Core1588 hardware. */
    memset(this_c1588, 0, sizeof(c1588_instance_t));
    this_c1588->base_address = base_addr;
  }
}

/*******************************************************************************
 * core1588_configure()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_configure(c1588_instance_t *this_c1588, c1588_cfg_t *cfg) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_GCFG,
                    cfg->config_mask);
  core1588_rtc_set_time(this_c1588, &cfg->initial_time);

  if (cfg->rtc_freq != 0) {
    core1588_rtc_set_increment_freq(this_c1588, cfg->rtc_freq);
  } else {
    core1588_rtc_set_increment(this_c1588, &cfg->initial_rtc_incr);
  }
}

/*******************************************************************************
 * core1588_ptp_control()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_ptp_control(c1588_instance_t *this_c1588, uint32_t control) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  if (control == C1588_ENABLE) {
    HAL_set_32bit_reg_field(this_c1588->base_address, CORE1588_REGS_GCFG_ENCOR,
                            1);
  } else {
    HAL_set_32bit_reg_field(this_c1588->base_address, CORE1588_REGS_GCFG_ENCOR,
                            0);
  }
}

/*******************************************************************************
 * core1588_one_step_sync_control()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_one_step_sync_control(c1588_instance_t *this_c1588,
                                    uint32_t control) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  if (control == C1588_ENABLE) {
    HAL_set_32bit_reg_field(this_c1588->base_address,
                            CORE1588_REGS_GCFG_ONE_STEP_SYNC, 1);
  } else {
    HAL_set_32bit_reg_field(this_c1588->base_address,
                            CORE1588_REGS_GCFG_ONE_STEP_SYNC, 0);
  }
}

/*******************************************************************************
 * core1588_get_enabled_irq()
 * See "core1588.h" file for details of how to use this function.
 */
uint32_t core1588_get_enabled_irq(c1588_instance_t *this_c1588) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t irq_en;

  irq_en = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_IER);

  return irq_en;
}

/*******************************************************************************
 * core1588_irq_control()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_irq_control(c1588_instance_t *this_c1588, uint32_t irq_mask,
                          uint32_t control) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t reg_value;

  reg_value = core1588_get_enabled_irq(this_c1588);

  if (control == C1588_ENABLE) {
    reg_value |= irq_mask;
  } else {
    reg_value &= ~irq_mask;
  }

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_IER, reg_value);
}

/*******************************************************************************
 * core1588_get_irq_src()
 * See "core1588.h" file for details of how to use this function.
 */
uint32_t core1588_get_irq_src(c1588_instance_t *this_c1588) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t ret = 0;

  ret = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_MIS);

  return ret;
}

/*******************************************************************************
 * core1588_clear_irq_src()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_clear_irq_src(c1588_instance_t *this_c1588, uint32_t irq_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RIS, irq_mask);
}

/*******************************************************************************
 * core1588_isr()
 * See core1588 for details of how to use this function.
 */
void core1588_isr(c1588_instance_t *this_c1588) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t irq;
  uint32_t irq_masked;

  irq = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_MIS);

  irq_masked = irq & C1588_RX_IRQ_MASK;
  if (irq_masked != 0u) {
    core1588_ptp_rx_default_handler(this_c1588, irq_masked);
    if (C1588_RX_IRQ_USER_HANDLER) {
      core1588_ptp_rx_user_handler(this_c1588, irq_masked);
    }
  }

  irq_masked = irq & C1588_TX_IRQ_MASK;
  if (irq_masked != 0u) {
    core1588_ptp_tx_default_handler(this_c1588, irq_masked);
    if (C1588_TX_IRQ_USER_HANDLER) {
      core1588_ptp_tx_user_handler(this_c1588, irq_masked);
    }
  }

  irq_masked = irq & C1588_LATCH_MASK_ALL;
  if (irq_masked != 0u) {
    core1588_latch_default_handler(this_c1588, irq_masked);
    if (C1588_LATCH_IRQ_USER_HANDLER) {
      core1588_latch_user_handler(this_c1588, irq_masked);
    }
  }

  irq_masked = irq & C1588_TRIGGER_MASK_ALL;
  if (irq_masked != 0u) {
    core1588_trigger_default_handler(this_c1588, irq_masked);
    if (C1588_TRIGGER_IRQ_USER_HANDLER) {
      core1588_trigger_user_handler(this_c1588, irq_masked);
    }
  }

  if (irq & C1588_RTCSEC_IRQ) {
    core1588_clear_irq_src(this_c1588, C1588_RTCSEC_IRQ);
    if (C1588_RTCSEC_IRQ_USER_HANDLER) {
      core1588_rtcsec_user_handler(this_c1588);
    }
  }
}

/*******************************************************************************
 * core1588_rtc_set_time()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_rtc_set_time(c1588_instance_t *this_c1588,
                                     c1588_timestamp_t *ts) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(ts != C1588_NULL_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTCL, ts->nsecs);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTCM,
                    (uint32_t)ts->secs);

  HAL_set_32bit_reg_field(this_c1588->base_address,
                          CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS,
                          ts->secs >> 32u);
  return ret;
}

/*******************************************************************************
 * core1588_rtc_get_time()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_rtc_get_time(c1588_instance_t *this_c1588,
                                     c1588_timestamp_t *ts) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(ts != C1588_NULL_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  ts->nsecs = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTCL);

  ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                   this_c1588->base_address,
                   CORE1588_REGS_RTCMSBSEC_RTC_SEC_MSB_16BITS)
               << 32) |
              (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                          CORE1588_REGS_RTCM));
  return ret;
}

/*******************************************************************************
 * core1588_rtc_set_increment()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_rtc_set_increment(c1588_instance_t *this_c1588,
                                          c1588_time_incr_t *incr) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(incr != C1588_NULL_TIME_INCR);

  c1588_status_t ret = C1588_SUCCESS;

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTCINCR,
                    (incr->incr_subns | (incr->incr_ns << 24u)));

  this_c1588->rtc_incr.incr_ns = incr->incr_ns;
  this_c1588->rtc_incr.incr_subns = incr->incr_subns;

  return ret;
}

/*******************************************************************************
 * core1588_rtc_set_increment_freq()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_rtc_set_increment_freq(c1588_instance_t *this_c1588,
                                               uint32_t freq) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(freq != 0);

  c1588_status_t ret = C1588_SUCCESS;

  this_c1588->rtc_incr.incr_ns = N_SEC_SYS_CLK_PERIOD_FROM_FREQ_HZ(freq);
  this_c1588->rtc_incr.incr_subns = SUB_NS_FROM_SYS_CLK_FREQ_HZ(freq);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTCINCR,
                    (this_c1588->rtc_incr.incr_subns |
                     (this_c1588->rtc_incr.incr_ns << 24u)));

  return ret;
}

/*******************************************************************************
 * core1588_rtc_get_increment()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_rtc_get_increment(c1588_instance_t *this_c1588,
                                          c1588_time_incr_t *incr) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(incr != C1588_NULL_TIME_INCR);

  c1588_status_t ret = C1588_SUCCESS;

  incr->incr_ns = this_c1588->rtc_incr.incr_ns;
  incr->incr_subns = this_c1588->rtc_incr.incr_subns;

  return ret;
}

/*******************************************************************************
 * core1588_rtc_adjfreq()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_rtc_adjfreq(c1588_instance_t *this_c1588,
                                    int32_t freq_adj) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  c1588_status_t ret = C1588_SUCCESS;
  c1588_time_incr_t current_incr;

  core1588_rtc_get_increment(this_c1588, &current_incr);
  current_incr.incr_ns += freq_adj;
  core1588_rtc_set_increment(this_c1588, &current_incr);

  return ret;
}

/*******************************************************************************
 * core1588_rtc_adjtime()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_rtc_adjtime(c1588_instance_t *this_c1588,
                                    int32_t adj_val) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  c1588_status_t ret = C1588_SUCCESS;

  if (adj_val < 0) {
    adj_val = -adj_val;
    HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTCCTRL,
                      (uint32_t)(adj_val | (1u << 31u)));
  } else {
    HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTCCTRL,
                      (uint32_t)adj_val);
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_get_rxstamp()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_ptp_get_rxstamp(c1588_instance_t *this_c1588,
                                        c1588_timestamp_t *ts,
                                        uint32_t rx_type) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(ts != C1588_NULL_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  if (HAL_get_32bit_reg_field(this_c1588->base_address,
                              CORE1588_REGS_RIS_RIRTS) == 0) {
    ret = C1588_FAILURE;
  }

  if (ret != C1588_FAILURE) {
    if ((rx_type == C1588_RXSYNC) || (rx_type == C1588_RXDELAYREQ)) {
      ts->nsecs =
          HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTSL);

      ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                       this_c1588->base_address,
                       CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SEC_MSB_16BITS)
                   << 32) |
                  (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                              CORE1588_REGS_RTSM));

      ts->subnsecs =
          (HAL_get_32bit_reg_field(this_c1588->base_address,
                                   CORE1588_REGS_RTSMSBSECSUBNS_PTP_RX_SUBNSEC)
           << 8U);
    } else if ((rx_type == C1588_RXPDELAYREQ) ||
               (rx_type == C1588_RXPDELAYRESP)) {
      ts->nsecs =
          HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_PEERRTSL);

      ts->secs =
          (((uint64_t)HAL_get_32bit_reg_field(
                this_c1588->base_address,
                CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSEC_MSB16BITS)
            << 32) |
           (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                       CORE1588_REGS_PEERRTSM));

      ts->subnsecs = (HAL_get_32bit_reg_field(
                          this_c1588->base_address,
                          CORE1588_REGS_PEERRTSMSBSECSUBNS_PTP_PEER_RXSUBNSEC)
                      << 8U);
    } else {
      ret = C1588_FAILURE;
    }
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_get_rx_seq_id()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_ptp_get_rx_seq_id(c1588_instance_t *this_c1588,
                                          c1588_ptp_packet_info_t *pkt_info,
                                          uint32_t rx_type) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(pkt_info != C1588_NULL_PACKET_INFO);

  c1588_status_t ret = C1588_SUCCESS;

  if ((rx_type == C1588_RXSYNC) || (rx_type == C1588_RXDELAYREQ)) {
    pkt_info->seq_id = (uint16_t)HAL_get_32bit_reg_field(
        this_c1588->base_address, CORE1588_REGS_RTSID2_PTP_RX_SEQ_ID);
  } else if ((rx_type == C1588_RXPDELAYREQ) ||
             (rx_type == C1588_RXPDELAYRESP)) {
    pkt_info->seq_id = (uint16_t)HAL_get_32bit_reg_field(
        this_c1588->base_address, CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSEQID);
  } else {
    ret = C1588_FAILURE;
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_get_rx_src_port_id()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_ptp_get_rx_src_port_id(c1588_instance_t *this_c1588,
                                               uint8_t id[10],
                                               uint32_t rx_type) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  c1588_status_t ret = C1588_SUCCESS;
  uint32_t id_0_31 = 0;
  uint32_t id_32_63 = 0;
  uint16_t id_64_79 = 0;

  if ((rx_type == C1588_RXSYNC) || (rx_type == C1588_RXDELAYREQ)) {
    id_0_31 = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTSID0);

    id_32_63 =
        HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_RTSID1);

    id_64_79 = (uint16_t)(HAL_get_32bit_reg_field(
        this_c1588->base_address,
        CORE1588_REGS_RTSID2_PTP_RX_SRC_PORTID_UPPER_16BITS));
  } else if ((rx_type == C1588_RXPDELAYREQ) ||
             (rx_type == C1588_RXPDELAYRESP)) {
    id_0_31 =
        HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_PEERRTSID0);

    id_32_63 =
        HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_PEERRTSID1);

    id_64_79 = HAL_get_32bit_reg_field(
        this_c1588->base_address,
        CORE1588_REGS_PEERRTSID2_PTP_PEER_RXSRCPORTID_MSB16BITS);
  } else {
    ret = C1588_FAILURE;
  }

  if (ret != C1588_FAILURE) {
    memcpy(id, &id_0_31, sizeof(id_0_31));
    memcpy(id + 0x4, &id_32_63, sizeof(id_32_63));
    memcpy(id + 0x8, &id_64_79, sizeof(id_64_79));
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_rx_handler()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_ptp_rx_default_handler(c1588_instance_t *this_c1588,
                                     uint32_t rx_irq_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t rx_type = rx_irq_mask & C1588_RX_IRQ_TYPE_MASK;
  c1588_ptp_packet_info_t pkt_info = {0};

  core1588_ptp_get_rxstamp(this_c1588, &(pkt_info.ts), rx_type);
  core1588_ptp_get_rx_src_port_id(this_c1588, pkt_info.src_port_id, rx_type);
  core1588_ptp_get_rx_seq_id(this_c1588, &pkt_info, rx_type);
  if ((rx_type == C1588_RXSYNC) || (rx_type == C1588_RXDELAYREQ)) {
    core1588_clear_irq_src(this_c1588,
                           (rx_irq_mask | C1588_RTSID_IRQ | C1588_RTS_IRQ));
  } else if ((rx_type == C1588_RXPDELAYREQ) ||
             (rx_type == C1588_RXPDELAYRESP)) {
    core1588_clear_irq_src(this_c1588,
                           (rx_irq_mask | C1588_PEERRTSID_IRQ | C1588_RTS_IRQ));
  } else {
    core1588_clear_irq_src(this_c1588, (rx_irq_mask | C1588_RTS_IRQ));
  }
  pkt_info.type = rx_type;

  core1588_ptp_rx_add_to_buffer(this_c1588, pkt_info);
}

/*******************************************************************************
 * core1588_ptp_rx_add_to_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_ptp_rx_add_to_buffer(c1588_instance_t *this_c1588,
                                   c1588_ptp_packet_info_t pkt_info) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
  core1588_irq_control(this_c1588, (irq_en & C1588_RX_IRQ_MASK), C1588_DISABLE);

  if (((this_c1588->rx_write_idx + 1) % C1588_RX_RING_SIZE) ==
      this_c1588->rx_read_idx) {
    this_c1588->rx_read_idx =
        ((this_c1588->rx_read_idx + 1) % C1588_RX_RING_SIZE);
  }

  this_c1588->rx_buf[this_c1588->rx_write_idx] = pkt_info;
  this_c1588->rx_buf_read[this_c1588->rx_write_idx] = 0;
  this_c1588->rx_write_idx =
      ((this_c1588->rx_write_idx + 1) % C1588_RX_RING_SIZE);

  core1588_irq_control(this_c1588, (irq_en & C1588_RX_IRQ_MASK), C1588_ENABLE);
}

/*******************************************************************************
 * core1588_ptp_rx_get_from_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t
core1588_ptp_rx_get_from_buffer(c1588_instance_t *this_c1588,
                                c1588_ptp_packet_info_t *pkt_info,
                                uint16_t seq_id_rx) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(pkt_info != C1588_NULL_PACKET_INFO);

  c1588_status_t ret = C1588_SUCCESS;

  if (this_c1588->rx_read_idx == this_c1588->rx_write_idx) {
    pkt_info = C1588_NULL_PACKET_INFO;
    ret = C1588_FAILURE;
  } else {
    uint16_t current_idx = this_c1588->rx_read_idx;

    while (current_idx != this_c1588->rx_write_idx) {
      if (this_c1588->rx_buf_read[current_idx] == 0) {
        if (this_c1588->rx_buf[current_idx].seq_id == seq_id_rx) {
          *pkt_info = this_c1588->rx_buf[current_idx];
          break;
        }
      }
      current_idx = ((current_idx + 1) % C1588_RX_RING_SIZE);
    }

    if (current_idx != this_c1588->rx_write_idx) {
      uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
      core1588_irq_control(this_c1588, (irq_en & C1588_RX_IRQ_MASK),
                           C1588_DISABLE);

      if (current_idx != this_c1588->rx_read_idx) {
        this_c1588->rx_buf_read[current_idx] = 1;
      }
      this_c1588->rx_read_idx =
          ((this_c1588->rx_read_idx + 1) % C1588_RX_RING_SIZE);

      core1588_irq_control(this_c1588, (irq_en & C1588_RX_IRQ_MASK),
                           C1588_ENABLE);
    } else {
      pkt_info = C1588_NULL_PACKET_INFO;
      ret = C1588_FAILURE;
    }
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_set_rx_unicast_addr()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_ptp_set_rx_unicast_addr(c1588_instance_t *this_c1588,
                                                uint8_t addr[16]) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  c1588_status_t ret = C1588_SUCCESS;

  uint32_t addr_0_31 =
      addr[0] | (addr[1] << 8) | (addr[2] << 16) | (addr[3] << 24);
  uint32_t addr_32_63 =
      addr[4] | (addr[5] << 8) | (addr[6] << 16) | (addr[7] << 24);
  uint32_t addr_64_95 =
      addr[8] | (addr[9] << 8) | (addr[10] << 16) | (addr[11] << 24);
  uint32_t addr_92_127 =
      addr[12] | (addr[13] << 8) | (addr[14] << 16) | (addr[15] << 24);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RXUCASTADDR0,
                    addr_0_31);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RXUCASTADDR1,
                    addr_32_63);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RXUCASTADDR2,
                    addr_64_95);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_RXUCASTADDR3,
                    addr_92_127);

  return ret;
}

/*******************************************************************************
 * core1588_ptp_get_txstamp()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_ptp_get_txstamp(c1588_instance_t *this_c1588,
                                        c1588_timestamp_t *ts,
                                        uint32_t tx_type) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(ts != C1588_NULL_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  if (HAL_get_32bit_reg_field(this_c1588->base_address,
                              CORE1588_REGS_RIS_RITTS) == 0) {
    ret = C1588_FAILURE;
  }

  if (ret != C1588_FAILURE) {
    if ((tx_type == C1588_TXSYNC) || (tx_type == C1588_TXDELAYREQ)) {
      ts->nsecs =
          HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_TTSL);

      ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                       this_c1588->base_address,
                       CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SEC_MSB_16BITS)
                   << 32u) |
                  (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                              CORE1588_REGS_TTSM));

      ts->subnsecs =
          (HAL_get_32bit_reg_field(this_c1588->base_address,
                                   CORE1588_REGS_TTSMSBSECSUBNS_PTP_TX_SUBNSEC)
           << 8U);
    } else if ((tx_type == C1588_TXPDELAYREQ) ||
               (tx_type == C1588_TXPDELAYRESP)) {
      ts->nsecs =
          HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_PEERTTSL);

      ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                       this_c1588->base_address,
                       CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSECMSB16BITS)
                   << 32u) |
                  (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                              CORE1588_REGS_PEERTTSM));

      ts->subnsecs = (HAL_get_32bit_reg_field(
                          this_c1588->base_address,
                          CORE1588_REGS_PEERTTSMSBSECSUBNS_PTP_PEER_TXSUBNSEC)
                      << 8U);
    } else {
      ret = C1588_FAILURE;
    }
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_get_tx_seq_id()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_ptp_get_tx_seq_id(c1588_instance_t *this_c1588,
                                          c1588_ptp_packet_info_t *pkt_info,
                                          uint32_t tx_type) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(pkt_info != C1588_NULL_PACKET_INFO);

  c1588_status_t ret = C1588_SUCCESS;

  if ((tx_type == C1588_TXSYNC) || (tx_type == C1588_TXDELAYREQ)) {
    pkt_info->seq_id = (uint16_t)HAL_get_32bit_reg_field(
        this_c1588->base_address, CORE1588_REGS_TTSID_PTP_TX_SEQ_ID);
  } else if ((tx_type == C1588_TXPDELAYREQ) ||
             (tx_type == C1588_TXPDELAYRESP)) {
    pkt_info->seq_id = (uint16_t)HAL_get_32bit_reg_field(
        this_c1588->base_address, CORE1588_REGS_PEERTTSID_PTP_PEER_TXSEQID);
  } else {
    ret = C1588_FAILURE;
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_tx_handler()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_ptp_tx_default_handler(c1588_instance_t *this_c1588,
                                     uint32_t tx_irq_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t tx_type = tx_irq_mask & C1588_TX_IRQ_TYPE_MASK;
  c1588_ptp_packet_info_t pkt_info = {0};

  core1588_ptp_get_txstamp(this_c1588, &(pkt_info.ts), tx_type);
  core1588_ptp_get_tx_seq_id(this_c1588, &pkt_info, tx_type);
  if ((tx_type == C1588_TXSYNC) || (tx_type == C1588_TXDELAYREQ)) {
    core1588_clear_irq_src(this_c1588,
                           (tx_irq_mask | C1588_TTSID_IRQ | C1588_TTS_IRQ));
  } else if ((tx_type == C1588_TXPDELAYREQ) ||
             (tx_type == C1588_TXPDELAYRESP)) {
    core1588_clear_irq_src(this_c1588,
                           (tx_irq_mask | C1588_PEERTTSID_IRQ | C1588_TTS_IRQ));
  } else {
    core1588_clear_irq_src(this_c1588, (tx_irq_mask | C1588_TTS_IRQ));
  }
  pkt_info.type = tx_type;

  core1588_ptp_tx_add_to_buffer(this_c1588, pkt_info);
}

/*******************************************************************************
 * core1588_ptp_tx_add_to_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_ptp_tx_add_to_buffer(c1588_instance_t *this_c1588,
                                   c1588_ptp_packet_info_t pkt_info) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
  core1588_irq_control(this_c1588, (irq_en & C1588_TX_IRQ_MASK), C1588_DISABLE);

  if (((this_c1588->tx_write_idx + 1) % C1588_TX_RING_SIZE) ==
      this_c1588->tx_read_idx) {
    this_c1588->tx_read_idx =
        ((this_c1588->tx_read_idx + 1) % C1588_TX_RING_SIZE);
  }

  this_c1588->tx_buf[this_c1588->tx_write_idx] = pkt_info;
  this_c1588->tx_buf_read[this_c1588->tx_write_idx] = 0;
  this_c1588->tx_write_idx =
      ((this_c1588->tx_write_idx + 1) % C1588_TX_RING_SIZE);

  core1588_irq_control(this_c1588, (irq_en & C1588_TX_IRQ_MASK), C1588_ENABLE);
}

/*******************************************************************************
 * core1588_ptp_tx_get_from_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t
core1588_ptp_tx_get_from_buffer(c1588_instance_t *this_c1588,
                                c1588_ptp_packet_info_t *pkt_info,
                                uint16_t seq_id_tx) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(pkt_info != C1588_NULL_PACKET_INFO);

  c1588_status_t ret = C1588_SUCCESS;

  if (this_c1588->tx_read_idx == this_c1588->tx_write_idx) {
    pkt_info = C1588_NULL_PACKET_INFO;
    ret = C1588_FAILURE;
  } else {
    uint16_t current_idx = this_c1588->tx_read_idx;

    while (current_idx != this_c1588->tx_write_idx) {
      if (this_c1588->tx_buf_read[current_idx] == 0) {
        if (this_c1588->tx_buf[current_idx].seq_id == seq_id_tx) {
          *pkt_info = this_c1588->tx_buf[current_idx];
          break;
        }
      }
      current_idx = ((current_idx + 1) % C1588_TX_RING_SIZE);
    }

    if (current_idx != this_c1588->tx_write_idx) {
      uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
      core1588_irq_control(this_c1588, (irq_en & C1588_TX_IRQ_MASK),
                           C1588_DISABLE);

      if (current_idx != this_c1588->tx_read_idx) {
        this_c1588->tx_buf_read[current_idx] = 1;
      }
      this_c1588->tx_read_idx =
          ((this_c1588->tx_read_idx + 1) % C1588_TX_RING_SIZE);

      core1588_irq_control(this_c1588, (irq_en & C1588_TX_IRQ_MASK),
                           C1588_ENABLE);
    } else {
      pkt_info = C1588_NULL_PACKET_INFO;
      ret = C1588_FAILURE;
    }
  }

  return ret;
}

/*******************************************************************************
 * core1588_ptp_set_tx_unicast_addr()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_ptp_set_tx_unicast_addr(c1588_instance_t *this_c1588,
                                                uint8_t addr[16]) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  c1588_status_t ret = C1588_SUCCESS;

  uint32_t addr_0_31 =
      addr[0] | (addr[1] << 8) | (addr[2] << 16) | (addr[3] << 24);
  uint32_t addr_32_63 =
      addr[4] | (addr[5] << 8) | (addr[6] << 16) | (addr[7] << 24);
  uint32_t addr_64_95 =
      addr[8] | (addr[9] << 8) | (addr[10] << 16) | (addr[11] << 24);
  uint32_t addr_92_127 =
      addr[12] | (addr[13] << 8) | (addr[14] << 16) | (addr[15] << 24);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TXUCASTADDR0,
                    addr_0_31);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TXUCASTADDR1,
                    addr_32_63);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TXUCASTADDR2,
                    addr_64_95);

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TXUCASTADDR3,
                    addr_92_127);

  return ret;
}

/*******************************************************************************
 * core1588_latch_check_enabled()
 * See "core1588.h" file for details of how to use this function.
 */
uint8_t core1588_latch_check_enabled(c1588_instance_t *this_c1588,
                                     uint32_t latch_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint8_t ret = 0;

  latch_mask = (latch_mask & C1588_LATCH_MASK_ALL);

  uint32_t cfg =
      HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_GCFG);

  ret = (cfg && latch_mask);

  return ret;
}

/*******************************************************************************
 * core1588_latch_control()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_latch_control(c1588_instance_t *this_c1588, uint32_t latch_mask,
                            uint32_t control) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t reg_value;

  reg_value = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_GCFG);

  if (control == C1588_ENABLE) {
    reg_value |= ((latch_mask & C1588_LATCH_MASK_ALL) << C1588_LATCH_CFG_SHIFT);
  } else {
    reg_value &=
        ~((latch_mask & C1588_LATCH_MASK_ALL) << C1588_LATCH_CFG_SHIFT);
  }

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_GCFG, reg_value);
}

/*******************************************************************************
 * core1588_latch_get_timestamp()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_latch_get_timestamp(c1588_instance_t *this_c1588,
                                            c1588_timestamp_t *ts,
                                            uint32_t latch_id) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(ts != C1588_NULL_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  if (latch_id == C1588_LATCH_0) {
    ts->nsecs = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_LT0L);

    ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                     this_c1588->base_address,
                     CORE1588_REGS_LT0MSBSEC_LT0_SEC_MSB_16BITS)
                 << 32) |
                (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                            CORE1588_REGS_LT0M));
  } else if (latch_id == C1588_LATCH_1) {
    ts->nsecs = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_LT1L);

    ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                     this_c1588->base_address,
                     CORE1588_REGS_LT1MSBSEC_LT1_SEC_MSB_16BITS)
                 << 32u) |
                (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                            CORE1588_REGS_LT1M));
  } else if (latch_id == C1588_LATCH_2) {
    ts->nsecs = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_LT2L);

    ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                     this_c1588->base_address,
                     CORE1588_REGS_LT2MSBSEC_LT2_SEC_MSB_16BITS)
                 << 32u) |
                (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                            CORE1588_REGS_LT2M));
  } else {
    ret = C1588_FAILURE;
  }

  return ret;
}

/*******************************************************************************
 * core1588_latch_handler()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_latch_default_handler(c1588_instance_t *this_c1588,
                                    uint32_t latch_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  if (latch_mask & C1588_LATCH_0) {
    c1588_rtc_event_timestamp_t latch_0_ts;
    core1588_latch_get_timestamp(this_c1588, &latch_0_ts.ts, C1588_LATCH_0);
    core1588_clear_irq_src(this_c1588, C1588_LT0_IRQ);
    latch_0_ts.id = C1588_LATCH_0;
    core1588_latch_add_to_buffer(this_c1588, latch_0_ts);
  }
  if (latch_mask & C1588_LATCH_1) {
    c1588_rtc_event_timestamp_t latch_1_ts;
    core1588_latch_get_timestamp(this_c1588, &latch_1_ts.ts, C1588_LATCH_1);
    core1588_clear_irq_src(this_c1588, C1588_LT1_IRQ);
    latch_1_ts.id = C1588_LATCH_1;
    core1588_latch_add_to_buffer(this_c1588, latch_1_ts);
  }
  if (latch_mask & C1588_LATCH_2) {
    c1588_rtc_event_timestamp_t latch_2_ts;
    core1588_latch_get_timestamp(this_c1588, &latch_2_ts.ts, C1588_LATCH_2);
    core1588_clear_irq_src(this_c1588, C1588_LT2_IRQ);
    latch_2_ts.id = C1588_LATCH_2;
    core1588_latch_add_to_buffer(this_c1588, latch_2_ts);
  }
}

/*******************************************************************************
 * core1588_latch_add_to_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_latch_add_to_buffer(c1588_instance_t *this_c1588,
                                  c1588_rtc_event_timestamp_t latch_ts) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
  core1588_irq_control(this_c1588, (irq_en & C1588_LATCH_MASK_ALL),
                       C1588_DISABLE);

  if (((this_c1588->latch_write_idx + 1) % C1588_LATCH_RING_SIZE) ==
      this_c1588->latch_read_idx) {
    this_c1588->latch_read_idx =
        ((this_c1588->latch_read_idx + 1) % C1588_LATCH_RING_SIZE);
  }

  this_c1588->latch_buf[this_c1588->latch_write_idx] = latch_ts;
  this_c1588->latch_write_idx =
      ((this_c1588->latch_write_idx + 1) % C1588_LATCH_RING_SIZE);

  core1588_irq_control(this_c1588, (irq_en & C1588_LATCH_MASK_ALL),
                       C1588_ENABLE);
}

/*******************************************************************************
 * core1588_latch_get_from_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t
core1588_latch_get_from_buffer(c1588_instance_t *this_c1588,
                               c1588_rtc_event_timestamp_t *latch_ts) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(latch_ts != C1588_NULL_RTC_EVENT_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  if (this_c1588->latch_read_idx == this_c1588->latch_write_idx) {
    latch_ts = C1588_NULL_RTC_EVENT_TIMESTAMP;
    ret = C1588_FAILURE;
  } else {
    uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
    core1588_irq_control(this_c1588, (irq_en & C1588_LATCH_MASK_ALL),
                         C1588_DISABLE);

    *latch_ts = this_c1588->latch_buf[this_c1588->latch_read_idx];
    this_c1588->latch_read_idx =
        ((this_c1588->latch_read_idx + 1) % C1588_LATCH_RING_SIZE);

    core1588_irq_control(this_c1588, (irq_en & C1588_LATCH_MASK_ALL),
                         C1588_ENABLE);
  }

  return ret;
}

/*******************************************************************************
 * core1588_trigger_check_enabled()
 * See "core1588.h" file for details of how to use this function.
 */
uint8_t core1588_trigger_check_enabled(c1588_instance_t *this_c1588,
                                       uint32_t trigger_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint8_t ret = 0;

  trigger_mask = (trigger_mask & C1588_TRIGGER_MASK_ALL);

  uint32_t cfg =
      HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_GCFG);

  ret = (cfg && trigger_mask);

  return ret;
}

/*******************************************************************************
 * core1588_trigger_control()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_trigger_control(c1588_instance_t *this_c1588,
                              uint32_t trigger_mask, uint32_t control) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t reg_value;

  reg_value = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_GCFG);

  if (control == C1588_ENABLE) {
    reg_value |=
        ((trigger_mask & C1588_TRIGGER_MASK_ALL) << C1588_TRIGGER_CFG_SHIFT);
  } else {
    reg_value &=
        ~((trigger_mask & C1588_TRIGGER_MASK_ALL) << C1588_TRIGGER_CFG_SHIFT);
  }

  HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_GCFG, reg_value);
}

/*******************************************************************************
 * core1588_trigger_set_timestamp()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_trigger_set_timestamp(c1588_instance_t *this_c1588,
                                              c1588_timestamp_t *ts,
                                              uint32_t trigger_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(ts != C1588_NULL_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  if (core1588_trigger_check_enabled(this_c1588, trigger_mask) == 0) {
    ret = C1588_FAILURE;
  }

  if (ret != C1588_FAILURE) {
    if (trigger_mask == C1588_TRIGGER_0) {
      HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT0L,
                        ts->nsecs);

      HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT0M,
                        (uint32_t)ts->secs);

      HAL_set_32bit_reg_field(this_c1588->base_address,
                              CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS,
                              (uint16_t)(ts->secs >> 32u));
    } else if (trigger_mask == C1588_TRIGGER_1) {
      HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT1L,
                        ts->nsecs);

      HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT1M,
                        (uint32_t)ts->secs);

      HAL_set_32bit_reg_field(this_c1588->base_address,
                              CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS,
                              (uint16_t)(ts->secs >> 32u));
    } else if (trigger_mask == C1588_TRIGGER_2) {
      HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT2L,
                        ts->nsecs);

      HAL_set_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT2M,
                        (uint32_t)ts->secs);

      HAL_set_32bit_reg_field(this_c1588->base_address,
                              CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS,
                              (uint16_t)(ts->secs >> 32u));
    }
  }

  return ret;
}

/*******************************************************************************
 * core1588_trigger_get_timestamp()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t core1588_trigger_get_timestamp(c1588_instance_t *this_c1588,
                                              c1588_timestamp_t *ts,
                                              uint32_t trigger_id) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(ts != C1588_NULL_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  if (trigger_id == C1588_TRIGGER_0) {
    ts->nsecs = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT0L);

    ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                     this_c1588->base_address,
                     CORE1588_REGS_TT0MSBSEC_TT0_SEC_MSB_16BITS)
                 << 32) |
                (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                            CORE1588_REGS_TT0M));
  } else if (trigger_id == C1588_TRIGGER_1) {
    ts->nsecs = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT1L);

    ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                     this_c1588->base_address,
                     CORE1588_REGS_TT1MSBSEC_TT1_SEC_MSB_16BITS)
                 << 32u) |
                (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                            CORE1588_REGS_TT1M));
  } else if (trigger_id == C1588_TRIGGER_2) {
    ts->nsecs = HAL_get_32bit_reg(this_c1588->base_address, CORE1588_REGS_TT2L);

    ts->secs = (((uint64_t)HAL_get_32bit_reg_field(
                     this_c1588->base_address,
                     CORE1588_REGS_TT2MSBSEC_TT2_SEC_MSB_16BITS)
                 << 32u) |
                (uint64_t)HAL_get_32bit_reg(this_c1588->base_address,
                                            CORE1588_REGS_TT2M));
  } else {
    ret = C1588_FAILURE;
  }

  return ret;
}

/*******************************************************************************
 * core1588_trigger_handler()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_trigger_default_handler(c1588_instance_t *this_c1588,
                                      uint32_t trigger_mask) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  if (trigger_mask & C1588_TRIGGER_0) {
    c1588_rtc_event_timestamp_t trigger_0_ts;
    core1588_trigger_get_timestamp(this_c1588, &trigger_0_ts.ts,
                                   C1588_TRIGGER_0);
    core1588_clear_irq_src(this_c1588, C1588_TRIGGER_0);
    trigger_0_ts.id = C1588_TRIGGER_0;
    core1588_trigger_add_to_buffer(this_c1588, trigger_0_ts);
  }
  if (trigger_mask & C1588_TRIGGER_1) {
    c1588_rtc_event_timestamp_t trigger_1_ts;
    core1588_trigger_get_timestamp(this_c1588, &trigger_1_ts.ts,
                                   C1588_TRIGGER_1);
    core1588_clear_irq_src(this_c1588, C1588_TRIGGER_1);
    trigger_1_ts.id = C1588_TRIGGER_1;
    core1588_trigger_add_to_buffer(this_c1588, trigger_1_ts);
  }
  if (trigger_mask & C1588_TRIGGER_2) {
    c1588_rtc_event_timestamp_t trigger_2_ts;
    core1588_trigger_get_timestamp(this_c1588, &trigger_2_ts.ts,
                                   C1588_TRIGGER_2);
    core1588_clear_irq_src(this_c1588, C1588_TRIGGER_2);
    trigger_2_ts.id = C1588_TRIGGER_2;
    core1588_trigger_add_to_buffer(this_c1588, trigger_2_ts);
  }
}

/*******************************************************************************
 * core1588_trigger_add_to_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
void core1588_trigger_add_to_buffer(c1588_instance_t *this_c1588,
                                    c1588_rtc_event_timestamp_t trigger_ts) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);

  uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
  core1588_irq_control(this_c1588, (irq_en & C1588_TRIGGER_MASK_ALL),
                       C1588_DISABLE);

  if (((this_c1588->trigger_write_idx + 1) % C1588_TRIGGER_RING_SIZE) ==
      this_c1588->trigger_read_idx) {
    this_c1588->trigger_read_idx =
        ((this_c1588->trigger_read_idx + 1) % C1588_TRIGGER_RING_SIZE);
  }

  this_c1588->trigger_buf[this_c1588->trigger_write_idx] = trigger_ts;
  this_c1588->trigger_write_idx =
      ((this_c1588->trigger_write_idx + 1) % C1588_TRIGGER_RING_SIZE);

  core1588_irq_control(this_c1588, (irq_en & C1588_TRIGGER_MASK_ALL),
                       C1588_ENABLE);
}

/*******************************************************************************
 * core1588_trigger_get_from_buffer()
 * See "core1588.h" file for details of how to use this function.
 */
c1588_status_t
core1588_trigger_get_from_buffer(c1588_instance_t *this_c1588,
                                 c1588_rtc_event_timestamp_t *trigger_ts) {
  HAL_ASSERT(this_c1588 != C1588_NULL_INSTANCE);
  HAL_ASSERT(trigger_ts != C1588_NULL_RTC_EVENT_TIMESTAMP);

  c1588_status_t ret = C1588_SUCCESS;

  if (this_c1588->trigger_read_idx == this_c1588->trigger_write_idx) {
    trigger_ts = C1588_NULL_RTC_EVENT_TIMESTAMP;
    ret = C1588_FAILURE;
  } else {
    uint32_t irq_en = core1588_get_enabled_irq(this_c1588);
    core1588_irq_control(this_c1588, (irq_en & C1588_TRIGGER_MASK_ALL),
                         C1588_DISABLE);

    *trigger_ts = this_c1588->trigger_buf[this_c1588->trigger_read_idx];
    this_c1588->trigger_read_idx =
        ((this_c1588->trigger_read_idx + 1) % C1588_TRIGGER_RING_SIZE);

    core1588_irq_control(this_c1588, (irq_en & C1588_TRIGGER_MASK_ALL),
                         C1588_ENABLE);
  }

  return ret;
}

#ifdef __cplusplus
}
#endif
