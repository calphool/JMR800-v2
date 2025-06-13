/**
 * @file KnobWidget.cpp
 * @brief Defines the base class for knob-based widgets, providing shared bezel drawing behavior.
 *
 * The KnobWidget is a visual representation of a rotary control, typically subclassed
 * by more specific widgets like PotentiometerKnobWidget.
 */

#include "widgets/KnobWidget.h"
#include <Adafruit_SH110X.h>
#include "ScreenManager.h"
#include "Logging.h"


/**
 * @brief Constructs a KnobWidget at the given position.
 *
 * Initializes the widget without highlighting by default.
 *
 * @param x The horizontal screen position.
 * @param y The vertical screen position.
 */
KnobWidget::KnobWidget(int x, int y)
    : Widget(x, y, false) {
  log(LOG_VERBOSE, "Inside KnobWidget->constructor", __func__);
}


/**
 * @brief Draws a standard knob bezel using line primitives.
 *
 * This function renders a stylized circular outline using horizontal and vertical lines,
 * intended to resemble a physical knob's contour.
 */
void KnobWidget::drawBezel() {
    ScreenManager::getDisplay()->drawLine(x + 3, y,     x + 6, y,     SH110X_WHITE);
    ScreenManager::getDisplay()->drawLine(x + 3, y + 5, x + 6, y + 5, SH110X_WHITE);
    ScreenManager::getDisplay()->drawLine(x + 1, y + 2, x + 1, y + 3, SH110X_WHITE);
    ScreenManager::getDisplay()->drawLine(x + 8, y + 2, x + 8, y + 3, SH110X_WHITE);
}

/**
 * @brief Returns the widget type identifier.
 *
 * Used for runtime type identification of the widget.
 *
 * @return WidgetType::Knob
 */
WidgetType KnobWidget::getType() const {
    return WidgetType::Knob;
}
