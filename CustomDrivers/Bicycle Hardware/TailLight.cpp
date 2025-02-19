 /** 
  * @file           : TailLight.c
  * @brief          : Implementation of driver for Tailight control.
  * Functionality:
  * Headlight will always be on low brightness on default
  * Braking will change the light to full brightness. Rising Edge GPIO EXTI will implement changing of brightness. 
  * Turn signal will blink the specfied light at full brightness. Timed GPIO Output will implement the blinking of TailLights. Trigger will be implemented by Rising edge GPIO EXTI
  * 
  * Input Pin for EXTI PA8(left), PA9(right), PA12 (brake). TODO these need to be 12V with transistor
  * Output Pin for turn signal transistor PB8 (left), PB9 (right), PB2 (brake)
  * 
  */
#include <TailLight.h>
 
class TailLight{
    public:
    SemaphoreHandle_t bsemaphore;
    TaskHandle_t vTurnOnTailLightHandle = NULL;
    TailLight


    void vTurnLeft(void* pvParameters);
    void vTurnRight(void* pvParameters);
    void vBrake(void* pvParameters);
    
}
    