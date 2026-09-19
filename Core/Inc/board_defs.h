/**
 ******************************************************************************
 * @file    board_defs.h
 * @brief   Board support definitions for DevEBox STM32F4XX-M (Pure CMSIS)
 ******************************************************************************
 */

#ifndef BOARD_DEFS_H_
#define BOARD_DEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

/* -------------------------------------------------------------------------- */
/* Board Information                                                          */
/* -------------------------------------------------------------------------- */
#define BOARD_NAME                  "DevEBox STM32F4XX-M"
#define BOARD_MCU                   "STM32F407VGT6"
#define BOARD_HSE_FREQ_HZ           8000000UL    /* 8.000 MHz HSE crystal */
#define BOARD_LSE_FREQ_HZ           32768UL      /* 32.768 kHz RTC crystal */

/* -------------------------------------------------------------------------- */
/* User LED (D2) - PA1 (Active LOW)                                           */
/* -------------------------------------------------------------------------- */
#define LED_PIN_POS                 (1U)
#define LED_PIN_MASK                (1U << LED_PIN_POS)
#define LED_GPIO_PORT               GPIOA

/* Drive PA1 LOW to turn ON LED */
#define LED_ON()                    (LED_GPIO_PORT->BSRR = (LED_PIN_MASK << 16U))
/* Drive PA1 HIGH to turn OFF LED */
#define LED_OFF()                   (LED_GPIO_PORT->BSRR = LED_PIN_MASK)
/* Toggle PA1 */
#define LED_TOGGLE()                (LED_GPIO_PORT->ODR ^= LED_PIN_MASK)

/* -------------------------------------------------------------------------- */
/* User Button (K1 / KEY0) - PA0 (Active HIGH)                                */
/* -------------------------------------------------------------------------- */
#define BUTTON_PIN_POS              (0U)
#define BUTTON_PIN_MASK             (1U << BUTTON_PIN_POS)
#define BUTTON_GPIO_PORT            GPIOA

#define BUTTON_IS_PRESSED()         ((BUTTON_GPIO_PORT->IDR & BUTTON_PIN_MASK) != 0U)

/* -------------------------------------------------------------------------- */
/* Onboard Winbond W25Q16 (16 Mbit / 2 MB) SPI Flash Pins                     */
/* SPI1: PA15 (CS), PB3 (SCK), PB4 (MISO), PB5 (MOSI)                         */
/* -------------------------------------------------------------------------- */
#define FLASH_CS_PIN_POS            (15U)
#define FLASH_CS_PIN_MASK           (1U << FLASH_CS_PIN_POS)
#define FLASH_CS_PORT               GPIOA

#define FLASH_CS_LOW()              (FLASH_CS_PORT->BSRR = (FLASH_CS_PIN_MASK << 16U))
#define FLASH_CS_HIGH()             (FLASH_CS_PORT->BSRR = FLASH_CS_PIN_MASK)

#ifdef __cplusplus
}
#endif

#endif /* BOARD_DEFS_H_ */
