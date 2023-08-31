#include "main.h"
#include "IPCCommunication_LowLevel.h"
#include "USART2.h"

static IPCCommLowLevel PCCommunication;

IPCCommLowLevel *IPCCommLowLevel_Init()
{
	PCCommunication.init = USART2_init;
	PCCommunication.start = USART2_start;
	PCCommunication.stop = USART2_stop;
	PCCommunication.send = USART2_send;

	return &PCCommunication;
}