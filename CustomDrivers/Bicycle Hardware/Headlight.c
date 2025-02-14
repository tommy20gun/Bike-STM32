 /** 
  * @file           : Headlight.c
  * @brief          : Implementation of driver for Headlight control. 
  * Headlight uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console
  * Input pin for EXTI is PA10
  * Output pin to open transistor PB0 
  */

 #include <Headlight.h>