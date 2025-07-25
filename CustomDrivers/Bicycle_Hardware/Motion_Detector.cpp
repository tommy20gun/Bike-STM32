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
  * Settings on breakoutboard:
  * Repeat Trigger
  * 3 second delay
  * 3 meters
  * 
  * Logic:
  * motion is detected for 5 seconds continuously: turn on all lights
  * motion is continually detected ~30 seconds honk the horn for 2 clicks every5 seconds
  * until motion is gone
  * 
  * on the activation or resume of a task, wait 60 seconds to initialize.
  * make sure the detector is able to signal when initialization is complete
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
void Motion_Detector::QueueSend(){};//TODO I will impelemnt this later, it depends on how i want to send data

void Motion_Detector::vTaskFunction(void* pvParameters){
    Motion_Detector* detector = (Motion_Detector*) pvParameters;
    detector->RTOSImplementation();
}

void Motion_Detector::RTOSImplementation(){
    /*
    account for standby phase of 1 minute on startup
  * Pin going low is "detected motion"
  * there is a delay of 3 seconds (blocked after the pin going back HIGH)
  * THere is an adjustable time delay during the last time motion is detected BEFORE pin going high again. 
  * 
  * interrupt on falling edge, input pin polling until back to high, 
  * 
  * logic:
  * light is off
motion detector triggers
light turns on

There is motion
if light is off, then motion occurs, turn light on
if light is on from the switch, then motion occurs, nothing happens

There is no more motion
no more motion, headlight is on due to motion detector = turn off headlight
no more motion, headlight is on from light switch = turn off headlight
no more motion, headlight is off due to light switch = nothing
  * */
  vTaskDelay(63000); // wait 1 minute for startup. The sensor should calibrate during this time so everytime
  // it becomes active (unlock->lock) state, it should do this calibration.
  struct uint32_t_Buffer buff;
  buff.tag = headlightON;
  //bool previouspin = true;
  uint64_t timeON = 0;
  while(1){
    //executes when motion is detected
    while (LL_GPIO_IsInputPinSet(readingPin.GPIOx,readingPin.PinMask) == true){
      timeON +=30;
      if (timeON >= 5000 && LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_0)){
        LL_GPIO_SetOutputPin(GPIOB,GPIO_PIN_0);
        buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_0);
        xQueueSendToBack(messenger, &buff , 0);
        //previouspin = true;
      }
      //executes on falling edge only
      else if(LL_GPIO_IsInputPinSet(readingPin.GPIOx,readingPin.PinMask) == false /*&& previouspin == true*/){
        LL_GPIO_ResetOutputPin(GPIOB,GPIO_PIN_0);
        buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_0);
        xQueueSendToBack(messenger, &buff , 0);
        //previouspin = false;
        timeON = 0;
        vTaskDelay(3000); //lockout is 3sec
        break;
      }
      else{
        vTaskDelay(30);
      }
      vTaskDelay(100);
    }
  }
}


