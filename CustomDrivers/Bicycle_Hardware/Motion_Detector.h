  /******************************************************************************
  * 
  * @brief          : Header for Motion_Detector.c
  *                 This file contains includes for Motion_Detector.c
  * */

 //prevent recursive inclusion
#ifndef __MOTION_DETECTION
#define __MOTION_DETECTION

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h" 
#include "Device.h"
class Motion_Detector : public Device{
  public:
  Motion_Detector(
        QueueHandle_t messenger,
        DataTable queueTag,
        GPIO_TypeDef* GPIOxReadingPin, 
        uint32_t PinMaskReadingPin)
{
    readingPin.GPIOx = GPIOxReadingPin;
    readingPin.PinMask = PinMaskReadingPin;
    this->messenger = messenger;
    this->queueTag = queueTag;
    initPeripherals();
    initTasks();
};
  protected:
  GPIOPin_Struct readingPin;
  void initTasks();
  void initPeripherals();
  static void vTaskFunction(void* pvParameters);//this is just 1 line FreeRTOSImplementation()
  void RTOSImplementation();
  
};

#ifdef __cplusplus
}
#endif //__cplusplus



#endif //__Peripheral_initialization

