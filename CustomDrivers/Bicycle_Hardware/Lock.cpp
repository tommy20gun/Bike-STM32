 /** 
  * 
  * @brief          : Locking function of bike, drivers for NFC Reader, access to states in the state machine.
  * 
  * Locking function of the bike will be the enabling of Fardriver. Fardriver requires 72V to be short on the Enable Pin to operate.
  * Communication with the NFC reader such as passwords, SPI protocol is implemented.
  * Provides commands to access the state machine for Lock and Unlocking
  *
  * Lock input-PA7
  * FardriverEN_72V - PA6
  * 
  */

#include "Lock.h"
#include <string.h>

void Lock::initPeripherals(){
    initInputPin(inputPin, true);
    initOutputPin(outputPin, true);
}

void Lock::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"Lock Function",64, this,2,&TaskHandle);
    if (xReturned != pdPASS){
    Error_Handler();
    }
}

void Lock::vTaskFunction(void* pvParameters){
    Lock* lock = (Lock*) pvParameters;
    lock->RTOSImplementation();
}

#define RXBUFF_SIZE (4)
static uint8_t uart_buffer[RXBUFF_SIZE];
static volatile uint8_t i = 0;
static volatile bool six_is_first_char_flag = 0;
static volatile bool disable_uart_isr = 0;
SemaphoreHandle_t UART_RX_sema = NULL;


void USART2_IRQHandler(void)
{
    uint8_t data = 0;
    if (LL_USART_IsActiveFlag_RXNE(USART2))
    {
        data = LL_USART_ReceiveData8(USART2);
    }
    if (!disable_uart_isr)
    {
        if (six_is_first_char_flag || data == 6) //if data[0] is 6, set flag and start saving. Otherwise skip
        {
            i++;
            if (i > RXBUFF_SIZE)
            {
                i = 1;
            }
            uart_buffer[i-1] = data;
            six_is_first_char_flag = 1;
        }
        //break from ISR
        if (i == 4)
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            disable_uart_isr = 1;
            xSemaphoreGiveFromISR(UART_RX_sema, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken)
        }
    }
}

/*
pin high external interrupt
check pin status with debounce
if false positive, pass
if truth, toggle state = 1, delay for 10 seconds

result:
- holding pin for more than 1 second will trigger second state change
- pressing pin for longer than debounce timer (+poll timer) will trigger state change
- pressing not long enough does nothing
*/
void Lock::RTOSImplementation(){
    bool inPinState;
    struct uint32_t_Buffer buff;
    buff.data = 2U;
    buff.tag = queueTag;
    xQueueSendToBack(messenger, &buff , 0);
    UART_RX_sema = xSemaphoreCreateBinary();
    while (buff.data == 2U)
    {
        xSemaphoreTake(UART_RX_sema, portMAX_DELAY); //block the task
        if (uart_buffer[0] == 6 && uart_buffer[1] == 7 && uart_buffer[2] == 6 && uart_buffer[3] == 7)
        {
            buff.data = 1U; //it will exit the loop
            xQueueSendToBack(messenger, &buff , 0);
            /* Set PA3 to input isntead of AF so the interrupt never fires */

        }
        else //reset UART everything and start again
        {
            disable_uart_isr = 0;
            six_is_first_char_flag = 0;
            memset(uart_buffer, 0, RXBUFF_SIZE);
            i = 0;
        }
    }
    //Transition out of start
    while(1){
        //every loop toggles the state
        inPinState = LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask);
        vTaskDelay(100); //debounce and poll rate
        if (LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask) == true && inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask)){
            vTaskDelay(100);
            if (LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask) == true && inPinState == LL_GPIO_IsInputPinSet(inputPin.GPIOx,inputPin.PinMask)){
                ToggleLock();
                buff.data = buff.data ^ 1U; //toggles the first bit
                xQueueSendToBack(messenger, &buff , 0);
                vTaskDelay(1000); //cannot change state again for more than seconds
            }
        }
    }
}

void Lock::ToggleLock(){
    xTaskNotifyGive(stateMachineHandle); //increments notif value by 1.
}
