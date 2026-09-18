/*
 * =========================================================
 * main.c
 *
 * Fingerprint Security System
 *
 * ATmega32
 *
 * Devices:
 *
 * R307       -> UART
 * I2C LCD    -> TWI
 * Keypad     -> DIO
 * LED        -> PB4
 * Buzzer     -> PB3
 *
 * =========================================================
 */


#include <util/delay.h>

#include "STD_TYPES.h"
#include "BIT_MATH.h"


/*=========================================================
 * MCAL
 *=========================================================*/

#include "MCAL/DIO/DIO_interface.h"

#include "MCAL/UART/UART_interface.h"

#include "MCAL/TWI/TWI_interface.h"


/*=========================================================
 * HAL
 *=========================================================*/

#include "HAL/LCD/LCD_interface.h"

#include "HAL/KEYPAD/KEYPAD_interface.h"

#include "HAL/R307/R307_interface.h"


/*=========================================================
 * SYSTEM CONFIGURATION
 *=========================================================*/

#define BUZZER_PORT             DIO_PORTB
#define BUZZER_PIN              DIO_PIN3

#define LED_PORT                DIO_PORTB
#define LED_PIN                 DIO_PIN4


#define MAX_TRIALS              3

#define FIRST_ID                1

#define NUMBER_OF_IDS           5


/*=========================================================
 * OUTPUT TYPES
 *=========================================================*/

#define OUTPUT_LED              1
#define OUTPUT_BUZZER           2


/*=========================================================
 * USER DATABASE
 *
 * R307 ID:
 *
 * 1 -> NOUR
 * 2 -> FATMA
 * 3 -> JANA
 * 4 -> ENG. FARED
 * 5 -> ENG. MOSTAFA
 *
 * IDs are fixed.
 *=========================================================*/

u8* GetUserName(u16 Copy_u16ID)
{
    switch(Copy_u16ID)
    {
        case 1:
            return (u8*)"NOUR";

        case 2:
            return (u8*)"FATMA";

        case 3:
            return (u8*)"JANA";

        case 4:
            return (u8*)"ENG. FARED";

        case 5:
            return (u8*)"ENG. MOSTAFA";

        default:
            return (u8*)"UNKNOWN";
    }
}


/*=========================================================
 * DISPLAY COMMUNICATION ERROR
 *=========================================================*/

void DisplayCommunicationError(void)
{
    LCD_clear();

    LCD_writeString(
        (u8*)"COMM ERROR"
    );

    _delay_ms(2000);
}


/*=========================================================
 * SELECT OUTPUT
 *
 * 1 -> LED
 * 2 -> BUZZER
 *=========================================================*/

u8 SelectOutput(void)
{
    u8 Local_u8Key = 0;


    while(1)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"CHOOSE OUTPUT"
        );


        LCD_goToSpecificPosition(
            LCD_LINE_TWO,
            0
        );


        LCD_writeString(
            (u8*)"1 LED  2 BUZZ"
        );


        Local_u8Key = 0;


        while(Local_u8Key == 0)
        {
            Local_u8Key =
                KEYPAD_GetPressed();
        }


        if(
            Local_u8Key == 1 ||
            Local_u8Key == 2
        )
        {
            return Local_u8Key;
        }


        LCD_clear();

        LCD_writeString(
            (u8*)"INVALID KEY"
        );

        _delay_ms(1000);
    }
}


/*=========================================================
 * ACTIVATE SUCCESSFUL OUTPUT
 *=========================================================*/

void ActivateSuccessfulOutput(
        u16 Copy_u16MatchedID)
{
    u8 Local_u8Output;


    /*
     * Show access granted
     */
    LCD_clear();

    LCD_writeString(
        (u8*)"ACCESS GRANTED"
    );


    LCD_goToSpecificPosition(
        LCD_LINE_TWO,
        0
    );


    LCD_writeString(
        GetUserName(Copy_u16MatchedID)
    );


    _delay_ms(2000);


    /*
     * Ask user which output to activate
     */
    Local_u8Output =
        SelectOutput();


    /*
     * -----------------------------------------------------
     * LED
     * -----------------------------------------------------
     */

    if(Local_u8Output == OUTPUT_LED)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"LED ACTIVE"
        );


        DIO_SetPinValue(
            LED_PORT,
            LED_PIN,
            DIO_PIN_HIGH
        );


        _delay_ms(3000);


        DIO_SetPinValue(
            LED_PORT,
            LED_PIN,
            DIO_PIN_LOW
        );
    }


    /*
     * -----------------------------------------------------
     * BUZZER
     * -----------------------------------------------------
     */

    else if(Local_u8Output == OUTPUT_BUZZER)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"BUZZER ACTIVE"
        );


        DIO_SetPinValue(
            BUZZER_PORT,
            BUZZER_PIN,
            DIO_PIN_HIGH
        );


        _delay_ms(3000);


        DIO_SetPinValue(
            BUZZER_PORT,
            BUZZER_PIN,
            DIO_PIN_LOW
        );
    }
}


/*=========================================================
 * SELECT USER FOR ENROLLMENT
 *=========================================================*/

u8 SelectUser(void)
{
    u8 Local_u8Key;


    while(1)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"1 NOUR 2 FATMA"
        );


        LCD_goToSpecificPosition(
            LCD_LINE_TWO,
            0
        );


        LCD_writeString(
            (u8*)"3 JANA 4 FARED"
        );


        Local_u8Key = 0;


        while(Local_u8Key == 0)
        {
            Local_u8Key =
                KEYPAD_GetPressed();
        }


        /*
         * IDs 1-4
         */
        if(
            Local_u8Key >= 1 &&
            Local_u8Key <= 4
        )
        {
            return Local_u8Key;
        }


        /*
         * ID 5
         */
        if(Local_u8Key == 5)
        {
            LCD_clear();

            LCD_writeString(
                (u8*)"5 MOSTAFA"
            );

            _delay_ms(1000);

            return 5;
        }


        /*
         * Invalid key
         */
        LCD_clear();

        LCD_writeString(
            (u8*)"INVALID KEY"
        );

        _delay_ms(1000);
    }
}


/*=========================================================
 * ENROLL FINGERPRINT
 *=========================================================*/

void EnrollFingerprint(u8 Copy_u8ID)
{
    u8 Local_u8Result;


    /*
     * -----------------------------------------------------
     * SHOW USER
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"REGISTERING"
    );


    LCD_goToSpecificPosition(
        LCD_LINE_TWO,
        0
    );


    LCD_writeString(
        GetUserName(Copy_u8ID)
    );


    _delay_ms(1500);


    /*
     * -----------------------------------------------------
     * FIRST SCAN
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"PLACE FINGER"
    );


    do
    {
        Local_u8Result =
            R307_GenImg();

    }while(
        Local_u8Result ==
        R307_NO_FINGER
    );


    /*
     * Scan error
     */
    if(Local_u8Result != R307_CONFIRM_OK)
    {
        if(
            Local_u8Result ==
            R307_COMMUNICATION_ERROR
        )
        {
            DisplayCommunicationError();
        }
        else
        {
            LCD_clear();

            LCD_writeString(
                (u8*)"SCAN FAILED"
            );

            _delay_ms(1500);
        }

        return;
    }


    /*
     * -----------------------------------------------------
     * CONVERT FIRST IMAGE
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"READING 1..."
    );


    _delay_ms(500);


    Local_u8Result =
        R307_Image2Tz(1);


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"IMAGE 1 FAIL"
        );

        _delay_ms(1500);

        return;
    }


    /*
     * -----------------------------------------------------
     * REMOVE FINGER
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"REMOVE FINGER"
    );


    _delay_ms(1000);


    do
    {
        Local_u8Result =
            R307_GenImg();

    }while(
        Local_u8Result !=
        R307_NO_FINGER
    );


    /*
     * -----------------------------------------------------
     * SECOND SCAN
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"PLACE AGAIN"
    );


    do
    {
        Local_u8Result =
            R307_GenImg();

    }while(
        Local_u8Result ==
        R307_NO_FINGER
    );


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"SCAN 2 FAILED"
        );

        _delay_ms(1500);

        return;
    }


    /*
     * -----------------------------------------------------
     * CONVERT SECOND IMAGE
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"READING 2..."
    );


    _delay_ms(500);


    Local_u8Result =
        R307_Image2Tz(2);


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"IMAGE 2 FAIL"
        );

        _delay_ms(1500);

        return;
    }


    /*
     * -----------------------------------------------------
     * CREATE MODEL
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"CREATING..."
    );


    _delay_ms(500);


    Local_u8Result =
        R307_RegModel();


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"MODEL FAILED"
        );

        _delay_ms(1500);

        return;
    }


    /*
     * -----------------------------------------------------
     * STORE
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"SAVING..."
    );


    _delay_ms(500);


    Local_u8Result =
        R307_Store(
            1,
            Copy_u8ID
        );


    /*
     * -----------------------------------------------------
     * RESULT
     * -----------------------------------------------------
     */

    LCD_clear();


    if(Local_u8Result == R307_CONFIRM_OK)
    {
        LCD_writeString(
            (u8*)"FINGER SAVED"
        );


        LCD_goToSpecificPosition(
            LCD_LINE_TWO,
            0
        );


        LCD_writeString(
            GetUserName(Copy_u8ID)
        );


        _delay_ms(2500);
    }
    else
    {
        LCD_writeString(
            (u8*)"SAVE FAILED"
        );

        _delay_ms(2000);
    }
}


/*=========================================================
 * DELETE FINGERPRINT
 *=========================================================*/

void DeleteFingerprint(void)
{
    u8 Local_u8Key;

    u8 Local_u8Result;


    /*
     * Ask ID
     */
    LCD_clear();

    LCD_writeString(
        (u8*)"DELETE ID 1-5"
    );


    LCD_goToSpecificPosition(
        LCD_LINE_TWO,
        0
    );


    LCD_writeString(
        (u8*)"PRESS 1 TO 5"
    );


    Local_u8Key = 0;


    while(Local_u8Key == 0)
    {
        Local_u8Key =
            KEYPAD_GetPressed();
    }


    /*
     * Validate ID
     */
    if(
        Local_u8Key < 1 ||
        Local_u8Key > 5
    )
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"INVALID ID"
        );

        _delay_ms(1500);

        return;
    }


    /*
     * Show selected user
     */
    LCD_clear();

    LCD_writeString(
        (u8*)"DELETE:"
    );


    LCD_goToSpecificPosition(
        LCD_LINE_TWO,
        0
    );


    LCD_writeString(
        GetUserName(Local_u8Key)
    );


    _delay_ms(1500);


    /*
     * Delete fingerprint
     */
    Local_u8Result =
        R307_Delete(Local_u8Key);


    /*
     * Result
     */
    LCD_clear();


    if(Local_u8Result == R307_CONFIRM_OK)
    {
        LCD_writeString(
            (u8*)"DELETED"
        );


        LCD_goToSpecificPosition(
            LCD_LINE_TWO,
            0
        );


        LCD_writeString(
            GetUserName(Local_u8Key)
        );


        _delay_ms(2000);
    }
    else
    {
        LCD_writeString(
            (u8*)"DELETE FAILED"
        );

        _delay_ms(2000);
    }
}


/*=========================================================
 * CHECK FINGERPRINT
 *=========================================================*/

u8 CheckFingerprint(
        u8* Copy_pu8Trials)
{
    u8 Local_u8Result;

    u16 Local_u16MatchedID;


    /*
     * -----------------------------------------------------
     * WAIT FOR FINGER
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"PLACE FINGER"
    );


    do
    {
        Local_u8Result =
            R307_GenImg();

    }while(
        Local_u8Result ==
        R307_NO_FINGER
    );


    /*
     * Communication error
     */
    if(
        Local_u8Result ==
        R307_COMMUNICATION_ERROR
    )
    {
        DisplayCommunicationError();

        return 0;
    }


    /*
     * Finger scan failed
     */
    if(Local_u8Result != R307_CONFIRM_OK)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"SCAN FAILED"
        );

        _delay_ms(1500);

        return 0;
    }


    /*
     * -----------------------------------------------------
     * IMAGE -> CHARACTER FILE
     * -----------------------------------------------------
     */

    LCD_clear();

    LCD_writeString(
        (u8*)"CHECKING..."
    );


    Local_u8Result =
        R307_Image2Tz(1);


    if(
        Local_u8Result ==
        R307_COMMUNICATION_ERROR
    )
    {
        DisplayCommunicationError();

        return 0;
    }


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        LCD_clear();

        LCD_writeString(
            (u8*)"IMAGE FAIL"
        );

        _delay_ms(1500);

        return 0;
    }


    /*
     * -----------------------------------------------------
     * SEARCH DATABASE
     *
     * Start ID = 1
     * Number of IDs = 5
     * -----------------------------------------------------
     */

    Local_u8Result =
        R307_Search(
            FIRST_ID,
            NUMBER_OF_IDS,
            &Local_u16MatchedID
        );


    /*
     * Communication error
     */
    if(
        Local_u8Result ==
        R307_COMMUNICATION_ERROR
    )
    {
        DisplayCommunicationError();

        return 0;
    }


    /*
     * -----------------------------------------------------
     * MATCH FOUND
     * -----------------------------------------------------
     */

    if(Local_u8Result == R307_CONFIRM_OK)
    {
        /*
         * Verify returned ID is inside our database.
         */
        if(
            Local_u16MatchedID >= FIRST_ID &&
            Local_u16MatchedID <= NUMBER_OF_IDS
        )
        {
            /*
             * Successful match.
             */
            *Copy_pu8Trials =
                MAX_TRIALS;


            ActivateSuccessfulOutput(
                Local_u16MatchedID
            );


            return 1;
        }


        /*
         * Invalid returned ID
         */
        LCD_clear();

        LCD_writeString(
            (u8*)"INVALID ID"
        );

        _delay_ms(1500);

        return 0;
    }


    /*
     * -----------------------------------------------------
     * NO MATCH
     * -----------------------------------------------------
     */

    (*Copy_pu8Trials)--;


    LCD_clear();

    LCD_writeString(
        (u8*)"ACCESS DENIED"
    );


    LCD_goToSpecificPosition(
        LCD_LINE_TWO,
        0
    );


    LCD_writeString(
        (u8*)"TRIALS LEFT: "
    );


    LCD_writeNumber(
        *Copy_pu8Trials
    );


    _delay_ms(2000);


    return 0;
}


/*=========================================================
 * MAIN
 *=========================================================*/

int main(void)
{
    u8 Local_u8Key;

    u8 Local_u8EnrollID;

    u8 Local_u8Trials =
        MAX_TRIALS;


    /*=====================================================
     * INITIALIZATION
     *=====================================================*/

    /*
     * TWI MUST BE INITIALIZED
     * BEFORE I2C LCD.
     */
    TWI_voidInitMaster(0);


    /*
     * LCD
     */
    LCD_init();


    /*
     * Keypad
     */
    KEYPAD_Init();


    /*
     * R307
     */
    R307_Init();


    /*=====================================================
     * BUZZER
     *=====================================================*/

    DIO_SetPinDirection(
        BUZZER_PORT,
        BUZZER_PIN,
        DIO_PIN_OUTPUT
    );


    DIO_SetPinValue(
        BUZZER_PORT,
        BUZZER_PIN,
        DIO_PIN_LOW
    );


    /*=====================================================
     * LED
     *=====================================================*/

    DIO_SetPinDirection(
        LED_PORT,
        LED_PIN,
        DIO_PIN_OUTPUT
    );


    DIO_SetPinValue(
        LED_PORT,
        LED_PIN,
        DIO_PIN_LOW
    );


    /*=====================================================
     * STARTUP SCREEN
     *=====================================================*/

    LCD_clear();

    LCD_writeString(
        (u8*)"FINGERPRINT"
    );


    LCD_goToSpecificPosition(
        LCD_LINE_TWO,
        1
    );


    LCD_writeString(
        (u8*)"SECURITY"
    );


    _delay_ms(2000);


    /*=====================================================
     * MAIN LOOP
     *=====================================================*/

    while(1)
    {
        /*
         * -------------------------------------------------
         * MAIN MENU
         * -------------------------------------------------
         */

        LCD_clear();


        LCD_writeString(
            (u8*)"1 CHECK"
        );


        LCD_goToSpecificPosition(
            LCD_LINE_TWO,
            0
        );


        LCD_writeString(
            (u8*)"2 ADD 3 DELETE"
        );


        /*
         * Wait for keypad
         */
        Local_u8Key = 0;


        while(Local_u8Key == 0)
        {
            Local_u8Key =
                KEYPAD_GetPressed();
        }


        /*=================================================
         * 1 -> CHECK
         *=================================================*/

        if(Local_u8Key == 1)
        {
            /*
             * If trials already reached zero,
             * lock system.
             */
            if(Local_u8Trials == 0)
            {
                LCD_clear();

                LCD_writeString(
                    (u8*)"SYSTEM LOCKED"
                );


                LCD_goToSpecificPosition(
                    LCD_LINE_TWO,
                    0
                );


                LCD_writeString(
                    (u8*)"3 FAILURES"
                );


                /*
                 * Buzzer ON
                 */
                DIO_SetPinValue(
                    BUZZER_PORT,
                    BUZZER_PIN,
                    DIO_PIN_HIGH
                );


                _delay_ms(5000);


                /*
                 * Buzzer OFF
                 */
                DIO_SetPinValue(
                    BUZZER_PORT,
                    BUZZER_PIN,
                    DIO_PIN_LOW
                );


                /*
                 * Reset trials
                 */
                Local_u8Trials =
                    MAX_TRIALS;


                continue;
            }


            /*
             * Perform fingerprint check.
             */
            CheckFingerprint(
                &Local_u8Trials
            );
        }


        /*=================================================
         * 2 -> ADD
         *=================================================*/

        else if(Local_u8Key == 2)
        {
            /*
             * Select fixed user ID.
             */
            Local_u8EnrollID =
                SelectUser();


            /*
             * Enroll fingerprint.
             */
            EnrollFingerprint(
                Local_u8EnrollID
            );
        }


        /*=================================================
         * 3 -> DELETE
         *=================================================*/

        else if(Local_u8Key == 3)
        {
            DeleteFingerprint();
        }


        /*=================================================
         * INVALID MAIN MENU KEY
         *=================================================*/

        else
        {
            LCD_clear();

            LCD_writeString(
                (u8*)"INVALID KEY"
            );

            _delay_ms(1000);
        }
    }
}
