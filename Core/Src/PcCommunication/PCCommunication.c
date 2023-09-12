#include "main.h"
#include "PCCommunication.h"
#include "IPCCommunication_LowLevel.h"
#include "PCCommunication_Frame.h"
#include "PCCommunication_Transmiter.h"

#include <string.h>

static IPCCommLowLevel *PCCommunication_LowLevel;

void PCComm_Init(void)
{
    PCCommunication_LowLevel = IPCCommLowLevel_Init();
    PCCommunication_LowLevel->init();
    PCCommunication_LowLevel->start();
    PCPCCommTransmiter_Init(PCCommunication_LowLevel->send);
}

void PCComm_SendCanFrame(uint32_t CAN_ID, uint8_t CAN_DLC, const uint8_t Data[8])
{
    PCComm_Frame_s NewFrame = {0};

    NewFrame.type = PCCOMM_TYPE_DATA;
    NewFrame.command = PCCOMM_TYPE_CANFRAME;
    NewFrame.data[0] = CAN_ID;
    NewFrame.data[1] = CAN_ID << 8;
    NewFrame.data[2] = CAN_ID << 16;
    NewFrame.data[3] = CAN_ID << 24;
    NewFrame.data[4] = CAN_DLC;
    memcpy(NewFrame.data + 5, Data, 8);
    // CalculateCRC;
    NewFrame.checksum = 0xFFFF;

    PCCommTransmiter_Transmite(&NewFrame);
}