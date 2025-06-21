/**
 * @file EncoderKnobWidget.h
 * @brief Represents a virtual knob controlled by an encoder input.
 *
 * This widget provides a visual knob that responds to rotary encoder movement
 * and button presses. It can be highlighted or pressed to reflect focus or interaction.
 */
#pragma once

#include "widgets/KnobWidget.h"

#ifdef TARGET_TEENSY
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#endif

/**
 * @class EncoderKnobWidget
 * @brief A knob-style widget driven by encoder input rather than analog voltage.
 *
 * This widget is typically used to adjust parameters via rotary encoder control.
 * It includes press state visualization and inherits drawing behavior from KnobWidget.
 */
class EncoderKnobWidget : public KnobWidget {
public:

    /**
     * @brief Constructs a new encoder knob widget.
     * @param x X-coordinate in pixels
     * @param y Y-coordinate in pixels
     */
    EncoderKnobWidget(int x, int y);

    /**
     * @brief Sets whether the knob appears pressed.
     * @param state True if pressed
     */
    void setPressed(bool state);

    /**
     * @brief Renders the knob to the screen, including press state.
     */
    void draw() override;

    /**
     * @brief Processes input events related to encoder movement or selection.
     */
    void handleInput() override;

    /**
     * @brief Identifies the widget as an EncoderKnob type.
     * @return WidgetType::EncoderKnob
     */
    WidgetType getType() const override;


private:
    bool pressed;///< Pressed state for visual feedback

};


