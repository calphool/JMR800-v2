#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include "widgets/Widget.h"
#include "screens/Screen.h"
#include "widgets/EncoderAttachedNumericWidget.h"

/* ----------------------------------------------------------------------
   |  DirectCommandScreen -- Allows the sending of commands directly    |
   |  without using the controls                                        |
   ---------------------------------------------------------------------- */
class DirectCommandScreen : public Screen {

private:
    std::vector<Widget*> widgets;
    EncoderAttachedNumericWidget* cmdWidget;
    EncoderAttachedNumericWidget* byteWidget;
    char buff[20];

public:
    DirectCommandScreen();
    ~DirectCommandScreen();  

    void addWidget(Widget* w);
    void draw() override;
    void handleInput() override;
    void onEnter() override;
    void onExit() override;
    void advanceActiveControl();
};