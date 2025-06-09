 /** 
  * @file           : Headlight.c
  * @brief          : Implementation of driver for Headlight control. 
  * Headlight uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console
  * Input pin for EXTI is PA10
  * Output pin to open transistor PB0 
  */

#include <Headlight.h>
TaskHandle_t Headlight::vTurnOnHeadlightHandle = nullptr;

Headlight::Headlight(){

}

void Headlight::initTasks(){
  BaseType_t xReturned;
  xReturned = xTaskCreate(vTurnonHeadlight,"Headlight On/off",64, this,2,&vTurnOnHeadlightHandle);
  if (xReturned != pdPASS){
    Error_Handler(__FILE__,__LINE__);
  }
  //Binary Semaphore used for ISR to turn on the headlight
  /*this->bsem = xSemaphoreCreateBinary();
  if(this->bsem == NULL){
    Error_Handler(__FILE__,__LINE__);
  }*/
}

void Headlight::initPeripherals(){
  ErrorStatus success;
  //Peripheral GPIO
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  //init PB0
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  success = LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  if (success != 0){
    Error_Handler(__FILE__,__LINE__);
  }
  //init PA11 as input wiht pulldown for the circuit
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  success =  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  if (success != 0){
    Error_Handler(__FILE__,__LINE__);
  }
  /*LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE10);

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_10;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  success =  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  LL_EXTI_Init(&EXTI_InitStruct);
  if (success != 0){
    Error_Handler(__FILE__,__LINE__);
  }
    */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);
}

void Headlight::vTurnonHeadlight(void* pvParameters){
  bool inPinState;
  bool prevPinState = LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_10);
  struct uint32_t_Buffer buff;
  buff.tag = headlightON;
  buff.data = 0;
  Headlight* headlight = (Headlight*) pvParameters;
  while(1){
    //subtracts semaphore back down to 0, next while loop will block again
    //xSemaphoreTake(headlight->bsem,portMAX_DELAY);
    //detects the rising or falling edge of the input pin
    //allows the switch to have on/off function
    inPinState = LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_10);
    vTaskDelay(100);

    if (inPinState == LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_10) && inPinState != prevPinState){
      if (inPinState){  
        LL_GPIO_SetOutputPin(GPIOB,GPIO_PIN_0);
        buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_0);
        xQueueSendToBack(headlight->messenger, &buff , 0);
        prevPinState = inPinState;
      }
      else if (!inPinState){
        LL_GPIO_ResetOutputPin(GPIOB,GPIO_PIN_0);
        buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_0);
        xQueueSendToBack(headlight->messenger, &buff , 0);
        prevPinState = inPinState;
      }
    }
  } 
}




