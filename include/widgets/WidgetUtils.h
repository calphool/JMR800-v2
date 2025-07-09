// WidgetUtils.h -------------------------------------------------------------
#pragma once
#include "ScreenManager.h"   // for IDisplay*

/**
 * Centre-justifies an ASCII string on the SH1106 display.
 *
 * @param str  Null-terminated C string to be rendered.
 * @param y    Baseline-top coordinate (same unit you already pass to setCursor).
 *
 * The routine assumes a fixed 5-pixel character width (as used by Adafruit_GFX
 * default 5 × 7 font) and the standard 128-pixel screen width.
 */
inline void centerText(const char* str, uint8_t y)
{
    if (!str) return;                        // defensive guard

    /*------------------------------------------------------------
      1. Determine printable length, ignoring trailing whitespace
    ------------------------------------------------------------*/
    const char* end = str;
    while (*end) ++end;                      // find '\0'

    const char* last = end;
    while (last != str && (*(last - 1) == ' ' || *(last - 1) == '\t'))
        --last;                              // step back over right-side blanks

    const std::size_t len = static_cast<std::size_t>(last - str);
    if (len == 0) return;                    // nothing visible to draw

    /*------------------------------------------------------------
      2. Calculate left-hand X coordinate for centring
    ------------------------------------------------------------*/
    constexpr uint8_t kScreenWidth = 128;
    constexpr uint8_t kCharWidth   = 6;      // 6 px per glyph, default font
    const uint8_t textWidth = static_cast<uint8_t>(len * kCharWidth);

    // Clamp to 0 if text is wider than the screen (unlikely)
    const uint8_t x = (textWidth < kScreenWidth)
                      ? static_cast<uint8_t>((kScreenWidth - textWidth) / 2)
                      : 0;

    /*------------------------------------------------------------
      3. Render
    ------------------------------------------------------------*/
    IDisplay* d = ScreenManager::getDisplay();
    d->setCursor(x, y);
    d->print(str);
}
