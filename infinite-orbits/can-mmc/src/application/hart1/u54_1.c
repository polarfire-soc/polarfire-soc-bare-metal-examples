/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmc/mss_mmc.h"
#include "drivers/mss/mss_mmc/mss_can.h"
#include "inc/common.h"

/*------------------------------------------------------------------------------
 * Constant Definitions
 */
#define LIM_BASE_ADDRESS        0x08000000u
#define LIM_SIZE                0x200000u
#define ERROR_INTERRUPT         0x8000u
#define TRANSFER_COMPLETE       0x1u
#define SECT_NO                 0u
#define USE_SDMA                0u
#define USE_ADMA2               1u
#define BLOCK_SIZE              512u
#define BUFFER_SIZE             4096u // multiple block data size
#define DEBUG                   0u

/*------------------------------------------------------------------------------
 * Private functions.
 */
static int8_t multi_block_write_transfer(uint32_t sector_number);
static int8_t multi_block_write(uint32_t sector_number, uint8_t dma_type);
static int8_t mmc_init_emmc(void);

void transfer_complete_handler(uint32_t status);

/*------------------------------------------------------------------------------
 * Static Variables.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\t******* Data Transfer Process *******\r\n\r\
This code defines the main function u54_1 for the hart1 processor. It initializes various variables and peripherals,"
" including CAN communication, UART, and eMMC. The main loop waits for CAN messages, processes them, "
"and performs data transfer operations to the eMMC memory device based on the received messages. "
"It also includes functions for handling CAN messages, reordering data, and handling transfer completion events.\r\n";

/*------------------------------------------------------------------------------
 * Global Variables.
 */
char message[50];
volatile uint32_t count_sw_ints_h1 = 0U;

/*------------------------------------------------------------------------------
 * MSS UART
 */
mss_uart_instance_t *g_uart= &g_mss_uart0_lo;

/*------------------------------------------------------------------------------
 * MSS CAN
 */
mss_can_instance_t* g_mss_can_0 = &g_mss_can_0_lo;
mss_can_instance_t* g_mss_can_1 = &g_mss_can_1_lo;
mss_can_filterobject pfilter;
mss_can_msgobject rx_buf;
mss_can_rxmsgobject rx_msg;

/*------------------------------------------------------------------------------
 * MSS eMMC
 */
uint8_t g_mmc_rx_buff[BUFFER_SIZE] = {0};
uint8_t g_mmc_tx_buff[BUFFER_SIZE] = {0};
mss_mmc_cfg_t g_mmc;
uint8_t g_mmc_initialized = 0u;

/*------------------------------------------------------------------------------
 * Global functions
 */
/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void u54_1(void)
{
    uint8_t ret_status;
    uint16_t sector_number = 0;
    uint32_t g_bytes_received = 0;
    uint32_t g_head = 0;
    uint32_t g_tail = 0;
    uint32_t  g_block_count = 0u;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /*Put this hart into WFI.*/
    do
    {
        __asm("wfi");
    }while(0 == (read_csr(mip) & MIP_MSIP));
#endif

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();
    /* Reset FPGA to access the SDIO register at FIC3 */
    SYSREG->SOFT_RESET_CR   &= (uint32_t)~(SOFT_RESET_CR_FPGA_MASK);

    mss_config_clk_rst(MSS_PERIPH_MMUART0, (uint8_t) 1, PERIPHERAL_ON);
    mss_config_clk_rst(MSS_PERIPH_EMMC, (uint8_t) 1, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CAN0, (uint8_t) 1, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_CAN1, (uint8_t) 1, PERIPHERAL_ON);

    PLIC_DisableIRQ(CAN0_PLIC);
    PLIC_DisableIRQ(CAN1_PLIC);

    MSS_UART_init(g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)g_greeting_msg);

    PLIC_init();
    __enable_irq();
    PLIC_SetPriority(MMC_main_PLIC, 2u);
    PLIC_SetPriority(MMC_wakeup_PLIC, 2u);

    /* DMA init for eMMC */
    ret_status = MSS_MPU_configure(MSS_MPU_MMC,
                                       MSS_MPU_PMP_REGION3,
                                       LIM_BASE_ADDRESS,
                                       LIM_SIZE,
                                       MPU_MODE_READ_ACCESS|MPU_MODE_WRITE_ACCESS|MPU_MODE_EXEC_ACCESS,
                                       MSS_MPU_AM_NAPOT,
                                       0u);

    /*--------------------------------------------------------------------------
         * Performs CAN Initialization and Message Buffer Configuration
         */
        /* ----------------------- CAN - 0 Initialization   ----------------- */
    ret_status = MSS_CAN_init(g_mss_can_0, CAN_SPEED_8M_1M,
                                         (pmss_can_config_reg)0, 6u, 6u);
    MSS_CAN_set_mode(g_mss_can_0, CANOP_MODE_NORMAL);
    MSS_CAN_start(g_mss_can_0);

    /* ----------------------- CAN - 1 Initialization   ----------------- */
    ret_status = MSS_CAN_init(g_mss_can_1, CAN_SPEED_8M_1M,
                                     (pmss_can_config_reg)0, 6u, 6u);
    MSS_CAN_set_mode(g_mss_can_1, CANOP_MODE_NORMAL);
    MSS_CAN_start(g_mss_can_1);

    /* Configure for receive */
    rx_msg.ID = 0x0u;
    rx_msg.DATAHIGH = 0u;
    rx_msg.DATALOW = 0u;
    rx_msg.AMR.L = 0xFFFFFFFFu;
    rx_msg.ACR.L = 0x00000000u;
    rx_msg.AMR_D = 0xFFFFFFFFu;
    rx_msg.ACR_D = 0x00000000u;
    rx_msg.RXB.DLC = 8u;
    rx_msg.RXB.IDE = 0u;

    /* Configure receive buffer For CAN 0 */
    ret_status = MSS_CAN_config_buffer_n(g_mss_can_0, 0, &rx_msg);
    if (CAN_OK != ret_status)
    {
        MSS_UART_polled_tx_string(g_uart,
               (const uint8_t *)"\n\r CAN 0 Message Buffer configuration Error");
    }

    /* Configure receive buffer For CAN 1 */
    rx_msg.ID = 0x00;
    ret_status = MSS_CAN_config_buffer_n(g_mss_can_1, 1, &rx_msg);
    if (CAN_OK != ret_status)
    {
        MSS_UART_polled_tx_string(g_uart,
               (const uint8_t *)"\n\r CAN 1 Message Buffer configuration Error");
    }

    ret_status = mmc_init_emmc();

    for (sector_number = 0; sector_number < 500; sector_number++)
    {
        ret_status = MSS_MMC_erase(sector_number, 1u);
    }

    sprintf(message, "\n\r erase, status: %d ", ret_status);
    MSS_UART_polled_tx_string(g_uart, message);

    for (sector_number = 0; sector_number < 500; sector_number++)
    {
        ret_status = MSS_MMC_single_block_read(sector_number, (uint32_t *)g_mmc_rx_buff);
        for (uint16_t loop_count = 0u; loop_count < BUFFER_SIZE; loop_count++)
        {
            if(g_mmc_rx_buff[loop_count] != 0)
            {
               sprintf(message, "\n \rsector %d: g_mmc_rx_buff[%d] = %02X ", sector_number, loop_count, g_mmc_rx_buff[loop_count]);
               MSS_UART_polled_tx_string(g_uart, message);
            }
        }
    }

    sprintf(message, "\n\r INIT OK, read status: %d ", ret_status);
    MSS_UART_polled_tx_string(g_uart, message);

    while (1)
    {
        ret_status = MSS_CAN_get_message_n(g_mss_can_1, 1u, &rx_buf);

        if (CAN_VALID_MSG == ret_status)
            {
                // Copy CAN message data to buffer
                uint32_t remaining_bytes = BUFFER_SIZE - g_bytes_received;
                uint32_t bytes_to_copy = rx_buf.DLC < remaining_bytes ? rx_buf.DLC : remaining_bytes;

                for (uint32_t loop_count = 0u; loop_count < bytes_to_copy; loop_count++)
                {
                    g_mmc_tx_buff[g_head] = rx_buf.DATA[loop_count];
                    g_head = (g_head + 1) % BUFFER_SIZE;
                    if (g_head == g_tail)
                    {
                        // Buffer is full, move tail pointer to overwrite oldest data
                        g_tail = (g_tail + 1) % BUFFER_SIZE;
                    }
                }

                // Increment byte count by bytes copied
                g_bytes_received += bytes_to_copy;

                // If the buffer is full, post-process and reset the buffer
                if (g_bytes_received >= BUFFER_SIZE)
                {
                    sprintf(message, "\rContents of g_mmc_tx_buff before write: %d ", g_block_count);
                    MSS_UART_polled_tx_string(g_uart, message);
                    multi_block_write_transfer(g_block_count);

                    memset(g_mmc_tx_buff, 0, sizeof(g_mmc_tx_buff));
                    g_block_count++;
                    g_head = 0;
                    g_tail = 0;
                    g_bytes_received = 0;

                }
            }
        }
}


void transfer_complete_handler(uint32_t status)
{
    uint32_t isr_err;

    if (ERROR_INTERRUPT & status)
    {
        isr_err = status >> 16u;
    }
    else if (TRANSFER_COMPLETE & status)
    {
        isr_err = 0u;   /*Transfer complete*/
    }
    else
    {
        ASSERT(0);
    }
}

static void mmc_reset_block(void)
{
    SYSREG->SUBBLK_CLOCK_CR |= (uint32_t)(SUBBLK_CLOCK_CR_MMC_MASK);
    SYSREG->SOFT_RESET_CR |= (uint32_t)(SOFT_RESET_CR_MMC_MASK);
    SYSREG->SOFT_RESET_CR &= ~(uint32_t)(SOFT_RESET_CR_MMC_MASK);
}

static int8_t mmc_init_emmc(void)
{
    mss_mmc_status_t ret_status;

    if (g_mmc_initialized == 1u)
    {
        ret_status = MSS_MMC_INIT_SUCCESS;
    }
    else
    {
        ASSERT(mss_does_xml_ver_support_switch() == true)

        if (switch_mssio_config(EMMC_MSSIO_CONFIGURATION) == false)
        {
            while(1u);
        }
        switch_external_mux(EMMC_MSSIO_CONFIGURATION);
        /* eMMC configuration */
        g_mmc.clk_rate = MSS_MMC_CLOCK_50MHZ;
        g_mmc.card_type = MSS_MMC_CARD_TYPE_MMC;
        g_mmc.bus_speed_mode = MSS_MMC_MODE_SDR;
        g_mmc.data_bus_width = MSS_MMC_DATA_WIDTH_8BIT;
        g_mmc.bus_voltage = MSS_MMC_1_8V_BUS_VOLTAGE;

        mmc_reset_block();

        ret_status = MSS_MMC_init(&g_mmc);
        if (ret_status == MSS_MMC_INIT_SUCCESS)
        {
            g_mmc_initialized = 1u;
        }
    }
    return ret_status;
}


/* hart1 software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}

static int8_t multi_block_write_transfer(uint32_t sector_number) {
    int8_t status = 0u;

    MSS_MMC_set_handler(transfer_complete_handler);

    status = multi_block_write(sector_number, USE_ADMA2);
    if (status == MSS_MMC_TRANSFER_SUCCESS) {
        status = multi_block_write(sector_number, USE_SDMA);
    }

    return status;
}

static int8_t multi_block_write(uint32_t sector_number, uint8_t dma_type) {
    int8_t status = 0u;
    uint8_t p_buff[128];

    if (USE_ADMA2 == dma_type) {
        status = MSS_MMC_adma2_write(g_mmc_tx_buff, sector_number, BUFFER_SIZE);
    } else if (USE_SDMA == dma_type) {
        status = MSS_MMC_sdma_write(g_mmc_tx_buff, sector_number, BUFFER_SIZE);
    } else {
        status = -1;
    }

    if (status == MSS_MMC_TRANSFER_IN_PROGRESS) {
        do {
            status = MSS_MMC_get_transfer_status();
        } while (status == MSS_MMC_TRANSFER_IN_PROGRESS);
    }

    return status;
}


