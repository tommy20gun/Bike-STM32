 /** 
  * @file           : Horn.c
  * @brief          : Implementation of driver for Horn control. 
  * 
  * The Horn uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console.
  * Input pin for EXTI is PA11
  * Output pin to open transistor PB1 
  */

#include <Horn.h>




Horn::Horn(){
  //set attribute to NULL
  vTurnOnHornHandle = NULL;
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

  //RTOS Task setup
  BaseType_t xReturned;
  xReturned = xTaskCreate(vTurnOnHorn,
    "Turn on and off headlight",
    512, 
    NULL,
    1,
    &vTurnOnHornHandle);
  assert_param(xReturned != pdPASS);
  //Binary Semaphore used for ISR to turn on the headlight
  bsem_horn = xSemaphoreCreateBinary();
  assert_param(bsem_horn != NULL);
}


void vTurnOnHorn(void* pvParameters){
  
  bool pinState = NULL;
  while(1){
    //subtracts semaphore back down to 0, next while loop will block again
    xSemaphoreTake(.bsem_horn,portMAX_DELAY);
    //detects the rising or falling edge of the input pin
    //allows the switch to have on/off function
    pinState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
    if (pinState){
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
      //TODO update memory map
    }
    else if (!pinState){
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
      //TODO update memory map
    }
  }
}
