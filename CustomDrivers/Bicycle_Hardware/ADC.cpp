 /** 
  * @file           : ADC.cpp
  * @brief          : Implementation of driver for ADC
  * Functionality:
  * PA4 will read the voltage of the battery
  * 
  * 
  */
#include "ADC.h"

void ADCDriver::initTasks(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vTaskFunction,"ADCPoll",64, this,1,&TaskHandle);
    if (xReturned != pdPASS){
        Error_Handler();
    }
}

void ADCDriver::initPeripherals(){
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = ADCPinBatt.PinMask | ADCPinThrottle.PinMask;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(ADCPinBatt.GPIOx,&GPIO_InitStruct);

    ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;//multiscan
    LL_ADC_Init(ADC1,&ADC_InitStruct);

    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_1RANK;
    ADC_REG_InitStruct.ContinuousMode =  LL_ADC_REG_CONV_SINGLE;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
    LL_ADC_REG_SetFlagEndOfConversion(ADC1,LL_ADC_REG_FLAG_EOC_UNITARY_CONV);
    ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1),&ADC_CommonInitStruct);

    LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_4);
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_4,LL_ADC_SAMPLINGTIME_112CYCLES);

    LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_2,LL_ADC_CHANNEL_1);
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_4,LL_ADC_SAMPLINGTIME_112CYCLES);
    
}
/*
Single Conversion Group
In Single conversion mode the ADC does one conversion. This mode is started with the
CONT bit at 0 by either:
• setting the SWSTART bit in the ADC_CR2 register (for a regular channel only)
• setting the JSWSTART bit (for an injected channel)
• external trigger (for a regular or injected channel)
Once the conversion of the selected channel is complete:
• If a regular channel was converted:
– The converted data are stored into the 16-bit ADC_DR register
– The EOC (end of conversion) flag is set
– An interrupt is generated if the EOCIE bit is set
• If an injected channel was converted:
– The converted data are stored into the 16-bit ADC_JDR1 register
– The JEOC (end of conversion injected) flag is set
– An interrupt is generated if the JEOCIE bit is set
Then the ADC stops.

Scan mode
This mode is used to scan a group of analog channels.
The Scan mode is selected by setting the SCAN bit in the ADC_CR1 register. Once this bit
has been set, the ADC scans all the channels selected in the ADC_SQRx registers (for
regular channels) or in the ADC_JSQR register (for injected channels). A single conversion
is performed for each channel of the group. After each end of conversion, the next channel
in the group is converted automatically. If the CONT bit is set, regular channel conversion
does not stop at the last selected channel in the group but continues again from the first
selected channel.
If the DMA bit is set, the direct memory access (DMA) controller is used to transfer the data
converted from the regular group of channels (stored in the ADC_DR register) to SRAM
after each regular channel conversion.
The EOC bit is set in the ADC_SR register:
• At the end of each regular group sequence if the EOCS bit is cleared to 0
• At the end of each regular channel conversion if the EOCS bit is set to 1
The data converted from an injected channel are always stored into the ADC_JDRx
registers.

*/
ADCDriver::ADCDriver(QueueHandle_t messenger, GPIO_TypeDef* GPIOxADC, uint32_t PinMaskInBatt, uint32_t PinMaskInThrottle){
    this->messenger = messenger;
    //this->pin4voltage = pin4voltage;
    this->ADCPinBatt.GPIOx = GPIOxADC;
    this->ADCPinBatt.PinMask = PinMaskInBatt;
    this->ADCPinThrottle.GPIOx = GPIOxADC;
    this->ADCPinThrottle.PinMask = PinMaskInThrottle;
    initTasks();
    initPeripherals();
}
void ADCDriver::vTaskFunction(void* pvParameters){
    ADCDriver* adc = (ADCDriver*) pvParameters;
    adc ->RTOSImplementation();
}
static bool pingpong = 0U; 
void ADCDriver::RTOSImplementation(){
    LL_ADC_Enable(ADC1);
    //just in case clear the flag even
    if (LL_ADC_IsActiveFlag_EOCS(ADC1)){
        LL_ADC_ClearFlag_EOCS(ADC1);
    }
    //delay to allow for ADC to enable 
    vTaskDelay(1000);
    if (!LL_ADC_IsEnabled(ADC1)){
        Error_Handler();
    }
    volatile bool eocflag = 0U;
    while(1){
        LL_ADC_REG_StartConversionSWStart(ADC1);
        vTaskDelay(100); //polling rate 1hz
        //wait for complete
        eocflag = LL_ADC_IsActiveFlag_EOCS(ADC1);
        if (eocflag)
        {
            if (pingpong == 0U)
            {
                raw_adc_read_batt = LL_ADC_REG_ReadConversionData12(ADC1); //rank 1
                pingpong ^= 1U;
            }
            else
            {
                raw_adc_read_throttle_V = LL_ADC_REG_ReadConversionData12(ADC1); //rank 2
                pingpong ^= 1U;
            }
            LL_ADC_ClearFlag_EOCS(ADC1);
        }
        QueueSend();
    }
}

void ADCDriver::QueueSend(){
    struct uint32_t_Buffer buffer;
    if (pingpong == 0U)
    {
        buffer.tag = VOLTAGE_12V_BATT;
        float volt_temp = raw2Voltage(raw_adc_read_batt, 4.0f);
        buffer.data = (uint32_t)(volt_temp * 3.0f * 100.0f + 0.5f); // add back cell in series, move decimal right 2 and round
        xQueueSendToBack(messenger,&buffer,0);

        buffer.tag = PERCENT_12V_BATT;
        buffer.data = (uint32_t)(ADCToBatteryPercent(volt_temp)* 100.0f + 0.5f); // move decimal right 2 and round
        xQueueSendToBack(messenger,&buffer,0);
    }
    else
    {
        buffer.tag = THROTTLE_V;
        buffer.data =((float)raw_adc_read_throttle_V * 3.33f / 4095.0f * 100.0f + 0.5f);
        xQueueSendToBack(messenger,&buffer,0);
    }
}

float ADCDriver::raw2Voltage(uint16_t raw, float scale)
{
    float vdd = 3.33; //you might have to calibrate this to the voltage of the MCU
    float resolution = 4095;
    float cellsInSeries = 3;
    float voltage = raw/resolution*vdd*scale; //assume 12bit resolution, 0-3.3V ADC range, yields 0-4.2V range
    float voltagePerCell = voltage/cellsInSeries;

    return voltagePerCell;
}

float ADCDriver::ADCToBatteryPercent(float voltagePerCell){
    //coeff
    //voltage > 3.63
    float a = 1839.98366f;
    float b = -28678.5498f;
    float c = 167243.8687f;
    float d = -432295.73505f;
    float e = 417750.60866f;

    //voltage < 3.63
    float a1 = 49.2366f;
    float b1 = -3.5f;

    //x = 1839.98366*y^4 - 28678.5498*y^3 + 167243.8687*y^2 - 432295.73505*y + 417750.60866 (y in range of 3.685-4.2V)
    //x = 49.2366*(y-3.5) (y < 3.685)
    float batteryPercentage;
    if (voltagePerCell > 3.685f){
        batteryPercentage = (((((a*voltagePerCell+b)*voltagePerCell) + c)*voltagePerCell + d) * voltagePerCell) + e;
    }
    else if (voltagePerCell <= 3.685f && voltagePerCell > 3.2f){
        batteryPercentage = a1*(voltagePerCell+b1);
    }
    else {
        batteryPercentage = -1.0f;//Error bc battery% cant be negative
    }
    return batteryPercentage;
}

