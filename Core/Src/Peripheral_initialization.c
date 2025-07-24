 /** 
  * 
  * @brief          : Perpherial initialization
  * 
  * Setup code for System Clock, RTOS for non hardware specific usage.
  * Specific GPIO, DMA, etc channels will be handeled by the driver of each component.
  *
  * 
  */

#include <Peripheral_initialization.h>

void SystemClock_Config(void)
{
/**
  * @brief System Clock Configuration. STM32Cube Generated
  * @retval None
  */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_12, 96, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  while (LL_PWR_IsActiveFlag_VOS() != 0) //if HAL_Init is not used, must be "!= 0"
  //if HALInit is used must be == 0
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(100000000);

	//use timer1 as time base
   /* Update the time base */
   //TODO Timer is used for HAL to work. it is not an option to get rid of this as 
  // the time base source in CubeMX
  /*if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }*/
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}


void GPIO_GlobalSetup(void){

  //enables all clock for GPIO
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  //enable EXTI
  /*
  int prio = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0);
  NVIC_SetPriority(EXTI15_10_IRQn, prio);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
  NVIC_SetPriority(EXTI9_5_IRQn, prio);
  NVIC_EnableIRQ(EXTI9_5_IRQn);
*/
  
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while(1){
    
  }
  /* USER CODE END Error_Handler_Debug */
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ){
  Error_Handler();
}
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

//Set up thread awareness
#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif
 
const int USED uxTopUsedPriority = configMAX_PRIORITIES - 1;

	
		