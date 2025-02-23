 /** 
  * @file           : Tailight.h
  * @brief          : Header for TailLight.c. 
  * 
  * This file contains includes for Horn.c.
  */

//prevent recursive inclusion
#ifndef __TAILLIGHT
#define __TAILLIGHT

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h" 
#include "main.h"

void vTurnLeft(void* pvParameters);
void vTurnRight(void* pvParameters);
void vBrake(void* pvParameters);

#ifdef __cplusplus
}
#endif //__cplusplus

class Taillight{
  public:
  SemaphoreHandle_t bsemleft;
  SemaphoreHandle_t bsemright;
  SemaphoreHandle_t bsembrake;
  Taillight();

};


#endif //__Peripheral_initialization

