#pragma once

#include <Adafruit_SH110X.h>
#include "Screen.h"

class ScreenManager {
private:
    Screen* activeScreen = nullptr;
    unsigned long lastScreenLoop = 0;
    static Adafruit_SH1106G display;

public:
    void init();
    void setScreen(Screen* screen);
    void loop();
    Screen* getScreen() const;
    static Adafruit_SH1106G* getDisplay();
};

// This declares the global instance that is defined elsewhere
extern ScreenManager screenManager;

// Optional constant you reference (must be defined somewhere)
constexpr unsigned long ScreenManagerLoopMS = 33;
