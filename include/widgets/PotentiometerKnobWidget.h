/**
 * @file PotentiometerKnobWidget.h
 * @brief Visual representation of a physical potentiometer for the OLED UI.
 *
 * This widget draws a knob with an arrow indicator whose position reflects
 * the current analog value. It is associated with a physical control read via ADC.
 */

#pragma once

#include "widgets/KnobWidget.h"

#ifdef TARGET_TEENSY
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#endif

/**
 * @class PotentiometerKnobWidget
 * @brief Widget for displaying and interacting with a hardware potentiometer.
 *
 * This widget represents the state of an analog control input using a circular knob
 * and arrow indicator. It can be highlighted, redrawn, and updated in response
 * to analog reads.
 */
class PotentiometerKnobWidget : public KnobWidget {
public:

    /**
     * @brief Constructs a new potentiometer knob widget.
     * @param _knobId Identifier associated with the physical knob
     * @param x Horizontal screen position
     * @param y Vertical screen position
     */
    PotentiometerKnobWidget(int _knobId, int x, int y);

    /**
     * @brief Sets the current analog value of the knob.
     * @param value Value in range 0–255 (mapped to 360° rotation)
     */
    void setValue(uint8_t value);       
    
    /**
     * @brief Enables or disables highlight (blink when true).
     * @param highlight True to highlight the knob
     */
    void setHighlighted(bool highlight); 

    /**
     * @brief Handles user interaction (optional override).
     */
    void handleInput();

    /**
     * @brief Draws the knob and arrow on the screen.
     */
    void draw() override;

    /**
     * @brief Returns the identifier of the associated knob.
     * @return Integer knob ID
     */
    int getKnobId();

    /**
     * @brief Returns the widget type as WidgetType::PotentiometerKnob.
     * @return WidgetType enum value
     */
    WidgetType getType() const override;


private:
    int knobId; ///< Logical identifier for this knob
    uint8_t value; ///< Analog value (0–255) for current knob state

    static const int8_t arrow_dx[360];  ///< pre-computed X-offsets for arrow direction, defined in the cpp file
    static const int8_t arrow_dy[360];  ///< pre-computed Y-offsets for arrow direction, defined in the cpp file


    /**
     * @brief Draws the indicator arrow based on the knob's value.
     * @param deg Degree of rotation (0–359)
     */
    void drawArrow(int deg);
};