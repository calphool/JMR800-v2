#pragma once

#include "widgets/CenterWidget.h"
#include "IDisplay.h"
#include "defines.h"



class RunModeWidgetWaveform : public CenterWidget{
    public:
        void draw(const knobConfig &cfg, uint8_t value, bool blinkOn);
};