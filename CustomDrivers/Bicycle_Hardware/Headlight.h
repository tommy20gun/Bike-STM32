/**
  ******************************************************************************
  * @file           : Headlight.h
  * @brief          : Header for Headlight.c
  *                 This file contains includes for Headlight.c
  * */


//prevent recursive inclusion
#ifndef __HEADLIGHT
#define __HEADLIGHT

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "main.h"
#include "MemoryMap.h"
#include "SwitchActivatedDevice.h"


class Headlight : public SwitchActivatedDevice{
    public:
    Headlight(GPIO_TypeDef* GPIOxIn, 
            uint32_t PinMaskIn, 
            GPIO_TypeDef* GPIOxOut, 
            uint32_t PinMaskOut, 
            QueueHandle_t messenger,
            DataTable queueTag):SwitchActivatedDevice(GPIOxIn,
                                                            PinMaskIn,
                                                            GPIOxOut,
                                                            PinMaskOut,
                                                            messenger,
                                                            queueTag)
    {
        initPeripherals();
        initTasks();
    };
    protected:
    void initTasks();
    static void vTaskFunction(void* pvParameters);//this is just 1 line FreeRTOSImplementation()
};

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


