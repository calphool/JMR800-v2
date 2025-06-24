#pragma once
#include "HardwareInterface.h"



#include <cstdint>

struct SimulatedTeensyHardwareState {
    int potentiometers[NUM_KNOBS] = {0};    // 56 potentiometer values (0–1023)
    bool buttons[NUM_BUTTONS] = {false};       // 4 pushbuttons
    bool redLED[NUM_BUTTONS] = {false};        // 4 red LEDs
    bool greenLED[NUM_BUTTONS] = {false};      // 4 green LEDs
    int encoderPosition = 0;         // Rotary encoder position
    bool encoderPressed = false;     // Rotary encoder pushbutton
};



class SimulatedTeensyHardwareManager : public HardwareInterface {
private:
    knobConfig knobConfigurations[NUM_KNOBS] = {0};  ///< Current knob-to-parameter mapping
    knobConfig knobConfigurations_bkup[NUM_KNOBS] = {0}; ///< Backup mapping
    knobConfig lastKnobConfig = {0}; ///< Most recent configuration state


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
    ~SimulatedTeensyHardwareManager();

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