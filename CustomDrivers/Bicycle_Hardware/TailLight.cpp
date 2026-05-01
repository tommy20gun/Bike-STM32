 /** 
  * @file           : Taillight.c
  * @brief          : Implementation of driver for Tailight control.
  * Functionality:
  * Headlight will always be on low brightness on default
  * Braking will change the light to full brightness. Rising Edge GPIO EXTI will implement changing of brightness. 
  * Turn signal will blink the specfied light at full brightness. Timed GPIO Output will implement the blinking of TailLights. Trigger will be implemented by Rising edge GPIO EXTI
  * 
  * Input Pin PB6(left), PB7(right), PA9 (brake). TODO these need to be 12V with transistor
  * Output Pin for turn signal transistor PB8 (left), PB9 (right), PA5 (brake), PB10 (Turnleft bright), PB12 (turnright bright)
  * 
  */
#include <Taillight.h>

void TailLight_Turn::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"TailLight On/Off",64, this,2,&TaskHandle);
    if(xReturned != pdPASS){
        Error_Handler();
    }
}

void TailLight_Turn::initPeripherals(){
    initInputPin(inputPin, true);
    initOutputPin(outputPin, true);
    initOutputPin(brightnessPin,true);
}

void TailLight_Turn::vTaskFunction(void* pvParameters){
  TailLight_Turn* taillightturn = (TailLight_Turn*) pvParameters;
  taillightturn->RTOSImplementation();
}

void TailLight_Turn::RTOSImplementation(){
  //manually turn on taillight at startup
  LL_GPIO_ResetOutputPin(outputPin.GPIOx, outputPin.PinMask);
  bool inPinState;
  while(1)
  {
    inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx, inputPin.PinMask);
    vTaskDelay(50);
    while(inPinState == true && LL_GPIO_IsInputPinSet(inputPin.GPIOx, inputPin.PinMask) == inPinState)
    {
      LL_GPIO_TogglePin(outputPin.GPIOx, outputPin.PinMask);
      LL_GPIO_ResetOutputPin(brightnessPin.GPIOx, brightnessPin.PinMask); //pull to ground, opens
      QueueSend();
      vTaskDelay(500);
    }
    LL_GPIO_SetOutputPin(outputPin.GPIOx, outputPin.PinMask);
    LL_GPIO_SetOutputPin(brightnessPin.GPIOx, brightnessPin.PinMask);
    QueueSend();
  }
}

void TailLight_Brake::initTasks(){
  BaseType_t xReturned;
  xReturned = xTaskCreate(vTaskFunction,"TailLight Brake On/Off",64, this,2,&TaskHandle);
  if(xReturned != pdPASS){
      Error_Handler();
  }
}

void TailLight_Brake::vTaskFunction(void* pvParameters){
  TailLight_Brake* taillightbrake = (TailLight_Brake*) pvParameters;
  taillightbrake->RTOSImplementation();
} 

void TailLight_Brake::RTOSImplementation(){
  initializeHWPinState();
  bool inPinState;
  bool prevPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);

  while(1){
    inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);
    //debounce
    vTaskDelay(50);
    //read again to confirm the signal, if not, ignore it
    if (inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask) && inPinState != prevPinState){
      if (inPinState){
        LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask); 
        LL_GPIO_SetOutputPin(GPIOA,GPIO_PIN_6); // Brake activated throttle
        QueueSend();
        prevPinState = inPinState;
      }
      else if (!inPinState){
        LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask);
        LL_GPIO_ResetOutputPin(GPIOA,GPIO_PIN_6); // Brake activated throttle
        QueueSend();
        prevPinState = inPinState;
      }
    }
  }
}