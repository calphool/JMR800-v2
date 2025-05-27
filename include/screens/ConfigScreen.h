#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"

/* -----------------------------------------------------------------
   |  ConfigScreen -- manages the screen where settings are updated|
   |                                                               |
   ----------------------------------------------------------------- */
class ConfigScreen : public Screen {

private:
    std::vector<Widget*> widgets;

public:
    ConfigScreen();
    ~ConfigScreen();  

    void addWidget(Widget* w);
    void draw() override;
    void handleInput() override;
    void onEnter() override;
    void onExit() override;
};
