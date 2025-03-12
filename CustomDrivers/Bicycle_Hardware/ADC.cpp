#include "ADC.h"

ADCDriver::ADCDriver(){

}

void ADCDriver::init(float maxVoltage){
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_REG_InitTypeDef ADC_REG_InitStruct = {0};
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

    ADC_REG_InitStruct



}