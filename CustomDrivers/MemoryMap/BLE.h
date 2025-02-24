/**
  ******************************************************************************
  *
  * @brief          : Header for BLE.c
  *                 This file contains includes for BLE.c
  * */



//prevent recursive inclusion
#ifndef __BLESEND
#define __BLESEND

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "main.h"

class BLE{
  public:
    typedef struct MemoryMap memorymap;
    UART_HandleTypeDef huart2;

    BLE();
    void initUART();
    void initPeripherials();
    MemoryMap* initBLEMemoryMap();
    void send05hz(MemoryMap* map);
    void send50hz(MemoryMap* map);
    void sendSpecialCommand(MemoryMap* map);
    MemoryMap* BLERead( MemoryMap* map);
};


#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization


