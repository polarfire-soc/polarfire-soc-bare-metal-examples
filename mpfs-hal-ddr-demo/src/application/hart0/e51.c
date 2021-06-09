/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
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
#include "inc/common.h"
#include "mpfs_hal/mss_hal.h"
#include "mpfs_hal/mpfs_hal_version.h"

#ifndef SIFIVE_HIFIVE_UNLEASHED
#include "drivers/mss_mmuart/mss_uart.h"
#include "../../middleware/ymodem/ymodem.h"
#else
#include "drivers/FU540_uart/FU540_uart.h"
#endif

/*******************************************************************************
 * Instruction message. This message will be transmitted over the UART to
 * HyperTerminal when the program starts.
 ******************************************************************************/
#ifdef DEBUG_DDR_INIT
const uint8_t g_message[] =
        "\r\n\
This program is run from E51\r\n\
This program can load a program to DDR using ymodem\r\n\
and has option to run the image using hart1 \r\n\
Type 0  Show this menu\r\n\
Type 1  Show ddr training values\r\n\
Type 2  Soft reset the MSS\r\n\
Type 3  Read/ write memory DDR tests\r\n\
Type 4  Display clock values\r\n\
Type 5  Print debug messages from hart0\r\n\
Type 6  load image to DDR\r\n\
Type 7  Start U54_1 running image in DDR\r\n\
Type 8  Start U54_2 running image in DDR\r\n\
Type 9  Start U54_3 running image in DDR\r\n\
Type a  Start U54_4 running image in DDR\r\n\
Type b  Display MSS PLL registers\r\n\
Type c  Load DDR test pattern and run.\r\n\
";
#else
const uint8_t g_message[] =
        "\r\n\r\n\r\n\
MPFS HAL DDR example version 0.3.3\r\n\
This program is run from E51\r\n\
This program can load a program to DDR using ymodem\r\n\
and has option to run the image using hart1 \r\n\
Type 0  Show this menu\r\n\
Type 1  Not used\r\n\
Type 2  Soft reset the MSS\r\n\
Type 3  Not used\r\n\
Type 4  Not used\r\n\
Type 5  Print debug messages from hart0\r\n\
Type 6  load image to DDR\r\n\
Type 7  Start U54_1 running image in DDR\r\n\
Type 8  Start U54_2 running image in DDR\r\n\
Type 9  Start U54_3 running image in DDR\r\n\
Type a  Start U54_4 running image in DDR\r\n\
Type b  Display MSS PLL registers\r\n\
";
#endif

#define twoGb 0x10000000UL  /* ((1024*1024*1024)*2)/sizeof(uint32_t) */
#define small_ver 0x00010000UL

#define OFFSET_BETWEEN_LINES    0x1000u

#define DDR_BASE            0x80000000u
#define DDR_SIZE            0x40000000u
#define CACHE_LINE_SIZE     0x40u

#define NB_CACHE_LINES_USED     (DDR_SIZE / OFFSET_BETWEEN_LINES)

#define MAX_BUFFER_SIZE 128

static volatile uint32_t count_sw_ints_h0 = 0U;
volatile uint32_t g_10ms_count;
uint8_t data_block[256];
uint64_t hart_jump_ddr  = 0U;
#ifndef DDR_BASE_BOARD
mss_uart_instance_t *g_uart= &g_mss_uart0_lo ;
mss_uart_instance_t *g_debug_uart= &g_mss_uart0_lo ;
#else
mss_uart_instance_t *g_uart= &g_mss_uart3_lo ;
mss_uart_instance_t *g_debug_uart= &g_mss_uart3_lo ;
#endif
volatile uint64_t uart_lock;
MEM_TYPE mem_area = E51_DDR_START_CACHE;
uint8_t *bin_base = (uint8_t *)LIBERO_SETTING_DDR_32_CACHE;
static uint8_t file_name[FILE_NAME_LENGTH + 1]; /* +1 for nul */

/*
 * external data
 */
extern uint64_t ddr_test;

/*
 * Local functions
 */
static void ddr_read_write_nc (uint32_t no_access);
static void display_clocks(void);
static void display_mss_regs(void);

/*
 * Extern functions
 */
#ifdef DEBUG_DDR_INIT
extern uint32_t tip_register_status (mss_uart_instance_t *g_mss_uart_debug_pt);
#endif


/**
 * Simple jump to application
 * @param mem_area
 */
void jump_to_application( MEM_TYPE mem_area_choice)
{
    /* Restore PLIC to known state: */
    __disable_irq();
    PLIC_init();

    /* Disable all interrupts: */
    write_csr(mie, 0);

    switch(mem_area_choice)
    {
        default:
        case E51_DDR_START_NON_CACHE:
            /* Start executing from the top of DDR memory: */
            __asm volatile("lui ra,0x0C000");
            __asm volatile("slli ra, ra,0x4");
            break;
        case E51_DDR_START_CACHE:
            /* Start executing from the top of DDR memory: */
            __asm volatile("lui ra,0x08000");
            __asm volatile("slli ra, ra,0x4");
            break;
    }
    __asm volatile("ret");
    /* User application execution should now start and never return here.... */
}

/* Main function for the HART0(E51 processor).
 * Application code running on HART0 is placed here.
 */
void e51(void)
{
    char info_string[100];
    uint64_t mcycle_start = 0U;
    uint64_t mcycle_end = 0U;
    uint64_t delta_mcycle = 0U;
    uint8_t rx_buff[1];
    uint64_t hartid = read_csr(mhartid);
    uint8_t rx_size = 0;
    uint8_t debug_hart0 = 0U;
    volatile uint32_t  received = 0;
    uint32_t g_rx_size = 1024 * 128;

    /* Turn on UART0 clock */
    SYSREG->SUBBLK_CLOCK_CR |= (SUBBLK_CLOCK_CR_MMUART0_MASK |\
            SUBBLK_CLOCK_CR_MMUART1_MASK |\
            SUBBLK_CLOCK_CR_MMUART2_MASK |\
            SUBBLK_CLOCK_CR_MMUART3_MASK |\
            SUBBLK_CLOCK_CR_MMUART4_MASK |\
            SUBBLK_CLOCK_CR_CFM_MASK);
    /* Remove soft reset */
    SYSREG->SOFT_RESET_CR   &= (uint32_t)~(SUBBLK_CLOCK_CR_MMUART0_MASK |\
            SUBBLK_CLOCK_CR_MMUART1_MASK |\
            SUBBLK_CLOCK_CR_MMUART2_MASK |\
            SUBBLK_CLOCK_CR_MMUART3_MASK |\
            SUBBLK_CLOCK_CR_MMUART4_MASK |\
            SUBBLK_CLOCK_CR_CFM_MASK);

    /* This mutex is used to serialize accesses to UART0 when all harts want to
     * TX/RX on UART0. This mutex is shared across all harts. */
    mss_init_mutex((uint64_t)&uart_lock);

    MSS_UART_init( g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    /*
     * Display system clocks measured values using the CFM
     */
    display_clocks();

//#define debug_uarts
#ifdef debug_uarts
    MSS_UART_init( &g_mss_uart1_lo,
        MSS_UART_115200_BAUD,
        MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_init( &g_mss_uart2_lo,
        MSS_UART_115200_BAUD,
        MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_init( &g_mss_uart3_lo,
        MSS_UART_115200_BAUD,
        MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_init( &g_mss_uart4_lo,
        MSS_UART_115200_BAUD,
        MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
#endif

#ifdef debug_uarts
    MSS_UART_polled_tx_string (&g_mss_uart1_lo, g_message);
    MSS_UART_polled_tx_string (&g_mss_uart2_lo, g_message);
    MSS_UART_polled_tx_string (&g_mss_uart3_lo, g_message);
    MSS_UART_polled_tx_string (&g_mss_uart4_lo, g_message);
#endif

    SysTick_Config();

#define DDR_TEST
#ifdef DDR_TEST
    /* write/read test */
    ddr_read_write_nc (small_ver);
#endif

    MSS_UART_polled_tx_string (g_uart, g_message);

//#define SOFT_RESET_TEST
#ifdef SOFT_RESET_TEST
    /* Implement a soft reset */
    SYSREG->MSS_RESET_CR = 0xdead;
#endif

    /* Start the other harts with appropriate UART input from user */
    while (1)
    {
        mcycle_start = readmcycle();

        if (1u == debug_hart0)
        {
          debug_hart0 = 0U;
          sprintf(info_string,"Hart %ld, %ld delta_mcycle %ld mtime\r\n",
          hartid, delta_mcycle, readmtime());
          MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
        }

        rx_size = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff, (uint32_t)sizeof(rx_buff));

        if (rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case '0':
                    sprintf(info_string,"MPFS HAL Version Major %d, Minor %d patch %d\r\n",\
                             MPFS_HAL_VERSION_MAJOR,MPFS_HAL_VERSION_MINOR, MPFS_HAL_VERSION_PATCH);
                    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
                    MSS_UART_polled_tx_string (g_uart, g_message );
                    break;
                case '1':
#ifdef DEBUG_DDR_INIT
                    tip_register_status (g_uart);
#endif
                    break;
                case '2':
                    MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"The MSS will now reset\n\r" );
                    SYSREG->MSS_RESET_CR = 0xdead;
                    break;
                case '3':
#ifdef DEBUG_DDR_INIT
                    ddr_read_write_nc (twoGb);
#endif
                    break;
                case '4':
                    display_clocks();
                    break;
                case '5':
                    debug_hart0 = 1;
                    break;
                case '6':
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\r\nsend .bin file using ymodem\r\n");
                    bin_base = (uint8_t *)LIBERO_SETTING_DDR_32_CACHE;
                    received = ymodem_receive(bin_base, g_rx_size, file_name);
                    if(received != 0U)
                    {
                        MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\n\rNow choose option 7,8,9 or a to run loaded program\n\r" );
                    }
                    else
                    {
                        MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\n\rfile receive failed\n\r" );
                    }
                    break;
                case '7':
                    raise_soft_interrupt(1u);
                    hart_jump_ddr = 1U;
#ifdef E51_ENTER_SLEEP_STATE
                    SysTick_off();
                    loop_in_dtim();
#endif
                    break;
                case '8':
                    raise_soft_interrupt(2u);
                    hart_jump_ddr = 2U;
#ifdef E51_ENTER_SLEEP_STATE
                    SysTick_off();
                    loop_in_dtim();
#endif
                    break;
                case '9':
                    raise_soft_interrupt(3u);
                    hart_jump_ddr = 3U;
#ifdef E51_ENTER_SLEEP_STATE
                    SysTick_off();
                    loop_in_dtim();
#endif
                    break;
                case 'a':
                    raise_soft_interrupt(4u);
                    hart_jump_ddr = 4U;
#ifdef E51_ENTER_SLEEP_STATE
                    SysTick_off();
                    loop_in_dtim();
#endif
                    break;
                case 'b':
                    display_mss_regs();
                    break;
                case 'c':
                    raise_soft_interrupt(1u);
                    ddr_test = 1U;
                    break;
                case 'x':
                	ddr_test = 2U;
                    break;

                default:
                    /* echo input */
                    MSS_UART_polled_tx_string(g_uart, rx_buff);
                    break;
            }
        }

        mcycle_end    = readmcycle();
        delta_mcycle = mcycle_end - mcycle_start;
    }
}

/* HART0 Software interrupt handler */
void Software_h0_IRQHandler(void)
{
    count_sw_ints_h0++;
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
    if(g_10ms_count < TIMER_INCREMENT)
        g_10ms_count = 0;
}

/**
 * idle in DTIM
 * Note. Switch code is copied to DTIM on start-up.
 */
#ifdef LOOP_IN_DTIM
__attribute__((section(".ram_codetext"))) \
        static void loop_in_dtim(void)
{
    /*Put this hart into WFI.*/
    while(1U)
    {
        do
        {
            __asm("wfi");
        }while(0 == (read_csr(mip) & MIP_MSIP));
    }
}
#endif

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
    /* Turn on UART0 clock */
    SYSREG->SUBBLK_CLOCK_CR |= (SUBBLK_CLOCK_CR_MMUART0_MASK);
    /* Remove soft reset */
    SYSREG->SOFT_RESET_CR   &= (uint32_t)~(SUBBLK_CLOCK_CR_MMUART0_MASK);
#else
    /* Turn on UART3 clock */
    SYSREG->SUBBLK_CLOCK_CR |= (SUBBLK_CLOCK_CR_MMUART3_MASK);
    /* Remove soft reset */
    SYSREG->SOFT_RESET_CR   &= (uint32_t)~(SUBBLK_CLOCK_CR_MMUART3_MASK);
#endif
    MSS_UART_init( uart,
        MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    return(0U);
}
#endif


/**
 *
 */
static void ddr_read_write_nc (uint32_t no_access)
{

    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\r             Accessing 2Gb DDR Non Cached ");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");
    //ddr_read_write_fn((uint64_t*)LIBERO_SETTING_DDR_64_NON_CACHE,(uint32_t)no_access,SW_CONFIG_PATTERN);
    ddr_read_write_fn((uint64_t*)LIBERO_SETTING_DDR_32_CACHE,(uint32_t)no_access/8,SW_CONFIG_PATTERN);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\r             Finished ");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");

    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\r             Accessing 2Gb DDR Cached ");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");
    //ddr_read_write_fn((uint64_t*)LIBERO_SETTING_DDR_64_CACHE,(uint32_t)no_access,SW_CONFIG_PATTERN);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\r             Finished ");
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\n\r ****************************************************** \n\r");
}

/*==============================================================================
 *
 */
static void display_clocks(void)
{
    char info_string[200];
    uint32_t count0;
    cfmChannelMode chMode;

    memset(&chMode, 0, sizeof(chMode));

    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\rSystem Clock Frequencies:\n\r");

    MSS_CLF_clk_configuration(0x1, 1, 0, 0x0, 0x1); /* group 1, ref clkis clk in rerf here it is 125 Mhz */
    CFM_REG->clkselReg &= ~(0x1U << CFM_CLK_MONSEL_SHIFT);
    MSS_CFM_runtime_register(0x2710);
    chMode.channel0 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;

    sprintf(info_string,"CPU clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    MSS_CFM_runtime_register(10000U);
    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string,"AXI clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string,"AHB clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string,"Reference clock   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string,"DFI clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    /* group 2, ref clkis clk in rerf here it is 125 Mhz */
    MSS_CLF_clk_configuration(0x2, 1, 0, 0x0, 0x1);
    chMode.channel0 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;
    sprintf(info_string,"MAC TX clock      = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string,"MAC TSU clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string,"MAC0 RX clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string,"MAC1 RX clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string,"SGMII CLK C OUT   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    MSS_CLF_clk_configuration(0x4, 1, 0, 0x0, 0x1);   /* group 4, ref clkis clk in rerf here it is 125 Mhz */
    chMode.channel0 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;
    sprintf(info_string,"SGMII PLL clock 0 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string,"SGMII PLL clock 1 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string,"SGMII PLL clock 2 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string,"SGMII PLL clock 3 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string,"SGMII DLL clock   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    MSS_CLF_clk_configuration(0x5, 1, 0, 0x0, 0x1);   /* group 5, ref clkis clk in rerf here it is 125 Mhz */
    chMode.channel2 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string,"FAB MAC0 TSU clk  = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string,"FAB MAC1 TSU clk  = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
}


/**
 *
 */
static void display_mss_regs(void)
{
    char info_string[200];

    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\rSCB MSS PLL Info\n\r");
    sprintf(info_string,"SOFT RESET         = %08X\n\r", MSS_SCB_MSS_PLL->SOFT_RESET);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL CAL            = %08X  ", MSS_SCB_MSS_PLL->PLL_CAL);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL CTRL           = %08X\n\r", MSS_SCB_MSS_PLL->PLL_CTRL);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL CTRL2          = %08X  ", MSS_SCB_MSS_PLL->PLL_CTRL2);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL DIV 0 1        = %08X\n\r", MSS_SCB_MSS_PLL->PLL_DIV_0_1);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL DIV 2 3        = %08X  ", MSS_SCB_MSS_PLL->PLL_DIV_2_3);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL FRACN          = %08X\n\r", MSS_SCB_MSS_PLL->PLL_FRACN);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL PHADJ          = %08X  ", MSS_SCB_MSS_PLL->PLL_PHADJ);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"PLL REF FB         = %08X\n\r", MSS_SCB_MSS_PLL->PLL_REF_FB);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"SSCG REG 0         = %08X  ", MSS_SCB_MSS_PLL->SSCG_REG_0);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"SSCG REG 1         = %08X\n\r", MSS_SCB_MSS_PLL->SSCG_REG_1);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"SSCG REG 2         = %08X  ", MSS_SCB_MSS_PLL->SSCG_REG_2);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);
    sprintf(info_string,"SSCG REG 3         = %08X\n\r", MSS_SCB_MSS_PLL->SSCG_REG_3);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string);

}


