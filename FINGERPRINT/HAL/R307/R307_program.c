/*
 * R307_program.c
 *
 * R307 Fingerprint Sensor Driver
 *
 * ATmega32 UART <-> R307
 *
 * UART:
 * ATmega32 PD0 (RXD) <- R307 TX
 * ATmega32 PD1 (TXD) -> R307 RX
 *
 * Default R307 baud rate:
 * 57600 bps
 *
 * Functions:
 *
 * R307_Init()
 * R307_GenImg()
 * R307_Image2Tz()
 * R307_Search()
 * R307_RegModel()
 * R307_Store()
 * R307_Delete()
 * R307_Enroll()
 *
 */


/*==========================================================
 * INCLUDES
 *==========================================================*/

#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"

#include "../../MCAL/UART/UART_interface.h"

#include "R307_interface.h"
#include "R307_private.h"
#include "R307_config.h"


/*==========================================================
 * PRIVATE FUNCTIONS
 *==========================================================*/


/*
 * Receive a normal R307 ACK packet.
 *
 * Normal ACK packet = 12 bytes
 *
 * Byte 0  = EF
 * Byte 1  = 01
 * Byte 2  = Address
 * Byte 3  = Address
 * Byte 4  = Address
 * Byte 5  = Address
 * Byte 6  = 07
 * Byte 7  = Length High
 * Byte 8  = Length Low
 * Byte 9  = Confirmation Code
 * Byte 10 = Checksum High
 * Byte 11 = Checksum Low
 *
 */

static u8 R307_ReceiveAck(u8 *Copy_pu8Response)
{
    u8 Local_u8Index;
    u8 Local_u8Result;


    /*----------------------------------------------
     * Check pointer
     *----------------------------------------------*/

    if(Copy_pu8Response == NULL)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*----------------------------------------------
     * Receive 12-byte response
     *----------------------------------------------*/

    for(Local_u8Index = 0;
        Local_u8Index < R307_ACK_RESPONSE_SIZE;
        Local_u8Index++)
    {
        Local_u8Result =
                UART_VoidRecieveByte(
                        &Copy_pu8Response[Local_u8Index]);


        /*------------------------------------------
         * UART timeout / communication error
         *------------------------------------------*/

        if(Local_u8Result != STD_TYPES_OK)
        {
            return R307_COMMUNICATION_ERROR;
        }
    }


    /*----------------------------------------------
     * Validate packet header
     *----------------------------------------------*/

    if(Copy_pu8Response[0] != R307_HEADER_HIGH)
    {
        return R307_COMMUNICATION_ERROR;
    }


    if(Copy_pu8Response[1] != R307_HEADER_LOW)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*----------------------------------------------
     * Validate packet type
     *----------------------------------------------*/

    if(Copy_pu8Response[6] != R307_ACK_PACKET)
    {
        return R307_COMMUNICATION_ERROR;
    }


    return STD_TYPES_OK;
}


/*==========================================================
 * R307 INITIALIZATION
 *==========================================================*/

void R307_Init(void)
{
    /*
     * R307 communicates through UART.
     */

    UART_VoidInit();
}


/*==========================================================
 * GENERATE IMAGE
 *
 * Instruction:
 * 01 = GenImg
 *
 * Command:
 *
 * EF 01
 * FF FF FF FF
 * 01
 * 00 03
 * 01
 * 00 05
 *
 *==========================================================*/

u8 R307_GenImg(void)
{
    u8 Local_u8Response[R307_ACK_RESPONSE_SIZE];

    u8 Local_u8Result;


    /*======================================================
     * SEND HEADER
     *======================================================*/

    UART_VoidSendByte(R307_HEADER_HIGH);
    UART_VoidSendByte(R307_HEADER_LOW);


    /*======================================================
     * SEND ADDRESS
     *======================================================*/

    UART_VoidSendByte(R307_ADDRESS_BYTE_1);
    UART_VoidSendByte(R307_ADDRESS_BYTE_2);
    UART_VoidSendByte(R307_ADDRESS_BYTE_3);
    UART_VoidSendByte(R307_ADDRESS_BYTE_4);


    /*======================================================
     * PACKET TYPE = COMMAND
     *======================================================*/

    UART_VoidSendByte(R307_COMMAND_PACKET);


    /*======================================================
     * PACKET LENGTH = 3
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x03);


    /*======================================================
     * INSTRUCTION = GENIMG
     *======================================================*/

    UART_VoidSendByte(R307_CMD_GENIMG);


    /*======================================================
     * CHECKSUM
     *
     * 01 + 00 + 03 + 01 = 05
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x05);


    /*======================================================
     * RECEIVE RESPONSE
     *======================================================*/

    Local_u8Result =
            R307_ReceiveAck(Local_u8Response);


    /*======================================================
     * COMMUNICATION ERROR
     *======================================================*/

    if(Local_u8Result != STD_TYPES_OK)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * RETURN CONFIRMATION CODE
     *======================================================*/

    return Local_u8Response[9];
}


/*==========================================================
 * IMAGE2TZ
 *
 * Buffer:
 *
 * 1 -> Character Buffer 1
 * 2 -> Character Buffer 2
 *
 *==========================================================*/

u8 R307_Image2Tz(u8 Copy_u8BufferID)
{
    u8 Local_u8Response[R307_ACK_RESPONSE_SIZE];

    u8 Local_u8Result;

    u8 Local_u8Checksum;


    /*======================================================
     * CHECK BUFFER ID
     *======================================================*/

    if((Copy_u8BufferID != 1) &&
       (Copy_u8BufferID != 2))
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * SEND HEADER
     *======================================================*/

    UART_VoidSendByte(R307_HEADER_HIGH);
    UART_VoidSendByte(R307_HEADER_LOW);


    /*======================================================
     * SEND ADDRESS
     *======================================================*/

    UART_VoidSendByte(R307_ADDRESS_BYTE_1);
    UART_VoidSendByte(R307_ADDRESS_BYTE_2);
    UART_VoidSendByte(R307_ADDRESS_BYTE_3);
    UART_VoidSendByte(R307_ADDRESS_BYTE_4);


    /*======================================================
     * PACKET TYPE = COMMAND
     *======================================================*/

    UART_VoidSendByte(R307_COMMAND_PACKET);


    /*======================================================
     * PACKET LENGTH = 4
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x04);


    /*======================================================
     * INSTRUCTION = IMAGE2TZ
     *======================================================*/

    UART_VoidSendByte(R307_CMD_IMAGE2TZ);


    /*======================================================
     * BUFFER ID
     *======================================================*/

    UART_VoidSendByte(Copy_u8BufferID);


    /*======================================================
     * CHECKSUM
     *
     * 01 + 04 + 02 + Buffer ID
     *======================================================*/

    Local_u8Checksum =
            0x01 +
            0x04 +
            0x02 +
            Copy_u8BufferID;


    UART_VoidSendByte(0x00);
    UART_VoidSendByte(Local_u8Checksum);


    /*======================================================
     * RECEIVE RESPONSE
     *======================================================*/

    Local_u8Result =
            R307_ReceiveAck(Local_u8Response);


    /*======================================================
     * COMMUNICATION ERROR
     *======================================================*/

    if(Local_u8Result != STD_TYPES_OK)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * RETURN CONFIRMATION CODE
     *======================================================*/

    return Local_u8Response[9];
}


/*==========================================================
 * SEARCH
 *
 * Searches the R307 database.
 *
 * Start ID = first ID
 *
 * Number = number of IDs to search
 *
 * Matched ID = returned fingerprint ID
 *
 *
 * Search response = 16 bytes
 *
 * Byte 9  = Confirmation Code
 * Byte 10 = Matched ID High
 * Byte 11 = Matched ID Low
 *
 *==========================================================*/

u8 R307_Search(
        u16 Copy_u16StartID,
        u16 Copy_u16Number,
        u16 *Copy_pu16MatchedID)
{
    u8 Local_u8Response[R307_SEARCH_RESPONSE_SIZE];

    u8 Local_u8Index;

    u8 Local_u8Result;

    u16 Local_u16Checksum;


    /*======================================================
     * CHECK POINTER
     *======================================================*/

    if(Copy_pu16MatchedID == NULL)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * CLEAR MATCHED ID
     *======================================================*/

    *Copy_pu16MatchedID = 0;


    /*======================================================
     * SEND HEADER
     *======================================================*/

    UART_VoidSendByte(R307_HEADER_HIGH);
    UART_VoidSendByte(R307_HEADER_LOW);


    /*======================================================
     * SEND ADDRESS
     *======================================================*/

    UART_VoidSendByte(R307_ADDRESS_BYTE_1);
    UART_VoidSendByte(R307_ADDRESS_BYTE_2);
    UART_VoidSendByte(R307_ADDRESS_BYTE_3);
    UART_VoidSendByte(R307_ADDRESS_BYTE_4);


    /*======================================================
     * PACKET TYPE = COMMAND
     *======================================================*/

    UART_VoidSendByte(R307_COMMAND_PACKET);


    /*======================================================
     * PACKET LENGTH = 8
     *
     * Instruction
     * Buffer ID
     * Start ID High
     * Start ID Low
     * Number High
     * Number Low
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x08);


    /*======================================================
     * INSTRUCTION = SEARCH
     *======================================================*/

    UART_VoidSendByte(R307_CMD_SEARCH);


    /*======================================================
     * BUFFER ID = 1
     *======================================================*/

    UART_VoidSendByte(0x01);


    /*======================================================
     * START ID
     *======================================================*/

    UART_VoidSendByte(
            (u8)(Copy_u16StartID >> 8));

    UART_VoidSendByte(
            (u8)Copy_u16StartID);


    /*======================================================
     * NUMBER OF IDs
     *======================================================*/

    UART_VoidSendByte(
            (u8)(Copy_u16Number >> 8));

    UART_VoidSendByte(
            (u8)Copy_u16Number);


    /*======================================================
     * CHECKSUM
     *
     * 01 + 08 + 04 + 01
     * + Start High
     * + Start Low
     * + Number High
     * + Number Low
     *
     *======================================================*/

    Local_u16Checksum =
            0x01 +
            0x08 +
            0x04 +
            0x01 +
            (Copy_u16StartID >> 8) +
            (Copy_u16StartID & 0xFF) +
            (Copy_u16Number >> 8) +
            (Copy_u16Number & 0xFF);


    /*======================================================
     * SEND CHECKSUM
     *======================================================*/

    UART_VoidSendByte(
            (u8)(Local_u16Checksum >> 8));

    UART_VoidSendByte(
            (u8)Local_u16Checksum);


    /*======================================================
     * RECEIVE SEARCH RESPONSE
     *======================================================*/

    for(Local_u8Index = 0;
        Local_u8Index < R307_SEARCH_RESPONSE_SIZE;
        Local_u8Index++)
    {
        Local_u8Result =
                UART_VoidRecieveByte(
                        &Local_u8Response[Local_u8Index]);


        if(Local_u8Result != STD_TYPES_OK)
        {
            return R307_COMMUNICATION_ERROR;
        }
    }


    /*======================================================
     * VALIDATE HEADER
     *======================================================*/

    if(Local_u8Response[0] != R307_HEADER_HIGH)
    {
        return R307_COMMUNICATION_ERROR;
    }


    if(Local_u8Response[1] != R307_HEADER_LOW)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * VALIDATE PACKET TYPE
     *======================================================*/

    if(Local_u8Response[6] != R307_ACK_PACKET)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * CHECK CONFIRMATION CODE
     *======================================================*/

    if(Local_u8Response[9] == R307_CONFIRM_OK)
    {
        /*----------------------------------------------
         * Extract matched ID
         *
         * Byte 10 = High
         * Byte 11 = Low
         *----------------------------------------------*/

        *Copy_pu16MatchedID =
                ((u16)Local_u8Response[10] << 8) |
                Local_u8Response[11];


        return R307_CONFIRM_OK;
    }


    /*======================================================
     * NO MATCH / R307 ERROR
     *======================================================*/

    return Local_u8Response[9];
}


/*==========================================================
 * REGISTER MODEL
 *
 * Combines:
 *
 * Buffer 1 + Buffer 2
 *
 *==========================================================*/

u8 R307_RegModel(void)
{
    u8 Local_u8Response[R307_ACK_RESPONSE_SIZE];

    u8 Local_u8Result;


    /*======================================================
     * SEND HEADER
     *======================================================*/

    UART_VoidSendByte(R307_HEADER_HIGH);
    UART_VoidSendByte(R307_HEADER_LOW);


    /*======================================================
     * SEND ADDRESS
     *======================================================*/

    UART_VoidSendByte(R307_ADDRESS_BYTE_1);
    UART_VoidSendByte(R307_ADDRESS_BYTE_2);
    UART_VoidSendByte(R307_ADDRESS_BYTE_3);
    UART_VoidSendByte(R307_ADDRESS_BYTE_4);


    /*======================================================
     * PACKET TYPE = COMMAND
     *======================================================*/

    UART_VoidSendByte(R307_COMMAND_PACKET);


    /*======================================================
     * PACKET LENGTH = 3
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x03);


    /*======================================================
     * INSTRUCTION = REGMODEL
     *======================================================*/

    UART_VoidSendByte(R307_CMD_REGMODEL);


    /*======================================================
     * CHECKSUM
     *
     * 01 + 03 + 05 = 09
     *
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x09);


    /*======================================================
     * RECEIVE RESPONSE
     *======================================================*/

    Local_u8Result =
            R307_ReceiveAck(Local_u8Response);


    /*======================================================
     * COMMUNICATION ERROR
     *======================================================*/

    if(Local_u8Result != STD_TYPES_OK)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * RETURN RESULT
     *======================================================*/

    return Local_u8Response[9];
}


/*==========================================================
 * STORE
 *
 * Buffer ID:
 * 1 or 2
 *
 * Page ID:
 * Fingerprint ID in database
 *
 *==========================================================*/

u8 R307_Store(
        u8 Copy_u8BufferID,
        u16 Copy_u16PageID)
{
    u8 Local_u8Response[R307_ACK_RESPONSE_SIZE];

    u8 Local_u8Result;

    u16 Local_u16Checksum;


    /*======================================================
     * CHECK BUFFER ID
     *======================================================*/

    if((Copy_u8BufferID != 1) &&
       (Copy_u8BufferID != 2))
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * SEND HEADER
     *======================================================*/

    UART_VoidSendByte(R307_HEADER_HIGH);
    UART_VoidSendByte(R307_HEADER_LOW);


    /*======================================================
     * SEND ADDRESS
     *======================================================*/

    UART_VoidSendByte(R307_ADDRESS_BYTE_1);
    UART_VoidSendByte(R307_ADDRESS_BYTE_2);
    UART_VoidSendByte(R307_ADDRESS_BYTE_3);
    UART_VoidSendByte(R307_ADDRESS_BYTE_4);


    /*======================================================
     * PACKET TYPE = COMMAND
     *======================================================*/

    UART_VoidSendByte(R307_COMMAND_PACKET);


    /*======================================================
     * PACKET LENGTH = 6
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x06);


    /*======================================================
     * INSTRUCTION = STORE
     *======================================================*/

    UART_VoidSendByte(R307_CMD_STORE);


    /*======================================================
     * BUFFER ID
     *======================================================*/

    UART_VoidSendByte(Copy_u8BufferID);


    /*======================================================
     * PAGE ID
     *======================================================*/

    UART_VoidSendByte(
            (u8)(Copy_u16PageID >> 8));

    UART_VoidSendByte(
            (u8)Copy_u16PageID);


    /*======================================================
     * CHECKSUM
     *
     * 01 + 06 + 06
     * + Buffer ID
     * + Page ID High
     * + Page ID Low
     *
     *======================================================*/

    Local_u16Checksum =
            0x01 +
            0x06 +
            0x06 +
            Copy_u8BufferID +
            (Copy_u16PageID >> 8) +
            (Copy_u16PageID & 0xFF);


    /*======================================================
     * SEND CHECKSUM
     *======================================================*/

    UART_VoidSendByte(
            (u8)(Local_u16Checksum >> 8));

    UART_VoidSendByte(
            (u8)Local_u16Checksum);


    /*======================================================
     * RECEIVE RESPONSE
     *======================================================*/

    Local_u8Result =
            R307_ReceiveAck(Local_u8Response);


    /*======================================================
     * COMMUNICATION ERROR
     *======================================================*/

    if(Local_u8Result != STD_TYPES_OK)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * RETURN RESULT
     *======================================================*/

    return Local_u8Response[9];
}


/*==========================================================
 * DELETE
 *
 * Deletes one fingerprint ID.
 *
 * Command:
 *
 * EF 01
 * FF FF FF FF
 * 01
 * 00 07
 * 0C
 * ID High
 * ID Low
 * 00 01
 * Checksum High
 * Checksum Low
 *
 *==========================================================*/

u8 R307_Delete(u16 Copy_u16PageID)
{
    u8 Local_u8Response[R307_ACK_RESPONSE_SIZE];

    u8 Local_u8Result;

    u16 Local_u16Checksum;


    /*======================================================
     * SEND HEADER
     *======================================================*/

    UART_VoidSendByte(R307_HEADER_HIGH);
    UART_VoidSendByte(R307_HEADER_LOW);


    /*======================================================
     * SEND ADDRESS
     *======================================================*/

    UART_VoidSendByte(R307_ADDRESS_BYTE_1);
    UART_VoidSendByte(R307_ADDRESS_BYTE_2);
    UART_VoidSendByte(R307_ADDRESS_BYTE_3);
    UART_VoidSendByte(R307_ADDRESS_BYTE_4);


    /*======================================================
     * PACKET TYPE = COMMAND
     *======================================================*/

    UART_VoidSendByte(R307_COMMAND_PACKET);


    /*======================================================
     * PACKET LENGTH = 7
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x07);


    /*======================================================
     * INSTRUCTION = DELETE
     *======================================================*/

    UART_VoidSendByte(R307_CMD_DELETE);


    /*======================================================
     * FINGERPRINT ID
     *======================================================*/

    UART_VoidSendByte(
            (u8)(Copy_u16PageID >> 8));

    UART_VoidSendByte(
            (u8)Copy_u16PageID);


    /*======================================================
     * NUMBER OF TEMPLATES = 1
     *======================================================*/

    UART_VoidSendByte(0x00);
    UART_VoidSendByte(0x01);


    /*======================================================
     * CHECKSUM
     *
     * 01 + 07 + 0C
     * + ID High
     * + ID Low
     * + 00 + 01
     *
     *======================================================*/

    Local_u16Checksum =
            0x01 +
            0x07 +
            0x0C +
            (Copy_u16PageID >> 8) +
            (Copy_u16PageID & 0xFF) +
            0x00 +
            0x01;


    /*======================================================
     * SEND CHECKSUM
     *======================================================*/

    UART_VoidSendByte(
            (u8)(Local_u16Checksum >> 8));

    UART_VoidSendByte(
            (u8)Local_u16Checksum);


    /*======================================================
     * RECEIVE RESPONSE
     *======================================================*/

    Local_u8Result =
            R307_ReceiveAck(Local_u8Response);


    /*======================================================
     * COMMUNICATION ERROR
     *======================================================*/

    if(Local_u8Result != STD_TYPES_OK)
    {
        return R307_COMMUNICATION_ERROR;
    }


    /*======================================================
     * RETURN RESULT
     *======================================================*/

    return Local_u8Response[9];
}


/*==========================================================
 * COMPLETE ENROLLMENT
 *
 * Copy_u16PageID = ID where fingerprint will be stored.
 *
 * Sequence:
 *
 * 1. Place finger
 * 2. GenImg
 * 3. Image2Tz Buffer 1
 * 4. Remove finger
 * 5. Place same finger again
 * 6. GenImg
 * 7. Image2Tz Buffer 2
 * 8. RegModel
 * 9. Store
 *
 *==========================================================*/

u8 R307_Enroll(u16 Copy_u16PageID)
{
    u8 Local_u8Result;


    /*======================================================
     * FIRST FINGER SCAN
     *======================================================*/

    do
    {
        Local_u8Result =
                R307_GenImg();

    }while(Local_u8Result == R307_NO_FINGER);


    /*======================================================
     * CHECK FIRST SCAN
     *======================================================*/

    if(Local_u8Result != R307_CONFIRM_OK)
    {
        return Local_u8Result;
    }


    /*======================================================
     * FIRST IMAGE -> BUFFER 1
     *======================================================*/

    Local_u8Result =
            R307_Image2Tz(1);


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        return Local_u8Result;
    }


    /*======================================================
     * WAIT FOR FINGER REMOVAL
     *======================================================*/

    do
    {
        Local_u8Result =
                R307_GenImg();

    }while(Local_u8Result == R307_CONFIRM_OK);


    /*======================================================
     * EXPECT NO FINGER
     *======================================================*/

    if(Local_u8Result != R307_NO_FINGER)
    {
        return Local_u8Result;
    }


    /*======================================================
     * SECOND FINGER SCAN
     *======================================================*/

    do
    {
        Local_u8Result =
                R307_GenImg();

    }while(Local_u8Result == R307_NO_FINGER);


    /*======================================================
     * CHECK SECOND SCAN
     *======================================================*/

    if(Local_u8Result != R307_CONFIRM_OK)
    {
        return Local_u8Result;
    }


    /*======================================================
     * SECOND IMAGE -> BUFFER 2
     *======================================================*/

    Local_u8Result =
            R307_Image2Tz(2);


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        return Local_u8Result;
    }


    /*======================================================
     * CREATE MODEL
     *======================================================*/

    Local_u8Result =
            R307_RegModel();


    if(Local_u8Result != R307_CONFIRM_OK)
    {
        return Local_u8Result;
    }


    /*======================================================
     * STORE MODEL
     *======================================================*/

    Local_u8Result =
            R307_Store(
                    1,
                    Copy_u16PageID);


    return Local_u8Result;
}
