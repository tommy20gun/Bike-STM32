 /** 
  * @file           : BLESend.c
  * @brief          : Implementation of BLE polling of memory map, interface with HC05, many BLE send commands
  * 
  * BLEKey/EN - PA0 output. This value must be set on startup to allow ATmode
  * BLEState - PA1 input
  * USART2TX - PA2
  * USART2RX - PA3
  * 
  */

#include "BLE.h"

 
Bluetooth(){
  
  initPeripherials();
  initBLEMemoryMap();
  
}


void Bluetooth::initPeripherials(){

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  //TXPA2 RXPA3 is automatic for AF7
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2| LL_GPIO_PIN_3 ;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP; 
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//todo make sure there is a pullup on RX for BLE module. Does this work?

  //init PB0
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  //BLEState - PA1 input
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //Ensure pin is reset for data mode on default
  LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_0);

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_USART_InitTypeDef USART_InitStruct = {0};

  USART_InitStruct.BaudRate = 9600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  if ((LL_USART_Init(USART2, &USART_InitStruct))){
    Error_Handler();
  }
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);


  //DMA init
  //TODO look at CubeMX again for the setup
  //TODO watch a video on DMA to understand it
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_6, LL_DMA_CHANNEL_4);
  LL_DMA_SetDataTransferDirection(DMA1,LL_DMA_STREAM_6, LL_DMA_DIRECTION_MEMORY_TO);
  LL_DMA_SetStreamPriorityLevel(DMA1,LL_DMA_STREAM_6,LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_6, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1,LL_DMA_STREAM_6, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1,LL_DMA_STREAM_6, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1,LL_DMA_STREAM_6, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_6, LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_6); //TODO what is this?

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  NVIC_SetPriority(DMA1_Stream6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA1_Stream6_IRQn);//TODO why do we need an IRQ for DMA?
};



void Bluetooth::mode(int mode){
  //todo pull this to low so on statup it will
  //todo put an oscilloscope on this to check behavior of startup/reset
  if (mode){
    LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_0);
  }
  else{
    LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_0);
  }
}
BLEState Bluetooth::getConnectionState(){
  return LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_1);
}

MemoryMap* Bluetooth::initBTMemoryMap(){
  //memorymap = {
  //  bool* HeadlightON;

  //}
};
MemoryMap* Bluetooth::BTRead( MemoryMap* map){}



void Bluetooth::send05hz(void* pvParameters){
  while(1){};
}
void Bluetooth::send50hz(void* pvParameters){
  while(1){};
}
void Bluetooth::sendSpecialCommand(MemoryMap* map){
  while(1){};
}

/*
void Bluetooth::setupSlaveToAndroid(){
  // should slow blink when in AT mode, fast blink in connection mode
  
  //enter Command mode
  mode(COMMAND_MODE);
  char* at_state = "AT+STATE?\r\n";
  char* at_name = "AT+NAME=DeezNuts\r\n";
  char* setRole = "AT+ROLE?\r\n";
  //role should be slave for phone.
  //role should be master for Fardriver and BMS

  //115200 baud 1 bit stop none parity
  char* uartConfig = "AT_UART=115200,0,0";
  //receive OK 

  //getaddr
  char* getaddr = "AT+ADDR?\r\n";
  //nn - NAP (16 bit Non-significant Address Portion)
  //uu - UAP (8 bit Upper Address Portion)
  //ll - LAP (24 bit Lower Address Portion)
  //"+ADDR:11:6:230154" = "11:06:23:01:54"
};
*/