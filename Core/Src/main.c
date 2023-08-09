
#include "main.h"
#include "led_blink.h"

#include "FreeRTOS.h"
#include "task.h"

static void prvSetupHardware(void);
void vTaskLedToggle(void);

int main(void)
{

	// Hardware configuration
	prvSetupHardware();

	xTaskCreate(vTaskLedToggle, "stillAliveLED", 200, NULL, 5, NULL);

	// Start the scheduler
	vTaskStartScheduler(); // should never return

	// Will only get here if there was not enough heap space

	while (1)
		;

	return 0;
}

static void prvSetupHardware(void)
{

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
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}
