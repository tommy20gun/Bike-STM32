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
    enum command{CHECKRDYFLAG, INLISTPASSIVETARGETS, INAUTOPOLL};
    uint8_t command1[2] = {0x02, 0x69}; //69 is magic stop
    uint8_t command2[11] = {
      0x01, 
      0x00, 
      0xFF, 
      sizeof(command2)-6, 
      256 - (sizeof(command2)-6),
      0xD4,
      0x4A,//inListPassiveTarget
      0x01, //maxTg
      0x00, //106kbps type A
      256 -(0x04+0x0A+0x01+0x00), //checksum is DCS = -(lower byte of TFI-pDN)
      //no postamble
      0x69 //magic number
    };
    uint8_t command3[12]{
      0x01,
      0x00,
      0xFF,
      sizeof(command3)-6,
      256-(sizeof(command3)-6),
      0xD4,
      0x60, //autoPoll
      0xFF, //endless polling
      0x06, //900ms polling period
      0x00, //type (Generic passive 106 kbps (ISO/IEC14443-4A, Mifare and DEP)
      256 -(0x04 + 0x00 + 0x0F + 0x06 + 0x00),
      //no postamble
      0x69 //magic number
    };
    uint8_t* commandArray[3] = {command1, command2, command3};

    void receiveData(uint8_t* dest, int length);
    void sendCommandFrame(command cmd);
    bool isReady();
    void cardRegistration();
    void startAutoPoll();
    void waitClockCycle(int cycles);
    void chipSelect(bool status);

};

void SPI3_IRQHandler(void);
#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization

