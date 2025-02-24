/**
  ******************************************************************************
  *
  * @brief          : Header for BLESend.c
  *                 This file contains includes for BLESend.c
  * */

/**
  ******************************************************************************
  * @file           : Headlight.h
  * @brief          : Header for Headlight.c
  *                 This file contains includes for Headlight.c
  * */


//prevent recursive inclusion
#ifndef __BLESEND
#define __BLESEND

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "main.h"



#ifdef __cplusplus
}
#endif //__cplusplus

class BLE{
  public:
    typedef struct MemoryMap memorymap;
    UART_HandleTypeDef huart2;

    BLE();
    MemoryMap* initBLEMemoryMap(MemoryMap* map);
    void send05hz(MemoryMap* map);
    void send50hz(MemoryMap* map);
    void sendSpecialCommand(MemoryMap* map);
    MemoryMap* BLERead( MemoryMap* map);
};



#endif //__Peripheral_initialization


