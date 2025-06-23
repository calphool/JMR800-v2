// DisplayInterface.h
#pragma once
#include <cstdint>


enum class FontSize {
    Default,
    Small
};

class DisplayInterface {
public:
    virtual void begin() = 0;
    virtual void clearDisplay() = 0;
    virtual void display() = 0;
    virtual void setCursor(int16_t x, int16_t y) = 0;
    virtual void setTextSize(uint8_t s) = 0;
    virtual void setTextColor(uint16_t c) = 0;
    virtual void setFont(FontSize fontSize) = 0;
    virtual void setFont() = 0;
    virtual void print(const char *text) = 0;
    virtual void print(const char ch) = 0;
    virtual void println(const char *text) = 0;
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) = 0;
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
    virtual void setTextWrap(bool b) = 0;
    virtual void drawChar(int16_t x, int16_t y, char c, uint16_t fg, uint16_t bg) = 0;
    virtual void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) = 0;
 

    virtual ~DisplayInterface() {}
}; 
