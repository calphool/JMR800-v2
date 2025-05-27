#pragma once


enum class WidgetType {
    Unknown,
    Rectangle,
    Knob,
    PotentiometerKnob,
    EncoderKnob,
    TextLabel,
    Button
};

class Widget {
public:
    Widget(int x, int y);
    virtual void draw() = 0;
    virtual void handleInput() = 0;
    virtual ~Widget() {}
    virtual WidgetType getType() const = 0;

protected:
    int x;
    int y;
};
