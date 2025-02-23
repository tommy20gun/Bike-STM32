 /** 
  * 
  * @brief          : Main program body
  * 
  * This is the main body of the program. The MCU will run setup code such as Clock config, ISRVector table, and start the RTOS.
  * Implements state machine.
  *
  * 
  */


#include <main.h> //includes LL
#include <string>

//CPP includes
#include <Headlight.h>
#include <Horn.h>
#include "Odometer.h"
#include "Motion_Detector.h"
#include "LED.h"
#include "Lock.h"
#include "TailLight.h"


using namespace std;

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

//task handles
TaskHandle_t vTurnOnHornHandle;
TaskHandle_t vTurnOnHeadlightHandle;
static Horn horn;
static Headlight headlight;

/* create tasks, create object instances. object constructors set up hardware and own the semaphore*/
void RTOS_Setup(void){
  //TODO make sure we are not using systick
  
  BaseType_t xReturned;
  xReturned = xTaskCreate(vTurnOnHorn,
    "horn On/Off",
    512, 
    &horn,
    1,
    &vTurnOnHornHandle);
  if(xReturned != pdPASS){
    Error_Handler();
  }
  //Binary Semaphore used for ISR to turn on the headlight
  horn.bsem = xSemaphoreCreateBinary();
  if(horn.bsem == NULL){
    Error_Handler();
  }

  
  xReturned = xTaskCreate(vTurnonHeadlight,
    "Headlight On/off",
    512, 
    &headlight,
    1,
    &vTurnOnHeadlightHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }
  //Binary Semaphore used for ISR to turn on the headlight
  headlight.bsem = xSemaphoreCreateBinary();
  if(headlight.bsem == NULL){
    Error_Handler();
  }
}


int main(){
  HAL_Init(); //TODO figure out what this does, figure out how to search all 
  SystemClock_Config();
  GPIO_Setup();
  RTOS_Setup();
  vTaskStartScheduler();
  
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ){
  Error_Handler();
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
 void EXTI15_10_IRQHandler(void)
 {
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
   if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10) != RESET)
   {
     LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
     //headlight TODO make correct semaphore
     xSemaphoreGiveFromISR(headlight.bsem, &xHigherPriorityTaskWoken);
   }
   if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11) != RESET)
   {
     LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
     //horn TODO make correct semaphore
     xSemaphoreGiveFromISR(horn.bsem, &xHigherPriorityTaskWoken);
   }
   if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_12) != RESET)
   {
     LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_12);
   }
   //Calls the next task Immediately instead of next Tick
   portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
 }

/*
 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (GPIO_Pin == GPIO_PIN_10){
      //headlight TODO make correct semaphore
      xSemaphoreGiveFromISR(headlight.bsem, &xHigherPriorityTaskWoken);
    }
    else if (GPIO_Pin == GPIO_PIN_11){
      //horn TODO make correct semaphore
      xSemaphoreGiveFromISR(horn.bsem, &xHigherPriorityTaskWoken);
    }
    //Calls the next task Immediately instead of next Tick
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/
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