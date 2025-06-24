#ifndef TARGET_TEENSY

#include "sim/SimulatedTeensyHardwareManager.h"
#include <iostream>
#include "defines.h"

void SimulatedTeensyHardwareManager::init() {}
void SimulatedTeensyHardwareManager::loop() { delay(1); }

SimulatedTeensyHardwareManager::~SimulatedTeensyHardwareManager() {}

void SimulatedTeensyHardwareManager::loadKnobs() {}
void SimulatedTeensyHardwareManager::saveKnobs() {}
int SimulatedTeensyHardwareManager::getKnobValue(unsigned int) { return 0; }
long SimulatedTeensyHardwareManager::getEncoderZeroTo(long d) { return 0; }
bool SimulatedTeensyHardwareManager::getEncoderSwitchStatus() { return false; }
long SimulatedTeensyHardwareManager::getEncoderValue() { return 0; }
bool SimulatedTeensyHardwareManager::isButtonPressed(unsigned int) { return false; }
bool SimulatedTeensyHardwareManager::buttonStateChanged(unsigned int, bool, bool) { return false; }
bool SimulatedTeensyHardwareManager::encoderSwitchStateChanged(bool, bool) { return false; }
void SimulatedTeensyHardwareManager::setButtonLights(unsigned int, bool, bool) {}
bool SimulatedTeensyHardwareManager::greenIsLit(unsigned int) { return false; }
bool SimulatedTeensyHardwareManager::redIsLit(unsigned int) { return false; }
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
  return 0;
}

long SimulatedTeensyHardwareManager::getEncoderModdedBy(long divisor) {
  return 0;
}


#endif