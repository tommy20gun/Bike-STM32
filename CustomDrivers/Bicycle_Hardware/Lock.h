/**
  ******************************************************************************
  * 
  * @brief          : Header for Lock.c
  *                 This file contains includes for Lock.c
  * */

 //prevent recursive inclusion
#ifndef __LOCK
#define __LOCK

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h" 
#include "main.h"
#include "MemoryMap.h"

class Lock{
  public:
    TaskHandle_t vLockFunctionHandle;
    TaskHandle_t stateMachineHandle;
    SemaphoreHandle_t bsem;

    Lock(TaskHandle_t statetask);
    void initPeripheral();
    void initTask();

    static void vLockFunction(void* pvParameters);
};

#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization

