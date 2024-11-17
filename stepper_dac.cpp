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
  stepper_dac.cpp - To set stepper current via DAC

  Part of Marlin

  Copyright (c) 2016 MarlinFirmware

  Marlin is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Marlin is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Marlin.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Marlin.h"

#if ENABLED(DAC_STEPPER_CURRENT)

  #include "stepper_dac.h"

  bool dac_present = false;
  const uint8_t dac_order[NUM_AXIS] = DAC_STEPPER_ORDER;
  uint16_t dac_channel_pct[XYZE];

  int dac_init() {
    #if PIN_EXISTS(DAC_DISABLE)
      OUT_WRITE(DAC_DISABLE_PIN, LOW);  // Enable DAC by setting pin low
    #endif

    if (!mcp4728_init()) {
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Error: Failed to initialize MCP4728 DAC.");
        return -1; // Return error code if initialization fails
    }

    if (mcp4728_simpleCommand(RESET)) {
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Error: MCP4728 DAC RESET command failed.");
        return -1; // Return error code if RESET command fails
    }

    dac_present = true; // Set DAC as present after successful initialization

    if (!mcp4728_setVref_all(DAC_STEPPER_VREF)) {
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Error: Failed to set VREF for MCP4728 DAC.");
        return -1; // Return error code if VREF setup fails
    }

    if (!mcp4728_setGain_all(DAC_STEPPER_GAIN)) {
        SERIAL_ECHO_START;
        SERIAL_ECHOLNPGM("Error: Failed to set Gain for MCP4728 DAC.");
        return -1; // Return error code if Gain setup fails
    }

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("DAC initialization successful.");
    return 0; // Return success code
}

  void dac_current_percent(uint8_t channel, float val) {
    if (!dac_present) return;

    NOMORE(val, 100);

    mcp4728_analogWrite(dac_order[channel], val * 0.01 * (DAC_STEPPER_MAX));
    mcp4728_simpleCommand(UPDATE);
  }

  void dac_current_raw(uint8_t channel, uint16_t val) {
    if (!dac_present) return;

    NOMORE(val, DAC_STEPPER_MAX);

    mcp4728_analogWrite(dac_order[channel], val);
    mcp4728_simpleCommand(UPDATE);
  }

  static float dac_perc(int8_t n) { return 100.0 * mcp4728_getValue(dac_order[n]) * (1.0 / (DAC_STEPPER_MAX)); }
  static float dac_amps(int8_t n) { return mcp4728_getDrvPct(dac_order[n]) * (DAC_STEPPER_MAX) * 0.125 * (1.0 / (DAC_STEPPER_SENSE)); }
  
  int16_t dac_current_get_percent(AxisEnum axis) { return mcp4728_getDrvPct(dac_order[axis]); }
  void dac_current_set_percents(int16_t pct[XYZE]) {
    LOOP_XYZE(i) dac_channel_pct[i] = pct[dac_order[i]];
    mcp4728_setDrvPct(dac_channel_pct);
  }

  void dac_print_values() {
    if (!dac_present) return;

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Stepper current values in % (Amps):");
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR(" X:",  dac_perc(X_AXIS)); 
    SERIAL_ECHOPAIR(" (",   dac_amps(X_AXIS));
    SERIAL_ECHOPAIR(") Y:", dac_perc(Y_AXIS));
    SERIAL_ECHOPAIR(" (",   dac_amps(Y_AXIS));
    SERIAL_ECHOPAIR(") Z:", dac_perc(Z_AXIS));
    SERIAL_ECHOPAIR(" (",   dac_amps(Z_AXIS));
    SERIAL_ECHOPAIR(") E:", dac_perc(E_AXIS));
    SERIAL_ECHOPAIR(" (",   dac_amps(E_AXIS));
    SERIAL_ECHOLN(")");
  }

  void dac_commit_eeprom() {
    if (!dac_present) return;
    mcp4728_eepromWrite();
  }

#endif // DAC_STEPPER_CURRENT
