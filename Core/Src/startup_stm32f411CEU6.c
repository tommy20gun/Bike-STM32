/* refer to startup_stm32f411xe.s for ARM assembly code*/

#include <stdint.h>
#include <system_stm32f4xx.h>

/* address for initialize data, data, bss */
/*flash*/
extern uint32_t  _sidata;
/* ram */
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

extern void __libc_init_array();
extern int main();
void Reset_Handler(void){
	// addresses of 
    uint32_t* sidata = &_sidata;
    uint32_t* sdata = &_sdata;
    uint32_t* edata = &_edata;

    while(sdata < edata){
        *sdata = *sidata;
		sdata++;
		sidata++;
    } 
    
	uint32_t* sbss = &_sbss;
	uint32_t* ebss = &_ebss;
	while(sbss < ebss){
		*sbss = 0;
		sbss++;
	}
	
    SystemInit();
	//find all c++ objects and "execute" constructors
    __libc_init_array();
    main();

    /* code needs to execute if main somehow returns*/
    while(1);

}
//unimplemented interrupts
void DefaultHandler(void){
	while(1);
}

void HardFault_Handler(void){
	while(1);
	// you can reimplement this to get out of hardfault.
}

__attribute__((section(".isr_vectors")))
const void (*VectorTable[]) (void) = {
	const void (*)(void) &_estack,
	const void (*Reset_Handler)(void),
	
};

