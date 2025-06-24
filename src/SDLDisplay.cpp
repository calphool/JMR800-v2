
#ifndef TARGET_TEENSY
#include "SDLDisplay.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include "SimulatedFontDefinitions.h"
#include "SDLDisplay.h"
#include <SDL2/SDL.h>


SDLDisplay::SDLDisplay(int screenWidth, int screenHeight, int pixelSize)
        : screenWidth(screenWidth), screenHeight(screenHeight), pixelSize(pixelSize) {}

bool SDLDisplay::initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("SDL_Init failed: %s\n", SDL_GetError());
            return false;
        }

        window = SDL_CreateWindow("SDL SH1106 Display", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   screenWidth * pixelSize, screenHeight * pixelSize, SDL_WINDOW_SHOWN);
        if (!window) {
            printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
            return false;
        }

        clearDisplay();
        return true;
}

SDLDisplay::~SDLDisplay() {
    shutdown();
}

void SDLDisplay::renderFrame() {
    display();
}

void SDLDisplay::shutdown() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    renderer = nullptr;
    window = nullptr;
    closed = false;
}

void SDLDisplay::begin() {
    clearDisplay();
    display();
}

void SDLDisplay::clearDisplay() {
    for (int y = 0; y < 64; ++y)
        for (int x = 0; x < 128; ++x)
            pixels[y][x] = false;
}

void SDLDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;
    pixels[y][x] = (color != 0);
}

void SDLDisplay::display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < screenHeight; ++y) {
        for (int x = 0; x < screenWidth; ++x) {
            if (pixels[y][x]) {
                SDL_Rect r = { x * pixelSize, y * pixelSize, pixelSize, pixelSize };
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void SDLDisplay::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_WINDOWEVENT && event.window.windowID == SDL_GetWindowID(window)) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
            closed = true;
        }
    }
}
void SDLDisplay::setCursor(int16_t x, int16_t y) { cursorX = x; cursorY = y; }
void SDLDisplay::setTextSize(uint8_t s) { textSize = s; }
void SDLDisplay::setTextColor(uint16_t c) { textColor = c; }
void SDLDisplay::setFont(FontSize fontSize) { currentFont = fontSize; }
void SDLDisplay::setFont() { currentFont = FontSize::Default; }



void SDLDisplay::print(const char* text) {
    int charWidth = (currentFont == FontSize::Small) ? 3 : 5;
    int spacing = 1;

    for (size_t i = 0; text[i]; ++i) {
        if (cursorX + charWidth >= 128) {
            cursorX = 0;
            cursorY += (currentFont == FontSize::Small) ? 6 : 8;
        }

        drawChar(cursorX, cursorY, text[i], textColor, 0);
        cursorX += charWidth + spacing;
    }
}

void SDLDisplay::print(char ch) {
    int charWidth = (currentFont == FontSize::Small) ? 3 : 5;
    int spacing = 1;

    if (cursorX + charWidth >= 128) {
        cursorX = 0;
        cursorY += (currentFont == FontSize::Small) ? 6 : 8;
    }

    drawChar(cursorX, cursorY, ch, textColor, 0);
    cursorX += charWidth + spacing;
}

void SDLDisplay::println(const char* text) {
    print(text);
    cursorX = 0;
    cursorY += (currentFont == FontSize::Small) ? 6 : 8;
}


void SDLDisplay::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (true) {
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void SDLDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    drawLine(x, y, x + w - 1, y, color);
    drawLine(x, y, x, y + h - 1, color);
    drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);
    drawLine(x, y + h - 1, x + w - 1, y + h - 1, color);
}

void SDLDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            drawPixel(x + j, y + i, color);
}

void SDLDisplay::setTextWrap(bool b) { textWrap = b; }


void SDLDisplay::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg) {
    if (c >= 128) return;  // Only support ASCII 0-127

    const uint8_t (*font)[5] = defaultFont;
    int width = 5;
    int height = 7;

    if (currentFont == FontSize::Small) {
        font = (const uint8_t (*)[5])smallFont;
        width = 3;
        height = 5;
    }

    for (int8_t i = 0; i < width; i++) {
        uint8_t line = font[c][i];
        for (int8_t j = 0; j < height; j++) {
            bool pixelOn = line & (1 << j);
            drawPixel(x + i, y + j, pixelOn ? color : bg);
        }
    }
}


void SDLDisplay::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int x = r, y = 0, err = 0;
    while (x >= y) {
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 - y, y0 - x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 + x, y0 - y, color);
        ++y;
        if (err <= 0) {
            err += 2*y + 1;
        } else {
            --x;
            err += 2*(y - x) + 1;
        }
    }
}


#endif