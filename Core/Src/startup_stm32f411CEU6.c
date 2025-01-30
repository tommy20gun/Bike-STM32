/* refer to startup_stm32f411xe.s for ARM assembly code*/

#include <stdint.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx.h>

/* address from Linker */
/*flash*/
extern uint32_t  _sidata; //start of data in flash
/* ram */
extern uint32_t _sdata; //start of data in ram
extern uint32_t _edata; //end of data in ram
extern uint32_t _sbss; //start of data in bss
extern uint32_t _ebss; //end of data in bss
extern uint32_t _estack; //stack pointer

//find all c++ objects and "execute" constructors
extern void __libc_init_array();
extern int main();

//main entry point to the program
void Reset_Handler(void){
	
    uint32_t* sidata = &_sidata;
    uint32_t* sdata = &_sdata;
    uint32_t* edata = &_edata;
    //move from flash into static ram
    while(sdata < edata){
        *sdata = *sidata;
		sdata++;
		sidata++;
    } 
    
	uint32_t* sbss = &_sbss;
	uint32_t* ebss = &_ebss;
    //initialize bss as 0
	while(sbss < ebss){
		*sbss = 0;
		sbss++;
	}
	
    SystemInit();
	//find all c++ objects and "execute" constructors
    __libc_init_array();
    main();

    /* code needs to execute if main somehow returns by accident*/
    while(1);

}
//unimplemented interrupts
void DefaultHandler(void){
	while(1);
}



//initialize all interrupt functions but give them the weak attribute so that it will use defaultHandler unless the function is implemented
__weak void   NMI_Handler(void){ DefaultHandler();}
__weak void   HardFault_Handler(void){DefaultHandler();}
__weak void   MemManage_Handler(void){ DefaultHandler();}
__weak void   BusFault_Handler(void){ DefaultHandler();}
__weak void   UsageFault_Handler(void){ DefaultHandler();}
__weak void   SVC_Handler(void){ DefaultHandler();}
__weak void   DebugMon_Handler(void){ DefaultHandler();}
__weak void   PendSV_Handler(void){ DefaultHandler();}
__weak void   SysTick_Handler(void){ DefaultHandler();}

/* External Interrupts */
__weak void      WWDG_IRQHandler                   (void){ DefaultHandler();}/* Window WatchDog              */                                        
__weak void      PVD_IRQHandler                    (void){ DefaultHandler();}/* PVD through EXTI Line detection */                        
__weak void      TAMP_STAMP_IRQHandler             (void){ DefaultHandler();}/* Tamper and TimeStamps through the EXTI line */            
__weak void      RTC_WKUP_IRQHandler               (void){ DefaultHandler();}/* RTC Wakeup through the EXTI line */                      
__weak void      FLASH_IRQHandler                  (void){ DefaultHandler();}/* FLASH                        */                                          
__weak void      RCC_IRQHandler                    (void){ DefaultHandler();}/* RCC                          */                                            
__weak void      EXTI0_IRQHandler                  (void){ DefaultHandler();}/* EXTI Line0                   */                        
__weak void      EXTI1_IRQHandler                  (void){ DefaultHandler();}/* EXTI Line1                   */                          
__weak void      EXTI2_IRQHandler                  (void){ DefaultHandler();}/* EXTI Line2                   */                          
__weak void      EXTI3_IRQHandler                  (void){ DefaultHandler();}/* EXTI Line3                   */                          
__weak void      EXTI4_IRQHandler                  (void){ DefaultHandler();}/* EXTI Line4                   */                          
__weak void      DMA1_Stream0_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream 0                */                  
__weak void      DMA1_Stream1_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream 1                */                   
__weak void      DMA1_Stream2_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream 2                */                   
__weak void      DMA1_Stream3_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream 3                */                   
__weak void      DMA1_Stream4_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream 4                */                   
__weak void      DMA1_Stream5_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream 5                */                   
__weak void      DMA1_Stream6_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream 6                */                   
__weak void      ADC_IRQHandler                    (void){ DefaultHandler();}/* ADC1, ADC2 and ADC3s         */                                 
__weak void      EXTI9_5_IRQHandler                (void){ DefaultHandler();}/* External Line[9:5]s          */                          
__weak void      TIM1_BRK_TIM9_IRQHandler          (void){ DefaultHandler();}/* TIM1 Break and TIM9          */         
__weak void      TIM1_UP_TIM10_IRQHandler          (void){ DefaultHandler();}/* TIM1 Update and TIM10        */         
__weak void      TIM1_TRG_COM_TIM11_IRQHandler     (void){ DefaultHandler();}/* TIM1 Trigger and Commutation and TIM11 */
__weak void      TIM1_CC_IRQHandler                (void){ DefaultHandler();}/* TIM1 Capture Compare         */                          
__weak void      TIM2_IRQHandler                   (void){ DefaultHandler();}/* TIM2                         */                   
__weak void      TIM3_IRQHandler                   (void){ DefaultHandler();}/* TIM3                         */                   
__weak void      TIM4_IRQHandler                   (void){ DefaultHandler();}/* TIM4                         */                   
__weak void      I2C1_EV_IRQHandler                (void){ DefaultHandler();}/* I2C1 Event                   */                          
__weak void      I2C1_ER_IRQHandler                (void){ DefaultHandler();}/* I2C1 Error                   */                          
__weak void      I2C2_EV_IRQHandler                (void){ DefaultHandler();}/* I2C2 Event                   */                          
__weak void      I2C2_ER_IRQHandler                (void){ DefaultHandler();}/* I2C2 Error                   */                            
__weak void      SPI1_IRQHandler                   (void){ DefaultHandler();}/* SPI1                         */                   
__weak void      SPI2_IRQHandler                   (void){ DefaultHandler();}/* SPI2                         */                   
__weak void      USART1_IRQHandler                 (void){ DefaultHandler();}/* USART1                       */                   
__weak void      USART2_IRQHandler                 (void){ DefaultHandler();}/* USART2                       */                   
__weak void      EXTI15_10_IRQHandler              (void){ DefaultHandler();}/* External Line[15:10]s        */                          
__weak void      RTC_Alarm_IRQHandler              (void){ DefaultHandler();}/* RTC Alarm (A and B) through EXTI Line */                 
__weak void      OTG_FS_WKUP_IRQHandler            (void){ DefaultHandler();}/* USB OTG FS Wakeup through EXTI line */                                         
__weak void      DMA1_Stream7_IRQHandler           (void){ DefaultHandler();}/* DMA1 Stream7                 */                          
__weak void      SDIO_IRQHandler                   (void){ DefaultHandler();}/* SDIO                         */                   
__weak void      TIM5_IRQHandler                   (void){ DefaultHandler();}/* TIM5                         */                   
__weak void      SPI3_IRQHandler                   (void){ DefaultHandler();}/* SPI3                         */                   
__weak void      DMA2_Stream0_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 0                */                   
__weak void      DMA2_Stream1_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 1                */                   
__weak void      DMA2_Stream2_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 2                */                   
__weak void      DMA2_Stream3_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 3                */                   
__weak void      DMA2_Stream4_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 4                */                                                       
__weak void      OTG_FS_IRQHandler                 (void){ DefaultHandler();}/* USB OTG FS                   */                   
__weak void      DMA2_Stream5_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 5                */                   
__weak void      DMA2_Stream6_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 6                */                   
__weak void      DMA2_Stream7_IRQHandler           (void){ DefaultHandler();}/* DMA2 Stream 7                */                   
__weak void      USART6_IRQHandler                 (void){ DefaultHandler();}/* USART6                       */                    
__weak void      I2C3_EV_IRQHandler                (void){ DefaultHandler();}/* I2C3 event                   */                          
__weak void      I2C3_ER_IRQHandler                (void){ DefaultHandler();}/* I2C3 error                   */                          
__weak void      FPU_IRQHandler                    (void){ DefaultHandler();}/* FPU                          */
__weak void      SPI4_IRQHandler                   (void){ DefaultHandler();}/* SPI4                         */
__weak void      SPI5_IRQHandler                   (void){ DefaultHandler();}/* SPI5                         */  

//load the interrupt vector table into .isr_vectors location defined by linker
__attribute__((section(".isr_vectors")))
//a vector table of function pointers to each interrupt.
const void (*VectorTable[]) (void) = {
	(const void (*)(void)) &_estack,
	Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    WWDG_IRQHandler                 ,
    PVD_IRQHandler                  ,
    TAMP_STAMP_IRQHandler           ,
    RTC_WKUP_IRQHandler             ,
    FLASH_IRQHandler                ,
    RCC_IRQHandler                  ,
    EXTI0_IRQHandler                ,
    EXTI1_IRQHandler                ,
    EXTI2_IRQHandler                ,
    EXTI3_IRQHandler                ,
    EXTI4_IRQHandler                ,
    DMA1_Stream0_IRQHandler         ,
    DMA1_Stream1_IRQHandler         ,
    DMA1_Stream2_IRQHandler         ,
    DMA1_Stream3_IRQHandler         ,
    DMA1_Stream4_IRQHandler         ,
    DMA1_Stream5_IRQHandler         ,
    DMA1_Stream6_IRQHandler         ,
    ADC_IRQHandler                  ,
    0               				        ,
    0              					        ,
    0                               ,
    0                               ,
    EXTI9_5_IRQHandler              ,
    TIM1_BRK_TIM9_IRQHandler        ,
    TIM1_UP_TIM10_IRQHandler        ,
    TIM1_TRG_COM_TIM11_IRQHandler   ,
    TIM1_CC_IRQHandler              ,
    TIM2_IRQHandler                 ,
    TIM3_IRQHandler                 ,
    TIM4_IRQHandler                 ,
    I2C1_EV_IRQHandler              ,
    I2C1_ER_IRQHandler              ,
    I2C2_EV_IRQHandler              ,
    I2C2_ER_IRQHandler              ,
    SPI1_IRQHandler                 ,
    SPI2_IRQHandler                 ,
    USART1_IRQHandler               ,
    USART2_IRQHandler               ,
    0               				        ,
    EXTI15_10_IRQHandler            ,
    RTC_Alarm_IRQHandler            ,
    OTG_FS_WKUP_IRQHandler          ,
    0                               ,
    0                               ,
    0                               ,
    0                               ,
    DMA1_Stream7_IRQHandler         ,
    0                               ,
    SDIO_IRQHandler                 ,
    TIM5_IRQHandler                 ,
    SPI3_IRQHandler                 ,
    0                               ,
    0                               ,
    0                               ,
    0                               ,
    DMA2_Stream0_IRQHandler         ,
    DMA2_Stream1_IRQHandler         ,
    DMA2_Stream2_IRQHandler         ,
    DMA2_Stream3_IRQHandler         ,
    DMA2_Stream4_IRQHandler         ,
    0                    			      ,
    0              					        ,
    0              					        ,
    0             					        ,
    0              					        ,
    0              					        ,
    OTG_FS_IRQHandler               ,
    DMA2_Stream5_IRQHandler         ,
    DMA2_Stream6_IRQHandler         ,
    DMA2_Stream7_IRQHandler         ,
    USART6_IRQHandler               ,
    I2C3_EV_IRQHandler              ,
    I2C3_ER_IRQHandler              ,
    0                               ,
    0                               ,
    0                               ,
    0                               ,
    0                               ,
    0                               ,
    0                               ,
    FPU_IRQHandler                  ,
    0                               ,
    0                               ,
    SPI4_IRQHandler                 ,
    SPI5_IRQHandler                
};

