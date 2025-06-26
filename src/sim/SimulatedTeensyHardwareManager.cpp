#ifndef TARGET_TEENSY

#include "sim/SimulatedTeensyHardwareManager.h"
#include <iostream>
#include "defines.h"

void SimulatedTeensyHardwareManager::init() {}
void SimulatedTeensyHardwareManager::loop() { delay(1); }

SimulatedTeensyHardwareManager::~SimulatedTeensyHardwareManager() {}

void SimulatedTeensyHardwareManager::loadKnobs() {}
void SimulatedTeensyHardwareManager::saveKnobs() {}

int SimulatedTeensyHardwareManager::getKnobValue(unsigned int knobIX) { 
  return getState()->potentiometers[knobIX] >> 2; // return smoothed value 
}

long SimulatedTeensyHardwareManager::getEncoderZeroTo(long d) { 
  long ll = getState()->encoderPosition ;
  if(ll < 0) ll = 0; // avoid negative values
  return (ll >> 2) % d;
}

bool SimulatedTeensyHardwareManager::getEncoderSwitchStatus() { 
  return getState()->encoderPressed; 
}

long SimulatedTeensyHardwareManager::getEncoderValue() { 
  return getState()->encoderPosition; 
}

bool SimulatedTeensyHardwareManager::isButtonPressed(unsigned int b) {
  return getState()->buttons[b];
}
bool SimulatedTeensyHardwareManager::buttonStateChanged(unsigned int b, bool upThenDown, bool clearFlag) { 
  static bool prevStates[4];

  if(getState()->buttons[b] == prevStates[b])
    return false;
  else {
    prevStates[b] = getState()->buttons[b];
    delay(250);
    return true;
  }
}
bool SimulatedTeensyHardwareManager::encoderSwitchStateChanged(bool upThenDown, bool clearFlag) { 
  static bool prevState = false;
  if(getState()->encoderPressed == prevState)
    return false;
  else {
    prevState = getState()->encoderPressed;
    delay(250);
    return true;
  }
}
void SimulatedTeensyHardwareManager::setButtonLights(unsigned int b, bool red, bool green) {
  getState()->redLED[b] = red;
  getState()->greenLED[b] = green;
}
bool SimulatedTeensyHardwareManager::greenIsLit(unsigned int b) { 
  return getState()->greenLED[b];
}
bool SimulatedTeensyHardwareManager::redIsLit(unsigned int b) { 
  return getState()->redLED[b]; 
}
void SimulatedTeensyHardwareManager::clearEncoderButton() {}
void SimulatedTeensyHardwareManager::setKnobConfiguration(unsigned int, const char*, uint8_t, uint8_t) {}
void SimulatedTeensyHardwareManager::restoreLedState() {}
void SimulatedTeensyHardwareManager::sendParameter(uint8_t, uint8_t) {}

void SimulatedTeensyHardwareManager::saveLedState() {};
void SimulatedTeensyHardwareManager::resetEncoder(uint i) {};

knobConfig SimulatedTeensyHardwareManager::getKnobConfiguration(uint index) {
      return knobConfigurations[index];
}

int SimulatedTeensyHardwareManager::AsciiToEncoder(char c) {
  int v = ((((int)c) - 65) * 4) + 132;
  return v;
}

long SimulatedTeensyHardwareManager::getEncoderModdedBy(long divisor) {
   return getState()->encoderPosition>>2 % divisor;
}


#endif