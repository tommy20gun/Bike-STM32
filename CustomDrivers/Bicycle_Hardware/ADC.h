/**
  ******************************************************************************
  * @file           : Headlight.h
  * @brief          : Header for Headlight.c
  *                 This file contains includes for Headlight.c
  * */


//prevent recursive inclusion
#ifndef __ADC
#define __ADC

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "main.h"
#include "Device.h"

class ADCDriver: public Device{
    public:
    ADCDriver(QueueHandle_t messenger, GPIO_TypeDef* GPIOxADC, uint32_t PinMaskIn);

    protected:
    GPIOPin_Struct ADCPin;
    //int pin4voltage;
    float pinreading;

    void initTasks();
    virtual void initPeripherals();
    static void vTaskFunction(void* pvParameters);
    void RTOSImplementation();
    static float ADCToBatteryPercent(float ADCReading,float scale);
    void QueueSend();
};

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


