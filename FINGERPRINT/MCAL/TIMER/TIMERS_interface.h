/*
 * TIMERS_interface.h
 *
 *  Created on: Sep 6, 2026
 *      Author: noure
 */

#ifndef MCAL_TIMERS_TIMERS_INTERFACE_H_
#define MCAL_TIMERS_TIMERS_INTERFACE_H_




void TIMERS_voidTimer0Init (void);


u8 TIMERS_u8Timer0OVFSetCallBack(void(*copy_pf)(void));

u8 TIMERS_u8Timer0CTCSetCallBack(void(*copy_pf)(void));

void TIMERS_voidTimer0SetCompareMatchValue(u8 Copy_OCR0Value);

void TIMER1_voidSetTimeValue (u16 Copy_u16Value);


#endif /* MCAL_TIMERS_TIMERS_INTERFACE_H_ */
