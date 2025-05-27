#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"

/* --------------------------------------------------------------
   |  RunModeScreen -- manages the runtime UI view, displaying   |
   |  widgets and routing input to them during operation         |
   -------------------------------------------------------------- */
class RunModeScreen : public Screen {

private:
    std::vector<Widget*> widgets;

public:
    RunModeScreen();
    ~RunModeScreen();  

    void addWidget(Widget* w);
    void draw() override;
    void handleInput() override;
    void onEnter() override;
    void onExit() override;
};
