#include "main.h"
#include "IPCCommunication_LowLevel.h"
#include "USART2.h"

static IPCCommLowLevel PCCommunication;

IPCCommLowLevel *IPCCommLowLevel_Init()
{
	PCCommunication.init = USART2_init;
	PCCommunication.send = USART2_send;
	PCCommunication.receive = USART2_receive;
	PCCommunication.subscribe = USART2_DMA_TC_Subscribe;

	return &PCCommunication;
}