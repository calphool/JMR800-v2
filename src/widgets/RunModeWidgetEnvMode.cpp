#include "widgets/RunModeWidgetEnvMode.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>
#include "widgets/WidgetUtils.h"

#define WHITE 1
#define BLACK 0 

void RunModeWidgetEnvMode::draw(const knobConfig &cfg,
                                uint8_t           value,
                                bool              blinkOn)
{
    /*--------------------------------------------------------------------
      1. Normalise knob value (0–255 ➜ 0–127)
    --------------------------------------------------------------------*/
    value >>= 1;

    /*--------------------------------------------------------------------
      2. Clear widget area and draw static frame/title/baseline
    --------------------------------------------------------------------*/
    IDisplay *d = ScreenManager::getDisplay();

    d->fillRect(4, 10, 120, 48, BLACK);
    d->drawRect(4, 10, 120, 48, WHITE);

    centerText(cfg.name, 15);

    /*--------------------------------------------------------------------
      3. Envelope geometry (common constants)
    --------------------------------------------------------------------*/
    constexpr int rectX1 = 14;
    constexpr int rectY1 = 23;
    constexpr int rectX2 = 114;
    constexpr int rectY2 = 37;
    constexpr int rectH  = rectY2 - rectY1;     // 20 px

    constexpr int attackTime   = 20;            // px
    constexpr int decayTime    = 20;            // px
    constexpr int sustainLevel = 30;            // % of height
    constexpr int releaseTime  = 30;            // px

    const int attackX  = rectX1 + attackTime;
    const int decayX   = attackX + decayTime;
    const int sustainX = rectX2 - releaseTime;

    /*--------------------------------------------------------------------
      4. Select envelope variant based on knob value
         0–31   → Env-2 Inverted
         32–63  → Env-2 Normal
         64–95  → Env-1 Inverted
         96–127 → Env-1 Normal
    --------------------------------------------------------------------*/
    const char *label = nullptr;

    if (value < 32)                                                    // Env-2 Inverted
    {
        const int sustainY = rectY1 + (rectH * sustainLevel / 100);    // lower sustain

        d->drawLine(rectX1, rectY1, attackX, rectY2, WHITE);           // Attack (down)
        d->drawLine(attackX, rectY2, decayX, sustainY, WHITE);         // Decay   (up)
        d->drawLine(decayX, sustainY, sustainX, sustainY, WHITE);      // Sustain (flat)
        d->drawLine(sustainX, sustainY, rectX2, rectY1, WHITE);        // Release (up)

        label = "Envelope 2 Inverted";
    }
    else if (value < 64)                                               // Env-2 Normal
    {
        const int sustainY = rectY2 - (rectH * sustainLevel / 100);    // upper sustain

        d->drawLine(rectX1, rectY2, attackX, rectY1, WHITE);           // Attack (up)
        d->drawLine(attackX, rectY1, decayX, sustainY, WHITE);         // Decay  (down)
        d->drawLine(decayX, sustainY, sustainX, sustainY, WHITE);      // Sustain
        d->drawLine(sustainX, sustainY, rectX2, rectY2, WHITE);        // Release (down)

        label = "Envelope 2 Normal";
    }
    else if (value < 96)                                               // Env-1 Inverted
    {
        const int sustainY = rectY1 + (rectH * sustainLevel / 100);    // lower sustain

        d->drawLine(rectX1, rectY1, attackX, rectY2, WHITE);
        d->drawLine(attackX, rectY2, decayX, sustainY, WHITE);
        d->drawLine(decayX, sustainY, sustainX, sustainY, WHITE);
        d->drawLine(sustainX, sustainY, rectX2, rectY1, WHITE);

        label = "Envelope 1 Inverted";
    }
    else                                                               // Env-1 Normal
    {
        const int sustainY = rectY2 - (rectH * sustainLevel / 100);    // upper sustain

        d->drawLine(rectX1, rectY2, attackX, rectY1, WHITE);
        d->drawLine(attackX, rectY1, decayX, sustainY, WHITE);
        d->drawLine(decayX, sustainY, sustainX, sustainY, WHITE);
        d->drawLine(sustainX, sustainY, rectX2, rectY2, WHITE);

        label = "Envelope 1 Normal";
    }

    /*--------------------------------------------------------------------
      5. Draw the mode label (centred at Y = 40)
    --------------------------------------------------------------------*/
    if (label) centerText(label, 40);

    /*--------------------------------------------------------------------
      6. Numeric read-out (0-127) centred at bottom
    --------------------------------------------------------------------*/
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%03u", value);
    centerText(buf, 48);
}
