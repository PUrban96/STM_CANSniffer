#pragma once

#include <stdbool.h>
#include "ICAN_LowLevel.h"

bool CAN1_init(void);
bool CAN1_start(void);
bool CAN1_stop(void);
bool CAN1_changeBaudRate(CANLowLevel_BaudRate_e Baudrate);
bool CAN1_transmite(CANLowLevel_Frame_s Frame);
void CAN1_subscribeReceivedIRQ(CANLowLevel_Receive_Callback_t ReceiveCallback);
void CAN1_subscribeErrorIRQ(CANLowLevel_Error_Callback_t ErrorCallback);