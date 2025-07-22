#include "widgets/RunModeWidgetRange.h"
#include "IDisplay.h"
#include "defines.h"
#include <ScreenManager.h>
#include <cstdio>   // for sprintf / snprintf
#include "widgets/WidgetUtils.h"  


#define WHITE 1
#define BLACK 0 


void RunModeWidgetRange::draw(const knobConfig &cfg,
                              uint8_t  rawValue,
                              bool     blinkOn)
{
    IDisplay* d = ScreenManager::getDisplay();

    /* ---------- clear & outline the 120×48 region ---------- */
    d->fillRect(4, 10, 120, 48, BLACK);
    d->drawRect(4, 10, 120, 48, WHITE);

    /* ---------- caption ---------- */
    centerText(cfg.name, 15);  

    /* ---------- baseline & tick labels ---------- */
    d->drawLine(14, 30, 14+93, 30, WHITE);    // 100-pixel baseline

    if(cfg.cmdbyte == 0x00) { // unassigned knob
        return; // nothing more to do
    }

    uint8_t legend = 2;                     // will print 2', 4', 8', …
    char    lbl[8];                          // largest is “32'” → 5 bytes inc. NUL; increased to avoid truncation

    for (int x = 14; x <= 112; x += 31) {
        d->drawLine(x, 31, x, 32, WHITE);   // tick mark

        // build C-style label like "2'"
        std::snprintf(lbl, sizeof(lbl), "%u'", legend);
        legend <<= 1;                       // ×2 for next pass

        d->setCursor(x - 6, 38);
        d->print(lbl);                      // print C string
    }

    /* ---------- position marker ---------- */
    uint8_t dispVal = rawValue >> 1;                   
    uint16_t pos    = 93 * (127 - dispVal) / 127;        
    uint8_t  xPixel = 14 + pos;

    if (blinkOn)
        d->drawLine(xPixel, 29, xPixel, 27, WHITE);     // visible tick
    else
        d->drawLine(xPixel, 29, xPixel, 27, BLACK);     // erase for blink-off

    /* ---------- numeric read-out ---------- */
    char buf[8];
    std::sprintf(buf, "%03u", dispVal);                 // “042”, etc.
    centerText(buf, 48);  
}
