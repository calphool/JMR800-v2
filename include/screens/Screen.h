#pragma once

class Screen {
public:
    virtual void draw() = 0;
    virtual void handleInput() = 0;
    virtual void onEnter() {}  // Optional: load state
    virtual void onExit() {}   // Optional: cleanup or save
    virtual ~Screen() {}
};

