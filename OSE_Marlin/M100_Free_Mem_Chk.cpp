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
 * M100 Free Memory Watcher
 * 
 * This code watches the free memory block between the bottom of the heap and the top of the stack.
 * This memory block is initialized and watched via the M100 command.
 * 
 * M100 I Initializes the free memory block and prints vitals statistics about the area
 * M100 F Identifies how much of the free memory block remains free and unused.  It also
 *    detects and reports any corruption within the free memory block that may have
 *    happened due to errant firmware.
 * M100 D Does a hex display of the free memory block along with a flag for any errant
 *    data that does not match the expected value.
 * M100 C x Corrupts x locations within the free memory block.   This is useful to check the
 *    correctness of the M100 F and M100 D commands.
 * 
 * Initial version by Roxy-3DPrintBoard
 */
#define M100_FREE_MEMORY_DUMPER       // Comment out to remove Dump sub-command
#define M100_FREE_MEMORY_CORRUPTOR    // Comment out to remove Corrupt sub-command

#include "Marlin.h"

#ifndef M100_FREE_MEMORY_WATCHER
  #define M100_FREE_MEMORY_WATCHER 1 // Default enabled
#endif

#include <string.h> // For memset()

//
// Utility functions used by M100 to get its work done.
//

char* top_of_stack();
void prt_hex_nibble(unsigned int);
void prt_hex_byte(unsigned int);
void prt_hex_word(unsigned int);
int how_many_E5s_are_here(char*);

// Placeholder implementations for code_seen and code_value_int
bool code_seen(char code) {
  // Replace with actual gcode parsing
  return false;
}

int code_value_int() {
  // Replace with actual gcode value extraction
  return 0;
}

void gcode_M100() {
  static bool m100_not_initialized = true;
  char* sp, *ptr;
  int i, j, n;

  #if ENABLED(M100_FREE_MEMORY_DUMPER)
    if (code_seen('D')) {
      ptr = _brkval ? __brkval : &_bss_end;
      ptr = (char*)((unsigned long)ptr & 0xFFF0); // Align to 16-byte boundary
      sp = top_of_stack();
      sp = (char*)((unsigned long)sp | 0x000F); // Align to 16-byte boundary
      n = sp - ptr;

      SERIAL_ECHOPGM("\nbss_end: ");
      prt_hex_word((unsigned int)ptr);
      SERIAL_ECHOPGM("\nStack Pointer: ");
      prt_hex_word((unsigned int)sp);
      SERIAL_EOL;

      while (ptr < sp) {
        prt_hex_word((unsigned int)ptr);
        SERIAL_CHAR(':');
        for (i = 0; i < 16; i++) {
          prt_hex_byte(*(ptr + i));
          SERIAL_CHAR(' ');
        }
        SERIAL_CHAR('|');
        for (i = 0; i < 16; i++) {
          SERIAL_CHAR(*(ptr + i) == (char)0xE5 ? ' ' : '?');
        }
        SERIAL_EOL;
        ptr += 16;
      }
      return;
    }
  #endif

  if (code_seen('F')) {
    int block_cnt = 0;
    ptr = _brkval ? __brkval : &_bss_end;
    sp = top_of_stack();
    n = sp - ptr;

    for (i = 0; i < n; i++) {
      if (*(ptr + i) == (char)0xE5) {
        j = how_many_E5s_are_here(ptr + i);
        if (j > 8) {
          SERIAL_ECHOPAIR("Found ", j);
          SERIAL_ECHOPGM(" bytes free at 0x");
          prt_hex_word((int)(ptr + i));
          SERIAL_EOL;
          i += j;
          block_cnt++;
        }
      }
    }
    if (block_cnt > 1)
      SERIAL_ECHOLNPGM("\nMemory Corruption detected in free memory area.");
    return;
  }

  #if ENABLED(M100_FREE_MEMORY_CORRUPTOR)
    if (code_seen('C')) {
      int x = code_value_int();
      if (x <= 0) {
        SERIAL_ECHOLNPGM("Invalid corruption count.");
        return;
      }

      SERIAL_ECHOLNPGM("Corrupting free memory block.");
      ptr = _brkval ? __brkval : &_bss_end;
      sp = top_of_stack();
      n = sp - ptr - 64;
      j = n / (x + 1);

      for (i = 1; i <= x; i++) {
        *(ptr + (i * j)) = i;
        SERIAL_ECHOPGM("Corrupting address: 0x");
        prt_hex_word((unsigned int)(ptr + (i * j)));
        SERIAL_EOL;
      }
      return;
    }
  #endif

  if (m100_not_initialized || code_seen('I')) {
    SERIAL_ECHOLNPGM("Initializing free memory block.");
    ptr = _brkval ? __brkval : &_bss_end;
    sp = top_of_stack();
    n = sp - ptr - 64;

    SERIAL_ECHOPGM("Free Memory: ");
    SERIAL_ECHO(n);
    SERIAL_ECHOLNPGM(" bytes initialized.");

    memset(ptr, 0xE5, n);

    for (i = 0; i < n; i++) {
      if (*(ptr + i) != (char)0xE5) {
        SERIAL_ECHOPAIR("? address: ", ptr + i);
        SERIAL_ECHOPAIR(" = ", *(ptr + i));
        SERIAL_EOL;
      }
    }

    m100_not_initialized = false;
    return;
  }
}

char* top_of_stack() {
  char* sp;
  _asm_ volatile ("mov %0, sp" : "=r" (sp)); // Fetch stack pointer
  return sp;
}

void prt_hex_nibble(unsigned int n) {
  if (n <= 9) SERIAL_ECHO(n);
  else if (n < 16) SERIAL_ECHO((char)('A' + n - 10));
  else SERIAL_ECHO('?');
}

void prt_hex_byte(unsigned int b) {
  prt_hex_nibble((b & 0xF0) >> 4);
  prt_hex_nibble(b & 0x0F);
}

void prt_hex_word(unsigned int w) {
  prt_hex_byte((w & 0xFF00) >> 8);
  prt_hex_byte(w & 0x00FF);
}

int how_many_E5s_are_here(char* p) {
  int n;
  for (n = 0; n < 32000; n++) {
    if (*(p + n) != (char)0xE5)
      return n;
  }
  return n;
}