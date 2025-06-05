/**
 * @file KnobWidget.h
 * @brief Base class for knob-style widgets in the JMR800-V2 interface.
 *
 * Provides common rendering routines for circular knob elements such as
 * bezels or positioning. This class is not intended to be used directly,
 * but rather subclassed by specific knob types (e.g., potentiometer, encoder).
 */

#pragma once

#include "widgets/Widget.h"
#include <Adafruit_SH110X.h>

/**
 * @class KnobWidget
 * @brief Shared base class for rotary-style knob widgets.
 *
 * Includes helper functions for drawing the outer bezel. This class simplifies
 * development of analog and encoder-driven knob interfaces by providing common
 * positioning and appearance behavior.
 */
class KnobWidget : public Widget {
public:

    /**
     * @brief Constructs a knob widget at the specified position.
     * @param x Horizontal screen position (in pixels)
     * @param y Vertical screen position (in pixels)
     */
    KnobWidget(int x, int y);
    
    /**
     * @brief Identifies the widget as a Knob type.
     * @return WidgetType::Knob
     */
    WidgetType getType() const override;

protected:

    /**
     * @brief Draws the circular bezel of the knob.
     *
     * This visual element can be reused by all subclasses.
     */
    void drawBezel();
};
