 /** 
  * @file           : Headlight.c
  * @brief          : Implementation of driver for Headlight control. 
  * Headlight uses an external GPIO Interrupt triggered on Rising edge caused by closing the switch of the bike control console
  * Input pin for EXTI is PA10
  * Output pin to open transistor PB0 
  */

#include <Headlight.h>
  
class Headlight{

  public:
  SemaphoreHandle_t bsemaphore;
  TaskHandle_t vTurnOnHeadLightHandle = NULL;

  Headlight(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTurnonHeadlight,
      "Turn on and off headlight",
      512, 
      NULL,
      1,
      &vTurnOnHeadLightHandle);
    assert_param(xReturned != pdPASS);
    //Binary Semaphore used for ISR to turn on the headlight
    bsemaphore = xSemaphoreCreateBinary();
    assert_param(bsemaphore != NULL);
  }
};

  void vTurnonHeadlight(void* pvParameters){
    bool pinState = NULL;
    
    while(1){
      //subtracts semaphore back down to 0, next while loop will block again
      xSemaphoreTake(bsemaphore,portMAX_DELAY);
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




