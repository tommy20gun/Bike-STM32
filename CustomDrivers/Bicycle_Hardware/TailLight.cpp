 /** 
  * @file           : Taillight.c
  * @brief          : Implementation of driver for Tailight control.
  * Functionality:
  * Headlight will always be on low brightness on default
  * Braking will change the light to full brightness. Rising Edge GPIO EXTI will implement changing of brightness. 
  * Turn signal will blink the specfied light at full brightness. Timed GPIO Output will implement the blinking of TailLights. Trigger will be implemented by Rising edge GPIO EXTI
  * 
  * Input Pin for EXTI PB6(left), PB7(right), PA9 (brake). TODO these need to be 12V with transistor
  * Output Pin for turn signal transistor PB8 (left), PB9 (right), PB2 (brake)
  * 
  */
#include <Taillight.h>

TaskHandle_t Taillight::vTurnLeftHandle = nullptr;
TaskHandle_t Taillight::vTurnRightHandle = nullptr;
TaskHandle_t Taillight::vBrakeHandle = nullptr;

Taillight::Taillight(){

}

void Taillight:: initTasks(){
  BaseType_t xReturned;
  vTurnLeftHandle = nullptr;
  xReturned = xTaskCreate(vTurnLeft,"TurnLeft On/off",64, this,2,&vTurnLeftHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }

  xReturned = xTaskCreate(vTurnRight,"TurnRight On/off",64, this,2,&vTurnRightHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }

  xReturned = xTaskCreate(vBrake,"Brake On/off",64,this,2,&vBrakeHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }
}

void Taillight::initPeripherals(){
  //TODO test the change pin setup
  //Peripheral GPIO
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  // PB8 (left), PB9 (right), PB2 (brake) as output
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9 | LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  //PB6(left), PB7(right), PA9 (brake)
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_INPUT);
}

void Taillight::vTurnLeft(void* pvParameters){
  Taillight* taillight = (Taillight*) pvParameters;
  struct uint32_t_Buffer buff;
  buff.tag = turningLeft;
  buff.data = 0;
  bool inPinState;
  while(1){
    inPinState = LL_GPIO_IsInputPinSet(GPIOB,GPIO_PIN_6);
    vTaskDelay(50);
    while(inPinState == true && LL_GPIO_IsInputPinSet(GPIOB,GPIO_PIN_6) == inPinState){
      LL_GPIO_TogglePin(GPIOB,GPIO_PIN_8);
      buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_8);
      xQueueSendToBack(taillight->messenger,  &buff , 0);//TODO this will not broadcast light off
      vTaskDelay(1000);
    }
  }
}
void Taillight::vTurnRight(void* pvParameters){
  Taillight* taillight = (Taillight*) pvParameters;
  struct uint32_t_Buffer buff;
  buff.tag = turningRight;
  buff.data = 0;
  bool inPinState;
  while(1){
    inPinState = LL_GPIO_IsInputPinSet(GPIOB,GPIO_PIN_7);
    vTaskDelay(50);
    while(inPinState == true && LL_GPIO_IsInputPinSet(GPIOB,GPIO_PIN_7) == inPinState){
      LL_GPIO_TogglePin(GPIOB,GPIO_PIN_9);
      buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_9);
      xQueueSendToBack(taillight->messenger,  &buff , 0);//TODO this will not broadcast light off
      vTaskDelay(1000);
    }
  }
}
void Taillight::vBrake(void* pvParameters){ //TODO make this a generic function,
  //TODO make the parent class work by adding pin designation
  Taillight* taillight = (Taillight*) pvParameters;
  bool inPinState;
  bool prevPinState = LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_9);
  struct uint32_t_Buffer buff;
  buff.tag = brakeON;
  buff.data = 0;
  while(1){
    inPinState = LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_9);
    vTaskDelay(100);
    if (inPinState == LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_9) && inPinState != prevPinState){
      if (inPinState){
        LL_GPIO_SetOutputPin(GPIOB,GPIO_PIN_2);
        buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_2);
        xQueueSendToBack(taillight->messenger,  &buff , 0);
        prevPinState = inPinState;
      }
      else if (!inPinState){
        LL_GPIO_ResetOutputPin(GPIOB,GPIO_PIN_2);
        buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_2);
        xQueueSendToBack(taillight->messenger,  &buff , 0);
        prevPinState = inPinState;
      }
    }
  }
}
    

    