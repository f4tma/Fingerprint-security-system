/*
 * =========================================================
 * LCD_program.c
 *
 * ATmega32
 *
 * I2C LCD Driver
 * PCF8574 I2C Backpack
 *
 * PCF8574:
 *
 * P0 -> RS
 * P1 -> RW
 * P2 -> EN
 * P3 -> Backlight
 * P4 -> D4
 * P5 -> D5
 * P6 -> D6
 * P7 -> D7
 *
 * =========================================================
 */


#include <util/delay.h>

#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"

#include "../../MCAL/TWI/TWI_interface.h"

#include "LCD_config.h"
#include "LCD_interface.h"


/*=========================================================
 * PRIVATE FUNCTION
 *
 * Send one byte to PCF8574
 *=========================================================*/

static void LCD_SendPCF8574Byte(u8 Copy_u8Data)
{
    TWI_ErrorStatus_t Local_Error;


    /*
     * START
     */
    Local_Error =
        TWI_ErrorStatusSendStartConditionWithAck();


    if(Local_Error != NoError)
    {
        TWI_voidSendStopCondition();

        return;
    }


    /*
     * Send slave address + WRITE
     *
     * PCF8574 address is 7-bit.
     * Shift left by one to add WRITE bit = 0.
     */
    Local_Error =
        TWI_ErrorStatusSendSlaveAddressWithWriteAck(
            LCD_I2C_ADDRESS
        );

    if(Local_Error != NoError)
    {
        TWI_voidSendStopCondition();

        return;
    }


    /*
     * Send data
     */
    Local_Error =
        TWI_ErrorStatusMasterWriteDataByteWithAck(
            Copy_u8Data
        );


    /*
     * STOP
     */
    TWI_voidSendStopCondition();
}


/*=========================================================
 * PRIVATE FUNCTION
 *
 * Generate LCD Enable pulse
 *=========================================================*/

static void LCD_EnablePulse(u8 Copy_u8Data)
{
    /*
     * EN = HIGH
     */
    SET_BIT(Copy_u8Data, LCD_EN_BIT);

    LCD_SendPCF8574Byte(Copy_u8Data);

    _delay_us(1);


    /*
     * EN = LOW
     */
    CLR_BIT(Copy_u8Data, LCD_EN_BIT);

    LCD_SendPCF8574Byte(Copy_u8Data);

    _delay_us(50);
}


/*=========================================================
 * PRIVATE FUNCTION
 *
 * Send 4 bits to LCD
 *
 * Copy_u8Value:
 *
 * bit 0 -> LCD D4
 * bit 1 -> LCD D5
 * bit 2 -> LCD D6
 * bit 3 -> LCD D7
 *
 * Copy_u8RS:
 *
 * 0 -> command
 * 1 -> data
 *=========================================================*/

static void LCD_Send4Bits(
        u8 Copy_u8Value,
        u8 Copy_u8RS)
{
    u8 Local_u8Data = 0;


    /*
     * -----------------------------------------------------
     * RS
     * -----------------------------------------------------
     */

    if(Copy_u8RS == 1)
    {
        SET_BIT(Local_u8Data, LCD_RS_BIT);
    }
    else
    {
        CLR_BIT(Local_u8Data, LCD_RS_BIT);
    }


    /*
     * -----------------------------------------------------
     * RW = 0
     *
     * We only write to LCD.
     * -----------------------------------------------------
     */

    CLR_BIT(Local_u8Data, LCD_RW_BIT);


    /*
     * -----------------------------------------------------
     * BACKLIGHT = ON
     * -----------------------------------------------------
     */

    SET_BIT(Local_u8Data, LCD_BACKLIGHT_BIT);


    /*
     * -----------------------------------------------------
     * D4
     * -----------------------------------------------------
     */

    if(GET_BIT(Copy_u8Value, 0))
    {
        SET_BIT(Local_u8Data, LCD_D4_BIT);
    }
    else
    {
        CLR_BIT(Local_u8Data, LCD_D4_BIT);
    }


    /*
     * -----------------------------------------------------
     * D5
     * -----------------------------------------------------
     */

    if(GET_BIT(Copy_u8Value, 1))
    {
        SET_BIT(Local_u8Data, LCD_D5_BIT);
    }
    else
    {
        CLR_BIT(Local_u8Data, LCD_D5_BIT);
    }


    /*
     * -----------------------------------------------------
     * D6
     * -----------------------------------------------------
     */

    if(GET_BIT(Copy_u8Value, 2))
    {
        SET_BIT(Local_u8Data, LCD_D6_BIT);
    }
    else
    {
        CLR_BIT(Local_u8Data, LCD_D6_BIT);
    }


    /*
     * -----------------------------------------------------
     * D7
     * -----------------------------------------------------
     */

    if(GET_BIT(Copy_u8Value, 3))
    {
        SET_BIT(Local_u8Data, LCD_D7_BIT);
    }
    else
    {
        CLR_BIT(Local_u8Data, LCD_D7_BIT);
    }


    /*
     * Send initial data
     */
    LCD_SendPCF8574Byte(Local_u8Data);


    /*
     * Enable pulse
     */
    LCD_EnablePulse(Local_u8Data);
}


/*=========================================================
 * LCD WRITE COMMAND
 *=========================================================*/

void LCD_writeCommand(u8 Copy_u8Command)
{
    /*
     * HIGH nibble
     */
    LCD_Send4Bits(
        (Copy_u8Command >> 4) & 0x0F,
        0
    );


    /*
     * LOW nibble
     */
    LCD_Send4Bits(
        Copy_u8Command & 0x0F,
        0
    );


    /*
     * Clear display and return home
     * need longer delay.
     */
    if(
        (Copy_u8Command == 0x01) ||
        (Copy_u8Command == 0x02)
      )
    {
        _delay_ms(2);
    }
    else
    {
        _delay_us(50);
    }
}


/*=========================================================
 * LCD WRITE CHARACTER
 *=========================================================*/

void LCD_writeChar(u8 Copy_u8Data)
{
    /*
     * HIGH nibble
     */
    LCD_Send4Bits(
        (Copy_u8Data >> 4) & 0x0F,
        1
    );


    /*
     * LOW nibble
     */
    LCD_Send4Bits(
        Copy_u8Data & 0x0F,
        1
    );


    _delay_us(50);
}


/*=========================================================
 * LCD INITIALIZATION
 *=========================================================*/

void LCD_init(void)
{
    /*
     * Wait for LCD power-up.
     */
    _delay_ms(50);


    /*
     * Backlight ON
     */
    LCD_SendPCF8574Byte(
        (1 << LCD_BACKLIGHT_BIT)
    );

    _delay_ms(5);


    /*
     * =====================================================
     * INITIAL 4-BIT SEQUENCE
     * =====================================================
     *
     * LCD starts in 8-bit mode after power-up.
     *
     * Send 0x03 three times.
     * Then send 0x02 to enter 4-bit mode.
     */


    /* First 0x03 */
    LCD_Send4Bits(0x03, 0);

    _delay_ms(5);


    /* Second 0x03 */
    LCD_Send4Bits(0x03, 0);

    _delay_us(150);


    /* Third 0x03 */
    LCD_Send4Bits(0x03, 0);

    _delay_us(150);


    /* Switch to 4-bit mode */
    LCD_Send4Bits(0x02, 0);

    _delay_us(150);


    /*
     * =====================================================
     * FUNCTION SET
     *
     * 4-bit
     * 2 lines
     * 5x8 font
     * =====================================================
     */

    LCD_writeCommand(0x28);


    /*
     * =====================================================
     * DISPLAY CONTROL
     *
     * Display ON
     * Cursor OFF
     * Blink OFF
     * =====================================================
     */

    LCD_writeCommand(0x0C);


    /*
     * =====================================================
     * CLEAR DISPLAY
     * =====================================================
     */

    LCD_writeCommand(0x01);

    _delay_ms(2);


    /*
     * =====================================================
     * ENTRY MODE
     *
     * Cursor moves right
     * Display doesn't shift
     * =====================================================
     */

    LCD_writeCommand(0x06);
}


/*=========================================================
 * LCD WRITE STRING
 *=========================================================*/

void LCD_writeString(u8* Copy_pu8String)
{
    u8 Local_u8Index = 0;


    if(Copy_pu8String != 0)
    {
        while(
            Copy_pu8String[Local_u8Index] != '\0'
        )
        {
            LCD_writeChar(
                Copy_pu8String[Local_u8Index]
            );

            Local_u8Index++;
        }
    }
}


/*=========================================================
 * LCD WRITE NUMBER
 *=========================================================*/

void LCD_writeNumber(u32 Copy_u32Number)
{
    u8 Local_u8Digits[10];

    u8 Local_u8Index = 0;


    /*
     * Number = 0
     */
    if(Copy_u32Number == 0)
    {
        LCD_writeChar('0');

        return;
    }


    /*
     * Extract digits
     */
    while(Copy_u32Number > 0)
    {
        Local_u8Digits[Local_u8Index] =
            (Copy_u32Number % 10) + '0';

        Copy_u32Number /= 10;

        Local_u8Index++;
    }


    /*
     * Print digits in correct order
     */
    while(Local_u8Index > 0)
    {
        Local_u8Index--;

        LCD_writeChar(
            Local_u8Digits[Local_u8Index]
        );
    }
}


/*=========================================================
 * LCD CLEAR
 *=========================================================*/

void LCD_clear(void)
{
    LCD_writeCommand(0x01);

    _delay_ms(2);
}


/*=========================================================
 * LCD GOTO POSITION
 *=========================================================*/

void LCD_goToSpecificPosition(
        u8 Copy_u8Line,
        u8 Copy_u8Position)
{
    /*
     * Valid positions:
     * 0 -> 15
     */
    if(Copy_u8Position > 15)
    {
        return;
    }


    /*
     * First line
     */
    if(Copy_u8Line == LCD_LINE_ONE)
    {
        LCD_writeCommand(
            0x80 + Copy_u8Position
        );
    }


    /*
     * Second line
     */
    else if(Copy_u8Line == LCD_LINE_TWO)
    {
        LCD_writeCommand(
            0xC0 + Copy_u8Position
        );
    }
}


/*=========================================================
 * LCD SHIFT
 *=========================================================*/

void LCD_shift(u8 Copy_u8Direction)
{
    /*
     * Shift left
     */
    if(Copy_u8Direction == LCD_SHIFT_LEFT)
    {
        LCD_writeCommand(0x18);
    }


    /*
     * Shift right
     */
    else if(Copy_u8Direction == LCD_SHIFT_RIGHT)
    {
        LCD_writeCommand(0x1C);
    }
}


/*=========================================================
 * COMPATIBILITY FUNCTION
 *
 * Old project code may call:
 * LCD_sendcmnd()
 *=========================================================*/

void LCD_sendcmnd(u8 Copy_u8Command)
{
    LCD_writeCommand(Copy_u8Command);
}


/*=========================================================
 * COMPATIBILITY FUNCTION
 *
 * Old project code may call:
 * LCD_sendChar()
 *=========================================================*/

void LCD_sendChar(u8 Copy_u8Data)
{
    LCD_writeChar(Copy_u8Data);
}


/*=========================================================
 * COMPATIBILITY FUNCTION
 *=========================================================*/

void writeHalfPort(u8 Copy_u8Value)
{
    LCD_Send4Bits(
        Copy_u8Value & 0x0F,
        0
    );
}
