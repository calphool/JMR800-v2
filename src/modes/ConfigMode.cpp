/**
 * @file ConfigMode.cpp
 * @brief Implements the System Configuration Mode for knob setup
 */

#include "modes/ConfigMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareInterface.h"

extern HardwareInterface* hardware;

/**
 * @brief Called when the System Config Mode is entered.
 *
 * Logs entry and performs any setup required for configuration context.
 */
void ConfigMode::onEnter() {
    log(LOG_VERBOSE, "Entering System Config Mode", __func__);
}

/**
 * @brief Called when the System Config Mode is exited.
 *
 * Logs exit and performs any teardown if necessary.
 */
void ConfigMode::onExit() {
    log(LOG_VERBOSE, "Exiting System Config Mode", __func__);
}


/**
 * @brief Main loop for configuration logic.
 *
 * Continuously reads the encoder to determine the active knob index,
 * highlights the corresponding knob on the screen, and enters
 * a knob configuration dialog when the encoder switch is pressed.
 */
void ConfigMode::loop() {
    uint active_knob = hardware->getEncoderModdedBy(NUM_KNOBS);

    theConfigScreen.highlightActiveKnob(active_knob);

    if(hardware->encoderSwitchStateChanged(true, true )) {
        theConfigScreen.activateKnobConfig(active_knob);
    }
}


/**
 * @brief Displays the configuration screen via the screen manager.
 */
void ConfigMode::showScreen() {
    screenManager.setScreen(&theConfigScreen);
}
