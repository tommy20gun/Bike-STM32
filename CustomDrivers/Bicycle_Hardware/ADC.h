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
    int pin4V;
    int pin5V;
    ADCDriver(int maxvoltage[]);
    void init();
    static void vADCPoll(void* pvParameters);

};

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


