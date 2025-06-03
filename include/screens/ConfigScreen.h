#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"
#include "screens/KnobConfigDialog.h"

/* -----------------------------------------------------------------
   |  ConfigScreen -- manages the screen where settings are updated|
   |                                                               |
   ----------------------------------------------------------------- */
class ConfigScreen : public Screen {

private:
    std::vector<Widget*> widgets;
    int active_knob = -1;
    KnobConfigDialog* knobConfigDialog = nullptr; // Pointer to the dialog for knob configuration

public:
    ConfigScreen();
    ~ConfigScreen();  

    void addWidget(Widget* w);
    void draw() override;
    void handleInput() override;
    void onEnter() override;
    void onExit() override;
    void highlightActiveKnob(int i);
    void changeScreenMode(uint knobIndex);
};
