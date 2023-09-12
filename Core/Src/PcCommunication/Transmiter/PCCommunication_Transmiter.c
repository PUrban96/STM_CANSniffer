#include "main.h"
#include "IPCCommunication_LowLevel.h"
#include "PCCommunication_Transmiter.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <string.h>

static TaskHandle_t Transmite_TaskHandler = {0};
static QueueHandle_t TransmiteBuffer_QueueHandler = {0};

static void vTaskPCCommTransmiteFrame(void *pvParameters);
static void PCCommTransmiter_FrameToMessage(const PCComm_Frame_s *Frame, uint8_t *Message);
static PCCommLowLevel_Send_t TransmiterCallback = NULL;

bool PCPCCommTransmiter_Init(PCCommLowLevel_Send_t SendCallback)
{
    TransmiterCallback = SendCallback;
    TransmiteBuffer_QueueHandler = xQueueCreate(TRANSMITE_QUEUE_SIZE, sizeof(PCComm_Frame_s));
    xTaskCreate(vTaskPCCommTransmiteFrame, "PCComm_Transmite", 400, NULL, 7, &Transmite_TaskHandler);
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
    BaseType_t status = xQueueSend(TransmiteBuffer_QueueHandler, Frame, pdMS_TO_TICKS(0));
    if (status == pdPASS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static void vTaskPCCommTransmiteFrame(void *pvParameters)
{
    (void)pvParameters;
    PCComm_Frame_s CurrentFrame = {0};
    uint8_t CurrentMessage[PCCOMM_FRAME_BYTE_AMOUNT] = {0};

    for (;;)
    {
        BaseType_t status = xQueueReceive(TransmiteBuffer_QueueHandler, &CurrentFrame, pdMS_TO_TICKS(1000));
        if (status == pdPASS)
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
