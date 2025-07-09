#include "widgets/RunModeWidget321Off.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include "widgets/WidgetUtils.h"

#define WHITE 1
#define BLACK 0 

void RunModeWidget321Off::draw(const knobConfig &cfg, uint8_t value, bool blinkOn)
{
        static char buf[16];
        value = value >> 1;
        IDisplay* d = ScreenManager::getDisplay();
        d->fillRect(4, 10, 120, 48, BLACK);
        d->drawRect(4, 10, 120, 48, WHITE);

        d->drawLine(14, 30, 114, 30, WHITE);
        centerText(cfg.name, 15);

        sprintf(buf, "%03d", value);
        centerText(buf, 48);
}