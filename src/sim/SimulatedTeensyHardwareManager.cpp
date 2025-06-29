#ifndef TARGET_TEENSY

#include "sim/SimulatedTeensyHardwareManager.h"
#include <iostream>
#include "defines.h"
#include "Logging.h"
#include <algorithm>
#include <cstring>

static bool s_savedRedLED[NUM_BUTTONS]   = {false};
static bool s_savedGreenLED[NUM_BUTTONS] = {false};
static bool s_prevButtonStates[NUM_BUTTONS] = {false};
static bool s_prevEncoderBtn = false;

void SimulatedTeensyHardwareManager::init() {
    // No physical peripherals to initialise, but we keep the method
    // to satisfy the interface and symmetry with the Teensy build.
}

void SimulatedTeensyHardwareManager::loop() { 
  delay(1);
}

void SimulatedTeensyHardwareManager::gatherControlSettings() {

}

SimulatedTeensyHardwareManager::~SimulatedTeensyHardwareManager() {}

void SimulatedTeensyHardwareManager::loadKnobs() {}
void SimulatedTeensyHardwareManager::saveKnobs() {}

int SimulatedTeensyHardwareManager::getKnobValue(unsigned int knobIX) { 
  return getState()->potentiometers[knobIX] >> 2; // return smoothed value 
}

long SimulatedTeensyHardwareManager::getEncoderZeroTo(long divisor) {
    long val = (getState()->encoderPosition >> 2) % divisor;
    if(val < 0)        val = 0;
    if(val >= divisor) val = divisor - 1;
    return val;
}

bool SimulatedTeensyHardwareManager::getEncoderSwitchStatus() {
  return getState()->encoderPressed; 
}

long SimulatedTeensyHardwareManager::getEncoderValue() { 
  return getState()->encoderPosition; 
}

bool SimulatedTeensyHardwareManager::isButtonPressed(uint index) {
    if(index >= NUM_BUTTONS) {
        log(LOG_ERROR, "SimulatedTeensyHardwareManager::isButtonPressed() index out of range", __func__);
        return false;
    }
    return getState()->buttons[index];
}

bool SimulatedTeensyHardwareManager::buttonStateChanged(uint index,
                                                        bool upThenDown,
                                                        bool clearFlag) {
    if(index >= NUM_BUTTONS) {
        log(LOG_ERROR, "SimulatedTeensyHardwareManager::buttonStateChanged() index out of range", __func__);
        return false;
    }

    bool cur  = getState()->buttons[index];
    bool prev = s_prevButtonStates[index];

    if(cur != prev) {
        bool triggered = false;
        if(upThenDown) {
            // Detect release
            if(!cur && prev) triggered = true;
        } else {
            // Detect press
            if(cur && !prev) triggered = true;
        }
        if(clearFlag) s_prevButtonStates[index] = cur;
        return triggered;
    }
    return false;
}

bool SimulatedTeensyHardwareManager::encoderSwitchStateChanged(bool upThenDown,
                                                               bool clearFlag) {
    bool cur = getState()->encoderPressed;

    if(cur != s_prevEncoderBtn) {
        bool triggered = false;
        if(upThenDown) {
            // Falling edge (press ➜ release)
            if(!cur && s_prevEncoderBtn) triggered = true;
        } else {
            // Rising edge (release ➜ press)
            if(cur && !s_prevEncoderBtn) triggered = true;
        }
        if(clearFlag) s_prevEncoderBtn = cur;
        return triggered;
    }
    return false;
}

void SimulatedTeensyHardwareManager::setButtonLights(uint buttonId,
                                                     bool red,
                                                     bool green) {
    if(buttonId >= NUM_BUTTONS) {
        log(LOG_ERROR, "SimulatedTeensyHardwareManager::setButtonLights() invalid button id", __func__);
        return;
    }
    getState()->redLED[buttonId]   = red;
    getState()->greenLED[buttonId] = green;
}

bool SimulatedTeensyHardwareManager::greenIsLit(uint buttonId) {
    return (buttonId < NUM_BUTTONS) ? getState()->greenLED[buttonId] : false;
}

bool SimulatedTeensyHardwareManager::redIsLit(uint buttonId) {
    return (buttonId < NUM_BUTTONS) ? getState()->redLED[buttonId] : false;
}


void SimulatedTeensyHardwareManager::clearEncoderButton() {
    getState()->encoderPressed = false;
    s_prevEncoderBtn           = false;
}

void SimulatedTeensyHardwareManager::setKnobConfiguration(uint knobIndex,
                                                          const char* name,
                                                          uint8_t cmdbyte,
                                                          uint8_t typecode) {
    if(knobIndex >= NUM_KNOBS) {
        log(LOG_ERROR, "SimulatedTeensyHardwareManager::setKnobConfiguration() invalid knob index", __func__);
        return;
    }

    strncpy(knobConfigurations[knobIndex].name, name,
            sizeof(knobConfigurations[knobIndex].name) - 1);
    knobConfigurations[knobIndex].name[sizeof(knobConfigurations[knobIndex].name) - 1] = 0x00;
    knobConfigurations[knobIndex].cmdbyte  = cmdbyte;
    knobConfigurations[knobIndex].typecode = typecode;
}

void SimulatedTeensyHardwareManager::restoreLedState() {
    for(uint i = 0; i < NUM_BUTTONS; ++i) {
        getState()->redLED[i]   = s_savedRedLED[i];
        getState()->greenLED[i] = s_savedGreenLED[i];
    }
}


void SimulatedTeensyHardwareManager::sendParameter(uint8_t paramID, uint8_t value) {
    char buf[128];
    sprintf(buf, "Simulated sendParameter — ID: %u  Value: %u", paramID, value);
    log(LOG_INFO, buf, __func__);
}

void SimulatedTeensyHardwareManager::saveLedState() {
    for(uint i = 0; i < NUM_BUTTONS; ++i) {
        s_savedRedLED[i]   = getState()->redLED[i];
        s_savedGreenLED[i] = getState()->greenLED[i];
    }
}

void SimulatedTeensyHardwareManager::resetEncoder(uint pos) {
    getState()->encoderPosition = static_cast<int>(pos);
}

knobConfig SimulatedTeensyHardwareManager::getKnobConfiguration(uint index) {
    if(index >= NUM_KNOBS) {
        log(LOG_ERROR, "SimulatedTeensyHardwareManager::getKnobConfiguration() invalid index", __func__);
        return knobConfig{};
    }
    return knobConfigurations[index];
}

int SimulatedTeensyHardwareManager::AsciiToEncoder(char c) {
    return (((static_cast<int>(c)) - 65) * 4) + 132;
}

long SimulatedTeensyHardwareManager::getEncoderModdedBy(long divisor) {
   long l = (getState()->encoderPosition>>2) % divisor;
   if(l<0) l=divisor-1;
   if(l>=divisor) l=0;
   return l;
}


#endif