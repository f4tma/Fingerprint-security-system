#ifndef HAL_LCD_LCD_CONFIG_H_
#define HAL_LCD_LCD_CONFIG_H_


/*=========================================================
 * I2C LCD CONFIGURATION
 *=========================================================*/

/*
 * Common PCF8574 LCD backpack address.
 *
 * If the LCD does not work later, we will test 0x3F.
 */
#define LCD_I2C_ADDRESS       0x27


/*=========================================================
 * LCD LINES
 *=========================================================*/

#define LCD_LINE_ONE          0
#define LCD_LINE_TWO          1


/*=========================================================
 * LCD MODES
 *=========================================================*/

#define LCD_4_BIT_MODE        0
#define LCD_8_BIT_MODE        1

#define LCD_MODE              LCD_4_BIT_MODE


/*=========================================================
 * PCF8574 PIN MAPPING
 *
 * P0 -> RS
 * P1 -> RW
 * P2 -> EN
 * P3 -> BACKLIGHT
 * P4 -> D4
 * P5 -> D5
 * P6 -> D6
 * P7 -> D7
 *=========================================================*/

#define LCD_RS_BIT            0
#define LCD_RW_BIT            1
#define LCD_EN_BIT            2
#define LCD_BACKLIGHT_BIT     3

#define LCD_D4_BIT            4
#define LCD_D5_BIT            5
#define LCD_D6_BIT            6
#define LCD_D7_BIT            7


#endif /* HAL_LCD_LCD_CONFIG_H_ */
