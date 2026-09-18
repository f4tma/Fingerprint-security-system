#ifndef MCAL_TWI_TWI_PRIVATE_H_
#define MCAL_TWI_TWI_PRIVATE_H_

#include "../../STD_TYPES.h"


/*=========================================================
 * TWI STATUS CODES
 *=========================================================*/

#define START_ACK                   0x08
#define REP_START_ACK               0x10

#define SLAVE_ADD_AND_WR_ACK        0x18
#define SLAVE_ADD_AND_RD_ACK        0x40

#define MSTER_WR_BYTE_WITH_ACK      0x28
#define MSTER_RD_BYTE_WITH_ACK      0x50

#define SLAVE_ADD_RCVD_RD_REQ       0xA8
#define SLAVE_ADD_RCVD_WR_REQ       0x60

#define SLAVE_DATA_RECEIVED         0x80
#define SLAVE_DATA_TRANSMITED       0xB8

#define STATUS_BIT_MASK             0xF8


/*=========================================================
 * TWI REGISTERS
 *=========================================================*/

#define TWI_u8_TWBR_REG             *((volatile u8*)0x20)
#define TWI_u8_TWSR_REG             *((volatile u8*)0x21)
#define TWI_u8_TWAR_REG             *((volatile u8*)0x22)
#define TWI_u8_TWDR_REG             *((volatile u8*)0x23)
#define TWI_u8_TWCR_REG             *((volatile u8*)0x56)


/*=========================================================
 * TWSR BITS
 *=========================================================*/

#define TWI_u8_TWSR_TWPS0           0
#define TWI_u8_TWSR_TWPS1           1


/*=========================================================
 * TWAR BITS
 *=========================================================*/

#define TWI_u8_TWAR_TWGCE           0


/*=========================================================
 * TWCR BITS
 *=========================================================*/

#define TWI_u8_TWCR_TWIE            0
#define TWI_u8_TWCR_TWIEN           2
#define TWI_u8_TWCR_TWWC            3
#define TWI_u8_TWCR_TWSTO           4
#define TWI_u8_TWCR_TWSTA           5
#define TWI_u8_TWCR_TWEA            6
#define TWI_u8_TWCR_TWINT           7


#endif /* MCAL_TWI_TWI_PRIVATE_H_ */
