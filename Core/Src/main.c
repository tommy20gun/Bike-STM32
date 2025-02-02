 /** 
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  */

#include <Peripheral_initialization.h>

void delay (volatile uint32_t clockticks){
  while (clockticks--);
}

void on(){
  GPIOC -> BSRR = (1U << 13);

}
void off(){
  GPIOC -> BSRR = ((1U << 13) << 16);

}
int main(){

  RCC_Enable_HSE();
  GPIO_Setup();
    while (1){
      on();
      delay(10000000);
      off();
      delay(10000000);
    }
}

