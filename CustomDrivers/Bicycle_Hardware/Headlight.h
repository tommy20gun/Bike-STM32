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

void vTurnonHeadlight(void* pvParameters);
class Headlight{
  public:
    SemaphoreHandle_t bsem;
    //TODO static
    TaskHandle_t vTurnOnHeadlightHandle;
    Headlight();
    static void vTurnonHeadlight(void* pvParameters);
};

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


