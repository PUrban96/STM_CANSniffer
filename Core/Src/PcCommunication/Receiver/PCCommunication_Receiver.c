#include "main.h"
#include "IPCCommunication_LowLevel.h"
#include "PCCommunication_Receiver.h"

#include "OSAL.h"

#include <string.h>

static osal_task_t Receiver_TaskHandler = {0};
static osal_queue_t ReceiverBuffer_QueueHandler = {0};
static PCCommLowLevel_Receive_t ReceiveCallback = NULL;

static uint8_t ReceivedMessageBuffer[PCCOMM_FRAME_BYTE_AMOUNT] = {0};

static void vTaskPCCommReceiveFrame(void *pvParameters);
static bool CheckIfMessageIsCorrectFrame(const uint8_t *Message, uint32_t lenght);
static void MessageToFrame(PCComm_Frame_s *Frame, const uint8_t *Message);

bool PCCommReceiver_Init(PCCommLowLevel_Receive_t callback)
{
    ReceiveCallback = callback;
    ReceiverBuffer_QueueHandler = osal_queue_create(RECEIVE_QUEUE_SIZE, sizeof(PCComm_Frame_s));
    Receiver_TaskHandler = osal_task_create(vTaskPCCommReceiveFrame, "PCComm_Receive", 400, NULL, 6);
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
        MessageToFrame(&FrameToAdd, ReceivedMessageBuffer);
        osal_queue_send(ReceiverBuffer_QueueHandler, &FrameToAdd, 0, true);
    }
    ReceiveCallback(ReceivedMessageBuffer, PCCOMM_FRAME_BYTE_AMOUNT);
}

static void vTaskPCCommReceiveFrame(void *pvParameters)
{
    (void)pvParameters;
    PCComm_Frame_s CurrentFrame = {0};

    for (;;)
    {
        while (osal_queue_receive(ReceiverBuffer_QueueHandler, &CurrentFrame, 1000, false) == true)
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
