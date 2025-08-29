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
#include "Speedometer.h"
#include "Motion_Detector.h"
#include "LED.h"
#include "Lock.h"
#include "Taillight.h"
#include "ADC.h"
#include "SwitchActivatedDevice.h"


static Horn* horn;
static Headlight* headlight;
static TailLight_Turn* left;
static TailLight_Turn* right;
static TailLight_Brake* brake;
static Bluetooth* bluetooth;
static ADCDriver* adc;
static Lock* bikelock;
static Motion_Detector* detector1;
//static Motion_Detector* detector2;
//static Motion_Detector* detector3;

TaskHandle_t stateMachineHandle;
static state_t state;
QueueHandle_t messenger;

/* create tasks, create object instances. object constructors set up hardware and own the semaphore*/
void GlobalSetup(void){
    //create global queue, assign to all obj
  messenger = xQueueCreate(13,sizeof(struct uint32_t_Buffer));
  BaseType_t xReturned = xTaskCreate(state_machine, "state_machine", 64, NULL, 2, &stateMachineHandle);
  if (xReturned != pdPASS){
    Error_Handler();
  }

  horn = new Horn(GPIOA,GPIO_PIN_8,GPIOB,GPIO_PIN_1,messenger,hornON);
  headlight = new Headlight(GPIOA,GPIO_PIN_10,GPIOB,GPIO_PIN_0,messenger,headlightON);
  left = new TailLight_Turn(GPIOB,GPIO_PIN_6,GPIOB,GPIO_PIN_8,GPIOB,GPIO_PIN_10,messenger,turningLeft);
  right = new TailLight_Turn(GPIOB,GPIO_PIN_7,GPIOB,GPIO_PIN_9,GPIOB, GPIO_PIN_12,messenger,turningRight);
  brake = new TailLight_Brake(GPIOA,GPIO_PIN_9,GPIOA,GPIO_PIN_5,messenger,brakeON);
  bluetooth = new Bluetooth(messenger); //bluetooth hard coded to PA0,PA1,PA2,PA3
  //channel4 (12V), channel6 (72V) PA4 and PA6 is hardcoded. So is Queuetag
  adc = new ADCDriver(messenger, 12, 5);      
  bikelock = new Lock(GPIOA,GPIO_PIN_7,GPIOB,GPIO_PIN_6,messenger,StateMachineStatus,stateMachineHandle);
  //detector1 = new Motion_Detector(messenger,headlightON, GPIOB, GPIO_PIN_13);
  //lock(state); This should initialize the state machine as locked after 
}

//TODO there is a possiblity to implement lock level 2 for motion detection
/*
State machine should stay in main
*/
void state_machine(void* pvParameters){
  state = STATE_LOCKED;
  typedef state_t (*state_Transition)(state_t);
  state_Transition transitiontable[2] = {lock,unlock};
  uint32_t notifiedValue;
  while(1){
    xTaskNotifyWait(0, 0, &notifiedValue, portMAX_DELAY);
    //notified value is the same enumeration where 1 calls unlock 0 calls locked
    //STATE_UNLOCKED = 1
    //STATE_LOCKED = 0
    state = transitiontable[notifiedValue & 1U](state); //reads the last bit to determine 0 or 1
  }
}
//TODO future make this an event group where the notification uint32 has mapping of each task handle
/*
FardriverEN_72V - PA6
*/
state_t unlock(state_t state){
  if (state == STATE_LOCKED){
    vTaskResume(headlight->getTaskHandle());
    vTaskResume(left->getTaskHandle());
    vTaskResume(right->getTaskHandle());
    vTaskResume(brake->getTaskHandle());
    LL_GPIO_ResetOutputPin(GPIOA,GPIO_PIN_6); //fardriver Pin
    //vTaskResume(LED);
    //vTaskSuspend(motionsensor);
    //log stateUnlocked suscessful
    return STATE_UNLOCKED;
  }
  //log stateunlocked failed
  return STATE_LOCKED;
}
/*
FardriverEN_72V - PA6
*/
state_t lock(state_t state){
  if (state == STATE_UNLOCKED){
    vTaskSuspend(headlight->getTaskHandle());
    vTaskSuspend(left->getTaskHandle());
    vTaskSuspend(right->getTaskHandle());
    vTaskSuspend(brake->getTaskHandle());
    LL_GPIO_SetOutputPin(GPIOA,GPIO_PIN_6); //fardriver Pin
    //vTaskSuspend(LED);
    //vTaskResume(motiondetection)
    //log stateUnlocked suscessful
    return STATE_LOCKED;
  }
  //log stateunlocked failed
  return STATE_UNLOCKED;
}

int main(){
  //HAL_Init(); //TODO Try to run code without this
  SystemClock_Config();
  GPIO_GlobalSetup();
  GlobalSetup();
  vTaskStartScheduler();
}

