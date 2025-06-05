/**
 * @file HardwareManager.h
 * @brief Manages hardware input/output for analog controls, digital buttons, encoder, and serial communication.
 */

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


/**
 * @struct SentCommand
 * @brief Represents a parameter-value pair sent to external hardware along with a timestamp.
 */
struct SentCommand {
  uint8_t paramID;
  uint8_t value;
  unsigned long timestamp;
};

/**
 * @class HardwareManager
 * @brief Handles initialization, polling, and control of physical interface components such as buttons, knobs, and LED indicators.
 */
class HardwareManager {
private:
    long iGatherCtr = 0;
    unsigned long lastPollTime = 0;
    const unsigned long hardwareManagerPollIntervalMS = 50;
    Encoder* encoderKnob = nullptr;

    uint enablePins[NUM_MUXES] = { U2_ENABLE_PIN, U3_ENABLE_PIN, U4_ENABLE_PIN, U6_ENABLE_PIN }; ///< Enable lines for each analog multiplexer
    uint MuxAddressPins[4] = { S0_PIN, S1_PIN, S2_PIN, S3_PIN }; ///< Enable lines for each analog multiplexer

    uint16_t AnalogValues[16][NUM_MUXES]; ///< Latest analog readings from all knobs
    uint16_t oldAnalogValues[16][NUM_MUXES]; ///< Previous readings for comparison

    uint8_t buttonStates[NUM_BUTTONS] = {0}; ///< Current digital button states
    uint8_t prevButtonStates[NUM_BUTTONS] = {0}; ///< Previous digital button states

    uint8_t ledstate;
    uint8_t prevLedState;

    // button statuses
    bool bEncoderBtn = false;
    bool bPrevEncoderBtn = false;

    // encoder position
    long lastEncoderPosition = 0;

    long lastPotScanTime = 0;

    ADC adc; ///< ADC manager for Teensy analog reads

  
    const uint8_t analogInPins[NUM_MUXES] = { U2_ANALOG_IN_PIN, U3_ANALOG_IN_PIN, U4_ANALOG_IN_PIN , U6_ANALOG_IN_PIN }; ///< ADC pins corresponding to each MUX
    const uint8_t buttonPins[NUM_BUTTONS] = {PUSH_BTN_SW1_PIN,PUSH_BTN_SW2_PIN,PUSH_BTN_SW3_PIN,PUSH_BTN_SW4_PIN }; ///< GPIO pin numbers for each digital button

    std::vector<SentCommand> recentCommands;  ///< Rolling log of recently sent PG-800 commands
    static volatile int bitIndex;
    static volatile uint16_t sendBuffer;
    knobConfig knobConfigurations[NUM_KNOBS];  ///< Current knob-to-parameter mapping
    knobConfig knobConfigurations_bkup[NUM_KNOBS]; ///< Backup mapping
    knobConfig lastKnobConfig; ///< Most recent configuration state


    void gatherControlSettings(); // internal method that reads hardware and sets internal hardware structures
    void gatherPotentiometerValues(); // internal method called by gatherControlSettings()
    void setAddressPins(uint val);
    void updateEncoder();
    void writeLedRegistersToHardware();
    static void onPG800ClockFall();
    bool knobChanged(int i);

public:
    /**
     * @brief Initializes hardware peripherals: GPIO, ADC, encoder, etc.
     */
    void init();      // Set up pins, ADC, etc.

    /**
     * @brief Main polling loop, throttled to a fixed interval.
     */
    void loop();      // Called every cycle, but throttled
    
    /**
     * @brief Destructor
     */
    ~HardwareManager();

    /**
     * @brief Loads the knob configuration from EEPROM or persistent storage.
     */
    void loadKnobs();
    
    /**
     * @brief Saves the current knob configuration to EEPROM.
     */
    void saveKnobs();

    /**
     * @brief Retrieves the current analog value of the specified knob.
     * @param index Knob index (0 to NUM_KNOBS-1)
     */
    int getKnobValue(uint index);
    
    /**
     * @brief Returns a normalized encoder value in range [0, divisor).
     */
    long getEncoderZeroTo(long divisor);

    /**
     * @brief Returns true if the encoder button is currently pressed.
     */    
    bool getEncoderSwitchStatus();

    /**
     * @brief Returns raw encoder position.
     */
    long getEncoderValue();

    /**
     * @brief Returns true if the specified button is currently pressed.
     */
    bool isButtonPressed(uint index);

    /**
     * @brief Detects state changes on buttons.
     * @param index Button index
     * @param bDirection If true, only detect rising edge
     * @param bClearFlag If true, clear state change flag after detection
     */
    bool buttonStateChanged(uint index, bool bDirection, bool bClearFlag);

    /**
     * @brief Detects state changes on encoder button.
     */
    bool encoderSwitchStateChanged(bool upThenDown, bool clearFlag);

    /**
     * @brief Sets the LED colors for a button.
     */
    void setButtonLights(uint buttonId, bool red, bool green);

    /**
     * @brief Checks if the green LED is lit for a given button.
     */
    bool greenIsLit(uint buttonId);

    /**
     * @brief Checks if the red LED is lit for a given button.
     */
    bool redIsLit(uint buttonId);

    /**
     * @brief Restores the previously saved LED state.
     */
    void restoreLedState();

    /**
     * @brief Saves the current LED state to internal memory.
     */
    void saveLedState();

    /**
     * @brief Sends a parameter command over the serial protocol.
     * @param paramID PG-800 parameter ID
     * @param value Value to send
     */
    void sendParameter(uint8_t paramID, uint8_t value);

    /**
     * @brief Resets the encoder’s internal count to a specific value.
     */
    void resetEncoder(uint i);

    /**
     * @brief Retrieves the configuration associated with a given knob.
     */
    knobConfig getKnobConfiguration(uint i);

};

/**
 * @brief Singleton instance of HardwareManager.
 */
extern HardwareManager hardware;