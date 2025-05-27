#pragma once

class Widget {
public:
    Widget(int x, int y);
    virtual void draw() = 0;
    virtual void handleInput() = 0;
    virtual ~Widget() {}

protected:
    int x;
    int y;
};
