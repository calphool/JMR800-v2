/**
 * @file ControlsTestMode.h
 * @brief Application mode for verifying control inputs in real-time.
 *
 * This mode launches a diagnostic screen that displays the live status
 * of hardware controls such as knobs and buttons. It is useful for testing
 * during development or calibration.
 */

#pragma once

#include "modes/AppMode.h"
#include "screens/ControlsTestScreen.h"


/**
 * @class ControlsTestMode
 * @brief Mode used for debugging and validating physical control input.
 *
 * Displays a diagnostic screen that visualizes analog and digital input states.
 * Useful during hardware development and field servicing.
 */
class ControlsTestMode : public AppMode {
private:
    ControlsTestScreen screen;                  ///< The screen used to show real-time control data

public:

    /**
     * @brief Returns the name of the mode.
     * @return Descriptive string identifier
     */
    const char* name() const override { return "Controls Test Mode"; }

    /**
     * @brief Returns the screen instance associated with this mode.
     * @return Pointer to the test screen
     */
    Screen* getScreen() override { return &screen; }

    /**
     * @brief Lifecycle hook invoked when entering this mode.
     */
    void onEnter() override;

    /**
     * @brief Lifecycle hook invoked when exiting this mode.
     */
    void onExit() override;

    /**
     * @brief Executes periodic tasks during the mode loop.
     */
    void loop() override;

    /**
     * @brief Triggers display of the mode's screen.
     */
    void showScreen() override;
};
