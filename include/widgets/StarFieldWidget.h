
#pragma once

#include "widgets/Widget.h"

#define NUM_STARS 50


struct Star {
  float dx, dy;     // Direction vector from center
  float x, y;       // Current position
  float speedMult;  // Speed multiplier
};

class StarFieldWidget : public Widget {
private:
    int width;      ///< Rectangle width in pixels
    int height;     ///< Rectangle height in pixels

    Star stars[NUM_STARS];

    void resetStar(Star& s);


public:
    /**
     * @brief Constructs a rectangle widget with position, size, and style.
     * @param x Horizontal position (pixels)
     * @param y Vertical position (pixels)
     * @param width Width of the rectangle
     * @param height Height of the rectangle
     */
    StarFieldWidget(int x, int y, int width, int height);

    /**
     * @brief Draws the rectangle on the display.
     */
    void draw() override;

    /**
     * @brief No-op input handler for compatibility with Widget interface.
     */
    void handleInput() override;

    /**
     * @brief Updates the rectangle’s screen position.
     * @param xpos New X coordinate
     * @param ypos New Y coordinate
     */
    void setPosition(int xpos, int ypos);

    /**
     * @brief Identifies the widget as a Rectangle type.
     * @return WidgetType::Starfield
     */
    WidgetType getType() const override;
};
