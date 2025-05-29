#include "widgets/PotentiometerKnobWidget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"

PotentiometerKnobWidget::PotentiometerKnobWidget(int _knobId, int x, int y) : KnobWidget(x, y),
      knobId(_knobId), value(0) {
  log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->constructor");
}

void PotentiometerKnobWidget::setValue(uint8_t v) {
    value = v;
}

void PotentiometerKnobWidget::setHighlighted(bool h) {
    bHighlighted = h;
}

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

void PotentiometerKnobWidget::draw() {
    log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->draw()");
    toggle = !toggle;

    bool fillBlack = bHighlighted && toggle;

    drawBezel();

    ScreenManager::getDisplay()->fillRect(x+2, y+1, 6, 4, fillBlack ? SH110X_BLACK : SH110X_WHITE);
    
    int deg = static_cast<int>((360.0f * value) / 255.0f);
    drawArrow(deg);

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

void PotentiometerKnobWidget::handleInput() {
    if(knobId > NUM_KNOBS || knobId < 0) {
        char buf[128];
        sprintf(buf, "PotentiometerKnobWidget->handleInput() invoked with an invalid knobId: %d", knobId);
        log(LOG_ERROR, buf);
        return;
    }

    setValue(hardware.getKnobValue(knobId));
}

WidgetType PotentiometerKnobWidget::getType() const {
    return WidgetType::PotentiometerKnob;
}