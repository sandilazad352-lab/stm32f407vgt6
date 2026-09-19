/*
 * FreeRTOS Kernel Configuration for STM32F407VGT6 (DevEBox STM32F4XX-M)
 * Pure CMSIS + Cortex-M4F Hardware FPU
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include "stm32f4xx.h"
    #include "board_defs.h"
    extern uint32_t SystemCoreClock;
#endif

/* -------------------------------------------------------------------------- */
/* Core Configuration                                                         */
/* -------------------------------------------------------------------------- */
#define configUSE_PREEMPTION                    1
#define configSUPPORT_STATIC_ALLOCATION          0
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      (SystemCoreClock)
#define configTICK_RATE_HZ                      ((TickType_t)1000)
#define configMAX_PRIORITIES                    (7)
#define configMINIMAL_STACK_SIZE                ((uint16_t)128)
#define configTOTAL_HEAP_SIZE                   ((size_t)(48 * 1024)) /* 48 KBytes */
#define configMAX_TASK_NAME_LEN                 (16)
#define configUSE_TRACE_FACILITY                1
#define configUSE_16_BIT_TICKS                  0
#ifndef TICK_TYPE_WIDTH_16_BITS
    #define TICK_TYPE_WIDTH_16_BITS             0
    #define TICK_TYPE_WIDTH_32_BITS             1
    #define TICK_TYPE_WIDTH_64_BITS             2
#endif
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS
#define configIDLE_SHOULD_YIELD                 1

/* Default trace macros if not defined */
#ifndef traceISR_ENTER
    #define traceISR_ENTER()
#endif
#ifndef traceISR_EXIT
    #define traceISR_EXIT()
#endif
#ifndef traceISR_EXIT_TO_SCHEDULER
    #define traceISR_EXIT_TO_SCHEDULER()
#endif
#define configUSE_MUTEXES                       1
#define configQUEUE_REGISTRY_SIZE               8
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1
#define configGENERATE_RUN_TIME_STATS           0

/* -------------------------------------------------------------------------- */
/* Software Timer Configuration                                               */
/* -------------------------------------------------------------------------- */
#define configUSE_TIMERS                        0

/* -------------------------------------------------------------------------- */
/* Set the following definitions to 1 to include the API function, or zero    */
/* to exclude the API function.                                               */
/* -------------------------------------------------------------------------- */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           0
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1

/* -------------------------------------------------------------------------- */
/* Cortex-M specific definitions.                                             */
/* -------------------------------------------------------------------------- */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS                     __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS                     4 /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

/* Interrupt priorities used by the kernel port layer itself. */
#define configKERNEL_INTERRUPT_PRIORITY         (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!! */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configCHECK_HANDLER_INSTALLATION        0

/* Normal assert() definition with clear, visible LED blink indication on failure */
#define configASSERT( x ) if( ( x ) == 0 ) { \
    taskDISABLE_INTERRUPTS(); \
    while( 1 ) { \
        LED_TOGGLE(); \
        for( volatile uint32_t _i = 0; _i < 4000000UL; _i++ ) {} \
    } \
}

/* -------------------------------------------------------------------------- */
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS   */
/* standard names.                                                            */
/* -------------------------------------------------------------------------- */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
