 /** 
  * @file           : Headlight.c
  * @brief          : Implementation of driver for Headlight control. 
  * Headlight uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console
  * Input pin for EXTI is PA10
  * Output pin to open transistor PB0 
  */

#include <Headlight.h>
  
Headlight::Headlight(){
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
    Error_Handler();
  }
  //init PA11 as input wiht pulldown for the circuit
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  success =  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  if (success != 0){
    Error_Handler();
  }
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE10);

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_10;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  success =  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  LL_EXTI_Init(&EXTI_InitStruct);
  if (success != 0){
    Error_Handler();
  }
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);

}

void vTurnonHeadlight(void* pvParameters){
  bool pinState;
  Headlight* headlight = (Headlight*) pvParameters;
  while(1){
    //subtracts semaphore back down to 0, next while loop will block again
    xSemaphoreTake(headlight->bsem,portMAX_DELAY);
    //detects the rising or falling edge of the input pin
    //allows the switch to have on/off function
    pinState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
    if (pinState){
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
      //TODO update memory map
    }
    else if (!pinState){
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
      //TODO update memory map
    }
  } 
}




