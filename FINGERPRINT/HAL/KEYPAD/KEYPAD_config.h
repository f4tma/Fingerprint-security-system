/*
 * KEYPAD_config.h
 *
 *  Created on: Sep 9, 2026
 *      Author: noure
 */

#ifndef HAL_KEYPAD_KEYPAD_CONFIG_H_
#define HAL_KEYPAD_KEYPAD_CONFIG_H_

#include "../../STD_TYPES.h"

extern const u8 KPD_Switch[4][4];

#define KEYPAD_PORT             DIO_PORTA

#define KEYPAD_COLUMN_START     DIO_PIN4
#define KEYPAD_COLUMN_END       DIO_PIN7

#define KEYPAD_ROW_START        DIO_PIN0
#define KEYPAD_ROW_END          DIO_PIN3

#endif
