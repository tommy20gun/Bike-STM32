 /** 
  * @file           : Taillight.c
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
#include <Taillight.h>

TaskHandle_t Taillight::vTurnLeftHandle = nullptr;
TaskHandle_t Taillight::vTurnRightHandle = nullptr;
TaskHandle_t Taillight::vBrakeHandle = nullptr;

Taillight::Taillight(){

}

void Taillight:: initTasks(){
  BaseType_t xReturned;
  vTurnLeftHandle = nullptr;
  xReturned = xTaskCreate(vTurnLeft,"TurnLeft On/off",64, this,1,&vTurnLeftHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }
  //Binary Semaphore used for ISR to turn on the headlight
  this->bsemleft = xSemaphoreCreateBinary();
  if(this->bsemleft == NULL){
    Error_Handler();
  } 

  xReturned = xTaskCreate(vTurnRight,"TurnRight On/off",64, this,1,&vTurnRightHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }
  //Binary Semaphore used for ISR to turn on the headlight
  this->bsemright = xSemaphoreCreateBinary();
  if(this->bsemright == NULL){
    Error_Handler();
  }

  xReturned = xTaskCreate(vBrake,"Brake On/off",64,this,1,&vBrakeHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }
  //Binary Semaphore used for ISR to turn on the headlight
  this->bsembrake = xSemaphoreCreateBinary();
  if(this->bsembrake == NULL){
    Error_Handler();
  } 
}

void Taillight::initPeripherals(){

  //Peripheral GPIO
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  //init PB8,PB9,PB2
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  //init PA8,PB9,PB2 as input wiht pulldown for the circuit
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE8);
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE9);
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE12);

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_8|LL_EXTI_LINE_9|LL_EXTI_LINE_12;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_12, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_INPUT);
}

void Taillight::vTurnLeft(void* pvParameters){
  Taillight* taillight = (Taillight*) pvParameters;
  struct taggedBuffer buff;
  buff.tag = turningLeft;
  while(1){
    xSemaphoreTake(taillight->bsemleft,portMAX_DELAY);
    while(xSemaphoreTake(taillight->bsemleft,0)== pdFALSE && LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_8)){
    LL_GPIO_TogglePin(GPIOB,GPIO_PIN_8);
    buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_8);
    xQueueSendToBack(taillight->messenger,  &buff , 0);
    vTaskDelay(1000);
    }
  }
}
void Taillight::vTurnRight(void* pvParameters){
  Taillight* taillight = (Taillight*) pvParameters;
  struct taggedBuffer buff;
  buff.tag = turningRight;
  while(1){
    xSemaphoreTake(taillight->bsemright,portMAX_DELAY);
    while(xSemaphoreTake(taillight->bsemright,0)== pdFALSE && LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_9)){
      LL_GPIO_TogglePin(GPIOB,GPIO_PIN_9);
      buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_9);
      xQueueSendToBack(taillight->messenger,  &buff , 0);
      vTaskDelay(1000);
    }
  }
}
void Taillight::vBrake(void* pvParameters){
  Taillight* taillight = (Taillight*) pvParameters;
  bool inPinState;
  struct taggedBuffer buff;
  buff.tag = brakeON;
  while(1){
    //subtracts semaphore back down to 0, next while loop will block again
    xSemaphoreTake(taillight->bsembrake,portMAX_DELAY);
    //detects the rising or falling edge of the input pin
    //allows the switch to have on/off function
    inPinState = LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_12);
    if (inPinState){
      LL_GPIO_SetOutputPin(GPIOB,GPIO_PIN_2);
      buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_2);
      xQueueSendToBack(taillight->messenger,  &buff , 0);
    }
    else if (!inPinState){
      LL_GPIO_ResetOutputPin(GPIOB,GPIO_PIN_2);
      buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_2);
      xQueueSendToBack(taillight->messenger,  &buff , 0);
    }
  }
}
    

    