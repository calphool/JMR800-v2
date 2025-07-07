#include "widgets/RunModeWidgetEnvMode.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>

#define WHITE 1
#define BLACK 0 

void RunModeWidgetEnvMode::draw( const knobConfig &cfg, uint8_t value, bool blinkOn)
{
        static char buf[16];
        value = value >> 1;
        IDisplay* d = ScreenManager::getDisplay();
        d->fillRect(4, 10, 120, 48, BLACK);
        d->drawRect(4, 10, 120, 48, WHITE);

        d->drawLine(14, 30, 114, 30, WHITE);
        d->setCursor(22, 15);
        d->print(cfg.name);

        d->setCursor(54,48);
        sprintf(buf, "%03d", value);
        d->print(buf);
}