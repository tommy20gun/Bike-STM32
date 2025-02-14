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