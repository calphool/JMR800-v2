#ifdef TARGET_TEENSY


#include "SH110XDisplay.h"
#include "Fonts/TomThumb.h"
#include "defines.h"

SH110XDisplay::SH110XDisplay() : disp(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

void SH110XDisplay::begin() {
    disp.begin(OLED_I2C_ADDRESS, false);
}

void SH110XDisplay::clearDisplay() {
    disp.clearDisplay();
}

void SH110XDisplay::display() {
    disp.display();
}

void SH110XDisplay::setCursor(int16_t x, int16_t y) {
    disp.setCursor(x, y);
}

void SH110XDisplay::setTextSize(uint8_t s) {
    disp.setTextSize(s);
}

void SH110XDisplay::setTextColor(uint16_t c) {
    disp.setTextColor(c);
}

void SH110XDisplay::setFont(FontSize fontSize) {
    if (fontSize == FontSize::Small) {
        disp.setFont(&TomThumb);
    } else {
        disp.setFont();  // default font
    }
}

void SH110XDisplay::setFont() {
    disp.setFont();
}


void SH110XDisplay::print(const char *text) {
    disp.print(text);
}

void SH110XDisplay::print(const char ch) {
    disp.print(ch);
}

void SH110XDisplay::println(const char *text) {
    disp.println(text);
}

void SH110XDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    disp.drawPixel(x, y, color);
}

void SH110XDisplay::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    disp.drawLine(x0, y0, x1, y1, color);
}

void SH110XDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    disp.drawRect(x, y, w, h, color);
}

void SH110XDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    disp.fillRect(x, y, w, h, color);
}

void SH110XDisplay::setTextWrap(bool b) {
    disp.setTextWrap(b);
}

void SH110XDisplay::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t fg, uint16_t bg)  {
    disp.drawChar(x, y, c, fg, bg, 1);
}

void SH110XDisplay::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    disp.drawCircle(x, y, r, color);
}



#endif