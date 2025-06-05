/**
 * @file PushButtonWidget.h
 * @brief Represents an interactive on-screen push button for the JMR800-V2 UI.
 *
 * This widget can be pressed, drawn with a label, and optionally attached to
 * encoder-based input focus for rotary navigation.
 */

#pragma once

#include "widgets/Widget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/**
 * @class PushButtonWidget
 * @brief On-screen momentary button with press state and optional encoder focus (like an OK or Cancel button).
 *
 * PushButtonWidget supports display, interaction, and optional encoder focus
 * for navigation systems. The visual label is rendered in a rectangular region.
 */
class PushButtonWidget : public Widget {
public:

    /**
     * @brief Constructs a labeled push button at a specific screen location.
     * @param label C-style string to display inside the button
     * @param x Horizontal position (pixels)
     * @param y Vertical position (pixels)
     */
    PushButtonWidget(const char* label, int x, int y);

    /**
     * @brief Sets the pressed state of the button (visual only).
     * @param state True if pressed
     */
    void setPressed(bool state);

    /**
     * @brief Renders the button with its current state and label.
     */
    void draw() override;

    /**
     * @brief Processes input events (e.g., encoder button press).
     */
    void handleInput();

    /**
     * @brief Flags this button as the target of encoder-driven input.
     */
    void attachToEncoder();

    /**
     * @brief Detaches this button from encoder focus.
     */
    void detachFromEncoder();

    /**
     * @brief Checks if this widget is currently attached to the encoder.
     * @return True if attached to encoder
     */
    bool isAttachedToEncoder();

    /**
     * @brief Identifies the widget as a PushButton type.
     * @return WidgetType::PushButton
     */ 
    WidgetType getType() const override;


private:
    bool pressed;               ///< Whether the button is currently pressed
    char text[25];              ///< Button label text
    bool bIsAttachedToEncoder;  ///< Encoder navigation focus flag


};


