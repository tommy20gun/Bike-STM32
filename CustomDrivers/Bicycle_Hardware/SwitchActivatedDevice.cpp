#include "SwitchActivatedDevice.h"

SwitchActivatedDevice::SwitchActivatedDevice(GPIO_TypeDef* GPIOxIn, uint32_t PinMaskIn, GPIO_TypeDef* GPIOxOut, uint32_t PinMaskOut, QueueHandle_t messenger, DataTable queueTag){
    inputPin.GPIOx = GPIOxIn;
    inputPin.PinMask = PinMaskIn;
    outputPin.GPIOx = GPIOxOut;
    outputPin.PinMask = PinMaskOut;
    this->messenger = messenger;
    this->queueTag = queueTag;
}

void SwitchActivatedDevice::initInputPin(GPIOPin_Struct pin, bool PullDown){
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin =  pin.PinMask;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  if (PullDown == true){
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  }
  else{
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  }
  LL_GPIO_Init(pin.GPIOx, &GPIO_InitStruct);
}
void SwitchActivatedDevice::initOutputPin(GPIOPin_Struct pin, bool openDrain){
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin.PinMask;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  if (openDrain == true){
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  }
  else{
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  }
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(pin.GPIOx, &GPIO_InitStruct);
}
void SwitchActivatedDevice::initPeripherals(){
    initInputPin(inputPin, true);
    initOutputPin(outputPin, false);
}
void SwitchActivatedDevice::QueueSend(){
  struct uint32_t_Buffer buff;
  buff.data = 0;
  buff.tag = queueTag;
  buff.data = LL_GPIO_IsOutputPinSet(outputPin.GPIOx,outputPin.PinMask);
  xQueueSendToBack(messenger, &buff , 0);
}

void SwitchActivatedDevice::RTOSImplementation(){
  bool inPinState;
  bool prevPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);

  while(1){
    inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);
    //debounce
    vTaskDelay(100);
    //read again to confirm the signal, if not, ignore it
    if (inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask) && inPinState != prevPinState){
      if (inPinState){
        LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask);
        QueueSend();
        prevPinState = inPinState;
      }
      else if (!inPinState){
        LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask);
        QueueSend();
        prevPinState = inPinState;
      }
    }
  }
}

void TestA::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"TestA On/Off",64, this,2,&TaskHandle);
    if(xReturned != pdPASS){
        Error_Handler();
    }
}

void TestA::vTaskFunction(void* pvParameters){
  TestA* testAobj = (TestA*) pvParameters;
  testAobj->RTOSImplementation();
}
