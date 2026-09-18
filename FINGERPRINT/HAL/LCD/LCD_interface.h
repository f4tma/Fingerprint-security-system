#ifndef HAL_LCD_LCD_INTERFACE_H_
#define HAL_LCD_LCD_INTERFACE_H_

#include "../../STD_TYPES.h"


/*=========================================================
 * LCD LINE DEFINITIONS
 *=========================================================*/

#define LCD_LINE_ONE       0
#define LCD_LINE_TWO       1


/*=========================================================
 * LCD MODE DEFINITIONS
 *=========================================================*/

#define LCD_4_BIT_MODE     0
#define LCD_8_BIT_MODE     1


/*=========================================================
 * LCD SHIFT DEFINITIONS
 *=========================================================*/

#define LCD_SHIFT_LEFT     0
#define LCD_SHIFT_RIGHT    1


/*=========================================================
 * PUBLIC FUNCTIONS
 *=========================================================*/

/* Initialize LCD */
void LCD_init(void);


/* Send command */
void LCD_writeCommand(u8 Copy_u8Command);


/* Send character */
void LCD_writeChar(u8 Copy_u8Data);


/* Send string */
void LCD_writeString(u8* Copy_pu8String);


/* Send number */
void LCD_writeNumber(u32 Copy_u32Number);


/* Move cursor */
void LCD_goToSpecificPosition(
        u8 Copy_u8Line,
        u8 Copy_u8Position
);


/* Clear LCD */
void LCD_clear(void);


/* Shift LCD */
void LCD_shift(u8 Copy_u8Direction);


/*
 * Compatibility functions
 * for older project code
 */
void LCD_sendcmnd(u8 Copy_u8Command);

void LCD_sendChar(u8 Copy_u8Data);

void writeHalfPort(u8 Copy_u8Value);


#endif /* HAL_LCD_LCD_INTERFACE_H_ */
