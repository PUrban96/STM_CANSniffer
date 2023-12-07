#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct _CANLowLevel_Frame_s
{
    uint32_t CAN_LL_ID;
    uint8_t CAN_LL_DLC;
    uint8_t CAN_LL_DATA[8];
} CANLowLevel_Frame_s;

typedef enum _CANLowLevel_BaudRate_e
{
    BAUDRATE_50KBIT,
    BAUDRATE_100KBIT,
    BAUDRATE_125KBIT,
    BAUDRATE_250KBIT,
    BAUDRATE_500KBIT,
    BAUDRATE_1000KBIT,
} CANLowLevel_BaudRate_e;

typedef enum _CANLowLevel_Error_e
{
    BIT_ERROR = -1,
    STUFF_ERROR = -2,
    CRC_ERROR = -3,
    FORM_ERROR = -4,
    ACK_ERROR = -5,
    ERROR_ACTIVE_STATE = -6,
    ERROR_PASIVE_STATE = -7,
    BUS_OFF_STATE = -8,
} CANLowLevel_Error_e;

typedef void (*CANLowLevel_Receive_Callback_t)(CANLowLevel_Frame_s);
typedef void (*CANLowLevel_Error_Callback_t)(CANLowLevel_Error_e);

typedef bool (*CANLowLevel_Init_t)(void);
typedef bool (*CANLowLevel_Start_t)(void);
typedef bool (*CANLowLevel_Stop_t)(void);
typedef bool (*CANLowLevel_ChangeBaudrate_t)(CANLowLevel_BaudRate_e);
typedef bool (*CANLowLevel_Transmit_t)(CANLowLevel_Frame_s);

typedef void (*CANLowLevel_ReceiveSubscribe_t)(CANLowLevel_Receive_Callback_t);
typedef void (*CANLowLevel_ErrorSubscribe_t)(CANLowLevel_Error_Callback_t);

typedef struct _ICANLowLevel
{
    CANLowLevel_Init_t init;
    CANLowLevel_Start_t start;
    CANLowLevel_Stop_t stop;
    CANLowLevel_ChangeBaudrate_t changebaudrate;
    CANLowLevel_Transmit_t transmit;
    CANLowLevel_ReceiveSubscribe_t receivesubscribe;
    CANLowLevel_ErrorSubscribe_t errorsubscribe;
} ICANLowLevel;

ICANLowLevel *ICANLowLevel_Init();