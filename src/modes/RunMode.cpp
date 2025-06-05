/**
 * @file RunMode.cpp
 * @brief Implements the behavior of the Run Mode application state.
 *
 * This mode is responsible for standard operation and interacts with the primary run-time screen.
 */

#include "modes/RunMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"


/**
 * @brief Called when the Run Mode becomes active.
 *
 * Used to perform any setup or initialization needed before the mode starts executing.
 */
void RunMode::onEnter() {
    log(LOG_VERBOSE, "Entering Run Mode");
}


/**
 * @brief Called when the Run Mode is deactivated.
 *
 * Used for cleanup or persisting state before transitioning out of the mode.
 */
void RunMode::onExit() {
    log(LOG_VERBOSE, "Exiting Run Mode");
}


/**
 * @brief Main loop logic for the Run Mode.
 *
 * Called regularly while this mode is active. Add polling, automation, or mode-specific control here.
 */
void RunMode::loop() {
    // Insert mode-specific polling or logic here
}


/**
 * @brief Displays the screen associated with the Run Mode.
 *
 * Invokes the screen manager to switch to the RunModeScreen instance.
 */
void RunMode::showScreen() {
    screenManager.setScreen(&screen);
}


