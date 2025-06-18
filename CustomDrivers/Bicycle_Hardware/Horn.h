/**
  ******************************************************************************
  * @file           : Horn.h
  * @brief          : Header for Horn.c file.
  *                   This file contains includes for Horn.c.
  * */

 //prevent recursive inclusion
#ifndef __HORN
#define __HORN


#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "main.h"
#include "SwitchActivatedDevice.h"

class Horn : public SwitchActivatedDevice{
    public:
    Horn(GPIO_TypeDef* GPIOxIn, 
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

