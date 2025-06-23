/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/



/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
//void TIM1_UP_TIM10_IRQHandler(void)
//{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
// HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
//} 


//IT function
void DMA1_Stream6_IRQHandler(void)
{
  //you cannot breakpoint this lol
  if(LL_DMA_IsActiveFlag_TC6(DMA1) == 1){
    LL_DMA_ClearFlag_TC6(DMA1);
  }
}

/* USER CODE BEGIN 1 */
/**
  * @brief This function handles EXTI line[15:10] interrupts.
  * EXTI for GPIOs must be in main
  * To put this function in each class, a static pointer to itself is required.
  * https://stackoverflow.com/questions/17966201/what-are-the-consequences-of-having-a-static-pointer-to-this
  * 
  * Then you create a static IRQ handler in an abstract class that takes the static instance pointer, 
  * inherit all objects from that class to have the same function.
  * which I will not do ever.
  * 
  * Must obtain object instance in this file to use the semaphore if these functions were to be used.
  */
 /*
void EXTI15_10_IRQHandler(void){
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10);
    xSemaphoreGiveFromISR(headlight.bsem, &xHigherPriorityTaskWoken);
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_11) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_11);
    xSemaphoreGiveFromISR(horn.bsem, &xHigherPriorityTaskWoken);
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_12) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_12);
    xSemaphoreGiveFromISR(taillight.bsembrake, &xHigherPriorityTaskWoken);
  }
   //Calls the next task Immediately instead of next Tick
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void EXTI9_5_IRQHandler(void){
  //wakes up H
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
    //if there is no Semaphore to take, then give it
    xSemaphoreGiveFromISR(taillight.bsemleft, &xHigherPriorityTaskWoken);
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
    xSemaphoreGiveFromISR(taillight.bsemright, &xHigherPriorityTaskWoken);
  }
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
}

void USART2_IRQHandler(void){
};

void SPI3_IRQHandler(void){

}*/
/* USER CODE END 1 */
