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
#include "MemoryMap.h"

class ADCDriver{
    public:
    TaskHandle_t vADCPollHandle;
    QueueHandle_t messenger;

    int pin4V;
    int pin5V;
    int pinreading[2];
    ADCDriver(int maxvoltage[]);
    void init();
    static void vADCPoll(void* pvParameters);

    private:
    static float ADCToBatteryPercent(uint16_t ADCReading,int maxvoltage);
    void sendDataThroughQueue();
};

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


