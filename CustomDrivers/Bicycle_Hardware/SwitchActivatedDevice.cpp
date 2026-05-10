#include "SwitchActivatedDevice.h"

SwitchActivatedDevice::SwitchActivatedDevice(GPIO_TypeDef* GPIOxIn, uint32_t PinMaskIn, GPIO_TypeDef* GPIOxOut, uint32_t PinMaskOut, QueueHandle_t messenger, DataTable queueTag){
    inputPin.GPIOx = GPIOxIn;
    inputPin.PinMask = PinMaskIn;
    outputPin.GPIOx = GPIOxOut;
    outputPin.PinMask = PinMaskOut;
    this->messenger = messenger;
    this->queueTag = queueTag;
}

void SwitchActivatedDevice::initPeripherals(){
    initInputPin(inputPin, true);
    initOutputPin(outputPin, true);
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

void SwitchActivatedDevice::QueueSend(){
  struct uint32_t_Buffer buff;
  buff.tag = queueTag;
  buff.data = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);
  xQueueSendToBack(messenger, &buff , 0);
}

void SwitchActivatedDevice::RTOSImplementation(){
  initializeHWPinState();
  bool inPinState;
  bool prevPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);

  while(1){
    inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask); //TODO make this an interrupt signal, 
    //where the task runs one time to check for debounce
    //debounce
    vTaskDelay(50);
    //read again to confirm the signal, if not, ignore it
    if (inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask) && inPinState != prevPinState){
      if (inPinState){
        LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask); //TODO make this a mutex for motion detect
        LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask);  //Slight delay to allow the readpin to work
        prevPinState = inPinState;
        QueueSend();
      }
      else if (!inPinState){
        LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask);
        LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask); //Slight delay to allow the readpin to work
        prevPinState = inPinState;
        QueueSend();
      }
    }
  }
}

void SwitchActivatedDevice::initializeHWPinState(){
  //initialize to previous hardware setting. I.e. if the light switch is on,
  //then the light should be on at reset
  if (LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask)){
    LL_GPIO_SetOutputPin(outputPin.GPIOx,outputPin.PinMask); //closes the circuit, ground the output
    QueueSend();
  }
  else{
    LL_GPIO_ResetOutputPin(outputPin.GPIOx,outputPin.PinMask); //open the circuit, open drain
    QueueSend();
  }
}

