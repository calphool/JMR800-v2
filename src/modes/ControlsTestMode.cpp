/**
 * @file ControlsTestMode.cpp
 * @brief Implements the Controls Test Mode for button interaction diagnostics.
 *
 * This mode allows testing of all physical buttons by cycling their LED states
 * upon press and rendering a diagnostic display via the associated screen.
 */

#include "modes/ControlsTestMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"


/**
 * @brief Called when the Controls Test Mode is activated.
 *
 * Logs mode entry. No specific setup required at this stage.
 */
void ControlsTestMode::onEnter() {
    log(LOG_VERBOSE, "Entering Controls Test Mode", __func__);
}


/**
 * @brief Called when the Controls Test Mode is deactivated.
 *
 * Turns off all button LEDs and logs mode exit.
 */
void ControlsTestMode::onExit() {
    log(LOG_VERBOSE, "Exiting Controls Test Mode", __func__);
    for(int i=0; i < NUM_BUTTONS; i++)
        hardware.setButtonLights(i, false, false);
}


/**
 * @brief Main loop for Controls Test Mode.
 *
 * Monitors all buttons. When a button press is detected, its LED state cycles through:
 * - off → red
 * - red → red + green
 * - red + green → off
 */
void ControlsTestMode::loop() {
    for(int i=0;i < NUM_BUTTONS; i++) {
        if(hardware.buttonStateChanged(i, true, true)) {
            if(hardware.redIsLit(i) == false && hardware.greenIsLit(i) == false) {
                hardware.setButtonLights(i, true, false);
            }
            else
            if(hardware.redIsLit(i) == true && hardware.greenIsLit(i) == false) {
                hardware.setButtonLights(i, true, true);
            }
            else
            if(hardware.redIsLit(i) == true && hardware.greenIsLit(i) == true) {
                hardware.setButtonLights(i, false, false);
            }
        }
    }
}


/**
 * @brief Displays the associated test screen via the screen manager.
 */
void ControlsTestMode::showScreen() {
    screenManager.setScreen(&screen);
}
