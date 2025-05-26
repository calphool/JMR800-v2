#pragma once

#include "AppMode.h"
#include "DirectCommandScreen.h"

class DirectCommandMode : public AppMode {
private:
    DirectCommandScreen screen;

public:
    const char* name() const override { return "Direct Command Mode"; }
    Screen* getScreen() override { return &screen; }
    void onEnter() override;
    void onExit() override;
    void loop() override;
    void showScreen() override;
};
