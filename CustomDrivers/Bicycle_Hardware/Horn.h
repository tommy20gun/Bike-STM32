/**
  ******************************************************************************
  * @file           : Horn.h
  * @brief          : Header for Horn.c file.
  *                   This file contains includes for Horn.c.
  * */

 //prevent recursive inclusion
#ifndef __HORN
#define __HORN


#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "main.h"


//void hornSetup();
void vTurnOnHorn(void* pvParameters);

#ifdef __cplusplus
}
#endif //__cplusplus


class Horn{
  public:
  SemaphoreHandle_t bsem_horn;
  TaskHandle_t vTurnOnHornHandle;

  Horn();
};



#endif //__Peripheral_initialization

