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
#include "Device.h"
#include <string.h>
//#include "MemoryMap.h"

#define DATA_MODE 0
#define COMMAND_MODE 1

#define BLEConnected 1
#define BLEDisconnected 0


typedef bool BTState; 
typedef bool BTMode; 

class Bluetooth : public Device{
  public:
    Bluetooth(QueueHandle_t messenger){
      this->messenger = messenger;
      initPeripherals();
      initTasks();
      initBTMemoryMap();
    };

  protected:
    MemoryMap map;
    BTMode mode;
    BTState state;

    //device.h
    void initPeripherals();
    void initTasks();
    static void vTaskFunction(void* pvParameters);
    void RTOSImplementation();
    void QueueSend();//no implementation, not used
 
    //helpers
    BTState getConnectionState();
    void initBTMemoryMap();
    void setmode(int mode);
    void emptyQueue();
    void ATModeTesting(); //used for setup only
    static void uartTransmitDMA(uint32_t size);
    static uint32_t CRC32MemoryMap(MemoryMap* map);
};


#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization


