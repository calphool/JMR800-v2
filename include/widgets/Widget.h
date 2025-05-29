#pragma once


enum class WidgetType {
    Unknown,
    Rectangle,
    Knob,
    PotentiometerKnob,
    EncoderKnob,
    TextLabel,
    Button,
    EncoderAttachedNumericWidget
};

class Widget {
public:
    Widget(int x, int y, bool highlighted);
    virtual void draw() = 0;
    virtual void handleInput() = 0;
    virtual ~Widget() {}
    virtual WidgetType getType() const = 0;
    void setHighlighted(bool b);

protected:
    int x;
    int y;
    bool bHighlighted;
    bool toggle;
};
