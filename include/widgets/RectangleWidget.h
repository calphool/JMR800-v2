#pragma once

#include "widgets/Widget.h"
#include <Adafruit_SH110X.h>

enum class RectColor : uint8_t {
    BLACK = SH110X_BLACK,
    WHITE = SH110X_WHITE
};

class RectangleWidget : public Widget {
private:
    int width, height;
    bool filled;
    RectColor color;

public:
    RectangleWidget(int x, int y, int width, int height, bool filled, RectColor color);

    void draw() override;
    void handleInput() override;

    void setPosition(int xpos, int ypos);
    void setSize(int w, int h);
    void setFilled(bool f);
    void setColor(RectColor c);

protected:
    WidgetType getType() const override;
};
