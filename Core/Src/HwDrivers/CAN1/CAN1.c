#include "main.h"
#include "CAN1.h"

#define DEFAULT_BAUDRATE BAUDRATE_500KBIT

static bool CAN1_InitialisationMode(void);
static bool CAN1_NormalMode(void);
static bool CAN1_ExitSleepMode(void);
static void CAN1_setOptions(void);
static void CAN1_setInterrupt(void);
static void CAN1_setBaudRate(CANLowLevel_BaudRate_e Baudrate);

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
    CAN1_setOptions();
    /* Set the bit timing register */
    CAN1_setBaudRate(DEFAULT_BAUDRATE);
    /* Enable CAN interrupt */
    CAN1_setInterrupt();

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

static bool CAN1_InitialisationMode(void)
{
    /* Request initialisation CAN_MCR_INRQ = 1 */
    CAN1->MCR |= CAN_MCR_INRQ;
    /* Wait initialisation acknowledge CAN_MSR_INAK */
    while (!(CAN1->MSR & CAN_MSR_INAK))
        ;
}

static bool CAN1_NormalMode(void)
{
    /* Request initialisation CAN_MCR_INRQ = 1 */
    CAN1->MCR &= ~(CAN_MCR_INRQ);
    /* Wait initialisation acknowledge CAN_MSR_INAK */
    while ((CAN1->MSR & CAN_MSR_INAK))
        ;
}

static bool CAN1_ExitSleepMode(void)
{
    /* Exit from sleep mode CAN_MCR_SLEEP */
    CAN1->MCR &= ~(CAN_MCR_SLEEP);
    /* Check Sleep mode leave acknowledge CAN_MSR_SLAK */
    while ((CAN1->MSR & CAN_MSR_SLAK))
        ;
}

static void CAN1_setOptions(void)
{
      /* Time Triggered Mode = DISABLE */
      /* Auto BusOff = DISABLE */
      /* Auto WakeUp = DISABLE */
      /* Auto Retransmission = ENABLE */
      /* Receive Fifo Locked = ENABLE */
      /* Transmit Fifo Priority = ENABLE */

      CAN1->MCR |= CAN_MCR_RFLM | CAN_MCR_TXFP;
}

static void CAN1_setInterrupt(void)
{
      /* Time Triggered Mode = DISABLE */
      /* Auto BusOff = DISABLE */
      /* Auto WakeUp = DISABLE */
      /* Auto Retransmission = ENABLE */
      /* Receive Fifo Locked = ENABLE */
      /* Transmit Fifo Priority = ENABLE */
}

bool CAN1_changeBaudRate(CANLowLevel_BaudRate_e Baudrate)
{
    bool status = CAN1_InitialisationMode();

    if (status == true)
    {
        CAN1_setBaudRate(Baudrate);
        CAN1_NormalMode();
    }

    return status;
}

static void CAN1_setBaudRate(CANLowLevel_BaudRate_e Baudrate)
{
    switch (Baudrate)
    {
    case BAUDRATE_50KBIT:
    {
        /* APB1 = 42MHz */
        /* Baud rate prescaler = 140 */
        /* Time segment 1 = 2 */
        /* Time segment 2 = 3 */
        /* Resynchronization jump width = 3 */
        /* Baud Rate = 50000 bit/s*/
        CAN1->BTR |= (139UL << CAN_BTR_BRP_Pos) | (1UL << CAN_BTR_TS1_Pos) | (2UL << CAN_BTR_TS2_Pos) | (2UL << CAN_BTR_SJW_Pos);
        break;
        break;
    }

    case BAUDRATE_100KBIT:
    {
        /* APB1 = 42MHz */
        /* Baud rate prescaler = 70 */
        /* Time segment 1 = 2 */
        /* Time segment 2 = 3 */
        /* Resynchronization jump width = 3 */
        /* Baud Rate = 100000 bit/s*/
        CAN1->BTR |= (69UL << CAN_BTR_BRP_Pos) | (1UL << CAN_BTR_TS1_Pos) | (2UL << CAN_BTR_TS2_Pos) | (2UL << CAN_BTR_SJW_Pos);
        break;
    }

    case BAUDRATE_125KBIT:
    {
        /* APB1 = 42MHz */
        /* Baud rate prescaler = 56 */
        /* Time segment 1 = 2 */
        /* Time segment 2 = 3 */
        /* Resynchronization jump width = 3 */
        /* Baud Rate = 124999 bit/s*/
        CAN1->BTR |= (55UL << CAN_BTR_BRP_Pos) | (1UL << CAN_BTR_TS1_Pos) | (2UL << CAN_BTR_TS2_Pos) | (2UL << CAN_BTR_SJW_Pos);
        break;
    }

    case BAUDRATE_250KBIT:
    {
        /* APB1 = 42MHz */
        /* Baud rate prescaler = 28 */
        /* Time segment 1 = 2 */
        /* Time segment 2 = 3 */
        /* Resynchronization jump width = 3 */
        /* Baud Rate = 249999 bit/s*/
        CAN1->BTR |= (27UL << CAN_BTR_BRP_Pos) | (1UL << CAN_BTR_TS1_Pos) | (2UL << CAN_BTR_TS2_Pos) | (2UL << CAN_BTR_SJW_Pos);
        break;
    }

    case BAUDRATE_500KBIT:
    {
        /* APB1 = 42MHz */
        /* Baud rate prescaler = 14 */
        /* Time segment 1 = 2 */
        /* Time segment 2 = 3 */
        /* Resynchronization jump width = 3 */
        /* Baud Rate = 499999 bit/s*/
        CAN1->BTR |= (13UL << CAN_BTR_BRP_Pos) | (1UL << CAN_BTR_TS1_Pos) | (2UL << CAN_BTR_TS2_Pos) | (2UL << CAN_BTR_SJW_Pos);
        break;
    }

    case BAUDRATE_1000KBIT:
    {
        /* APB1 = 42MHz */
        /* Baud rate prescaler = 7 */
        /* Time segment 1 = 2 */
        /* Time segment 2 = 3 */
        /* Resynchronization jump width = 3 */
        /* Baud Rate = 999999 bit/s*/
        CAN1->BTR |= (6UL << CAN_BTR_BRP_Pos) | (1UL << CAN_BTR_TS1_Pos) | (2UL << CAN_BTR_TS2_Pos) | (2UL << CAN_BTR_SJW_Pos);
        break;
    }
    }
}
