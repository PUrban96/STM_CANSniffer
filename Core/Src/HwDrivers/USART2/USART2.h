#pragma once

#define USART2_BAUDRATE 9600

void USART2_init(void);
void USART2_start(void);
void USART2_stop(void);
void USART2_send(const char* message);