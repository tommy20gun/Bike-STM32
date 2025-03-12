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

void ADCDriver::vADCPoll(void* pvParameters){
    while(1){
        
    }
}

