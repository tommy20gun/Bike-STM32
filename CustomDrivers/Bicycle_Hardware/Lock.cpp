 /** 
  * 
  * @brief          : Locking function of bike, drivers for NFC Reader, access to states in the state machine.
  * 
  * Locking function of the bike will be the enabling of Fardriver. Fardriver requires 72V to be short on the Enable Pin to operate.
  * Communication with the NFC reader such as passwords, SPI protocol is implemented.
  * Provides commands to access the state machine for Lock and Unlocking
  *
  * Lock input-PA7
  * FardriverEN_72V - PA6
  * 
  */

#include "Lock.h"

void Lock::initPeripherals(){
    initInputPin(inputPin, true);
    initOutputPin(outputPin, true);
}

void Lock::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"Lock Function",64, this,2,&TaskHandle);
    if (xReturned != pdPASS){
    Error_Handler();
    }
}

void Lock::vTaskFunction(void* pvParameters){
    Lock* lock = (Lock*) pvParameters;
    lock->RTOSImplementation();
}
/*
pin high external interrupt
check pin status with debounce
if false positive, pass
if truth, toggle state = 1, delay for 10 seconds

result:
- holding pin for more than 10 seconds will trigger second state change
- pressing pin for longer than debounce timer (+poll timer) will trigger state change
- pressing not long enough does nothing
*/
void Lock::RTOSImplementation(){
    bool inPinState;
    struct uint32_t_Buffer buff;
    buff.data = 0;
    buff.tag = StateMachineStatus;
    while(1){
        //every loop toggles the state
        inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);
        vTaskDelay(100); //debounce and poll rate
        if (inPinState == true && inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask)){
            xTaskNotifyGive(stateMachineHandle); //increments notif value by 1.
            buff.data = buff.data ^ 1U; //toggles the first bit
            xQueueSendToBack(messenger, &buff , 0);
            vTaskDelay(10000); //cannot change state again for more than seconds
        }
    }
}
