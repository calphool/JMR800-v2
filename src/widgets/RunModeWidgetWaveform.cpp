#include "widgets/RunModeWidgetWaveform.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include <algorithm>   // std::min
#include <cstdlib>     // std::rand, std::srand  (simulator only)
#include "widgets/WidgetUtils.h"  


#ifndef TARGET_TEENSY                 // native/simulator build
static inline long random(long from, long to) {
    // NOTE: call std::srand() once at program start if deterministic
    return from + (std::rand() % (to - from));
}
#endif

#define WHITE 1
#define BLACK 0 

void RunModeWidgetWaveform::draw(const knobConfig &cfg,
                                 uint8_t value,
                                 bool    /*blinkOn*/)
{
    static char buf[8];

    /*--------------------------------------------------------------
      1.  Normalise value to 0-127 and clear the widget area
    --------------------------------------------------------------*/
    value >>= 1;                                   // 0-255 → 0-127
    IDisplay *d = ScreenManager::getDisplay();

    d->fillRect(4, 10, 120, 48, BLACK);
    d->drawRect(4, 10, 120, 48, WHITE);

    centerText(cfg.name, 15);              

    /*--------------------------------------------------------------
      2.  Waveform name and miniature preview
          Preview X-range: 14-114   Y-range: 20-30 (10 px tall)
    --------------------------------------------------------------*/

    if (value < 32) {                              // Noise
        centerText("Noise", 37);
        int oldX = 14, oldY = 0;
        for (int x = 14; x <= 114; ) {
            int newX = std::min(x + static_cast<int>(random(0, 3)), 114);
            int newY = random(0, 8) - random(0, 6);
            d->drawLine(oldX, oldY + 28, newX, newY + 28, WHITE);
            oldX = newX;
            oldY = newY;
            x     = newX + 1;
        }
    }
    else if (value < 64) {                         // Square
        centerText("Square", 37);
        constexpr int y0 = 34, h = 10, w = 10;
        for (int x = 14; x <= 100; x += 2 * w) {
            d->drawLine(x, y0, x, y0 - h, WHITE);
            d->drawLine(x, y0 - h, x + w, y0 - h, WHITE);
            d->drawLine(x + w, y0 - h, x + w, y0, WHITE);
            d->drawLine(x + w, y0, x + 2 * w, y0, WHITE);
        }
    }
    else if (value < 96) {                         // Pulse
        centerText("Pulse", 37);
        constexpr int y0 = 34, h = 10, period = 20, pw = 3;
        for (int x = 14; x < 114; x += period) {
            d->drawLine(x, y0, x, y0 - h, WHITE);
            d->drawLine(x, y0 - h, x + pw, y0 - h, WHITE);
            d->drawLine(x + pw, y0 - h, x + pw, y0, WHITE);
            d->drawLine(x + pw, y0, x + period, y0, WHITE);
        }
    }
    else {                                         // Sawtooth
        centerText("Sawtooth", 37);
        constexpr int y0 = 34, h = 10, period = 20;
        for (int x = 14; x < 114; x += period) {
            d->drawLine(x, y0, x + period, y0 - h, WHITE);
            d->drawLine(x + period, y0 - h, x + period, y0, WHITE);
        }
    }

    /*--------------------------------------------------------------
      3.  Numeric value (0-127) at bottom-centre
    --------------------------------------------------------------*/
    sprintf(buf, "%03u", value);
    centerText(buf, 48); 
}
