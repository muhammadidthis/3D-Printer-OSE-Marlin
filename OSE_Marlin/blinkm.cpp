/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * blinkm.cpp - Library for controlling a BlinkM over i2c
 * Created by Tim Koster, August 21 2013.
 */

#include "Marlin.h"

#if ENABLED(BLINKM)

#include "blinkm.h"

void BlinkMInit() {
  Wire.begin();  // Initialize I2C communication once
}

void SendColors(byte red, byte grn, byte blu, byte address = 0x09) {
  Wire.beginTransmission(address);
  Wire.write('o'); 
  Wire.write('n');
  Wire.write(red);
  Wire.write(grn);
  Wire.write(blu);

  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.print("I2C Error: ");
    Serial.println(error);  // Error handling for communication
  }
}

#endif //BLINKM

