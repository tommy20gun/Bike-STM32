//prevent recursive inclusion
#ifndef __PERIPHERAL_INITIALIZATION
#define __PERIPHERAL_INITIALIZATION

#ifdef __cplusplus
extern "C" {
#endif
#include <stm32f411xe.h>

void RCC_Enable_HSE(void); //enable clock
void GPIO_Setup(void); //enable GPIO

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__Peripheral_initialization


