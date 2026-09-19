/**
 ******************************************************************************
 * @file    clock.h
 * @brief   System clock configuration header for STM32F407 (Pure CMSIS)
 ******************************************************************************
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Configure STM32F407 System Clock to 168 MHz using 8 MHz HSE crystal
 *         HSE (8 MHz) -> PLLM=8 (1 MHz) -> PLLN=336 (336 MHz) -> PLLP=/2 (168 MHz)
 *         HCLK = 168 MHz, APB1 = 42 MHz (DIV4), APB2 = 84 MHz (DIV2)
 */
void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */
