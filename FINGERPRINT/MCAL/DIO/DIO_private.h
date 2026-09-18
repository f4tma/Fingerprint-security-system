/*
 * DIO_private.h
 *
 *  Created on: Aug 27, 2026
 *      Author: noure
 */

#ifndef MCAL_DIO_DIO_PRIVATE_H_
#define MCAL_DIO_DIO_PRIVATE_H_


/*GROUP A REGISTER*/

#define PORTA *((volatile u8*)0x3B)
#define DDRA *((volatile u8*)0x3A)
#define PINA *((volatile u8*)0x39)


/*GROUP B REGISTER*/

#define PORTB *((volatile u8*)0x38)
#define DDRB *((volatile u8*)0x37)
#define PINB *((volatile u8*)0x36)


/*GROUP C REGISTER*/

#define PORTC *((volatile u8*)0x35)
#define DDRC *((volatile u8*)0x34)
#define PINC *((volatile u8*)0x33)


/*GROUP D REGISTER*/

#define PORTD *((volatile u8*)0x32)
#define DDRD *((volatile u8*)0x31)
#define PIND *((volatile u8*)0x30)




#endif /* MCAL_DIO_DIO_PRIVATE_H_ */
