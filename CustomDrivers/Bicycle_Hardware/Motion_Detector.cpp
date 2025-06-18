 /** 
  * 
  * @brief          : Driver to control Motion Detector
  * 
  * Driver to control Motion Detector circuit. 
  * Implementation of the detection threshold algorithm
  * 
  * GPIO EXTI 1- PB13
  * GPIO EXTI 2- PB14
  * GPIO EXTI 3- PB15
  * 
  * account for standby phase of 1 minute on startup
  * Pin going low is "detected motion"
  * there is a delay of 3 seconds (blocked after the pin going back HIGH)
  * THere is an adjustable time delay during the last time motion is detected BEFORE pin going high again. 
  * 
  * interrupt on falling edge, input pin polling until back to high, 
  * Settings on breakoutboard:
  * Repeat Trigger
  * 3 second delay
  * 3 meters
  *
  */
#include "Motion_Detector.h"
void Motion_Detector::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"Motion Detector",64, this,2,&TaskHandle);
    if(xReturned != pdPASS){
        Error_Handler();
    }
}

void Motion_Detector::initPeripherals(){
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = readingPin.PinMask;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(readingPin.GPIOx, &GPIO_InitStruct);

}

void Motion_Detector::vTaskFunction(void* pvParameters){
    Motion_Detector* detector = (Motion_Detector*) pvParameters;
    detector->RTOSImplementation();
}

void Motion_Detector::RTOSImplementation(){
    
}