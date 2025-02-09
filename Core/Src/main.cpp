 /** 
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  */


#include <main.h> //includes LL
#include <string>

using namespace std;

void job1_task(void const* args){
  while(1){
    osDelay(500);
    LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_13);
  }
}

void job2_task(void const* args){
  //const uint8_t buffer[] = "deez nuts";
  //UART_HandleTypeDef* huartaddr = USART2_Setup();// will this work if static is on the other file?
  while(1){
    osDelay(1250);
    LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_13);


    //Uart does not work without Bluetooth LOL
    /*HAL_UART_Transmit(huartaddr,buffer, sizeof(buffer), 1000);*/
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
static osThreadId job1Handle;
static osThreadId job2Handle;
void RTOS_Setup(void){
  //TODO make sure we are not using systick
  osThreadDef(job1,job1_task,osPriorityNormal,1,128);
  job1Handle = osThreadCreate(osThread(job1),NULL);
  osThreadDef(job2, job2_task,osPriorityNormal,1,128);
  job2Handle = osThreadCreate(osThread(job2),NULL);
}

int main(){
  HAL_Init(); //TODO figure out what this does, figure out how to search all 
  SystemClock_Config();
  GPIO_Setup();
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