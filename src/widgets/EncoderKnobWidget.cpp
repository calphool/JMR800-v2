#include "widgets/EncoderKnobWidget.h"
#include "ScreenManager.h"

EncoderKnobWidget::EncoderKnobWidget(int x, int y) : KnobWidget(x, y), pressed(false) {}

void EncoderKnobWidget::setPressed(bool p) {
    pressed = p;
}

void EncoderKnobWidget::draw() {
    drawBezel();
    ScreenManager::getDisplay()->fillRect(x+2, y+1, 6, 4, pressed ? SH110X_BLACK : SH110X_WHITE);
}

void EncoderKnobWidget::handleInput() {
    
}
