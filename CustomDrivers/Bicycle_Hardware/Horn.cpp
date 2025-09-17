 /** 
  * @file           : Horn.c
  * @brief          : Implementation of driver for Horn control. 
  * 
  * The Horn uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console.
  * Input pin for EXTI is PA8
  * Output pin to open transistor PB1 
  */

#include <Horn.h>

void Horn::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"Horn On/Off",64, this,2,&TaskHandle);
    if(xReturned != pdPASS){
        Error_Handler();
    }
}

void Horn::vTaskFunction(void* pvParameters){
  Horn* horn = (Horn*) pvParameters;
  horn->RTOSImplementation();
}

void Horn::RTOSImplementation(){
  bool inPinState;
  bool prevPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);

  while(1){
    inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);
    //debounce
    vTaskDelay(15);
    //read again to confirm the signal, if not, ignore it
    if (inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask) && inPinState != prevPinState){
      if (inPinState){
        LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask); //TODO make this a mutex for motion detect
        QueueSend();
        prevPinState = inPinState;
      }
      else if (!inPinState){
        LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask);
        QueueSend();
        prevPinState = inPinState;
      }
    }
  }
}
