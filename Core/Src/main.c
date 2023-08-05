
#include "main.h"
#include "stillAliveSignal.h"

int main(void)
{

/* SysTick  - config */
/* ***************************************** */
	SysTick_Config(16000000);
/* ***************************************** */

	stillAliveSignal_Init();
	stillAliveSignal_Start();

  while (1)
  {

  }

}

__attribute__((interrupt)) void SysTick_Handler(void)
{
	//LED_Blink();
}



