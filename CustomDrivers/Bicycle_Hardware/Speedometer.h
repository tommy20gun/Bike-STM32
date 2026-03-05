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
#define WHEEL_RADIUS (6u) //inches

class Speedometer: public Device{
  public:
  Speedometer(QueueHandle_t messenger, GPIO_TypeDef* GPIOxADC, uint32_t PinMaskIn);
  
  protected:
  int32_t RPM; //signed int
  float mph;
  uint32_t milis_elapsed;



  void initTasks();
  virtual void initPeripherals();
  static void vTaskFunction(void* pvParameters);
  void RTOSImplementation();
  void QueueSend();

  //helpers
  static float RPM2mph();

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

}
#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization

