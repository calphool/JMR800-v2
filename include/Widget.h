// Widget.h
#pragma once

class Widget {
public:
    virtual void draw() = 0;
    virtual void handleInput() = 0;
    virtual ~Widget() {}
};
