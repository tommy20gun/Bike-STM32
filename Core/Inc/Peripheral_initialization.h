/**
  ******************************************************************************
  * @file           : Peripheral_initialization.h
  * @brief          : Header for Peripheral_initialization.c file.
  *                   This file contains includes for Peripheral_initialization.c.
  ******************************************************************************
  * @attention
  * */

//prevent recursive inclusion
#ifndef __PERIPHERAL_INITIALIZATION
#define __PERIPHERAL_INITIALIZATION

#ifdef __cplusplus
extern "C" {
#endif
#include <stm32f411xe.h> //for baremetal GPIO Setup
#include "FreeRTOS.h" 
#include "stm32f4xx_hal.h" 
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_gpio.h"
#include "task.h"

void SystemClock_Config(void);
void GPIO_GlobalSetup(void); //enable GPIO
UART_HandleTypeDef* USART2_Setup(void); //enable USART2
void Error_Handler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );

#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization


