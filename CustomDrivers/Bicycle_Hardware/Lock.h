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
    uint8_t unlockCode[100];

    Lock(TaskHandle_t statetask);
    void initPeripheral();
    void initTask();

    static void vLockFunction(void* pvParameters);

    private:
    void chipSelect(bool status);
    void sendByte(uint8_t buff);
    void receiveByte(uint8_t* dest);
    void checkReady();

    void sendCommandFrame(int command);
    char* receiveCommandData();

    uint8_t* receiveNFC();

    
};

void SPI3_IRQHandler(void);
#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization

