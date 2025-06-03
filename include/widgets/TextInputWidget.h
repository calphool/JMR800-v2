#pragma once
#include "widgets/Widget.h"


class TextInputWidget : public Widget {

private:
    char text[25];
    int currentPosition;
    int width;

public:
    TextInputWidget(char* initialString, int x, int y, int w);
    void draw() override;
    void handleInput() override;
    WidgetType getType() const override;
    void advanceCurrentPosition();
    void backtrackCurrentPosition();
    void setCharAtCurrentPosition(char c);
};