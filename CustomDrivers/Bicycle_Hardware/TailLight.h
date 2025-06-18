 /** 
  * @file           : Tailight.h
  * @brief          : Header for TailLight.c. 
  * 
  * This file contains includes for Horn.c.
  */

//prevent recursive inclusion
#ifndef __TAILLIGHT
#define __TAILLIGHT

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h" 
#include "main.h"
#include "SwitchActivatedDevice.h"

class TailLight_Turn : public SwitchActivatedDevice{
    public:
    TailLight_Turn(GPIO_TypeDef* GPIOxIn, 
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
    void RTOSImplementation();
};


class TailLight_Brake : public SwitchActivatedDevice{
    public:
    TailLight_Brake(GPIO_TypeDef* GPIOxIn, 
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

