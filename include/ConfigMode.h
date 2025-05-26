#pragma once

#include "AppMode.h"
#include "ConfigScreen.h"

class ConfigMode : public AppMode {
private:
    ConfigScreen screen;

public:
    const char* name() const override { return "System Config Mode"; }
    Screen* getScreen() override { return &screen; }
    void onEnter() override;
    void onExit() override;
    void loop() override;
    void showScreen() override;
};
