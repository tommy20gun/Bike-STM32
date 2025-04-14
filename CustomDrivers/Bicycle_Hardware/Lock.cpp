 /** 
  * 
  * @brief          : Locking function of bike, drivers for NFC Reader, access to states in the state machine.
  * 
  * Locking function of the bike will be the enabling of Fardriver. Fardriver requires 72V to be short on the Enable Pin to operate.
  * Communication with the NFC reader such as passwords, SPI protocol is implemented.
  * Provides commands to access the state machine for Lock and Unlocking
  *
  * SPI communication protocol
  * CLK - PB3
  * MISO - PB4
  * MOSI - PB5
  * Chip select (CS) - PB12
  * 
  * FardriverEN_72V - PA6
  * 
  */

 #include "Lock.h"

 Lock::Lock(TaskHandle_t statetask){
    this->stateMachineHandle = statetask;
 }

 void Lock::initPeripheral(){
    /*Select the BR[2:0] bits to define the serial clock baud rate (see SPI_CR1 register).
    2. Select the CPOL and CPHA bits to define one of the four relationships between the
    data transfer and the serial clock (see Figure 194). This step is not required when the
    TI mode is selected.
    3. Set the DFF bit to define 8- or 16-bit data frame format
    4. Configure the LSBFIRST bit in the SPI_CR1 register to define the frame format. This
    step is not required when the TI mode is selected.
    5. If the NSS pin is required in input mode, in hardware mode, connect the NSS pin to a
    high-level signal during the complete byte transmit sequence. In NSS software mode,
    set the SSM and SSI bits in the SPI_CR1 register. If the NSS pin is required in output
    mode, the SSOE bit only should be set. This step is not required when the TI mode is
    selected.
    6. Set the FRF bit in SPI_CR2 to select the TI protocol for serial communications.
    7. The MSTR and SPE bits must*/
    LL_SPI_InitTypeDef SPI_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
    LL_GPIO_Init(GPIOB,&GPIO_InitStruct);

    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV64;
    SPI_InitStruct.BitOrder = LL_SPI_LSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 10; // not used
    LL_SPI_Init(SPI3,&SPI_InitStruct);
    LL_SPI_SetStandard(SPI3,LL_SPI_PROTOCOL_MOTOROLA);

    NVIC_SetPriority(SPI3_IRQn, 5);
    NVIC_EnableIRQ(SPI3_IRQn);

    //LL_SPI_EnableIT_RXNE(SPI3);
    LL_SPI_EnableIT_ERR(SPI3);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //fardriver
    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void Lock::initTask(){
    BaseType_t xReturned;
    xReturned = xTaskCreate(vLockFunction,"Lock Function",64, this,2,&vLockFunctionHandle);
    if (xReturned != pdPASS){
    Error_Handler();
    }
    bsemRXNE = xSemaphoreCreateBinary();
    bsemTXE = xSemaphoreCreateBinary();

}

void Lock::vLockFunction(void* PvParameters){
    Lock* SPILock = (Lock*) PvParameters;

    
    while(1){
        SPILock->isReady();
        vTaskDelay(1000);

        /*while(i < sizeof(buffer)){
            i = 0;
            xSemaphoreTake(SPILock->bsem,portMAX_DELAY);
            LL_SPI_Enable(SPI3);
            buffer[i] = LL_SPI_ReceiveData8(SPI3);
            while((uint32_t) LL_SPI_IsActiveFlag_BSY == 1U); //wait until not busy
            LL_SPI_Disable(SPI3);
            i++;
        }
        //check password
        for (uint32_t j = 0; j < sizeof(buffer); j++){
            if (buffer[j] != SPILock->unlockCode[j]){
                break; //does not notify task
            }
            //STATE_UNLOCKED is enumeration from main.h describing the state
            xTaskNotify(SPILock->stateMachineHandle, STATE_UNLOCKED, eSetBits);
        }*/
    }
}



void Lock:: receiveCFReturn(uint8_t* dest, int length){
    LL_SPI_Enable(SPI3);
    chipSelect(true);
    while(!LL_SPI_IsActiveFlag_TXE(SPI3));
    LL_SPI_TransmitData8(SPI3, 0x03);
    while (length > 0){//TODO fix

        xSemaphoreTake(bsemRXNE, portMAX_DELAY);
        *dest = LL_SPI_ReceiveData8(SPI3);
        dest++;
        length--;
    }
    //wait 1 SPI Clock cycle before turning off SPI
    waitClockCycle(1);
    LL_SPI_Disable(SPI3);
    chipSelect(false);
}
/*
    typedef enum command{checkRDYFlag,bruh, bruh2};
    char command1[1] = {0x03};
    char command2[5];
    char command3[5];
    char* commandArray[3] = {command1, command2, command3};
*/
void Lock::sendCommandFrame(command cmd){
    chipSelect(true);
    LL_SPI_Enable(SPI3);
    int commandIndex = 0;
    volatile uint8_t buff;
    while (commandArray[cmd][commandIndex] != 0x69){ //end of command magic number
        while(!LL_SPI_IsActiveFlag_TXE(SPI3));
        LL_SPI_TransmitData8(SPI3, commandArray[cmd][commandIndex]);
        LL_SPI_TransmitData8(SPI3, commandArray[cmd][commandIndex]);
        commandIndex++;

    }
    while(!LL_SPI_IsActiveFlag_TXE(SPI3));
    while(LL_SPI_IsActiveFlag_BSY(SPI3));


    LL_SPI_Disable(SPI3);
    chipSelect(false);
}

bool Lock::isReady(){
    volatile uint8_t buff;
    sendCommandFrame(CHECKRDYFLAG);
    chipSelect(true);
    LL_SPI_Enable(SPI3);
    //xSemaphoreTake(bsemRXNE, portMAX_DELAY);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI3));
    buff = LL_SPI_ReceiveData8(SPI3);
    if (buff){
        return true;
    }
    else{
        return false;
    }
    waitClockCycle(1);
    LL_SPI_Disable(SPI3);
    chipSelect(false);
}

void Lock::cardRegistration(){
    //bruh i didnt have to do this
}

void Lock::startAutoPoll(){
    uint8_t buff[20];
    sendCommandFrame(INAUTOPOLL);
    while (1){ //TODO test code only
        receiveCFReturn(buff, 20);
    }
}

void Lock::waitClockCycle(int cycles){
    for (volatile int i = 0; i < 32*cycles; i++) {
        __NOP(); 
    }
}

void Lock::chipSelect(bool status){
    if (!status){
        LL_GPIO_SetOutputPin(GPIOB, GPIO_PIN_12);
    }
    else{    
        LL_GPIO_ResetOutputPin(GPIOB, GPIO_PIN_12);
    }
}


