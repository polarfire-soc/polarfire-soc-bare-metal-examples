/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 * 
 * SPDX-License-Identifier: MIT
 * 
 * @file mcp7941x6_click.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief The MCP7941x RTC bare metal software driver implementation.
*/

/********************************************************57G****
 * Include section  
 ************************************************************/

#include <drivers/off-chip/mcp7941x/mcp7941x.h>
#include <stdint.h>
#include <string.h>
#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_i2c/mss_i2c.h"


/* bcd to bin conversion. */

#define BCD2BIN(val) ( ( ( val ) & 15 ) + ( ( val ) >> 4 ) * 10 )
#define BIN2BCD(val) ( ( ( ( val ) / 10 ) << 4 ) + ( val ) % 10 )


#define MASTER_ADDR                 0x02U


/************************************************************
 * Type defs / Global variables 
 ************************************************************/
static uint8_t i2c_address;
static int8_t cur_tz_hours;
static uint8_t cur_tz_minutes;
static bool dst_enabled;
mss_i2c_instance_t* i2c_inst ; 

/************************************************************
 * static functions 
 ************************************************************/

static void get_dst( uint8_t year, mcp7941x_time_t *start, mcp7941x_time_t *end );
static int64_t time_date_to_epoch( mcp7941x_time_t *ts );
static void time_epoch_to_date( int64_t e, mcp7941x_time_t *ts );


void i2c0_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status);

/************************************************************
 * Generic functions definitions.
 ************************************************************/

void mcp7941x_init_generic( void* this_i2c,uint8_t slave_addr )
{
    /*  Enable PLIC interrupts and set priority.
        set the slave address ,
        set the I2C init function */ 
        
#ifdef MSS_I2C_H_
    this_i2c = (mss_i2c_instance_t *)this_i2c; 
    PLIC_init();
    if (this_i2c == &g_mss_i2c0_lo || this_i2c == &g_mss_i2c0_hi )
    {
        PLIC_SetPriority(I2C0_MAIN_PLIC, 2);
        PLIC_SetPriority(I2C0_ALERT_PLIC, 2);
        PLIC_SetPriority(I2C0_SUS_PLIC, 2);

        __enable_irq();

        PLIC_EnableIRQ(I2C0_MAIN_PLIC);
        PLIC_EnableIRQ(I2C0_ALERT_PLIC);
        PLIC_EnableIRQ(I2C0_SUS_PLIC);
    }
    else
    {
        PLIC_SetPriority(I2C1_MAIN_PLIC, 2);
        PLIC_SetPriority(I2C1_ALERT_PLIC, 2);
        PLIC_SetPriority(I2C1_SUS_PLIC, 2);

        __enable_irq();

        PLIC_EnableIRQ(I2C1_MAIN_PLIC);
        PLIC_EnableIRQ(I2C1_ALERT_PLIC);
        PLIC_EnableIRQ(I2C1_SUS_PLIC);
    }
    mcp7941x_set_i2c_inst(this_i2c); 
    mcp7941x_set_slave(slave_addr); 

    MSS_I2C_init(i2c_inst,MASTER_ADDR,MSS_I2C_PCLK_DIV_192);
    MSS_I2C_register_transfer_completion_handler(i2c_inst, i2c0_completion_handler);    
#endif

/* The core i2c section */ 


}

void mcp7941x_set_slave( uint8_t address_id )
{
    i2c_address = address_id;
}

void mcp7941x_set_i2c_inst(mss_i2c_instance_t* this_i2c){

    i2c_inst = this_i2c ; 
}

void mcp7941x_write_generic( uint8_t reg_addr, uint8_t *tx_data, uint16_t tx_size )
{

    #ifdef MSS_I2C_H_

        uint8_t tx_buff[10] ; 
        tx_buff[0] = reg_addr ; 
        memcpy(&tx_buff[1],tx_data,tx_size);

        MSS_I2C_write(i2c_inst,i2c_address,tx_buff,tx_size+1,MSS_I2C_RELEASE_BUS); 
        MSS_I2C_wait_complete(i2c_inst,MSS_I2C_NO_TIMEOUT);
        while(MSS_I2C_get_status(i2c_inst) != MSS_I2C_SUCCESS);
    #endif
}

void mcp7941x_read_generic(uint8_t reg_addr,uint8_t *rx_data,uint16_t rx_size)
{

    #ifdef MSS_I2C_H_
        uint8_t tx_buff = reg_addr ; 
        MSS_I2C_write_read(i2c_inst,i2c_address,&tx_buff,1,rx_data,rx_size,MSS_I2C_RELEASE_BUS);

        MSS_I2C_wait_complete(i2c_inst,MSS_I2C_NO_TIMEOUT);
        while(MSS_I2C_get_status(i2c_inst) != MSS_I2C_SUCCESS) ; 
    #endif
}


/************************************************************
 * general function definitions  
 ************************************************************/

static void time_epoch_to_date( int64_t e, mcp7941x_time_t *ts )
{
   
    int64_t julian_day; 

    int64_t J;
    int64_t K;
    int64_t L;
    int64_t N;
    int64_t NN;
    int64_t I;

    /*
     * julian day conversion
     */

    julian_day = e;
    julian_day /= TIME_SEC_IN_24_HOURS;
    julian_day += JULIAN_DAY_1970;

    if( e < 0 )
        julian_day--;

     /*get day in week number*/

    ts->weekday = julian_day % 7;       

    /*
     * conversion to gregorian calendar
     */

    L = julian_day + 68569;
    N = 4 * L;
    N /= 146097;
    NN = 146097 * N;
    NN += 3;
    NN /= 4;
    L -= NN;
    I = 4000 * ( L + 1 );
    I /= 1461001;
    NN = 1461 * I;
    NN /= 4;
    NN -= 31;
    L -= NN;
    J = 80 * L;
    J /= 2447;
    K = L - ( 2447 * J ) / 80;
    L = J / 11;
    J = J + 2 - 12 * L;
    I = ( 100 * ( N - 49 ) + I + L ) - 2000;

    /*
     * now we have the date
     */

    ts->year = I;
    ts->month = J;
    ts->monthday = K;

    /*
     * compute time
     */
    if( e < 0 )
    {
        /*
         * if epoch is before 1970,
         * extract correct hours, minutes, seconds
         */

        e *= -1;
        e %= TIME_SEC_IN_24_HOURS;
        e = TIME_SEC_IN_24_HOURS - e;
    }

    L = e % TIME_SEC_IN_24_HOURS;
    K = L / TIME_SEC_IN_HOUR;
    ts->hours = K;

    K = L;
    K -= ( int64_t )ts->hours * TIME_SEC_IN_HOUR;
    K /= TIME_SEC_IN_MIN;
    ts->minutes = K;

    K = L % TIME_SEC_IN_MIN;
    ts->seconds = K;
    ts->weekday++;
}


/*
 * USA starting in 2007, extended DST to begin
 * on the 2nd Sunday in March (Day of month = 14 - (1+Y*5/4)mod 7) and
 * end on the 1st Sunday in November (Day of month = 7 - (1 + 5*Y/4) mod 7).
 */

static void get_dst( uint8_t year, mcp7941x_time_t *start, mcp7941x_time_t *end )
{
    start->seconds = 0;
    start->minutes = 0;
    start->hours = 2;
    start->weekday = SUNDAY;
    start->monthday = 14 - ( ( 1 + ( ( year * 5 ) / 4 ) ) % 7 );
    start->month = MARCH;
    start->year = year;

    end->seconds  = 0;
    end->minutes  = 0;
    end->hours  = 2;
    end->weekday  = SUNDAY;
    end->monthday  = 7 - ( ( 1 + ( ( year * 5 ) / 4 ) ) % 7 );
    end->month  = NOVEMBER;
    end->year  = year;
}

static int64_t time_date_to_epoch( mcp7941x_time_t *ts )
{

    int64_t   ji;
    int64_t   julian_day;
    uint64_t   sec;
    int64_t   e;

    /*
     * julian day calculation
     */
    ji = ts->year + 2000 + 4800;
    ji *= 12;
    ji += ts->month - 3;

    julian_day = ( 2 * ( ji % 12 ) + 7 + 365 * ji ) / 12;
    julian_day += ts->monthday + ( ji / 48 ) - 32083;
    if( julian_day > 2299171L )
        julian_day += ji / 4800 - ji / 1200 + 38;
    ts->weekday = ( julian_day % 7 );            // and now the day of the week

    /*
     * number of seconds elapsed since the beginning of the julian day
     */
    sec = ts->hours;
    sec *= TIME_SEC_IN_HOUR;
    sec += ts->minutes * TIME_SEC_IN_MIN;
    sec += ts->seconds;

    /*
     * number of seconds since 1970
     */
    e = ( julian_day - JULIAN_DAY_1970 );
    e *= TIME_SEC_IN_24_HOURS;
    e += sec;
    return( e );
}

/************************************************************
 * MCP7941x6 init and config functions 
 ************************************************************/

uint8_t mcp7941x_init( void *this_i2c, int8_t time_zone_hours , uint8_t time_zone_minutes)
{

    uint8_t status;

    if(  time_zone_hours > 14 || time_zone_hours < -12 || time_zone_minutes > 59 )
        return -1;
    cur_tz_hours = time_zone_hours;
    cur_tz_minutes = time_zone_minutes; 
    mcp7941x_init_generic( this_i2c, MCP7941X_SLAVE );

    mcp7941x_read_generic( MCP7941x_SECONDS_ADDR, &status, 1 );
    /* start internal oscillator  */
    status |= MCP7941x_START_OSC_MASK;
    mcp7941x_write_generic( MCP7941x_SECONDS_ADDR,&status, 1 );

    if( ( !status ) & 0x80 )
    {
        status |= MCP7941x_START_OSC_MASK;
        mcp7941x_write_generic( MCP7941x_SECONDS_ADDR,&status, 1 );
    }
    /* enable battery as a default option. */

    mcp7941x_battery_enable(); 
    return 0;
}

void mcp7941x_enable_dst()
{
    dst_enabled = true;
}

void mcp7941x_disable_dst()
{
    dst_enabled = false;
}

void mcp7941x_enable_swo( mcp7941x_swo_t swo )
{
    uint8_t temp;
    mcp7941x_read_generic( 0x07, &temp, 1 );
    temp |= ( 1 << 6 );

    switch( swo )
    {
        case MCP7941x_1HZ:
            temp &= ~( 0b00000111 );
            break;
        case MCP7941x_4_096KHZ:
            temp &= ~( 0b00000111 );
            temp |=  ( 0b00000001 );
            break;
        case MCP7941x_8_192KHZ:
            temp &= ~( 0b00000111 );
            temp |= ( 1 << 1 );
            break;
        case MCP7941x_32_768KHZ:
            temp &= ~( 0b00000111 );
            temp |= ( 0b00000011 );
            break;
        default:
            break;
    }
    mcp7941x_write_generic( 0x07, &temp, 1 );
}

void mcp7941x_disable_swo()
{
    uint8_t temp;
    mcp7941x_read_generic( 0x07, &temp, 1 );
    temp &= ~( 1 << 6 );
    mcp7941x_write_generic( 0x07, &temp, 1 );
}

void mcp7941x_battery_enable()
{
    uint8_t temp = 0;

    mcp7941x_read_generic( 0x03, &temp, 1 );
    temp |= ( 1 << 3 );
    mcp7941x_write_generic( 0x03, &temp, 1 );
}

/************************************************************
 * Date and time functions 
 ************************************************************/

mcp7941x_time_t *mcp7941x_get_gmt_time()
{
    static mcp7941x_time_t gmt_time;
    uint8_t buffer[MCP7941x_TIMEDATE_BYTES];

    memset( buffer, 0, sizeof( buffer ) );

    mcp7941x_read_generic( MCP7941x_SECONDS_ADDR, buffer, MCP7941x_TIMEDATE_BYTES );
    gmt_time.seconds = BCD2BIN( MCP7941x_SECONDS_MASK( buffer[MCP7941x_SECONDS_BYTE] ) );
    gmt_time.minutes = BCD2BIN( MCP7941x_MINUTES_MASK( buffer[MCP7941x_MINUTES_BYTE] ) );
    gmt_time.hours = BCD2BIN( MCP7941x_HOURS_MASK( buffer[MCP7941x_HOUR_BYTE] ) );
    gmt_time.weekday = BCD2BIN( MCP7941x_DAY_MASK( buffer[MCP7941x_DAY_BYTE] ) );
    gmt_time.monthday = BCD2BIN( MCP7941x_DATE_MASK( buffer[MCP7941x_DATE_BYTE] ) );
    gmt_time.month = BCD2BIN( MCP7941x_MONTH_MASK( buffer[MCP7941x_MONTH_BYTE] ) );
    gmt_time.year = BCD2BIN( MCP7941x_YEAR_MASK( buffer[MCP7941x_YEAR_BYTE] ) );

    return &gmt_time;
}

mcp7941x_time_t *mcp7941x_get_local_time()
{
    static mcp7941x_time_t local_time;

    uint32_t temp_time_unix = mcp7941x_get_gmt_unix_time();

    /* for now without milli seconds */

    temp_time_unix += ( (cur_tz_hours * 60 * 60) + (cur_tz_minutes*60));

    /*converting to date*/

    time_epoch_to_date ( temp_time_unix, &local_time );

    if( dst_enabled )
    {
        mcp7941x_time_t start, end;
        get_dst ( local_time.year, &start, &end );


        /* here we check if our current date is in the period where dst adds one hour
         * if it is a month between the start and end, not including the end,
         * not including the start, we surely need to add one hour */

        if( ( local_time.month > start.month ) && ( local_time.month < end.month ) )
        {
            temp_time_unix = time_date_to_epoch( &local_time );
            temp_time_unix += 60 * 60 * 1000;
        }

        /* or if it is the end month, but before the last day of DST,
         * we still need to add one hour
         * or if it is the last day,but before 02:00 hrs */

        else if( ( ( local_time.month = end.month ) &&
                   ( local_time.monthday < end.monthday ) ) ||
                 ( ( local_time.month = end.month ) && ( local_time.hours < 2 ) ) )
        {
            temp_time_unix = time_date_to_epoch( &local_time );
            temp_time_unix += 60 * 60 * 1000;
        }

        /* or if it's the starting month, passed the starting monthday.
         * if it is the starting monthday, we check if it is passed 02:00 */

        else if
        (
            ( ( local_time.month == start.month ) &&
              ( local_time.monthday > start.monthday ) )
            ||
            ( ( local_time.month == start.month ) &&
              ( local_time.monthday == start.monthday ) && ( local_time.hours >= 2 ) )
        )
        {
            temp_time_unix = time_date_to_epoch( &local_time );
            temp_time_unix += 60 * 60 * 1000;
        }

        /* in other cases, we subtract one hour */

        else temp_time_unix -= 60 * 60 * 1000;
    }

    /* converting back to date, returning the address */

    time_epoch_to_date( temp_time_unix, &local_time );
    return &local_time;

}


int mcp7941x_set_gmt_time( mcp7941x_time_t  time )
{
    uint8_t buffer[MCP7941x_TIMEDATE_BYTES];
    uint8_t temp;
    
    if( time.seconds > 59 ||
            time.minutes > 59 ||
            time.hours > 24 ||
            time.weekday > 7 ||
            time.monthday > 31 ||
            time.month > 12 ||
            time.year > 99 )
        return -1;



    mcp7941x_read_generic( MCP7941x_SECONDS_ADDR, buffer, MCP7941x_TIMEDATE_BYTES );

    buffer[MCP7941x_SECONDS_BYTE] = MCP7941x_SECONDS_CLEAR( buffer[MCP7941x_SECONDS_BYTE] );
    buffer[MCP7941x_SECONDS_BYTE] |= BIN2BCD( time.seconds );

    buffer[MCP7941x_MINUTES_BYTE] = MCP7941x_MINUTES_CLEAR( buffer[MCP7941x_MINUTES_BYTE] );
    buffer[MCP7941x_MINUTES_BYTE] |= BIN2BCD( time.minutes );

    buffer[MCP7941x_HOUR_BYTE] = MCP7941x_HOURS_CLEAR( buffer[MCP7941x_HOUR_BYTE] );
    buffer[MCP7941x_HOUR_BYTE] |= BIN2BCD( time.hours );

    buffer[MCP7941x_DAY_BYTE] = MCP7941x_DAY_CLEAR( buffer[MCP7941x_DAY_BYTE] );
    buffer[MCP7941x_DAY_BYTE] |= BIN2BCD( time.weekday );

    buffer[MCP7941x_DATE_BYTE] = MCP7941x_DATE_CLEAR( buffer[MCP7941x_DATE_BYTE] );
    buffer[MCP7941x_DATE_BYTE] |= BIN2BCD( time.monthday );

    buffer[MCP7941x_MONTH_BYTE] = MCP7941x_MONTH_CLEAR( buffer[MCP7941x_MONTH_BYTE] );
    buffer[MCP7941x_MONTH_BYTE] |= BIN2BCD( time.month );

    buffer[MCP7941x_YEAR_BYTE] = MCP7941x_YEAR_CLEAR( buffer[MCP7941x_YEAR_BYTE] );
    buffer[MCP7941x_YEAR_BYTE] |= BIN2BCD( time.year );

    mcp7941x_write_generic( MCP7941x_SECONDS_ADDR, buffer, MCP7941x_TIMEDATE_BYTES );
    mcp7941x_read_generic( MCP7941x_SECONDS_ADDR, buffer, MCP7941x_TIMEDATE_BYTES );


    return 0;
}


uint32_t mcp7941x_get_gmt_unix_time()
{
    mcp7941x_time_t *temp_time;
    uint32_t temp;
    temp_time = mcp7941x_get_gmt_time();

    temp =  time_date_to_epoch( temp_time );

    return temp;
}


uint32_t mcp7941x_get_local_unix_time()
{
    uint32_t temp;
    mcp7941x_time_t *temp_time = mcp7941x_get_local_time();

    temp =  time_date_to_epoch( temp_time );
    return temp;
}

bool mcp7941x_is_leap_year()
{

    uint8_t temp;
    mcp7941x_read_generic( 0x05, &temp, 1 );
    temp &= 0b00100000;
    if ( temp  == 0 )
        return false;
    else return true;

    return false;
}


bool mcp7941x_is_power_failure()
{
    uint8_t temp;

    mcp7941x_read_generic( 0x03, &temp, 1 );
    temp &= ( 1 << 4 );
    if ( temp == 0 ) return false;
    else return true;

    return false;
}


mcp7941x_time_t *mcp7941x_get_last_power_failure()
{
    static mcp7941x_time_t stamp = {0};

    uint8_t buffer[4];

    mcp7941x_read_generic( 0x18, buffer, 4 );
    stamp.minutes  = BCD2BIN( MCP7941x_MINUTES_MASK( buffer[0] ) );
    stamp.hours    = BCD2BIN( MCP7941x_HOURS_MASK( buffer[1] ) );
    stamp.monthday = BCD2BIN( MCP7941x_DATE_MASK( buffer[2] ) );
    stamp.weekday  = BCD2BIN ( ( buffer[3] & 0b11100000 ) );
    stamp.month    = BCD2BIN( ( buffer[3] & 0b00011111 ) );

    return &stamp;

    return 0;
}

/****************************************
 *         Alarms                        
 ***************************************/

void mcp7941x_set_alarm( mcp7941x_alarm_t alarm, mcp7941x_alarm_trigger_t trigger,
                    mcp7941x_time_t time )
{
    uint8_t buffer[6];
    uint8_t temp;

    switch ( alarm )
    {
        case MCP7941x_ALARM_0:

            mcp7941x_read_generic( 0x0A, buffer, 6 );
            /* Set time values for the alarm */
            buffer[MCP7941x_SECONDS_BYTE] = MCP7941x_SECONDS_CLEAR( buffer[MCP7941x_SECONDS_BYTE] );
            buffer[MCP7941x_SECONDS_BYTE] |= BIN2BCD( time.seconds );

            buffer[MCP7941x_MINUTES_BYTE] = MCP7941x_MINUTES_CLEAR( buffer[MCP7941x_MINUTES_BYTE] );
            buffer[MCP7941x_MINUTES_BYTE] |= BIN2BCD( time.minutes );

            buffer[MCP7941x_HOUR_BYTE] = MCP7941x_HOURS_CLEAR( buffer[MCP7941x_HOUR_BYTE] );
            buffer[MCP7941x_HOUR_BYTE] |= BIN2BCD( time.hours );

            buffer[MCP7941x_DAY_BYTE] = MCP7941x_DAY_CLEAR( buffer[MCP7941x_DAY_BYTE] );
            buffer[MCP7941x_DAY_BYTE] |= BIN2BCD( time.weekday );

            buffer[MCP7941x_DATE_BYTE] = MCP7941x_DATE_CLEAR( buffer[MCP7941x_DATE_BYTE] );
            buffer[MCP7941x_DATE_BYTE] |= BIN2BCD( time.monthday );

            buffer[MCP7941x_MONTH_BYTE] = MCP7941x_MONTH_CLEAR( buffer[MCP7941x_MONTH_BYTE] );
            buffer[MCP7941x_MONTH_BYTE] |= BIN2BCD( time.month );

            mcp7941x_write_generic( 0x0A, buffer, 6 );

            /* set the trigger */

            mcp7941x_read_generic( 0x0D, &temp, 1 );
            switch ( trigger )
            {
                case MCP7941x_ALARM_SECONDS:
                    temp &= ~( 0b01111000 );
                    break;
                case MCP7941x_ALARM_MINUTES:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 4 );
                    break;
                case MCP7941x_ALARM_HOURS:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 5 );
                    break;
                case MCP7941x_ALARM_DAY:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 4 );
                    temp |= ( 1 << 5 );
                    break;
                case MCP7941x_ALARM_DATE:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 6 );
                    break;
                case MCP7941x_ALARM_SEC_MIN_HOUR_DAY_DATE_MONTH:
                    temp &= ~( 0b01111000 );
                    temp |=  ( 0b01110000 );
                    break;

                default:
                    break;
            }
            /*set the polarity to one*/
            temp |= ( 1 << 7 ); 
            mcp7941x_write_generic( 0x0D, &temp, 1 );
            /* enable battery */
            mcp7941x_read_generic( 0x03, &temp, 1 ); 
            temp |= ( 1 << 3 );
            mcp7941x_write_generic( 0x03, &temp, 1 );

            mcp7941x_read_generic( 0x07, &temp, 1 );
            /* disable sqwo ( square wave output) */ 

            temp &= ~( 1 << 6 ); 
            /* activate alarm 0 */ 
            temp |= ( 1 << 4 );
            mcp7941x_write_generic( 0x07, &temp, 1 );
            break;

        case MCP7941x_ALARM_1:

            mcp7941x_read_generic( 0x11, buffer, 6 );
            /* Set time values for the alarm */
            buffer[MCP7941x_SECONDS_BYTE] = MCP7941x_SECONDS_CLEAR( buffer[MCP7941x_SECONDS_BYTE] );
            buffer[MCP7941x_SECONDS_BYTE] |= BIN2BCD( time.seconds );

            buffer[MCP7941x_MINUTES_BYTE] = MCP7941x_MINUTES_CLEAR( buffer[MCP7941x_MINUTES_BYTE] );
            buffer[MCP7941x_MINUTES_BYTE] |= BIN2BCD( time.minutes );

            buffer[MCP7941x_HOUR_BYTE] = MCP7941x_HOURS_CLEAR( buffer[MCP7941x_HOUR_BYTE] );
            buffer[MCP7941x_HOUR_BYTE] |= BIN2BCD( time.hours );

            buffer[MCP7941x_DAY_BYTE] = MCP7941x_DAY_CLEAR( buffer[MCP7941x_DAY_BYTE] );
            buffer[MCP7941x_DAY_BYTE] |= BIN2BCD( time.weekday );

            buffer[MCP7941x_DATE_BYTE] = MCP7941x_DATE_CLEAR( buffer[MCP7941x_DATE_BYTE] );
            buffer[MCP7941x_DATE_BYTE] |= BIN2BCD( time.monthday );

            buffer[MCP7941x_MONTH_BYTE] = MCP7941x_MONTH_CLEAR( buffer[MCP7941x_MONTH_BYTE] );
            buffer[MCP7941x_MONTH_BYTE] |= BIN2BCD( time.month );

            mcp7941x_write_generic( 0x11, buffer, 6 );

            mcp7941x_read_generic( 0x14, &temp, 1 );
            switch ( trigger )
            {
                case MCP7941x_ALARM_SECONDS:
                    temp &= ~( 0b01111000 );
                    break;
                case MCP7941x_ALARM_MINUTES:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 4 );
                    break;
                case MCP7941x_ALARM_HOURS:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 5 );
                    break;
                case MCP7941x_ALARM_DAY:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 4 );
                    temp |= ( 1 << 5 );
                    break;
                case MCP7941x_ALARM_DATE:
                    temp &= ~( 0b01111000 );
                    temp |= ( 1 << 6 );
                    break;
                case MCP7941x_ALARM_SEC_MIN_HOUR_DAY_DATE_MONTH:
                    temp &= ~( 0b01111000 );
                    temp |=  ( 0b01110000 );
                    break ;
                default:
                    break;
            }

            /* set the priority to one */ 
            temp |= ( 1 << 7 ); 
            mcp7941x_write_generic( 0x14, &temp, 1 );

            /* enable battery */ 
            mcp7941x_read_generic( 0x03, &temp, 1 ); 
            temp |= ( 1 << 3 );
            mcp7941x_write_generic( 0x03, &temp, 1 );

            mcp7941x_read_generic( 0x07, &temp, 1 );

            /* disable sqwo(square wave output) */ 

            temp &= ~( 1 << 6 );

            /* enable alarm 1 */  

            temp |= ( 1 << 5 );
            mcp7941x_write_generic( 0x07, &temp, 1 );
            break;
        }
    }




void mcp7941x_disable_alarm( mcp7941x_alarm_t alarm )
{
    uint8_t temp;
    mcp7941x_read_generic( 0x07, &temp, 1 );
    switch( alarm )
    {
        case MCP7941x_ALARM_0:
            temp &= ~( 1 << 4 );
            mcp7941x_write_generic( 0x07, &temp, 1 );
            break;
        case MCP7941x_ALARM_1:
            temp &= ~( 1 << 5 );
            mcp7941x_write_generic( 0x07, &temp, 1 );
            break;
    }
}


mcp7941x_time_t *mcp7941x_read_alarm( mcp7941x_alarm_t alarm )
{
    uint8_t buffer[6];
    static mcp7941x_time_t temp_time = {0};

    switch( alarm )
    {
        case MCP7941x_ALARM_0:
            mcp7941x_read_generic( 0x0A, buffer, 6 );
            temp_time.seconds = BCD2BIN( MCP7941x_SECONDS_MASK( buffer[MCP7941x_SECONDS_BYTE] ) );
            temp_time.minutes = BCD2BIN( MCP7941x_MINUTES_MASK( buffer[MCP7941x_MINUTES_BYTE] ) );
            temp_time.hours = BCD2BIN( MCP7941x_HOURS_MASK( buffer[MCP7941x_HOUR_BYTE] ) );
            temp_time.weekday = BCD2BIN( MCP7941x_DAY_MASK( buffer[MCP7941x_DAY_BYTE] ) );
            temp_time.monthday = BCD2BIN( MCP7941x_DATE_MASK( buffer[MCP7941x_DATE_BYTE] ) );
            temp_time.month = BCD2BIN( MCP7941x_MONTH_MASK( buffer[MCP7941x_MONTH_BYTE] ) );
            break;

        case MCP7941x_ALARM_1:
            mcp7941x_read_generic( 0x11, buffer, 6 );
            temp_time.seconds = BCD2BIN( MCP7941x_SECONDS_MASK( buffer[MCP7941x_SECONDS_BYTE] ) );
            temp_time.minutes = BCD2BIN( MCP7941x_MINUTES_MASK( buffer[MCP7941x_MINUTES_BYTE] ) );
            temp_time.hours = BCD2BIN( MCP7941x_HOURS_MASK( buffer[MCP7941x_HOUR_BYTE] ) );
            temp_time.weekday = BCD2BIN( MCP7941x_DAY_MASK( buffer[MCP7941x_DAY_BYTE] ) );
            temp_time.monthday = BCD2BIN( MCP7941x_DATE_MASK( buffer[MCP7941x_DATE_BYTE] ) );
            temp_time.month = BCD2BIN( MCP7941x_MONTH_MASK( buffer[MCP7941x_MONTH_BYTE] ) );
            break;

    }

    return &temp_time;

}

/************************************************************
 * Memory functions definitions.
 ************************************************************/

void mcp7941x_write_sram( uint8_t addr, uint8_t data_in )
{
            if( addr + RAM_START < RAM_END )
            {
                mcp7941x_set_slave( MCP7941X_SRAM_SLAVE );
                mcp7941x_write_generic( RAM_START + addr, &data_in, 1 );
                mcp7941x_set_slave( MCP7941X_SLAVE );
            }
            else
            {
                ASSERT(0);
            }
    
}

void mcp7941x_write_sram_bulk( uint8_t addr, void *data_in, size_t data_size )
{
            if( addr + RAM_START + data_size < RAM_END )
            {
                mcp7941x_set_slave( MCP7941X_SRAM_SLAVE );
                mcp7941x_write_generic( RAM_START + addr, data_in, data_size );
                mcp7941x_set_slave( MCP7941X_SLAVE );
            
            }
            else
            {
                ASSERT(0);
            }
    
}


uint8_t mcp7941x_read_sram( uint8_t addr )
{
    uint8_t temp = 0;

            if( addr + RAM_START < RAM_END )
            {
                mcp7941x_set_slave( MCP7941X_SRAM_SLAVE );
                mcp7941x_read_generic ( RAM_START + addr, &temp, 1 );
                mcp7941x_set_slave( MCP7941X_SLAVE );
            }
            else
            {

                ASSERT(0);
            }
    

    return temp;
}


void mcp7941x_read_sram_bulk( uint8_t addr, void *data_out, uint8_t data_size )
{
            if( addr + RAM_START + data_size < RAM_END )
            {
                mcp7941x_set_slave( MCP7941X_SRAM_SLAVE );
                mcp7941x_read_generic( RAM_START + addr, data_out, data_size );
                mcp7941x_set_slave( MCP7941X_SLAVE );
            }
            else
            {

                ASSERT(0);
            }
    
}

void i2c0_completion_handler(mss_i2c_instance_t * instance, mss_i2c_status_t status)
{
    if(status == MSS_I2C_SUCCESS)
    {
    }
    else
    {
        ASSERT(0);
    }
}
