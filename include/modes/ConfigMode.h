#pragma once

#include "modes/AppMode.h"
#include "screens/ConfigScreen.h"

class ConfigMode : public AppMode {
private:
    ConfigScreen theConfigScreen;;

public:
    const char* name() const override { return "System Config Mode"; }
    Screen* getScreen() override { return &theConfigScreen; }
    void onEnter() override;
    void onExit() override;
    void loop() override;
    void showScreen() override;
};
