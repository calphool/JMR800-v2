#include "widgets/RunModeWidget21Off.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include "widgets/WidgetUtils.h"

#define WHITE 1
#define BLACK 0 

void RunModeWidget21Off::draw(const knobConfig &cfg,
                              uint8_t           value,
                              bool              blinkOn)
{
    /*--------------------------------------------------------------------
      1 . Normalise knob value (incoming 0 – 255 ➜ 0 – 127)
    --------------------------------------------------------------------*/
    value >>= 1;

    /*--------------------------------------------------------------------
      2 . Clear widget region and draw static frame
    --------------------------------------------------------------------*/
    IDisplay *d = ScreenManager::getDisplay();

    d->fillRect(4, 10, 120, 48, BLACK);     // interior
    d->drawRect(4, 10, 120, 48, WHITE);     // border

    /*--------------------------------------------------------------------
      3 . Title
    --------------------------------------------------------------------*/
    centerText(cfg.name, 15);

    /*--------------------------------------------------------------------
      4 . Baseline, tick marks, and labels “2  1  OFF”
    --------------------------------------------------------------------*/
    constexpr int kStartX   = 14;
    constexpr int kEndX     = 114;
    constexpr int kBaseY    = 30;

    d->drawLine(kStartX, kBaseY, kEndX, kBaseY, WHITE);

    for (int x = kStartX, label = 2; x <= kEndX; x += 50, --label)
    {
        d->drawLine(x, kBaseY + 1, x, kBaseY + 2, WHITE);   // small tick

        if (label == 0) {
            d->setCursor(x - 12, 38);
            d->print("OFF");
        }
        else { 
            char buf[4];                                    
            sprintf(buf, "%d", label);
            d->setCursor(x - 2, 38);                         
            d->print(buf); 
        }
    }

    /*--------------------------------------------------------------------
      5 . Position marker (blinks when blinkOn == true)
          Mapping: 0 → far right (OFF), 127 → far left (2)
          pos = 0-100, then offset by 14 px.
    --------------------------------------------------------------------*/
    const uint8_t pos = static_cast<uint8_t>(100U * (127U - value) / 127U);
    const int     x   = kStartX + pos;

    if (blinkOn)
        d->drawLine(x, kBaseY - 1, x, kBaseY - 3, WHITE);
    else
        d->drawLine(x, kBaseY - 1, x, kBaseY - 3, BLACK);   // erase when “off”

    /*--------------------------------------------------------------------
      6 . Numeric read-out centred at bottom
    --------------------------------------------------------------------*/
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%03u", value);
    centerText(buf, 48);
}
