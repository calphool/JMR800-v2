/**
 * @file RunMode.cpp
 * @brief Implements the behavior of the Run Mode application state.
 *
 * This mode is responsible for standard operation and interacts with the primary run-time screen.
 */

#include "modes/RunMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "IHardwareManager.h"


extern IHardwareManager* hardware;

/**
 * @brief Called when the Run Mode becomes active.
 *
 * Used to perform any setup or initialization needed before the mode starts executing.
 */
void RunMode::onEnter() {
    log(LOG_VERBOSE, "Entering Run Mode", __func__);
}


/**
 * @brief Called when the Run Mode is deactivated.
 *
 * Used for cleanup or persisting state before transitioning out of the mode.
 */
void RunMode::onExit() {
    log(LOG_VERBOSE, "Exiting Run Mode", __func__);
}


/**
 * @brief Main loop logic for the Run Mode.
 *
 * Called regularly while this mode is active. Add polling, automation, or mode-specific control here.
 */
void RunMode::loop() {
    for(uint i = 0; i < NUM_KNOBS; i++) {
      if(hardware->knobValueChanged(i) && millis() > 12000) { // we start sending parameters after 10 seconds (allows knob leveling to settle)
        hardware->sendParameterToSynth(i);
        hardware->setLastTouchedKnob(i);
      }
    }
}


/**
 * @brief Displays the screen associated with the Run Mode.
 *
 * Invokes the screen manager to switch to the RunModeScreen instance.
 */
void RunMode::showScreen() {
    screenManager.setScreen(&screen);
}


