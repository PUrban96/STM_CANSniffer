
#include "main.h"
#include "led_blink.h"
#include "RCC_clock.h"

#include "FreeRTOS.h"
#include "task.h"
#include "stillAliveSignal.h"
#include "USART2_driver.h"

void prvSetupHardware(void);
void vTaskLedToggle(void);

int main(void)
{
	// DMA_Coppy();
	// DMA_Timer();

	//  Hardware configuration
	prvSetupHardware();
	led_init();
	// SysTick_Config(168000000ul / 8ul / 2ul);
	// SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	//  stillAliveSignal_Init();
	//  stillAliveSignal_Start();
	//  USART2_MirrorDMA_Init();

	xTaskCreate((TaskFunction_t)vTaskLedToggle, "stillAliveLED", 200, NULL, 5, NULL);

	// Start the scheduler
	vTaskStartScheduler(); // should never return

	// Will only get here if there was not enough heap space

	while (1)
		;

	return 0;
}

/*
void SysTick_Handler(void)
{

	if ((GPIOA->ODR & GPIO_ODR_OD5) == 0)
	{
		GPIOA->BSRR |= GPIO_BSRR_BS5;
	}
	else
	{
		GPIOA->BSRR |= GPIO_BSRR_BR5;
	}
}
*/

void prvSetupHardware(void)
{
	RCCClockConfiguration();
}

void vTaskLedToggle(void)
{
	led_init();
	for (;;)
	{
		if ((GPIOA->ODR & GPIO_ODR_OD5) == 0)
		{
			GPIOA->BSRR |= GPIO_BSRR_BS5;
		}
		else
		{
			GPIOA->BSRR |= GPIO_BSRR_BR5;
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
