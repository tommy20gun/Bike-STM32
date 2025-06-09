/**
  ******************************************************************************
  *
  * @brief          : Header for BLE.c
  *                 This file contains includes for BLE.c
  * */



//prevent recursive inclusion
#ifndef __BLUETOOTH
#define __BLUETOOTH

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "stm32f4xx_ll_usart.h"
#include "main.h"
#include "MemoryMap.h"
#include <string.h>
//#include "MemoryMap.h"

#define DATA_MODE 0
#define COMMAND_MODE 1

#define BLEConnected 1
#define BLEDisconnected 0


typedef bool BTState; 
typedef bool BTMode; 

class Bluetooth{
  public:
    TaskHandle_t sendHandle;
    TaskHandle_t sendSlowHandle;
    TaskHandle_t sendFastHandle;
    TaskHandle_t sendSpecialCommandHandle;
    QueueHandle_t messenger;
    //SemaphoreHandle_t sendSemaphore;

    MemoryMap map;
    BTMode mode;
    BTState state;
    Bluetooth();
    
    void initPeripherials();
    void initTasks();
    BTState getConnectionState();
    void initBTMemoryMap();
    

    //FreeRTOS tasks
    static void send(void* pvParameters);
    /*
    static void sendFast(void* pvParameters);
    static void sendSpecialCommand(void* pvParameters);*/
    
    

    private:
    void setmode(int mode);
    void ATModeTesting();
    static void uartTransmitDMA(uint32_t size);
    static uint32_t CRC32MemoryMap(MemoryMap* map);
    static void queueReceive(MemoryMap* map, Bluetooth* tooth);
};

void DMA1_Stream6_IRQHandler(void);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization


