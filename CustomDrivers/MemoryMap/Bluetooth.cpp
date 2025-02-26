 /** 
  * @file           : BLESend.c
  * @brief          : Implementation of BLE polling of memory map, interface with HC05, many BLE send commands
  * 
  * BLEKey/EN - PA0 output
  * BLEState - PA1 input
  * USART2TX - PA2
  * USART2RX - PA3
  * 
  * If the EGBT-045MS is powered ON with the CMD
  * pin at logic low (or open circuit), and then pulled
  * High a moment later, the UART parameters assumes a set of values that was previously fixed
  * using the AT+UART command, with 9600bps, 8 data bits, 1 stop bit, no parity, and no handshake as default parameter values.
  */

#include "Bluetooth.h"
#include <string.h>
 
Bluetooth::Bluetooth(){
  
  initPeripherials();
  //initBLEMemoryMap();
  
}

void Bluetooth::setupSlaveToAndroid(){
  // should slow blink when in AT mode, fast blink in connection mode
  
  //enter Command mode
  setmode(COMMAND_MODE);
  uint8_t rxbuff[30];
  char* atTest = "AT\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)atTest,strlen(atTest), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 4, HAL_MAX_DELAY); //OK
  memset(rxbuff, 0, sizeof rxbuff);
  char* changename = "AT+NAME=DeezNuts\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)changename,strlen(changename), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 4, HAL_MAX_DELAY); //OK
  memset(rxbuff, 0, sizeof rxbuff);
  char* Role = "AT+ROLE?\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)Role,strlen(Role), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 9, HAL_MAX_DELAY); //0 is slave
  memset(rxbuff, 0, sizeof rxbuff);
  char* addr = "AT+ADDR?\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)addr,strlen(addr), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 27, HAL_MAX_DELAY);
  memset(rxbuff, 0, sizeof rxbuff);
  //nn - NAP (16 bit Non-significant Address Portion)
  //uu - UAP (8 bit Upper Address Portion)
  //ll - LAP (24 bit Lower Address Portion)
  //"+ADDR:11:6:230154" = "11:06:23:01:54"
  char* baud = "AT+UART?\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)baud,strlen(baud), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 20, HAL_MAX_DELAY); //+UART:<baud>,<stop>,<parity>\r\nOK\r\n
  memset(rxbuff, 0, sizeof rxbuff);
  baud = "AT+UART=9600,0,1\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)baud,strlen(baud), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 4, HAL_MAX_DELAY); //OK
  memset(rxbuff, 0, sizeof rxbuff);
  baud =  "AT+UART?\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)baud,strlen(baud), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 20, HAL_MAX_DELAY); //+UART:<baud>,<stop>,<parity>\r\nOK\r\n
  memset(rxbuff, 0, sizeof rxbuff);
  char* pw = "AT+PSWD?\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)pw,strlen(pw), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 17, HAL_MAX_DELAY);
  memset(rxbuff, 0, sizeof rxbuff);
  pw = "AT+PSWD=\"6969\"\r\n";
  HAL_UART_Transmit(&huart2,(uint8_t*)pw,strlen(pw), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2,rxbuff, 4, HAL_MAX_DELAY);
  memset(rxbuff, 0, sizeof rxbuff);
  setmode(DATA_MODE);
  
  volatile Bluetooth::BTState state = getConnectionState();
  uint8_t txbuffer[10];

  while(1){
    HAL_UART_Receive(&huart2,txbuffer, 1, HAL_MAX_DELAY);
  }
};

void Bluetooth::initPeripherials(){
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
  //init PA0
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



void Bluetooth::setmode(int mode){
  if (mode){
    LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_0);
  }
  else{
    LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_0);
  }
}
Bluetooth::BTState Bluetooth::getConnectionState(){
  return LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_1);
}

Bluetooth::MemoryMap* Bluetooth::initBTMemoryMap(){
  //memorymap = {
  //  bool* HeadlightON;

  //}
};
Bluetooth::MemoryMap* Bluetooth::BTRead( MemoryMap* map){}



void Bluetooth::send05hz(void* pvParameters){
  while(1){};
}
void Bluetooth::send50hz(void* pvParameters){
  while(1){};
}
void Bluetooth::sendSpecialCommand(void* pvParameters){
  while(1){};
}
