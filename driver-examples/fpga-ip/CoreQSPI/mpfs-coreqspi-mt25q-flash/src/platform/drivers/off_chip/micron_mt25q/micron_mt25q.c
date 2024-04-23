/*******************************************************************************
 * Copyright 2024 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file micron_mt25q.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief Driver for MICRON_MT25Q QSPI flash memory. This driver uses the
 * CoreQSPI driver interface.
 *
 */

#include "mpfs_hal/mss_hal.h"
#include "drivers/off_chip/micron_mt25q/micron_mt25q.h"
#include "drivers/fpga_ip/CoreQSPI/core_qspi.h"

/*Following constant must be defined if you want to use the interrupt mode 
  transfers provided by the CoreQSPI driver. Comment this out to use the polling
  mode transfers.*/

//#define USE_QSPI_INTERRUPT      1u

#ifdef __cplusplus
extern "C" {
#endif

#define QSPI_BASE               0x60020000u

uint32_t status_reg = 0u;
uint8_t flag_status_reg = 0u;

#define PAGE_LENGTH                             256u

qspi_config_t beforexip_qspi_config={0};
qspi_config_t qspi_config={0};
qspi_config_t qspi_config_read={0};

volatile uint8_t g_rx_complete = 0u;
volatile uint8_t g_tx_complete = 0u;

static uint8_t g_flash_io_format = QSPI_NORMAL;
volatile uint8_t xip_en = 0xF3u;
volatile uint8_t xip_dis = 0x0u;
volatile static uint8_t g_enh_v_val __attribute__ ((aligned (4))) = 0x0u;
volatile static uint16_t g_nh_cfg_val __attribute__ ((aligned (4))) = 0x0u;

#ifdef USE_QSPI_INTERRUPT
void transfer_status_handler(uint32_t status)
{
    if (STATUS_RXDONE_MASK == (STATUS_RXDONE_MASK & status))
    {
        g_rx_complete = 1;
    }
    else if (STATUS_TXDONE_MASK == (STATUS_TXDONE_MASK & status))
    {
        g_tx_complete = 1;
    }
}

static void wait_for_tx_complete(void)
{
    while (0u == g_tx_complete);
    g_tx_complete = 0u;
}

static void wait_for_rx_complete(void)
{
    while (0u == g_rx_complete);
    g_rx_complete = 0u;
}
#endif

/*Make sure that the erase operation is complete. i.e. wait for  write enable bit to go 0*/
static void wait_for_wel(uint8_t method)
{
    status_reg = 0;
    do{
        Flash_read_statusreg((uint8_t*)&status_reg, method);
    }while (0x02u & status_reg);
}

/*Make sure that the erase operation is complete. i.e. wait for  write enable bit to go 0*/
static void wait_for_wip(uint8_t method)
{
    status_reg = 0;
    do{
        Flash_read_statusreg((uint8_t*)&status_reg, method);
    }while (0x01u & status_reg);
}

qspi_io_format Flash_probe_io_format
(
    void
)
{

#ifdef USE_QSPI_INTERRUPT
    QSPI_set_status_handler( transfer_status_handler);
#endif

    qspi_config.io_format = QSPI_QUAD_FULL;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);

    Flash_readid((uint8_t*)&g_enh_v_val);

    if (0xFFu != g_enh_v_val)
    {
        return QSPI_QUAD_FULL;
    }

    qspi_config.io_format = QSPI_DUAL_FULL;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);
    Flash_readid((uint8_t*)&g_enh_v_val);

    if (0xFFu != g_enh_v_val)
    {
        return QSPI_DUAL_FULL;
    }

    qspi_config.io_format = QSPI_NORMAL;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);

    Flash_readid((uint8_t*)&g_enh_v_val);
    if (0xFFu != g_enh_v_val)
    {
        Flash_read_nvcfgreg((uint8_t*)&g_nh_cfg_val);
        return QSPI_NORMAL;
    }
}

void Flash_force_normal_mode
(
    void
)
{
    qspi_config.io_format = QSPI_NORMAL;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);

    Flash_readid((uint8_t*)&g_enh_v_val);
    if (0xFFu != g_enh_v_val)
    {
        Flash_read_nvcfgreg((uint8_t*)&g_nh_cfg_val);
        g_flash_io_format = QSPI_NORMAL;
        return;
    }

    qspi_config.io_format = QSPI_DUAL_FULL;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);
    Flash_readid((uint8_t*)&g_enh_v_val);

    if (0xFFu != g_enh_v_val)
    {
        g_flash_io_format = QSPI_DUAL_FULL;
        goto push_normal;
    }

    qspi_config.io_format = QSPI_QUAD_FULL;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);
    Flash_readid((uint8_t*)&g_enh_v_val);
    if (0xFFu != g_enh_v_val)
    {
        g_flash_io_format = QSPI_QUAD_FULL;
        goto push_normal;
    }

push_normal:
    {
        qspi_config.io_format = g_flash_io_format;
        QSPI_configure(QSPI_INSTANCE, &qspi_config);

        Flash_read_enh_v_confreg((uint8_t*)&g_enh_v_val);
        g_enh_v_val = 0xFFu;
        Flash_write_enh_v_confreg((uint8_t*)&g_enh_v_val);  /*disable quad and DUAL mode*/

        /*we disabled QUAD and DUAL mode of the FLASH in previous step.
         * Now bring the QSPI controller to normal mode*/
        qspi_config.io_format = QSPI_NORMAL;
        QSPI_configure(QSPI_INSTANCE, &qspi_config);

        do{
            Flash_readid((uint8_t*)&g_enh_v_val);
        }while(0xFFu == g_enh_v_val);

        Flash_read_nvcfgreg((uint8_t*)&g_nh_cfg_val);
    }
}

void Flash_init
(
    qspi_io_format io_format,
    addr_t base_addr
)
{
    volatile qspi_io_format t_io_format = QSPI_NORMAL;

    QSPI_init(QSPI_INSTANCE, base_addr);

    /*Find out the current mode of the flash memory device
     * and configure qspi controller to that mode.*/
    t_io_format = Flash_probe_io_format();

    qspi_config.clk_div =  QSPI_CLK_DIV_30;
    qspi_config.sample = QSPI_SAMPLE_POSAGE_SPICLK;
    qspi_config.spi_mode = QSPI_MODE3;
    qspi_config.xip = QSPI_DISABLE;
    qspi_config.io_format = t_io_format;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);

    /*If the desired IO format is same as the currently configured IO Format
     * Then we are done. otherwise configure the Flash and qspi controller
     * to desired IO format*/

    if (io_format != t_io_format)
    {
        Flash_read_enh_v_confreg((uint8_t*)&g_enh_v_val);

        if (io_format == QSPI_QUAD_FULL)
        {
            g_enh_v_val |= 0x40u; /* set the dual mode bit*/
            g_enh_v_val &= ~0x80u; /*clear the quad mode bit*/
        }
        else if (io_format == QSPI_DUAL_FULL)
        {
            g_enh_v_val |= 0x80u;  /*set the quad mode bit*/
            g_enh_v_val &= ~0x40u; /*clear the dual mode but*/
        }
        else
            g_enh_v_val |= 0xC0u; /*normal*/

        Flash_write_enh_v_confreg((uint8_t*)&g_enh_v_val);

        qspi_config.io_format = io_format;
        QSPI_configure(QSPI_INSTANCE, &qspi_config);
        Flash_read_enh_v_confreg((uint8_t*)&g_enh_v_val);

        do{
            Flash_force_normal_mode();
            Flash_readid((uint8_t*)&g_enh_v_val);
        }while (0xFFu == g_enh_v_val);

    }

    g_flash_io_format = io_format; /*store the value for future reference.*/
    enable_4byte_addressing();
}

void Flash_enter_xip
(
    void
)
{
    disable_4byte_addressing();
    uint8_t command_buf[5] __attribute__ ((aligned (4))) = {MICRON_WRITE_ENABLE};
    uint8_t temp = 0;

    /*Write enable command must be executed before writing to volatile configuration register*/
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0, 0);

    command_buf[0] = MICRON_WR_V_CONFIG_REG;
    command_buf[1] = xip_en;
    command_buf[2] = 0x00u;
    command_buf[3] = 0x00u;

    qspi_config.io_format = QSPI_NORMAL;
    QSPI_configure(QSPI_INSTANCE, &qspi_config);

    /*Enable XIP by writing to volatile configuration register*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 1, (uint8_t*)0, 0, 0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 1, (uint8_t*)0, 0, 0);
#endif
    wait_for_wel(1);
    wait_for_wip(1);

    /*Drive XIP confirmation using FAST read and keeping DQ0 to 0 during idle cycle*/
    command_buf[0] = MICRON_FAST_READ;
    command_buf[1] = 0x00u;
    command_buf[2] = 0x00u;
    command_buf[3] = 0x00u;

    /*Following command must be sent in polling method only.
      Using interrupt method is not possible here because the after sending this
      command flash memory immediately goes into the XIP mode and reading the
      status register in the IRQ returns the flash memory value instead of
      register value and this will not allow interrupt to be processed properly.*/
    if ((QSPI_QUAD_FULL == g_flash_io_format) ||
            (QSPI_QUAD_EX_RW == g_flash_io_format) ||
            (QSPI_QUAD_EX_RO == g_flash_io_format))
    {
        QSPI_polled_transfer_block(QSPI_INSTANCE, 3, command_buf, 1, &temp, 4, 10);
    }
    else
    {
        QSPI_polled_transfer_block(QSPI_INSTANCE, 3, command_buf, 1, &temp, 4, 8);
    }

    QSPI_get_config(QSPI_INSTANCE, &beforexip_qspi_config);


    /*For the XIP to work correctly, we must use QSPI_SAMPLE_NEGAGE_SPICLK per spec*/
    beforexip_qspi_config.sample = QSPI_SAMPLE_NEGAGE_SPICLK;
    beforexip_qspi_config.xip = QSPI_ENABLE;

    QSPI_configure(QSPI_INSTANCE, &beforexip_qspi_config);
}

void Flash_exit_xip
(
    void
)
{
    uint8_t command_buf[5] __attribute__ ((aligned (4))) = {MICRON_FAST_READ};

    beforexip_qspi_config.sample = QSPI_SAMPLE_POSAGE_SPICLK;
    beforexip_qspi_config.xip = QSPI_DISABLE;
    QSPI_configure(QSPI_INSTANCE, &beforexip_qspi_config);

    /*Drive XIP confirmation bit using FAST read and keeping DQ0 to 1 during idle cycle
     * this will exit the XIP*/

    command_buf[0] = MICRON_FAST_READ;
    command_buf[1] = 0x00u;
    command_buf[2] = 0x00u;
    command_buf[3] = 0xFFu;

#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 3, command_buf, 0, (uint8_t*)&xip_dis, 1, 8);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 3, command_buf, 0, (uint8_t*)&xip_dis, 1, 8);
#endif
    enable_4byte_addressing();
}

void Flash_read_statusreg
(
    uint8_t* rd_buf, uint8_t method
)
{
    const uint8_t command_buf[1] __attribute__ ((aligned (4))) = {MICRON_READ_STATUS_REG};

    /*This command works for all modes. No Dummy cycles*/
    if (1u == method)
    {
        QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
    }
    else
    {
#ifdef USE_QSPI_INTERRUPT
        QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
        wait_for_rx_complete();
#else
        QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
#endif
    }

}

void Flash_read_nvcfgreg
(
    uint8_t* rd_buf
)
{
    uint8_t command_buf[1] __attribute__ ((aligned (4))) = {MICRON_READ_NV_CONFIG_REG};

    /*This command works for all modes. No Dummy cycles*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 2,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 2,0);
#endif
}

void Flash_read_enh_v_confreg
(
    uint8_t* rd_buf
)
{
    const uint8_t command_buf[1] __attribute__ ((aligned (4))) = {MICRON_READ_ENH_V_CONFIG_REG};

    /*This command works for all modes. No Dummy cycles*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
#endif
}

void Flash_write_enh_v_confreg
(
    uint8_t* enh_v_val
)
{
    uint8_t command_buf[2] __attribute__ ((aligned (4))) = {MICRON_WRITE_ENABLE};

    /*execute Write enable command again for writing the data*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0, 0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0, 0);
#endif
    command_buf[0] =  MICRON_WR_ENH_V_CONFIG_REG;
    command_buf[1] =  *enh_v_val;

    /*This command works for all modes. No Dummy cycles*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 1, (uint8_t*)0, 0, 0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 1, (uint8_t*)0, 0, 0);
#endif

}

void Flash_read_flagstatusreg
(
    uint8_t* rd_buf
)
{
    const uint8_t command_buf[1] __attribute__ ((aligned (4))) = {MICRON_READ_FLAG_STATUS_REG};

    /*This command works for all modes. No Dummy cycles*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 1,0);
#endif
}

void Flash_enter_normal_mode
(
    void
)
{
    uint8_t enh_v_val = 0x0u;

    Flash_read_enh_v_confreg(&enh_v_val);
    enh_v_val |= 0xC0u; /*quad*/
    Flash_write_enh_v_confreg(&enh_v_val);
}

void Flash_readid
(
    uint8_t* rd_buf
)
{
    const uint8_t command_buf[1] __attribute__ ((aligned (4))) = {MICRON_MIO_READ_ID_OPCODE};

    /*This command works for all modes. No Dummy cycles.
     * MICRON_READ_ID_OPCODE works only for Normal mode*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 3,0);
    wait_for_rx_complete();
#else

// To-Do tx byte size should be sizeof(command_buf)?
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, rd_buf, 3,0);
#endif

}

void Flash_read
(
    uint8_t* buf,
    uint32_t read_addr,
    uint32_t read_len
)
{
    uint8_t dummy_cycles = 0u;
    uint8_t command_buf[10] __attribute__ ((aligned (4))) = {0};

    command_buf[1] = (read_addr >> 24) & 0xFFu;
    command_buf[2] = (read_addr >> 16u) & 0xFFu;
    command_buf[3] = (read_addr >> 8u) & 0xFFu;
    command_buf[4] = read_addr & 0xFFu;

    switch(g_flash_io_format)
    {
       case QSPI_NORMAL:
           command_buf[0] = MICRON_4BYTE_FAST_READ;
           dummy_cycles = 8u;
           break;
       case QSPI_DUAL_EX_RO:
           command_buf[0] = MICRON_4BYTE_DUALO_FAST_READ;   /* 1-1-2 */
           dummy_cycles = 8u;
           break;
       case QSPI_QUAD_EX_RO:
           command_buf[0] = MICRON_4BYTE_QUADO_FAST_READ;   /* 1-1-4 */
           dummy_cycles = 8u;
           break;
       case QSPI_DUAL_EX_RW:
           command_buf[0] = MICRON_4BYTE_DUALIO_FAST_READ;   /* 1-2-2 */
           dummy_cycles = 8u;
           break;
       case QSPI_QUAD_EX_RW:
           command_buf[0] = MICRON_4BYTE_QUADIO_FAST_READ;   /* 1-4-4 */
           dummy_cycles = 10u;
           break;
       case QSPI_DUAL_FULL:
           command_buf[0] = MICRON_4BYTE_FAST_READ;
           dummy_cycles = 8u;
           break;
       case QSPI_QUAD_FULL:
           command_buf[0] = MICRON_4BYTE_FAST_READ;
           dummy_cycles = 8u;                              /* For Quad mode */
           break;
       default:
           ASSERT(0);
           break;
    }

#ifdef USE_QSPI_INTERRUPT
       QSPI_irq_transfer_block(QSPI_INSTANCE, 4, command_buf, 0, buf, read_len, dummy_cycles);
       wait_for_rx_complete();
#else
       QSPI_polled_transfer_block(QSPI_INSTANCE, 4, command_buf, 0, buf, read_len, dummy_cycles);
#endif
}

uint8_t Flash_program
(
    uint8_t* buf,
    uint32_t wr_addr,
    uint32_t wr_len
)
{
    uint8_t command_buf[512] __attribute__ ((aligned (4))) = {0};
    /*All commands in this function work in all modes....*/

    wait_for_wel(0);
    wait_for_wip(0);

    /*execute Write enable command again for writing the data*/
    command_buf[0] = MICRON_WRITE_ENABLE;
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
#endif
    /*This command works for all modes. No Dummy cycles*/
    /*now program the sector. This will set the desired bits to 0.*/

    command_buf[0] = MICRON_4BYTE_PAGE_PROG;
    command_buf[1] = (wr_addr >> 24) & 0xFFu;
    command_buf[2] = (wr_addr >> 16u) & 0xFFu;
    command_buf[3] = (wr_addr >> 8u) & 0xFFu;
    command_buf[4] = wr_addr & 0xFFu;

    for (uint16_t idx=0; idx< wr_len;idx++)
        command_buf[5 + idx] = *(uint8_t*)(buf+idx);

#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 4, command_buf, wr_len, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 4, command_buf, wr_len, (uint8_t*)0, 0,0);
#endif

    while(1) {
    Flash_read_flagstatusreg(&flag_status_reg);

    if ((flag_status_reg & FLAGSTATUS_BUSY_MASK) != 0)
        break;
    }
    wait_for_wel(0);
    wait_for_wip(0);

    return (flag_status_reg & FLAGSTATUS_PFAIL_MASK);
}

uint8_t
Flash_erase(void)
{
    uint8_t status = 0xFFu;
    uint8_t command_buf[5] __attribute__ ((aligned (4))) = {MICRON_WRITE_ENABLE};
    /*Both Write enable and Die erase can work in all modes*/

    /* Write enable command must be executed before erase
     * WRITE ENABLE 06h 1-0-0 2-0-0 4-0-0 0 no dummy cycles.
     * */

#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
    command_buf[0] = MICRON_BULK_ERASE;
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
    /* Erase the die. This will write 1 to all bits
     * DIE ERASE C4h 1-1-0 2-2-0 4-4-0 no dummy cycles
     * */
    command_buf[0] = MICRON_BULK_ERASE;
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
#endif

    while (1){
        Flash_read_flagstatusreg(&status);
        if ((status & FLAGSTATUS_BUSY_MASK) != 0)
            break;
    }

    return(status & FLAGSTATUS_EFAIL_MASK);

}

void
device_reset(void)
{
    uint8_t command_buf[1] __attribute__ ((aligned (4))) = {MICRON_RESET_ENABLE};

    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, (void *)command_buf, 0, 0, 0,0);
    for(volatile uint32_t idx =0u; idx < 1000u ; idx++);    /* delay */

    command_buf[1] = MICRON_RESET_MEMORY;
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, (void *)command_buf, 0, 0, 0,0);
    for(volatile uint32_t idx =0u; idx < 1000u ; idx++);    /* delay */
}

/*Micron sector size is = 64k (65536 bytes).
 * addr parameter value should be any address  within the sector that needs to be erased */
void Flash_sector_erase(uint32_t addr)
{
    uint8_t command_buf[5] __attribute__ ((aligned (4))) = {MICRON_WRITE_ENABLE};

    /*This command works for all modes. No Dummy cycles*/
    /*Write enable command must be executed before erase*/
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
#endif
    /*This command works for all modes. No Dummy cycles*/
    /*Erase the sector. This will write 1 to all bits*/
    command_buf[0] = MICRON_DIE_ERASE;
    command_buf[1] = (addr >> 16u) & 0xFFu;
    command_buf[2] = (addr >> 8u) & 0xFFu;
    command_buf[3] = addr & 0xFFu;

#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 3, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 3, command_buf, 0, (uint8_t*)0, 0,0);
#endif
}

void Flash_die_erase(void)
{

    uint8_t command_buf[5] __attribute__ ((aligned (4))) = {MICRON_WRITE_ENABLE};
    /*Both Write enable and Die erase can work in all modes*/

    /*Write enable command must be executed before erase
     * WRITE ENABLE 06h 1-0-0 2-0-0 4-0-0 0 no dummy cycles.
     * */
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
#endif
    /*Erase the die. This will write 1 to all bits
     * DIE ERASE C4h 1-1-0 2-2-0 4-4-0 no dummy cycles
     * */
    command_buf[0] = MICRON_DIE_ERASE;
    command_buf[1] = 0x00u;    /*memory address MSB*/
    command_buf[2] = 0x00u;
    command_buf[3] = 0x03u;
#ifdef USE_QSPI_INTERRUPT
    QSPI_irq_transfer_block(QSPI_INSTANCE, 3, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    QSPI_polled_transfer_block(QSPI_INSTANCE, 3, command_buf, 0, (uint8_t*)0, 0,0);
#endif
}

uint8_t
program_page
(
    uint8_t* buf,
    uint32_t addr,
    uint32_t len
)
{
    uint8_t status = 0u;
    uint8_t command_buf[300] __attribute__ ((aligned (4))) = {0};
    uint32_t length = len;
    uint32_t offset = addr % PAGE_LENGTH;

    if (len > PAGE_LENGTH)
    {
        length = PAGE_LENGTH;
    }

    if (offset && ((length + offset) > PAGE_LENGTH))
    {
        length = PAGE_LENGTH - offset;
    }

    Flash_force_normal_mode();
    /*execute Write enable command again for writing the data*/
    command_buf[0] = MICRON_WRITE_ENABLE;
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0,0);
    Flash_force_normal_mode();
    command_buf[0] = MICRON_4BYTE_PAGE_PROG;
    command_buf[1] = (addr >> 24) & 0xFFu;
    command_buf[2] = (addr >> 16) & 0xFFu;
    command_buf[3] = (addr >> 8) & 0xFFu;
    command_buf[4] = addr & 0xFFu;

    for (uint16_t idx=0; idx< length;idx++)
    {
        command_buf[5 + idx] = *(uint8_t*)(buf+idx);
    }

    QSPI_polled_transfer_block(QSPI_INSTANCE, 4, command_buf, length, (uint8_t*)0, 0,0);

    while (1){
        Flash_read_flagstatusreg(&status);
        if ((status & FLAGSTATUS_BUSY_MASK) != 0)
            break;
    }
    return (status & FLAGSTATUS_PFAIL_MASK);
}

static void
enable_4byte_addressing
(
    void
)
{
    uint8_t command_buf[1] __attribute__ ((aligned (4))) = {MICRON_WRITE_ENABLE};

    /* Write enable command must be executed before writing extended addr reg. */
    QSPI_polled_transfer_block(QSPI_INSTANCE,0, command_buf, 0, (uint8_t*)0, 0, 0);

    command_buf[0] =  MICRON_ENTER_4BYTE_ADDR_MODE;

    /* This command works for all modes. No Dummy cycles */
    QSPI_polled_transfer_block(QSPI_INSTANCE,0, command_buf, 0, (uint8_t*)0, 0, 0);

}

static void
disable_4byte_addressing
(
    void
)
{
    uint8_t command_buf[2] __attribute__ ((aligned (4))) = {MICRON_WRITE_ENABLE};

    /* Write enable command must be executed before writing extended addr reg. */
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0, 0);

    command_buf[0] =  MICRON_EXIT_4BYTE_ADDR_MODE;

    /* This command works for all modes. No Dummy cycles */
    QSPI_polled_transfer_block(QSPI_INSTANCE, 0, command_buf, 0, (uint8_t*)0, 0, 0);

}

/*QSPI interrupt handler function*/
uint8_t PLIC_f2m_6_IRQHandler(void)
{
    qspi_isr(QSPI_INSTANCE);
    return (uint8_t)EXT_IRQ_KEEP_ENABLED;
}

#ifdef __cplusplus
}
#endif
