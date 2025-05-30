#pragma once

#include "widgets/Widget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

class EncoderAttachedNumericWidget : public Widget {
private:
    String text;
    uint8_t fontSize;

public:
    EncoderAttachedNumericWidget(int x, int y, int min, int max, char* printFormat);

    void draw() override;
    void handleInput() override;
    void setPosition(int xpos, int ypos);
    void attachToEncoder();
    void detachFromEncoder();
    bool isAttachedToEncoder();
    uint getValue();

private:
    uint control_value;
    bool bIsAttachedToEncoder;
    int min;
    int max;
    char* printFormat;

protected:
    WidgetType getType() const override;
};
