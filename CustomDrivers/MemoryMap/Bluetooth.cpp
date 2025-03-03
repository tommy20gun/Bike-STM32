 /** 
  * @file           : BLESend.c
  * @brief          : Implementation of BLE polling of memory map, interface with HC05, many BLE send commands
  * 
  * BLEKey/EN - PA0 output. This value must be set on startup to allow ATmode
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
  

  //initBLEMemoryMap();
  
}

/*void Bluetooth::ATModeTesting(){
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
};*/

void Bluetooth::initPeripherials(){
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  //TXPA2 RXPA3 is automatic for AF7
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2| LL_GPIO_PIN_3 ;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP; 
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
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

  //DMA init
  //TODO look at CubeMX again for the setup
  //TODO watch a video on DMA to understand it

  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_6, LL_DMA_CHANNEL_4);
  LL_DMA_SetDataTransferDirection(DMA1,LL_DMA_STREAM_6, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetStreamPriorityLevel(DMA1,LL_DMA_STREAM_6,LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_6, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1,LL_DMA_STREAM_6, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1,LL_DMA_STREAM_6, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1,LL_DMA_STREAM_6, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_6, LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_6); //TODO what is this?

  NVIC_SetPriority(DMA1_Stream6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA1_Stream6_IRQn);//TODO why do we need an IRQ for DMA?
  
  //enables receive for UART IT
  NVIC_SetPriority(USART2_IRQn, 5);
  NVIC_EnableIRQ(USART2_IRQn);

  //enables Transfer Complete to trigger the IRQ
  LL_DMA_EnableIT_TC(DMA1,LL_DMA_STREAM_6);

 //UART init 
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
  LL_USART_EnableIT_RXNE(USART2);
  LL_USART_Enable(USART2);
};

void Bluetooth::initTasks(){
  BaseType_t xReturned;
  xReturned = xTaskCreate(sendTest,"sendTest",64, this,1,&sendTestHandle);
  if(xReturned != pdPASS){
    Error_Handler();
  }
}

void Bluetooth::setmode(int mode){
  // only works on powerup for hc05
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

void Bluetooth::initBTMemoryMap(){
  map = {0,1,0,0,0,0,0,0,0,0,0,0,0};
};

//Bluetooth::MemoryMap* Bluetooth::BTRead( MemoryMap* map){}



void Bluetooth::send05hz(void* pvParameters){
  while(1){};
}
void Bluetooth::send50hz(void* pvParameters){
  while(1){};
}
void Bluetooth::sendSpecialCommand(void* pvParameters){
  while(1){};
}
void Bluetooth::sendTest(void* pvParameters){
  //extracts the map out of bluetooth
  Bluetooth::MemoryMap* map = &((Bluetooth*)pvParameters)->map;
  LL_DMA_ConfigAddresses(DMA1,LL_DMA_STREAM_6, (uint32_t) map, LL_USART_DMA_GetRegAddr(USART2),LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  while(1){
    LL_DMA_DisableStream(DMA1,LL_DMA_STREAM_6);
    LL_DMA_SetDataLength(DMA1,LL_DMA_STREAM_6, sizeof(*map));
    LL_DMA_EnableStream(DMA1,LL_DMA_STREAM_6);
    LL_USART_EnableDMAReq_TX(USART2);

    //transfer every 10 seconds
    vTaskDelay(1000);
  };
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  //you cannot breakpoint this lol
  if(LL_DMA_IsActiveFlag_TC6(DMA1) == 1){
    LL_DMA_ClearFlag_TC6(DMA1);
  }
}
