/*
 * TIMERS_private.h
 *
 *  Created on: Sep 6, 2026
 *      Author: noure
 */

#ifndef MCAL_TIMERS_TIMERS_PRIVATE_H_
#define MCAL_TIMERS_TIMERS_PRIVATE_H_


#define  TIMERS_TCCR0_REG 		*((volatile u8* )0x53)

#define  TIMERS_TCNT0_REG 		*((volatile u8* )0x52)

#define  TIMERS_OCR0_REG 		*((volatile u8* )0x5C)

#define  TIMERS_TIMSK_REG 		*((volatile u8* )0x059)

#define  TIMERS_TIFR_REG 		*((volatile u8* )0x058)



/******************************TIMER1************************************/

#define  TIMERS_TCCR1A_REG 		*((volatile u8* )0x4F)

#define  TIMERS_TCCR1B_REG 		*((volatile u8* )0x4E)




#define  TIMERS_TCNT1L_REG 		*((volatile u8* )0x4C)
#define  TIMERS_TCNT1H_REG 		*((volatile u8* )0x4D)


#define  TIMERS_TCNT1_REG 		*((volatile u16* )0x4C)


#define  TIMERS_OCR1AL_REG 		*((volatile u8* )0x4A)
#define  TIMERS_OCR1AH_REG 		*((volatile u8* )0x49)
#define  TIMERS_OCR1A_REG 		*((volatile u16* )0x4A)



#define  TIMERS_ICR1AL_REG 		*((volatile u8* )0x46)
#define  TIMERS_ICR1AH_REG 		*((volatile u8* )0x47)
#define  TIMERS_ICR1A_REG 		*((volatile u16* )0x46)










#endif /* MCAL_TIMERS_TIMERS_PRIVATE_H_ */
