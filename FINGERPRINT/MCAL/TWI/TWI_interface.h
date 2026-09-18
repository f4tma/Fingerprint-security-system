#ifndef MCAL_TWI_TWI_INTERFACE_H_
#define MCAL_TWI_TWI_INTERFACE_H_

#include "../../STD_TYPES.h"


/*=========================================================
 * TWI ERROR STATUS
 *=========================================================*/

typedef enum
{
    NoError = 0,

    StartConditionError,

    RepeatedStartError,

    SlaveAddressWithWriteError,

    SlaveAddressWithReadError,

    MasterWriteByteWithAckError,

    MasterReadByteWithAckError,

    SlaveReadByteError

} TWI_ErrorStatus_t;


/*=========================================================
 * INITIALIZATION
 *=========================================================*/

void TWI_voidInitMaster(u8 Copy_u8Address);

void TWI_voidInitSlave(u8 Copy_u8Address);


/*=========================================================
 * START
 *=========================================================*/

TWI_ErrorStatus_t
TWI_ErrorStatusSendStartConditionWithAck(void);


/*=========================================================
 * REPEATED START
 *=========================================================*/

TWI_ErrorStatus_t
TWI_ErrorStatusSendRepeatedStartConditionWithAck(void);


/*=========================================================
 * SLAVE ADDRESS + WRITE
 *=========================================================*/

TWI_ErrorStatus_t
TWI_ErrorStatusSendSlaveAddressWithWriteAck(
        u8 Copy_u8SlaveAddress
);


/*=========================================================
 * SLAVE ADDRESS + READ
 *=========================================================*/

TWI_ErrorStatus_t
TWI_ErrorStatusSendSlaveAddressWithReadAck(
        u8 Copy_u8SlaveAddress
);


/*=========================================================
 * MASTER WRITE
 *=========================================================*/

TWI_ErrorStatus_t
TWI_ErrorStatusMasterWriteDataByteWithAck(
        u8 Copy_u8DataByte
);


/*=========================================================
 * MASTER READ
 *=========================================================*/

TWI_ErrorStatus_t
TWI_ErrorStatusMasterReadDataByteWithACK(
        u8* Copy_pu8ReceiveByte
);


/*=========================================================
 * SLAVE READ
 *=========================================================*/

TWI_ErrorStatus_t
TWI_ErrorStatusSlaveReadDataByteWithACK(
        u8* Copy_pu8ReceiveByte
);


/*=========================================================
 * STOP
 *=========================================================*/

void TWI_voidSendStopCondition(void);


#endif /* MCAL_TWI_TWI_INTERFACE_H_ */
