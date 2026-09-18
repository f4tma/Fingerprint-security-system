/*
 * KEYPAD_program.c
 *
 *  Created on: Sep 9, 2026
 *      Author: noure
 */



#include <util/delay.h>

#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"

#include "KEYPAD_config.h"
#include "KEYPAD_interface.h"






void KEYPAD_Init(void)

{

	//  SET COLUMNS (PINS => 0 : 3 ) AS AN OUTPUT

	DIO_SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN_START,DIO_PIN_OUTPUT );
	DIO_SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN_START+1,DIO_PIN_OUTPUT );
	DIO_SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN_START+2,DIO_PIN_OUTPUT );
	DIO_SetPinDirection(KEYPAD_PORT,KEYPAD_COLUMN_START+3,DIO_PIN_OUTPUT );


	//  SET ROWS (PINS => 4 : 7 ) AS AN INPUT

	DIO_SetPinDirection  (KEYPAD_PORT,KEYPAD_ROW_START,DIO_PIN_INPUT);
	DIO_SetPinDirection(KEYPAD_PORT,KEYPAD_ROW_START+1,DIO_PIN_INPUT);
	DIO_SetPinDirection(KEYPAD_PORT,KEYPAD_ROW_START+2,DIO_PIN_INPUT);
	DIO_SetPinDirection(KEYPAD_PORT,KEYPAD_ROW_START+3,DIO_PIN_INPUT);



	DIO_SetPortValue(KEYPAD_PORT,DIO_PORT_HIGH );

}







u8 KEYPAD_GetPressed (void)

{


	u8 Local_KeyPressed =0 ; //  it means key is not pressed

	u8 Local_KeyStatus ;

	static const  u8 KPD_Keys [4][4] =  {

			{1,2,3,4    },
			{5,6,7,8    },
			{9,10,11,12 },
			{13,14,15,16}
	};




	for (u8 col =0 ; col<4 ;  col++)
	{

		// Activate target column (drive Low)

		DIO_SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN_START+col,DIO_PIN_LOW);


		for(u8 row =0 ; row< 4 ; row++ )
		{

			// Read Row Input State
			DIO_GetPinValue(KEYPAD_PORT, KEYPAD_ROW_START+row , &Local_KeyStatus );


			if (Local_KeyStatus == 0)
			{
				_delay_ms(30); // software debounce

				// Re- Verify Key Press After Debounce Delay
				DIO_GetPinValue(KEYPAD_PORT, KEYPAD_ROW_START+row , &Local_KeyStatus );


				if (Local_KeyStatus == 0)
				{
					//0:3 //0
					Local_KeyPressed = KPD_Keys [row][col];  //0


					//wait for keys release (polling untill pin returns HIGH )

					while (Local_KeyStatus == 0){
						DIO_GetPinValue(KEYPAD_PORT, KEYPAD_ROW_START+row , &Local_KeyStatus );


					}

					// reset column High before exiting

					DIO_SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN_START+col,DIO_PIN_HIGH);

					return Local_KeyPressed;
				}






			}




		}

		//  Deactive column (set back High)//

		DIO_SetPinValue(KEYPAD_PORT,KEYPAD_COLUMN_START+col,DIO_PIN_HIGH);

	}




	return Local_KeyPressed;  // return 0 if nothing pressed

}


