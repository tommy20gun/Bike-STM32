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
//#include "MemoryMap.h"

#define DATA_MODE 0
#define COMMAND_MODE 1

#define BLEConnected 1
#define BLEDisconnected 0
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
typedef bool BTState; 
typedef bool BTMode; 
class Bluetooth{
  public:
    MemoryMap map;

    TaskHandle_t sendSlowHandle;
    TaskHandle_t sendFastHandle;
    TaskHandle_t sendSpecialCommandHandle;
    QueueHandle_t messenger;

    SemaphoreHandle_t sendSemaphore;
    BTMode mode;
    BTState state;
    Bluetooth();
    
    void initPeripherials();
    void initTasks();
    BTState getConnectionState();
    void initBTMemoryMap();
    

    //FreeRTOS tasks
    static void sendSlow(void* pvParameters);
    static void sendFast(void* pvParameters);
    static void sendSpecialCommand(void* pvParameters);
    
    

    private:
    void setmode(int mode);
    void ATModeTesting();
    static void uartTransmitDMA(uint32_t srcAddr, uint32_t size);
};

void DMA1_Stream6_IRQHandler(void);
#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__Peripheral_initialization


