/*
 * UART_private.h
 *
 *  Created on: Sep 9, 2026
 *      Author: YOSSEF ELSAEED
 */

#ifndef MCAL_UART_UART_PRIVATE_H_
#define MCAL_UART_UART_PRIVATE_H_

#define UART_UBRRH_REG	*((volatile u8*) 0x40)
#define UART_UCSRC_REG	*((volatile u8*) 0x40)

#define UART_UDR_REG	*((volatile u8*) 0x2C)

#define UART_UCSRA_REG	*((volatile u8*) 0x2B)
#define UART_UCSRB_REG	*((volatile u8*) 0x2A)

#define UART_UBRRL_REG	*((volatile u8*) 0x29)

#endif /* MCAL_UART_UART_PRIVATE_H_ */
