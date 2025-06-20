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
    ADCDriver(QueueHandle_t messenger, int pin4voltage, int pin5voltage){
      this->messenger = messenger;
      this->pin4voltage = pin4voltage;
      this->pin5voltage = pin5voltage; 
    };

    private:
    int pin4voltage;
    int pin5voltage;
    int pinreading[2];

    void initTasks();
    void initPeripherals();
    static void vTaskFunction(void* pvParameters);
    void RTOSImplementation();
    static float ADCToBatteryPercent(float ADCReading,float scale);
    void QueueSend();
};

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


