#include "stm32f4xx.h"

/**
 ******************************************************************************
 * @file    wifi.h
 * @brief   wifi module header for STM32F407 (Pure CMSIS)
 ******************************************************************************
 */

#ifndef WIFI_H_
#define WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_defs.h"
#include "stm32f4xx.h"

/**
 * @brief  Initialize onboard GPIO pins (User LED PA1, User Button PA0)
 */
void WIFI_Init();
void WIFI_Connect(char *ssid, char *password);
void WIFI_Disconnect();
void WIFI_Send_Data(char *data);
void WIFI_Receive_Data(char *data, uint32_t timeout);
void WIFI_Process_Data(char *data);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_H_ */
