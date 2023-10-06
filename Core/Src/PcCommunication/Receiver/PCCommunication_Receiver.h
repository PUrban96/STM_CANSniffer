#pragma once

#include "PCCommunication_Frame.h"

#define RECEIVE_QUEUE_SIZE 20

bool PCCommReceiver_Init(PCCommLowLevel_Receive_t callback);
void PCCommReceiver_AddFrameToBuffer(uint32_t lenght);