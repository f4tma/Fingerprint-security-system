#include "../../STD_TYPES.h"

#include "../../BIT_MATH.h"

#include "TIMERS_private.h"
#include "TIMERS_interface.h"

void TIMER1_voidSetTimerValue(u16 Copy_u16Value)
{
	TIMERS_TCNT1_REG = Copy_u16Value;

}

u16 TIMER1_u16ReadTimerValue(void)
{
	return TIMERS_TCNT1_REG;
}

void TIMERS_voidTimer1Init (void){

	//Normal Mode : All WGM bits = 0
		CLR_BIT(TIMERS_TCCR1A_REG, 0);
		CLR_BIT(TIMERS_TCCR1A_REG, 1);
		CLR_BIT(TIMERS_TCCR1B_REG, 3);
		CLR_BIT(TIMERS_TCCR1B_REG, 4);

	//Prescalar 8 (1 Tick = 1 us at 8MHz
		CLR_BIT(TIMERS_TCCR1B_REG, 0);
		SET_BIT(TIMERS_TCCR1B_REG, 1);
		CLR_BIT(TIMERS_TCCR1B_REG, 2);


		   CLR_BIT( TIMERS_TCCR0_REG,6 );
		   CLR_BIT( TIMERS_TCCR0_REG,3 );

			SET_BIT(TIMERS_TCCR0_REG,5);
		    CLR_BIT(TIMERS_TCCR0_REG,4);

/*

	Select Mode Fast Pwm   Mode => 14


	SET_BIT(TIMERS_TCCR1A_REG,1);

	CLR_BIT(TIMERS_TCCR1A_REG,0);

	SET_BIT(TIMERS_TCCR1B_REG,4);

	SET_BIT(TIMERS_TCCR1B_REG,3);

Select HW Action OCR1  => Non Inverting mode


	SET_BIT(TIMERS_TCCR1A_REG,7);

	CLR_BIT(TIMERS_TCCR1A_REG,6);



PreLoad Value of ICR

	TIMERS_ICR1AH_REG = (u8)(19999 >> 8);
		TIMERS_ICR1AL_REG = (u8)(19999 & 0x00FF);


	//Set Prescaler Value      CLK / 8

		CLR_BIT(TIMERS_TCCR1B_REG,0);
		SET_BIT(TIMERS_TCCR1B_REG,1);
		CLR_BIT(TIMERS_TCCR1B_REG,2);
*/


}


void TIMERS_voidTimer1SetCompareMatchValue(u16 Copy_OCR1Value)
{


	TIMERS_OCR1AH_REG = (u8)(Copy_OCR1Value >> 8);
		TIMERS_OCR1AL_REG = (u8)(Copy_OCR1Value & 0x00FF);


}





/*Global Pointer to Function Of Timer0  OVF & CTC*/


static void(*TIMERS_pfTimer0OVF)(void)  = NULL    ;

static void(*TIMERS_pfTimer0CTC)(void)  = NULL    ;



void TIMERS_voidTimer0SetCompareMatchValue(u8 Copy_OCR0Value){


	TIMERS_OCR0_REG=Copy_OCR0Value;
}







void TIMERS_voidTimer0Init (void)

{




/*

	CLR_BIT( TIMERS_TCCR0_REG,6 );
	SET_BIT( TIMERS_TCCR0_REG,3 );


//2- Disable OVF Interrupt

	CLR_BIT(TIMERS_TIMSK_REG,0);


//3-Enable CTC Interrupt

	SET_BIT(TIMERS_TIMSK_REG,1);


	//TIMERS_OCR0_REG = 99;									// OCR0  == TCNT0





    OVF MODE      configration
	//1- Select Mode => Normal Mode (Over Flow

   CLR_BIT( TIMERS_TCCR0_REG,6 );
   CLR_BIT( TIMERS_TCCR0_REG,3 );



	2- Enable OVF Interrupt

	SET_BIT(TIMERS_TIMSK_REG,0);


	3-Disable CTC Interrupt

	CLR_BIT(TIMERS_TIMSK_REG,1);


	4- Set Preload Value

	TIMERS_TCNT0_REG = 192 ;   // 256- 64 = 192  64  TICKES







	Set Prescaler Value         //CLK / 8



	CLR_BIT(TIMERS_TCCR0_REG,0);
	SET_BIT(TIMERS_TCCR0_REG,1);
	CLR_BIT(TIMERS_TCCR0_REG,2);
*/





	/*Select Fast PWM  Mode */

	SET_BIT(TIMERS_TCCR0_REG,6);
	SET_BIT(TIMERS_TCCR0_REG,3);


	/*Select Hardware Actoin  on oc0 => Non inverting */
	SET_BIT(TIMERS_TCCR0_REG,5);
		CLR_BIT(TIMERS_TCCR0_REG,4);


		//Set Prescaler Value      CLK / 8



		CLR_BIT(TIMERS_TCCR0_REG,0);
		SET_BIT(TIMERS_TCCR0_REG,1);
		CLR_BIT(TIMERS_TCCR0_REG,2);





}








u8 TIMERS_u8Timer0OVFSetCallBack(void(*copy_pf)(void))



{
	u8 Local_u8ErrorState =  STD_TYPES_OK ;

	if(copy_pf!=NULL)

	{
		TIMERS_pfTimer0OVF  = copy_pf  ;

	}

	else
	{
		Local_u8ErrorState = STD_TYPES_NOK ;

	}

	return Local_u8ErrorState  ;
}




u8 TIMERS_u8Timer0CTCSetCallBack(void(*copy_pf)(void)){

	u8 Local_u8ErrorState =  STD_TYPES_OK ;

	if(copy_pf!=NULL)

	{
		TIMERS_pfTimer0CTC  = copy_pf  ;

	}

	else
	{
		Local_u8ErrorState = STD_TYPES_NOK ;

	}

	return Local_u8ErrorState  ;




}






















/*Prototype  for ISR of TIMER0  OVF */

void __vector_11(void) __attribute__((signal));



void __vector_11(void)
{

	static u16 Local_u16CounterOVF = 0;

	Local_u16CounterOVF++;

	if (Local_u16CounterOVF==3907)

	{
		/*Uptade Preload Value */

		TIMERS_TCNT0_REG = 192 ;


		/*Clear Counter  */
		Local_u16CounterOVF =0 ;

		if (TIMERS_pfTimer0OVF !=NULL)  // X != 0   ADDERSS
		{
			TIMERS_pfTimer0OVF();



		}




	}




}







void __vector_10(void) __attribute__((signal));



void __vector_10(void)				// CTC   OCR 0      100    count 1 sec   ocr ==  TCNT
																		//	100  == 1000 000
{

	static u16 Local_u16CounterCTC = 0;

	Local_u16CounterCTC++;

	if (Local_u16CounterCTC == 10000)

	{



		/*Clear Counter  */
		Local_u16CounterCTC =0 ;

		if (TIMERS_pfTimer0CTC !=NULL)  // X != 0   ADDERSS
		{
			TIMERS_pfTimer0CTC();



		}




	}



}
