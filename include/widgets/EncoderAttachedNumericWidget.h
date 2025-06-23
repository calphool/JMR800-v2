/**
 * @file EncoderAttachedNumericWidget.h
 * @brief Widget for displaying and adjusting numeric values using an encoder.
 *
 * This widget shows a number on screen and allows encoder input to increment
 * or decrement the value within a specified range. Formatting and display
 * position are configurable.
 */

#pragma once

#include "widgets/Widget.h"
#include <stdint.h>
#ifdef TARGET_TEENSY
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#endif


/**
 * @class EncoderAttachedNumericWidget
 * @brief Numeric display widget controlled by rotary encoder input.
 *
 * This widget displays a formatted number on screen and allows it to be adjusted
 * using the encoder. It supports range limits and optional encoder attachment tracking.
 */
class EncoderAttachedNumericWidget : public Widget {

public:

    /**
     * @brief Constructs the numeric widget.
     * @param x X-coordinate on screen
     * @param y Y-coordinate on screen
     * @param min Minimum allowed value
     * @param max Maximum allowed value
     * @param printFormat Format string (e.g., "%03d")
     */
    EncoderAttachedNumericWidget(int x, int y, int min, int max, char* printFormat);

    /**
     * @brief Draws the current value on screen using the format string.
     */
    void draw() override;
 
    /**
     * @brief Responds to encoder input to modify the value.
     */
    void handleInput() override;
 
    /**
     * @brief Changes the screen position of the widget.
     * @param xpos New X position
     * @param ypos New Y position
     */
    //void setPosition(int xpos, int ypos);

    /**
     * @brief Enables encoder control for this widget.
     */
    void attachToEncoder();
    
    /**
     * @brief Disables encoder control for this widget.
     */
    void detachFromEncoder();
    
    /**
     * @brief Indicates whether the widget currently has encoder focus.
     * @return True if attached
     */
    bool isAttachedToEncoder();
    
    /**
     * @brief Gets the current numeric value.
     * @return Current value
     */
    uint8_t getValue();
    
    /**
     * @brief Returns the widget type for this control.
     * @return WidgetType::EncoderAttachedNumericWidget
     */
    WidgetType getType() const override;

    /**
     * @brief Set the control's value
     */
    void setValue(int x);

private:
    //String text;                    ///< Rendered text based on current value
    //uint8_t fontSize;               ///< Font size multiplier for display
    uint8_t control_value;             ///< Current numeric value
    bool bIsAttachedToEncoder;      ///< True if widget has input focus
    int min;                        ///< Minimum allowed value
    int max;                        ///< Maximum allowed value
    char printFormat[8];              ///< Format string for numeric display
};
