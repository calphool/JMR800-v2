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

  uint ctr = 0;
  strcpy(knobConfigurations[ctr].name, "DCO1 Range");     knobConfigurations[ctr].cmdbyte = 0x80; knobConfigurations[ctr].typecode = TYPE_CODE_RANGE;         ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO1 Waveform");  knobConfigurations[ctr].cmdbyte = 0x81; knobConfigurations[ctr].typecode = TYPE_CODE_WAVE_FORM;     ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO1 Tune");      knobConfigurations[ctr].cmdbyte = 0x82; knobConfigurations[ctr].typecode = TYPE_CODE_OCTAVE;        ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO1 LFO Depth"); knobConfigurations[ctr].cmdbyte = 0x83; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO1 Env Depth"); knobConfigurations[ctr].cmdbyte = 0x84; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;

  strcpy(knobConfigurations[ctr].name, "DCO2 Range");     knobConfigurations[ctr].cmdbyte = 0x85; knobConfigurations[ctr].typecode = TYPE_CODE_RANGE;         ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO2 Waveform");  knobConfigurations[ctr].cmdbyte = 0x86; knobConfigurations[ctr].typecode = TYPE_CODE_WAVE_FORM;     ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO Crossmod");   knobConfigurations[ctr].cmdbyte = 0x87; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO2 Tune");      knobConfigurations[ctr].cmdbyte = 0x88; knobConfigurations[ctr].typecode = TYPE_CODE_OCTAVE;        ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO2 Fine Tune"); knobConfigurations[ctr].cmdbyte = 0x89; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO2 LFO Depth"); knobConfigurations[ctr].cmdbyte = 0x8A; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO2 Env Depth"); knobConfigurations[ctr].cmdbyte = 0x8B; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;

  strcpy(knobConfigurations[ctr].name, "DCO Dynamics");   knobConfigurations[ctr].cmdbyte = 0x8F; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  strcpy(knobConfigurations[ctr].name, "DCO EG Mode");    knobConfigurations[ctr].cmdbyte = 0x90; knobConfigurations[ctr].typecode = TYPE_CODE_MODE;          ctr++;

  strcpy(knobConfigurations[ctr].name, "Mix DCO1");       knobConfigurations[ctr].cmdbyte = 0x91; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "Mix DCO2");       knobConfigurations[ctr].cmdbyte = 0x92; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "Mix Env");        knobConfigurations[ctr].cmdbyte = 0x93; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "Mix EG Dynamic"); knobConfigurations[ctr].cmdbyte = 0x94; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  strcpy(knobConfigurations[ctr].name, "Mix EG Mode");    knobConfigurations[ctr].cmdbyte = 0x95; knobConfigurations[ctr].typecode = TYPE_CODE_MODE;          ctr++;

  strcpy(knobConfigurations[ctr].name, "VCF HP Filter");  knobConfigurations[ctr].cmdbyte = 0x96; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  strcpy(knobConfigurations[ctr].name, "VCF Cutoff Frq"); knobConfigurations[ctr].cmdbyte = 0x97; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "VCF Resonance");  knobConfigurations[ctr].cmdbyte = 0x98; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "VCF LFO");        knobConfigurations[ctr].cmdbyte = 0x99; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "VCF Env");        knobConfigurations[ctr].cmdbyte = 0x9A; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "VCF Key Follow"); knobConfigurations[ctr].cmdbyte = 0x9B; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "VCF EG Dynamic"); knobConfigurations[ctr].cmdbyte = 0x9C; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  strcpy(knobConfigurations[ctr].name, "VCF EG Mode");    knobConfigurations[ctr].cmdbyte = 0x9D; knobConfigurations[ctr].typecode = TYPE_CODE_MODE;          ctr++;

  strcpy(knobConfigurations[ctr].name, "VCA Level");      knobConfigurations[ctr].cmdbyte = 0x9E; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "VCA EG Dynamic"); knobConfigurations[ctr].cmdbyte = 0x9F; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  strcpy(knobConfigurations[ctr].name, "VCA EG Mode");    knobConfigurations[ctr].cmdbyte = 0xAF; knobConfigurations[ctr].typecode = TYPE_CODE_ENV2_GATE;     ctr++;

  strcpy(knobConfigurations[ctr].name, "LFO Waveform");   knobConfigurations[ctr].cmdbyte = 0xA1; knobConfigurations[ctr].typecode = TYPE_CODE_LFO_WAVE_FORM; ctr++;
  strcpy(knobConfigurations[ctr].name, "LFO Delay");      knobConfigurations[ctr].cmdbyte = 0xA2; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "LFO Rate");       knobConfigurations[ctr].cmdbyte = 0xA3; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "LFO Sync");       knobConfigurations[ctr].cmdbyte = 0xD0; knobConfigurations[ctr].typecode = TYPE_CODE_2_1_OFF;       ctr++;
  
  strcpy(knobConfigurations[ctr].name, "EG Env1 Attk");   knobConfigurations[ctr].cmdbyte = 0xA4; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env1 Decay");  knobConfigurations[ctr].cmdbyte = 0xA5; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env1 Sust");   knobConfigurations[ctr].cmdbyte = 0xA6; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env1 Rel");    knobConfigurations[ctr].cmdbyte = 0xA7; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env1 Key");    knobConfigurations[ctr].cmdbyte = 0xA8; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env2 Attk");   knobConfigurations[ctr].cmdbyte = 0xA9; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env2 Decay");  knobConfigurations[ctr].cmdbyte = 0xAA; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env2 Sust");   knobConfigurations[ctr].cmdbyte = 0xAB; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env2 Rel");    knobConfigurations[ctr].cmdbyte = 0xAC; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "EG Env2 Key");    knobConfigurations[ctr].cmdbyte = 0xAD; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  
  strcpy(knobConfigurations[ctr].name, "PWM Mode");       knobConfigurations[ctr].cmdbyte = 0xD6; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "PMW1 Width");     knobConfigurations[ctr].cmdbyte = 0xB0; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "PWM1 Env");       knobConfigurations[ctr].cmdbyte = 0xB1; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "PMW1 LFO");       knobConfigurations[ctr].cmdbyte = 0xB2; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "PWM2 Width");     knobConfigurations[ctr].cmdbyte = 0xB3; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "PWM2 Env");       knobConfigurations[ctr].cmdbyte = 0xB4; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "PWM2 LFO");       knobConfigurations[ctr].cmdbyte = 0xB5; knobConfigurations[ctr].typecode = TYPE_CODE_0_TO_10;       ctr++;
  strcpy(knobConfigurations[ctr].name, "PWM Dyna");       knobConfigurations[ctr].cmdbyte = 0xCE; knobConfigurations[ctr].typecode = TYPE_CODE_3_2_1_OFF;     ctr++;
  
  strcpy(knobConfigurations[ctr].name, "Chorus");         knobConfigurations[ctr].cmdbyte = 0xA0; knobConfigurations[ctr].typecode = TYPE_CODE_2_1_OFF;       ctr++;

  strcpy(knobConfigurations[ctr].name, "Unassigned");     knobConfigurations[ctr].cmdbyte = 0x00; ctr++;
  strcpy(knobConfigurations[ctr].name, "Unassigned");     knobConfigurations[ctr].cmdbyte = 0x00; ctr++;
  strcpy(knobConfigurations[ctr].name, "Unassigned");     knobConfigurations[ctr].cmdbyte = 0x00; ctr++;
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

uint16_t SimulatedTeensyHardwareManager::getLastTouchedKnob() const {
    return _lastTouchedKnob;
};

void SimulatedTeensyHardwareManager::setLastTouchedKnob(uint16_t i) {
    _lastTouchedKnob = i;
};

void SimulatedTeensyHardwareManager::clearLastTouchedKnob() {
    _lastTouchedKnob = UINT16_MAX;
};

bool SimulatedTeensyHardwareManager::knobValueChanged(uint i) {
  if(oldSimHardwareState.potentiometers[i] == simHardwareState.potentiometers[i])
    return false;

  this->cloneCurrentStateToOld();
  return true;
};

void SimulatedTeensyHardwareManager::cloneCurrentStateToOld() {
    for(uint i=0;i<NUM_KNOBS;i++) oldSimHardwareState.potentiometers[i] = simHardwareState.potentiometers[i];
    for(uint i=0;i<NUM_BUTTONS;i++) {
        oldSimHardwareState.buttons[i] = simHardwareState.buttons[i];
        oldSimHardwareState.redLED[i] = simHardwareState.redLED[i];
        oldSimHardwareState.greenLED[i] = simHardwareState.greenLED[i];
    }
    oldSimHardwareState.encoderPosition = simHardwareState.encoderPosition;
    oldSimHardwareState.encoderPressed = simHardwareState.encoderPressed;
}


void SimulatedTeensyHardwareManager::sendParameterToSynth(uint knob)
{
    knobConfig cfg = this->getKnobConfiguration(knob);
    if (cfg.cmdbyte == 0x00)
        return;

    uint8_t kv = getKnobValue(knob);

    switch (cfg.typecode)
    {
        case TYPE_CODE_0_TO_10:
        case TYPE_CODE_OCTAVE:
            // Reverse value: 0 = max, 127 = min
            this->sendParameter(cfg.cmdbyte, 127 - kv);
            return;

        case TYPE_CODE_LFO_WAVE_FORM:
        case TYPE_CODE_2_1_OFF:
            if (kv < 32)
                this->sendParameter(cfg.cmdbyte, 16);   // Random / OFF
            else if (kv < 64)
                this->sendParameter(cfg.cmdbyte, 48);   // Square / 1
            else
                this->sendParameter(cfg.cmdbyte, 96);   // Triangle / 2
            return;

        case TYPE_CODE_ENV2_GATE:
            if (kv < 64)
                this->sendParameter(cfg.cmdbyte, 32);   // Gate
            else
                this->sendParameter(cfg.cmdbyte, 96);   // Normal
            return;

        case TYPE_CODE_RANGE:
        case TYPE_CODE_WAVE_FORM:
        case TYPE_CODE_3_2_1_OFF:
        case TYPE_CODE_MODE:
            if (kv < 32)
                this->sendParameter(cfg.cmdbyte, 16);   // Range 16', Noise, OFF, Env-2-Inverted
            else if (kv < 64)
                this->sendParameter(cfg.cmdbyte, 48);   // Range 8', Sawtooth, 1, Env-2-Normal
            else if (kv < 96)
                this->sendParameter(cfg.cmdbyte, 80);   // Range 4', Pulse, 2, Env-1-Inverted
            else
                this->sendParameter(cfg.cmdbyte, 112);  // Range 2', Square, 3, Env-1-Normal
            return;

        default:
            // Fallback: pass raw value (no transformation)
            this->sendParameter(cfg.cmdbyte, kv);
            return;
    }
}

#endif