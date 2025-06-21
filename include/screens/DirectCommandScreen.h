/**
 * @file DirectCommandScreen.h
 * @brief UI screen for manually sending raw commands to the synthesizer interface.
 *
 * This screen provides a low-level interface for developers or advanced users
 * to enter raw parameter IDs and values and transmit them directly,
 * bypassing mapped UI controls.
 */

#pragma once

#ifdef TARGET_TEENSY
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#endif
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"
#include "widgets/EncoderAttachedNumericWidget.h"
#include "widgets/PushButtonWidget.h"

/**
 * @class DirectCommandScreen
 * @brief UI screen for direct entry and sending of PG-800-style commands.
 *
 * This screen provides encoder-driven numeric input fields for selecting a parameter
 * ID and its value, along with a push button to transmit the resulting command.
 */
class DirectCommandScreen : public Screen {

private:
    std::vector<Widget*> widgets;               ///< UI widgets in the screen
    EncoderAttachedNumericWidget* cmdWidget;    ///< Numeric input for parameter ID
    EncoderAttachedNumericWidget* byteWidget;   ///< Numeric input for value byte
    PushButtonWidget* pushWidget;               ///< Button to trigger send command
    char buff[20];                              ///< Temporary buffer for rendering

public:

    /**
     * @brief Constructs a new DirectCommandScreen instance.
     */
    DirectCommandScreen();

    /**
     * @brief Destructor.
     */
    ~DirectCommandScreen();  

    /**
     * @brief Adds a widget to the screen layout.
     * @param w Pointer to the widget to add
     */
    void addWidget(Widget* w);

    /**
     * @brief Draws all widgets and labels on the screen.
     */
    void draw() override;
    
    /**
     * @brief Handles input routing between widgets.
     */
    void handleInput() override;
    
    /**
     * @brief Called when this screen becomes the active UI.
     */
    void onEnter() override;
    
    /**
     * @brief Called when this screen is exited or replaced.
     */
    void onExit() override;
    
    /**
     * @brief Advances focus to the next widget in the control cycle.
     */
    void advanceActiveControl();
    
    /**
     * @brief Retrieves the currently active widget (has encoder focus).
     * @return Pointer to the active Widget
     */
    Widget* getActiveWidget();
    
    /**
     * @brief Gets the current parameter ID selected in the input field.
     * @return Parameter ID (0–127)
     */
    uint getCmdValue();
    
    /**
     * @brief Gets the current byte value selected in the input field.
     * @return Value byte (0–127)
     */
    uint getByteValue();
};