#include "widgets/RunModeWidget0to10.h"
#include "IDisplay.h"
#include "defines.h"
#include "ScreenManager.h"
#include <cstdio>

#define WHITE 1
#define BLACK 0 

void RunModeWidget0to10::draw(const knobConfig &cfg, uint8_t value, bool blinkOn)
    {
        static char buf[16];
        value = value >> 1;
        IDisplay* d = ScreenManager::getDisplay();
        d->fillRect(4, 10, 120, 48, BLACK);
        d->drawRect(4, 10, 120, 48, WHITE);

        d->drawLine(14, 30, 114, 30, WHITE);
        d->setCursor(22, 15);
        d->print(cfg.name);

        if (cfg.cmdbyte == 0x89) {        // Fine-tune vs “normal” scale
            d->setCursor(10,38);  d->print("-50");
            d->setCursor(60,38);  d->print("0");
            d->setCursor(100,38); d->print("+50");
        } else {
            d->setCursor(10,38);  d->print("0");
            d->setCursor(100,38); d->print("100");
        }

        for (int x = 14; x <= 114; x += 5)
            d->drawLine(x, 31, x, 32, WHITE);

        uint8_t pos = 100 * (127 - value) / 127;
        d->drawLine(14 + pos,
                   blinkOn ? 27 : 29,          // two-pixel flash
                   14 + pos,
                   29,
                   WHITE);

        d->setCursor(54,48);
        sprintf(buf, "%03d", value);
        d->print(buf);
    }
