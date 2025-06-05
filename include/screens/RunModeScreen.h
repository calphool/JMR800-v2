/**
 * @file RunModeScreen.h
 * @brief Represents the active runtime screen for interacting with control widgets.
 *
 * This screen is responsible for drawing the live UI during device operation,
 * routing input to the appropriate widget, and maintaining widget state.
 */

#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"

/**
 * @class RunModeScreen
 * @brief UI screen that manages and displays control widgets during runtime.
 *
 * The RunModeScreen class maintains a list of interactive widgets and handles
 * input routing, drawing, and lifecycle events for the active operational UI.
 */
class RunModeScreen : public Screen {

private:
    std::vector<Widget*> widgets; ///< List of widgets displayed on this screen

public:
    /**
     * @brief Constructs a new RunModeScreen instance.
     */
    RunModeScreen();

    /**
     * @brief Destructor to clean up widget references.
     */
    ~RunModeScreen();  

    /**
     * @brief Adds a widget to the screen's layout.
     * @param w Pointer to the widget to add
     */
    void addWidget(Widget* w);

    /**
     * @brief Draws all widgets managed by this screen.
     */
    void draw() override;

    /**
     * @brief Delegates input handling to the appropriate widget(s).
     */
    void handleInput() override;

    /**
     * @brief Called when the screen becomes active.
     *
     * Typically used to reset or initialize widget states.
     */
    void onEnter() override;

    /**
     * @brief Called when the screen is exited.
     *
     * Can be used to save state or disable input handling.
     */
    void onExit() override;
};
