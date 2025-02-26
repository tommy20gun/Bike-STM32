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

class Bluetooth{
  public:
    typedef struct MemoryMap{
      bool* headlightON;
      
      double* motorTemp;
      double* current;
      double* ADCreading72V;
      double* ADCreading12V;
      double* BattTemp;
      struct PackVoltage{

      };
      double* Odometer;

    }MemoryMap;

    typedef int mode;
    mode mode;
    typedef bool BTState;
    BTState state;
    
    UART_HandleTypeDef huart2;
    char BLEAddr[14];
     

    Bluetooth();
    
    void initPeripherials();
    BLEState getConnectionState();
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


