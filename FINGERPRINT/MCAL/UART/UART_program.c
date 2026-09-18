#define F_CPU 8000000UL

#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"

#include "UART_interface.h"
#include "UART_private.h"

/*
 * R307 DEFAULT:
 * Baud     = 57600
 * Data     = 8 bit
 * Parity   = None
 * Stop     = 1
 * U2X      = 1
 *
 * F_CPU = 8 MHz
 * UBRR  = 16
 */

#define UART_RX_TIMEOUT 200000UL


void UART_VoidInit(void)
{
    u8 Local_u8UCSRCValue = 0;

    /*
     * Enable Double Speed
     */
    SET_BIT(UART_UCSRA_REG, 1);

    /*
     * Disable Multi Processor Mode
     */
    CLR_BIT(UART_UCSRA_REG, 0);

    /*
     * Enable RX
     */
    SET_BIT(UART_UCSRB_REG, 4);

    /*
     * Enable TX
     */
    SET_BIT(UART_UCSRB_REG, 3);

    /*
     * 8-bit data
     *
     * UCSZ2 = 0
     * UCSZ1 = 1
     * UCSZ0 = 1
     */
    CLR_BIT(UART_UCSRB_REG, 2);

    SET_BIT(Local_u8UCSRCValue, 2);
    SET_BIT(Local_u8UCSRCValue, 1);

    /*
     * Asynchronous
     */
    CLR_BIT(Local_u8UCSRCValue, 6);

    /*
     * No parity
     */
    CLR_BIT(Local_u8UCSRCValue, 5);
    CLR_BIT(Local_u8UCSRCValue, 4);

    /*
     * 1 stop bit
     */
    CLR_BIT(Local_u8UCSRCValue, 3);

    /*
     * IMPORTANT:
     * Select UCSRC register
     */
    SET_BIT(Local_u8UCSRCValue, 7);

    UART_UCSRC_REG = Local_u8UCSRCValue;

    /*
     * 57600 baud
     * F_CPU = 8 MHz
     * U2X = 1
     */
    UART_UBRRH_REG = 0;
    UART_UBRRL_REG = 16;
}


u8 UART_VoidRecieveByte(u8 *Copy_pu8RecByte)
{
    u32 Local_u32Timeout = 0;

    if(Copy_pu8RecByte == NULL)
    {
        return STD_TYPES_NOK;
    }

    /*
     * Wait for RXC
     * with timeout
     */
    while(GET_BIT(UART_UCSRA_REG, 7) == 0)
    {
        Local_u32Timeout++;

        if(Local_u32Timeout >= UART_RX_TIMEOUT)
        {
            return STD_TYPES_NOK;
        }
    }

    /*
     * Read received byte
     */
    *Copy_pu8RecByte = UART_UDR_REG;

    return STD_TYPES_OK;
}


void UART_VoidSendByte(u8 Copy_u8DataByte)
{
    /*
     * Wait until transmit buffer is empty
     */
    while(GET_BIT(UART_UCSRA_REG, 5) == 0)
    {
    }

    UART_UDR_REG = Copy_u8DataByte;
}
