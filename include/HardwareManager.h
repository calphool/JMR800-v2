#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <ADC.h>
#include <Encoder.h>
#include <vector>
#include "defines.h"
#include "Logging.h"


struct SentCommand {
  uint8_t paramID;
  uint8_t value;
  unsigned long timestamp;
};


class HardwareManager {
private:
    long iGatherCtr = 0;
    unsigned long lastPollTime = 0;
    const unsigned long hardwareManagerPollIntervalMS = 50;
    Encoder* encoderKnob = nullptr;

    // array of pins for mux enable
    uint enablePins[NUM_MUXES] = { U2_ENABLE_PIN, U3_ENABLE_PIN, U4_ENABLE_PIN, U6_ENABLE_PIN };

    // array of pins for MUX addresses
    uint MuxAddressPins[4] = { S0_PIN, S1_PIN, S2_PIN, S3_PIN };

    uint16_t AnalogValues[16][NUM_MUXES];
    uint16_t oldAnalogValues[16][NUM_MUXES];

    // button state variables
    uint8_t buttonStates[NUM_BUTTONS] = {0}; // Current button state (0 or 1)
    uint8_t prevButtonStates[NUM_BUTTONS] = {0}; // Previous button state

    uint8_t ledstate;
    uint8_t prevLedState;

    // button statuses
    bool bEncoderBtn = false;
    bool bPrevEncoderBtn = false;

    // encoder position
    long lastEncoderPosition = 0;

    long lastPotScanTime = 0;

    ADC adc;

    // array of analog ADC input pins   
    uint analogInPins[NUM_MUXES] = { U2_ANALOG_IN_PIN, U3_ANALOG_IN_PIN, U4_ANALOG_IN_PIN , U6_ANALOG_IN_PIN};
    const uint8_t buttonPins[NUM_BUTTONS] = {
      PUSH_BTN_SW1_PIN,
      PUSH_BTN_SW2_PIN,
      PUSH_BTN_SW3_PIN,
      PUSH_BTN_SW4_PIN
    };

    std::vector<SentCommand> recentCommands;
    static volatile int bitIndex;
    static volatile uint16_t sendBuffer;
    knobConfig knobConfigurations[NUM_KNOBS];
    knobConfig knobConfigurations_bkup[NUM_KNOBS];
    knobConfig lastKnobConfig;


    void gatherControlSettings(); // internal method that reads hardware and sets internal hardware structures
    void gatherPotentiometerValues(); // internal method called by gatherControlSettings()
    void setAddressPins(uint val);
    void updateEncoder();
    void writeLedRegistersToHardware();
    static void onPG800ClockFall();
    void loadKnobs();
    void saveKnobs();
    bool knobChanged(int i);

public:
    void init();      // Set up pins, ADC, etc.
    void loop();      // Called every cycle, but throttled
    ~HardwareManager();

    // Accessors
    int getKnobValue(uint index);
    long getEncoderZeroTo(long divisor);
    bool getEncoderSwitchStatus();
    long getEncoderValue();
    bool isButtonPressed(uint index);
    bool buttonStateChanged(uint index, bool bDirection, bool bClearFlag);
    bool encoderSwitchStateChanged(bool upThenDown, bool clearFlag);
    void setButtonLights(uint buttonId, bool red, bool green);
    bool greenIsLit(uint buttonId);
    bool redIsLit(uint buttonId);
    void restoreLedState();
    void saveLedState();
    void sendParameter(uint8_t paramID, uint8_t value);
    void resetEncoder(uint i);

};

extern HardwareManager hardware;