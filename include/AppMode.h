#pragma once

#include "Screen.h"

class AppMode {
public:
    virtual const char* name() const = 0;
    virtual Screen* getScreen() = 0;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void loop() = 0;
    virtual ~AppMode() {}
    virtual void showScreen() = 0;
};

