/*******************************************************************************
 * Copyright 2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 */
#include "../../middleware/fatfs/ff.h"
#include "inc/common.h"
#include "mpfs_hal/mss_hal.h"
#include <stdio.h>
#include <string.h>

#define LIM_BASE_ADDRESS 0x08000000u
#define LIM_SIZE 0x200000u
#define ERROR_INTERRUPT 0x8000u
#define TRANSFER_COMPLETE 0x1u
#define SECT_NO 1000u
#define BUFFER_SIZE 64u /* Multiple block data size */
#define RX_BUFF_SIZE 10u

/******************************************************************************
 * Global variable
 */
/* The buffer size determines the eMMC read/write transfers.
 * For maximum file/read write performance, the buffer size must be maximum
 * possible.Due to limited RAM in the debug mode,the buffer size is selected
 * as 4096.
 * In Release mode of this project you may want to increase this value to
 * extract faster disk read/write performance.
 */
FATFS fs;
DIR dir;
FIL fil;
FILINFO fno;
BYTE work[_MAX_SS]; /* Work area (larger is better for processing time)

/* This buffer is passed to the eMMC driver. When eMMC drivers are configured to
 * use internal DMA, the address of this buffer must be modulo-4.Otherwise DMA
 * Transfer will fail.
 */
uint8_t rbuffer[BUFFER_SIZE] __attribute__((aligned(4))) = "0\0";
uint8_t ChangeBuffer = 1u;
int8_t rBuffersize = 16;
char path[] = "0:";
uint8_t g_ui_buf[500];

const uint8_t g_greeting_msg[] =
    "\r\n\n\n **** PolarFire SoC MSS MMC - FATFS example ****\n\n\r";

const uint8_t g_menu_msg[] = "\n\n\n\rSelect option: \r\n\
        1) Mount \r\n\
        2) Create FATFS File System \r\n\
        3) Open Directory \r\n\
        4) Open File \r\n\
        5) Print first file name of the directory \r\n\
        6) Write into the file \r\n\
        7) Read from the file \r\n\
        8) Close File \r\n\
        9) Unmount \r\n";

volatile uint32_t count_sw_ints_h1 = 0U;

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart.
 */
void u54_1(void)
{
    uint8_t rx_size = 0;
    int8_t stage[1];

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart into WFI.*/
    do
    {
        __asm("wfi");
    } while (0 == (read_csr(mip) & MIP_MSIP));
#endif

    /* The hart is out of WFI, clear the SW interrupt. Hear onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();

    /* Reset FPGA to access the SDIO register at FIC3 */
    SYSREG->SOFT_RESET_CR &= (uint32_t) ~(SOFT_RESET_CR_FPGA_MASK);

    (void)mss_config_clk_rst(MSS_PERIPH_EMMC, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t)MPFS_HAL_FIRST_HART,
                             PERIPHERAL_ON);

    PLIC_init();

    PLIC_EnableIRQ(MMC_main_PLIC);
    PLIC_EnableIRQ(MMC_wakeup_PLIC);

    __enable_irq();

    /* DMA init for MMC */
    MSS_MPU_configure(
        MSS_MPU_MMC, MSS_MPU_PMP_REGION3, LIM_BASE_ADDRESS, LIM_SIZE,
        MPU_MODE_READ_ACCESS | MPU_MODE_WRITE_ACCESS | MPU_MODE_EXEC_ACCESS,
        MSS_MPU_AM_NAPOT, 0u);

    MSS_UART_init(
        &g_mss_uart1_lo, MSS_UART_115200_BAUD,
        (MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT));

    MSS_UART_polled_tx_string(&g_mss_uart1_lo, g_greeting_msg);

    dir.fs = &fs;
    fil.fs = &fs;

    MSS_UART_polled_tx_string(&g_mss_uart1_lo, g_menu_msg);

    while (1u)
    {
        rx_size = MSS_UART_get_rx(&g_mss_uart1_lo, stage, sizeof(stage));

        if (rx_size > 0u)
        {
            /* Echo the rx char */
            sprintf((char *)&g_ui_buf[0], "\r\nselected: %c    ", stage[0]);
            MSS_UART_polled_tx_string(&g_mss_uart1_lo, (uint8_t *)g_ui_buf);

            rx_size = 0u;

            switch (stage[0])
            {

            case '1':

                if (FR_OK == f_mount(0u, &fs))
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > file system mounted \n ");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > file system not mounted, "
                                   "Try to create File System \n ");
                }

                break;

            case '2':

                if (FR_OK == f_mkfs(0u, 0u, sizeof work))
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > file system created \n ");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > file system not created, "
                                   "Try to check ioctl functions \n ");
                }

                break;

            case '3':

                if (FR_OK == f_opendir(&dir, path))
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > you are in root directory \n ");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > this is not valid directory \n ");
                }

                break;

            case '4':

                if (FR_OK == f_open(&fil, "1stfile.txt",
                                    FA_OPEN_ALWAYS | FA_READ | FA_WRITE))
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > file is opened \n ");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > file is not opened  \n ");
                }

                break;

            case '5':

                if (FR_OK == f_readdir(&dir, &fno))
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > first file name in "
                                   "the directory : ");
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo, fno.fname);
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo, "\n");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > open the file first \n ");
                }

                break;

            case '6':

                f_lseek(&fil, 0u);

                if (ChangeBuffer)
                {
                    rBuffersize = f_puts("112233445566778899\n\0", &fil);
                    ChangeBuffer = 0u;

                    if (-1 != rBuffersize)
                    {
                        MSS_UART_polled_tx_string(
                            &g_mss_uart1_lo,
                            (uint8_t *)"\n\r   > written 112233445566778899 "
                                       "in file \n ");
                    }
                }
                else
                {
                    rBuffersize = f_puts("aabbccddeeffgghh\n\0", &fil);
                    ChangeBuffer = 1u;

                    if (-1 != rBuffersize)
                    {
                        MSS_UART_polled_tx_string(
                            &g_mss_uart1_lo,
                            (uint8_t *)"\n\r   > written aabbccddeeffgghh "
                                       "in file \n ");
                    }
                }

                if (-1 == rBuffersize)
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > unable to write in file \n ");
                }

                break;

            case '7':

                f_lseek(&fil, 0u);

                f_gets(rbuffer, rBuffersize, &fil);

                if (NULL != rbuffer)
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > Reading from file :  ");
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo, rbuffer);
                    MSS_UART_polled_tx_string(&g_mss_uart1_lo,
                                              (uint8_t *)"\n\r ");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > unable to read from file \n ");
                }

                break;

            case '8':

                if (FR_OK == f_close(&fil))
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > file is closed \n ");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > unable to close the file \n ");
                }

                break;

            case '9':

                if (FR_OK == f_mount(0u, NULL))
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > unmounted the file system \n ");
                }
                else
                {
                    MSS_UART_polled_tx_string(
                        &g_mss_uart1_lo,
                        (uint8_t *)"\n\r   > unable to unmount  \n ");
                }

                break;

            default:

                MSS_UART_polled_tx_string(
                    &g_mss_uart1_lo,
                    (uint8_t *)"\n\r   > please enter valid option"
                               " from menu \n  ");

                break;
            } /* Switch end */

            MSS_UART_polled_tx_string(&g_mss_uart1_lo, g_menu_msg);
            rx_size = 0u;
        }
    } /* While end */
} /* U54_1 end */

