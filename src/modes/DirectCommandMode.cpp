/**
 * @file DirectCommandMode.cpp
 * @brief Implements behavior for Direct Command Mode.
 *
 * This mode allows direct user input of parameter and value pairs using encoder-attached controls.
 * It includes screen transitions and input-driven control navigation and action triggering.
 */

#include "modes/DirectCommandMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareInterface.h"
#include "widgets/Widget.h"

extern HardwareInterface* hardware;

/**
 * @brief Called when the Direct Command Mode is activated.
 *
 * Prepares any state or logs entry into this mode.
 */
void DirectCommandMode::onEnter() {
    log(LOG_VERBOSE, "Entering Direct Command Mode", __func__);
}


/**
 * @brief Called when the Direct Command Mode is exited.
 *
 * Handles any cleanup or state persistence on mode deactivation.
 */
void DirectCommandMode::onExit() {
    log(LOG_VERBOSE, "Exiting Direct Command Mode", __func__);
}


/**
 * @brief Main loop logic for Direct Command Mode.
 *
 * Watches for button input to cycle active widgets or trigger actions, such as sending commands.
 * Button 0 advances the active control focus.
 * Encoder switch triggers command send if the active control is a push button.
 */
void DirectCommandMode::loop() {
    if(hardware->buttonStateChanged(0, true, true)) {
        screen.advanceActiveControl();
    }

    if(hardware->encoderSwitchStateChanged(true, true)) {
        Widget* w = screen.getActiveWidget();
        if(w != NULL) {
            if(w->getType() == WidgetType::PushButton) {
                hardware->sendParameter(screen.getCmdValue(), screen.getByteValue());
            }
        }
    }
}


/**
 * @brief Requests the screen manager to activate the Direct Command screen.
 */
void DirectCommandMode::showScreen() {
    screenManager.setScreen(&screen);
}
