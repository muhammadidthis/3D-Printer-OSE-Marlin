See https://wiki.opensourceecology.org/wiki/D3D

# OSE Marlin Firmware for 3D Printer

OSE Marlin is a customized version of the Marlin firmware, widely used for controlling 3D printers and other CNC machines. It is tailored for Open Source Ecology's 3D printers and is part of the open-source hardware initiative. This firmware supports flexible configurations, enabling users to adapt it to various machine setups.

---

## Features
- **Advanced Temperature Control**: PID control for stable hotend and heated bed temperatures.
- **Flexible Configuration**: Supports various build volumes, thermistors, stepper motor setups, and extruders.
- **Bed Leveling Options**: Manual, automatic, and mesh bed leveling for precision prints.
- **Power-Loss Recovery**: Resume prints seamlessly after unexpected power outages.
- **Filament Runout Detection**: Automatically pauses prints when filament runs out
---

## Getting Started

### **1. Prerequisites**
Before proceeding, ensure you have the following:
- A compatible 3D printer(E3D Titan Aero extruder)
- Computer with **Arduino IDE(version 1.6.8 and above)** or **PlatformIO IDE** installed.
- USB cable for connecting the machine to your computer.

---

### **2. Download and Set Up Firmware**

#### Step 1: Download the Firmware
1. Clone or download this repository:
Open Arduino IDE 1.6.8 and open OSE_Marlin.ino file in the folder - that is the firmware for the 3D printer controller board.

### 2. Configure the firmware
1. Navigate te the D3D 1807 Marlin/OSE_Marlin Folder
2. Open Configuration_TitanAero_1804.h and Configuration_TitanAero_lcd.1804.h files in your preferred code editor.
3. Edit the following to your liking:

Printer dimensions: Adjust X_MAX_POS, Y_MAX_POS, and Z_MAX_POS.

Endstops: Configure endstop logic for accurate homing.

Thermal settings: Set thermistor types for hotend and heated bed.

Stepper motors: Specify steps per millimeter for precise movements.

### Step 3: Compile and Upload
1. Open the project in Arduino IDE or PlatformIO.
2. Select your printer's board under Tools > Board. For this project use E3D Titan Aero
3. Compile and upload the firmware to your machine.

---

### Common commands:
Here are some common [G-Code Commands](commands.txt). 
For further exploration and additional commands visit https://cdn.imagearchive.com/homemodelenginemachinist/data/attach/87/87243-Marlin-GCodes7.pdf

### Common problems:
Stepper motors not moving: Check the ENDSTOP_INVERTING settings in Configuration.h.

Thermal runaway: Ensure thermistors are properly connected and their types match the configuration

---
### Contributing:

We welcome contributions to improve OSE Marlin! Please:

1. Fork this repository.
2. Create a branch for your feature:

 ```bash
git checkout -b feature/your-feature-name
```

3. Submit a pull request
---

### Versions:

1. D3D v18.07 Marlin - 0.4 mm nozzle, Prusa i3 MK2 Extruder
2. Marlin - This is the version for a 0.6 mm nozzle, E3D Titan Aero extruder.
3. Former versions of OSE Marlin are found at https://wiki.opensourceecology.org/wiki/File:OSE_Marlin.zip




