#include "main.h"
#include "IPCCommunication_LowLevel.h"
#include "PCCommunication_Transmiter.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static TaskHandle_t Transmite_TaskHandler = {0};
static QueueHandle_t TransmiteBuffer_QueueHandler = {0};

static void vTaskPCCommTransmiteFrame(void *pvParameters);
static PCCommLowLevel_Send_t sender = NULL;

bool PCPCCommTransmiter_Init(PCCommLowLevel_Send_t SendCallback)
{
    sender = SendCallback;
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

    for (;;)
    {
        BaseType_t status = xQueueReceive(TransmiteBuffer_QueueHandler, &CurrentFrame, pdMS_TO_TICKS(1000));
        if (status == pdPASS)
        {
            // Build transmite message
            // Send
            
        }
    }
}
