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

#define DATA_MODE 0
#define COMMAND_MODE 1

#define BLEConnected 1
#define BLEDisconnected 0

class BLE{
  public:
    typedef struct MemoryMap memorymap;
    typedef int Mode;
    typedef bool BLEState;
    
    UART_HandleTypeDef huart2;
    char BLEAddr[14];
     

    BLE();
    
    void initPeripherials();
    void initHC05();
    BLEState getConnectionState();
    MemoryMap* initBLEMemoryMap();
    void send05hz(MemoryMap* map);
    void send50hz(MemoryMap* map);
    void sendSpecialCommand(MemoryMap* map);
    MemoryMap* BLERead( MemoryMap* map);

    private:
    void mode(int mode);
};


#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization


