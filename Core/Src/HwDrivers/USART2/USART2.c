#include "main.h"
#include "USART2.h"

#include "IRCC_clock.h"
#include <string.h>

/* Alternatife function pin correct config sequence (STM32F4):
    1. GPIO_MODER
    2. GPIO_OTYPER etc.
    3. GPIO_AFRL/H
*/

/* DMA:
    USART2_RX -> DMA1 Stream5 Channel4
    USART2_TX -> DMA1 Stream6 Channel4
*/

/* USART2:
    PA3 - RxD
    PA2 - TxD
*/

void USART2_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA clock enable
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2 clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;   // DMA1 clock enable
    __DSB();

    GPIOA->MODER |= GPIO_MODER_MODE3_1 | GPIO_MODER_MODE2_1;                                                                     // PA3, PA2 alternate function mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1 | GPIO_OSPEEDER_OSPEEDR3_0 | GPIO_OSPEEDER_OSPEEDR2_1 | GPIO_OSPEEDER_OSPEEDR2_0; // PA3, PA2 high speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_1;                                                                                          // PA3 - pull down
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0;                                                  // PA3 alternate function -> AF7 (USART2_RX)
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;                                                  // PA2 alternate function -> AF7 (USART2_TX)

    USART2->BRR = RCCClock_Get_RCC_APB1_Frequency() / USART2_BAUDRATE; // Set USART2 baudrate to 9600
    USART2->CR3 = USART_CR3_DMAT;
    USART2->SR &= ~(USART_SR_TC);
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; // Enable USART2

    DMA1_Stream6->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_DIR_0 | DMA_SxCR_MINC | DMA_SxCR_TCIE;
    DMA1->LIFCR = DMA_HIFCR_CTCIF6;
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void USART2_start(void)
{
}

void USART2_stop(void)
{
}

void USART2_send(const char *message)
{
    while (!(USART2->SR & USART_SR_TC))
        ;
    USART2->SR &= ~(USART_SR_TC);
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream6->M0AR = (uint32_t)message;
    DMA1_Stream6->NDTR = (uint32_t)strlen(message);

    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void DMA1_Stream6_IRQHandler(void)
{
    if ((DMA1->HISR & DMA_HISR_TCIF6))
    {
        DMA1->HIFCR = DMA_HIFCR_CTCIF6;
    }
}
