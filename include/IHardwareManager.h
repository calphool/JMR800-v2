#pragma once

#include <stdint.h>
#include "defines.h"

typedef unsigned int uint;

class IHardwareManager {
public:
    virtual void init() = 0;
    virtual void loop() = 0;
    virtual void loadKnobs() = 0;
    virtual void saveKnobs() = 0;
    virtual int  getKnobValue(uint index) = 0;
    virtual long getEncoderZeroTo(long divisor) = 0;
    virtual bool getEncoderSwitchStatus() = 0;
    virtual long getEncoderValue() = 0;
    virtual bool isButtonPressed(uint index) = 0;
    virtual bool buttonStateChanged(uint index, bool bDirection, bool bClearFlag) = 0;
    virtual bool encoderSwitchStateChanged(bool upThenDown, bool clearFlag) = 0;
    virtual void setButtonLights(uint buttonId, bool red, bool green) = 0;
    virtual bool greenIsLit(uint buttonId) = 0;
    virtual void clearEncoderButton() = 0;
    virtual void setKnobConfiguration(uint knobIndex, const char *name, uint8_t cmdbyte, uint8_t typecode) = 0;
    virtual bool redIsLit(uint buttonId) = 0;
    virtual void restoreLedState() = 0;
    virtual void saveLedState() = 0;
    virtual void sendParameter(uint8_t paramID, uint8_t value) = 0;
    virtual void resetEncoder(uint i) = 0;
    virtual knobConfig getKnobConfiguration(uint i) = 0;
    virtual int  AsciiToEncoder(char c) = 0;
    virtual long getEncoderModdedBy(long i) = 0;
    virtual uint16_t getLastTouchedKnob() const = 0;
    virtual void setLastTouchedKnob(uint16_t knobix) = 0;
    virtual void clearLastTouchedKnob() = 0;
    virtual bool knobValueChanged(uint i) = 0;
    virtual void sendParameterToSynth(uint knob) = 0;
    

    virtual ~IHardwareManager() {};
};