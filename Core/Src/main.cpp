 /** 
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  */


#include <main.h> //includes LL

void job1(void const* args){
  while(1){
    osDelay(1000);
    LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_13);
  }
}

void RTOS_Setup(void){
  //TODO make sure we are not using systick
  osKernelInitialize();
  osThreadDef(job1, osPriorityNormal, 1, 0);
  osThreadCreate(osThread(job1),NULL);
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
  RTOS_Setup();
  osKernelStart();
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}