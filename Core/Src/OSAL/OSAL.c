#include <stdint.h>
#include <stdbool.h>
#include "OSAL.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

osal_task_t osal_task_create(osal_task_fun_t task, const char *taskName, const uint32_t stackSize, void *const taskParameters, uint32_t priority)
{
    TaskHandle_t hTask = {0};

    xTaskCreate(task, taskName, stackSize, taskParameters, priority / 4, &hTask);
    return (osal_task_t)hTask;
}

void osal_task_delete(osal_task_fun_t task)
{
    TaskHandle_t hTask = (TaskHandle_t)task;
    vTaskDelete(hTask);
}

osal_queue_t osal_queue_create(uint32_t queueElementsAmount, uint32_t queueElementSize)
{
    QueueHandle_t hQueue = {0};
    hQueue = xQueueCreate(queueElementsAmount, queueElementSize);
    return (osal_queue_t)hQueue;
}

void osal_queue_delete(osal_queue_t queue)
{
    QueueHandle_t hQueue = (QueueHandle_t)queue;
    vQueueDelete(hQueue);
}

bool osal_queue_send(osal_queue_t queue, const void *data, uint32_t timeout_ms, bool isIRQ)
{
    QueueHandle_t hQueue = (QueueHandle_t)queue;
    if (!isIRQ)
    {
        if (xQueueSend(hQueue, data, pdMS_TO_TICKS(timeout_ms)) == pdTRUE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    else
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xQueueSendFromISR(hQueue, data, &xHigherPriorityTaskWoken) == pdTRUE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

bool osal_queue_receive(osal_queue_t queue, void *data, uint32_t timeout_ms, bool isIRQ)
{
    QueueHandle_t hQueue = (QueueHandle_t)queue;
    if (!isIRQ)
    {
        if (xQueueReceive(hQueue, data, pdMS_TO_TICKS(timeout_ms)) == pdTRUE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xQueueReceiveFromISR(hQueue, data, &xHigherPriorityTaskWoken) == pdTRUE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
