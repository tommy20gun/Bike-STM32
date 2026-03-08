/**
  ******************************************************************************
  * @brief          : Header for Odometer.h
  *                 This file contains includes for Odometer.h
  * 
  * */

 //prevent recursive inclusion
#ifndef __ODOMETER
#define __ODOMETER

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"  
#include "Device.h"
#include "main.h"



class Speedometer: public Device{
  public:
  Speedometer(QueueHandle_t messenger, GPIO_TypeDef* GPIOx, uint32_t PinMaskIn);
  
  protected:
  GPIOPin_Struct inputPin;
  float mph;

  void initTasks();
  virtual void initPeripherals();
  static void vTaskFunction(void* pvParameters);
  void RTOSImplementation();
  void QueueSend();

  //helpers
  static float RPM2mph(float smoothed_rpm);
  static float calculateRPM(uint32_t milis_elapsed);
  static float getAverage(float* buffer, uint8_t count);

  //isr checkout caabd33 and previous
  /* 
  1. set up interrupt to trigger at hall sensor pin
  2. inside ISR, awaken a higher priority task

  //RTOS implementation
  3. task should first debounce the signal (or maybe it shouldnt)
  4. grab the time, grab the last time
  5. apply timing logic and convert into RPM and speed
  6. once task finishes, rid semaphore
  */

};

void EXTI15_10_IRQHandler(void);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__Peripheral_initialization

