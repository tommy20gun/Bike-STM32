 /** 
  * @file           : Horn.c
  * @brief          : Implementation of driver for Horn control. 
  * 
  * The Horn uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console.
  * Input pin for EXTI is PA11
  * Output pin to open transistor PB1 
  */

#include <Horn.h>

class Horn{
  public:
  SemaphoreHandle_t bsemaphore;
  TaskHandle_t vTurnOnHornHandle = NULL;
  HeadLight(){

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


  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

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
    bsemaphore = xSemaphoreCreateBinary();
    assert_param(bsemaphore != NULL);
  }

  void vTurnOnHorn(void* pvParameters){
    bool pinState = NULL;
    while(1){
      //subtracts semaphore back down to 0, next while loop will block again
      xSemaphoreTake(bsemaphore,portMAX_DELAY);
      //detects the rising or falling edge of the input pin
      //allows the switch to have on/off function
      pinState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11);
      if (pinstate){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
        //TODO update memory map
      }
      else if (!pinstate){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
        //TODO update memory map
      }
    }
  }
}