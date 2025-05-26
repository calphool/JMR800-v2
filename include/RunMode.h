#pragma once

#include "AppMode.h"
#include "RunModeScreen.h"

class RunMode : public AppMode {
private:
    RunModeScreen screen;

public:
    const char* name() const override { return "Run Mode"; }
    Screen* getScreen() override { return &screen; }
    void onEnter() override;
    void onExit() override;
    void loop() override;
    void showScreen() override;
};
