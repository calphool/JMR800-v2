#pragma once

#ifdef TARGET_TEENSY

#include "IDisplay.h"
#include <Adafruit_SH110X.h>

class SH110XDisplay : public IDisplay {
public:
    SH110XDisplay();

    void begin() override;
    void clearDisplay() override;
    void display() override;
    void setCursor(int16_t x, int16_t y) override;
    void setTextSize(uint8_t s) override;
    void setTextColor(uint16_t c) override;
    void setFont(FontSize fontSize) override;
    void setFont() override;
    void print(const char *text) override;
    void print(const char ch) override;
    void println(const char *text) override;
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void setTextWrap(bool b) override;
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t fg, uint16_t bg) override;
    void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;
 

private:
    Adafruit_SH1106G disp;
};



#endif