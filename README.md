# Smart E-Bike Project

This repository contains the implementation of a Tesla Center Console-like system mounted on an electric motorcycle.

- The display is an Android Tablet running a mobile app created using React Native
- Hardware I am controlling includes headlights, tail lights, car horn, NFC scanner and keycard unlock, cool LED lights, human detection alarm, and more!
- The Central Computer is an STM32F4 microcontroller running FreeRTOS. It allows all hardware to be controlled via the control cluster of the handlebar of the bike.
- The Central Computer sends Bluetooth data to the Android Tablet.

![image](Docs/Diagrams/SoftwareModuleSummary.png)

Picture of the E-Bike:


### State machine implementation of the Locking/Unlocking

![image](Docs/Diagrams/statemachine.png)

### Microcontroller Pinout made with KiCAD EDA:

![image](Docs/Diagrams/MCUPinout.png)