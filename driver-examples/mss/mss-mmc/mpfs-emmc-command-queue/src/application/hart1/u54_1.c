/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code demonstrating MMC transfers using command queue. Refer
 * README.md for details.
 *
 */
#include <stdio.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmc/mss_mmc.h"
#include "inc/common.h"

#define LIM_BASE_ADDRESS        0x08000000u
#define LIM_SIZE                0x200000u
#define ERROR_INTERRUPT         0x8000u
#define TRANSFER_COMPLETE       0x1u
#define CQ_INTERRUPT            0x4000u
#define SECT_NO                 180u
#define BUFFER_SIZE           	4096u
#define TASK_ID_WRITE           1u
#define TASK_ID_READ            2u
#define RX_BUFF_SIZE            10U

uint8_t g_mmc_rx_buff[BUFFER_SIZE];
uint8_t g_mmc_tx_buff[BUFFER_SIZE];
mss_mmc_cfg_t g_mmc;

volatile uint32_t count_sw_ints_h1 = 0U;

volatile uint8_t g_rx_size = 0u;
mss_uart_instance_t *g_uart= &g_mss_uart1_lo;
const uint8_t g_greeting_msg[] =
"\r\n\r\n\t  ******* PolarFire SoC MSS MMC Driver command queue Example *******\n\n\n\r";

static int8_t verify_write(uint8_t* write_buff, uint8_t* read_buff, uint32_t size);
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
    mss_mmc_cfg_t g_mmc;
    mss_mmc_status_t ret_status;
    uint32_t loop_count;
    uint8_t error = 0u;
    uint32_t sector_number = SECT_NO;

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

    mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
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
    loop_count = MSS_MPU_configure(MSS_MPU_MMC,
                                   MSS_MPU_PMP_REGION3,
                                   LIM_BASE_ADDRESS,
                                   LIM_SIZE,
                                   MPU_MODE_READ_ACCESS|MPU_MODE_WRITE_ACCESS|MPU_MODE_EXEC_ACCESS,
                                   MSS_MPU_AM_NAPOT,
                                   0u);

    ASSERT(mss_does_xml_ver_support_switch() == true)

    if (switch_mssio_config(EMMC_MSSIO_CONFIGURATION) == false)
    {
        while(1u);
    }
    switch_external_mux(EMMC_MSSIO_CONFIGURATION);

    /* Configure eMMC */
    g_mmc.clk_rate = MSS_MMC_CLOCK_50MHZ;
    g_mmc.card_type = MSS_MMC_CARD_TYPE_MMC;
    g_mmc.bus_speed_mode = MSS_MMC_MODE_SDR;
    g_mmc.data_bus_width = MSS_MMC_DATA_WIDTH_8BIT;
    g_mmc.bus_voltage = MSS_MMC_1_8V_BUS_VOLTAGE;

    ret_status = MSS_MMC_init(&g_mmc);
    if (ret_status == MSS_MMC_INIT_SUCCESS)
    {
        ret_status = MSS_MMC_cq_init();
        if (ret_status == MSS_MMC_INIT_SUCCESS)
        {
            MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Command queue initialized...\r\n");
            MSS_MMC_set_handler(transfer_complete_handler);

            ret_status = MSS_MMC_cq_write(g_mmc_tx_buff, sector_number, BUFFER_SIZE);
            if (ret_status == MSS_MMC_TRANSFER_IN_PROGRESS)
            {
                do
                {
                    ret_status = MSS_MMC_get_transfer_status();
                }while (ret_status == MSS_MMC_TRANSFER_IN_PROGRESS);
            }
        }
        else
        {
            MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Command queue initialization failed\r\n");
        }

        if (ret_status == MSS_MMC_TRANSFER_SUCCESS)
        {
            ret_status = MSS_MMC_cq_read(sector_number, g_mmc_rx_buff, BUFFER_SIZE);
            if (ret_status == MSS_MMC_TRANSFER_IN_PROGRESS)
            {
                do
                {
                    ret_status = MSS_MMC_get_transfer_status();
                }while (ret_status == MSS_MMC_TRANSFER_IN_PROGRESS);
            }

            if (ret_status == MSS_MMC_TRANSFER_SUCCESS)
            {
                error = verify_write(g_mmc_tx_buff, g_mmc_rx_buff, BUFFER_SIZE);
                if(0 != error)
                {
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Data transfer failed\r\n");
                }
            }
        }
        else
        {
            MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Data transfer failed\r\n");
        }
    }
    else
    {
        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"eMMC device initialization failed\r\n");
    }

    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Data transfer completed.\r\n");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Example is executed successfully if there are no error messages.\r\n");

    while (1)
    {
		/* Never return */
    }
}
/******************************************************************************/
static int8_t verify_write(uint8_t* write_buff, uint8_t* read_buff, uint32_t size)
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
/******************************************************************************/
void transfer_complete_handler(uint32_t status)
{
    uint32_t isr_err;

    if (ERROR_INTERRUPT & status)
    {
        isr_err = status >> 16u;
    }
    else if (TRANSFER_COMPLETE & status)
    {
        isr_err = 0u;   /* Queued transfer is complete*/
    }
    else if (CQ_INTERRUPT & status)
    {
        isr_err = 0u; /* All queued tasks completed */
    }
    else
    {
        ASSERT(0);
    }
}

/* hart1 software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}
