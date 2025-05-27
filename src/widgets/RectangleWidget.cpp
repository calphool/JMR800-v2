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
  log(LOG_VERBOSE, "Inside RectangleWidget->setPosition()");

    x = xpos;
    y = ypos;
}

void RectangleWidget::setSize(int w, int h) {
  log(LOG_VERBOSE, "Inside RectangleWidget->setSize()");
    width = w;
    height = h;
}

void RectangleWidget::setFilled(bool f) {
  log(LOG_VERBOSE, "Inside RectangleWidget->setFilled()");

    filled = f;
}

void RectangleWidget::setColor(RectColor c) {
    log(LOG_VERBOSE, "Inside RectangleWidget->setColor()");

    color = c;
}

void RectangleWidget::handleInput() {
  log(LOG_VERBOSE, "Inside RectangleWidget->handleInput()");
}