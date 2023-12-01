#pragma once

#include "PCCommunication_Frame.h"

#define TRANSMITE_QUEUE_SIZE 20

bool PCCommTransmiter_Init(PCCommLowLevel_Send_t SendCallback);
bool PCCommTransmiter_Transmite(const PCComm_Frame_s *Frame);
