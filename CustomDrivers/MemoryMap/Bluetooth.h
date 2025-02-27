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
#include "MemoryMap.h"
#include "stm32f4xx_ll_usart.h"

#define DATA_MODE 0
#define COMMAND_MODE 1

#define BLEConnected 1
#define BLEDisconnected 0

class Bluetooth{
  public:


    typedef int mode;
    mode mode;
    typedef bool BTState;
    BTState state;
    
    char BLEAddr[14];
     

    Bluetooth();
    
    void initPeripherials();
    BTState getConnectionState();
    MemoryMap* initBTMemoryMap();
    MemoryMap* BTRead( MemoryMap* map);

    //FreeRTOS tasks
    static void send05hz(MemoryMap* map);
    static void send50hz(MemoryMap* map);
    static void sendSpecialCommand(MemoryMap* map);

    

    private:
    void mode(int mode);
};


#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization


