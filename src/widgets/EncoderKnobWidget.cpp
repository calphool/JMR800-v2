#include "widgets/EncoderKnobWidget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"

EncoderKnobWidget::EncoderKnobWidget(int x, int y) : KnobWidget(x, y), pressed(false) {
  log(LOG_VERBOSE, "Inside EncoderKnobWidget->constructor");
}

void EncoderKnobWidget::setPressed(bool p) {
    pressed = p;
}

void EncoderKnobWidget::draw() {
    toggle = !toggle;
    drawBezel();
    ScreenManager::getDisplay()->fillRect(x+2, y+1, 6, 4, pressed ? SH110X_BLACK : SH110X_WHITE);
    if(bHighlighted) {
        if(toggle) {
            int cx = x + 4;  // center x
            int cy = y + 2;  // center y
            int radius = 5;  // padding to fully enclose
            ScreenManager::getDisplay()->drawCircle(cx, cy, radius, SH110X_WHITE);
        }
        else {
            int cx = x + 4;  // center x
            int cy = y + 2;  // center y
            int radius = 5;  // padding to fully enclose
            ScreenManager::getDisplay()->drawCircle(cx, cy, radius, SH110X_BLACK);
        }
    }
}

void EncoderKnobWidget::handleInput() {
    setPressed(hardware.getEncoderSwitchStatus());
}

WidgetType EncoderKnobWidget::getType() const {
    return WidgetType::EncoderKnob;
}
