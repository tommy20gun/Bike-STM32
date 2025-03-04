 /** 
  * @file           : Horn.c
  * @brief          : Implementation of driver for Horn control. 
  * 
  * The Horn uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console.
  * Input pin for EXTI is PA11
  * Output pin to open transistor PB1 
  */

#include <Horn.h>
TaskHandle_t Horn::vTurnOnHornHandle = nullptr;

Horn::Horn(){

}

void Horn::initTasks(){
  BaseType_t xReturned;
  xReturned = xTaskCreate(vTurnOnHorn,"horn On/Off",64, this,1,&vTurnOnHornHandle);
  if(xReturned != pdPASS){
    Error_Handler();
  }
  //Binary Semaphore used for ISR to turn on the headlight
  this->bsem = xSemaphoreCreateBinary();
  if(this->bsem == NULL){
    Error_Handler();
  }
}

void Horn::initPeripherals(){
    //Peripheral GPIO
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    //init PB1
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
    //init PA11 as input wiht pulldown for the circuit
    GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE11);
  
    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_11;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
    LL_EXTI_Init(&EXTI_InitStruct);
  
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_11, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_INPUT);
}

void Horn::vTurnOnHorn(void* pvParameters){
  Horn *horn = (Horn*) pvParameters;
  bool inPinState;
  struct taggedBuffer buff;
  buff.tag = hornON;
  while(1){
    //subtracts semaphore back down to 0, next while loop will block again
    xSemaphoreTake(horn->bsem,portMAX_DELAY);
    //detects the rising or falling edge of the input pin
    //allows the switch to have on/off function
    inPinState = LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_11);
    if (inPinState){
      LL_GPIO_SetOutputPin(GPIOB,GPIO_PIN_1);
      buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_1);
      xQueueSendToBack(horn->messenger, &buff , 0);
    }
    else if (!inPinState){
      LL_GPIO_ResetOutputPin(GPIOB,GPIO_PIN_1);
      buff.data = LL_GPIO_IsOutputPinSet(GPIOB,GPIO_PIN_1);
      xQueueSendToBack(horn->messenger, &buff , 0);
    }
  }
}
