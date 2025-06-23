/**
 * @file RectangleWidget.h
 * @brief Displays a static rectangle, optionally filled, as part of the OLED UI.
 *
 * This widget provides basic visual structure and layout assistance for UI screens.
 * It can be used for borders, boxes, dividers, or emphasis frames.
 */

#pragma once

#include <cstdint>

#include "widgets/Widget.h"
#ifdef TARGET_TEENSY
#include <Adafruit_SH110X.h>
#endif

/**
 * @enum RectColor
 * @brief Defines rendering colors for rectangle drawing.
 */
enum class RectColor : uint8_t {
    BLACK = 0,
    WHITE = 1
};


/**
 * @class RectangleWidget
 * @brief Renders a static rectangle on the OLED screen.
 *
 * This widget can be filled or outlined and is useful for basic layout
 * decoration or structural grouping of other UI elements.
 */
class RectangleWidget : public Widget {
private:
    int width;      ///< Rectangle width in pixels
    int height;     ///< Rectangle height in pixels
    bool filled;    ///< If true, draws a filled rectangle
    RectColor color;

public:
    /**
     * @brief Constructs a rectangle widget with position, size, and style.
     * @param x Horizontal position (pixels)
     * @param y Vertical position (pixels)
     * @param width Width of the rectangle
     * @param height Height of the rectangle
     * @param filled True to draw filled rectangle; false for outline only
     * @param color Drawing color
     */
    RectangleWidget(int x, int y, int width, int height, bool filled, RectColor color);

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
    //void setPosition(int xpos, int ypos);

    /**
     * @brief Sets the rectangle’s size.
     * @param w New width
     * @param h New height
     */
    //void setSize(int w, int h);

    /**
     * @brief Changes whether the rectangle is filled.
     * @param f True for filled; false for outline only
     */
    //void setFilled(bool f);

    /**
     * @brief Changes the drawing color.
     * @param c New color (WHITE or BLACK)
     */
    //void setColor(RectColor c);

    /**
     * @brief Identifies the widget as a Rectangle type.
     * @return WidgetType::Rectangle
     */
    WidgetType getType() const override;
};
