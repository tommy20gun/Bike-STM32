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
#include "MemoryMap.h"
class Taillight{
  public:
  SemaphoreHandle_t bsemleft;
  SemaphoreHandle_t bsemright;
  SemaphoreHandle_t bsembrake;
  static TaskHandle_t vTurnLeftHandle;
  static TaskHandle_t vTurnRightHandle;
  static TaskHandle_t vBrakeHandle;
  Taillight();
  static void vTurnLeft(void* pvParameters);
  static void vTurnRight(void* pvParameters);
  static void vBrake(void* pvParameters);
  void initTasks();
  void initPeripherals();
  private:

};

extern QueueHandle_t qhandle; 

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization

