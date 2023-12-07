#include "main.h"
#include "ICAN_LowLevel.h"
#include "CAN1.h"

static ICANLowLevel CANLowLevel;

ICANLowLevel *ICANLowLevel_Init()
{
    CANLowLevel.init = CAN1_init;
    CANLowLevel.start = CAN1_start;
    CANLowLevel.stop = CAN1_stop;
    CANLowLevel.changebaudrate = CAN1_changeBaudRate;
    CANLowLevel.transmit = CAN1_transmite;
    CANLowLevel.receivesubscribe = CAN1_subscribeReceivedIRQ;
    CANLowLevel.errorsubscribe = CAN1_subscribeErrorIRQ;
    return &CANLowLevel;
}