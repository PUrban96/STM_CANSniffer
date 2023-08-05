/*
 * led_blink.c
 *
 *  Created on: 31 lip 2023
 *      Author: Pietia
 */

#include "main.h"
#include "led_blink.h"


void led_init(void)
{
	/* GPIO PA5 - config as output */
	/* ***************************************** */
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;			// GPIOA - clock enable

		GPIOA->MODER |= GPIO_MODER_MODER5_0;
		GPIOA->MODER &= ~(GPIO_MODER_MODER5_1);			// PA5 - general purpose output mode

		GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);			// PA5 - output push-pull

		GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5_0);
		GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5_1);	// PA5 - low speed

		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5_0);
		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5_1);			// PA5 - no pull-up, pull-down

		GPIOA->BSRR |= GPIO_BSRR_BS5;					// PA5 - high state
	/* ***************************************** */
}

void led_toggle(void)
{
	if((GPIOA->ODR & GPIO_ODR_OD5) == 0)
	{
		GPIOA->BSRR |= GPIO_BSRR_BS5;
	}
	else
	{
		GPIOA->BSRR |= GPIO_BSRR_BR5;
	}
}
