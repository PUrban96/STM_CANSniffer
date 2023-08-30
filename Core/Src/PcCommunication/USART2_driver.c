#include "main.h"
#include "USART2_driver.h"
#include "led_blink.h"

static char SourceArray[] = {"To jest tekst do przekopiowania"};
static volatile char DestinationArray[50] = {0};

volatile uint32_t TimerValue[20] = {0};

volatile uint32_t ReceiveBuffer = 0;
volatile uint32_t TransmiteBuffer = 'a';

void DMA_Coppy(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // enable DMA2 clock (only the DMA2 controller is able to perform memory-to-memory transfers)
    __DSB();
    DMA2_Stream1->PAR = (uint32_t)SourceArray;
    DMA2_Stream1->M0AR = (uint32_t)DestinationArray;
    DMA2_Stream1->NDTR = sizeof(SourceArray);
    DMA2_Stream1->CR |= DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_DIR_1 | DMA_SxCR_TCIE;
    DMA2->LIFCR = DMA_LIFCR_CTCIF1;
    NVIC_EnableIRQ(DMA2_Stream1_IRQn);

    DMA2_Stream1->CR |= DMA_SxCR_EN;
}

__attribute__((interrupt)) void DMA2_Stream1_IRQHandler(void)
{
    if ((DMA2->LISR & DMA_LISR_TCIF1))
    {
        DMA2->LIFCR = DMA_LIFCR_CTCIF1;
        led_toggle();
        // DMA2_Stream1->CR |= DMA_SxCR_EN;
    }
}

void DMA_Timer(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 16000 - 1;
    TIM2->ARR = 0xFFFF;
    TIM2->EGR |= TIM_EGR_UG;

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 16000 - 1;
    TIM3->ARR = 250 - 1;
    TIM3->EGR |= TIM_EGR_UG;
    TIM3->DIER |= TIM_DIER_UDE;

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // enable DMA1 clock
    __DSB();
    DMA1_Stream2->PAR = (uint32_t)&TIM2->CNT;
    DMA1_Stream2->M0AR = (uint32_t)TimerValue;
    DMA1_Stream2->NDTR = (uint32_t)4;
    DMA1_Stream2->CR |= DMA_SxCR_MINC | DMA_SxCR_PSIZE_1 | DMA_SxCR_MSIZE_1 | DMA_SxCR_TCIE | DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_0;

    DMA1->LIFCR = DMA_LIFCR_CTCIF2;
    NVIC_EnableIRQ(DMA1_Stream2_IRQn);

    DMA1_Stream2->CR |= DMA_SxCR_EN;

    TIM2->CR1 |= TIM_CR1_CEN;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void DMA1_Stream2_IRQHandler(void)
{
    if ((DMA1->LISR & DMA_LISR_TCIF2))
    {
        DMA1->LIFCR = DMA_LIFCR_CTCIF2;
        led_toggle();
    }
}

void USART2_MirrorIRQ_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA clock enable
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2 clock enable

    /* Alternatife function pin correct config sequence (STM32F4):
        1. GPIO_MODER
        2. GPIO_OTYPER etc.
        3. GPIO_AFRL/H
    */

    /* USART2:
        PA3 - RxD
        PA2 - TxD
    */
    GPIOA->MODER |= GPIO_MODER_MODE3_1 | GPIO_MODER_MODE2_1;                                                                     // PA3, PA2 alternate function mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1 | GPIO_OSPEEDER_OSPEEDR3_0 | GPIO_OSPEEDER_OSPEEDR2_1 | GPIO_OSPEEDER_OSPEEDR2_0; // PA3, PA2 high speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_1;                                                                                          // PA3 - pull down
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0;                                                  // PA3 alternate function -> AF7 (USART2_RX)
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;                                                  // PA2 alternate function -> AF7 (USART2_TX)

    USART2->BRR = 16000000 / 256000;                                              // Set USART2 baudrate to 9600
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE; // Enable USART2

    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_RXNE) // Check if USART2 Read data register not empty
    {
        USART2->SR &= ~USART_SR_RXNE;
        uint32_t tmp_char = USART2->DR;
        USART2->DR = tmp_char;
    }
}

void USART2_MirrorDMA_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA clock enable
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2 clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;   // DMA1 clock enable

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
    GPIOA->MODER |= GPIO_MODER_MODE3_1 | GPIO_MODER_MODE2_1;                                                                     // PA3, PA2 alternate function mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1 | GPIO_OSPEEDER_OSPEEDR3_0 | GPIO_OSPEEDER_OSPEEDR2_1 | GPIO_OSPEEDER_OSPEEDR2_0; // PA3, PA2 high speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_1;                                                                                          // PA3 - pull down
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0;                                                  // PA3 alternate function -> AF7 (USART2_RX)
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;                                                  // PA2 alternate function -> AF7 (USART2_TX)

    // Receive DMA
    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)&ReceiveBuffer;
    DMA1_Stream5->NDTR = (uint32_t)1;
    DMA1_Stream5->CR |= DMA_SxCR_MINC | DMA_SxCR_PSIZE_1 | DMA_SxCR_MSIZE_1 | DMA_SxCR_CHSEL_2 | DMA_SxCR_CIRC; // Channel 4

    // Transmite DMA
    DMA1_Stream6->PAR = (uint32_t)&TransmiteBuffer;
    DMA1_Stream6->M0AR = (uint32_t)&USART2->DR;
    DMA1_Stream6->NDTR = (uint32_t)1;
    DMA1_Stream6->CR |= DMA_SxCR_MINC | DMA_SxCR_PSIZE_1 | DMA_SxCR_MSIZE_1 | DMA_SxCR_CHSEL_2 | DMA_SxCR_CIRC | DMA_SxCR_DIR_0; // Channel 4

    USART2->BRR = 16000000 / 9600;                                                // Set USART2 baudrate to 9600
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE; // Enable USART2
    USART2->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT;

    DMA1_Stream5->CR |= DMA_SxCR_EN;
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}
