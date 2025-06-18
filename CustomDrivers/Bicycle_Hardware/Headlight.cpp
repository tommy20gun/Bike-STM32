 /** 
  * @file           : Headlight.c
  * @brief          : Implementation of driver for Headlight control. 
  * Headlight uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console
  * Input pin for EXTI is PA10
  * Output pin to open transistor PB0 
  */

#include <Headlight.h>

void Headlight::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"Headlight On/Off",64, this,2,&TaskHandle);
    if(xReturned != pdPASS){
        Error_Handler();
    }
}

void Headlight::vTaskFunction(void* pvParameters){
  Headlight* headlight = (Headlight*) pvParameters;
  headlight->RTOSImplementation();
}
