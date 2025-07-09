#include "widgets/RunModeWidgetLFOWave.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include "widgets/WidgetUtils.h"  


#define WHITE 1
#define BLACK 0 

#ifndef TEENSY_TARGET      
  #include <cstdlib>                // std::rand
  #include <cmath>                  // std::sinf

  // 1. Arduino-style random(low, highExclusive)
  static inline int random(int low, int highExclusive)
  {
      return low + static_cast<int>(std::rand() % (highExclusive - low));
  }

  // 2. PI constant if not already provided
  #ifndef PI
    constexpr float PI = 3.14159265358979323846f;
  #endif

#endif

void RunModeWidgetLFOWave::draw(const knobConfig &cfg,
                                uint8_t           value,
                                bool              blinkOn)
{
    /*--------------------------------------------------------------------
      1. Normalise knob value (caller still passes 0–255 → shift ⇒ 0–127)
    --------------------------------------------------------------------*/
    value >>= 1;

    /*--------------------------------------------------------------------
      2. Prepare drawing surface
    --------------------------------------------------------------------*/
    IDisplay *d = ScreenManager::getDisplay();

    d->fillRect(4, 10, 120, 48, BLACK);   // clear widget region
    d->drawRect(4, 10, 120, 48, WHITE);   // border

    centerText(cfg.name, 15);           

    /*--------------------------------------------------------------------
      3. Static axis common to all waveforms
    --------------------------------------------------------------------*/
    constexpr int waveStartX = 14;
    constexpr int waveEndX   = 114;
    constexpr int waveStartY = 32;
    constexpr int waveHeight = 8;

    /*--------------------------------------------------------------------
      4. Waveform rendering decided by value
         < 32  → Random (Sample & Hold)
         < 64  → Square
         ≥ 64  → Sine
    --------------------------------------------------------------------*/
    if (value < 32)                     // ───── Random S-&-H ─────
    {
        constexpr int wavePeriod = 10;
        int lastX = waveStartX;
        int lastY = waveStartY - random(-waveHeight/2, waveHeight/2 + 1);

        for (int x = waveStartX; x < waveEndX; x += wavePeriod)
        {
            int newY = waveStartY - random(-waveHeight/2, waveHeight/2 + 1);

            d->drawLine(lastX,            lastY-3, x + wavePeriod, lastY-3, WHITE);
            d->drawLine(x + wavePeriod,   lastY-3, x + wavePeriod, newY-3, WHITE);

            lastX = x + wavePeriod;
            lastY = newY;
        }
        centerText("Random S&H", 35); 
    }
    else if (value < 64)                // ───── Square ─────
    {
        constexpr int wavePeriod = 20;
        for (int x = waveStartX; x < waveEndX; x += wavePeriod)
        {
            d->drawLine(x,                     waveStartY,              x,                   waveStartY - waveHeight, WHITE);
            d->drawLine(x,                     waveStartY - waveHeight, x + wavePeriod / 2,  waveStartY - waveHeight, WHITE);
            d->drawLine(x + wavePeriod / 2,    waveStartY - waveHeight, x + wavePeriod / 2,  waveStartY,              WHITE);
            d->drawLine(x + wavePeriod / 2,    waveStartY,              x + wavePeriod,      waveStartY,              WHITE);
        }
        centerText("Square", 35);
    }
    else                                 // ───── Sine ─────
    {
        const int waveWidth = waveEndX - waveStartX;
        int lastX = waveStartX;
        int lastY = waveStartY;

        for (int x = waveStartX; x <= waveEndX; ++x)
        {
            float angle = static_cast<float>(x - waveStartX) / waveWidth * 4.0f * PI;
            int   y     = waveStartY - static_cast<int>(sinf(angle) * waveHeight * 0.6f);

            d->drawLine(lastX, lastY-4, x, y-4, WHITE);
            lastX = x;
            lastY = y;
        }
        centerText("Sine", 35);
    }

    /*--------------------------------------------------------------------
      5. Numeric value read-out (0-127) at widget bottom
    --------------------------------------------------------------------*/
    static char buf[8];
    std::snprintf(buf, sizeof(buf), "%03u", value);
    centerText(buf, 48); 
}
