# Smart E-Bike Project

This repository contains the implementation of a Tesla Center Console-like system mounted on an electric motorcycle.

- The Central Computer is an STM32F4 microcontroller running FreeRTOS. It allows all hardware to be controlled via the control cluster of the handlebar of the bike.
- The Central Computer sends Bluetooth data to the Android Tablet.
- The display is an Android Tablet running a mobile app created using React Native
- Controlled hardware includes headlights, tail lights, car horn, NFC scanner and keycard unlock, cool LED lights, human detection alarm, and more!


![image](Docs/Diagrams/SoftwareModuleSummary.png)
<a href="https://github.com/tommy20gun/Bike-STM32/tree/master/CustomDrivers"><strong>Explore the Code » </strong> </a>

![image](Docs/Diagrams/bike.jpg)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### State machine implementation of the Locking/Unlocking

![image](Docs/Diagrams/statemachine.png)
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Microcontroller Pinout made with KiCAD EDA:

![image](Docs/Diagrams/MCUPinout.png)
<a href="https://github.com/tommy20gun/Bike-STM32/tree/master/Docs/Diagrams/Bike_PCB.pdf"><strong>Explore the eCAD » </strong> </a>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE-MIT.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Tommy Xu: xuhaohui20@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>