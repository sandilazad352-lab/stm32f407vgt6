/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file (Pure CMSIS + FreeRTOS)
 ******************************************************************************
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "board_defs.h"
#include "clock.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
