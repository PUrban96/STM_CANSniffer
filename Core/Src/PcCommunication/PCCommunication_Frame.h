#pragma once

#include "main.h"

#define PCCOMM_FRAME_START_BYTE 'r'
#define PCCOMM_FRAME_END_BYTE 'n'

typedef enum _PCComm_Type_e
{
    PCCOMM_TYPE_DATA,
    PCCOMM_TYPE_REQUEST,
    PCCOMM_TYPE_ACK
} PCComm_Type_e;

typedef enum _PCComm_Command_e
{
    PCCOMM_TYPE_CANFRAME,
} PCComm_Command_e;

typedef struct _PCComm_Frame_s
{
    PCComm_Type_e type;
    PCComm_Command_e command;
    uint16_t checksum;
    uint8_t data[13];
} PCComm_Frame_s;