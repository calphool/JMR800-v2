/**
 * @file Widget.cpp
 * @brief Implements the base Widget class for positioning and highlighting GUI elements.
 *
 * This class provides foundational properties and behaviors shared by all widgets,
 * such as position and highlight state.
 */

#include "widgets/Widget.h"


/**
 * @brief Constructs a Widget with specified position and highlight state.
 *
 * @param x Horizontal position on the screen.
 * @param y Vertical position on the screen.
 * @param highlighted Whether the widget is initially highlighted.
 */
Widget::Widget(int x, int y, bool highlighted)
    : x(x), y(y), bHighlighted(highlighted), toggle(false) {}


/**
 * @brief Constructs a Widget with specified position, not highlighted by default.
 *
 * @param x Horizontal position on the screen.
 * @param y Vertical position on the screen.
 */
Widget::Widget(int x, int y) : x(x), y(y), bHighlighted (false), toggle(false) {}


/**
 * @brief Sets the highlight status of the widget.
 *
 * @param b true to highlight the widget; false to remove highlighting.
 */
void Widget::setHighlighted(bool b) {
    bHighlighted = b;
}

bool Widget::getHighlightedStatus() {
    return bHighlighted;
}