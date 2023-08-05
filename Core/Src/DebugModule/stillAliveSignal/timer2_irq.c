/*
 * timer2_irq.c
 *
 *  Created on: 31 lip 2023
 *      Author: Pietia
 */

#include "main.h"
#include "IstillAliveTimer.h"

static timer_Callback_t timer2_CallBack = NULL;


void timer2_init(void)
{
/* TIM2 - config (with interrupt)*/
/* ***************************************** */
	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;	// TIM2 - clock enable
	TIM2 -> PSC = 16000 - 1;				// TIM2 - timer prescaler
	TIM2 -> ARR = 1000 - 1;					// TIM2 - timer auto reload register
	TIM2 -> DIER |= TIM_DIER_UIE;			// TIM2 - update interrupt enable
	TIM2 -> EGR |= TIM_EGR_UG;				// TIM2 - re-initialize the counter and generates an update of the registers

	NVIC_EnableIRQ(TIM2_IRQn);				// TIM2 - IRQ enable
/* ***************************************** */
}

void timer2_start()
{
	TIM2 -> CR1 |= TIM_CR1_CEN; 			// TIM2 - timer enable
}

void timer2_stop()
{
	TIM2 -> CR1 |= TIM_CR1_CEN; 			// TIM2 - timer enable
}

void timer2_subscribe_irq(timer_Callback_t callback)
{
	timer2_CallBack = callback;
}

__attribute__((interrupt)) void TIM2_IRQHandler(void)
{

	if ((TIM2->SR & TIM_SR_UIF) == 1)	// TIM2 - check update interrupt flag
	{
		TIM2->SR &= ~(TIM_SR_UIF);

		if(timer2_CallBack != NULL)
		{
			timer2_CallBack();
		}
	}
}

