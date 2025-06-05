/**
 * @file RunMode.h
 * @brief Defines the main runtime mode of the application.
 *
 * This mode activates the standard control screen and operational logic
 * for interacting with the synthesizer during live use.
 */

#pragma once

#include "modes/AppMode.h"
#include "screens/RunModeScreen.h"


/**
 * @class RunMode
 * @brief Application mode for normal runtime operation.
 *
 * This mode drives the `RunModeScreen` and represents the active state
 * during which the user manipulates controls. It handles initialization
 * and cleanup of runtime context.
 */
class RunMode : public AppMode {
private:
    RunModeScreen screen;           ///< The screen instance shown during run mode

public:

    /**
     * @brief Returns the name of this mode.
     * @return A string identifier
     */
    const char* name() const override { return "Run Mode"; }

    /**
     * @brief Retrieves the screen associated with this mode.
     * @return Pointer to a `Screen` object
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
