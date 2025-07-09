#include "widgets/RunModeWidgetOctave.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include "widgets/WidgetUtils.h"


#define WHITE 1
#define BLACK 0 

void RunModeWidgetOctave::draw(const knobConfig &cfg,
                               uint8_t          value,
                               bool             blinkOn)
{
    /*--------------------------------------------------------------------
      Normalise the incoming value.
      ─────────────────────────────
      The calling code still delivers 0-255; we need the familiar 0-127
      range used in the original routine, so shift right once.
    --------------------------------------------------------------------*/
    value >>= 1;                 // 0-127

    /*--------------------------------------------------------------------
      Obtain the display handle and clear the widget’s drawing region.
    --------------------------------------------------------------------*/
    IDisplay *d = ScreenManager::getDisplay();

    d->fillRect(4, 10, 120, 48, BLACK);   // interior
    d->drawRect(4, 10, 120, 48, WHITE);   // border

    /*--------------------------------------------------------------------
      Static artwork – title, centre baseline, tick marks, end labels.
    --------------------------------------------------------------------*/
    centerText(cfg.name, 15);  

    // horizontal baseline
    d->drawLine(14, 30, 114, 30, WHITE);

    // tick marks every 5 px along the baseline
    for (int x = 14; x <= 114; x += 5)
        d->drawLine(x, 31, x, 32, WHITE);

    // octave labels
    d->setCursor(10, 38);  d->print("-12");
    d->setCursor(100, 38); d->print("+12");
    d->setCursor(60, 38);  d->print("0");

    /*--------------------------------------------------------------------
      Dynamic marker – indicates the presently selected octave shift.
      The original routine reversed the direction (127 → far left = –12,
      0 → far right = +12).  We keep exactly that mapping here.
    --------------------------------------------------------------------*/
    const uint8_t pos = static_cast<uint8_t>(100U * (127U - value) / 127U);
    const int     x   = 14 + pos;          // 14-114 inclusive

    if (blinkOn)                           // honour blinking request
        d->drawLine(x, 29, x, 27, WHITE);

    /*--------------------------------------------------------------------
      Numeric read-out in the lower centre.
    --------------------------------------------------------------------*/
    static char buf[8];
    std::snprintf(buf, sizeof(buf), "%03u", value);
    centerText(buf, 48);
}
