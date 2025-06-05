/**
 * @file PotentiometerKnobWidget.cpp
 * @brief Implements the PotentiometerKnobWidget class, which visualizes a physical knob's current state.
 *
 * This widget provides a circular knob display with an arrow indicating value.
 * It supports visual highlighting and links to actual analog knob input on the hardware.
 */

#include "widgets/PotentiometerKnobWidget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"



const int8_t PotentiometerKnobWidget::arrow_dx[360] = {
    -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };

const int8_t PotentiometerKnobWidget::arrow_dy[360] = {
    3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    };


/**
 * @brief Constructs a PotentiometerKnobWidget instance for a specific knob.
 *
 * @param _knobId Hardware knob index this widget reflects.
 * @param x X-coordinate of the widget.
 * @param y Y-coordinate of the widget.
 */
PotentiometerKnobWidget::PotentiometerKnobWidget(int _knobId, int x, int y) : KnobWidget(x, y),
      knobId(_knobId), value(0) {
  log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->constructor");
}


/**
 * @brief Returns the ID of the knob this widget is linked to.
 *
 * @return int The knob ID.
 */
int PotentiometerKnobWidget::getKnobId() {
    return knobId;
}


/**
 * @brief Sets the value shown by the widget.
 *
 * @param v A value between 0 and 255 representing the knob's current position.
 */
void PotentiometerKnobWidget::setValue(uint8_t v) {
    value = v;
}


/**
 * @brief Sets whether this widget is visually highlighted.
 *
 * @param h true to highlight, false to display normally.
 */
void PotentiometerKnobWidget::setHighlighted(bool h) {
    bHighlighted = h;
}



/**
 * @brief Draws an arrow on the knob indicating position in degrees.
 *
 * @param deg Angle in degrees (0–359) used to compute arrow direction.
 */
void PotentiometerKnobWidget::drawArrow(int deg) {
    log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->drawArrow()");

    int cx = x + 4;
    int cy = y + 3;

    deg = (deg - 45 + 360) % 360;
    int index = deg % 360;

    int ex = cx + arrow_dx[index];
    int ey = cy + arrow_dy[index];

    ScreenManager::getDisplay()->drawLine(cx, cy, ex, ey, SH110X_BLACK);
}


/**
 * @brief Draws the full potentiometer knob on the display.
 *
 * Includes bezel, fill, and optional highlight circle. Arrow angle reflects value.
 */
void PotentiometerKnobWidget::draw() {
    log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->draw()");
    toggle = !toggle;

    bool fillBlack = bHighlighted && toggle;

    drawBezel();

    ScreenManager::getDisplay()->fillRect(x+2, y+1, 6, 4, fillBlack ? SH110X_BLACK : SH110X_WHITE);
    
    int deg = static_cast<int>((360.0f * value) / 255.0f);
    drawArrow(deg);

    if(bHighlighted) {
        int cx = x + 4;  // center x
        int cy = y + 2;  // center y
        int radius = 4;  // padding to fully enclose
        if(toggle) {
            ScreenManager::getDisplay()->drawCircle(cx, cy, radius, SH110X_WHITE);
        }
        else {
            ScreenManager::getDisplay()->drawCircle(cx, cy, radius, SH110X_BLACK);
        }
    }
}

/**
 * @brief Updates the widget value from the actual hardware knob.
 *
 * Reads analog value via HardwareManager and maps it to the 0–255 range.
 */
void PotentiometerKnobWidget::handleInput() {
    if(knobId > NUM_KNOBS || knobId < 0) {
        char buf[128];
        sprintf(buf, "PotentiometerKnobWidget->handleInput() invoked with an invalid knobId: %d", knobId);
        log(LOG_ERROR, buf);
        return;
    }

    setValue(hardware.getKnobValue(knobId));
}

/**
 * @brief Returns the widget type identifier.
 *
 * @return WidgetType::PotentiometerKnob
 */
WidgetType PotentiometerKnobWidget::getType() const {
    return WidgetType::PotentiometerKnob;
}