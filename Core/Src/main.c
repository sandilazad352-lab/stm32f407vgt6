/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Pure CMSIS + FreeRTOS application for STM32_BLACK_BOX
 *                   Target: DevEBox STM32F4XX-M (STM32F407VGT6)
 ******************************************************************************
 */

#include "main.h"

/* -------------------------------------------------------------------------- */
/* Private Function Prototypes                                                */
/* -------------------------------------------------------------------------- */
static void vHeartbeatTask(void *pvParameters);
static void vButtonTask(void *pvParameters);

/* -------------------------------------------------------------------------- */
/* Global / Static Variables                                                  */
/* -------------------------------------------------------------------------- */
static volatile uint32_t blink_delay_ms = 200;

/* -------------------------------------------------------------------------- */
/* Main Program Entry                                                         */
/* -------------------------------------------------------------------------- */
int main(void) {
  /* 1. Enable Hardware FPU (CP10 and CP11 full access) */
  SCB->CPACR |= ((3UL << (10 * 2)) | (3UL << (11 * 2)));

  /* 2. Relocate Vector Table to internal Flash base */
  SCB->VTOR = FLASH_BASE;

  /* 3. Configure 4 bits preemption priority (required by FreeRTOS Cortex-M) */
  NVIC_SetPriorityGrouping(0x00000003U);

  /* 4. Configure System Clock to 168 MHz via 8 MHz HSE crystal */
  SystemClock_Config();

  /* 5. Initialize GPIO peripherals (PA1 LED, PA0 Button) */
  GPIO_Init();

  /* 6. Create FreeRTOS Tasks */
  xTaskCreate(vHeartbeatTask, "Heartbeat", 128, NULL, tskIDLE_PRIORITY + 1,
              NULL);
  xTaskCreate(vButtonTask, "Button", 128, NULL, tskIDLE_PRIORITY + 2, NULL);

  /* 7. Start the FreeRTOS Scheduler */
  vTaskStartScheduler();

  /* Execution should never reach here unless heap allocation failed */
  Error_Handler();
}

/* -------------------------------------------------------------------------- */
/* FreeRTOS Port Timer Hook                                                   */
/* -------------------------------------------------------------------------- */
/**
 * @brief  Override FreeRTOS weak vPortSetupTimerInterrupt using CMSIS SysTick
 */
void vPortSetupTimerInterrupt(void) {
  /* SysTick must have the lowest priority (15) for FreeRTOS Cortex-M port */
  NVIC_SetPriority(SysTick_IRQn, 15);

  /* Configure SysTick reload for 1 kHz tick rate (configTICK_RATE_HZ) */
  SysTick->LOAD = (SystemCoreClock / configTICK_RATE_HZ) - 1UL;
  SysTick->VAL = 0UL;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
                  SysTick_CTRL_ENABLE_Msk;
}

/* -------------------------------------------------------------------------- */
/* FreeRTOS Tasks                                                             */
/* -------------------------------------------------------------------------- */
/**
 * @brief  Heartbeat LED Task - toggles User LED (PA1) at dynamic rate
 */
static void vHeartbeatTask(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    LED_TOGGLE();
    vTaskDelay(pdMS_TO_TICKS(blink_delay_ms));
  }
}

/**
 * @brief  Button Monitor Task - polls Button (PA0) and modulates blink speed
 */
static void vButtonTask(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    /* Accelerate heartbeat while user button is pressed */
    blink_delay_ms = BUTTON_IS_PRESSED() ? 60 : 500;
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

/* -------------------------------------------------------------------------- */
/* Fault / Error Handler                                                      */
/* -------------------------------------------------------------------------- */
void Error_Handler(void) {
  __disable_irq();
  while (1) {
    LED_TOGGLE();
    for (volatile uint32_t i = 0; i < 2000000UL; i++) {
    }
  }
}
