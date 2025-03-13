#include "ADC.h"

ADCDriver::ADCDriver(int maxvoltage[]){
    //channel4 (12V), channel5 (72V)
    if (sizeof(maxvoltage)/sizeof(maxvoltage[0]) != 2){
        Error_Handler();
    }
    pin4V = maxvoltage[0];
    pin5V = maxvoltage[1];
}

void ADCDriver::init(){
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LL_GPIO_PIN_4|LL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO; //TODO I need my own Pullup resistor in fritzing
    LL_GPIO_Init(GPIOA,&GPIO_InitStruct);

    ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC_InitStruct.DataAlighment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.SequencerScanMode - LL_ADC_SEQ_SCAN_ENABLE;//multiscan
    LL_ADC_Init(ADC1,&ADC_InitStruct);

    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS;
    ADC_REG_Initstruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_1RANK;
    ADC_REG_InitStruct.ContinuousMode =  LL_ADC_REG_CONV_SINGLE;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
    LL_ADC_REG_SetFlagEndOfConversion(ADC1,LL_ADC_REG_FLAG_EOC_UNITARY_CONV);
    LL_ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1),&ADC_CommonInitStruct);

    LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_4);
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_4,LL_ADC_SAMPLINGTIME_15CYCLES);

    LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_2,LL_ADC_CHANNEL_5);
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_4,LL_ADC_SAMPLINGTIME_15CYCLES);
    
    BaseType_t xReturned;
    xReturned = xTaskCreate(vADCPoll,"ADCPoll",64, this,1,&vADCPollHandle);
    if (xReturned != pdPASS){
    Error_Handler();
    }


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

Discont
This mode is enabled by setting the DISCEN bit in the ADC_CR1 register. It can be used to
convert a short sequence of n conversions (n ≤ 8) that is part of the sequence of conversions
selected in the ADC_SQRx registers. The value of n is specified by writing to the
DISCNUM[2:0] bits in the ADC_CR1 register.
When an external trigger occurs, it starts the next n conversions selected in the ADC_SQRx
registers until all the conversions in the sequence are done. The total sequence length is
defined by the L[3:0] bits in the ADC_SQR1 register.
Example:
• n = 3, channels to be converted = 0, 1, 2, 3, 6, 7, 9, 10
• 1st trigger: sequence converted 0, 1, 2. An EOC event is generated at each
conversion.
• 2nd trigger: sequence converted 3, 6, 7. An EOC event is generated at each
conversion
• 3rd trigger: sequence converted 9, 10.An EOC event is generated at each conversion
• 4th trigger: sequence converted 0, 1, 2. An EOC event is generated at each conversion
Note: When a regular group is converted in discontinuous mode, no rollover occurs.
When all subgroups are converted, the next trigger starts the conversion of the first
subgroup. In the example above, the 4th trigger reconverts the channels 0, 1 and 2 in the
1st subgroup.
*/
void ADCDriver::vADCPoll(void* pvParameters){
    ADCDriver* adcdriver = (ADCDriver*) pvParameters;
    LL_ADC_Enable(ADC1);
    //just in case clear the flag even
    if (LL_ADC_IsActiveFlag_EOCS){
        LL_ADC_ClearFlag_EOCS(ADC1);
    }
    //delay to allow for ADC to enable 
    vTaskDelay(1000);
    if (!LL_ADC_IsEnabled(ADC1)){
        Error_Handler();
    }
    //infiniteLoop
    while(1){
        for (int channel = 4; channel <= 5; channel++){
            LL_ADC_REG_StartConversionSWStart(ADC1);
            //wait for complete
            while(!LL_ADC_IsActiveFlag_EOCS(ADC1));
            adcdriver-> pinreading[channel-4] = LL_ADC_REG_ReadConversionData12(ADC1);
            LL_ADC_ClearFlag_EOCS(ADC1);
        }
        adcdriver->sendDataThroughQueue();
        vTaskDelay(1000); //polling rate 1hz
    }
}

void ADCDriver::sendDataThroughQueue(){
    struct taggedBuffer buffer;
    buffer.tag = ADCreading12V;
    buffer.data = pinreading[0];
    xQueueSendToBack(messenger,&buffer,0);
    buffer.tag = throttleV;
    buffer.data = pinreading[1];
    xQueueSendToBack(messenger,&buffer,0);
}

float ADCDriver::ADCToBatteryPercent(uint16_t ADCReading,int maxvoltage){
    float vdd = 3.33;
    int resolution = 0xFFF;
    float scale = maxvoltage/vdd;
    int cellsInSeries = 3;
    float voltage = ADCReading/resolution*vdd*scale/cellsInSeries; //assume 12bit resolution, 0-3.3V ADC range, yields 0-4.2V range

    //coeff
    //voltage > 3.63
    float a = -500.86;
    int b = 5720;
    int c = -21557;
    int d = 26840;

    //voltage < 3.63
    float a1 = 21.0769;
    int b1 = -3.5;

    //x = -500.86y^3 + 5720y^2 -21557y+26840 (y in range of 3.63-4.2V)
    //x = 21.0769*(y-3.5) (y < 3.63)
    float batteryPercentage;
    if (voltage > 3.63 && voltage < 4.3){
        batteryPercentage = (((a*voltage+b)*voltage) + c)*voltage + d;
    }
    else if (voltage <= 3.63 && voltage > 2){
        batteryPercentage = a1*(voltage+b1);
    }
    else {
        Error_Handler();
    }
    return batteryPercentage;
}

