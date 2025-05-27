#include "widgets/PotentiometerKnobWidget.h"
#include "ScreenManager.h"

PotentiometerKnobWidget::PotentiometerKnobWidget(int knobId, int x, int y) : KnobWidget(x, y),
      value(0), highlighted(false), blinkState(false) {}

void PotentiometerKnobWidget::setValue(uint8_t v) {
    value = v;
}

void PotentiometerKnobWidget::setHighlighted(bool h) {
    highlighted = h;
}

void PotentiometerKnobWidget::drawArrow(int deg) {
    int cx = x + 4;
    int cy = y + 3;

    deg = (deg - 45 + 360) % 360;
    int index = deg % 360;

    int ex = cx + arrow_dx[index];
    int ey = cy + arrow_dy[index];

    ScreenManager::getDisplay()->drawLine(cx, cy, ex, ey, SH110X_BLACK);
}

void PotentiometerKnobWidget::draw() {
    if (highlighted) {
        blinkState = !blinkState;
    }

    bool fillBlack = highlighted && blinkState;

    drawBezel();

    ScreenManager::getDisplay()->fillRect(x+2, y+1, 6, 4, fillBlack ? SH110X_BLACK : SH110X_WHITE);
    
    int deg = static_cast<int>((360.0f * value) / 255.0f);
    drawArrow(deg);
}

void PotentiometerKnobWidget::handleInput() {

}