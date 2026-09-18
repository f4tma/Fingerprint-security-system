#ifndef HAL_R307_R307_CONFIG_H_
#define HAL_R307_R307_CONFIG_H_

/*==========================================================
 * R307 DEFAULT ADDRESS
 *==========================================================*/

#define R307_ADDRESS_BYTE_1     0xFF
#define R307_ADDRESS_BYTE_2     0xFF
#define R307_ADDRESS_BYTE_3     0xFF
#define R307_ADDRESS_BYTE_4     0xFF

#define R307_DEFAULT_ADDRESS    0xFFFFFFFFUL


/*==========================================================
 * R307 DATABASE
 *==========================================================*/

#define R307_FIRST_ID            1
#define R307_NUMBER_OF_IDS       100


/*==========================================================
 * R307 STATUS CODES
 *==========================================================*/

#define R307_CONFIRM_OK          0x00
#define R307_NO_FINGER           0x02

/*
 * Returned by the driver when UART communication
 * with the R307 fails.
 */
#define R307_COMMUNICATION_ERROR 0xFE

#endif /* HAL_R307_R307_CONFIG_H_ */
