#include "main.h"
#include "IPCCommunication_LowLevel.h"
#include "PCCommunication_Receiver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <string.h>

static TaskHandle_t Receiver_TaskHandler = {0};
static QueueHandle_t ReceiverBuffer_QueueHandler = {0};
static PCCommLowLevel_Receive_t ReceiveCallback = NULL;

static uint8_t ReceivedMessageBuffer[PCCOMM_FRAME_BYTE_AMOUNT] = {0};

static void vTaskPCCommReceiveFrame(void *pvParameters);
static bool CheckIfMessageIsCorrectFrame(const uint8_t *Message, uint32_t lenght);
static void MessageToFrame(PCComm_Frame_s *Frame, const uint8_t *Message);

bool PCCommReceiver_Init(PCCommLowLevel_Receive_t callback)
{
    ReceiveCallback = callback;
    ReceiverBuffer_QueueHandler = xQueueCreate(RECEIVE_QUEUE_SIZE, sizeof(PCComm_Frame_s));
    xTaskCreate(vTaskPCCommReceiveFrame, "PCComm_Receive", 400, NULL, 6, &Receiver_TaskHandler);
    if (ReceiverBuffer_QueueHandler != NULL && Receiver_TaskHandler != NULL && ReceiveCallback != NULL)
    {
        ReceiveCallback(ReceivedMessageBuffer, PCCOMM_FRAME_BYTE_AMOUNT);
        return true;
    }
    else
    {
        return false;
    }
}

// Maybe use task notyfication here and add frame to buffer in another task??
void PCCommReceiver_AddFrameToBuffer(uint32_t lenght)
{
    if (CheckIfMessageIsCorrectFrame(ReceivedMessageBuffer, lenght) == true)
    {
        PCComm_Frame_s FrameToAdd = {0};
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;;
        MessageToFrame(&FrameToAdd, ReceivedMessageBuffer);
        xQueueSendFromISR(ReceiverBuffer_QueueHandler, &FrameToAdd, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    ReceiveCallback(ReceivedMessageBuffer, PCCOMM_FRAME_BYTE_AMOUNT);
}

static void vTaskPCCommReceiveFrame(void *pvParameters)
{
    (void)pvParameters;
    PCComm_Frame_s CurrentFrame = {0};

    for (;;)
    {
        while (xQueueReceive(ReceiverBuffer_QueueHandler, &CurrentFrame, pdMS_TO_TICKS(1000)) == pdPASS)
        {
            __NOP();
        }
    }
}

// TODO: Add check checksum
static bool CheckIfMessageIsCorrectFrame(const uint8_t *Message, uint32_t lenght)
{
    if (lenght == PCCOMM_FRAME_BYTE_AMOUNT && Message[PCCOMM_FRAME_START_START_BYTE] == PCCOMM_FRAME_START_SIGN && Message[PCCOMM_FRAME_END_START_BYTE] == PCCOMM_FRAME_END_SIGN)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// TODO
static void MessageToFrame(PCComm_Frame_s *Frame, const uint8_t *Message)
{
    Frame->type = Message[PCCOMM_FRAME_TYPE_START_BYTE];
    Frame->command = Message[PCCOMM_FRAME_CMD_START_BYTE];
    memcpy(Frame->data, Message + PCCOMM_FRAME_DATA_START_BYTE, (PCCOMM_FRAME_DATA_BYTE_LENGHT * sizeof(uint8_t)));
    Frame->checksum = (uint16_t)Message[PCCOMM_FRAME_CHECKSUM_START_BYTE + 1] << 8 | Message[PCCOMM_FRAME_CHECKSUM_START_BYTE + 1];
}
