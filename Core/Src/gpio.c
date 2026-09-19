/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   GPIO peripheral initialization implementation (Pure CMSIS)
 *          Board: DevEBox STM32F4XX-M (STM32F407VET6)
 ******************************************************************************
 */

#include "main.h"
#include "gpio.h"

/**
 * @brief  Initialize GPIOs using pure CMSIS registers
 *         - PA1: User LED (D2, Active-Low, Push-Pull, High-Speed)
 *         - PA0: User Button (K1, Active-High, Input with Pull-Down)
 */
void GPIO_Init(void)
{
  /* 1. Enable GPIOA peripheral clock */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  /* 2. Configure PA1 (User LED, Active-Low): Output mode (01), Push-Pull (0), High-Speed (10), No-Pull (00) */
  GPIOA->MODER   &= ~(3U << (LED_PIN_POS * 2U));
  GPIOA->MODER   |=  (1U << (LED_PIN_POS * 2U));
  GPIOA->OTYPER  &= ~(1U << LED_PIN_POS);
  GPIOA->OSPEEDR &= ~(3U << (LED_PIN_POS * 2U));
  GPIOA->OSPEEDR |=  (2U << (LED_PIN_POS * 2U));
  GPIOA->PUPDR   &= ~(3U << (LED_PIN_POS * 2U));

  /* Initial state: LED OFF (Drive HIGH for active-low LED) */
  LED_OFF();

  /* 3. Configure PA0 (User Button, Active-High): Input mode (00), Pull-Down (10) */
  GPIOA->MODER   &= ~(3U << (BUTTON_PIN_POS * 2U));
  GPIOA->PUPDR   &= ~(3U << (BUTTON_PIN_POS * 2U));
  GPIOA->PUPDR   |=  (2U << (BUTTON_PIN_POS * 2U));
}
