#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"

/* -----------------------------------------------------------------
   |  ControlsTestScreen -- shows all the controls and their state |
   |                                                               |
   ----------------------------------------------------------------- */
class ControlsTestScreen : public Screen {

private:
    std::vector<Widget*> widgets;

public:
    ControlsTestScreen();
    ~ControlsTestScreen();  

    void addWidget(Widget* w);
    void draw() override;
    void handleInput() override;
    void onEnter() override;
    void onExit() override;
};
