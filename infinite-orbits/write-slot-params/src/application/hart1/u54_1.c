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
#define BLOCK_SIZE_BYTES        512
#define SLOT_SIZE_BYTES         (100 * 1024 * 1024)  // 100 MB in bytes

typedef enum {
    REGION_SLOT_0 = 0,
    REGION_SLOT_1 = REGION_SLOT_0 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES),
    REGION_SLOT_2 = REGION_SLOT_1 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES),
    REGION_SLOT_3 = REGION_SLOT_2 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES),
    END_OF_SLOT_REGION = REGION_SLOT_3 + (SLOT_SIZE_BYTES / BLOCK_SIZE_BYTES)
} SlotsRegion;

// Enumeration to define the order of elements
typedef enum {
    LAST_FAILED_INDEX,      // Index for LastFailed
    CURRENT_TRY_INDEX,      // Index for CurrentTry
    LAST_SUCCESSFUL_INDEX,  // Index for LastSuccessful
    BOOT_SEQUENCE_INDEX     // Index for BootSequence
} ParamDataIndex;

typedef struct {
    uint8_t LastFailed;
    uint8_t CurrentTry;
    uint8_t LastSuccessful;
    uint8_t BootSequence[4];
} ParamData;

// Function to copy a ParamData structure to a buffer
void copyParamDataToBuffer(const ParamData *params, uint8_t *buffer) {
    buffer[LAST_FAILED_INDEX] = params->LastFailed;
    buffer[CURRENT_TRY_INDEX] = params->CurrentTry;
    buffer[LAST_SUCCESSFUL_INDEX] = params->LastSuccessful;
    for (size_t i = 0; i < sizeof(params->BootSequence); ++i) {
        buffer[BOOT_SEQUENCE_INDEX + i] = params->BootSequence[i];
    }
}

// Function to copy from a buffer to a ParamData structure
void copyBufferToParamData(const uint8_t *buffer, ParamData *params) {
    params->LastFailed = buffer[LAST_FAILED_INDEX];
    params->CurrentTry = buffer[CURRENT_TRY_INDEX];
    params->LastSuccessful = buffer[LAST_SUCCESSFUL_INDEX];
    for (size_t i = 0; i < sizeof(params->BootSequence); ++i) {
        params->BootSequence[i] = buffer[BOOT_SEQUENCE_INDEX + i];
    }
}


/*------------------------------------------------------------------------------
 * Private functions.
 */
static int8_t mmc_init_emmc(void);
static int8_t single_block_wr_rd(uint32_t sector_number);
static void read_slot_params(void);

/*------------------------------------------------------------------------------
 * Static Variables.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\t******* Application to Write Slot params in eMMC *******\n\r\ Click 1 to read the current values and set the new values.\r\n";

/*------------------------------------------------------------------------------
 * Global Variables.
 */
volatile uint8_t g_rx_size = 0u;
volatile uint32_t count_sw_ints_h1 = 0U;
uint8_t g_mmc_initialized = 0u;
uint32_t start_slot = 0u;
uint32_t end_slot = 0u;
uint8_t g_rx_buff[10u] = {0};
uint8_t status;
uint8_t p_buff[100];

/*------------------------------------------------------------------------------
 * MSS UART
 */
mss_uart_instance_t *g_uart= &g_mss_uart0_lo;

/*------------------------------------------------------------------------------
 * MSS eMMC
 */
mss_mmc_cfg_t g_mmc;
uint8_t g_mmc_rx_buff[BLOCK_SIZE_BYTES] = {1};
uint8_t g_mmc_tx_buff[BLOCK_SIZE_BYTES] = {1};

/*------------------------------------------------------------------------------
 * Global functions
 */
void u54_1(void)
{
    bool valid_sector = true;
    bool erase_failed = false;
    uint32_t sector_number = 0;
    uint8_t emmc_status = 0;

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
    status = MSS_MPU_configure(MSS_MPU_MMC,
                                   MSS_MPU_PMP_REGION3,
                                   LIM_BASE_ADDRESS,
                                   LIM_SIZE,
                                   MPU_MODE_READ_ACCESS|MPU_MODE_WRITE_ACCESS|MPU_MODE_EXEC_ACCESS,
                                   MSS_MPU_AM_NAPOT,
                                   0u);

    emmc_status = mmc_init_emmc();
    uint16_t counter= 0;
    while (1)
    {
        g_rx_size = MSS_UART_get_rx(g_uart, g_rx_buff, sizeof(g_rx_buff));

        if (g_rx_size > 0u)
        {
            switch (g_rx_buff[0u])
            {
                case '1':

                    if (emmc_status == MSS_MMC_INIT_SUCCESS)
                    {
                        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\r\n PREVIOUS VLAUES: \r\n");
                        read_slot_params();
                        sector_number = END_OF_SLOT_REGION;
                        ParamData params = {
                            .LastFailed = 1,
                            .CurrentTry = 1,
                            .LastSuccessful = 1,
                            .BootSequence = {1, 0, 0, 0}
                        };

                        // Copy ParamData structure to buffer
                        copyParamDataToBuffer(&params, g_mmc_tx_buff);
                        status = single_block_wr_rd(sector_number);

                        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\r\n CURRENT VLAUES: \r\n");
                        read_slot_params();
                    }
                    else
                    {
                        MSS_UART_polled_tx_string(g_uart,
                            (const uint8_t*)"\r\neMMC ... Failed\r\n");
                    }
                    break;

                default:
                    MSS_UART_polled_tx_string(g_uart ,
                            (const uint8_t*)"\r\nInvalid option selected!!!\r\n");
                    break;
            }
        }
    }
}

static void read_slot_params(void)
{
    ParamData Params_rx;
    status = MSS_MMC_single_block_read(END_OF_SLOT_REGION, (uint32_t *)g_mmc_rx_buff);
    copyBufferToParamData(g_mmc_rx_buff, &Params_rx);
    sprintf(p_buff,"\r LastFailed: %u\n", Params_rx.LastFailed);
    MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
    sprintf(p_buff,"\r CurrentTry: %u\n", Params_rx.CurrentTry);
    MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
    sprintf(p_buff,"\r LastSuccessful: %u\n", Params_rx.LastSuccessful);
    MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
    sprintf(p_buff,"\r BootSequence: ");
    MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
    for (int i = 0; i < 4; ++i) {
        sprintf(p_buff,"%u, ", Params_rx.BootSequence[i]);
        MSS_UART_polled_tx(g_uart, p_buff, strlen(p_buff));
    }
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n");
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

static int8_t single_block_wr_rd(uint32_t sector_number)
{
    int8_t status = 0u;
    /* Erase single block */
    status = MSS_MMC_erase(sector_number, 1u);
    if (status == MSS_MMC_TRANSFER_SUCCESS)
    {
        /* Single Block - write */
        status = MSS_MMC_single_block_write((uint32_t *)g_mmc_tx_buff, sector_number);
        if (status == MSS_MMC_TRANSFER_SUCCESS)
        {
            /* Single Block - read */
            status = MSS_MMC_single_block_read(sector_number, (uint32_t *)g_mmc_rx_buff);
            if (status == MSS_MMC_TRANSFER_SUCCESS)
            {
                //status = verify_write(g_mmc_tx_buff, g_mmc_rx_buff, BLOCK_SIZE); //single block
            }
        }
    }

    return (status);
}

/* hart1 software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
    count_sw_ints_h1++;
}

