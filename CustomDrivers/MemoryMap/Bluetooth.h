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
#include "main.h"
//#include "MemoryMap.h"
#include "stm32f4xx_ll_usart.h"

#define DATA_MODE 0
#define COMMAND_MODE 1

#define BLEConnected 1
#define BLEDisconnected 0

class Bluetooth{
  public:
    typedef struct MemoryMap{
      //slow data
      bool headlightON;
      double motorTemp;
      double ADCreading72V;
      double ADCreading12V;
      double battTemp;
      double Odometer;
      //fast data
      double speed;
      bool hornON;
      bool brakeON;
      bool turningLeft;
      bool turningRight;
      int RPM;
      double throttleV;

      //special command
      /*
      state_t* locked;
      state_t* unlocked;
      bool* BMSMOS_On;
      */
    }MemoryMap;

    MemoryMap map;

    TaskHandle_t sendTestHandle;

    typedef bool BTMode; 
    BTMode mode;
    typedef bool BTState; 
    BTState state;
    
    char BLEAddr[14];
     

    Bluetooth();
    
    void initPeripherials();
    void initTasks();
    BTState getConnectionState();
    void initBTMemoryMap();
    MemoryMap* BTRead( MemoryMap* map);
    

    //FreeRTOS tasks
    static void send05hz(void* pvParameters);
    static void send50hz(void* pvParameters);
    static void sendSpecialCommand(void* pvParameters);
    static void sendTest(void* pvParameters);
    

    private:
    void setmode(int mode);
    void ATModeTesting();
};

void DMA1_Stream6_IRQHandler(void);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization


