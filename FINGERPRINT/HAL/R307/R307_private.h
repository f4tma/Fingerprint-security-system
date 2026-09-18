#ifndef HAL_R307_R307_PRIVATE_H_
#define HAL_R307_R307_PRIVATE_H_

/*==========================================================
 * R307 PACKET TYPES
 *==========================================================*/

#define R307_COMMAND_PACKET      0x01
#define R307_DATA_PACKET         0x02
#define R307_ACK_PACKET          0x07
#define R307_END_PACKET          0x08


/*==========================================================
 * R307 PACKET HEADER
 *==========================================================*/

#define R307_HEADER_HIGH         0xEF
#define R307_HEADER_LOW          0x01


/*==========================================================
 * R307 INSTRUCTIONS
 *==========================================================*/

#define R307_CMD_GENIMG          0x01
#define R307_CMD_IMAGE2TZ        0x02
#define R307_CMD_SEARCH          0x04
#define R307_CMD_REGMODEL        0x05
#define R307_CMD_STORE           0x06
#define R307_CMD_DELETE          0x0C


/*==========================================================
 * R307 RESPONSE SIZES
 *==========================================================*/

#define R307_ACK_RESPONSE_SIZE   12
#define R307_SEARCH_RESPONSE_SIZE 16

#endif /* HAL_R307_R307_PRIVATE_H_ */
