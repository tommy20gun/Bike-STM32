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
#include <Bluetooth.h>
#include "Odometer.h"
#include "Motion_Detector.h"
#include "LED.h"
#include "Lock.h"
#include "Taillight.h"


using namespace std;

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

//task handles



static Horn horn;
static Headlight headlight;
static Taillight taillight;
static Bluetooth bluetooth;

/* create tasks, create object instances. object constructors set up hardware and own the semaphore*/
void RTOS_GlobalSetup(void){
  //TODO make sure we are not using systick
}


int main(){
  HAL_Init(); //TODO figure out what this does, figure out how to search all 
  SystemClock_Config();
  bluetooth.initPeripherials();
  bluetooth.setupSlaveToAndroid();
  
  //GPIO_GlobalSetup();
  //RTOS_GlobalSetup();
  //vTaskStartScheduler();
}



/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void){
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
    xSemaphoreGiveFromISR(headlight.bsem, &xHigherPriorityTaskWoken);
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
    xSemaphoreGiveFromISR(horn.bsem, &xHigherPriorityTaskWoken);
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_12) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_12);
    xSemaphoreGiveFromISR(taillight.bsembrake, &xHigherPriorityTaskWoken);
  }
   //Calls the next task Immediately instead of next Tick
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void EXTI9_5_IRQHandler(void){
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
    //if there is no Semaphore to take, then give it
    xSemaphoreGiveFromISR(taillight.bsemleft, &xHigherPriorityTaskWoken);
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
    xSemaphoreGiveFromISR(taillight.bsemright, &xHigherPriorityTaskWoken);
  }
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