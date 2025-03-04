/**
  ******************************************************************************
  * @file           : Headlight.h
  * @brief          : Header for Headlight.c
  *                 This file contains includes for Headlight.c
  * */


//prevent recursive inclusion
#ifndef __HEADLIGHT
#define __HEADLIGHT

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "main.h"
#include "MemoryMap.h"


class Headlight{
  public:
    SemaphoreHandle_t bsem;
    static TaskHandle_t vTurnOnHeadlightHandle;
    Headlight();
    static void vTurnonHeadlight(void* pvParameters);
    void initTasks();
    void initPeripherals();
    private:

};

extern QueueHandle_t qhandle; 

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


