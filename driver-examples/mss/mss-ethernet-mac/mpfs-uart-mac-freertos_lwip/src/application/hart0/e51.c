/***********************************************************************************
 * Copyright 2019 Microchip Corporation.
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
 * code running on e51
 *
 * SVN $Revision: 9661 $
 * SVN $Date: 2018-01-15 10:43:33 +0000 (Mon, 15 Jan 2018) $
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mpfs_hal/mss_hal.h"

#if PSE
#include "drivers/mss/mss_gpio/mss_gpio.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#else
#include "drivers/FU540_uart/FU540_uart.h"
#endif

#include "drivers/mss/mss_ethernet_mac/mss_ethernet_registers.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_sw_cfg.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac_regs.h"
#include "drivers/mss/mss_ethernet_mac/mss_ethernet_mac.h"

#include "drivers/mss/mss_ethernet_mac/phy.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "lwip/sockets.h"
#include "WebServer/if_utils.h"

typedef socklen_t SOCKLEN_T;
#include "lwip/def.h"
#include "lwip/inet.h"
/* lwIP includes. */
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"

#include "inc/common.h"

void http_server_netconn_thread(void *arg);
#if PSE
void dump_vsc8575_regs(mss_mac_instance_t *this_mac);
#endif

#if 0
#define MY_STATIC_IP_ADDRESS "192.168.0.30"
#define MY_STATIC_IP_GATEWAY "192.168.0.1"
#define MY_STATIC_IP_MASK    "255.255.255.0"
#else
#define MY_STATIC_IP_ADDRESS "192.168.128.249"
#define MY_STATIC_IP_GATEWAY "192.168.128.252"
#define MY_STATIC_IP_MASK    "255.255.255.0"
#endif

static uint8_t g_mac_tx_buffer[MSS_MAC_TX_RING_SIZE][MSS_MAC_MAX_TX_BUF_SIZE]
    __attribute__((aligned(4)));
static uint8_t g_mac_rx_buffer[MSS_MAC_RX_RING_SIZE][MSS_MAC_MAX_RX_BUF_SIZE]
    __attribute__((aligned(4)));

mss_mac_cfg_t g_mac_config;

/*==============================================================================
 * Network configuration globals.
 */

ntm_dhcp_mode_t g_network_addr_mode = NTM_DHCP_FIXED; /* Start off with fixed mode */

ip4_addr_t g_ip_address; /* Configured fixed/default address */
ip4_addr_t g_ip_mask; /* Configured fixed/default mask */
ip4_addr_t g_ip_gateway; /* Configured fixed/default gateway address */

typedef struct aligned_tx_buf
{
    uint64_t aligner;
    uint8_t packet[2000];
} ALIGNED_TX_BUF;

ALIGNED_TX_BUF tx_packet;
uint8_t tx_packet_data[60] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x06, 0x00,
    0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0xEC, 0x08, 0x6B, 0xE2, 0xCA, 0x17, 0xC0, 0xA8,
    0x80, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x80, 0xFC, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static volatile int tx_count = 0;
static volatile int rx_count = 0;

#if 1
/*==============================================================================
 *
 */
/* lwIP MAC configuration. */
struct netif g_EMAC_if;

/*==============================================================================
 *
 */
uint32_t
get_ipv4_address(int32_t select)
{
    uint32_t ret_val;

    switch (select)
    {
        case IP4_IP_MASK:
#if (LWIP_IPV4 && LWIP_IPV6)
            ret_val = ((uint32_t)(g_EMAC_if.netmask.u_addr.ip4.addr));
#else
            ret_val = (uint32_t)(g_EMAC_if.netmask.addr);
#endif
            break;

        case IP4_IP_GW:
#if (LWIP_IPV4 && LWIP_IPV6)
            ret_val = ((uint32_t)(g_EMAC_if.gw.u_addr.ip4.addr));
#else
            ret_val = (uint32_t)(g_EMAC_if.gw.addr);
#endif
            break;

        case IP4_IP_ADDR:
        default:
#if (LWIP_IPV4 && LWIP_IPV6)
            ret_val = ((uint32_t)(g_EMAC_if.ip_addr.u_addr.ip4.addr));
#else
            ret_val = (uint32_t)(g_EMAC_if.ip_addr.addr);
#endif
            break;
    }

    return (ret_val);
}

/*==============================================================================
 *
 */
void
get_mac_address(uint8_t *mac_addr)
{
    uint32_t inc;

    for (inc = 0; inc < 6; ++inc)
    {
        mac_addr[inc] = g_EMAC_if.hwaddr[inc];
    }
}

/*==============================================================================
 *
 */
void
str_to_ipv4_address(ip4_addr_t *address, char *string)
{
    int count;
    int index;
    uint8_t raw_addr[4];

    index = 0;
    for (count = 0; count != 4; count++)
    {
        raw_addr[count] = strtol(&string[index], 0, 10); /* Convert 1/4 of address to binary */

        while (isdigit(string[index])) /* Step over current number */
            index++;

        index++; /* Then step over '.' */
    }

    IP4_ADDR(address, raw_addr[0], raw_addr[1], raw_addr[2], raw_addr[3]);
}

/*==============================================================================
 *
 */

void
set_mac_address(uint8_t *mac_addr)
{
    /* Update stored copies of mac address */
    memcpy(g_EMAC_if.hwaddr, mac_addr, 6);
    memcpy(g_mac_config.mac_addr, mac_addr, 6);

    /* Reconfigure the actual hardware */
    //    MSS_MAC_update_hw_address(&g_mac_config);
}

/*==============================================================================
 *
 */
void
prvEthernetConfigureInterface(void *param)
{
    extern err_t ethernetif_init(struct netif * netif);

    /* Parameters are not used - suppress compiler error. */
    (void)param;

    /*
     * Set mac address
     */
    g_mac_config.mac_addr[0] = 0x00;
    g_mac_config.mac_addr[1] = 0xFC;
    g_mac_config.mac_addr[2] = 0x00;
    g_mac_config.mac_addr[3] = 0x12;
    g_mac_config.mac_addr[4] = 0x34;
    g_mac_config.mac_addr[5] = 0x57;

    memcpy(g_EMAC_if.hwaddr, g_mac_config.mac_addr, 6);

    /*
     * Start with a default fixed address so that we can bring the network
     * interface up quickly. Will most likely be the normal mode of operation
     * anyway...
     */
    g_network_addr_mode = NTM_DHCP_FIXED;

    str_to_ipv4_address(&g_ip_address, MY_STATIC_IP_ADDRESS);
    str_to_ipv4_address(&g_ip_gateway, MY_STATIC_IP_GATEWAY);
    str_to_ipv4_address(&g_ip_mask, MY_STATIC_IP_MASK);

    /* Start out with the default address */
    netif_add(&g_EMAC_if,
              &g_ip_address,
              &g_ip_mask,
              &g_ip_gateway,
              NULL,
              ethernetif_init,
              tcpip_input);

    /* bring it up */

    netif_set_up(&g_EMAC_if);

    if (NTM_DHCP_FIXED != g_network_addr_mode)
    {
        dhcp_start(&g_EMAC_if);
    }

    /* make it the default interface */
    netif_set_default(&g_EMAC_if);
}

/*==============================================================================
 *
 */
void ethernetif_tick(void);

mss_mac_speed_t g_net_speed;

void
prvLinkStatusTask(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    {
        volatile uint8_t linkup;
        uint8_t fullduplex;
        mss_mac_speed_t speed;
        uint32_t stats;

#if defined(G5_SOC_EMU_USE_GEM0) && \
    ((MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0_LOCAL))
        stats = MSS_MAC_read_stat(&g_mac0, 1);
        /* Run through loop every 500 milliseconds. */
        vTaskDelay(500 / portTICK_RATE_MS);
        linkup = MSS_MAC_get_link_status(&g_mac0, &speed, &fullduplex);
#else
        stats = MSS_MAC_read_stat(&g_mac1, 1);
        /* Run through loop every 500 milliseconds. */
        vTaskDelay(500 / portTICK_RATE_MS);
        linkup = MSS_MAC_get_link_status(&g_mac1, &speed, &fullduplex);
#endif
        g_net_speed = speed;
        ethernetif_tick();
    }
}

#endif
void e51_task(void *pvParameters);

volatile int second_task_count = 0;
#if MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0
/* MMUART 2 is on P9 pins 24(TXD) and 26(RXD) */
#define UART_DEMO &g_mss_uart2_lo
#else
#define UART_DEMO &g_mss_uart1_lo
#endif

void
blinky_task(void *pvParameters)
{
    (void)pvParameters;

    MSS_UART_polled_tx_string(UART_DEMO, "\r\nRunning FreeRTOS task blinky_task...");

    for (;;)
    {
        second_task_count++;
#if defined(TARGET_G5_SOC)
        /* Flash an LED to show something is happening */
        if ((second_task_count % 5) == 0)
        {
#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0)
            MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, (second_task_count / 5) & 1);
#else
            MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_16, (second_task_count / 5) & 1);
#endif
        }
#else
        if (second_task_count % 2U)
        {
            MSS_UART_polled_tx_string(UART_DEMO, "\rRunning FreeRTOS task blinky_task... -");
        }
        else
        {
            MSS_UART_polled_tx_string(UART_DEMO, "\rRunning FreeRTOS task blinky_task... +");
        }
#endif
        vTaskDelay(500);
    }
}

TaskHandle_t thandle_uart;
TaskHandle_t thandle_link;
TaskHandle_t thandle_web;
TaskHandle_t thandle_blinky;

void
e51(void)
{
    volatile int ix;
#if ((MSS_MAC_HW_PLATFORM != MSS_MAC_DESIGN_ICICLE_STD_GEM0_LOCAL) && \
     (MSS_MAC_HW_PLATFORM != MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0))
    free_rtos(); /* should never return */
#endif
    while (1)
    {
        ix++;
    }
}

/**
 * free_rtos(void)
 *   start freeRTOS
 *   will never return
 */
void
free_rtos(void)
{
    BaseType_t rtos_result;
    volatile int ix;

    rtos_result = xTaskCreate(e51_task, "e51", 4000, NULL, uartPRIMARY_PRIORITY, NULL);
    if (1 != rtos_result)
    {
        int ix;
        for (;;)
            ix++;
    }

    rtos_result =
        xTaskCreate(blinky_task, "e51-2nd", 4000, NULL, uartPRIMARY_PRIORITY + 2, &thandle_blinky);
    if (1 != rtos_result)
    {
        int ix;
        for (;;)
            ix++;
    }

    rtos_result = xTaskCreate(http_server_netconn_thread,
                              (char *)"http_server",
                              4000,
                              NULL,
                              uartPRIMARY_PRIORITY + 3,
                              &thandle_web);

    if (1 != rtos_result)
    {
        int ix;
        for (;;)
            ix++;
    }

    /* Create the task the Ethernet link status. */
    rtos_result = xTaskCreate(prvLinkStatusTask,
                              (char *)"EthLinkStatus",
                              4000,
                              NULL,
                              uartPRIMARY_PRIORITY + 1,
                              &thandle_link);
    if (1 != rtos_result)
    {
        int ix;
        for (;;)
            ix++;
    }

    vTaskSuspend(thandle_blinky);
    vTaskSuspend(thandle_link);
    vTaskSuspend(thandle_web);

    /* Start the kernel.  From here on, only tasks and interrupts will run. */
    vTaskStartScheduler();
}

/**
 *
 */

/*
 * Default to e51 values
 */
volatile uint64_t *mtime = (volatile uint64_t *)0x0200bff8;
volatile uint64_t *timecmp = (volatile uint64_t *)0x02004000;

void
e51_task(void *pvParameters)
{
    int count;

    volatile int i;
    int8_t info_string[100];
    uint64_t mcycle_start = 0;
    uint64_t mcycle_end = 0;
    uint64_t delta_mcycle = 0;
    uint32_t num_loops = 1000000;
    uint32_t hartid = read_csr(mhartid);
    static volatile uint64_t dummy_h0 = 0;
    uint8_t rx_buff[1];
    uint32_t rx_idx = 0;
    uint8_t rx_size = 0;
    volatile uint64_t delay_count;
    volatile uint32_t gpio_inputs;

    /*
     * Enable required clks - update with function call
     */
    SYSREG->SOFT_RESET_CR = 0U;
    SYSREG->SUBBLK_CLOCK_CR = 0xFFFFFFFFUL;

#if defined(TARGET_G5_SOC)

#if MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0_LOCAL
    /*
     * Icicle board setups with SGMII to VSC8662
     */
    /*
     * Start with GEM1 address as the standard Icicle design has the MDIO
     * connected to GEM1.
     */
    g_mac_config.phy_addr = PHY_VSC8662_1_MDIO_ADDR;
    g_mac_config.phy_type = MSS_MAC_DEV_PHY_VSC8662;
    g_mac_config.pcs_phy_addr = SGMII_MDIO_ADDR;
    g_mac_config.interface_type = TBI;
    g_mac_config.phy_autonegotiate = MSS_MAC_VSC8662_phy_autonegotiate;
    g_mac_config.phy_mac_autonegotiate = MSS_MAC_VSC8662_mac_autonegotiate;
    g_mac_config.phy_get_link_status = MSS_MAC_VSC8662_phy_get_link_status;
    g_mac_config.phy_init = MSS_MAC_VSC8662_phy_init;
    g_mac_config.phy_set_link_speed = MSS_MAC_VSC8662_phy_set_link_speed;

#if MSS_MAC_USE_PHY_DP83867
    g_mac_config.phy_extended_read = NULL_mmd_read_extended_regs;
    g_mac_config.phy_extended_write = NULL_mmd_write_extended_regs;
#endif

    MSS_GPIO_init(GPIO2_LO);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_16, MSS_GPIO_OUTPUT_MODE); /* LED 0 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_17, MSS_GPIO_OUTPUT_MODE); /* LED 1 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_18, MSS_GPIO_OUTPUT_MODE); /* LED 2 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_19, MSS_GPIO_OUTPUT_MODE); /* LED 3 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_26, MSS_GPIO_OUTPUT_MODE); /* PB 0 Force */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_27, MSS_GPIO_OUTPUT_MODE); /* PB 0 Force */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_28, MSS_GPIO_OUTPUT_MODE); /* PB 0 Force */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_30, MSS_GPIO_INPUT_MODE); /* PB 1 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_31, MSS_GPIO_INPUT_MODE); /* PB 2 */

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_16, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_17, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_18, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_19, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_26, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_27, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_28, 0);

    /* U54 2 values */
    mtime = (volatile uint64_t *)0x0200bff8;
    timecmp = ((volatile uint64_t *)0x02004000) + hartid;

#endif
#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0)

    MSS_GPIO_init(GPIO2_LO);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE); /* LED 0 */

    /* U54 2 values */
    mtime = (volatile uint64_t *)0x0200bff8;
    timecmp = ((volatile uint64_t *)0x02004000) + hartid;
#endif
#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0)
    /*
     * Icicle board setups with SGMII to VSC8662
     */
    /*
     * Start with GEM1 address as the standard Icicle design has the MDIO
     * connected to GEM1.
     */
    g_mac_config.phy_addr = PHY_VSC8662_1_MDIO_ADDR;
    g_mac_config.phy_type = MSS_MAC_DEV_PHY_VSC8662;
    g_mac_config.pcs_phy_addr = SGMII_MDIO_ADDR;
    g_mac_config.interface_type = TBI;
    g_mac_config.phy_autonegotiate = MSS_MAC_VSC8662_phy_autonegotiate;
    g_mac_config.phy_mac_autonegotiate = MSS_MAC_VSC8662_mac_autonegotiate;
    g_mac_config.phy_get_link_status = MSS_MAC_VSC8662_phy_get_link_status;
    g_mac_config.phy_init = MSS_MAC_VSC8662_phy_init;
    g_mac_config.phy_set_link_speed = MSS_MAC_VSC8662_phy_set_link_speed;

#if MSS_MAC_USE_PHY_DP83867
    g_mac_config.phy_extended_read = NULL_mmd_read_extended_regs;
    g_mac_config.phy_extended_write = NULL_mmd_write_extended_regs;
#endif

#endif

#if MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_SVG_GMII_GEM0

    /*
     * Single MAC setup with GMII to VSC8541
     *
     * Note: May need to add config to driver for G5SoC standard SGMII link
     * as this probably does not need/have MDIO interface and is "just there".
     */
    g_mac_config.phy_addr = PHY_VSC8541_MDIO_ADDR;
    g_mac_config.phy_type = MSS_MAC_DEV_PHY_VSC8541;
    g_mac_config.pcs_phy_addr = SGMII_MDIO_ADDR;
    g_mac_config.interface_type = GMII;
    g_mac_config.phy_autonegotiate = MSS_MAC_VSC8541_phy_autonegotiate;
    g_mac_config.phy_get_link_status = MSS_MAC_VSC8541_phy_get_link_status;
    g_mac_config.phy_init = MSS_MAC_VSC8541_phy_init;
    g_mac_config.phy_set_link_speed = MSS_MAC_VSC8541_phy_set_link_speed;

#if MSS_MAC_USE_PHY_DP83867
    g_mac_config.phy_extended_read = NULL_mmd_read_extended_regs;
    g_mac_config.phy_extended_write = NULL_mmd_write_extended_regs;
#endif

//    g_test_mac = &g_mac0;
#endif

#if MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_SVG_GMII_GEM1

    /*
     * Single MAC setup with GMII to VSC8541
     *
     * Note: May need to add config to driver for G5SoC standard SGMII link
     * as this probably does not need/have MDIO interface and is "just there".
     */
    g_mac_config.phy_addr = PHY_VSC8541_MDIO_ADDR;
    g_mac_config.phy_type = MSS_MAC_DEV_PHY_VSC8541;
    g_mac_config.pcs_phy_addr = SGMII_MDIO_ADDR;
    g_mac_config.interface_type = GMII;
    g_mac_config.phy_autonegotiate = MSS_MAC_VSC8541_phy_autonegotiate;
    g_mac_config.phy_get_link_status = MSS_MAC_VSC8541_phy_get_link_status;
    g_mac_config.phy_init = MSS_MAC_VSC8541_phy_init;
    g_mac_config.phy_set_link_speed = MSS_MAC_VSC8541_phy_set_link_speed;

#if MSS_MAC_USE_PHY_DP83867
    g_mac_config.phy_extended_read = NULL_mmd_read_extended_regs;
    g_mac_config.phy_extended_write = NULL_mmd_write_extended_regs;
#endif

    g_test_mac = &g_mac1;
#endif
/*****************************************************************************************/
#if MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_DUAL_EX_TI
    MSS_GPIO_init(GPIO1_LO);
    MSS_GPIO_config(GPIO1_LO,
                    MSS_GPIO_0,
                    MSS_GPIO_OUTPUT_MODE); /* DP83867 PHY reset - active low here as it is inverted
                                              in the bit-file */
    MSS_GPIO_config(GPIO1_LO,
                    MSS_GPIO_1,
                    MSS_GPIO_OUTPUT_MODE); /* SGMII core reset - active high */
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE); /* SGMII core MDIO address b0-b4 */
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_4, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_5, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_6, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_7, MSS_GPIO_INOUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_8, MSS_GPIO_INOUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_9, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_10, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_11, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_12, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_13, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_14, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_15, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_16, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_17, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_18, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_19, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_20, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_21, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_22, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_23, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_24, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_25, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_26, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_27, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_28, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_29, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_30, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO1_LO, MSS_GPIO_31, MSS_GPIO_INPUT_MODE);

    MSS_GPIO_set_output(GPIO1_LO, MSS_GPIO_2, 0); /* Set address to 16 */
    MSS_GPIO_set_output(GPIO1_LO, MSS_GPIO_3, 0);
    MSS_GPIO_set_output(GPIO1_LO, MSS_GPIO_4, 0);
    MSS_GPIO_set_output(GPIO1_LO, MSS_GPIO_5, 0);
    MSS_GPIO_set_output(GPIO1_LO, MSS_GPIO_6, 1);

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 0); /* Force VSC8575 into reset */
    MSS_GPIO_set_output(GPIO1_LO, MSS_GPIO_1, 1); /* Force SGMII core into reset */

    for (delay_count = 0; delay_count != 1000;)
        delay_count++;
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 1); /* Release reset line */
    MSS_GPIO_set_output(GPIO1_LO, MSS_GPIO_1, 0); /* Release reset line */
    for (delay_count = 0; delay_count != 1000000;)
        delay_count++;
/*
 * GEM 1 to TI PHY does not have reset complete line...
 *
 *   while(0 == ((gpio_inputs = MSS_GPIO_get_inputs(GPIO1_LO)) & 0x80))
 *   delay_count++;
 */
#endif

#if MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_DUAL_EX_VTS
    MSS_GPIO_init(GPIO2_LO);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE); /* VSC8575 Reset - active low */
    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_1,
                    MSS_GPIO_OUTPUT_MODE); /* SGMII core reset - active high */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE); /* SGMII core MDIO address b0-b4 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_4, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_5, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_6, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_7,
                    MSS_GPIO_INOUT_MODE); /* SGMII core reset done input and CRS output*/
    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_8,
                    MSS_GPIO_INOUT_MODE); /* Part of status vector input and COL output */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_9, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_10, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_11, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_12, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_13, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_14, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_15, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_16, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_17, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_18, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_19, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_20, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_21, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_22, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_23, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_24, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_25, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_26, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_27, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_28, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_29, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_30, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_31, MSS_GPIO_INPUT_MODE);
#if 0
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_2, 1); /* Set address to 16 */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_3, 1);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_4, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_5, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_6, 0);
#else
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_2, 0); /* Set address to 16 */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_3, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_4, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_5, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_6, 1);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_8, 1);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_9, 1);
#endif
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 0); /* Force VSC8575 into reset */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_1, 1); /* Force SGMII core into reset */

    for (delay_count = 0; delay_count != 1000;)
        delay_count++;
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 1); /* Release reset line */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_1, 0); /* Release reset line */
    for (delay_count = 0; delay_count != 1000000;)
        delay_count++;

    while (0 == ((gpio_inputs = MSS_GPIO_get_inputs(GPIO2_LO)) & 0x80))
        delay_count++;
#endif

#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_TBI) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_TBI_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_TBI_TI)
    MSS_GPIO_init(GPIO2_LO);
    MSS_GPIO_config(
        GPIO2_LO,
        MSS_GPIO_0,
        MSS_GPIO_INOUT_MODE); /* PHY Reset - active low / gtwiz_reset_rx_cdr_stable_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_1, MSS_GPIO_INPUT_MODE); /* gtwiz_reset_tx_done_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_2, MSS_GPIO_INPUT_MODE); /* gtwiz_reset_rx_done_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_3, MSS_GPIO_INPUT_MODE); /* resetdone */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_4, MSS_GPIO_INPUT_MODE); /* txpmaresetdone_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_5, MSS_GPIO_INPUT_MODE); /* rxpmaresetdone_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_6, MSS_GPIO_INPUT_MODE); /* rxcommadet_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_7, MSS_GPIO_INPUT_MODE); /* rxbyterealign_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_8, MSS_GPIO_INPUT_MODE); /* rxbyteisaligned_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_9, MSS_GPIO_INPUT_MODE); /* gtwiz_buffbypass_rx_error_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_10, MSS_GPIO_INPUT_MODE); /* gtwiz_buffbypass_rx_done_out */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_11, MSS_GPIO_INPUT_MODE); /* gtpowergood_out */

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 0); /* Force VSC8575 into reset */

    for (delay_count = 0; delay_count != 1000;)
        delay_count++;

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 1); /* Release reset line */

    for (delay_count = 0; delay_count != 1000000;)
        delay_count++;
#endif /* MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_TBI */

#if ((MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_GMII) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_GMII_GEM1))
    MSS_GPIO_init(GPIO2_LO);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE); /* VSC8575 Reset - active low */
    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_1,
                    MSS_GPIO_OUTPUT_MODE); /* SGMII core reset - active high */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE); /* SGMII core MDIO address b0-b4 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_4, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_5, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_6, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_7,
                    MSS_GPIO_INOUT_MODE); /* SGMII core reset done input and CRS output*/
    MSS_GPIO_config(GPIO2_LO,
                    MSS_GPIO_8,
                    MSS_GPIO_INOUT_MODE); /* Part of status vector input and COL output */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_9, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_10, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_11, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_12, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_13, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_14, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_15, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_16, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_17, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_18, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_19, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_20, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_21, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_22, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_23, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_24, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_25, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_26, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_27, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_28, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_29, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_30, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_31, MSS_GPIO_INPUT_MODE);

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_2, 0); /* Set address to 16 */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_3, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_4, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_5, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_6, 1);

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 0); /* Force VSC8575 into reset */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_1, 1); /* Force SGMII core into reset */

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_7, 0); /* Assert CRS */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_8, 0); /* Deassert COL */

    for (delay_count = 0; delay_count != 1000;)
        delay_count++;
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 1); /* Release reset line */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_1, 0); /* Release reset line */
    for (delay_count = 0; delay_count != 1000000;)
        delay_count++;

    while (0 == ((gpio_inputs = MSS_GPIO_get_inputs(GPIO2_LO)) & 0x80))
        delay_count++;

#endif /* MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_EMUL_GMII */
#endif /* defined(TARGET_G5_SOC) */

#if ((MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_SVG_GMII_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_SVG_GMII_GEM1))
    MSS_GPIO_init(GPIO2_LO);

    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_1, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);

    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_4, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_5, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_6, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_7, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_8, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_9, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_10, MSS_GPIO_INPUT_MODE);
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_11, MSS_GPIO_INPUT_MODE);

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_1, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_2, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_3, 0);

    for (delay_count = 0; delay_count != 1000;)
    {
        delay_count++;
    }

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 1); /* Release reset line */

    for (delay_count = 0; delay_count != 1000000;)
    {
        delay_count++;
    }
#endif
#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0)
    MSS_GPIO_init(GPIO2_LO);

#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0)
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE); /* USB ULPI Reset */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE); /* VSC8662 Reset */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE); /* VSC8662 Soft Reset */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_4, MSS_GPIO_OUTPUT_MODE); /* VSC8662 OSCEN */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_5, MSS_GPIO_OUTPUT_MODE); /* VSC8662 PLL Mode */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_6, MSS_GPIO_OUTPUT_MODE); /* VSC8662 CMODE 3 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_7, MSS_GPIO_OUTPUT_MODE); /* VSC8662 CMODE 4 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_8, MSS_GPIO_OUTPUT_MODE); /* VSC8662 CMODE 5 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_9, MSS_GPIO_OUTPUT_MODE); /* VSC8662 CMODE 6 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_10, MSS_GPIO_OUTPUT_MODE); /* VSC8662 CMODE 7 */
#endif
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_16, MSS_GPIO_OUTPUT_MODE); /* LED 0 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_17, MSS_GPIO_OUTPUT_MODE); /* LED 1 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_18, MSS_GPIO_OUTPUT_MODE); /* LED 2 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_19, MSS_GPIO_OUTPUT_MODE); /* LED 3 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_26, MSS_GPIO_OUTPUT_MODE); /* PB 0 Force */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_27, MSS_GPIO_OUTPUT_MODE); /* PB 0 Force */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_28, MSS_GPIO_OUTPUT_MODE); /* PB 0 Force */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_30, MSS_GPIO_INPUT_MODE); /* PB 1 */
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_31, MSS_GPIO_INPUT_MODE); /* PB 2 */

#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0)
    MSS_GPIO_config(GPIO2_LO, MSS_GPIO_11, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_0, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_2, 0); /* Assert reset */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_3, 1); /* De-assert soft reset */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_4, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_5, 1); /* Select 125MHz reference clock mode */
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_6, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_7, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_8, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_9, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_10, 0);
#endif
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_16, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_17, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_18, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_19, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_26, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_27, 0);
    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_28, 0);

#if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM1) || \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0)
    for (delay_count = 0; delay_count != 1000;)
    {
        delay_count++;
    }

    MSS_GPIO_set_output(GPIO2_LO, MSS_GPIO_2, 1); /* Release reset line */

    for (delay_count = 0; delay_count != 1000000;)
    {
        delay_count++;
    }
#endif
#endif /* #if (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM1) ||\ \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0) ||\ \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM1)   ||\ \
    (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0) */

    //    PLIC_init();
    __disable_local_irq((int8_t)MMUART0_E51_INT);

    vPortSetupTimer();

    MSS_UART_init(UART_DEMO,
                  MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(UART_DEMO, "Current IP settings\n\r");
    sprintf(info_string,
            "my ip address: %s\n\r",
            MY_STATIC_IP_ADDRESS,
            strlen(MY_STATIC_IP_ADDRESS));
    MSS_UART_polled_tx_string(UART_DEMO, info_string);
    sprintf(info_string,
            "my gateway address %s\n\r",
            MY_STATIC_IP_GATEWAY,
            strlen(MY_STATIC_IP_GATEWAY));
    MSS_UART_polled_tx_string(UART_DEMO, info_string);
    sprintf(info_string, "my ip mask %s\n\r", MY_STATIC_IP_MASK, strlen(MY_STATIC_IP_MASK));
    MSS_UART_polled_tx_string(UART_DEMO, info_string);
    MSS_UART_polled_tx_string(
        UART_DEMO,
        "Adjust as required, modify #define MY_STATIC_IP_ADDRESS xx.xx.xx.xx etc\n\r");

    //    PRINT_STRING("PolarFire MSS Ethernet Dual eMAC/pMAC Test program\n\r");

    //    PRINT_STRING("Polling method for TXRX. Typed characters will be echoed.\n\r");
    __enable_irq();

#if defined(MSS_MAC_USE_DDR) && (MSS_MAC_USE_DDR == MSS_MAC_MEM_CRYPTO)
    *ATHENA_CR = SYSREG_ATHENACR_RESET | SYSREG_ATHENACR_RINGOSCON;
    *ATHENA_CR = SYSREG_ATHENACR_RINGOSCON;
    CALIni();
    MSS_UART_polled_tx_string(UART_DEMO, "CALIni() done..\n\r");
#endif

#if 0
    PLIC_SetPriority(MAC0_INT_PLIC,  7);
    PLIC_SetPriority(MAC1_INT_PLIC,  7);
    PLIC_SetPriority(MAC0_EMAC_PLIC, 7);
    PLIC_SetPriority(MAC1_EMAC_PLIC, 7);
    PLIC_SetPriority(MAC0_MMSL_PLIC, 7);
    PLIC_SetPriority(MAC1_MMSL_PLIC, 7);

#if defined MSS_MAC_QUEUES
    PLIC_SetPriority(MAC0_QUEUE1_PLIC, 7);
    PLIC_SetPriority(MAC0_QUEUE2_PLIC, 7);
    PLIC_SetPriority(MAC0_QUEUE3_PLIC, 7);
    PLIC_SetPriority(MAC1_QUEUE1_PLIC, 7);
    PLIC_SetPriority(MAC1_QUEUE2_PLIC, 7);
    PLIC_SetPriority(MAC1_QUEUE3_PLIC, 7);
#endif
#endif
    //    low_level_init();

    tcpip_init(prvEthernetConfigureInterface, NULL);

    /* hack - must use a semaphore or other synchronisation?
     * Hold off starting these tasks until network is active
     * */
#if defined(G5_SOC_EMU_USE_GEM0) && \
    ((MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_SGMII_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_ICICLE_STD_GEM0_LOCAL) || \
     (MSS_MAC_HW_PLATFORM == MSS_MAC_DESIGN_BEAGLEV_FIRE_GEM0))
    while (MSS_MAC_AVAILABLE != g_mac0.mac_available)
#else
    while (MSS_MAC_AVAILABLE != g_mac1.mac_available)
#endif
    {
        vTaskDelay(1);
    }

    vTaskResume(thandle_web);
    vTaskResume(thandle_link);
    vTaskResume(thandle_blinky);

    while (1)
    {
#if 0
#if defined(TI_PHY)
        dump_ti_regs(&g_mac0);
#else
        dump_vsc8575_regs(&g_mac0);
#endif
#endif
        rx_size = MSS_UART_get_rx(UART_DEMO, rx_buff, sizeof(rx_buff));
        if (rx_size > 0)
        {
            MSS_UART_polled_tx_string(UART_DEMO, "=-");
            if (rx_buff[0] == '1')
            {
                hartid = read_csr(mhartid);
                mcycle_end = readmcycle();
                delta_mcycle = mcycle_end - mcycle_start;

                //    sprintf(info_string,"Hart %d, %d loops required %ld cycles, sw ints h0 = %d,
                //    sw ints h1 = %d, mtime = %d mtimecmp = %d\n\r", hartid,
                //            num_loops, delta_mcycle, count_sw_ints_h0, count_sw_ints_h1,
                //            CLINT->MTIME, CLINT->MTIMECMP[0]);
                //    MSS_UART_polled_tx_string (UART_DEMO, info_string);
            }
            else if (rx_buff[0] == '2')
            {
                raise_soft_interrupt((uint32_t)0);
                MSS_UART_polled_tx_string(UART_DEMO, "Raise sw int hart 0\n\r");
            }
            else if (rx_buff[0] == '3')
            {
                /*
                 * FIXME: This does not raise an interrupt
                 * Much the same code does on the unleashed-
                 */
                raise_soft_interrupt((uint32_t)1);
                MSS_UART_polled_tx_string(UART_DEMO, "Raise sw int hart 1\n\r");
            }
            else
            {
                /* echo the rx char */
                MSS_UART_polled_tx(UART_DEMO, rx_buff, rx_size);
            }
        }
        vTaskDelay(10);
    }
}

/*-----------------------------------------------------------*/

void
vApplicationMallocFailedHook(void)
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}
/*-----------------------------------------------------------*/

void
vApplicationIdleHook(void)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void
vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}
