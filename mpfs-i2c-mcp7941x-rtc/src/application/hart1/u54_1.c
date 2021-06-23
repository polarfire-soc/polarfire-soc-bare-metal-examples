/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * Example project demonstrating the use of MCP7941x Real-time Clock driver
 * Please refer to the README.md file for further info.
 */

#include <stdio.h>
#include <string.h>
#include <sys/_stdint.h>
#include "mpfs_hal/common/mss_peripherals.h"
#include "mpfs_hal/common/mss_plic.h"
#include "mpfs_hal/common/mss_util.h"
#include "drivers/mss/mss_i2c/mss_i2c.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "drivers/off-chip/mcp7941x/mcp7941x.h"

#define RX_BUFF_SIZE    6U
#define P_BUFF_SIZE     20U

mss_uart_instance_t *g_uart = &g_mss_uart1_lo;

/******************************************************************************
 * Instruction message. This message will be transmitted over the UART when
 * the program starts.
 *****************************************************************************/

const uint8_t g_draw_bar[] =
 "\r\n********************************************************************************\r\n";

const uint8_t g_draw_name[] =
"\r\n*               ------------------------------------              *\r\n\
+++++++++++++++|      Reading MCP7941x over I2C     |++++++++++++++\r\n\
*               ------------------------------------              *\r\n";


const uint8_t g_message1[] =
"\r\n\r\n\r\n **** PolarFire SoC ICicle kit RTC click (MCP7941x) click example ****\r\n\r\n\r\n";

const uint8_t g_message2[] =
        "This program is running from u54_1. By default, the battery backup is enable i.e \r\n\
        Register values will get incremented without VCC.\r\n\
Type 0  get current local time and date \r\n\
Type 1  get current gmt time and date \r\n\
Type 2  set current gmt time and date \r\n\
Type 3  sram write read transaction test.  \r\n\
Type 4  Show this menu\r\n\
 ";

const uint8_t g_message3[] =
        "\r\nThis application is running from u54_1.\r\n\
        please see the interface1 or mmuart1 for options. \r\n";

uint8_t g_rx_size = 0x00U;

uint8_t g_rx_buff[RX_BUFF_SIZE] = {0x00U};

/* Created for convenience */

static void print_date_time(mcp7941x_time_t * cur_time);

static void draw_bar(void);

static void draw_name(void);

void u54_1 (void)
{

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)

    /*Put this hart into WFI.*/

    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();
#endif
  __enable_irq();
    PLIC_init();


    /*Reset */

    (void) mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
    (void) mss_config_clk_rst(MSS_PERIPH_I2C0, (uint8_t) 1, PERIPHERAL_ON);



    MSS_UART_init( &g_mss_uart1_lo,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /*print greeting messages on serial port. */

    MSS_UART_polled_tx(g_uart,g_message1,sizeof(g_message1));
    MSS_UART_polled_tx(g_uart,g_message2,sizeof(g_message2));

    /* init function */
    /* setting offset of +5h 30m from GMT time i.e IST(Indian STD time ) */

    mcp7941x_init(&g_mss_i2c0_lo,5,30);

    mcp7941x_time_t* cur_time ;
    mcp7941x_time_t set_time;


    /*application loop */

    while(1)
    {
        g_rx_size = MSS_UART_get_rx(g_uart,g_rx_buff,sizeof(g_rx_buff));

        if( 1 == g_rx_size){

            switch(g_rx_buff[0]){

            case '0':

                cur_time = mcp7941x_get_local_time();
                print_date_time(cur_time);
                g_rx_size = 0u;
                break ;

            case '1':

                cur_time = mcp7941x_get_gmt_time();
                print_date_time(cur_time);
                g_rx_size = 0u ;
                break;

            case '2' :

                set_time.seconds = 00u;
                set_time.minutes = 9u;
                set_time.hours = 18u;
                set_time.weekday = 4u;
                set_time.monthday = 29u ;
                set_time.year = 21u ;
                set_time.month = 4u;

                draw_bar();
                MSS_UART_polled_tx_string(g_uart,
                        (const uint8_t*)"          UPDATED DATE & TIME\r\n");
                draw_bar();
                mcp7941x_set_gmt_time(set_time);
                print_date_time(&set_time);
                g_rx_size = 0u;

                break ;

            case '3' :

                draw_bar();
                MSS_UART_polled_tx_string(g_uart,
                        (uint8_t *)"performing Sram write and read operation\r\n");
                draw_bar();

                uint8_t send_val = 0x33u;
                uint8_t rec_val;
                mcp7941x_write_sram(0x20u,send_val);
                rec_val = mcp7941x_read_sram(0x20u);

                if( rec_val == send_val)
                {
                    draw_bar();
                    MSS_UART_polled_tx_string(g_uart,
                            (uint8_t*)" Sram transaction successful \r\n");
                }
                else
                {
                    draw_bar();
                    MSS_UART_polled_tx_string(g_uart,
                            (uint8_t *)"performing Sram write and read operation\r\n");
                    draw_bar();
                }

                g_rx_size = 0u;
                break;

            case '4' :

                MSS_UART_polled_tx(g_uart,g_message2,sizeof(g_message2));
                break;

            default :
                MSS_UART_polled_tx_string(g_uart,
                        (uint8_t *)"Please choose a valid option\r\n");
                g_rx_size = 0u;

            }
        }
    }

}

void draw_bar(void)
{
        /* Draw bar */

    MSS_UART_polled_tx_string(g_uart, g_draw_bar);
}

void draw_name(void)
{
    MSS_UART_polled_tx_string(g_uart,g_draw_name);
}

void print_date_time(mcp7941x_time_t * cur_time)
{
    draw_name();
    uint8_t p_buff[P_BUFF_SIZE] ;

    /*reseting the buffer to resolve uart output issue. */
        
    memset(p_buff,0u,sizeof(p_buff));
    sprintf(p_buff,"seconds : %d\r\n",cur_time->seconds);
    MSS_UART_polled_tx(g_uart,p_buff,sizeof(p_buff));

    memset(p_buff,0u,sizeof(p_buff));
    sprintf(p_buff,"minutes : %d \r\n",cur_time->minutes);
    MSS_UART_polled_tx(g_uart,p_buff,sizeof(p_buff));

    memset(p_buff,0u,sizeof(p_buff));
    sprintf(p_buff,"hours : %d \r\n",cur_time->hours);
    MSS_UART_polled_tx(g_uart,p_buff,sizeof(p_buff));

    memset(p_buff,0u,sizeof(p_buff));
    sprintf(p_buff,"weekday : %d\r\n",cur_time->weekday);
    MSS_UART_polled_tx(g_uart,p_buff,sizeof(p_buff));

    memset(p_buff,0u,sizeof(p_buff));
    sprintf(p_buff,"monthday : %d\r\n",cur_time->monthday);
    MSS_UART_polled_tx(g_uart,p_buff,sizeof(p_buff));

    memset(p_buff,0u,sizeof(p_buff));
    sprintf(p_buff,"month : %d\r\n",cur_time->month);
    MSS_UART_polled_tx(g_uart,p_buff,sizeof(p_buff));

    memset(p_buff,0u,sizeof(p_buff));
    sprintf(p_buff,"year : %d \r\n",cur_time->year);
    MSS_UART_polled_tx(g_uart,p_buff,sizeof(p_buff));

}
