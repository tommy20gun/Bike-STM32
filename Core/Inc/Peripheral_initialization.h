//prevent recursive inclusion
#ifndef __PERIPHERAL_INITIALIZATION
#define __PERIPHERAL_INITIALIZATION

#ifdef __cplusplus
extern "C" {
#endif
#include <stm32f411xe.h> //for baremetal GPIO Setup
#include "FreeRTOS.h" //for thread awareness line
#include <main.h> 

void SystemClock_Config(void);
void GPIO_Setup(void); //enable GPIO
UART_HandleTypeDef* USART2_Setup(void); //enable USART2
void Error_Handler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization


