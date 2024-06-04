
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
#include "inc/common.h"

/*------------------------------------------------------------------------------
 * Constant Definitions
 */
#define LIM_BASE_ADDRESS        0x08000000u
#define LIM_SIZE                0x200000u
#define BLOCK_SIZE_BYTES 512
#define SLOT_SIZE_BYTES (100 * 1024 * 1024)  // 100 MB in bytes

typedef enum {
    REGION_SLOT_0 = 0,
    REGION_SLOT_1 = REGION_SLOT_0 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES),
    REGION_SLOT_2 = REGION_SLOT_1 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES),
    REGION_SLOT_3 = REGION_SLOT_2 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES),
    END_OF_SLOT_REGION = REGION_SLOT_3 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES)
} SlotsRegion;

/*------------------------------------------------------------------------------
 * Private functions.
 */
static int8_t mmc_init_emmc(void);

/*------------------------------------------------------------------------------
 * Static Variables.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\t******* Application to Erase Content of a Specific Slot in eMMC *******\n\r\
To select the slot, enter the slot number you want to erase via serial port 0, and all blocks in the sector will be erased.\r\n";

/*------------------------------------------------------------------------------
 * Global Variables.
 */
volatile uint8_t g_rx_size = 0u;
volatile uint32_t count_sw_ints_h1 = 0U;
uint8_t g_mmc_initialized = 0u;
uint32_t start_slot = 0u;
uint32_t end_slot = 0u;
uint8_t g_rx_buff[10u] = {0};

/*------------------------------------------------------------------------------
 * MSS UART
 */
mss_uart_instance_t *g_uart= &g_mss_uart0_lo;

/*------------------------------------------------------------------------------
 * MSS eMMC
 */
mss_mmc_cfg_t g_mmc;

/*------------------------------------------------------------------------------
 * Global functions
 */
void u54_1(void)
{
    uint8_t ret_status;
    uint8_t p_buff[100];
    bool valid_sector = true;
    bool erase_failed = false;

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

    /* Init eMMC */
    int8_t  emmc_init_status = mmc_init_emmc();
    if (emmc_init_status != MSS_MMC_INIT_SUCCESS) {
        MSS_UART_polled_tx_string(g_uart,
                (const uint8_t*)"\r\nFailed to initialize eMMC\r\n");
    }

    while (1)
    {
        if (emmc_init_status == MSS_MMC_INIT_SUCCESS)
        {
            g_rx_size = MSS_UART_get_rx(g_uart, g_rx_buff, sizeof(g_rx_buff));

            if (g_rx_size > 0u)
            {
                switch (g_rx_buff[0u])
                {
                    case '0':
                        start_slot = REGION_SLOT_0;
                        break;
                    case '1':
                        start_slot = REGION_SLOT_1;
                        break;
                    case '2':
                        start_slot = REGION_SLOT_2;
                        break;
                    case '3':
                        start_slot = REGION_SLOT_3;
                        break;
                    default:
                        valid_sector = false;
                        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r Invalid sector");
                        break;
                }

                if(valid_sector)
                {
                   end_slot = ((SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES) + start_slot);
                   sprintf(p_buff,"\n\r start_slot=%d, end_slot=%d, total blocks = %d", start_slot, end_slot, end_slot - start_slot);
                   MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
                   sprintf(p_buff,"\n\r erasing sector %d .......... wait 5 min please", g_rx_buff[0u] - '0');
                   MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
                   while(start_slot < end_slot)
                   {
                       /* Erase single block */
                       ret_status = MSS_MMC_erase(start_slot, 1u);
                       if (ret_status != MSS_MMC_TRANSFER_SUCCESS)
                       {
                           erase_failed = true;
                           MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r Erase failed");
                           break;
                       }
                       start_slot++;
                   }

                   if(!erase_failed)
                   {
                        sprintf(p_buff,"\n\r sector %d deleted", g_rx_buff[0u] - '0');
                        MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
                   }
                }
                valid_sector = true;
                erase_failed = false;
            }
        }
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

