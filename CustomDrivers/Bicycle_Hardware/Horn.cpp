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
  initializeHWPinState();
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
        LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask);  //Slight delay to allow the readpin to work
        prevPinState = inPinState;
        QueueSend();
      }
      else if (!inPinState){
        LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask);
        LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask); //Slight delay to allow the readpin to work
        prevPinState = inPinState;
        QueueSend();
      }
    }
  }
}

void ShortHorn::RTOSImplementation(){
  initializeHWPinState();
  bool inPinState;
  bool prevPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);

  while(1){
    inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);
    //debounce
    vTaskDelay(30);
    //read again to confirm the signal, if not, ignore it
    if (inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask) && inPinState != prevPinState){
      if (inPinState){
        shortHonk();          
        prevPinState = inPinState;
      }
      else if (!inPinState){
        prevPinState = inPinState;
      }
    }
  }
}

void ShortHorn::shortHonk(){
  LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask);
  vTaskDelay(20);
  LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask);
  vTaskDelay(80);
  LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask);
  vTaskDelay(20);
  LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask);
  vTaskDelay(80);
}

