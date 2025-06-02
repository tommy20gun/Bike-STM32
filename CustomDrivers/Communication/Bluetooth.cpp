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

 

Bluetooth::Bluetooth(){

}


void Bluetooth::initPeripherials(){

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  //GPIO Init
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

  //DMA init
  //TODO watch a video on DMA to understand it

  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_6, LL_DMA_CHANNEL_4);
  LL_DMA_SetDataTransferDirection(DMA1,LL_DMA_STREAM_6, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetStreamPriorityLevel(DMA1,LL_DMA_STREAM_6,LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_6, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1,LL_DMA_STREAM_6, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1,LL_DMA_STREAM_6, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1,LL_DMA_STREAM_6, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_6, LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_6); 

  NVIC_SetPriority(DMA1_Stream6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA1_Stream6_IRQn);//IRQ is for transmit complete notification
  
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

  //CRC Init
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);
};

void Bluetooth::initTasks(){
  BaseType_t xReturned;
  xReturned = xTaskCreate(send,"send",64, this, 1, &sendHandle);
  if(xReturned != pdPASS){
    Error_Handler();
  }
  /*xReturned = xTaskCreate(sendFast,"sendFast",64, this,1,&sendFastHandle);
  if(xReturned != pdPASS){
    Error_Handler();
  }
  xReturned = xTaskCreate(sendSpecialCommand,"sendSpecialCommand",64, this,1,&sendSpecialCommandHandle);
  if(xReturned != pdPASS){
    Error_Handler();
  }
  sendSemaphore = xSemaphoreCreateMutex();
  if (sendSemaphore == NULL){
    Error_Handler();
  }
  //init as give
  xSemaphoreGive(sendSemaphore);*/
}

void Bluetooth::setmode(int mode){
  // only works on powerup for hc05
  //wire in switch to work on perffboard
  if (mode){
    LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_0);
  }
  else{
    LL_GPIO_ResetOutputPin(GPIOA, GPIO_PIN_0);
  }
}
BTState Bluetooth::getConnectionState(){
  return LL_GPIO_IsInputPinSet(GPIOA,GPIO_PIN_1);
}

void Bluetooth::initBTMemoryMap(){
  map = {69420,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //set the magic number
}

//this is static
void Bluetooth::send(void* pvParameters){
  Bluetooth* tooth = (Bluetooth*)pvParameters;
  MemoryMap* map = &(tooth->map);
  struct uint32_t_Buffer buff;
  LL_DMA_ConfigAddresses(DMA1,LL_DMA_STREAM_6, (uint32_t) map, LL_USART_DMA_GetRegAddr(USART2),LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  while(1){
    //xSemaphoreTake(tooth->sendSemaphore,portMAX_DELAY);
    //empty queue
    while (uxQueueMessagesWaiting(tooth->messenger) > 0){
      xQueueReceive(tooth->messenger, (void*) &buff, 0);
      map = receiveTaggedData(&buff,map);
    }
    map->CRC32 = CRC32MemoryMap(map);
    uartTransmitDMA(sizeof(*map)); //size is 80 I think, addr: start of struct
    //xSemaphoreGive(tooth->sendSemaphore);
    //TODO create CRC, create magic number for starting of queue
    vTaskDelay(100);
  }
}
uint32_t Bluetooth::CRC32MemoryMap(MemoryMap* map){
  /*To compute a CRC of the supported data, go through the following steps:
 1. Enable the CRC peripheral clock via the RCC peripheral.
 2. Set the CRC data register to the initial CRC value by configuring the initial CRC value 
register (CRC_INIT). In the more recent STM32 Series, it is possible to chain a CRC 
calculation based on the previous CRC calculation as initial value. In this case, the 
CRC_IDR register (not affected by the reset bit in CRC_CR) can be used. In HAL, this 
is implemented by HAL_CRC_Calculate.
3. Set the I/O reverse bit order through the REV_IN[1:0] and REV_OUT bits, respectively, 
in the CRC control register (CRC_CR).
4. Set the polynomial size and coefficients through the POLYSIZE[1:0] bits in CRC control 
register (CRC_CR) and CRC polynomial register (CRC_POL), respectively.
5. Reset the CRC peripheral through the Reset bit in the CRC control register (CRC_CR).
6. Set the data to the CRC data register.
7. Read the content of the CRC data register.
8. Disable the CRC peripheral clock.

 In the firmware package, the CRC_usage example runs the CRC checksum code 
computing an array data (DataBuffer) of 256 supported data type. For a full description, 
refer to the file Readme.txt in the CRC_usage folder*/

//calculates a checksum based on 12V ADC, 72V ADC, Odometer.
//this is because i can iterate through the struct and it is a waste of time.
//Uses CRC-32 polynomial: 0x4C11DB7
  
  LL_CRC_ResetCRCCalculationUnit(CRC);

  //doing it in this order triggers the hardware properly
  //headlightON is a bool
  LL_CRC_FeedData32(CRC, map->motorTemp);
  LL_CRC_FeedData32(CRC, map->headlightON);
  LL_CRC_FeedData32(CRC, map->ADCreading72V);
  LL_CRC_FeedData32(CRC, map->ADCreading12V);
  LL_CRC_FeedData32(CRC, map->battTemp);
  LL_CRC_FeedData32(CRC, map->Odometer);
  LL_CRC_FeedData32(CRC, map->speed);
  LL_CRC_FeedData32(CRC, map->hornON);
  LL_CRC_FeedData32(CRC, map->brakeON);
  LL_CRC_FeedData32(CRC, map->turningLeft);
  LL_CRC_FeedData32(CRC, map->turningRight);
  LL_CRC_FeedData32(CRC, map->RPM);
  LL_CRC_FeedData32(CRC, map->throttleV);
  LL_CRC_FeedData32(CRC, map->Lock);
  return LL_CRC_ReadData32(CRC);
}

/*
void Bluetooth::sendFast(void* pvParameters){
  Bluetooth* tooth = (Bluetooth*)pvParameters;
  MemoryMap* map = &(tooth->map);
  while(1){
    xSemaphoreTake(tooth->sendSemaphore,portMAX_DELAY);
    uartTransmitDMA((uint32_t)&(map->speed),24);//size is 24, addr is +48 after start of struct
    xSemaphoreGive(tooth->sendSemaphore);
    vTaskDelay(200); //sending at 5hz
  }
}
void Bluetooth::sendSpecialCommand(void* pvParameters){
  //MemoryMap* map = &((Bluetooth*)pvParameters)->map;
  while(1){
    vTaskDelay(200);
  }
}*/
//IT function
void DMA1_Stream6_IRQHandler(void)
{
  //you cannot breakpoint this lol
  if(LL_DMA_IsActiveFlag_TC6(DMA1) == 1){
    LL_DMA_ClearFlag_TC6(DMA1);
  }
}

//private functions
/**
  * @brief Helper for transmitting data through DMA given size and addr
  */
void Bluetooth::uartTransmitDMA(uint32_t size){
  LL_DMA_DisableStream(DMA1,LL_DMA_STREAM_6);
  LL_DMA_SetDataLength(DMA1,LL_DMA_STREAM_6, size);
  LL_DMA_EnableStream(DMA1,LL_DMA_STREAM_6);
  LL_USART_EnableDMAReq_TX(USART2);
  }

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */


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