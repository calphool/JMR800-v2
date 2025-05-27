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
    drawBezel();
    ScreenManager::getDisplay()->fillRect(x+2, y+1, 6, 4, pressed ? SH110X_BLACK : SH110X_WHITE);
}

void EncoderKnobWidget::handleInput() {
    setPressed(hardware.getEncoderSwitchStatus());
}

WidgetType EncoderKnobWidget::getType() const {
    return WidgetType::EncoderKnob;
}
