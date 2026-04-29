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
    uint16_t raw_adc_read;

    void initTasks();
    virtual void initPeripherals();
    static void vTaskFunction(void* pvParameters);
    void RTOSImplementation();
    void QueueSend();

    //helpers
    static float ADCToBatteryPercent(float voltagePerCell);
    static float raw2Voltage(uint16_t raw, float scale);


  };

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


