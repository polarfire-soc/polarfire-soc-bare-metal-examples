
/*******************************************************************************
* Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
*
* SPDX-License-Identifier: MIT
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
* @file mcp7941x6_click.h
* @author Microchip FPGA Embedded Systems Solutions
* @brief mcp7941x6 RTC bare metal software driver public API.
*/

#ifndef MCP7941X_H_
#define MCP7941X_H_

/************************************************************
 * Includes section  
 ************************************************************/
#include<stdbool.h>
#include<stdint.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_i2c/mss_i2c.h"

/************************************************************
 *  registers and constants
 ************************************************************/
#ifdef __cplusplus
extern "C" {
#endif



#define MCP7941x_SECONDS_ADDR   0x00U   
#define MCP7941x_SECONDS_MASK( value ) ( value & 0x7fU )
#define MCP7941x_SECONDS_CLEAR( value ) ( value & 0x80U )

#define MCP7941x_MINUTES_ADDR   0x01U
#define MCP7941x_MINUTES_MASK( value ) ( value & 0x7fU )
#define MCP7941x_MINUTES_CLEAR( value ) ( value & 0x80U )

#define MCP7941x_HOURS_ADDR     0x02U
#define MCP7941x_HOURS_MASK( value ) ( value & 0x3fU )
#define MCP7941x_HOURS_CLEAR( value ) ( value & 0xc0U )

#define MCP7941x_DAY_ADDR       0x03U
#define MCP7941x_DAY_MASK( value ) ( value & 0x07U )
#define MCP7941x_DAY_CLEAR( value ) ( value & 0xf8U )

#define MCP7941x_DATE_ADDR      0x04U
#define MCP7941x_DATE_MASK( value ) ( value & 0x3fU )
#define MCP7941x_DATE_CLEAR( value ) ( value & 0xc0U )

#define MCP7941x_MONTH_ADDR     0x05U
#define MCP7941x_MONTH_MASK( value ) ( value & 0x1fU )
#define MCP7941x_MONTH_CLEAR( value ) ( value & 0xe0U )

#define MCP7941x_YEAR_ADDR      0x06U
#define MCP7941x_YEAR_MASK( value ) ( value & 0xffU )
#define MCP7941x_YEAR_CLEAR( value ) ( value & 0x00U )

#define MCP7941x_CONFIG_ADDR    0x07U

#define MCP7941x_START_OSC_BIT  0x07U
#define MCP7941x_START_OSC_MASK ( 1 << MCP7941x_START_OSC_BIT )

#define MCP7941x_TIMEDATE_BYTES 0x07U
#define MCP7941x_SECONDS_BYTE   0x00U
#define MCP7941x_MINUTES_BYTE   0x01U
#define MCP7941x_HOUR_BYTE      0x02U
#define MCP7941x_DAY_BYTE       0x03U
#define MCP7941x_DATE_BYTE      0x04U
#define MCP7941x_MONTH_BYTE     0x05U
#define MCP7941x_YEAR_BYTE      0x06U

/* MCP7941x slave address */ 

#define MCP7941X_SLAVE         0x6FU
#define MCP7941X_SRAM_SLAVE    0x6FU
#define MCP7941X_EEPROM_SLAVE  0x57U
#define RAM_SIZE               64U
#define RAM_START              0x20U
#define RAM_END                0x5fU
#define EEPROM_SIZE            128 * 8
#define EEPROM_START           0x00U
#define EEPROM_END             EEPROM_SIZE
#define EEPROM_PAGE_SIZE       0x08U
#define EEPROM_STATUS          0xFFU

#define JULIAN_DAY_1970 2440588UL /*julian day calculation for jan 1 1970*/
#define TIME_SEC_IN_MIN             60U         
#define TIME_SEC_IN_HOUR            (TIME_SEC_IN_MIN * 60) 
#define TIME_SEC_IN_24_HOURS        (TIME_SEC_IN_HOUR * 24)


/************************************************************
 *Type definitions .
 ************************************************************/
typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t weekday;
    uint8_t monthday;
    uint8_t month;
    uint8_t year;
} mcp7941x_time_t;

/**
 *  Output Modes
 */
typedef enum
{
    MCP7941x_1HZ,      /**< 1 HZ Square wave output*/
    MCP7941x_4_096KHZ, /**< 4096 HZ Square wave output*/
    MCP7941x_8_192KHZ, /**< 8192 HZ Square wave output*/
    MCP7941x_32_768KHZ /**< 32768 HZ Square wave output*/
} mcp7941x_swo_t;

/**
 * Alarm selection  
 */
typedef enum
{
    MCP7941x_ALARM_0,
    MCP7941x_ALARM_1,
} mcp7941x_alarm_t;

/**
 *  Alarm Trigger time settings
 */

typedef enum
{
    MCP7941x_ALARM_SECONDS, /**< Alarm triggers when seconds match*/
    MCP7941x_ALARM_MINUTES, /**< Alarm triggers when minutes match*/
    MCP7941x_ALARM_HOURS,   /**< Alarm triggers when hours match*/
    MCP7941x_ALARM_DAY,     /**< Alarm triggers when days match*/
    MCP7941x_ALARM_DATE,    /**< Alarm triggers when date matches*/
    MCP7941x_ALARM_SEC_MIN_HOUR_DAY_DATE_MONTH, /**< Alarm triggers when all above matches*/
    MCP7941x_ALARM_WEEKDAY /**< Alarm triggers when weekday matches*/
} mcp7941x_alarm_trigger_t;


/**
 *  Month Definitions
 */
typedef enum
{
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
} mcp7941x_month_t;


/**
 * Day of Week Definition
 */

typedef enum
{
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
} mcp7941x_day_t;


/************************************************************
 * MCP7941x6 Generic functions.
 ************************************************************/
/**
 * @brief Initializes the proper i2c slave address and sets appropriate
 * function pointers
 * @param this_i2c - MSS_i2c_instanct_t either i2c0 or i2c1
 * @param slave_addr - Desired i2c slave address
 */

void mcp7941x_init_generic(void* this_i2c, uint8_t slave_addr );

/**
 * @brief Changes the i2c slave address
 *
 * @param slave_addr  - Desired i2c slave address
 */

void mcp7941x_set_slave( uint8_t slave_addr );

/**
 * @brief Sends data through the i2c bus
 *
 * @param reg_addr[IN] - Desired slave register address to write to
 * @param tx_data [IN] - pointer to the data to be written
 * @param tx_size[IN] - Number of bytes to write
 */

void mcp7941x_write_generic( uint8_t reg_addr, uint8_t *tx_data, uint16_t tx_size );

/**
 * @brief Reads data from slave through i2c
 *
 * @param reg_addr[IN] - Desired register address inside the i2c slave
 * @param rx_data [OUT] - Buffer to store the read data to
 * @param rx_size [IN] - Number of bytes to be read
 */

void mcp7941x_read_generic ( uint8_t reg_addr, uint8_t *rx_data, uint16_t rx_size);

/**
 * @brief sets the mss I2C instance either as I2C1 or I2C0  
 * @param mss_i2c_instance_t*[IN] - mss_i2c_instance_t 
*/

void mcp7941x_set_i2c_inst(mss_i2c_instance_t*);


/************************************************************
 * MCP7941x6 settings Functions 
 ************************************************************/

/**
 * @brief Initializes MCP7941x based on type and time zone also enables battery power. 
 *
 * @param time_zone_hours - gmt offset of the zone hours
 * @param time_zone_minutes - gmt offset of the zone  minutes
 *
 * @return
 *  @retval 0 - successful
 *  @retval -1 - error occured
 *
 * @code
 * if( mcp7941x_init( &g_mss_i2c0_lo, -8 ) )
 *     return error;
 * @endcode
 */

uint8_t mcp7941x_init( void *this_i2c, int8_t time_zone_hours, uint8_t time_zone_minutes );

/**
 * @brief Enables Daylight Savings Time compensation
 *
 */

void mcp7941x_enable_dst( void );

/**
 * @brief Disables Daylight Savings Time compensation
 *
 */
void mcp7941x_disable_dst( void );

/**
 * @brief Enable Square Wave Output
 *
 * @param swo - Frequency of output
 *
 * @code
 * mcp7941x_enable_swo( MCP7941x_1HZ );
 * @endcode
 */

void mcp7941x_enable_swo( mcp7941x_swo_t swo );

/**
 * @brief Disables Square Wave Output
 *
 * @note Not supported by all models
 */

void mcp7941x_disable_swo( void );

/**
 * @brief Enables automatic switch to battery on VCC failure
 *
 * @note Not supported by all models
 */

void mcp7941x_battery_enable( void );

/**
 * @brief Disables automatic switch to battery on VCC failure
 *
 * @note Not supported by all models
 */

void mcp7941x_battery_disable( void );


/****************************************
 *         General Usage                *
 ***************************************/
/**
 * @brief Gets the current gmt time set in the MCP7941x
 *
 * @return Returns gmt time
 */

mcp7941x_time_t *mcp7941x_get_gmt_time( void );

/**
 * @brief Calculates the current local time
 *
 * @return Returns local time
 */

mcp7941x_time_t *mcp7941x_get_local_time( void );

/**
 * @brief Sets the desired gmt time in the MCP7941x
 *
 * @param time - desired gmt time to be set
 * @return int
 * @retval -1 failed
 * @retval  0 successful
 */

int mcp7941x_set_gmt_time( mcp7941x_time_t time );

/**
 * @brief Sets desired local time
 *
 * @param time - desired local time to be set
 * @retval -1 failed
 * @retval  0 successful
 */

int mcp7941x_set_local_time( mcp7941x_time_t time );

/**
 * @brief Calculates the current gmt time in UNIX epoch time
 *
 * @return uint32_t
 * @retval gmt time converted to UNIX epoch time
 */

uint32_t mcp7941x_get_gmt_unix_time( void );

/**
 * @brief Calculates the current local time in UNIX epoch time
 * @return uint32_t
 * @retval Returns local time converted to UNIX epoch time
 */

uint32_t mcp7941x_get_local_unix_time( void );

/**
 * @brief Checks if the current year is a leap one
 *
 * @return bool
 * @retval true on a leap year
 * @retval false if not a leap year
 */

bool mcp7941x_is_leap_year( void );

/**
 * @brief Checks if a power failure has occured
 *
 * @return bool
 * @retval true if a power failure detected
 * @retval false if no power failure was detected
 *
 * @note Not supported by all models
 */
bool mcp7941x_is_power_failure( void );

/**
 * @brief Gets the time stamp of the last power failure
 *
 * @return mcp7941x_time_t - time stamp of the power failure
 *
 * @note Not supported by all models
 */
mcp7941x_time_t *mcp7941x_get_last_power_failure( void );

/****************************************
 *         Alarms                       
 ***************************************/
/**
 * @brief Sets the desired alarm
 *
 * @param alarm[IN] - Desired alarm to be set (alarm 0, alarm 1...)
 * @param trigger[IN] - Desired trigger for the alarm
 * @param time[IN] - Desired alarm time to be set
 *
 * @note Not supported by all models
 */

void mcp7941x_set_alarm( mcp7941x_alarm_t alarm, mcp7941x_alarm_trigger_t trigger,
                    mcp7941x_time_t time );

/**
 * @brief Disables the alarm
 *
 * @param alarm - Desired alarm to be disabled (alarm 0, alarm 1...)
 *
 * @note Not supported by all models
 */

void mcp7941x_disable_alarm( mcp7941x_alarm_t alarm );

/**
 * @brief Reads the alarm time
 *
 * @param alarm - Desired alarm to be read (alarm 0, alarm 1)
 * @return mcp7941x_time_t - alarm time
 *
 * @note Not supported by all models
 */

mcp7941x_time_t *mcp7941x_read_alarm( mcp7941x_alarm_t alarm );


/**********************************************************
 *         Memory functions
 **********************************************************/

/**
 * @brief Writes data to the SRAM registers of the MCP7941x
 *
 * @param addr[IN]  - Address of the desired register
 * @param data_in[IN]  - Data to be written
 *
 * Valid Addresses: 
 * MCP7941x 0x00 to 0x40
 *
 * @note Not supported by all models
 */

void mcp7941x_write_sram( uint8_t addr, uint8_t data_in );

/**
 * @brief Writes several bytes to SRAM registers of the MCP7941x
 *
 * @param addr[IN] - Address from where to start the writing
 * @param data_in[IN] - Data to be written to SRAM
 * @param data_size[IN] - Number of bytes to be sent
 *
 * @note Not supported by all models
 */

void mcp7941x_write_sram_bulk( uint8_t addr, void *data_in, size_t data_size );

/**
 * @brief Reads data from an SRAM register
 *
 * @param addr - Register address
 * @return uint8_t - read byte
 *
 * @note Not supported by all models
 */

uint8_t mcp7941x_read_sram( uint8_t addr );

/**
 * @brief Reads several bytes from SRAM registers
 *
 * @param addr[IN] - Address from where to start the reading
 * @param data_out[OUT] - Pointer to the variable which will hold the read bytes
 * @param data_size[IN] - Number of bytes to be read
 *
 * @note Not supported by all models
 */

void mcp7941x_read_sram_bulk( uint8_t addr, void *data_out, uint8_t data_size );


void mcp7941x_eeprom_write_protect_on( void );


void mcp7941x_eeprom_write_protect_off( void );


bool mcp7941x_eeprom_is_locked( void );

/**
 * @brief Writes data to EEPROM registers of MCP7941x
 *
 * @param addr[IN] - Address from where to start writing
 * @param data_in[IN] - Data to be written
 * @param data_size[IN]  - Number of bytes to be written
 *
 * @return bool
 * @retval true - writing was successful
 * @retval false - not successful
 *
 * @note Not supported by all models
 */

bool mcp7941x_write_eeprom( uint8_t addr, void *data_in, uint8_t data_size );

/**
 * @brief Reads data from EEPROM registers of the MCP7941x
 *
 * @param addr[IN] - Address from where to start reading
 * @param data_out[OUT] - Variable which will hold the read data
 * @param data_size[IN] - Number of bytes to be read
 *
 * @note Not supported by all models
 */

void mcp7941x_read_eeprom( uint8_t addr, void *data_out, uint8_t data_size );

/**
 * @brief Reads unique ID from EEPROM registers of the MCP7941x
 *
 * @returns uint8_t* - 8 bytes
 *
 * @note Not supported by all models
 */

uint8_t *mcp7941x_read_unique_id( void );

/**
 * @brief Writes unique ID to EEPROM registers
 *
 * @param uint8_t * id - array of 8 bytes
 *
 * @note Not supported by all models
 */

void mcp7941x_write_unique_id( uint8_t *id );

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* MCP7941X_H_ */
