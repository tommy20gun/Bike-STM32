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
#include "SwitchActivatedDevice.h"

class Lock: public SwitchActivatedDevice{
  public:
  Lock(GPIO_TypeDef* GPIOxIn, 
      uint32_t PinMaskIn, 
      GPIO_TypeDef* GPIOxOut, 
      uint32_t PinMaskOut, 
      QueueHandle_t messenger,
      DataTable queueTag,
      TaskHandle_t statetask):SwitchActivatedDevice(GPIOxIn,
                                                      PinMaskIn,
                                                      GPIOxOut,
                                                      PinMaskOut,
                                                      messenger,
                                                      queueTag)
    {
        this->stateMachineHandle = statetask;
        initPeripherals();
        initTasks();
    };
    
    TaskHandle_t stateMachineHandle;
    void initPeripherals();
    void initTasks();

    static void vTaskFunction(void* pvParameters);
    void RTOSImplementation();

};

#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization

