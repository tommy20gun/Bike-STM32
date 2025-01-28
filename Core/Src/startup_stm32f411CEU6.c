/* refer to startup_stm32f411xe.s for ARM assembly code*/

#include <stdint.h>
#include <system_stm32f4xx.h>

/* address for initialize data, data, bss */
extern uint32_t  _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern void __libc_init_array();
extern int main();
void Reset_Handler(void){
    uint32_t* sidata = &_sidata;
    uint32_t* sdata = &_sdata;
    uint32_t* edata = &_edata;

    while(sdata < edata){
        *sdata++ = _sdata;
    } 
    
    
    SystemInit();
    __libc_init_array();
    main();

    /* code needs to execute if main somehow returns*/
    while(1);

}