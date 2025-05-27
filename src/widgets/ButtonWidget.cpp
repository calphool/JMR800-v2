#include "widgets/Widget.h"
#include "widgets/ButtonWidget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"

ButtonWidget::ButtonWidget(int _buttonId, int x, int y) : Widget(x, y), 
  pressed(false), red(false), green(false), buttonId(_buttonId) {
  log(LOG_VERBOSE, "Inside ButtonWidget->constructor");
}

void ButtonWidget::setPressed(bool p) {
    pressed = p;
}

void ButtonWidget::setLEDs(bool bRed, bool bGreen) {
    red = bRed;
    green = bGreen;
}


void ButtonWidget::draw() {
  ScreenManager::getDisplay()->fillRect(x+1, y, 8, 6, SH110X_WHITE);
  ScreenManager::getDisplay()->drawLine(x+2, y+2, x+7, y+2, SH110X_BLACK);
  
  if(pressed)
    ScreenManager::getDisplay()->fillRect(x+2, y+2, 6, 3, SH110X_BLACK);

  if(green) 
    ScreenManager::getDisplay()->drawLine(x+2,y+1, x+3, y+1, SH110X_BLACK);
  if(red)
    ScreenManager::getDisplay()->drawLine(x+6,y+1, x+7, y+1, SH110X_BLACK);
}

void ButtonWidget::handleInput() {
  if(buttonId < 0 || buttonId > NUM_BUTTONS) {
        char buf[128];
        sprintf(buf, "ButtonWidget->handleInput() invoked with an invalid buttonId: %d", buttonId);
        log(LOG_ERROR, buf);
        return;
  }

  this->setPressed(hardware.isButtonPressed(buttonId));
}

WidgetType ButtonWidget::getType() const {
    return WidgetType::Button;
}
