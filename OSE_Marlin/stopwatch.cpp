/*
 * Marlin 3D Printer Firmware
 * Refactored Stopwatch Class
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "Marlin.h"
#include "stopwatch.h"

enum class StopwatchState { STOPPED, RUNNING, PAUSED };

class Stopwatch {
  private:
    StopwatchState state;
    millis_t startTimestamp;
    millis_t stopTimestamp;
    millis_t accumulator;

    // Debug helper
    static inline void debug(const char func[]) {
      #if ENABLED(DEBUG_STOPWATCH)
        if (DEBUGGING(INFO)) {
          SERIAL_ECHOPGM("Stopwatch::");
          serialprintPGM(func);
          SERIAL_ECHOLNPGM("()");
        }
      #endif
    }

  public:
    Stopwatch();

    bool start();
    bool stop();
    bool pause();
    void reset();

    bool isRunning() const;
    bool isPaused() const;

    millis_t duration(bool inSeconds = true) const;
};

// Constructor using member initializer list
Stopwatch::Stopwatch()
  : state(StopwatchState::STOPPED), startTimestamp(0), stopTimestamp(0), accumulator(0) {
}

// Start the stopwatch
bool Stopwatch::start() {
  debug(PSTR("start"));
  if (state != StopwatchState::RUNNING) {
    if (state == StopwatchState::PAUSED) accumulator = duration(false);
    else reset();

    state = StopwatchState::RUNNING;
    startTimestamp = millis();
    return true;
  }
  return false;
}

// Stop the stopwatch
bool Stopwatch::stop() {
  debug(PSTR("stop"));
  if (state == StopwatchState::RUNNING || state == StopwatchState::PAUSED) {
    state = StopwatchState::STOPPED;
    stopTimestamp = millis();
    return true;
  }
  return false;
}

// Pause the stopwatch
bool Stopwatch::pause() {
  debug(PSTR("pause"));
  if (state == StopwatchState::RUNNING) {
    state = StopwatchState::PAUSED;
    stopTimestamp = millis();
    return true;
  }
  return false;
}

// Reset the stopwatch
void Stopwatch::reset() {
  debug(PSTR("reset"));
  state = StopwatchState::STOPPED;
  startTimestamp = 0;
  stopTimestamp = 0;
  accumulator = 0;
}

// Check if stopwatch is running
inline bool Stopwatch::isRunning() const {
  return state == StopwatchState::RUNNING;
}

// Check if stopwatch is paused
inline bool Stopwatch::isPaused() const {
  return state == StopwatchState::PAUSED;
}

// Get elapsed duration
millis_t Stopwatch::duration(bool inSeconds) const {
  millis_t elapsedTime = ((state == StopwatchState::RUNNING ? millis() : stopTimestamp) 
                          - startTimestamp) + accumulator;
  return inSeconds ? elapsedTime / 1000UL : elapsedTime;
}