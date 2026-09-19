/**
 ******************************************************************************
 * @file    gpio.h
 * @brief   GPIO peripheral initialization header for STM32F407 (Pure CMSIS)
 ******************************************************************************
 */

#ifndef GPIO_H_
#define GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "board_defs.h"

/**
 * @brief  Initialize onboard GPIO pins (User LED PA1, User Button PA0)
 */
void GPIO_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H_ */
