#  ESP32 Industrial Dual-Channel PT100 & Power Monitor

##  Overview

This project is designed for field engineers and technicians who need to monitor temperature and power parameters simultaneously. It utilizes the **ESP32** microcontroller to process data from two **PT100 RTD sensors** (via MAX31865) and an **INA219** current sensor. The data is displayed on a crisp **2.8" ILI9341 TFT screen** with a custom graphical user interface.

##  Key Features

* **Dual Temperature Monitoring:** High-precision reading from 2x PT100 sensors using MAX31865 (SPI).
* **Power Analysis:** Real-time current monitoring (mA) for external loads via INA219 (I2C).
* **Battery Management:**
    * Integrated 3.7V Li-Po battery support.
    * On-board charging (TP4056) and Boost Converter (MT3608) for stable 5V system power.
    * **Smart Battery Level:** ADC-based voltage monitoring with a voltage divider (100kΩ/100kΩ) and software calibration.
* **User Interface:**
    * Custom boot logo (Splash Screen).
    * Color-coded alerts (Green/Yellow/Red) for battery levels and sensor status.
* **PCB Design:** Schematic and PCB layout created in **Altium Designer**.

##  Project Structure

```text
├── Software/          # Arduino source codes (.ino) and header files
├── Hardware/          # Altium Designer Project files
└── Docs/              # Documentation, schematics PDF, and images