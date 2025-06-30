#pragma once

#ifndef TARGET_TEENSY

#include "IDisplay.h"  // Your abstract base
#include <SDL2/SDL.h>

class SDLDisplay : public IDisplay {
public:
    SDLDisplay(int screenWidth = 128, int screenHeight = 64, int pixelSize = 6);
    ~SDLDisplay();

    void begin() override;
    void clearDisplay() override;
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void display() override;
    void setCursor(int16_t x, int16_t y) override;
    void setTextSize(uint8_t s) override;
    void setTextColor(uint16_t c) override;
    void setFont(FontSize fontSize) override;
    void setFont() override;
    void print(const char *text) override;
    void print(const char ch) override;
    void println(const char *text) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void setTextWrap(bool b) override;
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t fg, uint16_t bg) override;
    void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;

    bool initialize();
    void handleEvent(const SDL_Event& event);
    void renderFrame();
    void shutdown();
    bool shouldClose() const { return closed; }
    void setWindowPosition(int x, int y);


private:
    int screenWidth;
    int screenHeight;
    int pixelSize;

    SDL_Texture* texture = nullptr;
    bool closed = false;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool pixels[64][128] = {{false}};  // [y][x]
    int cursorX = 0;
    int cursorY = 0;
    uint8_t textSize = 1;
    uint16_t textColor = 1;
    bool textWrap = true;
    FontSize currentFont = FontSize::Default;
};


#endif