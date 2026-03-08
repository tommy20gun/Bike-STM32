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
#include <string>

void Bluetooth::initPeripherals(){

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
  xReturned = xTaskCreate(vTaskFunction,"Bluetooth Send UART",512, this, 1, &TaskHandle);
  if(xReturned != pdPASS){
    Error_Handler();
  }
  /*
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

void Bluetooth::QueueSend(){};

void Bluetooth::emptyQueue(){
  struct uint32_t_Buffer buff;
  while (uxQueueMessagesWaiting(this->messenger) > 0){
    xQueueReceive(this->messenger, (void*) &buff, 0);
    receiveTaggedData(&buff,&map);
  }
}

void Bluetooth::vTaskFunction(void* pvParameters){
  Bluetooth* tooth = (Bluetooth*)pvParameters;
  tooth->RTOSImplementation();
}

  //Testing code prints readable text to serial terminal
#define TESTING
void Bluetooth::RTOSImplementation(){
  #ifndef TESTING
    LL_DMA_ConfigAddresses(DMA1,LL_DMA_STREAM_6, (uint32_t) &map, LL_USART_DMA_GetRegAddr(USART2),LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    while(1){
      //xSemaphoreTake(tooth->sendSemaphore,portMAX_DELAY);
      //empty queue
      //TODO maybe do a mutex and separate these tasks
      emptyQueue();
      map.CRC32 = CRC32MemoryMap(&map);
      uartTransmitDMA(sizeof(map)); //size is 80 I think, addr: start of struct
      //xSemaphoreGive(this->sendSemaphore);
      vTaskDelay(100);
    }
  #else //code for testing is below
    std::string TestingString;
    while (1){
      emptyQueue();
      map.CRC32 = CRC32MemoryMap(&map);
      TestingString = "Magic Number: ";
      TestingString.append(std::to_string(map.magicNumber));
      TestingString.append("\nHeadlight ON: ");
      TestingString.append(std::to_string(map.headlightON));
      TestingString.append("\nMotor Temp: ");
      TestingString.append(std::to_string(map.motorTemp));
      TestingString.append("\nADC Reading 72V: ");
      TestingString.append(std::to_string(map.ADCreading72V));
      TestingString.append("\nADC Reading 12V: ");
      TestingString.append(std::to_string(map.ADCreading12V));
      TestingString.append("\nBattery Temp: ");
      TestingString.append(std::to_string(map.battTemp));
      TestingString.append("\nOdometer: ");
      TestingString.append(std::to_string(map.Odometer));
      TestingString.append("\nSpeed: ");
      TestingString.append(std::to_string(map.speed));
      TestingString.append("\nHorn ON: ");
      TestingString.append(std::to_string(map.hornON));
      TestingString.append("\nBrake ON: ");
      TestingString.append(std::to_string(map.brakeON));
      TestingString.append("\nTurning Left: ");
      TestingString.append(std::to_string(map.turningLeft));
      TestingString.append("\nTurning Right: ");
      TestingString.append(std::to_string(map.turningRight));
      TestingString.append("\nThrottle Voltage: ");
      TestingString.append(std::to_string(map.throttleV));
      TestingString.append("\nState Machine Status: ");
      TestingString.append(std::to_string(map.StateMachineStatus));
      TestingString.append("\nCRC32: ");
      TestingString.append(std::to_string(map.CRC32));
      TestingString.append("\n\n");
      LL_DMA_ConfigAddresses(DMA1,LL_DMA_STREAM_6,(uint32_t) TestingString.c_str(), LL_USART_DMA_GetRegAddr(USART2),LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
      uartTransmitDMA(TestingString.size());
      vTaskDelay(1000);
    }
  #endif
}

/*In the firmware package, the CRC_usage example runs the CRC checksum code 
computing an array data (DataBuffer) of 256 supported data type. For a full description, 
refer to the file Readme.txt in the CRC_usage folder

//calculates a checksum based on 12V ADC, 72V ADC, Odometer.
//this is because i can iterate through the struct and it is a waste of time.
//Uses CRC-32 polynomial: 0x4C11DB7*/
uint32_t Bluetooth::CRC32MemoryMap(MemoryMap* map){

  LL_CRC_ResetCRCCalculationUnit(CRC);

  //doing it in this order triggers the hardware properly
 
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
  LL_CRC_FeedData32(CRC, map->throttleV);
  LL_CRC_FeedData32(CRC, map->StateMachineStatus);
  return LL_CRC_ReadData32(CRC);
}

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

#ifdef ATTesting
void Bluetooth::ATModeTesting(){
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

#endif