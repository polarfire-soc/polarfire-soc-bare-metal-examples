/***************************************************************************//**
 * Copyright 2019-2022 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * Bare metal driver for the Winbond w25n01gv NAND flash memory.
 * This driver uses the MPFS MSS QSPI driver interface.
 */
#include "mpfs_hal/mss_hal.h"
#include "drivers/off_chip/winbond_w25n01gv/winbond_w25n01gv.h"

/* Following constant must be defined if you want to use the interrupt mode
   transfers provided by the MSS QSPI driver. Comment this out to use the
   polling mode transfers.
  */
//#define USE_QSPI_INTERRUPT      1u

#ifdef __cplusplus
extern "C" {
#endif
flash_info_t flashinfo[] = {
        { 0xEFAA21u, 2048u, 64u, 1024u, "Winbond W25N01GV" },
};

#define PAGE_LENGTH                             2048u
#define NUM_PAGES_PER_BLOCK                     64u
#define BLOCK_LENGTH                            (PAGE_LENGTH * NUM_PAGES_PER_BLOCK)
#define DIE_SIZE                                (BLOCK_LENGTH * 1024u)
#define LUT_MAX_ENTRIES                         20u

#define STATUS_REG_1                            0xA0u
#define STATUS_REG_2                            0xB0u
#define STATUS_REG_3                            0xC0u

#define STATUS_REG_2_ECC_E                      (1u << 4)
#define STATUS_REG_2_BUF                        (1u << 3)

#define STATUS_REG_3_BUSY                       (1u << 0u)
#define STATUS_REG_3_WEL                        (1u << 1u)
#define STATUS_REG_3_EFAIL                      (1u << 2u)
#define STATUS_REG_3_PFAIL                      (1u << 3u)
#define STATUS_REG_3_ECC0                       (1u << 4u)
#define STATUS_REG_3_ECC1                       (1u << 5u)
#define STATUS_REG_3_LUTF                       (1u << 6u)

#define DEVICE_RESET_OPCODE                     0xFF
#define READ_ID_OPCODE                          0x9F

#define READ_STATUS_REG_OPCODE                  0x05
#define WRITE_STATUS_REG_OPCODE                 0x01
#define WRITE_ENABLE_OPCODE                     0x06

#define PAGE_DATA_READ_OPCODE                   0x13
#define READ_DATA_OPCODE                        0x03
#define FREAD_OPCODE                            0x0B

#define BLOCK_ERASE_OPCODE                      0xD8

#define LOAD_PROGRAM_DATA_OPCODE                0x02
#define RANDOM_LOAD_PROG_DATA_OPCODE            0x84
#define PROGRAM_EXECUTE_OPCODE                  0x10

#define READ_BB_LUT_OPCODE                      0xA5
#define SWAP_BAD_BLOCK_OPCODE                   0xA1

#define FREAD_DUAL_O_OPCODE                     0x3B
#define FREAD_DUAL_O_ADDR4_OPCODE               0x3C
#define FREAD_DUAL_IO_OPCODE                    0xBB
#define FREAD_DUAL_IO_ADDR4_OPCODE              0xBC

#define FREAD_QUAD_O_OPCODE                     0x6B
#define FREAD_QUAD_O_ADDR4_OPCODE               0x6C
#define FREAD_QUAD_IO_OPCODE                    0xEB
#define FREAD_QUAD_IO_ADDR4_OPCODE              0xEC

/* 9bits CMDBYTES field (512 - 1 byte opcode - 2 byte address) */
#define MSS_QSPI_WR_BUF_LIMIT                   (511 - 3)
static mss_qspi_config_t g_qspi_config= {0};
static size_t qspi_index = -1;

#ifdef USE_QSPI_INTERRUPT
static volatile uint8_t g_rx_complete = 0u;
static volatile uint8_t g_tx_complete = 0u;
#endif

/*******************************************************************************
 * Local functions
 */
static uint8_t read_page(uint8_t* buf, uint32_t page, uint16_t column, uint32_t read_len);
static void send_write_enable_command(void);
static void disable_write_protect(void);
static uint8_t erase_block(uint32_t addr);
static void send_write_enable_command(void);
static uint8_t program_page(uint8_t* buf, uint32_t page, uint32_t wr_len);
static void read_statusreg(uint8_t status_reg_address, uint8_t* rd_buf);
static void write_statusreg(uint8_t address, uint8_t value);
static void wait_for_wip(void);

#ifdef USE_QSPI_INTERRUPT
void
transfer_status_handler
(
        uint32_t status
)
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

static void
wait_for_tx_complete
(
        void
)
{
    while (1)
    {
        if (g_tx_complete == 1)
        {
            break;
        }
    }

    g_tx_complete = 0u;
}

static void
wait_for_rx_complete
(
        void
)
{
    while (1)
    {
        if (g_rx_complete == 1)
        {
            break;
        }
    }

    g_rx_complete = 0u;
}

void
QSPI_TRANSFER_BLOCK
(
        uint8_t num_addr_bytes,
        const void * const tx_buffer,
        uint32_t tx_byte_size,
        const void * const rd_buffer,
        uint32_t rd_byte_size,
        uint8_t num_idle_cycles
)
{
            MSS_QSPI_irq_transfer_block(num_addr_bytes, tx_buffer, tx_byte_size,
                    rd_buffer, rd_byte_size, num_idle_cycles);

            if (tx_buffer && !rd_buffer)
            {
                wait_for_tx_complete();
            }

            if (rd_buffer && !tx_buffer)
            {
                wait_for_rx_complete();
            }

            if (tx_buffer && rd_byte_size && rd_buffer && tx_byte_size )
            {
               wait_for_rx_complete();
               wait_for_tx_complete();
            }
        }
#else
void
QSPI_TRANSFER_BLOCK
(
        uint8_t num_addr_bytes,
        const void * const tx_buffer,
        uint32_t tx_byte_size,
        const void * const rd_buffer,
        uint32_t rd_byte_size,
        uint8_t num_idle_cycles
)
{
    MSS_QSPI_polled_transfer_block(num_addr_bytes, tx_buffer, tx_byte_size,
            rd_buffer, rd_byte_size, num_idle_cycles);
}
#endif

bool
Flash_Get_Map
(
        const uint32_t jedec_id,
        size_t *indexOut
)
{
    bool result = false;

    for (size_t i = 0u; i < sizeof(flashinfo); i++)
    {
        if (flashinfo[i].jedecid == jedec_id)
        {
            *indexOut = i;
            result = true;
            break;
        }
    }

    return result;
}

static void
read_page_oob
(
    uint8_t* buf,
    uint32_t page,
    uint32_t column,
    uint32_t read_len
)
{
    uint32_t length = read_len;
    uint8_t tx_bytes = 0u;
    uint8_t command_buf[4] __attribute__ ((aligned (4))) = {0};
    uint8_t dummy_cycles = 0u;

    command_buf[0] = PAGE_DATA_READ_OPCODE;
    command_buf[1] = 0;
    command_buf[2] = (page >> 8u) & 0xFFu;
    command_buf[3] = page & 0xFFu;

    QSPI_TRANSFER_BLOCK(0, command_buf, 3, 0, 0, 0);
    wait_for_wip();
    for (volatile uint32_t i=0; i<100000;i++);

    command_buf[1] = (column >> 8u) & 0xFF;
    command_buf[2] = column & 0xFF;

    command_buf[2] = 0xFFu;
    command_buf[3] = 0xFFu;

    if((MSS_QSPI_QUAD_FULL == g_qspi_config.io_format) ||
       (MSS_QSPI_QUAD_EX_RO == g_qspi_config.io_format) ||
       (MSS_QSPI_QUAD_EX_RW == g_qspi_config.io_format))
    {
        /* Working combo
        command_buf[0] = FREAD_QUAD_O_OPCODE; //6b
        tx_bytes = 4u;
        dummy_cycles = 8u;

        command_buf[0] = FREAD_QUAD_IO_OPCODE; //Eb
        tx_bytes = 1u;
        dummy_cycles = 4u;

        With this we are only seeing the D0 and D1 correctly.
        D2 and D3 are still missing.
        D2,D3 are visible on the logic analyzer, QSPI controller is unable to read them.
         */
        command_buf[0] = FREAD_QUAD_IO_OPCODE; //Eb
        dummy_cycles = 4u;

    }
    else if((MSS_QSPI_DUAL_FULL == g_qspi_config.io_format) ||
            (MSS_QSPI_DUAL_EX_RO == g_qspi_config.io_format) ||
            (MSS_QSPI_DUAL_EX_RW == g_qspi_config.io_format))
    {
        /*
        command_buf[0] = FREAD_DUAL_O_OPCODE;   //3b
        tx_bytes = 3u;
        dummy_cycles = 8u;
        command_buf[0] = FREAD_DUAL_IO_OPCODE;   //bb
        tx_bytes = 1u;
        dummy_cycles = 8u;
        */
        command_buf[0] = FREAD_DUAL_O_OPCODE;   //3b
        dummy_cycles = 8u;
    }
    else
    {
        command_buf[0] = READ_DATA_OPCODE;
        dummy_cycles = 0u;
    }

    if(read_len > flashinfo[qspi_index].pagesize)
    {
        length = flashinfo[qspi_index].pagesize;
    }

    mss_qspi_io_format temp = g_qspi_config.io_format;
    MSS_QSPI_configure(&g_qspi_config);

    QSPI_TRANSFER_BLOCK(2, command_buf, tx_bytes, buf, length, dummy_cycles);

    g_qspi_config.io_format = MSS_QSPI_NORMAL;
    MSS_QSPI_configure(&g_qspi_config);
    g_qspi_config.io_format = temp;
}

static bool
is_bad_block
(
        uint32_t block_nb
)
{
    bool result = false;
    uint8_t receive_buf[64 ]={0};

    /*
     * Read bytes from the Spare area of the flash.
     */
    read_page_oob(receive_buf, (block_nb * flashinfo[qspi_index].pagesperblock),
            flashinfo[qspi_index].pagesize, 64);

    /*
     * Check for 0xFF in the First or Second Byte of spare area
     * if the bytes are non-0xFF then its a bad block.
     */
    if((receive_buf[0] != 0xFF) || (receive_buf[1] != 0xFF))
    {
      result = true;
    }

    return(result);
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
void
Flash_init
(
    mss_qspi_io_format io_format
)
{
    volatile uint8_t transmit_buffer[2];
    volatile uint8_t receive_buffer[3];
    uint8_t status_reg2_value;

    MSS_QSPI_init();
#ifdef USE_QSPI_INTERRUPT
    MSS_QSPI_set_status_handler(transfer_status_handler);
#endif
    g_qspi_config.clk_div =  MSS_QSPI_CLK_DIV_30;     //Tested OK MICRON_FAST_READ command at MSS_QSPI_CLK_DIV_12
    g_qspi_config.sample = MSS_QSPI_SAMPLE_POSAGE_SPICLK;
    g_qspi_config.spi_mode = MSS_QSPI_MODE3;
    g_qspi_config.xip = MSS_QSPI_DISABLE;
    g_qspi_config.io_format = MSS_QSPI_NORMAL;
    MSS_QSPI_configure(&g_qspi_config);

    /*store the format for later use in READ and Write APIs*/
    if((MSS_QSPI_QUAD_FULL == io_format) ||
       (MSS_QSPI_QUAD_EX_RO == io_format) ||
       (MSS_QSPI_QUAD_EX_RW == io_format))
    {
        g_qspi_config.io_format = MSS_QSPI_QUAD_EX_RW;
    }
    else if((MSS_QSPI_DUAL_FULL == io_format) ||
            (MSS_QSPI_DUAL_EX_RO == io_format) ||
            (MSS_QSPI_DUAL_EX_RW == io_format))
    {
        g_qspi_config.io_format = MSS_QSPI_DUAL_EX_RO;
    }
    else
    {
        g_qspi_config.io_format = MSS_QSPI_NORMAL;
    }

    /*Reset Flash memory*/
    transmit_buffer[0] = DEVICE_RESET_OPCODE;
    QSPI_TRANSFER_BLOCK(0, (const void * const)transmit_buffer, 0,
            (const void * const)receive_buffer, 0, 0);

    /* Read JEDSEC ID */
    for(int inc = 0; inc < sizeof(receive_buffer); inc++)
    {
        receive_buffer[inc] = 0x55;
    }

    transmit_buffer[0] = READ_ID_OPCODE;
    QSPI_TRANSFER_BLOCK(0, (const void * const)transmit_buffer, 0,
            (const void * const)receive_buffer, 3, 8);

    uint32_t jedec_id = ((receive_buffer[0] << 16) | (receive_buffer[1] <<8) |
            (receive_buffer[2]));
    if (!Flash_Get_Map(jedec_id, &qspi_index))
    {
        return;
    }

    read_statusreg(STATUS_REG_2, (uint8_t *)&status_reg2_value);

    /* Turn on Buffer mode and ECC */
    status_reg2_value |= (STATUS_REG_2_ECC_E | STATUS_REG_2_BUF);

    write_statusreg(STATUS_REG_2, status_reg2_value);
    read_statusreg(STATUS_REG_2, (uint8_t *)&status_reg2_value);
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
void
Flash_readid
(
    uint8_t* buf
)
{
    const uint8_t command_buf[1] __attribute__ ((aligned (4))) = {READ_ID_OPCODE};

    QSPI_TRANSFER_BLOCK(0, (const void * const)command_buf, 0,
            (const void * const)buf, 3, 8);
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
void
Flash_read
(
    uint8_t* buf,
    uint32_t addr,
    uint32_t len
)
{
    /* We only deal with reads from page boundaries for now. */
    ASSERT((addr % flashinfo[qspi_index].pagesize) == 0);

    uint8_t status = 0u;
    uint8_t * target_buf = buf;
    int32_t remaining_length = (int32_t)len;
    uint32_t block_size, page, target_offset, block;
    target_offset = addr;

    page = addr / flashinfo[qspi_index].pagesize;
    block_size = flashinfo[qspi_index].pagesperblock * flashinfo[qspi_index].pagesize;

    while(remaining_length > 0)
    {
        block = (uint32_t) (target_offset/block_size);
        if (is_bad_block(block))
        {
            /* Skip the bad block and move to next block */
            target_offset += block_size;
            continue;
        }

        uint32_t length;
        if(remaining_length > flashinfo[qspi_index].pagesize)
        {
            length = flashinfo[qspi_index].pagesize;
        }
        else
        {
            length = remaining_length;
        }

        page = (uint32_t)(target_offset/flashinfo[qspi_index].pagesize);
        status = read_page(target_buf, page, 0, length);
        if (status)
        {
            break;
        }

        remaining_length = remaining_length - length;
        target_offset += length;
        target_buf = target_buf + length;
    }
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
uint8_t
Flash_erase
(
    void
)
{
    uint32_t block_nb, max_blocks;
    uint8_t status = 0xFFu;

    disable_write_protect();

    for(block_nb = 0; block_nb < 1024; block_nb++)
    {
        status = erase_block(block_nb);
    }

    return(status);
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
uint8_t
Flash_program
(
    uint8_t* buf,
    uint32_t addr,
    uint32_t len
)
{
    int32_t remaining_length = (int32_t)len;
    uint32_t target_offset = addr;
    uint32_t block_size, page, block;
    uint8_t status = 0xFF;

    disable_write_protect();

    block_size = flashinfo[qspi_index].pagesperblock * flashinfo[qspi_index].pagesize;

    while(remaining_length > 0)
    {
       block = (uint32_t) (target_offset/block_size);
       if (is_bad_block(block))
       {
            /* Skip the bad block and move to next block */
            target_offset += block_size;
            continue;
        }

        uint32_t page_length;

        if(remaining_length >= BLOCK_LENGTH)
        {
            page_length = flashinfo[qspi_index].pagesize;
        }
        else
        {
            page_length = remaining_length;
        }

        page = (uint32_t)(target_offset/flashinfo[qspi_index].pagesize);
        status = program_page(buf, page, page_length);
        remaining_length -= page_length;
        target_offset += page_length;
    }

    return (status);   //flag_status_reg;
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
void
Flash_add_entry_to_bb_lut
(
    uint16_t lba,
    uint16_t pba
)
{
    uint8_t command_buf[5] __attribute__ ((aligned (4)));
    uint8_t status_reg3_value;

    read_statusreg(STATUS_REG_3, (uint8_t *)&status_reg3_value);

    if ((status_reg3_value & 0x40) == 0)
    {

        command_buf[0] = SWAP_BAD_BLOCK_OPCODE;
        command_buf[1] = (lba >> 8u) & 0xffu;
        command_buf[2] = lba & 0xffu;
        command_buf[3] = (pba >> 8u) & 0xffu;
        command_buf[4] = lba & 0xff;

        QSPI_TRANSFER_BLOCK(0, (const void * const)command_buf, 4,
                (const void * const)0, 0, 0);
    }
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
void
Flash_read_status_regs
(
    uint8_t * buf
)
{
    read_statusreg(STATUS_REG_1, buf);
    read_statusreg(STATUS_REG_2, buf+1);
    read_statusreg(STATUS_REG_3, buf+2);
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
uint8_t
Flash_read_bb_lut
(
    w25_bb_lut_entry_t* lut_ptr
)
{
    uint8_t num_bb;
    uint8_t buf[4*LUT_MAX_ENTRIES] = {0};
    uint8_t command_buf[4] __attribute__ ((aligned (4)));

    /*
     * Read Bad block LUT
     * 8 dummy cycles
     * 20 LUT entries of 4 bytes each
     */
    command_buf[0] = READ_BB_LUT_OPCODE;
    QSPI_TRANSFER_BLOCK(0, command_buf, 0, buf, 80, 8);

    for (int lut_idx = 0u, buf_idx = 0u; lut_idx < LUT_MAX_ENTRIES; lut_idx++, buf_idx +=4)
    {
        lut_ptr[lut_idx].enable = ((buf[buf_idx] & 0x80u)? 1u : 0u);
        lut_ptr[lut_idx].invalid = ((buf[buf_idx] & 0x40u)? 1u : 0u);
        lut_ptr[lut_idx].lba = (((uint16_t)(buf[buf_idx] & 0x3u) << 8u) | buf[buf_idx+1]);
        lut_ptr[lut_idx].pba = (((uint16_t)buf[buf_idx+2] << 8u) | buf[buf_idx+3]);
    }

    for (int lut_idx = 0; lut_idx < LUT_MAX_ENTRIES; lut_idx++)
    {
        if ((1u == lut_ptr[lut_idx].enable) && (0u == lut_ptr[lut_idx].invalid))
        {
            num_bb++;
        }
    }

    return num_bb;
}

/***************************************************************************//**
 * See winbond_w25n01gv.h for details of how to use this function.
 */
uint32_t Flash_scan_for_bad_blocks(uint16_t* buf)
{
    volatile uint32_t block_num, bad_count, page_num;
    uint8_t receive_buf[PAGE_LENGTH]={0};

    bad_count = 0;
    page_num = 0;

    for(block_num = 0; block_num < 1024; block_num++)
    {
        read_page_oob(receive_buf, (block_num * flashinfo[qspi_index].pagesperblock),\
                    flashinfo[qspi_index].pagesize, flashinfo[qspi_index].pagesize );

        if((receive_buf[0] != 0xFF) || (receive_buf[1] != 0xFF))
        {
            buf[bad_count++] = block_num;
        }

        page_num += 64;
    }

    return(bad_count);
}

/*******************************************************************************
 * Local functions
 */

static void
send_write_enable_command
(
    void
)
{
    uint8_t command_buf[1] __attribute__ ((aligned (4)));

    command_buf[0] = WRITE_ENABLE_OPCODE;
    QSPI_TRANSFER_BLOCK(0, command_buf, 0, (uint8_t*)0, 0,0);
}

static void
disable_write_protect
(
    void
)
{
    write_statusreg(STATUS_REG_1, 0x0u);
}

static uint8_t
erase_block
(
    uint32_t block_nb
)
{
    uint8_t command_buf[4] __attribute__ ((aligned (4)));
    uint8_t status;

    if (is_bad_block (block_nb))
    {
        return STATUS_REG_3_EFAIL;
    }
    else
    {
        send_write_enable_command();

        command_buf[0] = BLOCK_ERASE_OPCODE;
        command_buf[1] = 0u;
        command_buf[2] = ((block_nb * 64) >> 8u) & 0xFFu;
        command_buf[3] = (block_nb * 64) & 0xFFu;


        QSPI_TRANSFER_BLOCK(0, command_buf, 3, (uint8_t*)0, 0,0);

        do
        {
            read_statusreg(STATUS_REG_3, &status);
        } while ((STATUS_REG_3_BUSY & status) || (STATUS_REG_3_WEL & status));

        return (STATUS_REG_3_EFAIL & status);
    }
}

static uint8_t
program_page
(
    uint8_t* buf,
    uint32_t page,
    uint32_t wr_len
)
{
    uint8_t command_buf[PAGE_LENGTH + 3] __attribute__ ((aligned (4))) = {0};
    uint16_t idx;
    uint16_t column = 0;
    uint8_t status = 0xFFu;
    int32_t remaining_length = (int32_t)wr_len;

    ASSERT(wr_len <= flashinfo[qspi_index].pagesize);
    while(remaining_length > 0u)
    {
        uint32_t subpage_length;

        if (remaining_length > MSS_QSPI_WR_BUF_LIMIT)
        {
            subpage_length = MSS_QSPI_WR_BUF_LIMIT;
        }
        else
        {
            subpage_length = remaining_length;
        }

        command_buf[0] = RANDOM_LOAD_PROG_DATA_OPCODE;
        command_buf[1] = ( column>> 8) & 0xFFu;
        command_buf[2] =  column & 0xFFu;
        for (idx = 0; idx < (subpage_length); idx++)
        {
            command_buf[3 + idx] = buf[column + idx];
        }

        send_write_enable_command();
        QSPI_TRANSFER_BLOCK(2, command_buf, subpage_length, (uint8_t*)0,
                0,0);
        read_statusreg(STATUS_REG_3, &status);

        if(remaining_length >= subpage_length)
        {
            remaining_length -= subpage_length;
        }
        else
        {
            ASSERT(0u);
        }

        column += subpage_length;
    }

    command_buf[0] = PROGRAM_EXECUTE_OPCODE;
    command_buf[1] = 0u;
    command_buf[2] = (page >> 8) & 0xFFu;
    command_buf[3] = page & 0xFFu;
    send_write_enable_command();
    QSPI_TRANSFER_BLOCK(0, command_buf, 3, (uint8_t*)0, 0,0);

    do
    {
        read_statusreg(STATUS_REG_3, &status);
    } while ((STATUS_REG_3_BUSY & status) || (STATUS_REG_3_WEL & status));

    return ((STATUS_REG_3_ECC1 | STATUS_REG_3_PFAIL) & status);
}

/*
 * Make sure that the program/erase operation is complete. i.e. wait for
 * busy bit to go 0.
 */
static void
wait_for_wip
(
    void
)
{
    uint8_t status_reg = 0;
    do
    {
        read_statusreg(STATUS_REG_3, (uint8_t*)&status_reg);
    } while (STATUS_REG_3_BUSY & status_reg);
}

static void
write_statusreg
(
    uint8_t address,
    uint8_t value
)
{
    uint8_t command_buf[3] __attribute__ ((aligned (4)));

    command_buf[0] = WRITE_STATUS_REG_OPCODE;
    command_buf[1] = address;
    command_buf[2] = value;

    QSPI_TRANSFER_BLOCK(0, command_buf, 2, (uint8_t*)0, 0,0);
}

static void
read_statusreg
(
    uint8_t status_reg_address,
    uint8_t* rd_buf
)
{
    const uint8_t command_buf[2] __attribute__ ((aligned (4))) =
    {
        READ_STATUS_REG_OPCODE,
        status_reg_address
    };

    QSPI_TRANSFER_BLOCK(1, command_buf, 0, rd_buf, 1,0);
}

static uint8_t
read_page
(
    uint8_t* buf,
    uint32_t page,
    uint16_t column,
    uint32_t read_len
)
{
    uint32_t length = read_len;
    uint8_t tx_bytes = 0u;
    uint8_t command_buf[4] __attribute__ ((aligned (4))) = {0};
    uint8_t dummy_cycles = 0u;
    uint8_t status;

    wait_for_wip();

    command_buf[0] = PAGE_DATA_READ_OPCODE;
    command_buf[1] = 0;
    command_buf[2] = (page >> 8u) & 0xFFu;
    command_buf[3] = page & 0xFFu;

    QSPI_TRANSFER_BLOCK(0, command_buf, 3, 0, 0, 0);
    wait_for_wip();

    for (volatile uint32_t i=0; i<100000;i++);
    command_buf[1] = (column >> 8u) & 0xFFu;
    command_buf[2] = column & 0xFFu;
    tx_bytes = 0u;

    if((MSS_QSPI_QUAD_FULL == g_qspi_config.io_format) ||
       (MSS_QSPI_QUAD_EX_RO == g_qspi_config.io_format) ||
       (MSS_QSPI_QUAD_EX_RW == g_qspi_config.io_format))
    {
        /*
         * command_buf[0] = FREAD_QUAD_O_OPCODE; //6b
         * tx_bytes = 4u;
         * dummy_cycles = 8u;
         *
         * command_buf[0] = FREAD_QUAD_IO_OPCODE; //Eb
         * tx_bytes = 1u;
         * dummy_cycles = 4u;
         *
         * With this we are only seeing the D0 and D1 correctly.
         * D2 and D3 are still missing.
         * D2,D3 are visible on the logic analyzer,
         * QSPI controller is unable to read them.
         */
        command_buf[0] = FREAD_QUAD_IO_OPCODE; //Eb
        dummy_cycles = 4u;
    }
    else if((MSS_QSPI_DUAL_FULL == g_qspi_config.io_format) ||
            (MSS_QSPI_DUAL_EX_RO == g_qspi_config.io_format) ||
            (MSS_QSPI_DUAL_EX_RW == g_qspi_config.io_format))
    {
        /*
         * command_buf[0] = FREAD_DUAL_O_OPCODE;   //3b
         * tx_bytes = 3u;
         * dummy_cycles = 8u;
         * command_buf[0] = FREAD_DUAL_IO_OPCODE;   //bb
         * tx_bytes = 1u;
         * dummy_cycles = 8u;
         */
        command_buf[0] = FREAD_DUAL_O_OPCODE;   //3b
        dummy_cycles = 8u;
    }
    else
    {
        command_buf[0] = READ_DATA_OPCODE;
        dummy_cycles = 8u;
    }

    if(read_len > flashinfo[qspi_index].pagesize)
    {
        length = flashinfo[qspi_index].pagesize;
    }

    mss_qspi_io_format temp = g_qspi_config.io_format;
    MSS_QSPI_configure(&g_qspi_config);
    QSPI_TRANSFER_BLOCK(2, command_buf, tx_bytes, buf, length, dummy_cycles);
    g_qspi_config.io_format = MSS_QSPI_NORMAL;
    MSS_QSPI_configure(&g_qspi_config);
    g_qspi_config.io_format = temp;

    do
    {
        read_statusreg(STATUS_REG_3, &status);
    } while ((STATUS_REG_3_BUSY & status) || (STATUS_REG_3_WEL & status));

    status &= (STATUS_REG_3_ECC1 & STATUS_REG_3_ECC0);

    return status;
}

#ifdef __cplusplus
}
#endif /* MSS_WINBOND_W25N_H_*/
