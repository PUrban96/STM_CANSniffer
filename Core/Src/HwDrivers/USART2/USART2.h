#pragma once

#include "IPCCommunication_LowLevel.h"

#define USART2_BAUDRATE 9600

void USART2_init(void);
void USART2_DMA_TC_Subscribe(PCCommLowLevel_NewFrame_Callback_t callback);
void USART2_send(const uint8_t *message, uint16_t lenght);
void USART2_receive(uint8_t *message, uint16_t lenght);
