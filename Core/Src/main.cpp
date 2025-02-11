 /** 
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  */


#include <main.h> //includes LL
#include <string>

using namespace std;

void vTurnonHeadlight(void const* args){
  while(1){
    //subtracts semaphore back down to 0, next while loop will block again
    xSemaphoreTake(xSemaphore,portMAX_DELAY);
    HAL_GPIO_WritePin(GPIOB,GPIO_Pin_0,SET);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (GPIO_Pin == GPIO_PIN_0){
      xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    }
    //Calls the next task Immediately instead of next Tick
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
static TaskHandle_t TurnonHeadlight = NULL;
static SemaphoreHandle_t xSemaphore;

void RTOS_Setup(void){
  //TODO make sure we are not using systick
  BaseType_t xReturned;
  xReturned = xTaskCreate(vTurnOnHeadlight,
   "Turn on and off headlight",
   512, 
   NULL,
   1,
   &TurnonHeadlight );
  assert_param(xReturned != pdPASS);
  //Binary Semaphore used for ISR to turn on the headlight
  xSemaphore = xSemaphoreCreateBinary();
  assert_param(xSemaphore != NULL);
}

int main(){
  HAL_Init(); //TODO figure out what this does, figure out how to search all 
  SystemClock_Config();
  GPIO_Setup();
  RTOS_Setup();
  vTaskStartScheduler();
  
}

//static TaskHandle_t job2Handle;
/*void UART_task(void const* args){
  //const uint8_t buffer[] = "deez nuts";
  //UART_HandleTypeDef* huartaddr = USART2_Setup();// will this work if static is on the other file?
  while(1){
    osDelay(1250);
    LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_13);


    //Uart does not work without Bluetooth LOL
    //HAL_UART_Transmit(huartaddr,buffer, sizeof(buffer), 1000);
  }
}
*/

/*
BareMetal Scratch work
void delay (volatile uint32_t clockticks){
  while (clockticks--);
}

void on(){
  GPIOC -> BSRR = (1U << 13);

}
void off(){
  GPIOC -> BSRR = ((1U << 13) << 16);

}*/