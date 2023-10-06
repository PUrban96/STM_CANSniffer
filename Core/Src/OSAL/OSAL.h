#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void (*osal_task_fun_t)(void *parameters);
typedef void *osal_task_t;
osal_task_t osal_task_create(osal_task_fun_t task, const char *taskName, const uint32_t stackSize, void *const taskParameters, uint32_t priority);

typedef void *osal_queue_t;
osal_queue_t osal_queue_create(uint32_t queueElementsAmount, uint32_t queueElementSize);
bool osal_queue_send(osal_queue_t queue, const void *data, uint32_t timeout_ms, bool isIRQ);
bool osal_queue_receive(osal_queue_t queue, void *data, uint32_t timeout_ms, bool isIRQ);