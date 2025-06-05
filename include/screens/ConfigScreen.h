/**
 * @file ConfigScreen.h
 * @brief Screen for accessing and modifying configuration settings.
 *
 * The ConfigScreen allows users to interact with widgets tied to system
 * configuration, including knob assignments. It supports launching
 * a modal dialog to change knob mappings.
 */

#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"
#include "screens/KnobConfigDialog.h"

/**
 * @class ConfigScreen
 * @brief User interface screen for updating device configuration settings.
 *
 * This screen allows editing of control mappings (e.g., knob assignments)
 * through on-screen widgets and pop-up modal dialogs. It manages a collection
 * of configuration widgets and input routing.
 */
class ConfigScreen : public Screen {

private:
    std::vector<Widget*> widgets;                   ///< Configuration widgets displayed on screen
    int active_knob = -1;                           ///< Index of the currently active knob for configuration
    KnobConfigDialog* knobConfigDialog = nullptr;   ///< Modal dialog for editing knob configuration

public:
    /**
     * @brief Constructs a new ConfigScreen instance.
     */
    ConfigScreen();

    /**
     * @brief Destructor.
     */
    ~ConfigScreen();  

    /**
     * @brief Adds a widget to the configuration screen.
     * @param w Pointer to the widget to add
     */
    void addWidget(Widget* w);

    /**
     * @brief Draws all widgets and active dialogs.
     */
    void draw() override;

    /**
     * @brief Handles input routing and dialog activation.
     */
    void handleInput() override;

    /**
     * @brief Called when this screen becomes active.
     */
    void onEnter() override;

    /**
     * @brief Called when this screen is exited.
     */
    void onExit() override;

    /**
     * @brief Highlights a knob widget to indicate it is active.
     * @param i Index of the knob to highlight
     */
    void highlightActiveKnob(int i);

    /**
     * @brief Launches the knob configuration dialog for a given knob.
     * @param knobIndex Index of the knob to configure
     */
    void changeScreenMode(uint knobIndex);
};
