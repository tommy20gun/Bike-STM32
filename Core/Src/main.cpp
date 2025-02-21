 /** 
  * 
  * @brief          : Main program body
  * 
  * This is the main body of the program. The MCU will run setup code such as Clock config, ISRVector table, and start the RTOS.
  * Implements state machine.
  *
  * 
  */


#include <main.h> //includes LL
#include <string>

using namespace std;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (GPIO_Pin == GPIO_PIN_10){
      //headlight TODO make correct semaphore
      //xSemaphoreGiveFromISR(bsemaphore, &xHigherPriorityTaskWoken);
    }
    else if (GPIO_Pin == GPIO_PIN_11){
      //horn TODO make correct semaphore
      xSemaphoreGiveFromISR(bsem_horn, &xHigherPriorityTaskWoken);
    }
    //Calls the next task Immediately instead of next Tick
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


void RTOS_Setup(void){
  //TODO make sure we are not using systick
  hornSetup();
}

int main(){
  HAL_Init(); //TODO figure out what this does, figure out how to search all 
  SystemClock_Config();
  GPIO_Setup();
  RTOS_Setup();
  vTaskStartScheduler();
  
}

//static TaskHandle_t job2Handle;
/*void UART_task(void const* args){
  //const uint8_t buffer[] = "deez nuts";
  //UART_HandleTypeDef* huartaddr = USART2_Setup();// will this work if static is on the other file?
  while(1){
    osDelay(1250);
    LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_13);


    //Uart does not work without Bluetooth LOL
    //HAL_UART_Transmit(huartaddr,buffer, sizeof(buffer), 1000);
  }
}
*/

/*
BareMetal Scratch work
void delay (volatile uint32_t clockticks){
  while (clockticks--);
}

void on(){
  GPIOC -> BSRR = (1U << 13);

}
void off(){
  GPIOC -> BSRR = ((1U << 13) << 16);

}*/