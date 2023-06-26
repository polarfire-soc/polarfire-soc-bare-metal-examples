#include "phy.h"
#include "core10gbasekr_phy_link_training.h"

/*------------------------Private Definitions---------------------------------*/
#define AN_RESET                        (1U)
#define AN_ENABLE                       (1U)
#define AN_RESTART                      (1U)

#define TX_CTRL_DATA                    (0U)
#define TX_CTRL_AN                      (2U)
#define TX_CTRL_LT                      (3U)

/*------------------------Public Variables------------------------------------*/
extern uint32_t prbs_cnt;

/*------------------------Public Function-------------------------------------*/

/***************************************************************************//**
  Please see phy.h for description
 */
void
PHY10GKR_init
(
    phy10gkr_instance_t * this_phy,
    addr_t base_addr
)
{
    /* Set Core10GBaseKR PHY Base addresses */
    this_phy->base_addr = base_addr;
    this_phy->an_base_addr = this_phy->base_addr | C10GB_AN_BASE_OFFSET;
    this_phy->lt_base_addr = this_phy->base_addr | C10GB_LT_BASE_OFFSET;
    this_phy->tx_ctrl_base_addr = this_phy->base_addr |
                                    C10GB_TX_CTRL_BASE_OFFSET;
    this_phy->rx_status_base_addr = this_phy->base_addr |
                                        C10GB_RX_STATUS_BASE_OFFSET;

    PHY10GKR_config(this_phy);
}

/***************************************************************************//**
  Please see phy.h for description
 */
void
PHY10GKR_config
(
    phy10gkr_instance_t * this_phy
)
{
    /* set data path to AN block */
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                            C10GB_TX_CTRL_PMA_DATA,
                            TX_CTRL_AN);

    /* reset phy tx and rx */
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                            C10GB_TX_CTRL_TX_RESET,
                            0x1);
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                            C10GB_TX_CTRL_RX_RESET,
                            0x1);

    /* set data path to AN block */
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                            C10GB_TX_CTRL_PMA_DATA,
                            TX_CTRL_AN);

    /* transmit equalization tap limit configurations */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MAX_MAIN_TAP,
                            C10GBKR_LT_MAIN_TAP_MAX_LIMIT);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MIN_MAIN_TAP,
                            C10GBKR_LT_MAIN_TAP_MIN_LIMIT);

    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MAX_POST_TAP,
                            C10GBKR_LT_POST_TAP_MAX_LIMIT);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MIN_POST_TAP,
                            C10GBKR_LT_POST_TAP_MIN_LIMIT);

    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MAX_PRE_TAP,
                            C10GBKR_LT_PRE_TAP_MAX_LIMIT);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_MIN_PRE_TAP,
                            C10GBKR_LT_PRE_TAP_MIN_LIMIT);

    /* link partner preset conditions */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_PRESET_MAIN_TAP,
                            C10GBKR_LT_PRESET_MAIN_TAP);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_PRESET_POST_TAP,
                            C10GBKR_LT_PRESET_POST_TAP);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_PRESET_PRE_TAP,
                            C10GBKR_LT_PRESET_PRE_TAP);

    /* link partner preset conditions */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_INIT_MAIN_TAP,
                            C10GBKR_LT_INITIALIZE_MAIN_TAP);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_INIT_POST_TAP,
                            C10GBKR_LT_INITIALIZE_POST_TAP);
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                            C10GB_LT_INIT_PRE_TAP,
                            C10GBKR_LT_INITIALIZE_PRE_TAP);

    /* track number of fails since begining  */
    uint32_t tmp_fail_count = this_phy->lt.fail_cnt;
    uint32_t tmp_complete_count = this_phy->lt.complete_cnt;
    this_phy->lt = (phy10gkr_lt_instance_t){0};
    this_phy->lt.fail_cnt = tmp_fail_count;
    this_phy->lt.complete_cnt = tmp_complete_count;

    /* set the request that will be sent to the link partner */
    this_phy->lt.rx_request = C10GBKR_LT_INITIAL_REQUEST;

    /* Set AN and LT api state machine to init state */
    this_phy->an.api_state = AN_API_SM_INIT;
    this_phy->lt.api_state = LT_API_SM_INIT;

    /* Set status to original condition */
    this_phy->an.status = STATUS_AN_INCOMPLETE;
    this_phy->lt.status = STATUS_LT_INCOMPLETE;

    /* clear link training tap data */
    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
            C10GB_LT_LOCAL_RCVR_LOCKED, LOCAL_RXCVR_UNLOCKED);
    this_phy->lt.local_rxcvr = LOCAL_RXCVR_UNLOCKED;


    this_phy->lt.main = (const phy10gkr_coeff_update_t) {0};
    this_phy->lt.post = (const phy10gkr_coeff_update_t) {0};
    this_phy->lt.pre = (const phy10gkr_coeff_update_t) {0};

    this_phy->lt.lp_cal_sweep_state = MAIN_TAP;

    this_phy->lt.tx_equ_cnt = 0;
    this_phy->lt.rx_cal_cnt = 0;

    prbs_cnt = 0;
}

/***************************************************************************//**
  Please see phy.h for description
 */
void
PHY10GKR_autonegotiate_sm
(
    phy10gkr_instance_t * this_phy
)
{
    switch(this_phy->an.api_state)
    {
        case AN_API_SM_INIT:
            /* Tx control: Auto-negotiation, PCS data transmitted to Serdes */
            HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                    C10GB_TX_CTRL_PMA_DATA, TX_CTRL_AN);

            /* set link fail inhibit timer time to 500 ms*/
            HAL_set_32bit_reg(this_phy->an_base_addr,
                                C10GB_AN_LINK_FAIL_INHIBIT_TIMER,
                                C10GBKR_AN_LINK_FAIL_INHITBIT_TIMER);

            /* Reset auto-negotiation. */
            HAL_set_32bit_reg_field(this_phy->an_base_addr,
                                    C10GB_AN_RESET,
                                    AN_RESET);

            /* Enable auto-negotiation. */
            HAL_set_32bit_reg_field(this_phy->an_base_addr,
                                    C10GB_AN_ENABLE,
                                    AN_ENABLE);

            /* Restart auto-negotiation */
            HAL_set_32bit_reg_field(this_phy->an_base_addr,
                                    C10GB_AN_RESTART,
                                    AN_RESTART);

            this_phy->an.status = STATUS_AN_INCOMPLETE;
            this_phy->an.api_state = AN_API_SM_STATUS_UPDATE;
        break;

        case AN_API_SM_STATUS_UPDATE:
            /* Get auto-negotiation state machine state */
            this_phy->an.state = HAL_get_32bit_reg_field(this_phy->an_base_addr,
                                                            C10GB_AN_STATE);
            if(ST_AN_GOOD_CHECK == this_phy->an.state)
            {
                this_phy->an.complete_cnt++;

                this_phy->an.status = STATUS_AN_COMPLETE;


                /* Set data path to LT block, enable LT and timers */
                HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                        C10GB_LT_MAX_WAIT_TIMER,
                                        MAX_WAIT_TIMER_500MS);

                HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                        C10GB_TX_CTRL_PMA_DATA,
                                        TX_CTRL_LT);

                this_phy->lt.timer.start = PHY10GKR_get_current_time_ms();
            }
        break;

        default:
            HAL_ASSERT(0);
        break;
    }
}

/***************************************************************************//**
  Please see phy.h for description
 */
void
PHY10GKR_link_training_sm
(
    phy10gkr_instance_t * this_phy
)
{
    uint32_t c10gbkr_status = 0;

    this_phy->lt.state = HAL_get_32bit_reg_field(this_phy->lt_base_addr,
                                                    C10GB_LT_TRAINING_SM);

    switch(this_phy->lt.api_state)
    {
        case LT_API_SM_INIT:
            if(C10GBKR_LT_PRESET == this_phy->lt.rx_request)
            {
                HAL_set_32bit_reg(this_phy->lt_base_addr, C10GB_LT_CTRL,
                        (C10GB_LT_RESTART_EN_MASK | C10GB_LT_PRESET_MASK));
            }
            else if(C10GBKR_LT_INITALISE == this_phy->lt.rx_request)
            {
                HAL_set_32bit_reg(this_phy->lt_base_addr,
                        C10GB_LT_CTRL,
                        (C10GB_LT_RESTART_EN_MASK | C10GB_LT_INIT_MASK));
            }

            this_phy->lt.api_state = LT_API_SM_STATUS_UPDATE;
        break;

        case LT_API_SM_STATUS_UPDATE:
            c10gbkr_status = HAL_get_32bit_reg(this_phy->lt_base_addr,
                                                    C10GB_LT_STATUS);

            this_phy->lt.timer.end = PHY10GKR_get_current_time_ms() -
                                        this_phy->lt.timer.start;

            if((c10gbkr_status & C10GB_LT_TRAINING_FAIL_MASK) ||
                    this_phy->lt.timer.end > LT_SOFTWARE_WAIT_TIMER_MS)
            {
                this_phy->lt.fail_cnt++;
                this_phy->lt.status = STATUS_LT_FAILURE;

                /* disable lt hardware in the case that the software timer
                 * trigger the failure
                 */
                HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                        C10GB_LT_RESTART_EN,
                                        0x0);
                break;
            }

            if(c10gbkr_status & C10GB_LT_REQ_TX_EQUAL_MASK)
            {
                uint32_t tx_main_tap;
                uint32_t tx_post_tap;
                uint32_t tx_pre_tap;

                this_phy->lt.tx_equ_cnt++;

                tx_main_tap = HAL_get_32bit_reg_field(this_phy->lt_base_addr,
                                C10GB_LT_TX_NEW_MAIN_TAP);
                tx_post_tap = HAL_get_32bit_reg_field(this_phy->lt_base_addr,
                                C10GB_LT_TX_NEW_POST_TAP) -
                                C10GBKR_LT_POST_TAP_MAX_LIMIT;
                tx_pre_tap = HAL_get_32bit_reg_field(this_phy->lt_base_addr,
                                C10GB_LT_TX_NEW_PRE_TAP) -
                                C10GBKR_LT_PRE_TAP_MAX_LIMIT;

                PHY10GKR_serdes_tx_equalization(tx_main_tap,
                                                tx_post_tap,
                                                tx_pre_tap);

                /* signal to hardware to set tx status report field */
                HAL_set_32bit_reg(this_phy->lt_base_addr,
                                    C10GB_LT_TX_EQUAL,
                                    C10GB_LT_TX_EQUAL_PRE_DONE_MASK |
                                    C10GB_LT_TX_EQUAL_POST_DONE_MASK |
                                    C10GB_LT_TX_EQUAL_MAIN_DONE_MASK |
                                    C10GB_LT_TX_EQUAL_DONE_MASK);
            }

            if((c10gbkr_status & C10GB_LT_REQ_RX_CAL_MASK) &&
                    LOCAL_RXCVR_UNLOCKED == this_phy->lt.local_rxcvr)
            {
                /* signal to hardware to set tx coefficient update field */
                HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                        C10GB_LT_RX_CAL_DONE,
                                        1U);

                HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                        C10GB_LT_RX_CAL_DONE,
                                        0U);

                this_phy->lt.rx_cal_cnt++;
                if(C10GBKR_LT_PRESET == this_phy->lt.tx_request)
                {
                    link_partner_calibration_sm(this_phy);
                }
                else if(C10GBKR_LT_INITALISE == this_phy->lt.tx_request)
                {
                    this_phy->lt.local_rxcvr = LOCAL_RXCVR_LOCKED;

                    this_phy->lt.rcvr_cnt++;

                    HAL_set_32bit_reg_field(this_phy->lt_base_addr,
                                            C10GB_LT_LOCAL_RCVR_LOCKED,
                                            LOCAL_RXCVR_LOCKED);
                }

            }

            if(c10gbkr_status & C10GB_LT_SIGNAL_DETECT_MASK)
            {
                this_phy->lt.complete_cnt++;

                /* Tx control: CL49 Data, PCS data transmitted to Serdes */
                HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
                                        C10GB_TX_CTRL_PMA_DATA,
                                        TX_CTRL_DATA);
                this_phy->lt.sig_cnt++;
                this_phy->lt.status = STATUS_LT_COMPLETE;
                break;
            }
        break;

        default:
            HAL_ASSERT(0);
        break;
    }
    this_phy->lt.sm_cycle_cnt++;
}

/***************************************************************************//**
  Please see phy.h for description
 */
uint32_t
PHY10GKR_10gbasekr_sm
(
    phy10gkr_instance_t * this_phy
)
{
    switch(this_phy->c10gbkr_state)
    {
        case AN_SERDES_CONFIG:
            this_phy->c10gbkr_status = AN_SERDES_CONFIGURATION;
            PHY10GKR_config(this_phy);
            PHY10GKR_serdes_an_config();
            this_phy->c10gbkr_state = AN_SM;
        break;

        case AN_SM:
            this_phy->c10gbkr_status = AN_IN_PROGRESS;

            PHY10GKR_autonegotiate_sm(this_phy);
            if(STATUS_AN_COMPLETE == this_phy->an.status)
            {
                this_phy->c10gbkr_status = AN_COMPLETE;

                this_phy->c10gbkr_state = LT_SERDES_CONFIG;
            }
        break;

        case LT_SERDES_CONFIG:
            this_phy->c10gbkr_status = LT_SERDES_CONFIGURATION;
            if(PHY10GKR_serdes_lt_config() >= 1)
            {
                this_phy->c10gbkr_status = LT_SERDES_CAL_FAILURE;
                this_phy->c10gbkr_state = AN_SERDES_CONFIG;
            }
            else
            {
                this_phy->c10gbkr_status = LT_SERDES_CAL_COMPLETE;
                this_phy->c10gbkr_state = LT_SM;
            }
        break;

        case LT_SM:
            this_phy->c10gbkr_status = LT_IN_PROGRESS;
            PHY10GKR_link_training_sm(this_phy);

            if(STATUS_LT_FAILURE == this_phy->lt.status)
            {
                /* reset the state machine for next time its called */
                this_phy->c10gbkr_state = AN_SERDES_CONFIG;

                this_phy->c10gbkr_status = LT_FAILURE;
            }
            else if(STATUS_LT_COMPLETE == this_phy->lt.status)
            {
                this_phy->c10gbkr_state = LINK_ESTABLISHED_CHECK;
                this_phy->c10gbkr_status = LINK_ESTABLISHED;
            }
        break;

        case LINK_ESTABLISHED_CHECK:
            if(PHY10GKR_serdes_cdr_lock() > 0)
            {
                /* reset the state machine for next time its called */
                this_phy->c10gbkr_state = AN_SERDES_CONFIG;

                this_phy->c10gbkr_status = LINK_BROKEN;
            }
            else
            {
                this_phy->c10gbkr_status = LINK_ESTABLISHED;
            }
        break;

        default:
            HAL_ASSERT(0);
        break;
    }
    return this_phy->c10gbkr_status;
}

/***************************************************************************//**
  Please see phy.h for description
 */
void
PHY10GKR_set_lane_los_signal
(
    phy10gkr_instance_t * this_phy,
    uint32_t state
)
{
    HAL_set_32bit_reg_field(this_phy->tx_ctrl_base_addr,
            C10GB_TX_CTRL_XCVR_LOS, state);
}
