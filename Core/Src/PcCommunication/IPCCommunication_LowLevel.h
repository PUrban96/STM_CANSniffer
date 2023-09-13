#pragma once

typedef void (*PCCommLowLevel_Callback_t)(void);

typedef void (*PCCommLowLevel_Init_t)(void);
typedef void (*PCCommLowLevel_Start_t)(void);
typedef void (*PCCommLowLevel_Stop_t)(void);
typedef void (*PCCommLowLevel_Send_t)(const uint8_t *, uint16_t lenght);
typedef void (*PCCommLowLevel_Receive_t)(uint8_t *, uint16_t lenght);
//typedef void (*PCCommLowLevel_Subscribe_t)(PCCommLowLevel_Callback_t callback);

typedef struct IPCCommLowLevel
{
	PCCommLowLevel_Init_t init;
	PCCommLowLevel_Start_t start;
	PCCommLowLevel_Stop_t stop;
    PCCommLowLevel_Send_t send;
	PCCommLowLevel_Receive_t receive;
	//PCCommLowLevel_Subscribe_t subscribe;
}IPCCommLowLevel;


IPCCommLowLevel *IPCCommLowLevel_Init();