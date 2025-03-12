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
    ADCDriver(uint16_t readingpin);
    void init(float maxVoltage);

    private:
};

#ifdef __cplusplus
}
#endif //__cplusplus




#endif //__Peripheral_initialization


