#ifndef MCAL_UART_UART_INTERFACE_H_
#define MCAL_UART_UART_INTERFACE_H_

#include "../../STD_TYPES.h"

void UART_VoidInit(void);

void UART_VoidSendByte(u8 Copy_u8DataByte);

u8 UART_VoidRecieveByte(u8 *Copy_pu8RecByte);

#endif
