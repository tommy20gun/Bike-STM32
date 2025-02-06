 /** 
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  */

#include <Peripheral_initialization.h>
#include <cmsis_os.h>

void default_task_handler(void const* args){
  while(1){
    osDelay(10);
  }
}

void RTOS_Setup(void){
  //TODO make sure we are not using systick
  osThreadDef(default_task_handler, osPriorityNormal, 1, 0);
  osThreadCreate(osThread(default_task_handler),NULL);
  
}

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

