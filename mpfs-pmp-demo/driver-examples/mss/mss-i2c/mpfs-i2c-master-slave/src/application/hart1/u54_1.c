/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solution.
 *
 * SPDX-License-Identifier: MIT
 *
 * Application code running on U54_1
 *
 * Example project demonstrates reading and writing data between a pair of MSS
 * I2Cs configured as a master and a slave.
 * Please refer README.txt in the root folder of this example project.
 *
 */

#include <stdint.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_i2c/mss_i2c.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"

volatile uint32_t count_sw_ints_h1 = 0U;
uint32_t menu_command;
uint64_t uart_lock;

/*------------------------------------------------------------------------------
 * MSS I2C instance
 */
#define I2C_MASTER &g_mss_i2c0_lo
#define I2C_SLAVE  &g_mss_i2c1_lo

/*------------------------------------------------------------------------------
 * I2C master serial address.
 */
#define MASTER_SER_ADDR     0x21

/*-----------------------------------------------------------------------------
 * I2C slave serial address.
 */
#define SLAVE_SER_ADDR      0x30

/*-----------------------------------------------------------------------------
 * Receive buffer size.
 */
#define BUFFER_SIZE    32u
#define ENTER          13u

/*-----------------------------------------------------------------------------
 * I2C operation timeout value in mS. Define as MSS_I2C_NO_TIMEOUT to disable
 * the timeout functionality.
 */
#define DEMO_I2C_TIMEOUT 3000u

#define RX_BUFF_SIZE    64

uint8_t g_rx_buff[RX_BUFF_SIZE] = {0};

/*-----------------------------------------------------------------------------
 * Local functions.
 */
mss_i2c_slave_handler_ret_t slave_write_handler(mss_i2c_instance_t *, uint8_t *, uint16_t);
mss_i2c_status_t do_write_transaction(uint8_t, uint8_t * , uint8_t);
mss_i2c_status_t do_read_transaction(uint8_t, uint8_t * , uint8_t);
mss_i2c_status_t do_write_read_transaction(uint8_t , uint8_t * , uint8_t , uint8_t * , uint8_t);
static void display_greeting(void);
static void select_mode_i2c(void);
uint8_t get_data(void);
void press_any_key_to_continue(void);
void i2c0_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status);
void i2c1_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status);

/*------------------------------------------------------------------------------
 * I2C buffers. These are the buffers where data written transferred via I2C
 * will be stored. RX
 */
static uint8_t g_slave_rx_buffer[BUFFER_SIZE];
static uint8_t g_slave_tx_buffer[BUFFER_SIZE] = "<<-------Slave Tx data ------->>";
static uint8_t g_master_rx_buf[BUFFER_SIZE];
static uint8_t g_master_tx_buf[BUFFER_SIZE];

/*------------------------------------------------------------------------------
 * Counts of data sent by master and received by slave.
 */
static uint8_t g_tx_length = 0x00;

/******************************************************************************
 * Global variable
 */
mss_uart_instance_t *g_uart= &g_mss_uart1_lo;

/* Main function for the hart1(U54_1 processor).
 * Application code running on hart1 is placed here
 *
 * The hart1 goes into WFI. hart0 brings it out of WFI when it raises the first
 * Software interrupt to this hart
 */
void u54_1(void)
{
    uint64_t hartid = read_csr(mhartid);
    volatile uint32_t icount = 0U;
    uint8_t loop_count;
    uint8_t rx_buff[1];
    uint8_t rx_size = 0;
    mss_i2c_status_t instance;

#if (IMAGE_LOADED_BY_BOOTLOADER == 0)
    /* Clear pending software interrupt in case there was any.
     * Enable only the software interrupt so that the E51 core can bring this
     * core out of WFI by raising a software interrupt. */
    clear_soft_interrupt();
    set_csr(mie, MIP_MSIP);

    /* Put this hart in WFI. */
    do
    {
        __asm("wfi");
    } while (0 == (read_csr(mip) & MIP_MSIP));
#endif

    /* The hart is out of WFI, clear the SW interrupt. Here onwards Application
     * can enable and use any interrupts as required */
    clear_soft_interrupt();

    (void)mss_config_clk_rst(MSS_PERIPH_MMUART1, (uint8_t) 1, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_I2C0, (uint8_t) 1, PERIPHERAL_ON);
    (void)mss_config_clk_rst(MSS_PERIPH_I2C1, (uint8_t) 1, PERIPHERAL_ON);

    PLIC_init();
    PLIC_SetPriority(I2C0_MAIN_PLIC, 2);
    PLIC_SetPriority(I2C1_MAIN_PLIC, 2);

    __enable_irq();

    /* All harts use MMUART0 to display messages on Terminal. This mutex helps
     * serializing MMUART0 accesses from multiple harts. */
    MSS_UART_init(g_uart,
            MSS_UART_115200_BAUD,
            MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    /*-------------------------------------------------------------------------
     * Initialize the MSS I2C slave driver with its I2C serial address and
     * serial clock divider.
     */
    MSS_I2C_init(I2C_SLAVE, SLAVE_SER_ADDR, MSS_I2C_PCLK_DIV_192);
    MSS_I2C_set_slave_tx_buffer(I2C_SLAVE, g_slave_tx_buffer,
                                sizeof(g_slave_tx_buffer));
    MSS_I2C_set_slave_mem_offset_length(I2C_SLAVE, 2);
    MSS_I2C_set_slave_rx_buffer(I2C_SLAVE, g_slave_rx_buffer,
                                sizeof(g_slave_rx_buffer));
    MSS_I2C_register_write_handler(I2C_SLAVE, slave_write_handler);
    MSS_I2C_register_transfer_completion_handler(I2C_SLAVE,
                                                 i2c1_completion_handler);

    /*-------------------------------------------------------------------------
     * Enable I2C Slave.
     */
    MSS_I2C_enable_slave(I2C_SLAVE);

    /*-------------------------------------------------------------------------
     * Initialize the MSS I2C master Driver
     */
    MSS_I2C_init(I2C_MASTER, MASTER_SER_ADDR, MSS_I2C_PCLK_DIV_192);
    MSS_I2C_register_transfer_completion_handler(I2C_MASTER,
                                                 i2c0_completion_handler);
    /*
     * If the systick 10mS calibration factor is set correctly in your design
     * you can use the following:
     * SysTick_Config((SysTick->CALIB));
     */
    SysTick_Config();

    /*-------------------------------------------------------------------------
     * Display the initial information about the demo followed by the main
     * menu.
     */
    display_greeting();
    select_mode_i2c();
    for (loop_count = 0; loop_count < BUFFER_SIZE; loop_count++)
    {
        g_slave_rx_buffer[loop_count] = 0x00;
        g_master_rx_buf[loop_count] = 0x00;
        g_master_tx_buf[loop_count] = 0x00;
    }

    /*--------------------------------------------------------------------------
     * Infinite loop processing user commands received over the UART command
     * line interface.
     */
    do
    {
        /* Start command line interface if any key is pressed. */
        rx_size = MSS_UART_get_rx(g_uart, rx_buff, sizeof(rx_buff));
        if (rx_size > 0)
        {
            switch (rx_buff[0])
            {
                case '1':
                    /*-------------------------------------------------------------------------
                     * Send The data from Master
                     */
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\n\rMT-SR Mode is Selected\n\r");
                    g_tx_length = get_data();
                    /* Perform Master Transmit - Slave Receive */
                    instance = do_write_transaction(SLAVE_SER_ADDR, g_master_tx_buf, g_tx_length);

                    if (MSS_I2C_SUCCESS == instance)
                    {
                    if (0 == g_tx_length)
                    {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"0 Byte Data Write Successful\n\r");
                    }
                    else
                    {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write Successful and Data is: ");
                            MSS_UART_polled_tx(g_uart, g_slave_rx_buffer, g_tx_length);
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\n\r");
                    }
                        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"------------------------------------------------------------------------------\n\r");
                    }
                    else
                    {
                        /*
                         * Distinguish between an identified failure, a time out and just to be paranoid
                         * none of the above.
                         */
                        if (MSS_I2C_FAILED == instance)
                        {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write Failed!\n\r");
                        }
                        else if (MSS_I2C_TIMED_OUT == instance)
                        {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write Timed Out!\n\r");
                        }
                        else
                        {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write Unknown Response!\n\r");
                        }

                        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r------------------------------------------------------------------------------\n\r");
                    }
                    /* Display I2C Modes */
                    press_any_key_to_continue();
                break;

                case '2':
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\n\rMR-ST Mode is Selected\n\r");
                    /* Perform Master Receive - Slave Transmit */
                    instance = do_read_transaction(SLAVE_SER_ADDR, g_master_rx_buf, sizeof(g_master_rx_buf));

                    if (MSS_I2C_SUCCESS == instance)
                    {
                        MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Read Successful and Data is: ");
                        MSS_UART_polled_tx(g_uart, g_master_rx_buf, sizeof(g_master_rx_buf));
                        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r------------------------------------------------------------------------------\n\r");
                    }
                    else
                    {
                        /*
                         * Distinguish between an identified failure, a time out and just to be paranoid
                         * none of the above.
                         */
                        if (MSS_I2C_FAILED == instance)
                        {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Read Failed!\n\r");
                        }
                        else if (MSS_I2C_TIMED_OUT == instance)
                        {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Read Timed Out!\n\r");
                        }
                        else
                        {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Read Unknown Response!\n\r");
                        }

                        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r------------------------------------------------------------------------------\n\r");
                    }
                    /* Display I2C Modes */
                    press_any_key_to_continue();

                break;

                case '3':
                    /*-------------------------------------------------------------------------
                     * Write the Data to Slave and Read The data from Slave
                     */
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\n\rMT-MR Mode is Selected\n\r");
                    /* Perform Master Transmit - Master Receive */
                    g_tx_length = get_data();
                    if (0 == g_tx_length)
                    {
                        MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Error: 0 byte transfers not allowed for Write-Read!\n\r");
                    }
                    else
                    {
                        /* Perform write-Read operation */
                        instance = do_write_read_transaction(SLAVE_SER_ADDR, g_master_tx_buf, g_tx_length, g_master_rx_buf, g_tx_length);

                        if (MSS_I2C_SUCCESS == instance)
                        {
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write-Read Successful\n\r");
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Received in Slave Receive Buffer is: ");
                            MSS_UART_polled_tx(g_uart, g_slave_rx_buffer, g_tx_length);
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\n\rData Received to Master Receive Buffer is: ");
                            MSS_UART_polled_tx(g_uart, g_master_rx_buf, g_tx_length);
                            MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r------------------------------------------------------------------------------\n\r");
                        }
                        else
                        {
                            /*
                             * Distinguish between an identified failure, a time out and just to be paranoid
                             * none of the above.
                             */
                            if (MSS_I2C_FAILED == instance)
                            {
                                MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write-Read Failed!\n\r");
                            }
                            else if (MSS_I2C_TIMED_OUT == instance)
                            {
                                MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write-Read Timed Out!\n\r");
                            }
                            else
                            {
                                MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"Data Write-Read Unknown Response!\n\r");
                            }

                            MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r------------------------------------------------------------------------------\n\r");
                        }
                    }
                    /* Display I2C Modes */
                    press_any_key_to_continue();

                break;

                case '4':
                    /* To Exit from the application */
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\n\rReturn from the Main function \n\r\n\r");
                break;

                default:
                    /* To Invalid Entry */
                    MSS_UART_polled_tx_string(g_uart, (const uint8_t *)"\n\rEnter A Valid Key: 1, 2, 3 or 4\n\r");
                    select_mode_i2c();
                break;
            }
        }
    } while (rx_buff[0] !='4');
    /* Never return */
}


/*------------------------------------------------------------------------------
 * Perform read transaction with parameters gathered from the command line
 * interface. This function is called as a result of the user's input in the
 * command line interface.
 */
mss_i2c_status_t do_read_transaction
(
    uint8_t serial_addr,
    uint8_t * rx_buffer,
    uint8_t read_length
)
{
    mss_i2c_status_t status;

    MSS_I2C_read(I2C_MASTER, serial_addr, rx_buffer, read_length,
                 MSS_I2C_RELEASE_BUS);

    status = MSS_I2C_wait_complete(I2C_MASTER, DEMO_I2C_TIMEOUT);

    return status;
}

/*------------------------------------------------------------------------------
 * Perform write transaction with parameters gathered from the command line
 * interface. This function is called as a result of the user's input in the
 * command line interface.
 */
mss_i2c_status_t do_write_transaction
(
    uint8_t serial_addr,
    uint8_t * tx_buffer,
    uint8_t write_length
)
{
    mss_i2c_status_t status;

    MSS_I2C_write(I2C_MASTER, serial_addr, tx_buffer, write_length,
                  MSS_I2C_RELEASE_BUS);

    status = MSS_I2C_wait_complete(I2C_MASTER, DEMO_I2C_TIMEOUT);

    return status;
}

/*------------------------------------------------------------------------------
 * Perform write-read transaction with parameters gathered from the command
 * line interface. This function is called as a result of the user's input in
 * the command line interface.
 */
mss_i2c_status_t do_write_read_transaction
(
    uint8_t serial_addr,
    uint8_t * tx_buffer,
    uint8_t write_length,
    uint8_t * rx_buffer,
    uint8_t read_length
)
{
    mss_i2c_status_t status;

    MSS_I2C_write_read(I2C_MASTER,
                       serial_addr,
                       tx_buffer,
                       write_length,
                       rx_buffer,
                       read_length,
                       MSS_I2C_RELEASE_BUS);

    status = MSS_I2C_wait_complete(I2C_MASTER, DEMO_I2C_TIMEOUT);

    return status;
}
/*------------------------------------------------------------------------------
 * Slave write handler function called as a result of a the I2C slave being the
 * target of a write transaction. This function simply stores the received data
 * in g_slave_tx_buffer.
 */
mss_i2c_slave_handler_ret_t slave_write_handler
(
    mss_i2c_instance_t * this_i2c,
    uint8_t * p_rx_data,
    uint16_t rx_size
)
{
    uint8_t loop_count;

    if (rx_size > BUFFER_SIZE) /* Safety check and limit the data length */
    {
        rx_size = BUFFER_SIZE;
    }

    /* Copy only the data we have received */
    for (loop_count = 0; loop_count < rx_size; loop_count++)
    {
        g_slave_tx_buffer[loop_count] = g_slave_rx_buffer[loop_count];
    }

    return MSS_I2C_REENABLE_SLAVE_RX;
}

/*------------------------------------------------------------------------------
 * I2C-0 completion handler
 */
void i2c0_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status)
{
    if (status == MSS_I2C_SUCCESS)
    {
        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\rI2C0 Transfer completed.\n\r");
    }
}

/*------------------------------------------------------------------------------
 * I2C-1 completion handler
 */
void i2c1_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status)
{
    if (status == MSS_I2C_SUCCESS)
    {
        MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\rI2C1 Transfer completed.\n\r");
    }
}

/*------------------------------------------------------------------------------
 * Display greeting message when application is started.
 */
static void display_greeting (void)
{
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r******************************************************************************\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"************************ PolarFire SoC MSS I2C Example ***********************\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"******************************************************************************\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"This example project demonstrates the use of I2C in Different Modes\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"MSS I2C0 is configured in Master Mode and MSS I2C1 is configured in Slave Mode\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r------------------------------------------------------------------------------\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"************* I2C Modes supported by this example project are ****************\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"1. MT-SR :- Master Transmit - Slave Receiver Mode (Write To Slave)\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"2. MR-ST :- Master Receive  - Slave transmit Mode (Read 32 bytes From Slave)\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"3. MT-MR :- Master Transmit - Master Receive Mode (Write To + Read From Slave) \n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"------------------------------------------------------------------------------\n\r");
}

/*------------------------------------------------------------------------------
 * Select the I2C Mode.
 */
static void select_mode_i2c (void)
{
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r*********************** Select the I2C Mode to perform ***********************\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Press Key '1' to perform MT-SR (Master Transmit - Slave Receive)\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Press Key '2' to perform MR-ST (Master Receive  - Slave transmit)\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Press Key '3' to perform MT-MR (Master Transmit - Master Receive)\n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"Press Key '4' to EXIT from the Application \n\r");
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"------------------------------------------------------------------------------\n\r");
}

/*------------------------------------------------------------------------------
 * Function to get the key from user
 */
uint8_t get_data ()
{
    uint8_t complete = 0;
    uint8_t rx_buff[1];
    uint8_t count = 0;
    uint8_t rx_size = 0;

    /*--------------------------------------------------------------------------
     * Read the key strokes entered by user and store them for transmission to
     * the slave.
     */
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\rEnter up to 32 characters to write to I2C1: ");
    count = 0;
    while (!complete)
    {
        rx_size = MSS_UART_get_rx(g_uart, rx_buff, sizeof(rx_buff));
        if (rx_size > 0)
        {
            MSS_UART_polled_tx(g_uart, rx_buff, sizeof(rx_buff));

            /* Is it to terminate from the loop */
            if (ENTER == rx_buff[0])
            {
                complete = 1;
            }
            /* Is a character to add to our transmit string */
            else
            {
                g_master_tx_buf[count] = rx_buff[0];
                count++;
                if (32 == count)
                {
                   complete = 1;
                }
            }
        }
    }
    MSS_UART_polled_tx_string(g_uart, (const uint8_t*)"\n\r");

    return(count);
}

/*------------------------------------------------------------------------------
 * Display "Press any key to continue." message and wait for key press.
 */
void press_any_key_to_continue(void)
{
    size_t rx_size;
    uint8_t rx_char;
    uint8_t press_any_key_msg[] = "\n\rPress any key to continue.";
    MSS_UART_polled_tx(g_uart, press_any_key_msg, sizeof(press_any_key_msg));
    do {
        rx_size = MSS_UART_get_rx(g_uart, &rx_char, sizeof(rx_char));
    } while (rx_size == 0);
    select_mode_i2c();
}

/*------------------------------------------------------------------------------
 * Service the I2C timeout functionality (SysTick_Handler is called every 10mS).
 */
void SysTick_Handler_h1_IRQHandler(void)
{
    MSS_I2C_system_tick(I2C_MASTER, 10);
    MSS_I2C_system_tick(I2C_SLAVE, 10);
}

/* hart1 Software interrupt handler */
void Software_h1_IRQHandler(void)
{
    uint64_t hart_id = read_csr(mhartid);
	count_sw_ints_h1++;
}
