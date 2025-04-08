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
    SemaphoreHandle_t bsemRXNE;
    SemaphoreHandle_t bsemTXE;
    uint8_t unlockCode[100];

    Lock(TaskHandle_t statetask);
    void initPeripheral();
    void initTask();

    static void vLockFunction(void* pvParameters);

    private:
    typedef enum command{checkRDYFlag,bruh, bruh2};
    char command1[2] = {0x03, 0x69}; //69 is magic stop
    char command2[5];
    char command3[5];
    char* commandArray[3] = {command1, command2, command3};

    void receiveData(uint8_t* dest, int length);
    void sendCommandFrame(command cmd);
    bool isReady();
    void waitClockCycle(int cycles);
    void chipSelect(bool status);

};

void SPI3_IRQHandler(void);
#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization

