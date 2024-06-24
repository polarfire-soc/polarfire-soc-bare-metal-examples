/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HAL Embedded Software example
 *
 */

/*******************************************************************************
 *
 * Code running on E51
 *
 */

#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "mpfs_hal/mpfs_hal_version.h"
#include "inc/common.h"
#include "inc/menu_prints.h"
#include "inc/menu_selector.h"

#include "../../middleware/ymodem/ymodem.h"

/*******************************************************************************
 * Instruction message. This message will be transmitted over the UART to the
 * terminal emulator when the program starts.
 ******************************************************************************/



#define twoGb 0x10000000UL  /* ((1024*1024*1024)*2)/sizeof(uint32_t) */
#define small_ver 0x00010000UL
#define DDR_TEST

#define OFFSET_BETWEEN_LINES    0x1000U

#define DDR_BASE            0x80000000U
#define DDR_SIZE            0x40000000U
#define CACHE_LINE_SIZE     0x40U
#define MIN_OFFSET          1u
#define MAX_OFFSET          16u

#define NB_CACHE_LINES_USED     (DDR_SIZE / OFFSET_BETWEEN_LINES)

#define MAX_BUFFER_SIZE 128

static volatile uint32_t count_sw_ints_h0 = 0U;
volatile uint32_t g_10ms_count;
uint8_t data_block[256];
uint64_t hart_jump_ddr = 0U;
#ifndef DDR_BASE_BOARD
mss_uart_instance_t *g_uart = &g_mss_uart0_lo;
mss_uart_instance_t *g_debug_uart = &g_mss_uart0_lo;
#else
mss_uart_instance_t *g_uart = &g_mss_uart3_lo ;
mss_uart_instance_t *g_debug_uart = &g_mss_uart3_lo ;
#endif

uint32_t ddr_sr_test;
uint8_t verif_flag;
uint8_t *bin_base = (uint8_t *)DDR_BASE;
uint32_t uart0_mutex;

#define NO_OF_ITERATIONS    2
uint32_t pattern_offset = 12U;

/*
 * External data
 */

/*
 * Local functions
 */
static void ddr_read_write_nc(uint32_t no_access);
static void display_mss_regs(void);
static void main_menu_options(uint8_t* rx_buff, uint8_t get_uart_rx);
static uint8_t bus_error_unit(void);

/*
 * Extern functions
 */
#ifdef DEBUG_DDR_INIT
extern uint32_t tip_register_status(mss_uart_instance_t *g_mss_uart_debug_pt);
#endif

/**
 * idle in DTIM
 */
#ifdef E51_ENTER_SLEEP_STATE
__attribute__((section(".ram_codetext"))) static void loop_in_dtim(void)
{
    mb();
    /*Put this hart into WFI.*/
    while (1U)
    {
        do
        {
            __asm("wfi");
        }while (0 == (read_csr(mip) & MIP_MSIP));
    }
}
#endif

/**
 * Simple jump to application
 * @param mem_area
 */
void jump_to_application(HLS_DATA* hls, MODE_CHOICE mode_choice,
                         uint64_t next_addr)
{
    uint32_t hartid = read_csr(mhartid);
    /* Restore PLIC to known state: */
    __disable_irq();
    PLIC_init();

    /* Disable all interrupts: */
    write_csr(mie, 0);

    switch(mode_choice)
    {
        default:
        case M_MODE:
            /* User application execution should now start and never return
             * here.... */
            write_csr(mepc, next_addr);
            break;
        case S_MODE:
            /* User application execution should now start and never return
             * here.... */
            write_csr(mepc, next_addr);
            break;
    }

    register unsigned long a0 asm("a0") = hartid;
    register unsigned long a1 asm("a1") = (unsigned long)hls;
    __asm__ __volatile__("mret" : : "r"(a0), "r"(a1));
    __builtin_unreachable();
}

char info_string[100];

/* Main function for the HART0(E51 processor).
 * Application code running on HART0 is placed here.
 */
void e51(void)
{
    uint8_t rx_buff[20];
    uint8_t get_uart_rx = 0;

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART4, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);

    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();

    /* This mutex is used to serialize accesses to UART0 when all harts want to
     * TX/RX on UART0. This mutex is shared across all harts. */
    HART_SHARED_DATA* hart_share = (HART_SHARED_DATA *)hls->shared_mem;

    MSS_UART_init(g_uart, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS
                  | MSS_UART_NO_PARITY
                  | MSS_UART_ONE_STOP_BIT);

    /* set point for sharing across harts */
    hart_share->g_mss_uart0_lo = &g_mss_uart0_lo;
    /* Init spinlock mutex */
    hart_share->mutex_uart0 = 0U;

    sprintf(info_string,
            "\r\nHart %u, HLS mem address 0x%lx, Shared mem 0x%lx\r\n",
            hls->my_hart_id, (uint64_t)hls, (uint64_t)hls->shared_mem);
    spinlock(&hart_share->mutex_uart0);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,
                       (uint32_t)strlen(info_string));
    spinunlock(&hart_share->mutex_uart0);

    /*
     * Display system clocks measured values using the CFM
     */
    display_clocks();

#ifdef DDR_TEST
    /* write/read test */
    ddr_read_write_nc(small_ver);
#endif

    SysTick_Config();

    /* Construct info string and print to UART */
    sprintf(info_string, "MPFS HAL Version Major %d, Minor %d patch %d\r\n",
            MPFS_HAL_VERSION_MAJOR, MPFS_HAL_VERSION_MINOR,
            MPFS_HAL_VERSION_PATCH);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,
                       (uint32_t)strlen(info_string));

    MSS_UART_polled_tx_string(g_uart, menu_power_saving);

    /* Wake up hart 1 */
    raise_soft_interrupt(1U);

    /* Start the other harts with appropriate UART input from user */
    while (1)
    {
        get_uart_rx = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff,
                                                (uint32_t)sizeof(rx_buff));
        if (get_uart_rx++)
        {
            main_menu_options(rx_buff, get_uart_rx);
        }
    } /* End while(1) loop */
}

/* HART0 Software interrupt handler */
void Software_h0_IRQHandler(void)
{
    count_sw_ints_h0++;
}


static void main_menu_options(uint8_t rx_buff[], uint8_t get_uart_rx)
{
    switch(rx_buff[0])
    {
        default:
            MSS_UART_polled_tx_string(g_uart, menu_power_saving);
            break;
        case '0':
            MSS_UART_polled_tx_string(g_uart, menu_power_saving);
            break;
        case '1':
            /* 1  How to turn on RAM to Parked Hart RAM at bootup */
            MSS_UART_polled_tx_string(g_uart,
                                        msg_turn_on_park_hart_ram);
            break;
        case '2':
            /* 2  How to turn off RAM to Parked Hart RAM at bootup */
            MSS_UART_polled_tx_string(g_uart,
                                        msg_turn_off_park_hart_ram);
            break;
        case '3':
            /* 3  How to turn on U54 Floating Point Units(FPU) at
            bootup */
            MSS_UART_polled_tx_string(g_uart, msg_turn_on_fpu);
            break;
        case '4':
            /* 4  How to turn off U54 Floating Point Units(FPU) at
            bootup */
            MSS_UART_polled_tx_string(g_uart, msg_turn_off_fpu);
            break;
        case '5':
            /* 5  How to turn on RAM of Unused Peripherals at bootup */
            MSS_UART_polled_tx_string(g_uart,
                                        msg_turn_on_unused_perif_ram);
            break;
        case '6':
            /* 6  How to turn off RAM of Unused Peripherals at bootup */
            MSS_UART_polled_tx_string(g_uart,
                                        msg_turn_off_unused_perif_ram);
            mss_turn_off_unused_ram_clks();
            break;
        case '7':
            /* 7  Display DDR self refresh menu */
            MSS_UART_polled_tx_string(g_uart, display_menu_self_refresh);
            select_ddr_self_refresh_option();
            break;
        case '8':
            /* 8  Display clock scaling menu */
            MSS_UART_polled_tx_string(g_uart, display_menu_clock_scaling);
            select_clock_scaling_option();
            break;
    } /* End of switch statement */
}

#if defined(TARGET_RENODE)
#define TIMER_INCREMENT 35 /* Adjust this to get the C prompt from YMODEM giving
                            * one C every second or so */
#else
#define TIMER_INCREMENT 1
#endif

void SysTick_Handler_h0_IRQHandler(void)
{
    g_10ms_count += TIMER_INCREMENT;
    if (g_10ms_count < TIMER_INCREMENT)
        g_10ms_count = 0;
}
/**
 * Setup serial port if DDR debug required during start-up
 * @param uart Ref to uart you want to use
 * @return
 */
#ifdef DEBUG_DDR_INIT
__attribute__((section(".text_init")))\
uint32_t setup_ddr_debug_port(mss_uart_instance_t * uart)
{
#ifndef   DDR_BASE_BOARD
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
#else
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
#endif
    MSS_UART_init(uart, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS
                  | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    return (0U);
}
#endif


/**
 * Instantiated here to allow simple modification,
 * modify to test ddr writes as required
 * @return
 */
static void ddr_read_write_nc(uint32_t num_access)
{
    uint64_t mem_size = LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_1 + \
                        (LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_0 + 1U);
    uint32_t max_num_access = mem_size / 8U;

    if (num_access > max_num_access)
    {
        num_access = max_num_access;
    }

    setup_ddr_segments(DEFAULT_SEG_SETUP);

    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)
                              "\n\r             Accessing DDR Non Cached ");
    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");
    ddr_read_write_fn((uint64_t*)BASE_ADDRESS_NON_CACHED_64_DDR,
                      (uint32_t)num_access,SW_CONFIG_PATTERN);
    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)
                              "\n\r             Finished ");
    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");
    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)
                              "\n\r             Accessing DDR Cached ");
    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");
    ddr_read_write_fn((uint64_t*)LIBERO_SETTING_DDR_64_CACHE,
                      (uint32_t)num_access,SW_CONFIG_PATTERN);
    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)
                              "\n\r             Finished ");
    MSS_UART_polled_tx_string(g_uart,
                              (const uint8_t*)
                              "\n\n\r *************************"
                              "***************************** \n\r");

    setup_ddr_segments(LIBERO_SEG_SETUP);
}

/**
 * Instantiated here to allow simple modification,
 * modify to test interrupts as required
 * @return
 */
static uint8_t bus_error_unit(void)
{
    uint8_t hart_id;
    /* Init BEU in all harts - enable local interrupt */
    for(hart_id = MPFS_HAL_FIRST_HART; hart_id <= MPFS_HAL_LAST_HART; hart_id++)
    {
        BEU->regs[hart_id].ENABLE      = (uint64_t)0xFFU;
        BEU->regs[hart_id].PLIC_INT    = (uint64_t)0xFFU;
        BEU->regs[hart_id].LOCAL_INT   = (uint64_t)0x00U; //BEU_LOCAL_INT;
        BEU->regs[hart_id].CAUSE       = 0ULL;
        BEU->regs[hart_id].ACCRUED     = 0ULL;
        BEU->regs[hart_id].VALUE       = 0ULL;
    }
    return (0U);
}

/**
 * Reads the full DDR range, enables interrupts
 * (enable local or global in the bus_error_unit())
 * @return
 */
void ddr_read_full_range(void)
{
    volatile uint32_t value = 0U;
    uint32_t *address = (uint32_t *)BASE_ADDRESS_NON_CACHED_64_DDR;

    DDRCFG->ECC.CFG_ECC_1BIT_INT_THRESH.CFG_ECC_1BIT_INT_THRESH = 0xF;
    value = DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH;

    uint64_t mem_size = LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_1 + \
                        (LIBERO_SETTING_CFG_AXI_END_ADDRESS_AXI2_0 + 1U);

    PLIC_init();
    __enable_irq();

    PLIC_SetPriority(PLIC_ECC_ERROR_INT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_ECC_CORRECT_INT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_E51_BUS_ERROR_UNIT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_U54_1_BUS_ERROR_UNIT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_U54_2_BUS_ERROR_UNIT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_U54_3_BUS_ERROR_UNIT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_U54_4_BUS_ERROR_UNIT_OFFSET, 1U);

    PLIC_SetPriority(PLIC_L2_METADATA_CORR_INT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_L2_METADAT_UNCORR_INT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_L2_DATA_CORR_INT_OFFSET, 1U);
    PLIC_SetPriority(PLIC_L2_DATA_UNCORR_INT_OFFSET, 1U);
    PLIC_SetPriority(E51_0_MAINTENACE_INT_OFFSET, 1U);

    bus_error_unit();

    /* Enable Interrupts. */
    PLIC_EnableIRQ(PLIC_ECC_ERROR_INT_OFFSET);
    PLIC_EnableIRQ(PLIC_ECC_CORRECT_INT_OFFSET);
    PLIC_EnableIRQ(PLIC_E51_BUS_ERROR_UNIT_OFFSET);
    PLIC_EnableIRQ(PLIC_U54_1_BUS_ERROR_UNIT_OFFSET);
    PLIC_EnableIRQ(PLIC_U54_2_BUS_ERROR_UNIT_OFFSET);
    PLIC_EnableIRQ(PLIC_U54_3_BUS_ERROR_UNIT_OFFSET);
    PLIC_EnableIRQ(PLIC_U54_4_BUS_ERROR_UNIT_OFFSET);


    PLIC_EnableIRQ(PLIC_L2_METADATA_CORR_INT_OFFSET);
    PLIC_EnableIRQ(PLIC_L2_METADAT_UNCORR_INT_OFFSET);
    //PLIC_EnableIRQ(PLIC_L2_DATA_CORR_INT_OFFSET);
    //PLIC_EnableIRQ(PLIC_L2_DATA_UNCORR_INT_OFFSET);

    PLIC_EnableIRQ(E51_0_MAINTENACE_INT_OFFSET);

    setup_ddr_segments(DEFAULT_SEG_SETUP);

    while (((uint64_t)address & 0xFFFFFFFF) < mem_size)
    {
        value = *address;
        address++;
        if ((((uint64_t)address) & 0x3FFFULL) == 0U)
        {
            sprintf(info_string, "poll for ECC errors, address offset: \
                    %08llx\r\n", ((uint32_t)((uint64_t)(address)))
                    & 0xFFFFFFFFULL);
            MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,\
                               (uint32_t)strlen(info_string));
        }
    }

    (void)value;
    setup_ddr_segments(LIBERO_SEG_SETUP);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_E51_Maintence_IRQHandler(void)
{
    while (1);
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_ecc_error_IRQHandler(void)
{
    while (1);
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_ecc_correct_IRQHandler(void)
{
    while (1);
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_E51_bus_error_unit_IRQHandler(void)
{
    uint32_t hart_id = read_csr(mhartid);

    sprintf(info_string,"PLIC hart id : %x CAUSE : %lx ACCRUED : %lx VALUE :"
            " %lx STAT_INT_ECC_1BIT_THRESH : %x \r\n",
            hart_id, BEU->regs[hart_id].CAUSE,
            BEU->regs[hart_id].ACCRUED,
            BEU->regs[hart_id].VALUE,
            DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,
                       (uint32_t)strlen(info_string));
    /* Clear ECC interrupt */
    BEU->regs[hart_id].CAUSE       = 0ULL;
    BEU->regs[hart_id].ACCRUED     = 0ULL;
    BEU->regs[hart_id].VALUE       = 0ULL;
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_U54_1_bus_error_unit_IRQHandler(void)
{
    uint32_t hart_id = read_csr(mhartid);

    sprintf(info_string,"PLIC hart id : %x CAUSE : %lx ACCRUED :"
            " %lx VALUE : %lx STAT_INT_ECC_1BIT_THRESH : %x \r\n",
            hart_id, BEU->regs[hart_id].CAUSE, BEU->regs[hart_id].ACCRUED,
            BEU->regs[hart_id].VALUE,
            DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,
                       (uint32_t)strlen(info_string));
    /* Clear ECC interrupt */
    BEU->regs[hart_id].CAUSE       = 0ULL;
    BEU->regs[hart_id].ACCRUED     = 0ULL;
    BEU->regs[hart_id].VALUE       = 0ULL;
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_U54_2_bus_error_unit_IRQHandler(void)
{
    while (1);
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_U54_3_bus_error_unit_IRQHandler(void)
{
    while (1);
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_U54_4_bus_error_unit_IRQHandler(void)
{
    while (1);
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_l2_metadata_corr_IRQHandler(void)
{
    while (1);
    if (DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH != 0U)
        ;
    return (0U);
}

/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_l2_metadata_uncorr_IRQHandler(void)
{
    while (1);
    if (DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH != 0U)
        ;
    return (0U);
}

/**
 * Reading from unit ddr ends up here
 * @return
 */
uint8_t PLIC_l2_data_corr_IRQHandler(void)
{
    uint32_t hart_id = read_csr(mhartid);

    sprintf(info_string, "PLIC hart id : %x CAUSE : %lx ACCRUED : %lx VALUE :"
            " %lx STAT_INT_ECC_1BIT_THRESH : %x \r\n",
            hart_id, BEU->regs[hart_id].CAUSE,
            BEU->regs[hart_id].ACCRUED,
            BEU->regs[hart_id].VALUE,
            DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,
                       (uint32_t)strlen(info_string));
    if (DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH != 0U)
        ;
    return (0U);
}

/**
 * Instantiate to trap error during testing
 * @return
 */
uint8_t PLIC_l2_data_uncorr_IRQHandler(void)
{
    while (1);
    if (DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH != 0U)
        ;
    return (0U);
}


/**
 * Instantiate to trap error during testing
 * @return
 */
void handle_local_beu_interrupt(void)
{
    uint32_t hart_id = read_csr(mhartid);

    sprintf(info_string, "PLIC hart id : %x CAUSE : %lx ACCRUED : %lx VALUE :"
            " %lx STAT_INT_ECC_1BIT_THRESH : %x \r\n",
            hart_id, BEU->regs[hart_id].CAUSE,
            BEU->regs[hart_id].ACCRUED,
            BEU->regs[hart_id].VALUE,
            DDRCFG->ECC.STAT_INT_ECC_1BIT_THRESH.STAT_INT_ECC_1BIT_THRESH);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,
                       (uint32_t)strlen(info_string));

    /* Clear ECC interrupt */
    BEU->regs[hart_id].CAUSE       = 0ULL;
    BEU->regs[hart_id].ACCRUED     = 0ULL;
    BEU->regs[hart_id].VALUE       = 0ULL;
}

/**
 * Display mss registers
 * @return

 */
static void display_mss_regs(void)
{
    char info_string_regs[200];

    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)
                              "\n\rSCB MSS PLL Info\n\r");
    sprintf(info_string_regs, "SOFT RESET         = %08X\n\r",
            MSS_SCB_MSS_PLL->SOFT_RESET);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL CAL            = %08X  ",
            MSS_SCB_MSS_PLL->PLL_CAL);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL CTRL           = %08X\n\r",
            MSS_SCB_MSS_PLL->PLL_CTRL);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL CTRL2          = %08X  ",
            MSS_SCB_MSS_PLL->PLL_CTRL2);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL DIV 0 1        = %08X\n\r",
            MSS_SCB_MSS_PLL->PLL_DIV_0_1);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL DIV 2 3        = %08X  ",
            MSS_SCB_MSS_PLL->PLL_DIV_2_3);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL FRACN          = %08X\n\r",
            MSS_SCB_MSS_PLL->PLL_FRACN);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL PHADJ          = %08X  ",
            MSS_SCB_MSS_PLL->PLL_PHADJ);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "PLL REF FB         = %08X\n\r",
            MSS_SCB_MSS_PLL->PLL_REF_FB);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "SSCG REG 0         = %08X  ",
            MSS_SCB_MSS_PLL->SSCG_REG_0);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "SSCG REG 1         = %08X\n\r",
            MSS_SCB_MSS_PLL->SSCG_REG_1);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "SSCG REG 2         = %08X  ",
            MSS_SCB_MSS_PLL->SSCG_REG_2);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);
    sprintf(info_string_regs, "SSCG REG 3         = %08X\n\r",
            MSS_SCB_MSS_PLL->SSCG_REG_3);
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)info_string_regs);

}
/*
 * Enable (uncommment) the define MEASURED_UNUSED_PERIPHERAL_RAM below if you
 * want to measure power if unused peripherals are left on.
 */

/* #define MEASURED_UNUSED_PERIPHERAL_RAM */

#ifdef MEASURED_UNUSED_PERIPHERAL_RAM
void mss_turn_off_unused_ram_clks(void)
{
    (void);
    /*
     * Test function, so RAM is unused peripherals remain ON
     * Power difference can be measured
     */
}
#endif
