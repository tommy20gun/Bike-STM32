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
class Taillight{
  public:
  SemaphoreHandle_t bsemleft;
  SemaphoreHandle_t bsemright;
  SemaphoreHandle_t bsembrake;
  TaskHandle_t vTurnLeftHandle;
  TaskHandle_t vTurnRightHandle;
  TaskHandle_t vBrakeHandle;
  Taillight();
  static void vTurnLeft(void* pvParameters);
  static void vTurnRight(void* pvParameters);
  static void vBrake(void* pvParameters);
};



#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization

