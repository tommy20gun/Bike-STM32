#include <Peripheral_initialization.h>

	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/

//TODO do I need to set ifdef cplusplus etc
#define PLLN 96
#define PLLM 12

void RCC_Enable_HSE(void){
	RCC-> CR |= RCC_CR_HSEON; //turn on HSE Clock
	while (!(RCC -> CR & RCC_CR_HSERDY)); //when HSE is ready then move on

	RCC->AHB1ENR |= RCC_APB1ENR_PWREN; //enable the Peripheral clock
	PWR->CR |= PWR_CR_VOS; //enable voltage regulator

	//Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH-> ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS ;

	//Configure the PRESCALARS HCLK, PCLK1, PCLK2
	RCC->CFGR |=  RCC_CFGR_PPRE1_DIV2; 
	RCC->CFGR &= ~(RCC_CFGR_HPRE_3 | RCC_CFGR_PPRE2_2);

	//set PLLP to 2, set PLLM to 12, set PLLN to 96, 
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |= (PLLN << 6U)|(PLLM << 0);
	//source to HSE
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	//turn on PLL and wait to be ready
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	//	7. Select the Clock Source (PLL_P) and wait for it to be set
	RCC->CFGR |=  (1UL << 1);
	while (!(RCC->CFGR & (1UL<<3)));
	}

void GPIO_Setup(void){
	//enable GPIO C13 as PP Output
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC -> MODER |= GPIO_MODER_MODE13_0;
	GPIOC -> OTYPER = 0;
	GPIOC -> OSPEEDR = 0;
}

void USART2_Setup(void){
	
}




	
		