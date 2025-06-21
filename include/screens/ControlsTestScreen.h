/**
 * @file ControlsTestScreen.h
 * @brief Diagnostic screen for visualizing the state of all physical controls.
 *
 * This screen renders all relevant control widgets (knobs, buttons, etc.)
 * and shows their current values or states in real time, useful for testing and calibration.
 */

#pragma once

#ifdef TARGET_TEENSY
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#endif
#include <vector>
#include "widgets/Widget.h"
#include "widgets/TextLabelWidget.h"
#include "screens/Screen.h"

/**
 * @class ControlsTestScreen
 * @brief Debug or test screen for monitoring live control states.
 *
 * This screen is used during development or calibration to verify that all hardware
 * inputs are functioning and routed correctly. It displays all attached widgets
 * and updates their display values in real time.
 */
class ControlsTestScreen : public Screen {

private:
    std::vector<Widget*> widgets;       ///< Collection of testable UI controls

public:

    /**
     * @brief Constructs a new ControlsTestScreen instance.
     */
    ControlsTestScreen();

    /**
     * @brief Destructor.
     */
    ~ControlsTestScreen();  

    /**
     * @brief Adds a widget to the screen for monitoring.
     * @param w Pointer to the widget to add
     */
    void addWidget(Widget* w);

    /**
     * @brief Renders all control widgets and output indicators.
     */
    void draw() override;

    /**
     * @brief Routes input events (e.g., encoder presses) if applicable.
     */
    void handleInput() override;

    /**
     * @brief Called when the test screen becomes active.
     */
    void onEnter() override;

    /**
     * @brief Called when the test screen is deactivated.
     */
    void onExit() override;

private:
    TextLabelWidget* numBox;
};
