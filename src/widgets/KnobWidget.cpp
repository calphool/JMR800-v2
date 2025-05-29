#include "widgets/KnobWidget.h"
#include <Adafruit_SH110X.h>
#include "ScreenManager.h"
#include "Logging.h"

KnobWidget::KnobWidget(int x, int y)
    : Widget(x, y, false) {
  log(LOG_VERBOSE, "Inside KnobWidget->constructor");
}

void KnobWidget::drawBezel() {
    ScreenManager::getDisplay()->drawLine(x + 3, y,     x + 6, y,     SH110X_WHITE);
    ScreenManager::getDisplay()->drawLine(x + 3, y + 5, x + 6, y + 5, SH110X_WHITE);
    ScreenManager::getDisplay()->drawLine(x + 1, y + 2, x + 1, y + 3, SH110X_WHITE);
    ScreenManager::getDisplay()->drawLine(x + 8, y + 2, x + 8, y + 3, SH110X_WHITE);
}

WidgetType KnobWidget::getType() const {
    return WidgetType::Knob;
}
