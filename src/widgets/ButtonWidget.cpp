/**
 * @file ButtonWidget.cpp
 * @brief Implements the ButtonWidget class, representing a physical button with LED indicators.
 *
 * This widget visually reflects the state of a hardware button, including press state and LED color indicators (red/green).
 * It supports highlighting and handles its own polling and display.
 */

#include "widgets/Widget.h"
#include "widgets/ButtonWidget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"


/**
 * @brief Constructs a ButtonWidget tied to a specific hardware button ID.
 *
 * @param _buttonId Logical ID of the hardware button.
 * @param x X-coordinate of the widget.
 * @param y Y-coordinate of the widget.
 */
ButtonWidget::ButtonWidget(int _buttonId, int x, int y) : Widget(x, y, false), 
  pressed(false), red(false), green(false), buttonId(_buttonId) {
  log(LOG_VERBOSE, "Inside ButtonWidget->constructor");
}

/**
 * @brief Sets the visual pressed state of the button.
 *
 * @param p True if the button is pressed, false otherwise.
 */
void ButtonWidget::setPressed(bool p) {
    pressed = p;
}


/**
 * @brief Sets the red and green LED states for the button.
 *
 * @param bRed Whether the red LED is lit.
 * @param bGreen Whether the green LED is lit.
 */
void ButtonWidget::setLEDs(bool bRed, bool bGreen) {
    red = bRed;
    green = bGreen;
}


/**
 * @brief Draws the button widget, including LED indicators, press state, and optional highlight.
 */
void ButtonWidget::draw() {
  toggle = !toggle;
  ScreenManager::getDisplay()->fillRect(x+1, y, 8, 6, SH110X_WHITE);
  ScreenManager::getDisplay()->drawLine(x+2, y+2, x+7, y+2, SH110X_BLACK);
  
  if(pressed)
    ScreenManager::getDisplay()->fillRect(x+2, y+2, 6, 3, SH110X_BLACK);

  if(red) 
    ScreenManager::getDisplay()->drawLine(x+2,y+1, x+3, y+1, SH110X_BLACK);
  if(green)
    ScreenManager::getDisplay()->drawLine(x+6,y+1, x+7, y+1, SH110X_BLACK);

  if(bHighlighted) {
    if(toggle)
      ScreenManager::getDisplay()->drawRect(x+1, y, 8, 6, SH110X_WHITE);
    else
      ScreenManager::getDisplay()->drawRect(x+1, y, 8, 6, SH110X_BLACK);
  }
}


/**
 * @brief Interrogates the hardware class to determine status and updates its visual state accordingly.
 *
 * Also checks for valid button index range and logs an error if invalid.
 */
void ButtonWidget::handleInput() {
  if(buttonId < 0 || buttonId > NUM_BUTTONS) {
        char buf[128];
        sprintf(buf, "ButtonWidget->handleInput() invoked with an invalid buttonId: %d", buttonId);
        log(LOG_ERROR, buf);
        return;
  }

  this->setPressed(hardware.isButtonPressed(buttonId));
  this->setLEDs(hardware.redIsLit(buttonId), hardware.greenIsLit(buttonId));
}


/**
 * @brief Returns the widget type identifier.
 *
 * @return WidgetType::Button
 */
WidgetType ButtonWidget::getType() const {
    return WidgetType::Button;
}
