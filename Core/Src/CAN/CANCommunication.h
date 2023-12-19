#pragma once

#include <stdint.h>

typedef enum _CAN_Type_e
{
    CAN_STD = 0,
    CAN_EXT = 1,
} CAN_Type_e;

typedef enum _CAN_RTR_e
{
    CAN_RTR_DATA = 0,
    CAN_RTR_REMOTE = 1,
} CAN_RTR_e;

typedef struct _CANCommunication_Frame_s
{
    uint32_t CAN_ID;
    CAN_Type_e CAN_TYPE;
    CAN_RTR_e CAN_RTR;
    uint8_t CAN_DLC;
    uint8_t CAN_DATA[8];
} CANCommunication_Frame_s;

void CANCommunication_Init(void);
void CANCommunication_Transmit(CANCommunication_Frame_s Frame);

typedef void (*CANCommunication_Receive_Callback_t)(CANCommunication_Frame_s);
void CANCommunication_SubscribeReceiver(CANCommunication_Receive_Callback_t Callback);