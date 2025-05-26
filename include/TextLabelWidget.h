// TextLabelWidget.h
#pragma once

#include "Widget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

extern const GFXfont TomThumb;

enum class LabelColor : uint8_t {
    BLACK = SH110X_BLACK,
    WHITE = SH110X_WHITE
};

class TextLabelWidget : public Widget {
private:
    Adafruit_SH1106G* display;
    String text;
    int x, y;
    uint8_t fontSize;
    bool useTinyFont;
    LabelColor color;

public:
    TextLabelWidget(Adafruit_SH1106G* disp, const String& t, int xpos, int ypos,
                    uint8_t fs = 1, bool tiny = false, LabelColor color = LabelColor::WHITE);

    void draw() override;
    void handleInput() override;

    void setText(const String& newText);
    void setPosition(int xpos, int ypos);
    void setFontSize(uint8_t fs);
    void useTiny(bool tiny);
    void setColor(LabelColor c);
};
