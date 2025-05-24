 /** 
  * 
  * @brief          : Locking function of bike, drivers for NFC Reader, access to states in the state machine.
  * 
  * Locking function of the bike will be the enabling of Fardriver. Fardriver requires 72V to be short on the Enable Pin to operate.
  * Communication with the NFC reader such as passwords, SPI protocol is implemented.
  * Provides commands to access the state machine for Lock and Unlocking
  *
  * SPI communication protocol-PA7
  * 
  * FardriverEN_72V - PA6
  * 
  */

#include "Lock.h"

Lock::Lock(TaskHandle_t statetask){
    this->stateMachineHandle = statetask;
}

void Lock::initPeripheral(){

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_INPUT);
}

void Lock::initTask(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vLockFunction,"Lock Function",64, this,2,&vLockFunctionHandle);
    if (xReturned != pdPASS){
    Error_Handler();
    }
    bsem = xSemaphoreCreateBinary();

}

void Lock::vLockFunction(void* PvParameters){
    //Lock* SPILock = (Lock*) PvParameters;
    while(1){
        vTaskDelay(1000);
    }
}

