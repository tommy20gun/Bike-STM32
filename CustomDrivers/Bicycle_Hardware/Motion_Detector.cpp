 /** 
  * 
  * @brief          : Driver to control Motion Detector
  * 
  * Driver to control Motion Detector circuit. 
  * Implementation of the detection threshold algorithm
  * 
  * GPIO EXTI 1- PB13
  * GPIO EXTI 2- PB14
  * GPIO EXTI 3- PB15
  * 
  * account for standby phase of 1 minute on startup
  * Pin going low is "detected motion"
  * there is a delay of 3 seconds (blocked after the pin going back HIGH)
  * THere is an adjustable time delay during the last time motion is detected BEFORE pin going high again. 
  * 
  * Settings on breakoutboard:
  * Repeat Trigger
  * 3 second delay
  * 3 meters
  *
  */