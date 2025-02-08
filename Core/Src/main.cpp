 /** 
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  */


#include <main.h> //includes LL
#include <cmsis_os.h> 
#include <string>

using namespace std;

void job1_task(void const* args){
  while(1){
    osDelay(1000);
    LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_13);
  }
}

void job2_task(void const* args){
  while(1){
    osDelay(2000);
    HAL_UART_Transmit()
  }
}
static osThreadId job1Handle;
void RTOS_Setup(void){
  //TODO make sure we are not using systick
  osThreadDef(job1,job1_task,osPriorityNormal,1,0);
  job1Handle = osThreadCreate(osThread(job1),NULL);
}

int main(){
  SystemClock_Config();
  GPIO_Setup();
  USART2_Setup();
  RTOS_Setup();
  osKernelStart();
}

/*
void delay (volatile uint32_t clockticks){
  while (clockticks--);
}

void on(){
  GPIOC -> BSRR = (1U << 13);

}
void off(){
  GPIOC -> BSRR = ((1U << 13) << 16);

}*/