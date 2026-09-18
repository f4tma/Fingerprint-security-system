#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"

#include "TWI_private.h"
#include "TWI_interface.h"


void TWI_voidInitMaster(u8 Copy_u8Address)
{
    /* Prescaler = 1 */
    CLR_BIT(TWI_u8_TWSR_REG, TWI_u8_TWSR_TWPS0);
    CLR_BIT(TWI_u8_TWSR_REG, TWI_u8_TWSR_TWPS1);

    /* F_CPU = 8 MHz
     * SCL = 100 kHz
     */
    TWI_u8_TWBR_REG = 32;

    /* Enable TWI */
    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWIEN);

    /* Enable ACK */
    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEA);

    if (Copy_u8Address != 0)
    {
        TWI_u8_TWAR_REG = (Copy_u8Address << 1);
    }
}


void TWI_voidInitSlave(u8 Copy_u8Address)
{
    TWI_u8_TWAR_REG = (Copy_u8Address << 1);

    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWIEN);
    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEA);
}


TWI_ErrorStatus_t TWI_ErrorStatusSendStartConditionWithAck(void)
{
    TWI_ErrorStatus_t Local_ErrorStatus = NoError;

    TWI_u8_TWCR_REG =
            (1 << TWI_u8_TWCR_TWINT) |
            (1 << TWI_u8_TWCR_TWSTA) |
            (1 << TWI_u8_TWCR_TWIEN);

    while (GET_BIT(TWI_u8_TWCR_REG,
                   TWI_u8_TWCR_TWINT) == 0)
    {
    }

    if ((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != START_ACK)
    {
        Local_ErrorStatus = StartConditionError;
    }

    return Local_ErrorStatus;
}


TWI_ErrorStatus_t TWI_ErrorStatusSendRepeatedStartConditionWithAck(void)
{
    TWI_ErrorStatus_t Local_ErrorStatus = NoError;

    TWI_u8_TWCR_REG =
            (1 << TWI_u8_TWCR_TWINT) |
            (1 << TWI_u8_TWCR_TWSTA) |
            (1 << TWI_u8_TWCR_TWIEN);

    while (GET_BIT(TWI_u8_TWCR_REG,
                   TWI_u8_TWCR_TWINT) == 0)
    {
    }

    if ((TWI_u8_TWSR_REG & STATUS_BIT_MASK) != REP_START_ACK)
    {
        Local_ErrorStatus = RepeatedStartError;
    }

    return Local_ErrorStatus;
}


TWI_ErrorStatus_t
TWI_ErrorStatusSendSlaveAddressWithWriteAck(u8 Copy_u8SlaveAddress)
{
    TWI_ErrorStatus_t Local_ErrorStatus = NoError;

    TWI_u8_TWDR_REG = (Copy_u8SlaveAddress << 1);

    CLR_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWSTA);

    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

    while (GET_BIT(TWI_u8_TWCR_REG,
                   TWI_u8_TWCR_TWINT) == 0)
    {
    }

    if ((TWI_u8_TWSR_REG & STATUS_BIT_MASK)
            != SLAVE_ADD_AND_WR_ACK)
    {
        Local_ErrorStatus = SlaveAddressWithWriteError;
    }

    return Local_ErrorStatus;
}


TWI_ErrorStatus_t
TWI_ErrorStatusSendSlaveAddressWithReadAck(u8 Copy_u8SlaveAddress)
{
    TWI_ErrorStatus_t Local_ErrorStatus = NoError;

    TWI_u8_TWDR_REG =
            (Copy_u8SlaveAddress << 1) | 0x01;

    CLR_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWSTA);

    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

    while (GET_BIT(TWI_u8_TWCR_REG,
                   TWI_u8_TWCR_TWINT) == 0)
    {
    }

    if ((TWI_u8_TWSR_REG & STATUS_BIT_MASK)
            != SLAVE_ADD_AND_RD_ACK)
    {
        Local_ErrorStatus = SlaveAddressWithReadError;
    }

    return Local_ErrorStatus;
}


TWI_ErrorStatus_t
TWI_ErrorStatusMasterWriteDataByteWithAck(u8 Copy_u8DataByte)
{
    TWI_ErrorStatus_t Local_ErrorStatus = NoError;

    TWI_u8_TWDR_REG = Copy_u8DataByte;

    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

    while (GET_BIT(TWI_u8_TWCR_REG,
                   TWI_u8_TWCR_TWINT) == 0)
    {
    }

    if ((TWI_u8_TWSR_REG & STATUS_BIT_MASK)
            != MSTER_WR_BYTE_WITH_ACK)
    {
        Local_ErrorStatus = MasterWriteByteWithAckError;
    }

    return Local_ErrorStatus;
}


TWI_ErrorStatus_t
TWI_ErrorStatusMasterReadDataByteWithACK(
        u8 *Copy_pu8ReceiveByte)
{
    TWI_ErrorStatus_t Local_ErrorStatus = NoError;

    if (Copy_pu8ReceiveByte == 0)
    {
        return MasterReadByteWithAckError;
    }

    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEA);
    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

    while (GET_BIT(TWI_u8_TWCR_REG,
                   TWI_u8_TWCR_TWINT) == 0)
    {
    }

    if ((TWI_u8_TWSR_REG & STATUS_BIT_MASK)
            != MSTER_RD_BYTE_WITH_ACK)
    {
        Local_ErrorStatus = MasterReadByteWithAckError;
    }
    else
    {
        *Copy_pu8ReceiveByte = TWI_u8_TWDR_REG;
    }

    return Local_ErrorStatus;
}


TWI_ErrorStatus_t
TWI_ErrorStatusSlaveReadDataByteWithACK(
        u8 *Copy_pu8ReceiveByte)
{
    TWI_ErrorStatus_t Local_ErrorStatus = NoError;

    if (Copy_pu8ReceiveByte == 0)
    {
        return SlaveReadByteError;
    }

    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWEA);
    SET_BIT(TWI_u8_TWCR_REG, TWI_u8_TWCR_TWINT);

    while (GET_BIT(TWI_u8_TWCR_REG,
                   TWI_u8_TWCR_TWINT) == 0)
    {
    }

    if ((TWI_u8_TWSR_REG & STATUS_BIT_MASK)
            != SLAVE_DATA_RECEIVED)
    {
        Local_ErrorStatus = SlaveReadByteError;
    }
    else
    {
        *Copy_pu8ReceiveByte = TWI_u8_TWDR_REG;
    }

    return Local_ErrorStatus;
}


void TWI_voidSendStopCondition(void)
{
    TWI_u8_TWCR_REG =
            (1 << TWI_u8_TWCR_TWINT) |
            (1 << TWI_u8_TWCR_TWSTO) |
            (1 << TWI_u8_TWCR_TWIEN);
}
