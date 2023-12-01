#include "main.h"
#include "IPCCommunication_LowLevel.h"
#include "PCCommunication_Transmiter.h"

#include "OSAL.h"

#include <string.h>

static osal_task_t Transmite_TaskHandler = {0};
static osal_queue_t TransmiteBuffer_QueueHandler = {0};

static void vTaskPCCommTransmiteFrame(void *pvParameters);
static void PCCommTransmiter_FrameToMessage(const PCComm_Frame_s *Frame, uint8_t *Message);
static PCCommLowLevel_Send_t TransmiterCallback = NULL;

bool PCCommTransmiter_Init(PCCommLowLevel_Send_t SendCallback)
{
    TransmiterCallback = SendCallback;
    TransmiteBuffer_QueueHandler = osal_queue_create(TRANSMITE_QUEUE_SIZE, sizeof(PCComm_Frame_s));
    Transmite_TaskHandler = osal_task_create(vTaskPCCommTransmiteFrame, "PCComm_Transmite", 400, NULL, 7);
    if (TransmiteBuffer_QueueHandler != NULL && Transmite_TaskHandler != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PCCommTransmiter_Transmite(const PCComm_Frame_s *Frame)
{
    bool status = osal_queue_send(TransmiteBuffer_QueueHandler, Frame, 0, false);
    return status;
}

static void vTaskPCCommTransmiteFrame(void *pvParameters)
{
    (void)pvParameters;
    PCComm_Frame_s CurrentFrame = {0};
    uint8_t CurrentMessage[PCCOMM_FRAME_BYTE_AMOUNT] = {0};

    for (;;)
    {
        bool status = osal_queue_receive(TransmiteBuffer_QueueHandler, &CurrentFrame, 1000, false);
        if (status == true)
        {
            PCCommTransmiter_FrameToMessage(&CurrentFrame, CurrentMessage);
            TransmiterCallback(CurrentMessage, PCCOMM_FRAME_BYTE_AMOUNT);
        }
    }
}

static void PCCommTransmiter_FrameToMessage(const PCComm_Frame_s *Frame, uint8_t *Message)
{
    Message[PCCOMM_FRAME_START_START_BYTE] = PCCOMM_FRAME_START_SIGN;
    Message[PCCOMM_FRAME_TYPE_START_BYTE] = Frame->type;
    Message[PCCOMM_FRAME_CMD_START_BYTE] = Frame->command;
    memcpy(Message + PCCOMM_FRAME_DATA_START_BYTE, Frame->data, (PCCOMM_FRAME_DATA_BYTE_LENGHT * sizeof(uint8_t)));
    Message[PCCOMM_FRAME_CHECKSUM_START_BYTE] = (uint8_t)(((Frame->checksum) & 0xFF00) >> 8);
    Message[PCCOMM_FRAME_CHECKSUM_START_BYTE + 1] = (uint8_t)((Frame->checksum) & 0x00FF);
    Message[PCCOMM_FRAME_END_START_BYTE] = PCCOMM_FRAME_END_SIGN;
}
