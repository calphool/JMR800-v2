#include "widgets/RectangleWidget.h"
#include "ScreenManager.h"
#include "Logging.h"



/* ------------------------------------------------------------------------------------------------
   |  Constructor()                                                                               |
   ------------------------------------------------------------------------------------------------ */
RectangleWidget::RectangleWidget(int x, int y, int w, int h, bool f, RectColor c) :
  Widget(x, y, false), width(w), height(h), filled(f), color(c) {
  log(LOG_VERBOSE, "Inside RectangleWidget->constructor");
}


/* ------------------------------------------------------------------------------------------------
   |  draw() - draws this rectangle widget, and blinks its border if it is highlighted            |
   ------------------------------------------------------------------------------------------------ */
void RectangleWidget::draw() {
  log(LOG_VERBOSE, "Inside RectangleWidget->draw()");
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