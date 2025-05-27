#include "widgets/RectangleWidget.h"
#include "ScreenManager.h"
#include "Logging.h"

RectangleWidget::RectangleWidget(int x, int y, int w, int h, bool f, RectColor c) :
  Widget(x, y), width(w), height(h), filled(f), color(c) {
  log(LOG_VERBOSE, "Inside RectangleWidget->constructor");
}

void RectangleWidget::draw() {
  log(LOG_VERBOSE, "Inside RectangleWidget->draw()");
  if (filled) {
      ScreenManager::getDisplay()->fillRect(x, y, width, height, static_cast<uint16_t>(color));
  } else {
      ScreenManager::getDisplay()->drawRect(x, y, width, height, static_cast<uint16_t>(color));
  }
}

void RectangleWidget::setPosition(int xpos, int ypos) {
    x = xpos;
    y = ypos;
}

void RectangleWidget::setSize(int w, int h) {
    width = w;
    height = h;
}

void RectangleWidget::setFilled(bool f) {
    filled = f;
}

void RectangleWidget::setColor(RectColor c) {
    color = c;
}

void RectangleWidget::handleInput() {
}

WidgetType RectangleWidget::getType() const {
    return WidgetType::Rectangle;
}