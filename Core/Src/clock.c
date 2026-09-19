/**
 ******************************************************************************
 * @file    clock.c
 * @brief   System clock configuration implementation for STM32F407 (Pure CMSIS)
 *          Board: DevEBox STM32F4XX-M (STM32F407VGT6, 8 MHz HSE crystal)
 ******************************************************************************
 */

#include "main.h"
#include "clock.h"

/**
 * @brief  Configure STM32F407 System Clock to 168 MHz using 8 MHz HSE crystal
 *         HSE (8 MHz) -> PLLM=8 (1 MHz) -> PLLN=336 (336 MHz) -> PLLP=/2 (168 MHz)
 *         PLLQ = 7 (48 MHz USB/SDIO)
 *         HCLK = 168 MHz, APB1 = 42 MHz (DIV4), APB2 = 84 MHz (DIV2)
 */
void SystemClock_Config(void)
{
  /* 1. Enable Power Controller clock */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;

  /* 2. Set Voltage Scaling to Scale 1 (allows up to 168 MHz) */
  PWR->CR |= PWR_CR_VOS;

  /* 3. Enable HSE oscillator (8 MHz crystal) and wait for ready */
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY)) {}

  /* 4. Configure Flash: Prefetch, Instruction cache, Data cache, 5 wait states */
  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

  /* 5. Configure PLL: PLLM=8, PLLN=336, PLLP=/2, PLLQ=7, Source=HSE */
  RCC->PLLCFGR = (8U   << RCC_PLLCFGR_PLLM_Pos)
               | (336U << RCC_PLLCFGR_PLLN_Pos)
               | (0U   << RCC_PLLCFGR_PLLP_Pos)
               | RCC_PLLCFGR_PLLSRC_HSE
               | (7U   << RCC_PLLCFGR_PLLQ_Pos);

  /* 6. Enable PLL and wait for lock */
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY)) {}

  /* 7. Configure bus prescalers: AHB=DIV1, APB1=DIV4, APB2=DIV2 */
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

  /* 8. Switch System Clock to PLL */
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

  /* 9. Update CMSIS core clock variable */
  SystemCoreClock = 168000000UL;
}
