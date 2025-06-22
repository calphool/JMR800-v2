/**
 * @file TeensyHardwareManager.h
 * @brief Manages hardware input/output for analog controls, digital buttons, encoder, and serial communication.
 */

#pragma once

#ifdef TARGET_TEENSY


#include "HardwareInterface.h"
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
  uint8_t paramID; ///< PG-800 parameter ID
  uint8_t value; ///< Value associated with the parameter
  unsigned long timestamp; ///< Timestamp when the command was sent
};

/**
 * @class TeensyHardwareManager
 * @brief Handles initialization, polling, and control of physical interface components such as buttons, knobs, and LED indicators.
 */
class TeensyHardwareManager : public HardwareInterface {
private:
    long iGatherCtr = 0;
    unsigned long lastPollTime = 0;
    const unsigned long TeensyHardwareManagerPollIntervalMS = 50;
    Encoder* encoderKnob = nullptr;

    uint enablePins[NUM_MUXES] = { U2_ENABLE_PIN, U3_ENABLE_PIN, U4_ENABLE_PIN, U6_ENABLE_PIN }; ///< Enable lines for each analog multiplexer
    uint MuxAddressPins[4] = { S0_PIN, S1_PIN, S2_PIN, S3_PIN }; ///< Enable lines for each analog multiplexer

    uint16_t AnalogValues[16][NUM_MUXES] = {0};///< Latest analog readings from all knobs
    uint16_t oldAnalogValues[16][NUM_MUXES] = {0}; ///< Previous readings for comparison

    uint8_t buttonStates[NUM_BUTTONS] = {0}; ///< Current digital button states
    uint8_t prevButtonStates[NUM_BUTTONS] = {0}; ///< Previous digital button states

    uint8_t ledstate = 0;    ///< Bitfield representing current LED state
    uint8_t prevLedState = 0;///< Bitfield representing previous LED state

    // button statuses
    bool bEncoderBtn = false;     ///< Current encoder button state
    bool bPrevEncoderBtn = false; ///< Previous encoder button state

    // encoder position
    long lastEncoderPosition = 0; ///< Last recorded rotary encoder position

    long lastPotScanTime = 0;

    ADC adc; ///< ADC manager for Teensy analog reads

  
    const uint8_t analogInPins[NUM_MUXES] = { U2_ANALOG_IN_PIN, U3_ANALOG_IN_PIN, U4_ANALOG_IN_PIN , U6_ANALOG_IN_PIN }; ///< ADC pins corresponding to each MUX
    const uint8_t buttonPins[NUM_BUTTONS] = {PUSH_BTN_SW1_PIN,PUSH_BTN_SW2_PIN,PUSH_BTN_SW3_PIN,PUSH_BTN_SW4_PIN }; ///< GPIO pin numbers for each digital button

    std::vector<SentCommand> recentCommands;  ///< Rolling log of recently sent PG-800 commands
    static volatile int bitIndex; ///< Serial transmission bit index
    static volatile uint16_t sendBuffer; ///< Serial transmission buffer
    knobConfig knobConfigurations[NUM_KNOBS] = {0};  ///< Current knob-to-parameter mapping
    knobConfig knobConfigurations_bkup[NUM_KNOBS] = {0}; ///< Backup mapping
    knobConfig lastKnobConfig = {0}; ///< Most recent configuration state


    void gatherControlSettings(); ///< Reads all analog and digital inputs
    void gatherPotentiometerValues(); ///< Reads and stores current potentiometer values
    void setAddressPins(uint val); ///< Sets the MUX address lines
    void updateEncoder(); ///< Reads and updates encoder state
    void writeLedRegistersToHardware() const; ///< Pushes current LED state to shift registers
    static void onPG800ClockFall(); ///< ISR for clocking out serial bits
    bool knobChanged(int i) const; ///< Detects change in knob value

public:
    /**
     * @brief Initializes hardware peripherals: GPIO, ADC, encoder, etc.
     */
    void init() override;      // Set up pins, ADC, etc.

    /**
     * @brief Main polling loop, throttled to a fixed interval.
     */
    void loop() override;      // Called every cycle, but throttled
    
    /**
     * @brief Destructor
     */
    ~TeensyHardwareManager();

    /**
     * @brief Loads the knob configuration from EEPROM or persistent storage.
     */
    void loadKnobs() override;
    
    /**
     * @brief Saves the current knob configuration to EEPROM.
     */
    void saveKnobs() override;

    /**
     * @brief Retrieves the current analog value of the specified knob.
     * @param knobIX Knob index (0 to NUM_KNOBS-1)
     */
    int getKnobValue(uint knobIX) override;
    
    /**
     * @brief Returns a normalized encoder value in range [0, divisor).
     * @param divisor Maximum value for wrapping
     * @return Normalized encoder count
     */
    long getEncoderZeroTo(long divisor) override;

    /**
     * @brief Returns true if the encoder button is currently pressed.
     */    
    bool getEncoderSwitchStatus() override;

    /**
     * @brief Returns raw encoder position.
     */
    long getEncoderValue() override;

    /**
     * @brief Returns true if the specified button is currently pressed.
     * @param index Button index (0 to NUM_BUTTONS-1)
     * @return True if pressed
     */
    bool isButtonPressed(uint index) override;

    /**
     * @brief Detects state changes on buttons.
     * @param index Button index
     * @param bDirection If true, only detect rising edge
     * @param bClearFlag If true, clear state change flag after detection
     */
    bool buttonStateChanged(uint index, bool upThenDown, bool clearFlag) override;

    /**
     * @brief Detects state changes on encoder button.
     * @param upThenDown If true, require a full press/release cycle
     * @param clearFlag If true, clear the change flag after detection
     * @return True if the encoder button changed state
     */
    bool encoderSwitchStateChanged(bool upThenDown, bool clearFlag) override;

    /**
     * @brief Sets the LED colors for a button.
     * @param buttonId Button index
     * @param red Set red LED on/off
     * @param green Set green LED on/off
     */
    void setButtonLights(uint buttonId, bool red, bool green) override;

    /**
     * @brief Checks if the green LED is lit for a given button.
     * @param buttonId Button index
     * @return True if green LED is on
     */
    bool greenIsLit(uint buttonId) override;

    void clearEncoderButton() override;

    void setKnobConfiguration(uint knobIndex, const char *name, uint8_t cmdbyte, uint8_t typecode) override;

    /**
     * @brief Checks if the red LED is lit for a given button.
     * @param buttonId Button index
     * @return True if red LED is on
     */
    bool redIsLit(uint buttonId) override;

    /**
     * @brief Restores the previously saved LED state.
     */
    void restoreLedState() override;

    /**
     * @brief Saves the current LED state to internal memory.
     */
    void saveLedState() override;

    /**
     * @brief Sends a parameter command over the serial protocol.
     * @param paramID PG-800 parameter ID
     * @param value Value to send (0–127)
     */
    void sendParameter(uint8_t paramID, uint8_t value) override;

    /**
     * @brief Resets the encoder’s internal count to a specific value.
     * @param i New count value
     */
    void resetEncoder(uint i) override;

    /**
     * @brief Retrieves the configuration associated with a given knob.
     * @param i Knob index
     * @return knobConfig structure
     */
    knobConfig getKnobConfiguration(uint index) override;

    /**
     * @brief Converts an ASCII value to an encoder setting
     * @param c character
     * @return the encoder setting
     */
    int AsciiToEncoder(char c) override;

    long getEncoderModdedBy(long divisor) override;
};

#endif