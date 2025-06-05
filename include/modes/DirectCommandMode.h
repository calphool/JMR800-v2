/**
 * @file DirectCommandMode.h
 * @brief Application mode for sending direct parameter/value commands.
 *
 * This mode activates a screen where users can manually select and transmit
 * parameter-value pairs without relying on mapped controls.
 */

#pragma once

#include "modes/AppMode.h"
#include "screens/DirectCommandScreen.h"


/**
 * @class DirectCommandMode
 * @brief Mode for direct interaction with PG-800-style command interface.
 *
 * Allows advanced users or developers to issue low-level commands manually,
 * bypassing automated knob and button mappings.
 */
class DirectCommandMode : public AppMode {
private:
    DirectCommandScreen screen;             ///< The screen shown during this mode

public:

    /**
     * @brief Returns the name of this mode.
     * @return A string identifier
     */
    const char* name() const override { return "Direct Command Mode"; }

    /**
     * @brief Returns the screen associated with this mode.
     * @return Pointer to the screen
     */
    Screen* getScreen() override { return &screen; }

    /**
     * @brief Called when the mode is activated.
     */
    void onEnter() override;

    /**
     * @brief Called when the mode is exited.
     */
    void onExit() override;

    /**
     * @brief Executes logic that should run continuously during the mode.
     */
    void loop() override;

    /**
     * @brief Displays or refreshes the screen associated with this mode.
     */
    void showScreen() override;
};
