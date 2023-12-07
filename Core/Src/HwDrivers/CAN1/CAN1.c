#include "main.h"
#include "CAN1.h"

static void CAN1_InitialisationMode(void);
static void CAN1_NormalMode(void);
static void CAN1_ExitSleepMode(void);

bool CAN1_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // GPIOB clock enable
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;  // CAN1 clock enable

    // GPIO Config (PB9 - CAN1)
    GPIOB->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE8_1; // Alternate function mode
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1 | GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR8_0;
    GPIOB->AFR[1] = GPIO_AFRH_AFSEL9_3 | GPIO_AFRH_AFSEL9_1; // AF9 - CAN1_TX PB9;
    GPIOB->AFR[1] = GPIO_AFRH_AFSEL8_3 | GPIO_AFRH_AFSEL8_1; // AF9 - CAN1_RX PB8;

    CAN1_InitialisationMode();
    CAN1_ExitSleepMode();
    /* Configure CAN options */
    /* Set the bit timing register */

    /* Request leave initialisation CAN_MCR_INRQ = 0*/
    /* Wait the acknowledge CAN_MSR_INAK */
    CAN1_NormalMode();

    return true;
}

bool CAN1_start(void)
{
    CAN1_NormalMode();

    return true;
}

bool CAN1_stop(void)
{
    CAN1_InitialisationMode();

    return true;
}

static void CAN1_InitialisationMode(void)
{
    /* Request initialisation CAN_MCR_INRQ = 1 */
    CAN1->MCR |= CAN_MCR_INRQ;
    /* Wait initialisation acknowledge CAN_MSR_INAK */
    while (!(CAN1->MSR & CAN_MSR_INAK))
        ;
}

static void CAN1_NormalMode(void)
{
    /* Request initialisation CAN_MCR_INRQ = 1 */
    CAN1->MCR &= ~(CAN_MCR_INRQ);
    /* Wait initialisation acknowledge CAN_MSR_INAK */
    while ((CAN1->MSR & CAN_MSR_INAK))
        ;
}

static void CAN1_ExitSleepMode(void)
{
    /* Exit from sleep mode CAN_MCR_SLEEP */
    CAN1->MCR &= ~(CAN_MCR_SLEEP);
    /* Check Sleep mode leave acknowledge CAN_MSR_SLAK */
    while ((CAN1->MSR & CAN_MSR_SLAK))
        ;
}
