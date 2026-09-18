/*
 * DIO_program.c
 *
 *  Created on: Aug 27, 2026
 *      Author: noure
 */



#include "../../STD_TYPES.H"
#include "../../BIT_MATH.h"

#include "DIO_interface.h"
#include "DIO_private.h"

void DIO_SetPinDirection 				(u8 PortId, u8 PinId, u8 PinDirection)
{

	if ((PortId <= 3) && (PinId <= 7) &&
	    ((DIO_PIN_OUTPUT == PinDirection) || (DIO_PIN_INPUT == PinDirection)))
	{

		switch (PortId)
		{
		case DIO_PORTA :
			if (DIO_PIN_OUTPUT == PinDirection)
			{
				SET_BIT(DDRA,PinId);
			}
			else
			{
				CLR_BIT(DDRA,PinId);
			}
			break;


		case DIO_PORTB :
			if (DIO_PIN_OUTPUT==PinDirection)
			{
				SET_BIT(DDRB,PinId);
			}
			else
			{
				CLR_BIT(DDRB,PinId);
			}
			break;


		case DIO_PORTC :
			if (DIO_PIN_OUTPUT==PinDirection)
			{
				SET_BIT(DDRC,PinId);
			}
			else
			{
				CLR_BIT(DDRC,PinId);
			}
			break;


		case DIO_PORTD :
			if (DIO_PIN_OUTPUT==PinDirection)
			{
				SET_BIT(DDRD,PinId);
			}
			else
			{
				CLR_BIT(DDRD,PinId);
			}
			break;


	}

	}
	else
	{
		//Do nothing
	}
}

void DIO_SetPinValue 					(u8 PortId, u8 PinId, u8 PinValue)
{

	switch (PortId)
	{
	case DIO_PORTA:
		if (DIO_PIN_HIGH==PinValue)
		{
			SET_BIT(PORTA,PinId);
		}
		else
		{
			CLR_BIT(PORTA,PinId);
		}
		break;


	case DIO_PORTB:
		if (DIO_PIN_HIGH==PinValue)
		{
			SET_BIT(PORTB,PinId);
		}
		else
		{
			CLR_BIT(PORTB,PinId);
		}
		break;


	case DIO_PORTC:
		if (DIO_PIN_HIGH==PinValue)
		{
			SET_BIT(PORTC,PinId);
		}
		else
		{
			CLR_BIT(PORTC,PinId);
		}
		break;


	case DIO_PORTD:
		if (DIO_PIN_HIGH==PinValue)
		{
			SET_BIT(PORTD,PinId);
		}
		else
		{
			CLR_BIT(PORTD,PinId);
		}
		break;
	}
}

void DIO_GetPinValue 					(u8 PortId, u8 PinId, u8* PinValue)
{

	switch(PortId)
	{

	case DIO_PORTA:
		*PinValue = GET_BIT(PINA,PinId);
	break;

	case DIO_PORTB:
		*PinValue = GET_BIT(PINB,PinId);
	break;

	case DIO_PORTC:
		*PinValue = GET_BIT(PINC,PinId);
	break;

	case DIO_PORTD:
		*PinValue = GET_BIT(PIND,PinId);
	break;

	}
}

void DIO_TogglePinValue					(u8 PortId, u8 PinId)
{

	switch(PortId)
	{

	case DIO_PORTA:
		TOG_BIT(PORTA,PinId);
		break;

	case DIO_PORTB:
			TOG_BIT(PORTB,PinId);
			break;

	case DIO_PORTC:
			TOG_BIT(PORTC,PinId);
			break;

	case DIO_PORTD:
			TOG_BIT(PORTD,PinId);
			break;
	}
}

void DIO_ActivePinInPullUpResistance 	(u8 PortId, u8 PinId)
{
	switch (PortId)
	{
	case DIO_PORTA:
		SET_BIT(PORTA,PinId);
		break;

	case DIO_PORTB:
			SET_BIT(PORTB,PinId);
			break;

	case DIO_PORTC:
			SET_BIT(PORTC,PinId);
			break;

	case DIO_PORTD:
			SET_BIT(PORTD,PinId);
			break;

	}
}


/*********************		PORT WORK		**********************/

void DIO_SetPortDirection 				(u8 PortId, u8 PortDirection)
{
	if(PortId<=3){

		switch (PortId)
		{
		case DIO_PORTA : DDRA = PortDirection ;
		break ;

		case DIO_PORTB : DDRB = PortDirection ;
		break ;

		case DIO_PORTC : DDRC = PortDirection ;
		break ;

		case DIO_PORTD : DDRD = PortDirection ;
		break ;

		}
	}
}

void DIO_SetPortValue 					(u8 PortId, u8 PortValue)
{
	if(PortId<=3)
	{
		switch (PortId)
		{

		case 0:
		PORTA = PortValue;
		break;

		case 1:
		PORTB = PortValue;
		break;

		case 2:
		PORTC = PortValue;
		break;

		case 3:
		PORTD = PortValue;
		break;

		}
	}

}

void DIO_GetPortValue 					(u8 PortId, u8* PortValue)
{
	if(  (PortId<=3) && (PortValue !=0) )
		{

			switch (PortId)
			{
			case 0 :
			*PortValue = PINA ;
			break;

			case 1 :
			*PortValue = PINB ;
			break;

			case 2 :
			*PortValue = PINC ;
			break;

			case 3 :
			*PortValue = PIND ;
			break;

			}
		}
}

void DIO_TogglePortValue				(u8 PortId)
{
	if (PortId<=3)
		{
			switch (PortId)
			{
			case 0 :
			PORTA=~PORTA ;
			break;

			case 1 :
			PORTB=~PORTB ;
			break;

			case 2 :
			PORTC=~PORTC ;
			break;

			case 3 :
			PORTD=~PORTD ;
			break;

			}
		}
}

void DIO_ActivePortInPullUpResistance 	(u8 PortId)
{
	if (PortId<=3)
		{
			switch (PortId)
			{
			case 0 :
			PORTA = 0xFF;
			break;

			case 1 :
			PORTB = 0xFF;
			break;


			case 2 :
			PORTC = 0xFF;
			break;


			case 3 :
			PORTD = 0xFF;
			break;

			}
		}
}

