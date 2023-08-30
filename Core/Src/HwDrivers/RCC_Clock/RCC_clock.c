#include "main.h"
#include "RCC_clock.h"

/* Target */
/* **************************** */
// Default HSI clock - 16MHz
// HSE crystal - 12MHz
// Max bus frequency:
// SYSCLK - 168MHz
// HCLK (AHB2, AHB1) - 168MHz
// PCLK2 (APB2) - 84MHz
// PCLK1 (APB1) - 42MHz
/* **************************** */

/* Limits PLL */
/* **************************** */
// 1MHz ≤ fpllin ≤ 2MHz -> should be as close 2MHz as possible
// 2 ≤ PLLM ≤ 63

// 100MHz ≤ fVCO ≤ 432MHz
// 50 ≤ PLLN ≤ 432

// 2 ≤ PLLQ ≤ 15

// 2 ≤ PLLR ≤ 7
/* **************************** */

/* Calculation PLL */
/* **************************** */
// fpllin = HSE / M = 12MHz / 6 = 2MHz  (PLLM = 6)
// fVCO = fpllin * PLLN = 2MHz * 168 = 336MHz (PLLN = 168)
// fPLLOCLK = fVCO / PLLP = 336MHz / 2 = 168MHz (PLLP = 2, 00 -> PLLP = 2)
// fUSBOTG = fVCO / PLLQ = 336MHz / 7 = 48MHz (PLLQ = 7)

// (PLLR = 7)
/* **************************** */

/* Calculation AHB, APB */
/* **************************** */

/* **************************** */

void RCCClockConfiguration(void)
{
    /* PLL Configuration */
    /* ******************************************************************************************************* */
    RCC->CR |= RCC_CR_HSEON;

    RCC->PLLCFGR = 6UL << RCC_PLLCFGR_PLLM_Pos | 168UL << RCC_PLLCFGR_PLLN_Pos |
                   7UL << RCC_PLLCFGR_PLLQ_Pos | 7UL << RCC_PLLCFGR_PLLR_Pos | RCC_PLLCFGR_PLLSRC_HSE;

    while (!(RCC->CR & RCC_CR_HSERDY))
        ; // HSERDY goes low after 6 HSE oscillator clock cycles.

    RCC->CR |= RCC_CR_PLLON;
    RCC->CFGR = RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_HPRE_DIV1;
    /* ******************************************************************************************************* */

    /* Flash latency*/
    /* ******************************************************************************************************* */
    // 5 WS (6 CPU cycles)
    // LATENCY = 5

    FLASH->ACR = FLASH_ACR_DCRST | FLASH_ACR_ICRST; // flash buffer reset
    FLASH->ACR = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

    while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_5WS) // wait for flash latency ready
        ;

    /* ******************************************************************************************************* */

    /* PLL Run */
    /* ******************************************************************************************************* */
    while (!(RCC->CR & RCC_CR_PLLRDY)) // wait for PLL ready
        ;

    RCC->CFGR |= RCC_CFGR_SW_PLL;                          // PLL selected as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) // wait for PLL used as the system clock
        ;

    RCC->CR &= ~RCC_CR_HSION; // disable HSI
    /* ******************************************************************************************************* */

    /*  IO Compensation Cell*/
    /* ******************************************************************************************************* */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    __DSB();
    SYSCFG->CMPCR = SYSCFG_CMPCR_CMP_PD;
    while (!(SYSCFG->CMPCR & SYSCFG_CMPCR_READY))
        ;

    /* ******************************************************************************************************* */
}