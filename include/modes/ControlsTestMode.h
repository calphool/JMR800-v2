#pragma once

#include "modes/AppMode.h"
#include "screens/ControlsTestScreen.h"

class ControlsTestMode : public AppMode {
private:
    ControlsTestScreen screen;

public:
    const char* name() const override { return "Controls Test Mode"; }
    Screen* getScreen() override { return &screen; }
    void onEnter() override;
    void onExit() override;
    void loop() override;
    void showScreen() override;
};
