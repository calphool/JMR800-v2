/**
 * @file EncoderKnobWidget.cpp
 * @brief Implements a UI widget that visually represents the encoder knob and its pressed state.
 *
 * This widget draws a graphical encoder knob with optional highlight animation and reacts to
 * the hardware encoder button press.
 */

#include "widgets/EncoderKnobWidget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"


/**
 * @brief Constructs an EncoderKnobWidget at the given screen coordinates.
 *
 * Initializes the widget in an unpressed state.
 *
 * @param x Horizontal position of the knob.
 * @param y Vertical position of the knob.
 */
EncoderKnobWidget::EncoderKnobWidget(int x, int y) : KnobWidget(x, y), pressed(false) {
  log(LOG_VERBOSE, "Inside EncoderKnobWidget->constructor");
}


/**
 * @brief Sets the pressed state of the encoder widget.
 *
 * @param p True if the encoder button is pressed.
 */
void EncoderKnobWidget::setPressed(bool p) {
    pressed = p;
}


/**
 * @brief Draws the encoder knob on the display.
 *
 * Draws the bezel, fills the knob based on press state, and shows a pulsing highlight if enabled.
 */
void EncoderKnobWidget::draw() {
    toggle = !toggle;
    drawBezel();
    ScreenManager::getDisplay()->fillRect(x+2, y+1, 6, 4, pressed ? SH110X_BLACK : SH110X_WHITE);
    if(bHighlighted) {
        int cx = x + 4;  // center x
        int cy = y + 2;  // center y
        int radius = 5;  // padding to fully enclose
        if(toggle) {
            ScreenManager::getDisplay()->drawCircle(cx, cy, radius, SH110X_WHITE);
        }
        else {
            ScreenManager::getDisplay()->drawCircle(cx, cy, radius, SH110X_BLACK);
        }
    }
}

/**
 * @brief Sets the pressed state of the encoder knob widget based on current hardware status
 */
void EncoderKnobWidget::handleInput() {
    setPressed(hardware.getEncoderSwitchStatus());
}


/**
 * @brief Returns the type of the widget.
 *
 * Used for identifying the widget as an encoder knob.
 *
 * @return WidgetType::EncoderKnob
 */
WidgetType EncoderKnobWidget::getType() const {
    return WidgetType::EncoderKnob;
}
