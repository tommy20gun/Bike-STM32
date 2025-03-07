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

#include "Peripheral_initialization.h"

//CPP includes
#include <Headlight.h>
#include <Horn.h>
#include <Bluetooth.h>
//#include <MemoryMap.h>
#include "Odometer.h"
#include "Motion_Detector.h"
#include "LED.h"
#include "Lock.h"
#include "Taillight.h"
#include "MemoryMap.h"


static Horn horn;
static Headlight headlight;
static Taillight taillight;
static Bluetooth bluetooth;
QueueHandle_t messenger;

/* create tasks, create object instances. object constructors set up hardware and own the semaphore*/
void GlobalSetup(void){
  bluetooth.initPeripherials();
  bluetooth.initTasks();
  bluetooth.initBTMemoryMap();
  horn.initPeripherals();
  horn.initTasks();
  headlight.initPeripherals();
  headlight.initTasks();
  taillight.initPeripherals();
  taillight.initTasks();

  //create global queue
  messenger = xQueueCreate(13,sizeof(struct taggedBuffer));
  bluetooth.messenger = messenger;
  headlight.messenger = messenger;
  horn.messenger = messenger;
  taillight.messenger = messenger;

}

int main(){
  HAL_Init(); //TODO figure out what this does, figure out how to search all 
  SystemClock_Config();
  GPIO_GlobalSetup();
  GlobalSetup();
  vTaskStartScheduler();
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void){
  //wakes up H
  //TODO All buttons need to be debounced with timer instead of GPIO exti
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

void USART2_IRQHandler(void){

};