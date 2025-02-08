//prevent recursive inclusion
#ifndef __PERIPHERAL_INITIALIZATION
#define __PERIPHERAL_INITIALIZATION

#ifdef __cplusplus
extern "C" {
#endif
#include <stm32f411xe.h>
#include <main.h>

void SystemClock_Config(void);
void GPIO_Setup(void); //enable GPIO
void USART2_Setup(void); //enable USART2
void Error_Handler(void);

#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization


