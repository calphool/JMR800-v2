#pragma once

#include "widgets/Widget.h"
#include <Adafruit_SH110X.h>

class KnobWidget : public Widget {
public:
    KnobWidget(int x, int y);

protected:
    void drawBezel();
    WidgetType getType() const override;
};
