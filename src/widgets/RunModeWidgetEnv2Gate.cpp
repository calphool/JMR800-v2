#include "widgets/RunModeWidgetEnv2Gate.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include "widgets/WidgetUtils.h"

#define WHITE 1
#define BLACK 0 

void RunModeWidgetEnv2Gate::draw(const knobConfig &cfg,
                                 uint8_t           value,
                                 bool              /*blinkOn*/)
{
    /*--------------------------------------------------------------------
      1. Normalise knob value (0–255 ➜ 0–127) and set-up drawing surface
    --------------------------------------------------------------------*/
    value >>= 1;                                   // 0-127

    IDisplay *d = ScreenManager::getDisplay();
    d->fillRect(4, 10, 120, 48, BLACK);
    d->drawRect(4, 10, 120, 48, WHITE);

    centerText(cfg.name, 15);

    /*--------------------------------------------------------------------
      2. Common geometry constants for the envelope / gate box
    --------------------------------------------------------------------*/
    constexpr int rectX1 = 14;
    constexpr int rectY1 = 23;
    constexpr int rectX2 = 114;
    constexpr int rectY2 = 35;
    constexpr int rectH  = rectY2 - rectY1;        // 14 px

    // ADSR-ish proportions (unchanged from the old sketch)
    constexpr int attackTime   = 20;               // px
    constexpr int decayTime    = 20;               // px
    constexpr int sustainLevel = 30;               // % of height
    constexpr int releaseTime  = 30;               // px

    /*--------------------------------------------------------------------
      3. Render either a Gate pulse (< 64) or Env-2 (> = 64)
    --------------------------------------------------------------------*/
    if (value < 64)                                // ---- Gate ----
    {
        constexpr int leadLowTime = 20;            // px
        constexpr int gateHighTime = 60;           // px

        const int gateStartX = rectX1 + leadLowTime;
        const int gateEndX   = gateStartX + gateHighTime;

        // Low → High → Low rectangle
        d->drawLine(rectX1,     rectY2, gateStartX, rectY2, WHITE); // leading low
        d->drawLine(gateStartX, rectY2, gateStartX, rectY1, WHITE); // rising edge
        d->drawLine(gateStartX, rectY1, gateEndX,   rectY1, WHITE); // high
        d->drawLine(gateEndX,   rectY1, gateEndX,   rectY2, WHITE); // falling edge
        d->drawLine(gateEndX,   rectY2, rectX2,     rectY2, WHITE); // trailing low

        centerText("Gate", 35);
    }
    else                                           // ---- Envelope 2 ----
    {
        const int attackX  = rectX1 + attackTime;
        const int decayX   = attackX + decayTime;
        const int sustainX = rectX2 - releaseTime;
        const int sustainY = rectY2 - (rectH * sustainLevel / 100); // 30 % up

        // A D S R plot (normal, not inverted)
        d->drawLine(rectX1, rectY2, attackX, rectY1, WHITE);        // Attack
        d->drawLine(attackX, rectY1, decayX,  sustainY, WHITE);     // Decay
        d->drawLine(decayX,  sustainY, sustainX, sustainY, WHITE);  // Sustain
        d->drawLine(sustainX, sustainY, rectX2, rectY2, WHITE);     // Release

        centerText("Envelope 2", 35);
    }

    /*--------------------------------------------------------------------
      4. Numeric read-out centred at the bottom
    --------------------------------------------------------------------*/
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%03u", value);
    centerText(buf, 48);
}
