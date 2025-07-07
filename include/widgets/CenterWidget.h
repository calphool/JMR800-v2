#pragma once
#include "IDisplay.h"
#include "defines.h"

class CenterWidget {
public:
    virtual ~CenterWidget() = default;

    /** Draw the widget’s contents.
     *  @param d         reference to SDLDisplay / SH110X wrapper
     *  @param cfg       active knob configuration (for cmdbyte tests)
     *  @param value     0-127 knob value
     *  @param blinkOn   true every other frame for blinking elements
     */
    virtual void draw(const knobConfig &cfg,
                      uint8_t value,
                      bool blinkOn) = 0;
};
