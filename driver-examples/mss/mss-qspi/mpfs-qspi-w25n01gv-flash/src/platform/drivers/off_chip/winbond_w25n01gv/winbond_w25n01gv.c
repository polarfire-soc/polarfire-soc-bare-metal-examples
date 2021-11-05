/***************************************************************************//**
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * Driver for WINBOND W25N01 QSPI flash memory.
 * This driver uses the MPFS MSS QSPI driver interface.
 */

#include "drivers/off_chip/winbond_w25n01gv/winbond_w25n01gv.h"
#include "mpfs_hal/common/mss_plic.h"

/* Following constant must be defined if you want to use the interrupt mode
 * transfers provided by the MSS QSPI driver. Comment this out to use the polling
 * mode transfers.*/
//#define USE_QSPI_INTERRUPT      1u

#ifdef __cplusplus
extern "C" {
#endif

/* Winbond flash page length */
#define PAGE_LENGTH     2048

#define BLOCK_LENGTH    (PAGE_LENGTH * 64)

#define STATUS_REG_1    0xA0
#define STATUS_REG_2    0xB0
#define STATUS_REG_3    0xC0

uint32_t status_reg = 0u;
uint8_t flag_status_reg = 0u;

mss_qspi_config_t beforexip_qspi_config={0};
mss_qspi_config_t qspi_config={0};
mss_qspi_config_t qspi_config_read={0};

volatile uint8_t g_rx_complete = 0u;
volatile uint8_t g_tx_complete = 0u;

static uint8_t g_flash_io_format = MSS_QSPI_NORMAL;
static volatile uint8_t g_enh_v_val __attribute__ ((aligned (4))) = 0x0u;

static void disable_write_protect(void);
static void erase_block(uint32_t addr);
static void read_status_reg
(
    uint8_t status_reg_address,
    uint8_t* rd_buf
);

static void write_statusreg
(
    uint8_t address,
    uint8_t new_status_reg
);

#ifdef USE_QSPI_INTERRUPT
void transfer_status_handler(uint32_t status)
{
    if (STTS_RDONE_MASK == (STTS_RDONE_MASK & status))
    {
        g_rx_complete = 1;
    }
    else if (STTS_TDONE_MASK == (STTS_TDONE_MASK & status))
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

/* Make sure that the erase operation is complete. i.e. wait for  write enable
 * bit to go 0 */
static void wait_for_wel(void)
{
    status_reg = 0;
    do{
        Flash_read_statusreg((uint8_t*)&status_reg);
    }while ((0x02u & status_reg) == 0);
}

/* Make sure that the erase operation is complete. i.e. wait for  write enable
 * bit to go 0
 */
static void wait_for_wip(void)
{
    status_reg = 0;
    do{
        Flash_read_statusreg((uint8_t*)&status_reg);
    }while (0x01u & status_reg);
}

void Flash_force_normal_mode
(
    void
)
{
    qspi_config.io_format = MSS_QSPI_NORMAL;
    MSS_QSPI_configure(&qspi_config);

    Flash_readid((uint8_t*)&g_enh_v_val);
    if (0xFFu != g_enh_v_val)
    {
        g_flash_io_format = MSS_QSPI_NORMAL;
        return;
    }

    qspi_config.io_format = MSS_QSPI_DUAL_FULL;
    MSS_QSPI_configure(&qspi_config);
    Flash_readid((uint8_t*)&g_enh_v_val);

    if (0xFFu != g_enh_v_val)
    {
        g_flash_io_format = MSS_QSPI_DUAL_FULL;
        goto push_normal;
    }

    qspi_config.io_format = MSS_QSPI_QUAD_FULL;
    MSS_QSPI_configure(&qspi_config);
    Flash_readid((uint8_t*)&g_enh_v_val);
    if (0xFFu != g_enh_v_val)
    {
        g_flash_io_format = MSS_QSPI_QUAD_FULL;
        goto push_normal;
    }

    push_normal:
    {
        qspi_config.io_format = g_flash_io_format;
        MSS_QSPI_configure(&qspi_config);

        /* we disabled quad and DUAL mode of the FLASH in previous step.
         * Now bring the QSPI controller to normal mode*/
        qspi_config.io_format = MSS_QSPI_NORMAL;
        MSS_QSPI_configure(&qspi_config);

        do{
            Flash_readid((uint8_t*)&g_enh_v_val);
        }while(0xFFu == g_enh_v_val);
    }
}

/*==============================================================================
 *
 */
void Flash_init
(
    mss_qspi_io_format io_format
)
{
    volatile uint8_t transmit_buffer[32];
    volatile uint8_t receive_buffer[32];

    /* Reset the flash */
    const uint8_t command_buf[1] __attribute__ ((aligned (4))) = {WINBOND_DEVICE_RESET};

    mss_qspi_io_format t_io_format = MSS_QSPI_NORMAL;

    MSS_QSPI_init();

    t_io_format = Flash_probe_io_format();

    qspi_config.clk_div =  MSS_QSPI_CLK_DIV_30;

    qspi_config.sample = MSS_QSPI_SAMPLE_POSAGE_SPICLK;
    qspi_config.spi_mode = MSS_QSPI_MODE3;
    qspi_config.xip = MSS_QSPI_DISABLE;
    qspi_config.io_format = t_io_format;
    MSS_QSPI_configure(&qspi_config);

    /* Reset The QSPI flash */
    qspi_config.clk_div =  MSS_QSPI_CLK_DIV_10;
    qspi_config.io_format = MSS_QSPI_NORMAL;
    MSS_QSPI_configure(&qspi_config);
}

void Flash_read_statusreg
(
    uint8_t* rd_buf
)
{
    const uint8_t command_buf[2] __attribute__ ((aligned (4))) = {WINBOND_READ_STATUS_REGISTER, 0xC0};

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(1, command_buf, 0, rd_buf, 1,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(1, command_buf, 0, rd_buf, 1,0);
#endif
}

static void write_statusreg
(
    uint8_t address,
    uint8_t new_status_reg
)
{
    uint8_t command_buf[3] __attribute__ ((aligned (4)));

    command_buf[0] = WINBOND_WRITE_STATUS_REG;
    command_buf[1] = address;
    command_buf[2] = new_status_reg;

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 2, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 2, (uint8_t*)0, 0,0);
#endif

}

static void read_status_reg
(
    uint8_t status_reg_address,
    uint8_t* rd_buf
)
{
    const uint8_t command_buf[2] __attribute__ ((aligned (4))) =
    {
            WINBOND_READ_STATUS_REGISTER,
            status_reg_address
    };

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(1, command_buf, 0, rd_buf, 1,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(1, command_buf, 0, rd_buf, 1,0);
#endif

}

void Flash_readid
(
    uint8_t* rd_buf
)
{
    const uint8_t command_buf[1] __attribute__ ((aligned (4))) = {WINBOND_READ_JEDEC_ID_OPCODE};

    /* This command works for all modes. No Dummy cycles. */
#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 0, rd_buf, 3,8);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 0, rd_buf, 3,8);
#endif
}

/*------------------------------------------------------------------------------
 *
 */
void read_page
(
    uint8_t* buf,
    uint32_t page,
    uint32_t read_len
)
{
    uint32_t length = read_len;
    uint8_t command_buf[8] __attribute__ ((aligned (4))) = {WINBOND_PAGE_DATA_READ};

    wait_for_wip();

    command_buf[1] = 0;
    command_buf[2] = (page >> 8u) & 0xFFu;
    command_buf[3] = page & 0xFFu;

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 3, 0, 0, 0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 3, 0, 0, 0);
#endif

    wait_for_wip();

    command_buf[0] = WINBOND_READ_DATA; /* WINBOND_READ_DATA; */
    command_buf[1] = 0;
    command_buf[2] = 0;
    command_buf[3] = 0;
#ifdef FAST_READ_QUAD
    command_buf[4] = 0;
    command_buf[5] = 0;
    command_buf[6] = 0;
    //    command_buf[7] = 0;
#endif
    if(read_len > PAGE_LENGTH)
    {
        length = PAGE_LENGTH;
    }

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 3, buf, length, 0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 3, buf, length, 0);
#endif

    wait_for_wip();
}

/*==============================================================================
 *
 */
void Flash_read
(
    uint8_t* buf,
    uint32_t read_addr,
    uint32_t read_len
)
{
    /* We only deal with reads from page boundaries for now. */

    uint32_t page_nb;
    uint8_t * target_buf = buf;
    int32_t remaining_length = (int32_t)read_len;

    page_nb = read_addr / PAGE_LENGTH;

    while(remaining_length > 0)
    {
        uint32_t length;

        if(remaining_length > PAGE_LENGTH)
        {
            length = PAGE_LENGTH;
        }
        else
        {
            length = remaining_length;
        }

        read_page(target_buf, page_nb, length);
        remaining_length = remaining_length - length;
        page_nb++;
        target_buf = target_buf + length;
    }
}

/*==============================================================================
 *
 */
uint8_t program_page
(
    uint8_t* buf,
    uint32_t page,
    uint32_t wr_len
)
{
    uint8_t command_buf[PAGE_LENGTH + 3] __attribute__ ((aligned (4))) = {0};

    ASSERT(wr_len <= PAGE_LENGTH);

    wait_for_wip();

    /* Write Enable */
    command_buf[0] = WINBOND_WRITE_ENABLE;

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 0, (uint8_t*)0, 0,0);
#endif

    volatile uint8_t status_reg_1;
    volatile uint8_t status_reg_2;
    read_status_reg(0xA0, (uint8_t *)&status_reg_1);
    read_status_reg(0xB0, (uint8_t *)&status_reg_2);
    ++status_reg_1;
    ++status_reg_2;

    wait_for_wel();
    wait_for_wip();
    uint16_t idx;
    uint16_t colomhack;
    colomhack = 0;
    for(int inc = 0; inc < 8; inc++)
    {
        /* Program page buffer */
        command_buf[0] = 0x84; /* Random Load Program data */
        command_buf[1] = (colomhack >> 8) & 0xFFu;
        command_buf[2] = colomhack & 0xFFu;

        for (idx = 0; idx < 256; idx++)
            command_buf[3 + idx] = *(uint8_t*)(buf+idx+colomhack);

#ifdef USE_QSPI_INTERRUPT
        MSS_QSPI_irq_transfer_block(2, command_buf, 256, (uint8_t*)0, 0,0);
        wait_for_rx_complete();
#else
        MSS_QSPI_polled_transfer_block(2, command_buf, 256, (uint8_t*)0, 0,0);
#endif

        colomhack += 256;
    }

    /* Send Program Execute command */
    command_buf[0] = WINBOND_PROGRAM_EXECUTE;
    command_buf[1] = 0u;
    command_buf[2] = (page >> 8) & 0xFFu;
    command_buf[3] = page & 0xFFu;

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 3, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 3, (uint8_t*)0, 0,0);
#endif

    wait_for_wip();

    return 0;
}

/*==============================================================================
 *
 */
uint8_t program_block
(
    uint8_t* buf,
    uint32_t wr_addr,
    uint32_t wr_len
)
{
    uint32_t page_nb = wr_addr / PAGE_LENGTH;
    int32_t remaining_length = (int32_t)wr_len;

    ASSERT(wr_len <= BLOCK_LENGTH);
    while(remaining_length > 0)
    {
        uint32_t page_length;

        if(remaining_length >= PAGE_LENGTH)
        {
            page_length = PAGE_LENGTH;
        }
        else
        {
            page_length = remaining_length;
        }

        program_page(buf, page_nb, page_length);
        remaining_length = remaining_length - page_length;
        buf += page_length;
        page_nb++;
    }

    return 0;   //flag_status_reg;
}

/*==============================================================================
 *
 */
void Flash_erase
(
    void
)
{
    uint32_t page_nb;

    for(page_nb = 0; page_nb < 1024; page_nb++)
    {
        erase_block(page_nb);
    }
}

/*==============================================================================
 * Program the flash memory
 */
uint8_t Flash_program
(
    uint8_t* buf,
    uint32_t wr_addr,
    uint32_t wr_len
)
{
    int32_t remaining_length = (int32_t)wr_len;
    uint32_t target_offset = wr_addr;

    disable_write_protect();

    while (remaining_length > 0)
    {
        uint32_t block_length;

        if(remaining_length >= BLOCK_LENGTH)
        {
            block_length = BLOCK_LENGTH;
        }
        else
        {
            block_length = remaining_length;
        }
        program_block(buf, target_offset, block_length);
        remaining_length -= block_length;
        buf += block_length;
        target_offset += block_length;
    }

    return 0;
}

/* Winbond block erase
 */
void Flash_die_erase
(
    void
)
{
    uint8_t command_buf[5] __attribute__ ((aligned (4))) = {WINBOND_WRITE_ENABLE};
    /*Both Write enable and Die erase can work in all modes*/

    /*Write enable command must be executed before erase
     * WRITE ENABLE 06h 1-0-0 2-0-0 4-0-0 0 no dummy cycles.
     * */
#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 0, (uint8_t*)0, 0,0);
#endif
    /*Erase the die. This will write 1 to all bits
     * DIE ERASE C4h 1-1-0 2-2-0 4-4-0 no dummy cycles
     * */
    command_buf[0] = WINBOND_BLOCK_ERASE;
    command_buf[1] = 0x00u;    /*memory address MSB*/
    command_buf[2] = 0x00u;
    command_buf[3] = 0x03u;
#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(3, command_buf, 0, (uint8_t*)0, 0,8);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(3, command_buf, 0, (uint8_t*)0, 0,0);
#endif
}

/*==============================================================================
 * Disable write protect
 */
static void disable_write_protect
(
    void
)
{
    uint8_t command_buf[3] __attribute__ ((aligned (4)));

    command_buf[0] = WINBOND_WRITE_STATUS_REG;
    command_buf[1] = STATUS_REG_1;
    command_buf[2] = 0x00;

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_irq_transfer_block(0, command_buf, 2, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 2, (uint8_t*)0, 0,0);
#endif
}

/*==============================================================================
 * Block is 128Kbytes (64 pages of 2Kbytes)
 */
static void erase_block
(
    uint32_t block_nb
)
{
    uint8_t command_buf[4] __attribute__ ((aligned (4)));
    uint32_t block_page_nb;
    wait_for_wip();

    /* Write Enable */
    command_buf[0] = WINBOND_WRITE_ENABLE;
#ifdef USE_QSPI_INTERRUPT

    MSS_QSPI_irq_transfer_block(0, command_buf, 0, (uint8_t*)0, 0,0);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 0, (uint8_t*)0, 0,0);
#endif
    wait_for_wel();
    block_page_nb = block_nb * 64;

    /* Erase */
    command_buf[0] = WINBOND_BLOCK_ERASE;
    command_buf[1] = 0u;
    command_buf[2] = (block_page_nb >> 8u) & 0xFFu;
    command_buf[3] = block_page_nb & 0xFFu;

#ifdef USE_QSPI_INTERRUPT

    MSS_QSPI_irq_transfer_block(0, command_buf, 3, (uint8_t*)0, 0,8);
    wait_for_rx_complete();
#else
    MSS_QSPI_polled_transfer_block(0, command_buf, 3, (uint8_t*)0, 0,8);
#endif
    wait_for_wip();
}

mss_qspi_io_format Flash_probe_io_format
(
    void
)
{

#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_set_status_handler(transfer_status_handler);
#endif

    qspi_config.io_format = MSS_QSPI_QUAD_FULL;
    MSS_QSPI_configure(&qspi_config);
    Flash_readid((uint8_t*)&g_enh_v_val);
    if (0xFFu != g_enh_v_val)
    {
        return MSS_QSPI_QUAD_FULL;
    }

    qspi_config.io_format = MSS_QSPI_DUAL_FULL;
    MSS_QSPI_configure(&qspi_config);
    Flash_readid((uint8_t*)&g_enh_v_val);

    if (0xFFu != g_enh_v_val)
    {
        return MSS_QSPI_DUAL_FULL;
    }

    qspi_config.io_format = MSS_QSPI_NORMAL;
    MSS_QSPI_configure(&qspi_config);

    Flash_readid((uint8_t*)&g_enh_v_val);
    if (0xFFu != g_enh_v_val)
    {
        return MSS_QSPI_NORMAL;
    }
}

#ifdef __cplusplus
}
#endif
