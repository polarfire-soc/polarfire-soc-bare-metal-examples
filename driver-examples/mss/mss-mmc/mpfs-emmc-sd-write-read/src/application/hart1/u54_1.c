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
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmc/mss_mmc.h"
#include "inc/common.h"
#include "inc/uart_mapping.h"

#define LIM_BASE_ADDRESS        0x08000000u
#define LIM_SIZE                0x200000u
#define ERROR_INTERRUPT         0x8000u
#define TRANSFER_COMPLETE       0x1u
#define SECT_NO                 1000u
#define BUFFER_SIZE             4096u // multiple block data size
#define USE_SDMA                0u
#define USE_ADMA2               1u
#define RX_BUFF_SIZE            10u
#define BLOCK_SIZE              512u

uint8_t g_mmc_rx_buff[BUFFER_SIZE] = {1};
uint8_t g_mmc_tx_buff[BUFFER_SIZE] = {1};
mss_mmc_cfg_t g_mmc;
volatile uint32_t count_sw_ints_h1 = 0U;

volatile uint8_t g_rx_size = 0u;
extern struct mss_uart_instance* p_uartmap_u54_1;
mss_uart_instance_t *g_uart;
static uint8_t g_mmc_initialized = 0u;

#if defined(MPFS_DISCOVERY_KIT)
const uint8_t g_greeting_msg[] =
   "\r\n\r\n\t******* PolarFire SoC MSS eMMC/SD Driver Example *******\n\n\n\r\
Data transfer status:\n\r\
  0 = Success \n\r\
 -1 = Wrong DMA mode selected (multi-block transfers only) \n\r\
 -2 = Read buffer did not match written values \r\n\
 status > 0 = Transfer failed. Refer to mss_mmc_status_t in mss_mmc.h \r\n\n\
Note: Ensure that the Libero design supports the desired memory device. \r\n\n\
Refer to README.md in the project root folder for more details.\r\n\n\n\
Menu:\r\n\
\r\n2. Execute data transfers on SD card \r\n";

#else
const uint8_t g_greeting_msg[] =
    "\r\n\r\n\t******* PolarFire SoC MSS eMMC/SD Driver Example *******\n\n\n\r\
Data transfer status:\n\r\
  0 = Success \n\r\
 -1 = Wrong DMA mode selected (multi-block transfers only) \n\r\
 -2 = Read buffer did not match written values \r\n\
 status > 0 = Transfer failed. Refer to mss_mmc_status_t in mss_mmc.h \r\n\n\
Note: Ensure that the Libero design supports the desired memory device. \r\n\n\
Refer to README.md in the project root folder for more details.\r\n\n\n\
Menu:\r\n\
\r\n1. Execute data transfers on eMMC device \r\n\
\r\n2. Execute data transfers on SD card\r\n";

#endif

static int8_t verify_write(uint8_t* write_buff, uint8_t* read_buff, uint32_t size);
static int8_t mmc_init_sdcard(void);
static int8_t mmc_init_emmc(void);
static int8_t multi_block_transfer(uint32_t sector_number);
static int8_t multi_block_wr_rd(uint32_t sector_number, uint8_t dma_type);
static int8_t single_block_wr_rd(uint32_t sector_number);
static void report_status(int8_t status);
void transfer_complete_handler(uint32_t status);

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void u54_1(void)
{
    uint8_t p_buff[100];
    uint8_t g_rx_buff[RX_BUFF_SIZE] = {0};
    uint32_t loop_count = 0;
    uint32_t sector_number = SECT_NO;
    int8_t status = 0u;

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

    g_uart = p_uartmap_u54_1;
    mss_config_clk_rst(MSS_PERIPH_MMUART_U54_1, (uint8_t) 1, PERIPHERAL_ON);
    mss_config_clk_rst(MSS_PERIPH_EMMC, (uint8_t) 1, PERIPHERAL_ON);

    MSS_UART_init(g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)g_greeting_msg);

    /* Initialize the write and read Buffers */
    for (loop_count = 0; loop_count < (BUFFER_SIZE); loop_count++)
    {
        g_mmc_tx_buff[loop_count] = 0x45u + loop_count;
        g_mmc_rx_buff[loop_count] = 0x00u;
    }

    PLIC_init();
    __enable_irq();
    PLIC_SetPriority(MMC_main_PLIC, 2u);
    PLIC_SetPriority(MMC_wakeup_PLIC, 2u);

    /* DMA init for eMMC */
    loop_count = MSS_MPU_configure(
            MSS_MPU_MMC,
            MSS_MPU_PMP_REGION3,
            LIM_BASE_ADDRESS,
            LIM_SIZE,
            MPU_MODE_READ_ACCESS|MPU_MODE_WRITE_ACCESS|MPU_MODE_EXEC_ACCESS,
            MSS_MPU_AM_NAPOT,
            0u);

    while (1)
    {
        g_rx_size = MSS_UART_get_rx(g_uart, g_rx_buff, sizeof(g_rx_buff));

        if (g_rx_size > 0u)
        {
            switch (g_rx_buff[0u])
            {
#if defined(ICICLE_KIT_ES)
                case '1':
                    status = mmc_init_emmc();
                    if (status == MSS_MMC_INIT_SUCCESS)
                    {
                        MSS_UART_polled_tx_string(
                            g_uart,
                            (const uint8_t*)
                            "\r\nExecuting data transfers on eMMC device\r\n"
                            );

                        status = single_block_wr_rd(sector_number);
                        sprintf(
                                p_buff,
                                "\r\neMMC - SINGLE block transfer status = %d",
                                status);
                        MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
                        report_status(status);

                        if (!status)
                        {
                            status = multi_block_transfer(sector_number);
                            sprintf(
                                    p_buff,
                                    "\r\neMMC - MULTI block transfer "
                                    "status = %d",
                                    status);
                            MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
                            report_status(status);
                        }
                    }
                    else
                    {
                        MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"\r\neMMC ... Failed\r\n");
                    }
                    break;
#endif
                case '2':
                    status = mmc_init_sdcard();
                    if (status == MSS_MMC_INIT_SUCCESS)
                    {
                        MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"\r\nExecuting data transfers "
                                    "on SD card\r\n");

                        status = single_block_wr_rd(sector_number);
                        sprintf(p_buff,"\r\nSD card - SINGLE block transfer "
                                "status = %d", status);
                        MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
                        report_status(status);

                        if (!status)
                        {
                            status = multi_block_transfer(sector_number);
                            sprintf(p_buff,"\r\nSD card - MULTI block transfer "
                                    "status = %d", status);
                            MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
                            report_status(status);
                        }
                    }
                    else
                    {
                        MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"\r\nSD Card ... Failed\r\n");
                    }

                    break;

                default:
                    MSS_UART_polled_tx_string(g_uart ,
                            (const uint8_t*)
                            "\r\nInvalid option selected!!!\r\n");
                    break;
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
        switch_demux_using_fabric_ip(EMMC_MSSIO_CONFIGURATION);
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

static int8_t mmc_init_sdcard(void)
{
    mss_mmc_status_t ret_status;

    if (g_mmc_initialized == 2u)
    {
        ret_status = MSS_MMC_INIT_SUCCESS;
    }
    else
    {
        ASSERT(mss_does_xml_ver_support_switch() == true)
#if defined(ICICLE_KIT_ES)
        if (switch_mssio_config(SD_MSSIO_CONFIGURATION) == false)
        {
            while(1u);
        }
        switch_demux_using_fabric_ip(SD_MSSIO_CONFIGURATION);
#endif
        /* SD Card configuration */
        g_mmc.card_type = MSS_MMC_CARD_TYPE_SD;
        g_mmc.data_bus_width = MSS_MMC_DATA_WIDTH_4BIT;
        g_mmc.bus_speed_mode = MSS_SDCARD_MODE_HIGH_SPEED;
        g_mmc.clk_rate = MSS_MMC_CLOCK_50MHZ;

        mmc_reset_block();

        ret_status = MSS_MMC_init(&g_mmc);
        if (ret_status == MSS_MMC_INIT_SUCCESS)
        {
            g_mmc_initialized = 2u;
        }
    }
    return ret_status;
}

static int8_t single_block_wr_rd(uint32_t sector_number)
{
    int8_t status = 0u;
    /* Erase single block */
    status = MSS_MMC_erase(sector_number, 1u);
    if (status == MSS_MMC_TRANSFER_SUCCESS)
    {
        /* Single Block - write */
        status = MSS_MMC_single_block_write(
                (uint32_t *)g_mmc_tx_buff,
                sector_number);
        if (status == MSS_MMC_TRANSFER_SUCCESS)
        {
            /* Single Block - read */
            status = MSS_MMC_single_block_read(
                    sector_number,
                    (uint32_t *)g_mmc_rx_buff);
            if (status == MSS_MMC_TRANSFER_SUCCESS)
            {
                status = verify_write(
                        g_mmc_tx_buff,
                        g_mmc_rx_buff,
                        BLOCK_SIZE); //single block
            }
        }
    }

    return (status);
}

static int8_t multi_block_transfer(uint32_t sector_number)
{
    int8_t status = 0u;
    /* Erase multi block */
    status = MSS_MMC_erase(sector_number, BUFFER_SIZE/BLOCK_SIZE);
    if (status == MSS_MMC_TRANSFER_SUCCESS)
    {
        MSS_MMC_set_handler(transfer_complete_handler);

        status = multi_block_wr_rd(sector_number, USE_SDMA);
        if (status == MSS_MMC_TRANSFER_SUCCESS)
        {
            status = multi_block_wr_rd(sector_number, USE_ADMA2);
        }
    }

    return (status);
}

static int8_t multi_block_wr_rd(uint32_t sector_number, uint8_t dma_type)
{
    int8_t status = 0u;

    if (USE_ADMA2 == dma_type)
    {
        status =  MSS_MMC_adma2_write(
                g_mmc_tx_buff,
                sector_number,
                BUFFER_SIZE);
    }
    else if (USE_SDMA == dma_type)
    {
        status =  MSS_MMC_sdma_write(
                g_mmc_tx_buff,
                sector_number,
                BUFFER_SIZE);
    }
    else
    {
        status = -1;
    }

    if (status == MSS_MMC_TRANSFER_IN_PROGRESS)
    {
        do
        {
            status =  MSS_MMC_get_transfer_status();
        }while (status == MSS_MMC_TRANSFER_IN_PROGRESS);
    }

    if (status == MSS_MMC_TRANSFER_SUCCESS)
    {
        if (USE_ADMA2 == dma_type)
        {
            status =  MSS_MMC_adma2_read(
                    sector_number,
                    g_mmc_rx_buff,
                    BUFFER_SIZE);
        }
        else if (USE_SDMA == dma_type)
        {
            status =  MSS_MMC_sdma_read(
                    sector_number,
                    g_mmc_rx_buff,
                    BUFFER_SIZE);
        }
        else
        {
            status = -1;
        }

        if (status == MSS_MMC_TRANSFER_IN_PROGRESS)
        {
            do
            {
                status = MSS_MMC_get_transfer_status();
            }while (status == MSS_MMC_TRANSFER_IN_PROGRESS);
        }

        if (status == MSS_MMC_TRANSFER_SUCCESS)
        {
            status = verify_write(g_mmc_tx_buff, g_mmc_rx_buff, BUFFER_SIZE);
        }
    }

    return (status);
}

static int8_t verify_write(
        uint8_t* write_buff,
        uint8_t* read_buff,
        uint32_t size)
{
    int8_t error = 0u;
    uint32_t index = 0u;

    while (size != 0u)
    {
        if (write_buff[index] != read_buff[index])
        {
            error = -2;
            break;
        }

        index++;
        size--;
    }

    return error;
}

/* hart1 software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}

static void report_status(int8_t status)
{
    if(0 == status)
    {
        MSS_UART_polled_tx_string(g_uart,
                (const uint8_t*)"\r\nTransfer successful \r\n");
    }
    else
    {
        MSS_UART_polled_tx_string(g_uart,
                (const uint8_t*)"\r\nTransfer failed \r\n");
    }
}
