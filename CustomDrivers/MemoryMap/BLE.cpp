 /** 
  * @file           : BLESend.c
  * @brief          : Implementation of BLE polling of memory map, interface with HC05, many BLE send commands
  * 
  * BLEKey/EN - PA0
  * BLEState - PA1
  * USART2TX - PA2
  * USART2RX - PA3
  * 
  */

#include "BLE.h"
 
BLE::BLE(){
  initUART();
  initPeripherials();
  initBLEMemoryMap();
}

void BLE::initUART(){};
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
};
MemoryMap* BLE::initBLEMemoryMap(){
  //memorymap = {
  //  bool* HeadlightON;

  //}
};
void BLE::send05hz(MemoryMap* map){}
void BLE::send50hz(MemoryMap* map){}
void BLE::sendSpecialCommand(MemoryMap* map){}
MemoryMap* BLE::BLERead( MemoryMap* map){}
