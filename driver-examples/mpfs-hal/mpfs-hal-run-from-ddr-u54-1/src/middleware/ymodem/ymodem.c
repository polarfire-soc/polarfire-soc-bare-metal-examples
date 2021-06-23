/*
 * copyright (c) 2015 Microsemi Inc
 *
 * based on ymodem.c for rtdsr,   copyright (c) 2011 Pete B.
 * based on ymodem.c for bootldr, copyright (c) 2001 John G Dorsey
 * baded on ymodem.c for reimage, copyright (c) 2009 Rich M Legrand
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 */

#define RTG4_DEMO

#include <stdint.h>
#include <string.h>
#ifndef RTG4_DEMO
#include "sf2_bl_options.h"
#include "sf2_bl_defs.h"
#else
#include "drivers/mss/mss_mmuart/mss_uart.h"
#endif
#include "ymodem.h"


extern mss_uart_instance_t *g_uart;
extern volatile uint32_t g_10ms_count;

/***************************************************************************//**
 * Calculate CRC for block of data.
 */
uint16_t sf2bl_crc16(const uint8_t *buf, uint32_t count)
{
    uint16_t crc = 0;
    int i;

    while(count--)
    {
        crc = crc ^ *buf++ << 8;

        for (i=0; i<8; i++)
        {
            if (crc & 0x8000)
            {
                crc = crc << 1 ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }

    return crc;
}


/***************************************************************************//**
 * The rest of this is only needed for YMODEM builds.
 */
#if SF2BL_COMMS_OPTION == SF2BL_COMMS_YMODEM

#ifndef RTG4_DEMO

#if SF2BL_YMODEM_PORT == SF2BL_CORE16550
#include "core_16550.h"
#else
#include "mss_uart.h"
#endif


#if SF2BL_YMODEM_PORT == SF2BL_CORE16550
/* To keep things consistent define an instance and a pointer for the UART data
 * so that MSS and CORE code is as similar as possible.
 */
uart_16550_instance_t g_my_uart_instance;
uart_16550_instance_t *g_my_uart;

#else

#if (SF2BL_YMODEM_PORT == SF2BL_MSS_UART0)
mss_uart_instance_t *g_my_uart = &g_mss_uart0;
#else
mss_uart_instance_t *g_my_uart = &g_mss_uart1;
#endif

#endif

#ifndef RTG4_DEMO
/***************************************************************************//**
 *
 */
void sf2bl_ymodem_init(void)
{
#if SF2BL_YMODEM_PORT == SF2BL_CORE16550
    uint16_t baud_value;
    uint32_t temp;

    /* baud_value = (clock /(baud_rate * 16))
     * Use 32 bits to preserve precision before down sizing to 16 bit */
#if SF2BL_CORE16550_FIC == 0
    temp = (uint32_t)(MSS_SYS_FIC_0_CLK_FREQ) / ((uint32_t)(SF2BL_YMODEM_BAUD) * 16u);
#else
    temp = (uint32_t)(MSS_SYS_FIC_1_CLK_FREQ) / ((uint32_t)(SF2BL_YMODEM_BAUD) * 16u);
#endif
    baud_rate = temp;

    UART_16550_init(g_my_uart, SF2BL_CORE16550_HW_BASE, baud_value,
                    UART_16550_DATA_8_BITS | UART_16550_EVEN_PARITY | UART_16550_ONE_STOP_BIT);

    g_driver_init |=  SF2BL_DRIVER_CORE_UART;
#else
    MSS_UART_init(g_my_uart, SF2BL_MODEM_BAUD,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    if(g_my_uart == &g_mss_uart0)
    {
        g_driver_init |=  SF2BL_DRIVER_MSS_UART0;
    }
    else
    {
        g_driver_init |=  SF2BL_DRIVER_MSS_UART1;
    }
#endif
}
#endif

/***************************************************************************//**
 *
 */
void sf2bl_ymodem_deinit(void)
{
#if SF2BL_YMODEM_PORT == SF2BL_CORE16550
    UART_16550_deinit(g_my_uart, SF2BL_CORE16550_HW_BASE);
    g_driver_init &=  ~SF2BL_DRIVER_CORE_UART;
#else
    MSS_UART_deinit(g_my_uart);
    if(g_my_uart == &g_mss_uart0)
    {
        g_driver_init &= ~SF2BL_DRIVER_MSS_UART0;
    }
    else
    {
        g_driver_init &= ~SF2BL_DRIVER_MSS_UART1;
    }
#endif
}

#endif //#ifndef RTG4_DEMO

/*
 * These Ymodem calls are aimed at embedded software and tailored to
 * work against Microsoft's HyperTerminal. Some of the Ymodem protocol
*  operations have been voluntarily left out.
 *
 * To be able to use these functions, you must provide:
 * o int _getchar(int timeout): A serial getchar() call, with a
 *   timeout expressed in seconds. Negative means infinite timeout.
 *   should return the read character, as an int, or negative on
 *   error/timeout.
 * o void _putchar(int c): A serial putchar() call
 */


/***************************************************************************//**
 *
 */
static void _sleep(uint32_t seconds_delay)

{
    uint32_t start_time;

    start_time = g_10ms_count; /* record starting point */
    seconds_delay *= 1000;     /* Convert to ms */

    while((g_10ms_count - start_time) < seconds_delay)
        ;
//    while(start_time++ < seconds_delay);

}



/***************************************************************************//**
 *
 */
static int32_t _getchar(int32_t timeout)
{
    uint32_t start_time;
    uint8_t  rx_byte;
    int32_t  done;
    int32_t received;
    int32_t ret_value;

    ret_value = -1; /* Assume failure/timeout to simplify things */
    done = 0;

    if(timeout < 0) /* blocking mode, exit only on rx character or error */
    {
        while(!done)
        {
#ifndef RTG4_DEMO
            received = MSS_UART_get_rx(g_my_uart, &rx_byte, 1);
#else
            received = MSS_UART_get_rx(g_uart, &rx_byte, 1 );
#endif
            if(0 != received)
            {
                done = 1;
#ifndef RTG4_DEMO
                if(MSS_UART_NO_ERROR == MSS_UART_get_rx_status(g_my_uart))
#endif
                {
                    ret_value = (int32_t)rx_byte;
                }
            }
        }
    }
    else if(timeout > 0) /* time limited mode */
    {
        start_time = g_10ms_count; /* record starting point */
        timeout *= 1000;           /* Convert timeout to ms */
        while(!done)
        {
#ifndef RTG4_DEMO
            received = MSS_UART_get_rx(g_my_uart, &rx_byte, 1);
#else
            received = MSS_UART_get_rx(g_uart, &rx_byte, 1 );
#endif
            if(0 != received)
            {
                done = 1;
#ifndef RTG4_DEMO
                if(MSS_UART_NO_ERROR == MSS_UART_get_rx_status(g_my_uart))
#endif
                {
                    ret_value = (int32_t)rx_byte;
                }
            }

           if((g_10ms_count) >= start_time + timeout)
           {
                /* Timed out so exit with ret_value == -1 */
                done = 1;
           }
        }
    }
    else /* one shot mode */
    {
#ifndef RTG4_DEMO
            received = MSS_UART_get_rx(g_my_uart, &rx_byte, 1);
#else
            received = MSS_UART_get_rx(g_uart, &rx_byte, 1 );
#endif
#ifndef RTG4_DEMO
            if((0 != received) && (MSS_UART_NO_ERROR == MSS_UART_get_rx_status(g_my_uart)))
#else
                if(0 != received)
#endif
        {
            ret_value = (int32_t)rx_byte;
        }
    }

    return(ret_value);
}


/***************************************************************************//**
 *
 */
/*
 * Debug data and code to capture last hundred sends to see what sequence took
 * place.
 */
#if 0
static uint8_t __tx_buffer[100];
static int32_t __tx_index = 0;
#endif

void _putchar(int32_t data)
{
    uint8_t tx_byte;

#if 0
    __tx_buffer[__tx_index++] = (uint8_t)data;
    if(__tx_index >= 100)
        __tx_index = 0;
#endif

    tx_byte = (uint8_t)data;

#ifndef RTG4_DEMO
    MSS_UART_polled_tx(g_my_uart, &tx_byte, 1);
#else
    MSS_UART_polled_tx(g_uart, &tx_byte, 1 );
#endif
}


/***************************************************************************//**
 *
 */
void _putstring(uint8_t *string)
{

#ifndef RTG4_DEMO
    MSS_UART_polled_tx(g_my_uart, string, strlen((const char *)string));
#else
    MSS_UART_polled_tx(g_uart, string, strlen((const char *)string) );
#endif
}


/***************************************************************************//**
 *
 */
static uint32_t str_to_u32(uint8_t *str)
{
    const uint8_t *s = str;
    uint32_t acc;
    uint8_t c;

    /* strip leading spaces if any */
    do
    {
        c = *s++;
    } while (c == ' ');

    for(acc = 0; (c >= '0') && (c <= '9'); c = *s++)
    {
        c -= '0';
        acc *= 10;
        acc += c;
    }

    return acc;
}


/***************************************************************************//**
 * Returns 0 on success, 1 on corrupt packet, -1 on error (timeout):
 * *length will be set to the length of
 */
static int32_t receive_packet(uint8_t *data, int32_t *length)
{
    volatile int32_t index;
    int32_t rx_char;
    volatile int32_t return_val = 0; /* Assume everything is ok */
    uint32_t packet_size = 0U;

    *length = 0;

    //_putchar('N');
    rx_char = _getchar(PACKET_TIMEOUT);

    if(rx_char < 0)
    {
        return_val = -1;
    }
    else
    {
        switch(rx_char)
        {
        case SOH: /* 128 byte packet start */
            packet_size = PACKET_SIZE;
            break;

        case STX: /* 1024 byte packet start */
            packet_size = PACKET_1K_SIZE;
            break;

        case EOT: /* end of file marker */
            return_val = -2; /* Signifies end of file */
            break;

        case CAN: /* Possible cancellation of transfer */
            rx_char  = _getchar(PACKET_TIMEOUT);
            if(CAN == rx_char)
            {
                /* Got CAN CAN so really is cancel */
                *length = -1;
                return_val = -3; /* Signifies cancelled */
                break;
            }
            /* signal an error, CAN followed by not CAN is probably an
             * error on the line
             */
            *length = -1;
            return_val = -4; /* Signifies unexpected start of packet character */
            break;
        default:
            /* This case could be the result of corruption on the first octet
            * of the packet, but it's more likely that it's the user banging
            * on the terminal trying to abort a transfer. Technically, the
            * former case deserves a NAK, but for now we'll just treat this
            * as an abort case.
            */
            *length = -1;
            return_val = -4; /* Signifies unexpected start of packet character */
            break;
        }

        if(0 == return_val) /* Still ok */
        {
            *data = (uint8_t)rx_char; /* Store first character of packet */

            for(index = 1; (index < (int32_t)(packet_size + PACKET_OVERHEAD)) && (0 == return_val); ++index)
            {
                rx_char = _getchar(PACKET_TIMEOUT);
                if (rx_char < 0)
                {
                    return_val = -1;
                }
                else
                {
                    data[index] = (uint8_t)rx_char;
                }
            }

            /* Just a sanity check on the sequence number/complement value.
             * Caller should check for in-order arrival.
             *
             */
            int y = (0 == return_val);
            uint8_t x = data[PACKET_SEQNO_INDEX];
            uint8_t z = ((uint8_t)((data[PACKET_SEQNO_COMP_INDEX] ^ 0xffU) & 0xffU));
            if(y && (x != z))
            {
                return_val = 1;
            }

            if((0 == return_val) && (sf2bl_crc16(data + PACKET_HEADER, packet_size + PACKET_TRAILER) != 0))
            {
                return_val = 1;
            }

            if(0 == return_val) /* All ok ! */
            {
                *length = packet_size;
            }
        }
    }

    if(return_val < -1) /* returning 0 but *length == -1 or 0 so fix up return value */
    {
        return_val = 0;
    }

    return(return_val);
}


/***************************************************************************//**
 *
 */
/* Returns the length of the file received, or 0 on error: */
uint32_t ymodem_receive(uint8_t *buf, uint32_t length, uint8_t *file_name)
{
    static uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD]; /* Declare as static as 1K is a lot to put on our stack */
    uint8_t file_size[FILE_SIZE_LENGTH + 1];
    uint8_t *file_ptr;
    int32_t  packet_length;
    int32_t  index;
    int32_t  file_done;
    int32_t  session_done;

    int32_t  crc_nak;
    uint32_t packets_received;
    uint32_t errors;
    int32_t  first_try = 1;
    uint8_t *buf_ptr;
    uint32_t size = 0;
    uint32_t return_val = 0; /* Default to abnormal exit */
    uint32_t temp;
    int32_t  rx_status;

    file_name[0] = 0;
    session_done = 0;
    errors       = 0;

    while(0 == session_done)
    {
        crc_nak   = 1;

        if(!first_try)
        {
            _putchar(CRC);
        }

        first_try        = 0;
        packets_received = 0;
        file_done        = 0;
        buf_ptr          = buf;

        while(0 == file_done)
        {
            rx_status = receive_packet(packet_data, &packet_length);
            switch(rx_status)
            {
            case 0: /* Success */
                errors = 0;
                switch(packet_length)
                {
                case -1:  /* abort */
                    _putchar(ACK);

                    /* Terminate transfer immediately */
                    file_done    = 1;
                    session_done = 1;
                    break;

                case 0:   /* end of transmission */
                    _putchar(ACK);
                    /* Should add some sort of sanity check on the number of
                     * packets received and the advertised file length.
                     */
                    file_done = 1;
                    return_val = 1; /* Signal normal exit */
                    break;

                default:  /* normal packet */
                    if((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                    {
                        /*
                         * Hmmm, Tera Term 4.86 doesn't seem to like the ACK+C
                         * response and resends the header packet. Responding
                         * with just C seems to work. Only try this if we get a
                         * repeat of packet 0...
                         */
                        if((1 == packets_received) && (0 == (packet_data[PACKET_SEQNO_INDEX] & 0xff)))
                        {
                        _putchar(CRC); /* Repeated packet 0 error */
                        }
                        else
                        {
                        _putchar(NAK); /* Normal out of sequence packet error */
                        }
                    }
                    else
                    {
                        if(0 == packets_received)
                        {
                            /* The spec suggests that the whole data section should
                             * be zeroed, but I don't think all senders do this. If
                             * we have a NULL filename and the first few digits of
                             * the file length are zero, we'll call it empty.
                             */
                            temp = 0;
                            for(index = PACKET_HEADER; index < PACKET_HEADER + 4; index++)
                            {
                                temp += (uint32_t)packet_data[index];
                            }

                            if(0 != temp) /* looks like there is something there... */
                            {  /* filename packet has data */
                                file_ptr = packet_data + PACKET_HEADER;
                                /* Copy file name until nul or too much */
                                for(index = 0; *file_ptr && (index < FILE_NAME_LENGTH);)
                                {
                                    file_name[index++] = *file_ptr++;
                                }

                                file_name[index] = '\0';

                                while(*file_ptr != 0) /* Search for nul terminator if not there already */
                                {
                                    ++file_ptr;
                                }

                                ++file_ptr; /* Step over nul */

                                for(index = 0; *file_ptr && (*file_ptr != ' ') && (index < FILE_SIZE_LENGTH);)
                                {
                                    file_size[index++] = *file_ptr++;
                                }

                                file_size[index] = '\0';

                                size = str_to_u32(file_size);
                                if(size > length)
                                {
                                    _putchar(CAN);
                                    _putchar(CAN);
                                    _sleep(1);

                                    /* Terminate transfer immediately */
                                    file_done    = 1;
                                    session_done = 1;
                                }
                                else
                                {
                                    _putchar(ACK);
                                    _putchar(crc_nak ? CRC : NAK);
                                    crc_nak = 0;
                                }
                            }
                            else
                            {  /* filename packet is empty; end session */
                                _putchar(ACK);
                                file_done = 1;
                                session_done = 1;
                                break;
                            }
                        }
                        else
                        {
                            /* This shouldn't happen, but we check anyway in case the
                             * sender lied in its filename packet:
                             */
                            if((buf_ptr + packet_length) - buf > length)
                            {
                                _putchar(CAN);
                                _putchar(CAN);
                                _sleep(1);

                                /* Terminate transfer immediately */
                                file_done    = 1;
                                session_done = 1;
                            }
                            else
                            {
                                for (index=0; index < packet_length; index++)
                                {
                                    buf_ptr[index] = packet_data[PACKET_HEADER + index];
                                }

                                buf_ptr += packet_length;
                                _putchar(ACK);
                            }
                        }

                        ++packets_received;
                    }  /* sequence number ok */
                    break;
                }
                break;

            default: /* timeout or error */
                if(packets_received != 0)
                {
                    if(++errors >= MAX_ERRORS)
                    {
                        _putchar(CAN);
                        _putchar(CAN);
                        _sleep(1);

                        /* Terminate transfer immediately */
                        file_done    = 1;
                        session_done = 1;
                    }
                }

                if(0 == session_done)
                {
                    _putchar(CRC);
                }
                break;
            }
        }  /* receive packets */
    }  /* receive files */

    return(return_val == 1 ?  size : 0 );
}

#endif /* SF2BL_COMMS_OPTION == SF2BL_COMMS_YMODEM */
