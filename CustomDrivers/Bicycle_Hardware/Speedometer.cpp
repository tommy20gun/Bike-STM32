 /** 
  * 
  * @brief          : Driver to interface Odometer
  * 
  * receives BLE data from Fardriver regarding RPM.asm
  * Implements algorithm for converting to Miles traveled
  * Stores persistent value in flash memory
  * 
  */


#include "Speedometer.h"

#define WHEEL_DIAMETER (12.0f) //inches
#define PI (3.14159f)
#define RPM_TO_MPH (60.0f / 63360.0f) // minutes / hour / (inch / mile)
#define COUNTS_PER_REV (1u) // 1 magnet
#define MOVING_AVG_SIZE (5u)
#define MS_PER_MINUTE (60000u)
#define MAX_PULSE_GAP_MS (1000u)

static SemaphoreHandle_t s_speedometer_IRQ_sema;

Speedometer::Speedometer(QueueHandle_t messenger, GPIO_TypeDef* GPIOx, uint32_t PinMaskIn)
{
    this->messenger = messenger;
    inputPin.GPIOx = GPIOx;
    inputPin.PinMask = PinMaskIn;
    s_speedometer_IRQ_sema = xSemaphoreCreateBinary();
    initPeripherals();
    initTasks();
}

void Speedometer::initTasks()
{
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"Speedometer",128, this,1,&TaskHandle);
    if(xReturned != pdPASS){
        Error_Handler();
    }
}

void Speedometer::QueueSend(){
  struct uint32_t_Buffer buff;
  buff.tag = BIKE_SPEED;
  buff.data = (uint32_t) (mph* 100.0f + 0.5f); //2 decimal float conversion
  xQueueSendToBack(messenger, &buff , 0);
}

void Speedometer::initPeripherals()
{
    ErrorStatus success = ERROR;
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin =  inputPin.PinMask;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    success = LL_GPIO_Init(inputPin.GPIOx, &GPIO_InitStruct);
    if (success != 0){
        Error_Handler();
    }
    LL_GPIO_SetPinPull(inputPin.GPIOx, inputPin.PinMask, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetPinMode(inputPin.GPIOx, inputPin.PinMask, LL_GPIO_MODE_INPUT);

    //hardcoded pin 14
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE14);

    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

    EXTI_InitStruct.Line_0_31 = inputPin.PinMask;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;

    uint32_t success_exti = LL_EXTI_Init(&EXTI_InitStruct);
    if (success_exti != 0){
        Error_Handler();
    }

    __disable_irq();
    uint32_t prio = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0);
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, prio);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    __enable_irq();
}

void Speedometer::RTOSImplementation()
{
    uint32_t current_time = 0;
    uint32_t previous_time = 0;

    float rpm_buffer[MOVING_AVG_SIZE] = {0};
    uint8_t buffer_index = 0;
    bool buffer_full = false;
    bool is_first_pulse = true;

    while(1)
    {
        if (xSemaphoreTake(s_speedometer_IRQ_sema, pdMS_TO_TICKS(MAX_PULSE_GAP_MS)) == pdPASS)
        {    
            current_time = xTaskGetTickCount() * (1000 / configTICK_RATE_HZ);
            if (is_first_pulse)
            {
                previous_time = current_time;
                is_first_pulse = false;
                continue; // function ends
            }

            uint32_t milis_elapsed = current_time - previous_time;

            // ignore impossible speeds
            if (milis_elapsed > 10)
            {
                float raw_rpm = calculateRPM(milis_elapsed);
                rpm_buffer[buffer_index] = raw_rpm;
                buffer_index = (buffer_index + 1) % MOVING_AVG_SIZE;
                if (buffer_index == 0)
                {
                    buffer_full = true;
                }

                float smoothed_rpm = getAverage(rpm_buffer, buffer_full ? MOVING_AVG_SIZE : buffer_index);

                this->mph = RPM2mph(smoothed_rpm);
                this->QueueSend();
            }
            previous_time = current_time;
        }
        else
        {
            this->mph = 0.0f;
            this->QueueSend();
            is_first_pulse = true;
            buffer_index = 0;
            buffer_full = false;

            // clear buffer
            for (uint8_t i = 0; i < MOVING_AVG_SIZE; i++) rpm_buffer[i] = 0.0f;
        }
        // __enable_irq(); // irq is disabled inside IRQ
    }
}

float Speedometer::RPM2mph(float smoothed_rpm)
{
    return smoothed_rpm * WHEEL_DIAMETER * PI * RPM_TO_MPH;
}
float Speedometer::calculateRPM(uint32_t milis_elapsed)
{
    if (milis_elapsed < 10)
    {
        return 0.0f;
    }
    return (float) (60000) / (float) (milis_elapsed); 
}

float Speedometer::getAverage(float* buffer, uint8_t count)
{
    if (count == 0) return 0.0f;
    float sum = 0;
    for (uint8_t i = 0; i < count; i++) {
        sum += buffer[i];
    }
    return sum / count;
}

void EXTI15_10_IRQHandler(void)
{
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);
    //wakes up H
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;


    xSemaphoreGiveFromISR(s_speedometer_IRQ_sema, &xHigherPriorityTaskWoken);

    //Calls the next task Immediately instead of next Tick
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Speedometer::vTaskFunction(void* pvParameters){
  Speedometer* speedometer = (Speedometer*) pvParameters;
  speedometer->RTOSImplementation();
}