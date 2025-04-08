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
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_3|LL_GPIO_PIN_4;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
    LL_GPIO_Init(GPIOA,&GPIO_InitStruct);

    SPI_InitStruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 10; // not used
    LL_SPI_Init(SPI3,&SPI_InitStruct);
    LL_SPI_SetStandard(SPI3,LL_SPI_PROTOCOL_MOTOROLA);

    NVIC_SetPriority(SPI3_IRQn, 5);
    NVIC_EnableIRQ(SPI3_IRQn);

    LL_SPI_EnableIT_RXNE(SPI3);

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
    bsem = xSemaphoreCreateBinary();

}

void Lock::vLockFunction(void* PvParameters){
    Lock* SPILock = (Lock*) PvParameters;
    uint8_t buffer[100] = {0};
    uint32_t i;
    
    while(1){
        //receive buffer


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

void chipSelect(bool status){
    if (!status){
        LL_GPIO_SetOutputPin(GPIOB, GPIO_PIN_12);
    }
    else{    
        LL_GPIO_ResetOutputPin(GPIOB, GPIO_PIN_12);
    }
}

void Lock:: sendByte(uint8_t buff){

}

void Lock:: receiveByte(uint8_t* dest){
    /*1. Set the RXONLY bit in the SPI_CR1 register.
    2. Enable the SPI by setting the SPE bit to 1:
    a) In master mode, this immediately activates the generation of the SCK clock, and
    data are serially received until the SPI is disabled (SPE=0).
    b) In slave mode, data are received when the SPI master device drives NSS low and
    generates the SCK clock.
    3. Wait until RXNE=1 and read the SPI_DR register to get the received data (this clears
    the RXNE bit). Repeat this operation for each data item to be received.
    This procedure can also be implemented using dedicated interrupt subroutines launched at
    each rising edge of the RXNE flag.
    Note: If it is required to disable the SPI after the last transfer, follow the recommendation
    described in Section 20.3.8: Disabling the SPI.*/

    xSemaphoreTake(bsem,portMAX_DELAY);
    *dest = LL_SPI_ReceiveData8(SPI3);
}

void Lock::sendCommandFrame(int command){
    /*
    Transmit-only procedure (BIDIMODE=0 RXONLY=0)
In this mode, the procedure can be reduced as described below and the BSY bit can be
used to wait until the completion of the transmission (see Figure 201 and Figure 202).
1. Enable the SPI by setting the SPE bit to 1.
2. Write the first data item to send into the SPI_DR register (this clears the TXE bit).
3. Wait until TXE=1 and write the next data item to be transmitted. Repeat this step for
each data item to be transmitted.
4. After writing the last data item into the SPI_DR register, wait until TXE=1, then wait until
BSY=0, this indicates that the transmission of the last data is complete.
This procedure can be also implemented using dedicated interrupt subroutines launched at
each rising edge of the TXE flag.
Note: During discontinuous communications, there is a 2 APB clock period delay between the
write operation to SPI_DR and the BSY bit setting. As a consequence, in transmit-only
mode, it is mandatory to wait first until TXE is set and then until BSY is cleared after writing
the last data.
After transmitting two data items in transmit-only mode, the OVR flag is set in the SPI_SR
register since the received data are never read.
*/
    LL_SPI_SetTransferDirection(SPI3,LL_SPI_HALF_DUPLEX_TX);
    chipSelect(true);
    LL_SPI_Enable(SPI3);
    while (1){ //not empty
        while(LL_SPI_IsActiveFlag_TXE);
        Lock::sendByte(1);
    }
    while(LL_SPI_IsActiveFlag_TXE && !LL_SPI_IsActiveFlag_BSY);
     
    

    LL_SPI_Disable(SPI3);
    chipSelect(false);
}

    


