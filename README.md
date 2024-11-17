# 3D-Printer

See https://wiki.opensourceecology.org/wiki/D3D

Added Instructions:

# OSE Marlin Firmware

OSE Marlin is a customized version of the Marlin firmware, widely used for controlling 3D printers and other CNC machines. It is tailored for Open Source Ecology's 3D printers and is part of the open-source hardware initiative. This firmware supports flexible configurations, enabling users to adapt it to various machine setups.

---

## Features
- **Customizable Configuration**: Easily adjust for different printer specifications (build volume, thermistors, stepper motors, etc.).
- **CNC and Laser Support**: Includes features for CNC tools and laser engravers.
- **Advanced Calibration Tools**: Supports auto bed leveling, Z-offset adjustments, and skew compensation.
- **Community Driven**: Continually improved by a dedicated open-source community.

---

## Getting Started

### **1. Prerequisites**
Before proceeding, ensure you have the following:
- A compatible 3D printer or CNC machine.
- Computer with **Arduino IDE** or **PlatformIO IDE** installed.
- USB cable for connecting the machine to your computer.

---

### **2. Download and Set Up Firmware**

#### Step 1: Download the Firmware
1. Clone or download the repository:
   ```bash
   git clone https://github.com/<your-fork>/OSE-Marlin.git





Original:

Download the folder of Files in Arduino language for OSE's 3D printer - by clicking the green button labeled Clone or Download, then download the zip. 

Open Arduino IDE 1.6.8 and open OSE_Marlin.ino file in the folder - that is the firmware for the 3D printer controller board.

Versions:

1. D3D v18.07 Marlin - 0.4 mm nozzle, Prusa i3 MK2 Extruder
2. Marlin - This is the version for a 0.6 mm nozzle, E3D Titan Aero extruder.
3. Former versions of OSE Marlin are found at ttps://wiki.opensourceecology.org/wiki/File:OSE_Marlin.zip


NOTES:

Note that D3D 1807 Marlin/OSE_Marlin does not have OSE_Marlin.ino file, whereas the other folder - OSE_Marlin - does have OSE_Marlin.ino. What gives?



