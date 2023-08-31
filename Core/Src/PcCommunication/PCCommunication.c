#include "main.h"
#include "PCCommunication.h"
#include "IPCCommunication_LowLevel.h"

static IPCCommLowLevel *PCCommunication_LowLevel;

void PCComm_Init(void)
{
    PCCommunication_LowLevel = IPCCommLowLevel_Init();
    PCCommunication_LowLevel->init();
    PCCommunication_LowLevel->start();
}

void PCComm_SendString(uint32_t *message)
{
    PCCommunication_LowLevel->send(message);
}