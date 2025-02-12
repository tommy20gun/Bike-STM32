 /** 
  * @file           : TailLight.c
  * @brief          : Implementation of driver for Tailight control.
  * Functionality:
  * Headlight will always be on low brightness on default
  * Braking will change the light to full brightness. Rising Edge GPIO EXTI will implement changing of brightness. 
  * Turn signal will blink the specfied light at full brightness. Timed GPIO Output will implement the blinking of TailLights. Trigger will be implemented by Rising edge GPIO EXTI
  * @attention
  */
 #include <TailLight.h>
 