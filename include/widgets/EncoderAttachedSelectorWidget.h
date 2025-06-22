/**
 * @file EncoderAttachedSelectorWidget.h
 * @brief Widget for displaying and adjusting an array of string values
 */

#pragma once

#include <vector>
#include "widgets/Widget.h"

#ifdef TARGET_TEENSY
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#endif


/**
 * @class EncoderAttachedSelectorWidget
 * @brief Widget for displaying and adjusting an array of string values using a rotary encoder.
 *
 */
class EncoderAttachedSelectorWidget : public Widget {

public:

    /**
     * @brief Constructs the widget.
     * @param x X-coordinate on screen
     * @param y Y-coordinate on screen
     */
    EncoderAttachedSelectorWidget(int x, int y);

    /**
     * @brief Draws the current value on screen using the format string.
     */
    void draw() override;
 
    /**
     * @brief Responds to encoder input to modify the value.
     */
    void handleInput() override;

    void setValue(uint x);

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
    uint getValue();

    /**
     * @brief Returns the widget type for this control.
     * @return WidgetType::EncoderAttachedSelectorWidget
     */
    WidgetType getType() const override;

    /**
     * @brief Set the control's value
     */
    void addLabelAt(const char* s, uint x);

    //void clearData();


private:
    uint currentValue;
    bool bIsAttachedToEncoder = false; ///< Flag indicating if the widget is attached to the encoder
    std::vector<const char*> labels;
    uint width;

    void recalculatePixelWidth();
};
