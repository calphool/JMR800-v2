/**
 * @file ButtonWidget.h
 * @brief Represents an on-screen button widget with LED state indicators.
 *
 * This widget is used to reflect and control the state of a physical button.
 * It displays press status and can illuminate red/green indicators to reflect
 * associated hardware states.
 */

#pragma once

#include "widgets/Widget.h"

#ifdef TARGET_TEENSY
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#endif

/**
 * @class ButtonWidget
 * @brief A UI element that represents a hardware button and its LED status.
 *
 * This widget supports visual highlighting, press indication, and two-color
 * LED simulation. It is typically linked to a button on the panel.
 */


class ButtonWidget : public Widget {
public:

    /**
     * @brief Constructs a ButtonWidget with a given ID and position.
     * @param _buttonId Logical identifier for the associated hardware button
     * @param x X-coordinate on screen (pixels)
     * @param y Y-coordinate on screen (pixels)
     */
    ButtonWidget(int _buttonId, int x, int y);

    /**
     * @brief Sets the pressed state of the button (visual feedback only).
     * @param state True if the button is considered pressed
     */
    void setPressed(bool state);

    /**
     * @brief Sets the red and green LED states.
     * @param bRed True to enable red LED
     * @param bGreen True to enable green LED
     */
    void setLEDs(bool bRed, bool bGreen);

    /**
     * @brief Draws the button, including LED and press state.
     */
    void draw() override;

    /**
     * @brief Handles user interaction (encoder press, etc.).
     */
    void handleInput();

    /**
     * @brief Returns the widget type as Button.
     * @return WidgetType::Button
     */
    WidgetType getType() const override;

private:
    bool pressed;  ///< Whether the button is currently pressed
    bool red;      ///< Red LED state
    bool green;    ///< Green LED state
    int buttonId;  ///< Identifier for linking to hardware
};


