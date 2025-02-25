 /** 
  * @file           : BLESend.c
  * @brief          : Implementation of BLE polling of memory map, interface with HC05, many BLE send commands
  * 
  * BLEKey/EN - PA0 output
  * BLEState - PA1 input
  * USART2TX - PA2
  * USART2RX - PA3
  * 
  */

#include "BLE.h"

 
BLE::BLE(){
  
  initPeripherials();
  initBLEMemoryMap();
}


void BLE::initPeripherials(){
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
 

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
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
  
  /* HAL is taking care of this currently in HAL_UART_Mspinit()
  //TX PA2
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP; 
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  //todo make sure there is a pullup on RX for BLE module. Does this work?

  //RX PA3
  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//todo gpio init make sure there is a pullup on RX, 
*/
};

void BLE::initHC05(){
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

void BLE::mode(int mode){
  if (mode){
    LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_0);
  }
  else{
    LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_0);
  }
}
BLEState BLE::getConnectionState(){
  return LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_1);
}

MemoryMap* BLE::initBLEMemoryMap(){
  //memorymap = {
  //  bool* HeadlightON;

  //}
};
void BLE::send05hz(MemoryMap* map){}
void BLE::send50hz(MemoryMap* map){}
void BLE::sendSpecialCommand(MemoryMap* map){}
MemoryMap* BLE::BLERead( MemoryMap* map){}
