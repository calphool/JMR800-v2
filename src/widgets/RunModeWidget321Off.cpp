#include "widgets/RunModeWidget321Off.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include "widgets/WidgetUtils.h"

#define WHITE 1
#define BLACK 0 

void RunModeWidget321Off::draw(const knobConfig &cfg,
                               uint8_t           value,
                               bool              blinkOn)
{
    // Normalize value: incoming is 0–255, shift to 0–127
    value >>= 1;

    // Display object
    IDisplay *d = ScreenManager::getDisplay();

    // Clear the drawing area
    d->fillRect(4, 10, 120, 48, BLACK);
    d->drawRect(4, 10, 120, 48, WHITE);

    // Title and baseline
    centerText(cfg.name, 15);
    d->drawLine(14, 30, 113, 30, WHITE);

    // Draw tick marks and labels
    uint8_t ctr = 3;
    for (int x = 14; x <= 114; x += 33, --ctr)
    {
        d->drawLine(x, 31, x, 32, WHITE);    // tick mark
        d->setCursor(x - 2, 38);

        if (cfg.cmdbyte == 0x87 && ctr == 3)
        {
            d->setCursor(x - 3, 38);
            d->print("XMod");
        }
        else if (ctr == 0)
        {
            d->setCursor(x - 13, 38);
            d->print("OFF");
        }
        else
        {
            char buf[8];
            std::snprintf(buf, sizeof(buf), "%u", ctr);
            d->print(buf);
        }
    }

    // Marker position logic: 0 → far right, 127 → far left
    const uint8_t pos = static_cast<uint8_t>(99U * (127U - value) / 127U);
    const int markerX = 14 + pos;

    if (blinkOn)
        d->drawLine(markerX, 29, markerX, 27, WHITE);
    else
        d->drawLine(markerX, 29, markerX, 27, BLACK); // hide marker

    // Numeric display centered at bottom
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%03u", value);
    centerText(buf, 48);
}
