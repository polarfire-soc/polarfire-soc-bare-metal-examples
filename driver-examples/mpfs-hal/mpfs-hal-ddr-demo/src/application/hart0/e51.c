/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
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

#ifndef SIFIVE_HIFIVE_UNLEASHED
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
"\r\n\r\n\r\n\
This program is run from E51\r\n\
\r\n\
DDR test options:\r\n\
1  Show DDR training values\r\n\
2  Soft reset the MSS\r\n\
3  Read/write memory - DDR tests\r\n\
4  Display clock values\r\n\
5  Load DDR test pattern and run.\r\n\
b  Display MSS PLL registers\r\n\
\r\n\
Bootloader options:\r\n\
6  Load image to DDR using YMODEM\r\n\
7  Start U54_1 from DDR @0x80000000\r\n\
8  Start U54 1 & 2 from DDR @0x80000000\r\n\
9  Start U54 1, 2 & 3 from DDR @0x80000000\r\n\
a  Start all U54s from DDR @0x80000000 \r\n\
\r\n\
Type 0  to show this menu again\r\n\
";
#else
const uint8_t g_message[] =
"\r\n\r\n\r\n\
This program is run from E51\r\n\
\r\n\
DDR test options:\r\n\
1  Not used\r\n\
2  Soft reset the MSS\r\n\
3  Not used\r\n\
4  Display clock values\r\n\
5  Not used\r\n\
b  Display MSS PLL registers\r\n\
\r\n\
Bootloader options:\r\n\
6  Load image to DDR using YMODEM\r\n\
7  Start U54_1 from DDR @0x80000000\r\n\
8  Start U54 1 & 2 from DDR @0x80000000\r\n\
9  Start U54 1, 2 & 3 from DDR @0x80000000\r\n\
a  Start all U54s from DDR @0x80000000 \r\n\
\r\n\
Type 0  to show this menu again\r\n\
";
#endif

#define twoGb 0x10000000UL  /* ((1024*1024*1024)*2)/sizeof(uint32_t) */
#define small_ver 0x00010000UL

#define OFFSET_BETWEEN_LINES    0x1000u

#define DDR_BASE            0x80000000u
#define DDR_SIZE            0x40000000u
#define CACHE_LINE_SIZE     0x40u
#define MIN_OFFSET          1U
#define MAX_OFFSET          16U

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

uint8_t *bin_base = (uint8_t *)DDR_BASE;
static uint8_t file_name[FILE_NAME_LENGTH + 1]; /* +1 for nul */
uint32_t uart0_mutex;

#define NO_OF_ITERATIONS    2
uint32_t pattern_offset = 12U;

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
 * idle in DTIM
 */
#ifdef E51_ENTER_SLEEP_STATE
__attribute__((section(".ram_codetext"))) static void loop_in_dtim(void)
{
    mb();
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
 * Simple jump to application
 * @param mem_area
 */
void jump_to_application(HLS_DATA* hls, MODE_CHOICE mode_choice, uint64_t next_addr)
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
            /* User application execution should now start and never return here.... */
            write_csr(mepc, next_addr);
            break;
        case S_MODE:
            /* User application execution should now start and never return here.... */
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
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;
    volatile uint32_t  received = 0;
    uint32_t g_rx_size = 1024 * 128 * 2;

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART2, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART3, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART4, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CFM, (uint8_t) MPFS_HAL_FIRST_HART, PERIPHERAL_ON);

    HLS_DATA* hls = (HLS_DATA*)(uintptr_t)get_tp_reg();
    /* This mutex is used to serialize accesses to UART0 when all harts want to
     * TX/RX on UART0. This mutex is shared across all harts. */
    HART_SHARED_DATA * hart_share = (HART_SHARED_DATA *)hls->shared_mem;

    MSS_UART_init( g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /* set point for sharing across harts */
    hart_share->g_mss_uart0_lo = &g_mss_uart0_lo;

    sprintf(info_string, "\r\nHart %u, HLS mem address 0x%lx, Shared mem 0x%lx\r\n",\
                                                          hls->my_hart_id, (uint64_t)hls, (uint64_t)hls->shared_mem);
    spinlock(&hart_share->mutex_uart0);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,(uint32_t)strlen(info_string));
    spinunlock(&hart_share->mutex_uart0);

    /*
     * Display system clocks measured values using the CFM
     */
    display_clocks();

#define DDR_TEST
#ifdef DDR_TEST
    /* write/read test */
    ddr_read_write_nc (small_ver);
#endif

    SysTick_Config();

    sprintf(info_string,"MPFS HAL Version Major %d, Minor %d patch %d\r\n",\
             MPFS_HAL_VERSION_MAJOR,MPFS_HAL_VERSION_MINOR, MPFS_HAL_VERSION_PATCH);
    MSS_UART_polled_tx(g_uart, (const uint8_t*)info_string,(uint32_t)strlen(info_string));

    MSS_UART_polled_tx_string (g_uart, g_message);

    /* Start the other harts with appropriate UART input from user */
    while (1)
    {
        rx_size = (uint8_t)MSS_UART_get_rx(g_uart, (uint8_t*)rx_buff, (uint32_t)sizeof(rx_buff));

        if (rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case '0':
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
#ifdef DEBUG_DDR_INIT
                    /*
                     * U54-1 runs the test program
                     * It writes a test pattern to DDR. X exits the program.
                     * Status displayed on hart0
                     */
                    ddr_test = 1U;
                    raise_soft_interrupt(1u);
#endif
                    break;
                case '6':
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\r\nSend .bin file using YMODEM\r\n");
                    bin_base = (uint8_t *)DDR_BASE;
                    received = ymodem_receive(bin_base, g_rx_size, file_name);
                    if(received != 0U)
                    {
                        MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\n\rNow choose option 7,8,9 or 'a' key to run loaded program. E51 will go into WFI after starting the U54(s)\n\r\n\r" );
                    }
                    else
                    {
                        MSS_UART_polled_tx_string (g_uart,(const uint8_t*)"\n\rfile receive failed\n\r" );
                    }
                    break;
                case '7':
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"We are leaving the boot loader\r\n");
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"to run in loaded DDR program\r\n");
                    raise_soft_interrupt(1u);
                    hart_jump_ddr = 1U;
#ifdef E51_ENTER_SLEEP_STATE
                    disable_systick();
                    loop_in_dtim();
#endif
                    break;
                case '8':
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"We are leaving the boot loader\r\n");
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"to run in loaded DDR program\r\n");
                    raise_soft_interrupt(1u);
                    raise_soft_interrupt(2u);
                    hart_jump_ddr = 12U;
#ifdef E51_ENTER_SLEEP_STATE
                    disable_systick();
                    loop_in_dtim();
#endif
                    break;
                case '9':
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"We are leaving the boot loader\r\n");
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"to run in loaded DDR program\r\n");
                    raise_soft_interrupt(1u);
                    raise_soft_interrupt(2u);
                    raise_soft_interrupt(3u);
                    hart_jump_ddr = 123U;
#ifdef E51_ENTER_SLEEP_STATE
                    disable_systick();
                    loop_in_dtim();
#endif
                    break;
                case 'a':
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"We are leaving the boot loader\r\n");
                    MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"to run in loaded DDR program\r\n");
                    raise_soft_interrupt(1u);
                    raise_soft_interrupt(2u);
                    raise_soft_interrupt(3u);
                    raise_soft_interrupt(4u);
                    hart_jump_ddr = 1234U;
#ifdef E51_ENTER_SLEEP_STATE
                    disable_systick();
                    loop_in_dtim();
#endif
                    break;
                case 'b':
                    display_mss_regs();
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
    char info_string_clks[200];
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

    sprintf(info_string_clks,"CPU clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    MSS_CFM_runtime_register(10000U);
    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string_clks,"AXI clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks,"AHB clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks,"Reference clock   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string_clks,"DFI clock         = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    /* group 2, ref clkis clk in rerf here it is 125 Mhz */
    MSS_CLF_clk_configuration(0x2, 1, 0, 0x0, 0x1);
    chMode.channel0 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;
    sprintf(info_string_clks,"MAC TX clock      = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string_clks,"MAC TSU clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks,"MAC0 RX clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks,"MAC1 RX clock     = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string_clks,"SGMII CLK C OUT   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    MSS_CLF_clk_configuration(0x4, 1, 0, 0x0, 0x1);   /* group 4, ref clkis clk in rerf here it is 125 Mhz */
    chMode.channel0 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_0, &count0))
        ;
    sprintf(info_string_clks,"SGMII PLL clock 0 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel0 = 0;
    chMode.channel1 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_1, &count0))
        ;
    sprintf(info_string_clks,"SGMII PLL clock 1 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel1 = 0;
    chMode.channel2 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks,"SGMII PLL clock 2 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks,"SGMII PLL clock 3 = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel3 = 0;
    chMode.channel4 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_4, &count0))
        ;
    sprintf(info_string_clks,"SGMII DLL clock   = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    MSS_CLF_clk_configuration(0x5, 1, 0, 0x0, 0x1);   /* group 5, ref clkis clk in rerf here it is 125 Mhz */
    chMode.channel2 = 1;
    chMode.channel4 = 0;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_2, &count0))
        ;
    sprintf(info_string_clks,"FAB MAC0 TSU clk  = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);

    chMode.channel2 = 0;
    chMode.channel3 = 1;
    MSS_CFM_channel_mode(chMode);
    MSS_CFM_control_start();
    while(ERROR_INVALID_CFM_BUSY == MSS_CFM_get_count(CFM_COUNT_3, &count0))
        ;
    sprintf(info_string_clks,"FAB MAC1 TSU clk  = %uHz\n\r", count0 * 12500U);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_clks);
}


/**
 *
 */
static void display_mss_regs(void)
{
    char info_string_regs[200];

    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)"\n\rSCB MSS PLL Info\n\r");
    sprintf(info_string_regs,"SOFT RESET         = %08X\n\r", MSS_SCB_MSS_PLL->SOFT_RESET);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL CAL            = %08X  ", MSS_SCB_MSS_PLL->PLL_CAL);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL CTRL           = %08X\n\r", MSS_SCB_MSS_PLL->PLL_CTRL);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL CTRL2          = %08X  ", MSS_SCB_MSS_PLL->PLL_CTRL2);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL DIV 0 1        = %08X\n\r", MSS_SCB_MSS_PLL->PLL_DIV_0_1);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL DIV 2 3        = %08X  ", MSS_SCB_MSS_PLL->PLL_DIV_2_3);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL FRACN          = %08X\n\r", MSS_SCB_MSS_PLL->PLL_FRACN);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL PHADJ          = %08X  ", MSS_SCB_MSS_PLL->PLL_PHADJ);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"PLL REF FB         = %08X\n\r", MSS_SCB_MSS_PLL->PLL_REF_FB);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"SSCG REG 0         = %08X  ", MSS_SCB_MSS_PLL->SSCG_REG_0);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"SSCG REG 1         = %08X\n\r", MSS_SCB_MSS_PLL->SSCG_REG_1);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"SSCG REG 2         = %08X  ", MSS_SCB_MSS_PLL->SSCG_REG_2);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);
    sprintf(info_string_regs,"SSCG REG 3         = %08X\n\r", MSS_SCB_MSS_PLL->SSCG_REG_3);
    MSS_UART_polled_tx_string(g_uart,(const uint8_t*)info_string_regs);

}


