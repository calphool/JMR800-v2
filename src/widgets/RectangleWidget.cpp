/**
 * @file RectangleWidget.cpp
 * @brief Implements the RectangleWidget class for drawing rectangular shapes with optional fill and highlight behavior.
 *
 * This widget is used to render filled or outlined rectangles, with optional blinking borders when highlighted.
 */

#include "widgets/RectangleWidget.h"
#include "ScreenManager.h"
#include "Logging.h"



/**
 * @brief Constructs a RectangleWidget with position, size, fill mode, and color.
 *
 * @param x X-coordinate of the rectangle's top-left corner.
 * @param y Y-coordinate of the rectangle's top-left corner.
 * @param w Width of the rectangle in pixels.
 * @param h Height of the rectangle in pixels.
 * @param f Whether the rectangle should be filled (true) or outlined (false).
 * @param c Color of the rectangle (from the RectColor enum).
 */
RectangleWidget::RectangleWidget(int x, int y, int w, int h, bool f, RectColor c) :
  Widget(x, y, false), width(w), height(h), filled(f), color(c) {
  log(LOG_VERBOSE, "Inside RectangleWidget->constructor", __func__);
}


/**
 * @brief Renders the rectangle on the screen.
 *
 * Draws either a filled or outlined rectangle in the specified color. If the widget is highlighted,
 * an additional animated border is toggled to indicate focus.
 */
void RectangleWidget::draw() {
  log(LOG_VERBOSE, "Inside RectangleWidget->draw()", __func__);
  toggle = !toggle;
  
  if (filled) {
      ScreenManager::getDisplay()->fillRect(x, y, width, height, static_cast<uint16_t>(color));
  } else {
      ScreenManager::getDisplay()->drawRect(x, y, width, height, static_cast<uint16_t>(color));
  }

  if(bHighlighted) {
    if(toggle) {
      ScreenManager::getDisplay()->drawRect(x+1,y+1,width-1,height-1, (uint16_t) 0);
    }
    else {
      ScreenManager::getDisplay()->drawRect(x+1,y+1,width-1,height-1, (uint16_t) 1);
    }
  }
}


/**
 * @brief Updates the rectangle's screen position.
 *
 * @param xpos New x-coordinate.
 * @param ypos New y-coordinate.
 */
void RectangleWidget::setPosition(int xpos, int ypos) {
    x = xpos;
    y = ypos;
}


/**
 * @brief Sets the width and height of the rectangle.
 *
 * @param w New width in pixels.
 * @param h New height in pixels.
 */
void RectangleWidget::setSize(int w, int h) {
    width = w;
    height = h;
}


/**
 * @brief Sets whether the rectangle should be filled or outlined.
 *
 * @param f True for filled rectangle, false for outline only.
 */
void RectangleWidget::setFilled(bool f) {
    filled = f;
}


/**
 * @brief Sets the rectangle's color.
 *
 * @param c Color from the RectColor enum.
 */
void RectangleWidget::setColor(RectColor c) {
    color = c;
}


/**
 * @brief Handles input. RectangleWidget has no input behavior.
 */
void RectangleWidget::handleInput() {
}


/**
 * @brief Returns the widget type as `WidgetType::Rectangle`.
 * @return WidgetType Enumeration value representing a Rectangle
 */
WidgetType RectangleWidget::getType() const {
    return WidgetType::Rectangle;
}