#include "main.h"
#include "CAN1.h"

#define DEFAULT_BAUDRATE BAUDRATE_500KBIT

static bool CAN1_InitialisationMode(void);
static bool CAN1_NormalMode(void);
static bool CAN1_ExitSleepMode(void);
static void CAN1_setOptions(void);
static void CAN1_setInterrupt(void);
static void CAN1_setBaudRate(CANLowLevel_BaudRate_e Baudrate);
static int8_t CAN1_getFreeTxMailboxNumber(void);
static void CAN1_addMessageToTxMailbox(CANLowLevel_Frame_s *Frame, uint8_t MailboxNumber);

bool CAN1_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;  // CAN1 clock enable
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // GPIOB clock enable
    __DSB();

    // GPIO Config (PB9 - CAN1)
    GPIOB->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE8_1; // Alternate function mode
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1 | GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR8_0;
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD9_1) & ~(GPIO_PUPDR_PUPD9_0); 
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_1) & ~(GPIO_PUPDR_PUPD8_0); 
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_3 | GPIO_AFRH_AFSEL9_0; // AF9 - CAN1_TX PB9;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_3 | GPIO_AFRH_AFSEL8_0; // AF9 - CAN1_RX PB8;

    CAN1_InitialisationMode();
    CAN1_ExitSleepMode();
    /* Configure CAN options */
    CAN1_setOptions();
    /* Set the bit timing register */
    CAN1_setBaudRate(DEFAULT_BAUDRATE);
    /* Enable CAN interrupt */
    CAN1_setInterrupt();

    /* Start CAN */
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
    while ((CAN1->MSR & CAN_MSR_INAK) == 0U)
        ;
}

static bool CAN1_NormalMode(void)
{
    /* Request leave initialisation CAN_MCR_INRQ = 0*/
    CAN1->MCR &= ~(CAN_MCR_INRQ);
    /* Wait the acknowledge CAN_MSR_INAK */
    /* This bit is cleared by hardware when the CAN hardware has left the initialization mode (to
    be synchronized on the CAN bus). To be synchronized the hardware has to monitor a
    sequence of 11 consecutive recessive bits on the CAN RX signal. */
    while ((CAN1->MSR & CAN_MSR_INAK) != 0U)
        ;
}

static bool CAN1_ExitSleepMode(void)
{
    /* Exit from sleep mode CAN_MCR_SLEEP */
    CAN1->MCR &= ~(CAN_MCR_SLEEP);
    /* Check Sleep mode leave acknowledge CAN_MSR_SLAK */
    while ((CAN1->MSR & CAN_MSR_SLAK) != 0U)
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
        CAN1->BTR |= (uint32_t)(13UL << CAN_BTR_BRP_Pos) | (1UL << CAN_BTR_TS1_Pos) | (2UL << CAN_BTR_TS2_Pos) | (2UL << CAN_BTR_SJW_Pos);
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

bool CAN1_transmite(CANLowLevel_Frame_s Frame)
{
    // Check if mailbox avaible (chack all CAN_TSR_TME bits)
    // Add data to free mailbox or wait for avaible mailbox
    // Set transmision request (TXRQ bit in the CAN_TIxR register)
    // Wait for transmission being succesful or error (RQCP and TXOK bits in the CAN_TSR register) -> optional
    int8_t Mailbox = CAN1_getFreeTxMailboxNumber();
    if (Mailbox >= 0)
    {
        CAN1_addMessageToTxMailbox(&Frame, Mailbox);
        return true;
    }
    else
    {
        return false;
    }
}

static int8_t CAN1_getFreeTxMailboxNumber(void)
{
    if (!((CAN1->TSR) & CAN_TSR_TME0))
    {
        return 0;
    }
    else if (!((CAN1->TSR) & CAN_TSR_TME1))
    {
        return 1;
    }
    else if (!((CAN1->TSR) & CAN_TSR_TME2))
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

static void CAN1_addMessageToTxMailbox(CANLowLevel_Frame_s *Frame, uint8_t MailboxNumber)
{
    /* Add ID to mailbox */
    if (Frame->CAN_LL_TYPE == CAN_LL_STD)
    {
        CAN1->sTxMailBox[MailboxNumber].TIR = (Frame->CAN_LL_ID << CAN_TI0R_STID_Pos);
    }
    else if (Frame->CAN_LL_TYPE == CAN_LL_EXT)
    {
        CAN1->sTxMailBox[MailboxNumber].TIR = (Frame->CAN_LL_ID << CAN_TI0R_EXID_Pos) | (CAN_TI0R_IDE);
    }

    /* Add RTR to mailbox */
    if (Frame->CAN_LL_RTR == CAN_LL_RTR_REMOTE)
    {
        CAN1->sTxMailBox[MailboxNumber].TIR |= CAN_TI0R_RTR;
    }

    /* Add DLC to mailbox */
    CAN1->sTxMailBox[MailboxNumber].TDTR |= (Frame->CAN_LL_DLC << CAN_TDT0R_DLC_Msk);

    /* Add Data to mailbox */
    CAN1->sTxMailBox[MailboxNumber].TDLR = (Frame->CAN_LL_DATA[0] << CAN_TDL0R_DATA0_Pos) |
                                           (Frame->CAN_LL_DATA[1] << CAN_TDL0R_DATA1_Pos) |
                                           (Frame->CAN_LL_DATA[2] << CAN_TDL0R_DATA2_Pos) |
                                           (Frame->CAN_LL_DATA[3] << CAN_TDL0R_DATA3_Pos);

    CAN1->sTxMailBox[MailboxNumber].TDHR = (Frame->CAN_LL_DATA[4] << CAN_TDH0R_DATA4_Pos) |
                                           (Frame->CAN_LL_DATA[5] << CAN_TDH0R_DATA5_Pos) |
                                           (Frame->CAN_LL_DATA[6] << CAN_TDH0R_DATA6_Pos) |
                                           (Frame->CAN_LL_DATA[7] << CAN_TDH0R_DATA7_Pos);

    /* Transmit mailbox request */
    CAN1->sTxMailBox[MailboxNumber].TIR |= CAN_TI0R_TXRQ;
}

void CAN1_subscribeReceivedIRQ(CANLowLevel_Receive_Callback_t ReceiveCallback)
{
}

void CAN1_subscribeErrorIRQ(CANLowLevel_Error_Callback_t ErrorCallback)
{
}
