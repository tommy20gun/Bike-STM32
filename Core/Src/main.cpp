 /** 
  * 
  * @brief          : Main program body
  * 
  * This is the main body of the program. The MCU will run setup code such as Clock config, ISRVector table, and start the RTOS.
  * Implements state machine.
  *
  * 
  */


#include "main.h" //includes LL
#include <string>

#include "Peripheral_initialization.h"

//CPP includes
#include "Headlight.h"
#include "Horn.h"
#include "Bluetooth.h"
#include "MemoryMap.h"
#include "Odometer.h"
#include "Motion_Detector.h"
#include "LED.h"
#include "Lock.h"
#include "Taillight.h"
#include "ADC.h"


static Horn horn;
static Headlight headlight;
static Taillight taillight;
static Bluetooth bluetooth;
static ADCDriver adc;
static Lock* SPILock;
QueueHandle_t messenger;
TaskHandle_t stateMachineHandle;
static state_t state;


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
  adc.init();

  //create global queue, assign to all obj
  messenger = xQueueCreate(13,sizeof(struct uint32_t_Buffer));
  bluetooth.messenger = messenger;
  headlight.messenger = messenger;
  horn.messenger = messenger;
  taillight.messenger = messenger;
  adc.messenger = messenger;

  

  BaseType_t xReturned = xTaskCreate(state_machine, "state_machine", 64, NULL, 2, &stateMachineHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }
  SPILock = new Lock(stateMachineHandle);
  SPILock->initPeripheral();
  SPILock->initTask();
}

void state_machine(void* pvParameters){
  state = STATE_LOCKED;
  typedef state_t (*state_Transition)(state_t);
  state_Transition transitiontable[2] = {lock,unlock};
  uint32_t notifiedValue;
  while(1){
    xTaskNotifyWait(pdFALSE, 0xFFFFFFFF, &notifiedValue, portMAX_DELAY);
    //notified value is the same enumeration where 1 calls unlock 0 calls locked
    //STATE_UNLOCKED = 1
    //STATE_LOCKED = 0
    state = transitiontable[notifiedValue](state);
  }

}
//TODO future make this an event group where the notification uint32 has mapping of each task handle
state_t unlock(state_t state){
  if (state == STATE_LOCKED){
    //vTaskSuspend(motionsensor);
    vTaskResume(headlight.vTurnOnHeadlightHandle);
    vTaskResume(taillight.vTurnLeftHandle);
    vTaskResume(taillight.vTurnRightHandle);
    vTaskResume(taillight.vBrakeHandle);
    //vtaskResume(Fardriver);
    //vTaskResume(ParkingBrake);
    //vTaskResume(LED);
    //log stateUnlocked suscessful
    return STATE_UNLOCKED;
  }
  //log stateunlocked failed
  return STATE_LOCKED;
}

state_t lock(state_t state){
  if (state == STATE_UNLOCKED){
    vTaskSuspend(headlight.vTurnOnHeadlightHandle);
    vTaskSuspend(taillight.vTurnLeftHandle);
    vTaskSuspend(taillight.vTurnRightHandle);
    vTaskSuspend(taillight.vBrakeHandle);
    //vTaskSuspend(LED);
    //vtaskSuspend(ParkingBrake);
    //vtaskSuspend(Fardriver);

    //vTaskResume(motiondetection)
    //log stateUnlocked suscessful
    return STATE_LOCKED;
  }
  //log stateunlocked failed
  return STATE_UNLOCKED;
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

void SPI3_IRQHandler(void){
  /*  // handle received
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if(LL_SPI_IsActiveFlag_RXNE(SPI3)){
    //this should autoclear
      xSemaphoreGiveFromISR(SPILock->bsemRXNE,&xHigherPriorityTaskWoken);
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  else if (LL_SPI_IsActiveFlag_OVR(SPI3)){
    Error_Handler();
  }
  else if(LL_SPI_IsActiveFlag_TXE(SPI3)){
    Error_Handler();
  }
  else{
    Error_Handler();
  }
  */
}