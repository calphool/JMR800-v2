#include "widgets/PotentiometerKnobWidget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"

PotentiometerKnobWidget::PotentiometerKnobWidget(int _knobId, int x, int y) : KnobWidget(x, y),
      knobId(_knobId), value(0), highlighted(false), blinkState(false) {
  log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->constructor");
}

void PotentiometerKnobWidget::setValue(uint8_t v) {
    log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->setValue()");
    value = v;
}

void PotentiometerKnobWidget::setHighlighted(bool h) {
    log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->setHighlighted()");
    highlighted = h;
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
    log(LOG_VERBOSE, "Inside PotentiometerKnobWidget->handleInput()");

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